/***************************************************************************
    revision             : $Id: dial.c,v 1.1.1.1 2003-01-06 15:57:59 tellini Exp $
	copyright            : (C) 1999-2003 by Simone Tellini
	email                : tellini@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/// Include
#ifndef LATTICE // for ODU
#define LATTICE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <exec/types.h>                 // exec
#include <exec/memory.h>
#include <exec/execbase.h>
#include <intuition/intuition.h>        // intuition
#include <dos/dos.h>                    // dos
#include <libraries/mui.h>              // libraries
#include <libraries/locale.h>
#include <libraries/asl.h>
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/locale_protos.h>
#include <clib/genesislogger_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/locale_pragmas.h>
#include <pragmas/genesislogger_pragmas.h>

#ifdef CMANAGER_MCC
#include <proto/utility.h>
#endif

#include <devices/serial.h>             //  devices
#include <devices/timer.h>
#include <libraries/dos.h>              //  libraries
#include <libraries/OwnDevUnit.h>

#include <mymacros.h>

#define CATCOMP_NUMBERS

#include "/CManager.h"
#include "dial.h"
#ifndef CMANAGER_MCC
#include "MUI.h"
#else
#include "mui.h"
#include "CManager_mcc.h"
#endif
#include "/locale.h"

#include "/Library/CManager_protos.h"
#include "/Library/CManager_lib_lib.h"
///
/// Prototypes
static ULONG    DialWindow_New( struct IClass *, Object *, struct opSet * );
static ULONG    DialWindow_Open( struct IClass *, Object *, struct MUIP_DialWindow_Open * );
static ULONG    DialWindow_Close( struct IClass *, Object *, Msg );

static BOOL     OpenSerial( struct DialWindowData *, STRPTR, ULONG, ULONG, BOOL );
static void     CloseSerial( struct DialWindowData * );

static LONG     SerialPRead( struct DialWindowData *, UBYTE *, ULONG );
static LONG     InternalRead( struct DialWindowData *, UBYTE *, ULONG );
static LONG     SerialTRead( struct DialWindowData *, UBYTE *, ULONG, ULONG );
static LONG     SerialGets( struct DialWindowData *, UBYTE *, ULONG );
static LONG     SerialTGets( struct DialWindowData *, UBYTE *, ULONG, ULONG );

static BOOL     SerialWrite( struct DialWindowData *, UBYTE *, ULONG );
static BOOL     SerialPrintf( struct DialWindowData *, STRPTR, ... );
static BOOL     SerialReadLn( struct DialWindowData *, UBYTE *, ULONG );

static void     FlushMem( void );
static void     SetTimeOut( struct DialWindowData *, ULONG );

static void     SerialFlush( struct DialWindowData * );
static void     SerialAbort( struct DialWindowData * );

static BOOL     SetupSerAttrs( struct DialWindowData *, ULONG, BOOL );

extern struct ExecBase *SysBase;
///
/// Data
#define MUIV_SerialAbort    (TAGBASE + 1)

struct MUI_CustomClass     *CL_DialWindow;

#ifdef CMANAGER_MCC
extern struct Library      *GenesisLoggerBase;
#else
struct Library             *GenesisLoggerBase;
#endif

TEXT    SerDevice[256] = "serial.device";
UBYTE   SerUnit = 0;
BOOL    SerShared = FALSE;
TEXT    SerInit[256] = "AT&F";
TEXT    DialCommand[30] = "ATDT";
UWORD   RedialDelay = 30;
UWORD   RedialAttempts;
BOOL    LogCalls;
///


/// SerialPRead
static LONG SerialPRead( struct DialWindowData *data, UBYTE *buf, ULONG bufsize )
{
    if( bufsize ) {

        data->ior->IOSer.io_Command = SDCMD_QUERY;

        DoIO(( struct IORequest *)data->ior );

        if( data->ior->IOSer.io_Actual > 0 ) {

            if( data->ior->IOSer.io_Actual < bufsize )
                bufsize = data->ior->IOSer.io_Actual;

            data->ior->IOSer.io_Command = CMD_READ;
            data->ior->IOSer.io_Data    = buf;
            data->ior->IOSer.io_Length  = bufsize;

            if( DoIO(( struct IORequest * )data->ior ))
                return( -1 );

            return(( LONG )data->ior->IOSer.io_Actual );
        }
    }

    return( 0 );
}
///
/// InternalRead
/*
 * Wait for the timeout, or data being available.
 * If timeout, return 0.
 * If data available, grab all of it (up to bufsize) and return amount.
 */
