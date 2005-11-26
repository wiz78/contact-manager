/***************************************************************************
    revision             : $Id: mui.c,v 1.1.1.1 2003-01-06 16:00:30 tellini Exp $
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

#include <exec/types.h>                 // exec
#include <intuition/intuition.h>        // intuition
#include <dos/dos.h>                    // dos
#include <mui/listtree_mcc.h>           // mui
#include <mui/nlistview_mcc.h>
#include <mui/nlist_mcc.h>
#include <mui/textinput_mcc.h>
#include <mui/SpeedBar_mcc.h>
#include <mui/SpeedButton_mcc.h>
#include <libraries/mui.h>              // libraries
#include <libraries/gadtools.h>
#include <libraries/asl.h>
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/icon_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/icon_pragmas.h>

#include <mymacros.h>

#include "MUI.h"
#include "entrylist.h"
#include "grouplist.h"
#include "dial.h"
#include "edit.h"
#include "search.h"
#include "/about.h"

#include "/Library/CManager_protos.h"
#include "/Library/CManager_lib_lib.h"
///
/// Prototypes
#ifdef MY_DEBUG

extern void kprintf( char *, ... );

#define DBG(x)  { kprintf( "CManager.mcc - %s - ", __FUNC__ ); x; }

#else

#define DBG(x)

#endif
///
/// Data
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
    return((Object *) Child, PopaslObject,
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
/// LocalizeSpeedBar
void LocalizeSpeedBar( struct MUIS_SpeedBar_Button *button )
{
    while( button->Img != MUIV_SpeedBar_End ) {

        if( button->Text )
            button->Text = CM_GetString(( ULONG )button->Text );

        if( button->Help )
            button->Help = CM_GetString(( ULONG )button->Help );

        button++;
    }
}
///


/// InitClasses
BOOL InitClasses( void )
{
    CL_EntryList = MUI_CreateCustomClass( NULL, MUIC_NListview,
                                          NULL, sizeof( struct EntryListData ),
                                          EntryList_Dispatcher );

    CL_GroupList = MUI_CreateCustomClass( NULL, MUIC_Listtree,
                                          NULL, sizeof( struct GroupListData ),
                                          GroupList_Dispatcher );

    CL_DialWindow = MUI_CreateCustomClass( NULL, MUIC_Window,
                                           NULL, sizeof( struct DialWindowData ),
                                           DialWindow_Dispatcher );

    CL_Edit = MUI_CreateCustomClass( NULL, MUIC_Group,
                                     NULL, sizeof( struct EditData ),
                                     Edit_Dispatcher );

    CL_EditWWW = MUI_CreateCustomClass( NULL, MUIC_Group,
                                        NULL, sizeof( struct EditWWWData ),
                                        EditWWW_Dispatcher );

    CL_EditFTP = MUI_CreateCustomClass( NULL, MUIC_Group,
                                        NULL, sizeof( struct EditFTPData ),
                                        EditFTP_Dispatcher );

    CL_EditIRC = MUI_CreateCustomClass( NULL, MUIC_Group,
                                        NULL, sizeof( struct EditIRCData ),
                                        EditIRC_Dispatcher );

    CL_EditTelnet = MUI_CreateCustomClass( NULL, MUIC_Group,
                                           NULL, sizeof( struct EditTelnetData ),
                                           EditTelnet_Dispatcher );

    CL_EditWindow = MUI_CreateCustomClass( NULL, MUIC_Window,
                                           NULL, sizeof( struct EditWindowData ),
                                           EditWindow_Dispatcher );

    CL_AboutWindow = MUI_CreateCustomClass( NULL, MUIC_Window,
                                            NULL, sizeof( struct AboutData ),
                                            AboutWindow_Dispatcher );

    CL_SearchWindow = MUI_CreateCustomClass( NULL, MUIC_Window,
                                             NULL, sizeof( struct SearchWindowData ),
                                             SearchWindow_Dispatcher );

/*
    CL_ImageGad = MUI_CreateCustomClass( NULL, MUIC_Area,
                                         NULL, sizeof( struct ImageGadData ),
                                         ImageGad_Dispatcher );
*/
    DBG(
         kprintf( "EntryList:\t%08lx\n",      CL_EntryList );
         kprintf( "\t\tGroupList:\t%08lx\n",  CL_GroupList );
         kprintf( "\t\tDialWindow:\t%08lx\n", CL_DialWindow );
         kprintf( "\t\tEdit:\t%08lx\n",       CL_Edit );
         kprintf( "\t\tEditWWW:\t%08lx\n",    CL_EditWWW );
         kprintf( "\t\tEditFTP:\t%08lx\n",    CL_EditFTP );
         kprintf( "\t\tEditIRC:\t%08lx\n",    CL_EditIRC );
         kprintf( "\t\tEditWindow:\t%08lx\n", CL_EditWindow );
         kprintf( "\t\tAboutWindow:\t%08lx\n", CL_AboutWindow );
         kprintf( "\t\tEditTelnet:\t%08lx\n",  CL_EditTelnet );
    );

    if( CL_EntryList && CL_GroupList && CL_DialWindow && CL_Edit && CL_EditWWW &&
        CL_EditFTP && CL_EditIRC && CL_EditWindow && CL_AboutWindow && CL_EditTelnet &&
        CL_SearchWindow )
        return( TRUE );

    return( FALSE );
}
///
/// ExitClasses
void ExitClasses( void )
{
    DBG( kprintf( "start\n" ));

    DBG( kprintf( "EntryList\n" ));
    if( CL_EntryList         )  MUI_DeleteCustomClass( CL_EntryList         );
    DBG( kprintf( "GroupList\n" ));
    if( CL_GroupList         )  MUI_DeleteCustomClass( CL_GroupList         );
    DBG( kprintf( "DialWindow\n" ));
    if( CL_DialWindow        )  MUI_DeleteCustomClass( CL_DialWindow        );
    DBG( kprintf( "Edit\n" ));
    if( CL_Edit              )  MUI_DeleteCustomClass( CL_Edit              );
    DBG( kprintf( "EditWWW\n" ));
    if( CL_EditWWW           )  MUI_DeleteCustomClass( CL_EditWWW           );
    DBG( kprintf( "EditFTP\n" ));
    if( CL_EditFTP           )  MUI_DeleteCustomClass( CL_EditFTP           );
    DBG( kprintf( "EditIRC\n" ));
    if( CL_EditIRC           )  MUI_DeleteCustomClass( CL_EditIRC           );
    DBG( kprintf( "EditTelnet\n" ));
    if( CL_EditTelnet        )  MUI_DeleteCustomClass( CL_EditTelnet        );
    DBG( kprintf( "EditWindow\n" ));
    if( CL_EditWindow        )  MUI_DeleteCustomClass( CL_EditWindow        );
    DBG( kprintf( "AboutWindow\n" ));
    if( CL_AboutWindow       )  MUI_DeleteCustomClass( CL_AboutWindow       );
    if( CL_SearchWindow      )  MUI_DeleteCustomClass( CL_SearchWindow      );
//    if( CL_ImageGad          )  MUI_DeleteCustomClass( CL_ImageGad          );

    CL_EntryList  = CL_GroupList    = CL_DialWindow = CL_Edit        = CL_EditWWW =
    CL_EditFTP    = CL_EditIRC      = CL_EditWindow = CL_AboutWindow =
    CL_EditTelnet = CL_SearchWindow = NULL;

    DBG( kprintf( "end\n" ));
}
///
