/***************************************************************************
    revision             : $Id: mui.c,v 1.1.1.1 2003-01-06 15:54:05 tellini Exp $
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

/// Includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define INTUI_V36_NAMES_ONLY
#define CATCOMP_NUMBERS

#include <exec/types.h>                 // exec
#include <intuition/intuition.h>        // intuition
#include <dos/dos.h>                    // dos
#include <mui/listtree_mcc.h>           // mui
#include <mui/nlistview_mcc.h>
#include <mui/nlist_mcc.h>
#include <mui/textinput_mcc.h>
#include <libraries/mui.h>              // libraries
#include <libraries/gadtools.h>
#include <libraries/asl.h>
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/locale_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/icon_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/locale_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/icon_pragmas.h>

#include <mymacros.h>

#include "MUI.h"
#include "main.h"
#include "import.h"
#include "MCC/grouplist.h"
#include "MCC/entrylist.h"
#include "prefs.h"
#include "about.h"
//#include "image.h"
#include "Locale.h"

#include "MCC/CManager_mcc.h"

#include "Library/CManagerMsg.h"
#include "Library/CManager_protos.h"
#include "Library/CManager_lib_lib.h"
///
/// Prototypes
static SAVEDS ASM ULONG Application_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );

static ULONG Application_New( struct IClass *, Object *, struct opSet * );
static ULONG Application_Dispose( struct IClass *, Object *, Msg );
static ULONG Application_DisposeObj( struct IClass *, Object *, struct MUIP_MyApp_DisposeObj * );
static ULONG Application_Message( struct IClass *, Object *, Msg );
///
/// Data
struct MUI_CustomClass *CL_MyApplication;
struct MUI_CustomClass *CL_MainWindow;

struct Catalog         *Catalog;
///

/// mui_set
void mui_set( APTR obj, ULONG attr, APTR val )
{
    SetAttrs( obj, attr, val, TAG_DONE );
}
///
/// mui_get
void mui_get( APTR obj, ULONG attr, APTR valp )
{
    GetAttr( attr, obj, (ULONG *)valp );
}
///
/// mui_nnset
void mui_nnset( APTR obj, ULONG attr, LONG val )
{
    SetAttrs( obj, MUIA_NoNotify, TRUE, attr, val, TAG_DONE );
}
///
/// xget
LONG xget( Object *obj, ULONG attribute )
{
    LONG    x;

    get( obj, attribute, &x );

    return( x );
}
///
/// getstr
void getstr( Object *obj, STRPTR str )
{
    strcpy( str, (STRPTR)xget( obj, MUIA_String_Contents ));
}
///

/// DoSuperNew
ULONG DoSuperNew( struct IClass *cl, Object *obj, ULONG tag1, ... )
{
    return( DoSuperMethod( cl, obj, OM_NEW, &tag1, NULL ));
}
///

/// MakeFile
Object *MakeFile( Object **Obj, ULONG Size, ULONG Txt, LONG Help )
{
    return( Child, PopaslObject,
            MUIA_Popstring_String,  *Obj = MakeString( Size, Txt, Help ),
            MUIA_Popstring_Button,  PopButton( MUII_PopFile ),
            MUIA_Popasl_Type,       ASL_FileRequest,
            End );
}
///
/// MakeCheck
Object *MakeCheck( ULONG str, LONG help )
{
    Object *obj = MUI_MakeObject( MUIO_Checkmark, CM_GetString( str ));

    if( obj ) {

        set( obj, MUIA_CycleChain, TRUE );

        if( help != -1 )
            set( obj, MUIA_ShortHelp, CM_GetString( help ));
    }

    return( obj );
}
///
/// MakeButton
Object *MakeButton( ULONG str, LONG help )
{
    Object *obj = MUI_MakeObject( MUIO_Button, CM_GetString( str ));

    if( obj ) {

        set( obj, MUIA_CycleChain, TRUE );

        if( help != -1 )
            set( obj, MUIA_ShortHelp, CM_GetString( help ));
    }

    return( obj );
}
///
/// MakeButton2
Object *MakeButton2( STRPTR str )
{
    Object *obj = MUI_MakeObject( MUIO_Button, str );

    if( obj )
        set( obj, MUIA_CycleChain, TRUE );

    return( obj );
}
///
/// MakeString
Object *MakeString( ULONG maxlen, ULONG str, LONG help )
{
    Object *obj;
    UBYTE   ch = '\0', *ptr;

    if( str != (ULONG)-1 )
        if( ptr = strchr( CM_GetString( str ), '_' ))
            if( ch = ptr[1])
                ch = tolower( ch );

    obj = TextinputObject,
                StringFrame,
                MUIA_CycleChain,                            1,
                MUIA_Textinput_AdvanceOnCR,                 TRUE,
                MUIA_Textinput_Multiline,                   FALSE,
                MUIA_Textinput_MaxLen,                      maxlen,
                ch ? MUIA_ControlChar : TAG_IGNORE,         ch,
                help != -1 ? MUIA_ShortHelp : TAG_IGNORE,   CM_GetString( help ),
          End;

    return( obj );
}
///
/// MakeSecretString
Object *MakeSecretString( ULONG maxlen, ULONG str, LONG help )
{
    Object *obj;
    UBYTE   ch = '\0', *ptr;

    if( str != (ULONG)-1 )
        if( ptr = strchr( CM_GetString( str ), '_' ))
            if( ch = ptr[1])
                ch = tolower( ch );

    obj = TextinputObject,
                StringFrame,
                MUIA_CycleChain,                            1,
                MUIA_Textinput_Secret,                      TRUE,
                MUIA_Textinput_AdvanceOnCR,                 TRUE,
                MUIA_Textinput_Multiline,                   FALSE,
                MUIA_Textinput_MaxLen,                      maxlen,
                ch ? MUIA_ControlChar : TAG_IGNORE,         ch,
                help != -1 ? MUIA_ShortHelp : TAG_IGNORE,   CM_GetString( help ),
          End;

    return( obj );
}
///
/// MakeInteger
Object *MakeInteger( ULONG maxlen, ULONG str, LONG help )
{
    Object *obj;
    UBYTE   ch = '\0', *ptr;

    if( ptr = strchr( CM_GetString( str ), '_' ))
        if( ch = ptr[1])
            ch = tolower( ch );

    obj = TextinputObject,
                StringFrame,
                MUIA_CycleChain,                            1,
                MUIA_Textinput_AdvanceOnCR,                 TRUE,
                MUIA_Textinput_Multiline,                   FALSE,
                MUIA_Textinput_MaxLen,                      maxlen,
                MUIA_Textinput_AcceptChars,                 "0123456789",
                ch ? MUIA_ControlChar : TAG_IGNORE,         ch,
                help != -1 ? MUIA_ShortHelp : TAG_IGNORE,   CM_GetString( help ),
          End;

    return( obj );
}
///
/// MakeCycle
Object *MakeCycle( STRPTR *array, ULONG num )
{
    Object *obj = MUI_MakeObject( MUIO_Cycle, CM_GetString( num ), array );

    if( obj )
        set( obj, MUIA_CycleChain, TRUE );

    return( obj );
}
///
/// MakeSlider
Object *MakeSlider( ULONG str, LONG min, LONG max )
{
    Object *obj = MUI_MakeObject( MUIO_Slider, CM_GetString( str ), min, max );

    if( obj )
        set( obj, MUIA_CycleChain, TRUE );

    return( obj );
}
///
/// MakeLabel1
Object *MakeLabel1( ULONG str )
{
    return( MUI_MakeObject( MUIO_Label, CM_GetString( str ), MUIO_Label_SingleFrame ));
}
///

/// LocalizeMenus
void LocalizeMenus( struct NewMenu *menu )
{
    while( menu->nm_Type != NM_END ) {

        if( menu->nm_Label != NM_BARLABEL )
            menu->nm_Label = CM_GetString( (ULONG)menu->nm_Label );

        if( menu->nm_CommKey )
            menu->nm_CommKey = CM_GetString( (ULONG)menu->nm_CommKey );

        menu++;
    }
}
///
/// LocalizeArray
void LocalizeArray( UBYTE **Array )
{
    while( *Array ) {
        *Array = CM_GetString( (ULONG)*Array );
        *Array++;
    }
}
///


/// InitClasses
BOOL InitClasses( void )
{
    CL_MyApplication = MUI_CreateCustomClass( NULL, MUIC_Application,
                                              NULL, sizeof( struct AppData ),
                                              Application_Dispatcher );

    CL_MainWindow = MUI_CreateCustomClass( NULL, MUIC_Window,
                                           NULL, sizeof( struct MainWindowData ),
                                           MainWindow_Dispatcher );

    CL_ListWindow = MUI_CreateCustomClass( NULL, MUIC_Window,
                                           NULL, sizeof( struct ListWindowData ),
                                           ListWindow_Dispatcher );

    CL_AboutWindow = MUI_CreateCustomClass( NULL, MUIC_Window,
                                            NULL, 0,
                                            AboutWindow_Dispatcher );

    CL_PrefsWindow = MUI_CreateCustomClass( NULL, MUIC_Window,
                                            NULL, sizeof( struct PrefsWindowData ),
                                            PrefsWindow_Dispatcher );

/*
    CL_ImageGad = MUI_CreateCustomClass( NULL, MUIC_Area,
                                         NULL, sizeof( struct ImageGadData ),
                                         ImageGad_Dispatcher );
*/
    if( CL_MyApplication && CL_MainWindow &&
        CL_ListWindow && CL_AboutWindow && CL_PrefsWindow )
        return( TRUE );

    return( FALSE );
}
///
/// ExitClasses
void ExitClasses( void )
{
    if( CL_MyApplication     )  MUI_DeleteCustomClass( CL_MyApplication     );
    if( CL_MainWindow        )  MUI_DeleteCustomClass( CL_MainWindow        );
    if( CL_ListWindow        )  MUI_DeleteCustomClass( CL_ListWindow        );
    if( CL_AboutWindow       )  MUI_DeleteCustomClass( CL_AboutWindow       );
    if( CL_PrefsWindow       )  MUI_DeleteCustomClass( CL_PrefsWindow       );
//    if( CL_ImageGad          )  MUI_DeleteCustomClass( CL_ImageGad          );
}
///