static LONG InternalRead( struct DialWindowData *data, UBYTE *buf, ULONG bufsize )
{
    LONG    bytes;
    ULONG   sigmask = 0, signals, ser_sig, time_sig;

    if( bytes = SerialPRead( data, buf, bufsize ))
        return( bytes );

    /* No data. Wait for something to happen. */
    data->ior->IOSer.io_Command = CMD_READ;
    data->ior->IOSer.io_Data    = buf;
    data->ior->IOSer.io_Length  = 1;


    ser_sig  = 1L << data->Port->mp_SigBit;
    time_sig = 1L << data->TimeoutPort->mp_SigBit;


    /*  Clear the serial signal     */
    SetSignal( 0L, ser_sig );

    SendIO(( struct IORequest *)data->ior );

    signals = ser_sig | time_sig | SIGBREAKF_CTRL_C;

    /* Wait for data or timeout. */
    while( DoMethod( data->AppObj, MUIM_Application_Input, &sigmask ) != MUIV_SerialAbort ) {

        if(!( sigmask ))
            continue;

        sigmask = Wait( signals | sigmask );

        if( sigmask & SIGBREAKF_CTRL_C )
            break;

        /* Was it data? */
        if( CheckIO(( struct IORequest * )data->ior )) {

            WaitIO(( struct IORequest * )data->ior );

            bytes = data->ior->IOSer.io_Actual;

            if(( bytes ) && ( --bufsize ))
                bytes += SerialPRead( data, buf + 1, bufsize );

            return( bytes );
        }

        /*  timeout?    */
        if( CheckIO(( struct IORequest * )data->TimeoutIO ))
            break;
    }

    /* Nope - a timeout */
    AbortIO(( struct IORequest * )data->ior );
    WaitIO(( struct IORequest * )data->ior );

    return( 0 );
}
///
/// SerialTRead
static LONG SerialTRead( struct DialWindowData *data, UBYTE *buf, ULONG bufsize, ULONG timeout )
{
    LONG    bytes;

    if( timeout == 0 )
        return( SerialPRead( data, buf, bufsize ));

    SetTimeOut( data, timeout );

    bytes = InternalRead( data, buf, bufsize );

    SetTimeOut( data, 0 );

    return( bytes );
}
///
/// SerialGets
static LONG SerialGets( struct DialWindowData *data, UBYTE *buf, ULONG bufsize )
{
    UBYTE  *bufp = buf;
    int     i;

    for( i = 0; i < bufsize; i++ ) {
        char c;

        switch( InternalRead( data, &c, 1 )) {

            case 0:
                return( 0 );

            case 1:
                if( c != '\n' ) {
                    *bufp++ = c;
                    if( c == '\r' )
                        return( i + 1 );
                }
                break;

            default:
                return( -1 );
        }
    }

    return( i );
}
///
/// SerialTGets
static LONG SerialTGets( struct DialWindowData *data, UBYTE *buf, ULONG bufsize, ULONG timeout )
{
    int bytes;

    SetTimeOut( data, timeout );

    bytes = SerialGets( data, buf, bufsize );

    SetTimeOut( data, 0 );

    return( bytes );
}
///

/// SetTimeOut
static void SetTimeOut( struct DialWindowData *data, ULONG Sec )
{
    if( Sec != 0 ) {

        data->TimeoutIO->tr_node.io_Command = TR_ADDREQUEST;
        data->TimeoutIO->tr_time.tv_secs    = Sec;
        data->TimeoutIO->tr_time.tv_micro   = 0;

        SendIO(( struct IORequest * )data->TimeoutIO );

    } else if( data->TimeoutIO ) {

        AbortIO(( struct IORequest * )data->TimeoutIO );
        WaitIO(( struct IORequest * )data->TimeoutIO );
    }
}
///

