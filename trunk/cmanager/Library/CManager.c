/***************************************************************************
    revision             : $Id: CManager.c,v 1.1.1.1 2003-01-06 15:55:43 tellini Exp $
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
#define __USE_SYSBASE
#include <exec/types.h>                 //  exec
#include <exec/memory.h>
#include <exec/libraries.h>
#include <dos/dos.h>                    //  dos
#include <dos/rdargs.h>
#include <dos/dostags.h>
#include <graphics/gfx.h>               //  graphics
#include <libraries/genesis.h>          //  libraries
#include <libraries/mui.h>
#include <libraries/locale.h>
#include <clib/alib_protos.h>           //  clib
#include <proto/exec.h>                 //  proto
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/graphics.h>
#include <proto/genesis.h>
#include <proto/intuition.h>
#include <proto/locale.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <pragmas/genesis_pragmas.h>

#include "/mymacros.h"
#ifdef __STORM__
#include <extrafuncs.h>
#pragma libbase Library;
#endif

#define CATCOMP_ARRAY

#include "/CManager.h"
#include "/Locale.h"

#include "CManagerMsg.h"
///
/// Prototypes
#define DATA_VERSION    11

struct CMHandle {
        struct MsgPort *MyPort;
        struct MsgPort *CMPort;
        STRPTR          CMARexx;
};

#ifdef MY_DEBUG

extern void kprintf( char *, ... );

#define DBG(x)  { kprintf( "CManager.library - %s - ", __FUNC__ ); x; }

#else

#define DBG(x)

#endif

static BOOL                 SendMsg( struct CMHandle *, ULONG, ULONG );

static void                 FreeSection( struct CMGroup * );

static void                 LoadUsers( BPTR, struct CMGroup *, ULONG );
static void                 LoadWWWs( BPTR, struct CMGroup *, ULONG );
static void                 LoadFTPs( BPTR, struct CMGroup *, ULONG );
static void                 LoadChat( BPTR, struct CMGroup *, ULONG );
static void                 LoadTelnet( BPTR, struct CMGroup *, ULONG );

static void                 SaveUsers( BPTR, struct CMGroup * );
static void                 SaveWWWs( BPTR, struct CMGroup * );
static void                 SaveFTPs( BPTR, struct CMGroup * );
static void                 SaveChat( BPTR, struct CMGroup * );
static void                 SaveTelnet( BPTR, struct CMGroup * );

static ULONG                CountNodes( struct MinList * );

static void                 Crypt( UBYTE *, UBYTE *, ULONG, ULONG );
static void                 FPutString( BPTR, STRPTR );
static void                 FGetString( BPTR, STRPTR, ULONG );

static LONG                 my_rand( ULONG * );
///
/// Data
static APTR                     Pool;
static struct SignalSemaphore   PoolSemaphore;
struct GfxBase                 *GfxBase;
#ifndef __STORM__
struct ExecBase                *SysBase;
struct DosLibrary              *DOSBase;
struct Library                 *UtilityBase;
#endif
static struct Library          *GenesisBase;
struct Library          *LocaleBase;

static struct Catalog          *Catalog;

static UBYTE                    CMID[] = { 'C', 'M', 'D', 'F' };
///

#define sprintf MySprintf

UWORD fmtfunc[] = { 0x16c0, 0x4e75 };

void MySprintf( char *to, char *fmt, ... )
{
    RawDoFmt( fmt, &fmt + 1, (APTR)fmtfunc, to );
}

#ifndef __STORM__
#define LibCreatePool CreatePool
#define LibDeletePool DeletePool
#define LibFreePooled FreePooled
#define LibAllocPooled AllocPooled
#endif

/// Init
#ifdef __STORM__
void INIT_3_Init( void )
#else
long SAVEDS ASM __UserLibInit( REG(a6) struct Library *libbase )
#endif
{
#ifndef __STORM__
    SysBase = *(( struct ExecBase** )4 );
    DOSBase = (struct DosLibrary *) OpenLibrary( "dos.library", 37 );

    if( !DOSBase )
        return( -1 );

    UtilityBase = OpenLibrary( "utility.library", 37 );

    if( !UtilityBase ) {
        CloseLibrary( (struct Library *)DOSBase );
        return( -1 );
    }
#endif

    DBG( kprintf( "Init\n" ));

    GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 36 );

    Pool    = LibCreatePool( MEMF_CLEAR, sizeof( struct CMUser ) * 50, sizeof( struct CMUser ) * 50 );

    memset( &PoolSemaphore, 0, sizeof( PoolSemaphore ));

    InitSemaphore( &PoolSemaphore );

    if( LocaleBase = OpenLibrary( "locale.library", 36 )) {
        ULONG   cnt;

        Catalog = OpenCatalog( NULL, "CManager.catalog", OC_Version, 0, TAG_DONE );

        for( cnt = 0; cnt < ( sizeof( CatCompArray ) / sizeof( struct CatCompArrayType )); cnt++ )
            CatCompArray[ cnt ].cca_Str = GetCatalogStr( Catalog, CatCompArray[ cnt ].cca_ID, CatCompArray[ cnt ].cca_Str );
    }

#ifndef __STORM__
    return( 0 );
#endif
}
///
/// Exit
#ifdef __STORM__
void EXIT_3_Exit( void )
#else
void SAVEDS ASM __UserLibCleanup( void )
#endif
{
    DBG( kprintf( "Exit\n" ));

    if( Pool ) {
        LibDeletePool( Pool );
        Pool = NULL;
    }

    if( Catalog ) {
        CloseCatalog( Catalog );
        Catalog = NULL;
    }

    if( LocaleBase ) {
        CloseLibrary( LocaleBase );
        LocaleBase = NULL;
    }

    if( GfxBase ) {
        CloseLibrary(( struct Library * )GfxBase );
        GfxBase = NULL;
    }

#ifndef __STORM__
    CloseLibrary( (struct Library *)DOSBase );
    CloseLibrary( UtilityBase );
#endif
}
///

/// SendMsg
static BOOL SendMsg( struct CMHandle *cm, ULONG Command, ULONG Data )
{
    BOOL    ret = FALSE;

    Forbid();

#ifdef DEBUG
    kprintf( "SendMsg( %08lx, %ld, %08lx );\n", cm, Command, Data );
#endif

    if( FindPort( cm->CMARexx )) {
        struct CMMessage    msg = { 0 };

        msg.Msg.mn_ReplyPort = cm->MyPort;
        msg.Msg.mn_Length    = sizeof( msg );
        msg.Command          = Command;
        msg.Data             = Data;

#ifdef DEBUG
        kprintf( "SendMsg - PutMsg()\n" );
#endif

        if( Command == CMC_QUIT )
            Signal( cm->CMPort->mp_SigTask, SIGBREAKF_CTRL_C );
        else
            PutMsg( cm->CMPort, (struct Message *)&msg );

        Permit();

#ifdef DEBUG
        kprintf( "SendMsg - WaitPort()\n" );
#endif
        if( Command != CMC_QUIT )
            do {
                WaitPort( cm->MyPort );
            } while(!( GetMsg( cm->MyPort )));

#ifdef DEBUG
        kprintf( "SendMsg - exiting\n" );
#endif

        ret = TRUE; // msg sent

    } else
        Permit();

    return( ret );
}
///

/// AllocPooledVec
static void *AllocPooledVec( APTR Pool, ULONG Size )
{
    ULONG  *ptr;

    Size += 4;

    if( ptr = LibAllocPooled( Pool, Size ))
        *ptr++ = Size;

    return( ptr );
}
///
/// FreePooledVec
static void FreePooledVec( APTR Pool, APTR Buffer )
{
    ULONG  *ptr;

    if( ptr = Buffer ) {
        ULONG   size;

        size = *--ptr;

        LibFreePooled( Pool, ptr, size );
    }
}
///


/// CM_GetString
STRPTR SAVEDS ASM CM_GetString( REG( d0 ) ULONG ID )
{
    struct CatCompArrayType    *cc;
    ULONG                       cnt;

    cc = CatCompArray;

    for( cnt = 0; cnt < ( sizeof( CatCompArray ) / sizeof( struct CatCompArrayType )); cnt++, cc++ )
        if( cc->cca_ID == ID )
            return( cc->cca_Str );

    return( "" );
}
///

/// CM_FreeHandle
void SAVEDS ASM CM_FreeHandle( REG( a0 ) APTR Handle, REG( d0 ) BOOL Close )
{
    struct CMHandle    *cm;

#ifdef DEBUG
    kprintf( "--------------- CM_FreeHandle() ---------------\n" );
#endif

    if( cm = Handle ) {

        if( cm->MyPort ) {

            if( Close ) {

#ifdef DEBUG
                kprintf( "About to call SendMsg( cm, CMC_QUIT, 0 )\n" );
#endif
                SendMsg( cm, CMC_QUIT, 0 );
            }

#ifdef DEBUG
            kprintf( "Deleting msg port\n" );
#endif

            DeleteMsgPort( cm->MyPort );
        }

        ObtainSemaphore( &PoolSemaphore );

        FreePooledVec( Pool, cm );

        ReleaseSemaphore( &PoolSemaphore );
    }

#ifdef DEBUG
    kprintf( "--------------- CM_FreeHandle() ---------------\n" );
#endif
}
///

/// CM_StartManager
APTR SAVEDS ASM CM_StartManager( REG( a0 ) STRPTR File, REG( a1 ) STRPTR PubScreen )
{
    struct CMHandle    *cm;

    ObtainSemaphore( &PoolSemaphore );

    if( cm = AllocPooledVec( Pool, sizeof( struct CMHandle ))) {
        TEXT    path[256] = "", command[512];
        BOOL    ok = FALSE;
        BPTR    lock;

        GetVar( "vapor/CManager_path", path, sizeof( path ), 0 );

        strcpy( command, path );

        if(!( lock = Lock( command, ACCESS_READ ))) {

            if( lock = Lock( "CManager", ACCESS_READ ))
                strcpy( command, "CManager" );

            else if( lock = Lock( "SYS:Prefs/CManager", ACCESS_READ ))
                strcpy( command, "SYS:Prefs/CManager" );

#ifdef NETCONNECT
            else if( lock = Lock( "netconnect2:programs/cmanager/cmanager", ACCESS_READ ))
                strcpy( command, "netconnect2:programs/cmanager/cmanager" );
#endif
        }

        if( lock ) {

            UnLock( lock );

            if( File ) {
                strcat( command, " \"" );
                strcat( command, File );
                strcat( command, "\"" );
            }

            if( PubScreen ) {
                strcat( command, " PUBSCREEN \"" );
                strcat( command, PubScreen );
                strcat( command, "\"" );
            }

            if( cm->MyPort = CreateMsgPort()) {

                sprintf( path, " PORT %ld", cm->MyPort );

                strcat( command, path );

                if(!( SystemTags( command,
                                  SYS_Asynch,   TRUE,
                                  SYS_Input,    Open( "NIL:", MODE_NEWFILE ),
                                  SYS_Output,   Open( "NIL:", MODE_NEWFILE ),
//                                  SYS_Output,   Open( "CON:0/0/640/200/CManager debug/WAIT/CLOSE", MODE_NEWFILE ),
                                  TAG_END ))) {

#ifdef DEBUG
                    kprintf( "SystemTags() ok\n" );
#endif

                    do {
                        struct CMMessage   *msg;

                        WaitPort( cm->MyPort );

                        while( msg = (struct CMMessage *) GetMsg( cm->MyPort )) {

                            switch( msg->Command ) {

                                case CMC_PORT:
                                    cm->CMPort = (struct MsgPort *) msg->Data;
                                    break;

                                case CMC_AREXX:
                                    cm->CMARexx = (STRPTR) msg->Data;
                                    break;
                            }

                            ReplyMsg(( struct Message * )msg );
                        }

                        if( cm->CMPort && cm->CMARexx )
                            ok = TRUE;

                    } while(!( ok ));
                }
            }
        }

        if( ok == FALSE ) {

#ifdef DEBUG
            kprintf( "CM_StartManager() failed, calling CM_FreeHandle()\n" );
#endif

            CM_FreeHandle( cm, FALSE );

            cm = NULL;
        }
    }

    ReleaseSemaphore( &PoolSemaphore );

    return( cm );
}
///

/// CM_GetEntry
APTR SAVEDS ASM CM_GetEntry( REG( a0 ) APTR Handle, REG( d0 ) ULONG Flags )
{
    struct CMHandle    *cm;
    APTR                entry = NULL;

    if( cm = Handle ) {

#ifdef DEBUG
        kprintf( ">>> GetEntry()\n" );
#endif

        if( SendMsg( cm, CMC_GET_ENTRY, Flags )) {
            BOOL    run = TRUE;

            do {
                struct CMMessage   *msg;

#ifdef DEBUG
                kprintf( "GetEntry - WaitPort\n" );
#endif
                WaitPort( cm->MyPort );

                while( msg = (struct CMMessage *) GetMsg( cm->MyPort )) {

                    if( msg->Command == CMC_GET_ENTRY ) {
                        entry = (APTR) msg->Data;
                        run   = FALSE;
                    }

#ifdef DEBUG
                    kprintf( "GetEntry - ReplyMsg\n" );
#endif
                    ReplyMsg(( struct Message * )msg );
                }

            } while( run );
        }

#ifdef DEBUG
        kprintf( "<<< GetEntry()\n" );
#endif
    }

    return( entry );
}
///
/// CM_GetParent
struct CMGroup * SAVEDS ASM CM_GetParent( REG( a0 ) struct CMGroup *List, REG( a1 ) struct CMGroup *Current )
{
    struct CMGroup *group;

    if( Current == List )
        return( List );

    for( group = (struct CMGroup *) List->SubGroups.mlh_Head; group->Succ; group = group->Succ ) {
        struct CMGroup *temp;

        if( group == Current )
            return( List );

        else if( temp = CM_GetParent( group, Current ))
            return( temp );
    }

    return( NULL );
}
///

/// CM_AllocEntry
APTR SAVEDS ASM CM_AllocEntry( REG( d0 ) ULONG Type )
{
    APTR    entry = NULL;
    ULONG   size = 0;

    if( Pool == NULL )
        return( NULL );

    ObtainSemaphore( &PoolSemaphore );

    switch( Type ) {

        case CME_SECTION:
        case CME_GROUP:
            if( entry = AllocPooledVec( Pool, sizeof( struct CMGroup ))) {

                memset( entry, 0, sizeof( struct CMGroup ));

                ((struct CMGroup *)entry )->Type = Type;

                if( Type == CME_SECTION )
                    ((struct CMGroup *)entry )->Flags = GRPF_BOLD;

                NewList(( struct List * )&((struct CMGroup *)entry )->Entries   );
                NewList(( struct List * )&((struct CMGroup *)entry )->SubGroups );
            }
            break;

        case CME_USER:
            size = sizeof( struct CMUser );
            break;

        case CME_WWW:
            size = sizeof( struct CMWWW );
            break;

        case CME_FTP:
            size = sizeof( struct CMFTP );
            break;

        case CME_CHAT:
            size = sizeof( struct CMChat );
            break;

        case CME_TELNET:
            size = sizeof( struct CMTelnet );
            break;

        case CME_IMAGE:
            size = sizeof( struct CMImage );
            break;

        case CME_LIST:
            if( entry = AllocPooledVec( Pool, sizeof( struct MinList )))
                NewList(( struct List * )entry );
            break;
    }

    if( size )
        if( entry = AllocPooledVec( Pool, size )) {
            memset( entry, 0, size );
            ((struct CMUser *)entry )->Type = Type;
        }

    ReleaseSemaphore( &PoolSemaphore );

    return( entry );
}
///
/// CM_DeleteBitMap
void SAVEDS ASM CM_DeleteBitMap( REG( a0 ) struct BitMap *bm )
{
    if( bm ) {
        if((( struct Library * )GfxBase )->lib_Version >= 39 ) {
            FreeBitMap( bm );
        } else {
            FreeVec( bm->Planes[0] );
            FreeMem( bm, sizeof( struct BitMap ));
        }
    }
}
///
/// CM_FreeEntry
void SAVEDS ASM CM_FreeEntry( REG( a0 ) APTR entry )
{
    ObtainSemaphore( &PoolSemaphore );

    if( entry ) {

        switch((( struct CMUser * )entry )->Type ) {

            case CME_SECTION:
            case CME_GROUP: {
                struct CMUser  *sub;
                struct CMGroup *grp;

                while( sub = (struct CMUser *) RemTail(( struct List * )&((struct CMGroup *)entry )->Entries ))
                    CM_FreeEntry( sub );

                while( grp = (struct CMGroup *) RemTail(( struct List * )&((struct CMGroup *)entry )->SubGroups ))
                    CM_FreeEntry( grp );
            }   break;

            case CME_USER:
                CM_FreeEntry((( struct CMUser * )entry )->Image );
                break;

            case CME_WWW:
            case CME_FTP:
            case CME_CHAT:
            case CME_TELNET:
                break;

            case CME_IMAGE:
                CM_DeleteBitMap((( struct CMImage * )entry )->BitMap );

                FreeVec((( struct CMImage * )entry )->Colors );
                break;
        }

        FreePooledVec( Pool, entry );
    }

    ReleaseSemaphore( &PoolSemaphore );
}
///
/// CM_FreeList
void SAVEDS ASM CM_FreeList( REG( a0 ) struct MinList *list )
{
    if( list ) {
        APTR    entry;

        ObtainSemaphore( &PoolSemaphore );

        while( entry = RemTail(( struct List * )list ))
            CM_FreeEntry( entry );

        FreePooledVec( Pool, list );

        ReleaseSemaphore( &PoolSemaphore );
    }
}
///

/// CM_LoadData
BOOL SAVEDS ASM CM_LoadData( REG( a0 ) STRPTR PrefsName, REG( a1 ) struct CMData *Data, REG( a2 ) STRPTR User )
{
    BPTR    file;
    BOOL    ret = FALSE;

    if( Data && ( file = Open( PrefsName, MODE_OLDFILE ))) {
        UBYTE           ID[4];

        FRead( file, ID, sizeof( ID ), 1 );

        if( memcmp( CMID, ID, sizeof( ID )) == 0 ) {
            ULONG   Version;
            TEXT    FileUser[41];

            FRead( file, &Version, sizeof( ULONG ), 1 );

            FGetString( file, FileUser, sizeof( FileUser ));

            if(( User == NULL ) || ( FileUser[0] == '\0' ) || ( stricmp( FileUser, User ) == 0 )) {

                LoadUsers( file,  Data->Users,  Version );
                LoadWWWs( file,   Data->WWWs,   Version );
                LoadFTPs( file,   Data->FTPs,   Version );
                LoadChat( file,   Data->Chat,   Version );

                if( Version >= 9 )
                    LoadTelnet( file, Data->Telnet, Version );
            }

            ret = TRUE;
        }

        Close( file );
    }

    return( ret );
}

BOOL SAVEDS ASM CM_LoadDataOld( REG( a0 ) STRPTR PrefsName, REG( a1 ) struct CMDataOld *Data, REG( a2 ) STRPTR User )
{
    struct CMData   dt;

    memset( &dt, 0, sizeof( dt ));

    CopyMem( Data, &dt, sizeof( dt ));

    dt.SizeOf = sizeof( struct CMDataOld );

    return( CM_LoadData( PrefsName, &dt, User ));
}
///
/// CM_SaveData
void SAVEDS ASM CM_SaveData( REG( a0 ) STRPTR PrefsName, REG( a1 ) struct CMData *Data, REG( a2 ) STRPTR User )
{
    BPTR    file;

    DBG( kprintf( "user: 0x%08lx (%s)\n", User, User ? User : "" ));

    if( file = Open( PrefsName, MODE_NEWFILE )) {
        ULONG   n;

        n = DATA_VERSION;

        FWrite( file, CMID, sizeof( CMID ),  1 );
        FWrite( file, &n,   sizeof( ULONG ), 1 );

        FPutString( file, User ? User : (STRPTR)"" );

        SaveUsers( file,  Data->Users  );
        SaveWWWs( file,   Data->WWWs   );
        SaveFTPs( file,   Data->FTPs   );
        SaveChat( file,   Data->Chat   );
        SaveTelnet( file, Data->Telnet );

        Close( file );
    }
}

void SAVEDS ASM CM_SaveDataOld( REG( a0 ) STRPTR PrefsName, REG( a1 ) struct CMDataOld *Data, REG( a2 ) STRPTR User )
{
    struct CMData   dt;

    memset( &dt, 0, sizeof( dt ));

    CopyMem( Data, &dt, sizeof( dt ));

    dt.SizeOf = sizeof( struct CMDataOld );

    CM_SaveData( PrefsName, &dt, User );
}
///
/// CM_FreeData
void SAVEDS ASM CM_FreeData( REG( a0 ) struct CMData *Data )
{
    if( Data ) {

        DBG( kprintf( "Freeing 0x%08lx, size %ld, version %ld\n", Data, Data->SizeOf, Data->Version ));

        FreeSection( Data->Users );
        FreeSection( Data->WWWs  );
        FreeSection( Data->FTPs  );
        FreeSection( Data->Chat  );

        if( Data->Version >= 1 )
            FreeSection( Data->Telnet );

        if( Data->FreeMe ) {

            CM_FreeEntry( Data->Users );
            CM_FreeEntry( Data->WWWs  );
            CM_FreeEntry( Data->FTPs  );
            CM_FreeEntry( Data->Chat  );

            if( Data->Version >= 1 )
                CM_FreeEntry( Data->Telnet );

            ObtainSemaphore( &PoolSemaphore );

            FreePooledVec( Pool, Data );

            ReleaseSemaphore( &PoolSemaphore );
        }
    }
}

void SAVEDS ASM CM_FreeDataOld( REG( a0 ) struct CMDataOld *Data )
{
    if( Data->FreeMe )
        CM_FreeData(( struct CMData * )Data );

    else {
        struct CMData   dt;

        memset( &dt, 0, sizeof( dt ));

        CopyMem( Data, &dt, sizeof( dt ));

        dt.SizeOf = sizeof( struct CMDataOld );

        CM_FreeData( &dt );
    }
}
///
/// CM_AllocCMData
struct CMData * SAVEDS ASM CM_AllocCMData( void )
{
    struct CMData  *cm;

    ObtainSemaphore( &PoolSemaphore );

    if( cm = AllocPooledVec( Pool, sizeof( struct CMData ))) {
        struct Process *proc;
        APTR            wnd;

        memset( cm, 0, sizeof( struct CMData ));

        cm->FreeMe  = TRUE;
        cm->Version = CMD_CURRENT_VERSION;
        cm->SizeOf  = sizeof( *cm );

        proc = (struct Process *)FindTask( NULL );
        wnd  = proc->pr_WindowPtr;

        proc->pr_WindowPtr = (APTR)-1;

        if( GenesisBase = OpenLibrary( GENESISNAME, 2 )) {
            struct User    *ud;

            if( ud = GetGlobalUser() ) {

                strcpy( cm->CurrentUser, ud->us_name );

                FreeUser( ud );
            }

            CloseLibrary( GenesisBase );
        }

        proc->pr_WindowPtr = wnd;

        DBG( kprintf( "CMData allocated at 0x%08lx, size %ld\n", cm, cm->SizeOf ));

        cm->Users  = CM_AllocEntry( CME_SECTION );
        cm->WWWs   = CM_AllocEntry( CME_SECTION );
        cm->FTPs   = CM_AllocEntry( CME_SECTION );
        cm->Chat   = CM_AllocEntry( CME_SECTION );
        cm->Telnet = CM_AllocEntry( CME_SECTION );

        if( cm->Users && cm->WWWs && cm->FTPs && cm->Chat && cm->Telnet ) {

            stccpy( cm->Users->Name,  CM_GetString( MSG_USERS  ), sizeof( cm->Users->Name  ));
            stccpy( cm->WWWs->Name,   CM_GetString( MSG_WWW    ), sizeof( cm->WWWs->Name   ));
            stccpy( cm->FTPs->Name,   CM_GetString( MSG_FTP    ), sizeof( cm->FTPs->Name   ));
            stccpy( cm->Chat->Name,   CM_GetString( MSG_CHAT   ), sizeof( cm->Chat->Name   ));
            stccpy( cm->Telnet->Name, CM_GetString( MSG_TELNET ), sizeof( cm->Telnet->Name ));

        } else {

            CM_FreeData( cm );
            cm = NULL;
        }
    }

    ReleaseSemaphore( &PoolSemaphore );

    return( cm );
}
///
/// CM_LoadCurrentUserData
struct CMData * SAVEDS ASM CM_LoadCurrentUserData( REG( d0 ) BOOL AskUser )
{
    struct CMData  *cm;

    if( cm = CM_AllocCMData() ) {
        struct Process *proc;
        APTR            wnd;
        TEXT            file[80];

        cm->CurrentUser[0] = '\0';
        cm->Path[0]        = '\0';

        if( GetVar( "vapor/CManager_path", cm->Path, sizeof( cm->Path ), 0 ) > 0 ) {
            STRPTR  ptr;

            if( ptr = PathPart( cm->Path ))
                *ptr = '\0';
        }

        strcpy( file, "Default.data" );

        proc = (struct Process *)FindTask( NULL );
        wnd  = proc->pr_WindowPtr;

        proc->pr_WindowPtr = (APTR)-1;

        if( GenesisBase = OpenLibrary( GENESISNAME, 2 )) {
            struct User    *ud;

            if((!( ud = GetGlobalUser())) && AskUser )
                ud = GetUser( NULL, NULL, NULL, 0 );
            else
                GetUserName( 0, cm->CurrentUser, sizeof( cm->CurrentUser ));

            if( ud ) {

                strcpy( cm->CurrentUser, ud->us_name );

                SetGlobalUser( ud );

                FreeUser( ud );
            }

            if( cm->CurrentUser[0] )
                sprintf( file, "%s/%s.data", cm->CurrentUser, cm->CurrentUser );

            CloseLibrary( GenesisBase );
        }

        proc->pr_WindowPtr = wnd;

        AddPart( cm->Path, file, sizeof( cm->Path ));

        CM_LoadData( cm->Path, cm, cm->CurrentUser[0] ? cm->CurrentUser : NULL );
    }

    return( cm );
}
///

/// CM_AddEntry
BOOL SAVEDS ASM CM_AddEntry( REG( a0 ) APTR entry )
{
    BOOL            ret = FALSE;
    struct CMData  *cm;

    if( cm = CM_LoadCurrentUserData( TRUE )) {
        UBYTE           type;
        struct CMUser  *newentry;

        type = (( struct CMUser * )entry )->Type;

        if( newentry = CM_AllocEntry( type )) {
            struct CMGroup *list;
            ULONG           size;

            switch( type ) {

                case CME_USER:
                    list = cm->Users;
                    size = sizeof( struct CMUser );
                    break;

                case CME_WWW:
                    list = cm->WWWs;
                    size = sizeof( struct CMWWW );
                    break;

                case CME_FTP:
                    list = cm->FTPs;
                    size = sizeof( struct CMFTP );
                    break;

                case CME_CHAT:
                    list = cm->Chat;
                    size = sizeof( struct CMChat );
                    break;

                case CME_TELNET:
                    list = cm->Telnet;
                    size = sizeof( struct CMTelnet );
                    break;
            }

            CopyMem( entry, newentry, size );

            AddTail(( struct List * )&list->Entries, ( struct Node * )newentry );

            CM_SaveData( cm->Path, cm, cm->CurrentUser );

            ret = TRUE;
        }

        CM_FreeData( cm );
    }

    return( ret );
}
///
/// CM_CreateBitMap
struct BitMap * SAVEDS ASM CM_CreateBitMap( REG( d0 ) ULONG width, REG( d1 ) ULONG height, REG( d2 ) ULONG depth,
                                REG( d3 ) ULONG flags, REG( a0 ) struct BitMap *friend )
{
    struct BitMap *bm;

    if((( struct Library * )GfxBase )->lib_Version >= 39 ) {
        bm = AllocBitMap( width, height, depth, flags | BMF_CLEAR, friend );
    } else {

        if( bm = AllocMem( sizeof( struct BitMap ), 0L )) {

            InitBitMap( bm, depth, width, height );

            if( bm->Planes[0] = (PLANEPTR) AllocVec( depth * RASSIZE( width, height ), MEMF_CHIP | MEMF_CLEAR )) {
                LONG i;

                for( i = 1; i < depth; i++ )
                    bm->Planes[ i ] = bm->Planes[ i - 1 ] + RASSIZE( width, height );

            } else {
                FreeMem( bm, sizeof( struct BitMap ));
                bm = NULL;
            }
        }
    }

    return( bm );
}
///

/// FreeSection
static void FreeSection( struct CMGroup *Section )
{
    if( Section ) {
        struct CMGroup *entry;

        while( entry = (struct CMGroup *) RemTail(( struct List * )&Section->SubGroups ))
            CM_FreeEntry( entry );
    }
}
///

/// LoadUsers
static void LoadUsers( BPTR File, struct CMGroup *Group, ULONG Version )
{
    ULONG   n, i;

    if( !Group )
        return;

    NewList(( struct List * )&Group->Entries   );
    NewList(( struct List * )&Group->SubGroups );

    Group->Flags = FGetC( File );
    FGetC( File );

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld entries\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMUser  *entry;

        if( entry = CM_AllocEntry( CME_USER )) {

            entry->Flags = FGetC( File );
            FGetC( File );

            FGetString( File, entry->Name,     sizeof( entry->Name      ));
            FGetString( File, entry->LastName, sizeof( entry->LastName  ));
            FGetString( File, entry->Address,  sizeof( entry->Address   ));
            FGetString( File, entry->City,     sizeof( entry->City      ));
            FGetString( File, entry->Country,  sizeof( entry->Country   ));
            FGetString( File, entry->ZIP,      sizeof( entry->ZIP       ));
            FGetString( File, entry->Comment,  sizeof( entry->Comment   ));
            FGetString( File, entry->EMail,    sizeof( entry->EMail     ));
            FGetString( File, entry->WWW,      sizeof( entry->WWW       ));
            FGetString( File, entry->FTP,      sizeof( entry->FTP       ));
            FGetString( File, entry->Phone,    sizeof( entry->Phone     ));
            FGetString( File, entry->Fax,      sizeof( entry->Fax       ));
            FGetString( File, entry->Alias,    sizeof( entry->Alias     ));

            if( Version >= 1 ) {

                FGetString( File, entry->Mobile, sizeof( entry->Mobile ));

                if( Version >= 4 ) {

                    FGetString( File, entry->ICQ, sizeof( entry->ICQ ));

                    if( Version >= 5 ) {

                        FGetString( File, entry->Company,     sizeof( entry->Company      ));
                        FGetString( File, entry->BJobTitle,   sizeof( entry->BJobTitle    ));
                        FGetString( File, entry->BOffice,     sizeof( entry->BOffice      ));
                        FGetString( File, entry->BAddress,    sizeof( entry->BAddress     ));
                        FGetString( File, entry->BCity,       sizeof( entry->BCity        ));
                        FGetString( File, entry->BCountry,    sizeof( entry->BCountry     ));
                        FGetString( File, entry->BZIP,        sizeof( entry->BZIP         ));
                        FGetString( File, entry->BDepartment, sizeof( entry->BDepartment  ));
                        FGetString( File, entry->BPhone,      sizeof( entry->BPhone       ));
                        FGetString( File, entry->BFax,        sizeof( entry->BFax         ));
                        FGetString( File, entry->BEMail,      sizeof( entry->BEMail       ));

                        if( Version >= 6 ) {

                            FGetString( File, entry->PGPUserID, sizeof( entry->PGPUserID ));

                            if( Version >= 7 ) {

                                FGetString( File, entry->BMobile, sizeof( entry->BMobile ));
                            }
                        }
                    }
                }
            }

            AddTail(( struct List * )&Group->Entries, ( struct Node * )entry );
        }
    }

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld subgroups\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMGroup *group;

        if( group = CM_AllocEntry( CME_GROUP )) {

            AddTail(( struct List * )&Group->SubGroups, ( struct Node * )group );

            FGetString( File, group->Name, sizeof( group->Name ));

            LoadUsers( File, group, Version );
        }
    }
}
///
/// LoadWWWs
static void LoadWWWs( BPTR File, struct CMGroup *Group, ULONG Version )
{
    ULONG   n, i;

    if( !Group )
        return;

    NewList(( struct List * )&Group->Entries   );
    NewList(( struct List * )&Group->SubGroups );

    Group->Flags = FGetC( File );
    FGetC( File );

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld entries\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMWWW   *entry;

        if( entry = CM_AllocEntry( CME_WWW )) {

            entry->Flags = FGetC( File );
            FGetC( File );

            FGetString( File, entry->Name,      sizeof( entry->Name      ));
            FGetString( File, entry->Comment,   sizeof( entry->Comment   ));
            FGetString( File, entry->WWW,       sizeof( entry->WWW       ));
            FGetString( File, entry->WebMaster, sizeof( entry->WebMaster ));
            FGetString( File, entry->EMail,     sizeof( entry->EMail     ));

            if( Version >= 8 ) {

                FRead( File, &entry->LastModified, sizeof( entry->LastModified ), 1 );
                FRead( File, &entry->LastVisited,  sizeof( entry->LastVisited  ), 1 );

                if( Version >= 10 )
                    FGetString( File, entry->Alias, sizeof( entry->Alias ));
            }

            AddTail(( struct List * )&Group->Entries, ( struct Node * )entry );
        }
    }

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld subgroups\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMGroup *group;

        if( group = CM_AllocEntry( CME_GROUP )) {

            AddTail(( struct List * )&Group->SubGroups, ( struct Node * )group );

            FGetString( File, group->Name, sizeof( group->Name ));

            LoadWWWs( File, group, Version );
        }
    }
}
///
/// LoadFTPs
static void LoadFTPs( BPTR File, struct CMGroup *Group, ULONG Version )
{
    ULONG   n, i;

    if( !Group )
        return;

    NewList(( struct List * )&Group->Entries   );
    NewList(( struct List * )&Group->SubGroups );

    Group->Flags = FGetC( File );
    FGetC( File );

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld entries\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMFTP   *entry;

        if( entry = CM_AllocEntry( CME_FTP )) {

            entry->Flags = FGetC( File );
            FGetC( File );

            FRead( File, &entry->Port,    sizeof( entry->Port    ), 1 );
            FRead( File, &entry->Retries, sizeof( entry->Retries ), 1 );

            FGetString( File, entry->Name,     sizeof( entry->Name     ));
            FGetString( File, entry->Comment,  sizeof( entry->Comment  ));
            FGetString( File, entry->FTP,      sizeof( entry->FTP      ));
            FGetString( File, entry->Username, sizeof( entry->Username ));
            FGetString( File, entry->Password, sizeof( entry->Password ));

            if( Version >= 2 ) {

                FGetString( File, entry->Local, sizeof( entry->Local ));

                if( Version >= 8 ) {

                    FRead( File, &entry->LastModified, sizeof( entry->LastModified ), 1 );
                    FRead( File, &entry->LastVisited,  sizeof( entry->LastVisited  ), 1 );

                    if( Version >= 10 ) {
                        FGetString( File, entry->Alias, sizeof( entry->Alias ));

                        if( Version >= 11 ) {

                            FRead( File, &entry->ExtraFlags,  sizeof( entry->ExtraFlags  ), 1 );
                            FRead( File, &entry->WindowStyle, sizeof( entry->WindowStyle ), 1 );
                        }
                    }
                }
            }

            AddTail(( struct List * )&Group->Entries, ( struct Node * )entry );
        }
    }

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld subgroups\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMGroup *group;

        if( group = CM_AllocEntry( CME_GROUP )) {

            AddTail(( struct List * )&Group->SubGroups, ( struct Node * )group );

            FGetString( File, group->Name, sizeof( group->Name ));

            LoadFTPs( File, group, Version );
        }
    }
}
///
/// LoadChat
static void LoadChat( BPTR File, struct CMGroup *Group, ULONG Version )
{
    ULONG   n, i;

    if( !Group )
        return;

    NewList(( struct List * )&Group->Entries   );
    NewList(( struct List * )&Group->SubGroups );

    Group->Flags = FGetC( File );
    FGetC( File );

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld entries\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMChat  *entry;

        if( entry = CM_AllocEntry( CME_CHAT )) {

            entry->Flags = FGetC( File );
            FGetC( File );

            FGetString( File, entry->Channel,    sizeof( entry->Channel    ));
            FGetString( File, entry->Server,     sizeof( entry->Server     ));
            FGetString( File, entry->Maintainer, sizeof( entry->Maintainer ));
            FGetString( File, entry->Nick,       sizeof( entry->Nick       ));
            FGetString( File, entry->WWW,        sizeof( entry->WWW        ));
            FGetString( File, entry->Comment,    sizeof( entry->Comment    ));

            if( Version >= 3 )
                FGetString( File, entry->Password, sizeof( entry->Password ));

            AddTail(( struct List * )&Group->Entries, ( struct Node * )entry );
        }
    }

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld subgroups\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMGroup *group;

        if( group = CM_AllocEntry( CME_GROUP )) {

            AddTail(( struct List * )&Group->SubGroups, ( struct Node * )group );

            FGetString( File, group->Name, sizeof( group->Name ) );

            LoadChat( File, group, Version );
        }
    }
}
///
/// LoadTelnet
static void LoadTelnet( BPTR File, struct CMGroup *Group, ULONG Version )
{
    ULONG   n, i;

    if( !Group )
        return;

    NewList(( struct List * )&Group->Entries   );
    NewList(( struct List * )&Group->SubGroups );

    Group->Flags = FGetC( File );
    FGetC( File );

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld entries\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMTelnet    *entry;

        if( entry = CM_AllocEntry( CME_TELNET )) {

            entry->Flags = FGetC( File );
            FGetC( File );

            FGetString( File, entry->Host,     sizeof( entry->Host     ));
            FGetString( File, entry->Login,    sizeof( entry->Login    ));
            FGetString( File, entry->Password, sizeof( entry->Password ));
            FGetString( File, entry->Comment,  sizeof( entry->Comment  ));

            FRead( File, &entry->Port, sizeof( ULONG ), 1 );

            AddTail(( struct List * )&Group->Entries, ( struct Node * )entry );
        }
    }

    FRead( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "load %ld subgroups\n", n ));

    for( i = 0; i < n; i++ ) {
        struct CMGroup *group;

        if( group = CM_AllocEntry( CME_GROUP )) {

            AddTail(( struct List * )&Group->SubGroups, ( struct Node * )group );

            FGetString( File, group->Name, sizeof( group->Name ) );

            LoadTelnet( File, group, Version );
        }
    }
}
///

/// SaveUsers
static void SaveUsers( BPTR File, struct CMGroup *Group )
{
    struct CMUser  *entry;
    ULONG           n;

    FPutC( File, Group->Flags );
    FPutC( File, 0 );

    n = CountNodes( &Group->Entries );

    FWrite( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "save %ld entries\n", n ));

    for( entry = (struct CMUser *) Group->Entries.mlh_Head; entry->Succ; entry = entry->Succ ) {

        FPutC( File, entry->Flags );
        FPutC( File, 0 );

        FPutString( File, entry->Name        );
        FPutString( File, entry->LastName    );
        FPutString( File, entry->Address     );
        FPutString( File, entry->City        );
        FPutString( File, entry->Country     );
        FPutString( File, entry->ZIP         );
        FPutString( File, entry->Comment     );
        FPutString( File, entry->EMail       );
        FPutString( File, entry->WWW         );
        FPutString( File, entry->FTP         );
        FPutString( File, entry->Phone       );
        FPutString( File, entry->Fax         );
        FPutString( File, entry->Alias       );
        FPutString( File, entry->Mobile      );
        FPutString( File, entry->ICQ         );
        FPutString( File, entry->Company     );
        FPutString( File, entry->BJobTitle   );
        FPutString( File, entry->BOffice     );
        FPutString( File, entry->BAddress    );
        FPutString( File, entry->BCity       );
        FPutString( File, entry->BCountry    );
        FPutString( File, entry->BZIP        );
        FPutString( File, entry->BDepartment );
        FPutString( File, entry->BPhone      );
        FPutString( File, entry->BFax        );
        FPutString( File, entry->BEMail      );
        FPutString( File, entry->PGPUserID   );
        FPutString( File, entry->BMobile     );
    }

    n = CountNodes( &Group->SubGroups );

    FWrite( File, &n, sizeof( ULONG ), 1 );

    DBG( kprintf( "save %ld subgroups\n", n ));

    for( Group = (struct CMGroup *) Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ ) {

        FPutString( File, Group->Name );

        SaveUsers( File, Group );
    }
}
///
/// SaveWWWs
static void SaveWWWs( BPTR File, struct CMGroup *Group )
{
    struct CMWWW   *entry;
    ULONG           n;

    FPutC( File, Group->Flags );
    FPutC( File, 0 );

    n = CountNodes( &Group->Entries );

    FWrite( File, &n, sizeof( ULONG ), 1 );

    for( entry = (struct CMWWW *) Group->Entries.mlh_Head; entry->Succ; entry = entry->Succ ) {

        FPutC( File, entry->Flags );
        FPutC( File, 0 );

        FPutString( File, entry->Name      );
        FPutString( File, entry->Comment   );
        FPutString( File, entry->WWW       );
        FPutString( File, entry->WebMaster );
        FPutString( File, entry->EMail     );

        FWrite( File, &entry->LastModified, sizeof( entry->LastModified ), 1 );
        FWrite( File, &entry->LastVisited,  sizeof( entry->LastVisited  ), 1 );

        FPutString( File, entry->Alias );
    }

    n = CountNodes( &Group->SubGroups );

    FWrite( File, &n, sizeof( ULONG ), 1 );

    for( Group = (struct CMGroup *) Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ ) {

        FPutString( File, Group->Name );

        SaveWWWs( File, Group );
    }
}
///
/// SaveFTPs
static void SaveFTPs( BPTR File, struct CMGroup *Group )
{
    struct CMFTP   *entry;
    ULONG           n;

    FPutC( File, Group->Flags );
    FPutC( File, 0 );

    n = CountNodes( &Group->Entries );

    FWrite( File, &n, sizeof( ULONG ), 1 );

    for( entry = (struct CMFTP *) Group->Entries.mlh_Head; entry->Succ; entry = entry->Succ ) {

        FPutC( File, entry->Flags );
        FPutC( File, 0 );

        FWrite( File, &entry->Port,    sizeof( entry->Port    ), 1 );
        FWrite( File, &entry->Retries, sizeof( entry->Retries ), 1 );

        FPutString( File, entry->Name     );
        FPutString( File, entry->Comment  );
        FPutString( File, entry->FTP      );
        FPutString( File, entry->Username );
        FPutString( File, entry->Password );
        FPutString( File, entry->Local    );

        FWrite( File, &entry->LastModified, sizeof( entry->LastModified ), 1 );
        FWrite( File, &entry->LastVisited,  sizeof( entry->LastVisited  ), 1 );

        FPutString( File, entry->Alias );

        FWrite( File, &entry->ExtraFlags,  sizeof( entry->ExtraFlags  ), 1 );
        FWrite( File, &entry->WindowStyle, sizeof( entry->WindowStyle ), 1 );
    }

    n = CountNodes( &Group->SubGroups );

    FWrite( File, &n, sizeof( ULONG ), 1 );

    for( Group = (struct CMGroup *) Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ ) {

        FPutString( File, Group->Name );

        SaveFTPs( File, Group );
    }
}
///
/// SaveChat
static void SaveChat( BPTR File, struct CMGroup *Group )
{
    struct CMChat  *entry;
    ULONG           n;

    FPutC( File, Group->Flags );
    FPutC( File, 0 );

    n = CountNodes( &Group->Entries );

    FWrite( File, &n, sizeof( ULONG ), 1 );

    for( entry = (struct CMChat *) Group->Entries.mlh_Head; entry->Succ; entry = entry->Succ ) {

        FPutC( File, entry->Flags );
        FPutC( File, 0 );

        FPutString( File, entry->Channel    );
        FPutString( File, entry->Server     );
        FPutString( File, entry->Maintainer );
        FPutString( File, entry->Nick       );
        FPutString( File, entry->WWW        );
        FPutString( File, entry->Comment    );
        FPutString( File, entry->Password   );
    }

    n = CountNodes( &Group->SubGroups );

    FWrite( File, &n, sizeof( ULONG ), 1 );

    for( Group = (struct CMGroup *) Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ ) {

        FPutString( File, Group->Name );

        SaveChat( File, Group );
    }
}
///
/// SaveTelnet
static void SaveTelnet( BPTR File, struct CMGroup *Group )
{
    struct CMTelnet    *entry;
    ULONG               n;

    FPutC( File, Group ? Group->Flags : 0 );
    FPutC( File, 0 );

    n = Group ? CountNodes( &Group->Entries ) : 0;

    FWrite( File, &n, sizeof( ULONG ), 1 );

    if( Group )
        for( entry = (struct CMTelnet *) Group->Entries.mlh_Head; entry->Succ; entry = entry->Succ ) {

            FPutC( File, entry->Flags );
            FPutC( File, 0 );

            FPutString( File, entry->Host     );
            FPutString( File, entry->Login    );
            FPutString( File, entry->Password );
            FPutString( File, entry->Comment  );

            FWrite( File, &entry->Port, sizeof( ULONG ), 1 );
        }

    n = Group ? CountNodes( &Group->SubGroups ) : 0;

    FWrite( File, &n, sizeof( ULONG ), 1 );

    if( Group )
        for( Group = (struct CMGroup *) Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ ) {

            FPutString( File, Group->Name );

            SaveTelnet( File, Group );
        }
}
///

/// CountNodes
static ULONG CountNodes( struct MinList *List )
{
    ULONG           cnt;
    struct MinNode *node;

    cnt = 0;

    for( node = List->mlh_Head; node->mln_Succ; node = node->mln_Succ )
        ++cnt;

    return( cnt );
}
///

/// Crypt
static void Crypt( UBYTE *From, UBYTE *To, ULONG len, ULONG maxlen )
{
    ULONG   i, seed;

    seed = len;

    len = MIN( len, maxlen );

    for( i = 0; i < len; i++ )
        *To++ = *From++ ^ my_rand( &seed );
}
///
/// FPutString 
static void FPutString( BPTR file, STRPTR str )
{
    UWORD   len;
    TEXT    buffer[512];

    len = strlen( str );

    FWrite( file, &len, sizeof( UWORD ), 1 );

    Crypt( str, buffer, len, len );

    FWrite( file, buffer, len, 1 );
}
///
/// FGetString
static void FGetString( BPTR file, STRPTR str, ULONG maxlen )
{
    UWORD   len;

    FRead( file, &len, sizeof( UWORD ), 1 );

    FRead( file, str, MIN( len, maxlen ), 1 );

    maxlen -= 1; // '\0' char

    str[ MIN( len, maxlen ) ] = '\0';

    Crypt( str, str, len, maxlen );

    if( len > ++maxlen ) {
        Flush( file );
        Seek( file, len - maxlen, OFFSET_CURRENT );
    }
}
///

/// my_rand
static LONG my_rand( ULONG *seed )
{
    LONG    ret, a;

    a     = (LONG)*seed;    // MOVE.L  0040(A4),D0
    ret   = a;              // MOVE.L  D0,D1
    ret <<= 3;              // ASL.L   #3,D1
    ret  -= a;              // SUB.L   D0,D1
    ret <<= 3;              // ASL.L   #3,D1
    ret  += a;              // ADD.L   D0,D1
    ret  += ret;            // ADD.L   D1,D1
    ret  += a;              // ADD.L   D0,D1
    ret <<= 4;              // ASL.L   #4,D1
    ret  -= a;              // SUB.L   D0,D1
    ret  += ret;            // ADD.L   D1,D1
    ret  -= a;              // SUB.L   D0,D1
    ret  += 0x00000E60;     // ADDI.L  #00000e60,D1
    ret  &= 0x7FFFFFFF;     // ANDI.L  #7fffffff,D1
    a     = ret;            // MOVE.L  D1,D0
    a    -= 1;              // SUBQ.L  #1,D0
    *seed = (ULONG)a;       // MOVE.L  D0,0040(A4)

    return( ret );          // MOVE.L  D1,D0
}
///