/// Application_Dispatcher
ULONG SAVEDS ASM Application_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( Application_New( cl, obj, (APTR)msg ));

        case OM_DISPOSE:
            return( Application_Dispose( cl, obj, (APTR)msg ));

        case MUIM_MyApp_DisposeObj:
            return( Application_DisposeObj( cl, obj, (APTR)msg ));

        case MUIM_MyApp_Message:
            return( Application_Message( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// Application_New
static ULONG Application_New( struct IClass *cl, Object *obj, struct opSet *msg )
{

    if( obj = ( Object * )DoSuperNew( cl, obj,
                                      TAG_MORE, msg->ops_AttrList )) {
        struct AppData *data;

        data = INST_DATA( cl, obj );

        data->Handler.ihn_Object  = obj;
        data->Handler.ihn_Signals = 1L << MyLibPort->mp_SigBit;
        data->Handler.ihn_Flags   = 0;
        data->Handler.ihn_Method  = MUIM_MyApp_Message;

        DoMethod( obj, MUIM_Application_AddInputHandler, &data->Handler );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// Application_Dispose
static ULONG Application_Dispose( struct IClass *cl, Object *obj, Msg msg )
{
    struct AppData *data = INST_DATA( cl, obj );

    DoMethod( obj, MUIM_Application_RemInputHandler, &data->Handler );

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// Application_DisposeObj
static ULONG Application_DisposeObj( struct IClass *cl, Object *obj, struct MUIP_MyApp_DisposeObj *msg )
{
    MUI_DisposeObject( msg->Object );

    return( 0 );
}
///
/// Application_Message
static ULONG Application_Message( struct IClass *cl, Object *obj, Msg m )
{
    struct CMMessage   *msg;

#ifdef DEBUG
    kprintf( "CManager - Application_Message()\n" );
#endif

    while( msg = (struct CMMessage *) GetMsg( MyLibPort )) {

#ifdef DEBUG
        kprintf( "CManager - got msg\n" );
#endif

        if( msg->Msg.mn_Node.ln_Type != NT_REPLYMSG )
            switch( msg->Command ) {

                case CMC_QUIT:
#ifdef DEBUG
                    kprintf( "CManager - pushed quit method\n" );
#endif
                    DoMethod( obj, MUIM_Application_PushMethod, obj, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit );
                    break;

                case CMC_GET_ENTRY: {
                    struct MainWindowData  *data = INST_DATA( CL_MainWindow->mcc_Class, MainWin );

                    AcceptEntry = msg->Data;
#ifdef DEBUG
                    kprintf( "CManager - got CMC_GET_ENTRY, setting click hook\n" );
#endif
                    set( data->CMObj, MUIA_CManager_AppDoubleClick, &data->ClickHook );
                }   break;
            }

#ifdef DEBUG
        kprintf( "CManager - ReplyMsg()\n" );
#endif
        ReplyMsg(( struct Message * )msg );
    }

    return( 0 );
}
///

/// RemoveWindow
void RemoveWindow( Object *Window )
{
    Object *AppObj;

    set( Window, MUIA_Window_Open, FALSE );

    get( Window, MUIA_ApplicationObject, &AppObj );

    DoMethod( AppObj, OM_REMMEMBER, Window );

    DoMethod( AppObj, MUIM_Application_PushMethod, AppObj, 2, MUIM_MyApp_DisposeObj, Window );
}
///