/// OpenSerial
static BOOL OpenSerial( struct DialWindowData *data, STRPTR Device, ULONG Unit, ULONG Baud, BOOL Shared )
{
    if( OwnDevUnitBase ) {

        if( AttemptDevUnit( Device, Unit, "CManager", 0 )) {

            if(!( Shared ))
                return( FALSE );
        }
    }

    if(!( data->Port = CreateMsgPort() ))
        return( FALSE );

    if(!( data->iow = ( struct IOExtSer * )CreateIORequest( data->Port, sizeof( struct IOExtSer ))))
        return( FALSE );

    data->iow->IOSer.io_Flags = IOF_QUICK;

    if(!( data->ior = ( struct IOExtSer * )CreateIORequest( data->Port, sizeof( struct IOExtSer ))))
        return( FALSE );

    data->iow->io_SerFlags = SERF_7WIRE;

    if( Shared )
        data->iow->io_SerFlags |= SERF_SHARED;

    if( OpenDevice( Device, Unit, ( struct IORequest * )data->iow, 0L )) {

        FlushMem();

        if( OpenDevice( Device, Unit, ( struct IORequest * )data->iow, 0L ))
            return( FALSE );
    }

    if(!( SetupSerAttrs( data, Baud, Shared )))
        return( FALSE );

    if(!( data->TimeoutPort = CreateMsgPort() ))
        return( FALSE );

    if(!( data->TimeoutIO = CreateIORequest( data->TimeoutPort, sizeof( struct timerequest ))))
        return( FALSE );

    if( OpenDevice( "timer.device", UNIT_VBLANK, (struct IORequest *)data->TimeoutIO, 0 ))
        return( FALSE );

    return( TRUE );
}
///
/// CloseSerial
static void CloseSerial( struct DialWindowData *data )
{
    if( data->TimeoutIO ) {

        if(( data->TimeoutIO->tr_node.io_Device != 0 ) && ((long)data->TimeoutIO->tr_node.io_Device != -1 )) {

            if(!( CheckIO(( struct IORequest * )data->TimeoutIO ))) {
                AbortIO(( struct IORequest * )data->TimeoutIO );
                WaitIO(( struct IORequest * )data->TimeoutIO );
            }

            CloseDevice(( struct IORequest * )data->TimeoutIO );
        }

        DeleteIORequest(( struct IORequest * )data->TimeoutIO );

        data->TimeoutIO = NULL;
    }

    if( data->iow ) {
        struct Device  *dev;

        if(( data->iow->IOSer.io_Device != 0 ) && (( long )data->iow->IOSer.io_Device != -1 )) {

            if( data->ior ) {
                if( data->ior->IOSer.io_Message.mn_Node.ln_Type != NT_MESSAGE )
                    if(!( CheckIO(( struct IORequest * )data->ior ))) {
                        AbortIO(( struct IORequest * )data->ior );
                        WaitIO(( struct IORequest * )data->ior );
                    }
            }

            if( data->iow->IOSer.io_Message.mn_Node.ln_Type != NT_MESSAGE )
                if(!( CheckIO(( struct IORequest * )data->iow ))) {
                    AbortIO(( struct IORequest * )data->iow );
                    WaitIO(( struct IORequest * )data->iow );
                }

            CloseDevice(( struct IORequest *)data->iow );
        }

        DeleteIORequest(( struct IORequest *)data->iow );
        data->iow = NULL;

        Forbid();   /*  Remove the device from memory   */

        if( dev = (struct Device *)FindName( &SysBase->DeviceList, SerDevice ))
            RemDevice( dev );

        Permit();
    }

    if( data->ior ) {
        DeleteIORequest(( struct IORequest * )data->ior );
        data->ior = NULL;
    }

    if( data->Port ) {
        DeleteMsgPort( data->Port );
        data->Port = NULL;
    }

    if( OwnDevUnitBase )
        FreeDevUnit( SerDevice, SerUnit );
}
///

/// SerialWrite
static BOOL SerialWrite( struct DialWindowData *data, UBYTE *Buffer, ULONG BufLen )
{
    if( WaitIO(( struct IORequest *)data->iow )) {
        data->iow->IOSer.io_Flags = IOF_QUICK;
        return( FALSE );
    }

    data->iow->IOSer.io_Command = CMD_WRITE;
    data->iow->IOSer.io_Data    = Buffer;
    data->iow->IOSer.io_Length  = BufLen;

    DoIO(( struct IORequest *)data->iow );

    return( data->iow->IOSer.io_Error ? FALSE : TRUE );
}
///
/// SerialPrintf
BOOL SerialPrintf( struct DialWindowData *data, STRPTR Format, ... )
{
    va_list ap;
    UBYTE   buf[ 1024 ];

    va_start( ap, Format );
    vsprintf( buf, Format, ap );
    va_end( ap );

    strcpy( data->LastCmd, buf );

    return( SerialWrite( data, buf, strlen( buf )));
}
///

/// SerialReadLn
static BOOL SerialReadLn( struct DialWindowData *data, UBYTE *Dest, ULONG Time )
{
    BOOL    ret = FALSE, Something = FALSE;
    TEXT    Line[128];
    UBYTE  *Buffer;

    Line[0] = '\0';

    SetTimeOut( data, Time );

    Buffer = Line;

    for(;;) {
        LONG    n;

        if(( n = InternalRead( data, Buffer, 1 )) < 0 )
#ifdef DEBUG
        {
            Printf( "InternalRead() failed\n" );
#endif
            break;
#ifdef DEBUG
        }
#endif
        else if( n == 0 ) {     // Timeout
            Line[0] = '\0';
            break;
        }

        if(( *Buffer == '\r' ) || ( *Buffer == '\n' )) {

            if( Something ) {                           // Skip initial <CR><LF>

                *Buffer = '\0';

#ifdef DEBUG
                Printf( "SerialReadLn() - got %s\n", Line );
#endif

                if( strncmp( Line, data->LastCmd, strlen( Line )) != 0 ) {    // not an echo?
                    ret = TRUE;
                    break;
                } else {
                    Buffer    = Line;
                    Something = FALSE;
                }
            }

        } else {

            Something = TRUE;

            if( ++Buffer > &Line[ sizeof( Line ) - 1 ])
                Buffer = Line;
        }

    }

    SetTimeOut( data, 0 );

    strcpy( Dest, Line );

#ifdef DEBUG
    Printf( "SerialReadLn() - final: ret = %ld, line = %s\n", ret, Dest );
#endif

    return( ret );
}
///

/// FlushMem
static void FlushMem( void )
{
    APTR    mem;

    if( mem = AllocMem(( ULONG )-1, 0L )) {
        FlushMem();
        FreeMem( mem, (ULONG)-1 );
    }
}
///

/// SerialFlush
static void SerialFlush( struct DialWindowData *data )
{
    if(!( CheckIO(( struct IORequest * )data->ior ))) {

        AbortIO(( struct IORequest * )data->ior );
        WaitIO(( struct IORequest * )data->ior );
    }

    data->ior->IOSer.io_Command = CMD_CLEAR;

    DoIO(( struct IORequest * )data->ior );
}
///
/// SerialAbort
static void SerialAbort( struct DialWindowData *data )
{
    if(!( CheckIO(( struct IORequest * )data->iow ))) {

        AbortIO(( struct IORequest * )data->iow );
        WaitIO(( struct IORequest * )data->iow );
    }

    if(!( CheckIO(( struct IORequest * )data->ior ))) {

        AbortIO(( struct IORequest * )data->ior );
        WaitIO(( struct IORequest * )data->ior );
    }
}
///

/// SetupSerAttrs
static BOOL SetupSerAttrs( struct DialWindowData *data, ULONG Baud, BOOL Shared )
{
    SerialAbort( data );

    data->iow->IOSer.io_Command = SDCMD_SETPARAMS;
    data->iow->io_ExtFlags      = 0;
    data->iow->io_ReadLen       = 8;
    data->iow->io_WriteLen      = 8;
    data->iow->io_StopBits      = 1;
    data->iow->io_Baud          = Baud;
    data->iow->io_SerFlags      = SERF_XDISABLED | SERF_7WIRE;

    if( Shared )
        data->iow->io_SerFlags |= SERF_SHARED;

    DoIO(( struct IORequest * )data->iow );

    data->iow->IOSer.io_Flags = IOF_QUICK;

    *data->ior = *data->iow;

    return(( data->iow->IOSer.io_Error ) ? FALSE : TRUE );
}
///


/// DialWindow_Dispatcher
ULONG SAVEDS ASM DialWindow_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( DialWindow_New( cl, obj, (APTR)msg ));

        case MUIM_DialWindow_Open:
            return( DialWindow_Open( cl, obj, (APTR)msg ));

        case MUIM_DialWindow_Close:
            return( DialWindow_Close( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// DialWindow_New
static ULONG DialWindow_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct DialWindowData   temp = { 0 };
    Object                 *Abort;

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            MUIA_Window_Title,      CM_GetString( GAD_TO_PHONE ),
                            MUIA_Window_ID,         MAKE_ID( 'D', 'I', 'A', 'L' ),
                            MUIA_Window_LeftEdge,   MUIV_Window_LeftEdge_Centered,
                            MUIA_Window_TopEdge,    MUIV_Window_TopEdge_Centered,
                            MUIA_Window_NoMenus,    TRUE,
#ifndef CMANAGER_MCC
                            PubScreen[0] ? MUIA_Window_PublicScreen : TAG_IGNORE, PubScreen,
#endif

                            WindowContents, VGroup,

                                Child, temp.Text = TextObject,
                                    TextFrame,
                                    InnerSpacing( 8, 8 ),
                                    MUIA_Background,        MUII_TextBack,
                                    MUIA_Text_PreParse,     "\033c",
                                End,

                                Child, HGroup,
                                    Child, HSpace( 0 ),
                                    Child, Abort = MakeButton( GAD_CANCEL, -1 ),
                                    Child, HSpace( 0 ),
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct DialWindowData  *data = INST_DATA( cl, obj );

        *data = temp;

#ifdef CMANAGER_MCC
        data->CMObj = (Object *) GetTagData( MUIA_DialWindow_CMObj, NULL, msg->ops_AttrList );
#endif

        DoMethod( obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 1, MUIM_DialWindow_Close );

        /*  Gadgets */
        DoMethod( Abort, MUIM_Notify, MUIA_Pressed, FALSE, obj, 1, MUIM_DialWindow_Close );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// DialWindow_Open
static ULONG DialWindow_Open( struct IClass *cl, Object *obj, struct MUIP_DialWindow_Open *msg )
{
    struct DialWindowData  *data = INST_DATA( cl, obj );
    TEXT                    buffer[1024];
    BOOL                    go;

    data->MainWnd = msg->MainWnd;
    data->AppObj  = _app( obj );

    set( data->MainWnd, MUIA_Window_Sleep, TRUE );

    sprintf( buffer, CM_GetString( MSG_DIALLING ), msg->Number, 1, RedialAttempts );

    set( data->Text, MUIA_Text_Contents, buffer );

    set( obj, MUIA_Window_Open, TRUE );

    data->Aborted = FALSE;

    do {

        go = FALSE;

        if( OpenSerial( data, SerDevice, SerUnit, 9600, SerShared )) {
            BOOL    loop;
            UWORD   attempt = 0;

            SerialPrintf( data, "\r" );
            Delay( 35 );
            SerialFlush( data );

            SerialPrintf( data, "ATZ\r" );

            if(!( SerialReadLn( data, buffer, 10 ))) {

                if( go = MUI_Request( _app( obj ), data->MainWnd, 0, NULL,
                                      CM_GetString( ANS_RETRY_CANCEL ),
                                      CM_GetString( ERR_MODEM_NOT_RESPONDING )))
                    CloseSerial( data );

            } else {
                UBYTE  *ptr;

                Delay( 15 );

                if( ptr = strchr( SerInit, '\\' ))
                    if( ptr[1] == 'r' )
                        *ptr = '\0';

                SerialPrintf( data, "%s\r", SerInit );
                SerialReadLn( data, buffer, 10 );

                Delay( 15 );

                do {
                    APTR    loghandle = NULL;

                    loop = FALSE;

                    ++attempt;

                    sprintf( buffer, CM_GetString( MSG_DIALLING ), msg->Number, attempt, RedialAttempts );

                    set( data->Text, MUIA_Text_Contents, buffer );

                    SerialFlush( data );
                    SerialPrintf( data, "%s%s\r", DialCommand, msg->Number );

#ifdef CMANAGER_MCC
                    if( LogCalls && GenesisLoggerBase )
#else
                    if( LogCalls )
                        if( GenesisLoggerBase = OpenLibrary( "AmiTCP:libs/genesislogger.library", 0 ))
#endif
                            GL_StartLogger( msg->Number );

                    do {

                        SerialReadLn( data, buffer, 65000 );

                        if( data->Aborted )
                            break;

                        if(( strncmp( buffer, "NO ANSWER", 9 ) == 0 ) ||
                           ( strncmp( buffer, "8", 1 ) == 0 ) ||
                           ( strncmp( buffer, "BUSY", 4 ) == 0 ) ||
                           ( strncmp( buffer, "7", 1 ) == 0 ))
                            loop = TRUE;

                    } while( strncmp( buffer, "RINGING", 7 ) == 0 );

                    if( loop && ( attempt <= RedialAttempts )) {
                        ULONG   i;

                        for( i = RedialDelay; i > 0; i-- ) {
                            ULONG   sigmask = 0;

                            sprintf( buffer, CM_GetString( MSG_WAITING ), i );

                            set( data->Text, MUIA_Text_Contents, buffer );

                            Delay( TICKS_PER_SECOND );

                            if( DoMethod( _app( obj ), MUIM_Application_Input, &sigmask ) == MUIV_SerialAbort ) {
                                loop = FALSE;
                                break;
                            }
                        }

                    } else
                        loop = FALSE;

                    if( !loop && LogCalls && GenesisLoggerBase ) {

                        GL_StopLogger( loghandle );

#ifndef CMANAGER_MCC
                        CloseLibrary( GenesisLoggerBase );
#endif
                    }

                } while( loop );

                SerialAbort( data );

                SerialPrintf( data, "\r" );

                SerialFlush( data );
            }

        } else {

            go = MUI_Request( _app( obj ), data->MainWnd, 0, NULL,
                              CM_GetString( ANS_RETRY_CANCEL ),
                              CM_GetString( ERR_NO_DEVICE    ),
                              SerUnit, SerDevice );
        }

    } while( go );

    CloseSerial( data );

#ifdef CMANAGER_MCC

    set( obj, MUIA_Window_Open, FALSE );

    DoMethod( data->AppObj, OM_REMMEMBER, obj );

    DoMethod( data->AppObj, MUIM_Application_PushMethod, data->CMObj, 2, MUIM_CManager_DisposeObj, obj );

#else

    RemoveWindow( obj );

#endif

    set( data->MainWnd, MUIA_Window_Sleep, FALSE );

    return( 0 );
}
///
/// DialWindow_Close
static ULONG DialWindow_Close( struct IClass *cl, Object *obj, Msg msg )
{
    struct DialWindowData  *data = INST_DATA( cl, obj );

    data->Aborted = TRUE;

    DoMethod( _app( obj ), MUIM_Application_ReturnID, MUIV_SerialAbort );

    return( 0 );
}
///

