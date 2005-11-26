/***************************************************************************
    revision             : $Id: entrylist.c,v 1.1.1.1 2003-01-06 15:51:35 tellini Exp $
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

#define INTUI_V36_NAMES_ONLY
#define CATCOMP_NUMBERS

#include <exec/types.h>                 // exec
#include <exec/memory.h>
#include <intuition/intuition.h>        // intuition
#include <dos/dos.h>                    // dos
#include <dos/dostags.h>
#include <mui/nlistview_mcc.h>          // mui
#include <mui/nlist_mcc.h>
#include <mui/listtree_mcc.h>
#include <rexx/storage.h>               // rexx
#include <rexx/errors.h>
#include <iffp/ftxt.h>                  // iffp
#include <libraries/mui.h>              // libraries
#include <libraries/gadtools.h>
#include <libraries/openurl.h>
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/utility_protos.h>
#include <clib/rexxsyslib_protos.h>
#include <clib/openurl_protos.h>
#include <clib/iffparse_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/utility_pragmas.h>
#include <pragmas/rexxsyslib_pragmas.h>
#include <pragmas/openurl_pragmas.h>
#include <pragmas/iffparse_pragmas.h>

#include <mymacros.h>

#include "CManager.h"
#include "entrylist.h"
#include "edit.h"
#include "mui.h"
#include "rexx.h"
#include "main.h"
#include "prefs.h"
#include "about.h"
#include "locale.h"

#include "Library/CManagerMsg.h"
#include "Library/CManager_protos.h"
#include "Library/CManager_lib_lib.h"
///
/// Prototypes
#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d)    ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#define SCP_TO_CLIP     ((ULONG)-1)

static ULONG EntryList_New( struct IClass *, Object *, struct opSet * );
static ULONG EntryList_Dispose( struct IClass *, Object *, Msg );
static ULONG EntryList_Set( struct IClass *, Object *, struct opSet * );
static ULONG EntryList_Get( struct IClass *, Object *, struct opGet * );
static ULONG EntryList_Setup( struct IClass *, Object *, struct opSet * );
static ULONG EntryList_Cleanup( struct IClass *, Object *, struct opSet * );
static ULONG EntryList_HandleInput( struct IClass *, Object *, struct MUIP_HandleInput * );
static ULONG EntryList_ContextMenuChoice( struct IClass *, Object *, struct MUIP_ContextMenuChoice * );
static ULONG EntryList_Import(struct IClass *, Object *, struct MUIP_Import * );
static ULONG EntryList_Export(struct IClass *, Object *, struct MUIP_Export * );
static ULONG EntryList_Remove( struct IClass *, Object *, Msg );
static ULONG EntryList_Clone( struct IClass *, Object *, Msg );
static ULONG EntryList_StartScript( struct IClass *, Object *, struct MUIP_EntryList_StartScript * );
static ULONG EntryList_Edit( struct IClass *, Object *, Msg );
static ULONG EntryList_DoubleClick( struct IClass *, Object *, Msg );
static ULONG EntryList_HandleARexx( struct IClass *, Object *, Msg );
static ULONG EntryList_CopyToClip( struct IClass *, Object *, Msg );

static ULONG EditWindow_New( struct IClass *, Object *, struct opSet * );
static ULONG EditWindow_Get( struct IClass *, Object *, struct opGet * );
static ULONG EditWindow_Open( struct IClass *, Object *, struct MUIP_EditWindow_Open * );
static ULONG EditWindow_Close( struct IClass *, Object *, struct MUIP_EditWindow_Close * );

static BOOL ShowEditWindow( Object *, APTR );
static void CloseEditWindow( Object *, APTR );
///
/// Data
struct MUI_CustomClass *CL_EntryList;
struct MUI_CustomClass *CL_EditWindow;

static STRPTR   Scripts[] = {
                        WWWScript,
                        FTPScript,
                        MailScript,
                        NULL,
                        "SendFax.rexx",
                        IRCScript,
                        "SendDOpusFTP.rexx"
                };

ULONG   AcceptEntry;
///

/// EntryList_Dispatcher
ULONG SAVEDS ASM EntryList_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_SET:
            return( EntryList_Set( cl, obj, (APTR)msg ));

        case OM_GET:
            return( EntryList_Get( cl, obj, (APTR)msg ));

        case OM_NEW:
            return( EntryList_New( cl, obj, (APTR)msg ));

        case OM_DISPOSE:
            return( EntryList_Dispose( cl, obj, (APTR)msg ));

        case MUIM_Setup:
            return( EntryList_Setup( cl, obj, (APTR)msg ));

        case MUIM_Cleanup:
            return( EntryList_Cleanup( cl, obj, (APTR)msg ));

        case MUIM_HandleInput:
            return( EntryList_HandleInput( cl, obj, (APTR)msg ));

        case MUIM_ContextMenuChoice:
            return( EntryList_ContextMenuChoice( cl, obj, (APTR)msg ));

        case MUIM_Import:
            return( EntryList_Import( cl, obj, (APTR)msg ));

        case MUIM_Export:
            return( EntryList_Export( cl, obj, (APTR)msg ));

        case MUIM_EntryList_Remove:
            return( EntryList_Remove( cl, obj, (APTR)msg ));

        case MUIM_EntryList_Clone:
            return( EntryList_Clone( cl, obj, (APTR)msg ));

        case MUIM_EntryList_StartScript:
            return( EntryList_StartScript( cl, obj, (APTR)msg ));

        case MUIM_EntryList_Edit:
            return( EntryList_Edit( cl, obj, (APTR)msg ));

        case MUIM_EntryList_DoubleClick:
            return( EntryList_DoubleClick( cl, obj, (APTR)msg ));

        case MUIM_EntryList_HandleARexx:
            return( EntryList_HandleARexx( cl, obj, (APTR)msg ));

        case MUIM_EntryList_CopyToClip:
            return( EntryList_CopyToClip( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///

/// EntryList_New
static ULONG EntryList_New( struct IClass *cl, Object *obj, struct opSet *msg )
{

    if( obj = ( Object * )DoSuperNew( cl, obj,
                                      TAG_MORE, msg->ops_AttrList )) {
        struct EntryListData   *data;

        data = INST_DATA( cl, obj );

        data->ContextMenu = NULL;
        data->RexxPort    = CreateMsgPort();

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// EntryList_Dispose
static ULONG EntryList_Dispose( struct IClass *cl, Object *obj, Msg msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );

    if( data->ContextMenu )
        MUI_DisposeObject( data->ContextMenu );

    if( data->RexxPort ) {

        while( data->RexxOut ) {
            struct RexxMsg *RxMsg;

            WaitPort( data->RexxPort );

            if( RxMsg = (struct RexxMsg *) GetMsg( data->RexxPort )) {

                DeleteArgstring( RxMsg->rm_Args[0] );
                DeleteRexxMsg( RxMsg );

                data->RexxOut -= 1;
            }
        }

        DeleteMsgPort( data->RexxPort );
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// EntryList_Set
static ULONG EntryList_Set( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );
    struct TagItem         *tag;
    BOOL                    quiet = FALSE;
    static BOOL             FirstTime = TRUE;

    if( tag = FindTagItem( MUIA_EntryList_Type, msg->ops_AttrList )) {
        static struct NewMenu UserMenu[] = {
                NM_TITLE, (STRPTR)MSG_USERS, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)GAD_TO_WWW, NULL, 0, 0, (APTR)SCP_WWW,
                  NM_ITEM, (STRPTR)GAD_TO_FTP, NULL, 0, 0, (APTR)SCP_FTP,
                  NM_ITEM, (STRPTR)MSG_TO_DOPUS, NULL, 0, 0, (APTR)SCP_DOPUSFTP,
                  NM_ITEM, (STRPTR)GAD_TO_EMAIL, NULL, 0, 0, 0,
                    NM_SUB, (STRPTR)MSG_HOME, NULL, 0, 0, (APTR)SCP_EMAIL,
                    NM_SUB, (STRPTR)MSG_BUSINESS, NULL, 0, 0, (APTR)SCP_BUSINESS_EMAIL,
                  NM_ITEM, (STRPTR)GAD_TO_FAX, NULL, 0, 0, 0,
                    NM_SUB, (STRPTR)MSG_HOME, NULL, 0, 0, (APTR)SCP_FAX,
                    NM_SUB, (STRPTR)MSG_BUSINESS, NULL, 0, 0, (APTR)SCP_BUSINESS_FAX,
                  NM_ITEM, (STRPTR)GAD_TO_PHONE, NULL, 0, 0, (APTR)0,
                    NM_SUB, (STRPTR)MSG_PHONE, NULL, 0, 0, (APTR)SCP_PHONE,
                    NM_SUB, (STRPTR)MSG_MOBILE, NULL, 0, 0, (APTR)SCP_MOBILE,
                    NM_SUB, (STRPTR)MSG_BUSINESS, NULL, 0, 0, (APTR)SCP_BUSINESS_PHONE,
                NM_END, NULL, NULL, 0, 0, NULL
        };
        static struct NewMenu WWWMenu[] = {
                NM_TITLE, (STRPTR)MSG_WWW, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)GAD_TO_WWW, NULL, 0, 0, (APTR)SCP_WWW,
                  NM_ITEM, (STRPTR)MSG_TO_CLIP, NULL, 0, 0, (APTR)SCP_TO_CLIP,
                NM_END, NULL, NULL, 0, 0, NULL
        };
        static struct NewMenu FTPMenu[] = {
                NM_TITLE, (STRPTR)MSG_FTP, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)GAD_TO_FTP, NULL, 0, 0, (APTR)SCP_FTP,
                  NM_ITEM, (STRPTR)MSG_TO_DOPUS, NULL, 0, 0, (APTR)SCP_DOPUSFTP,
                  NM_ITEM, (STRPTR)GAD_TO_WWW, NULL, 0, 0, (APTR)SCP_WWW,
                  NM_ITEM, (STRPTR)MSG_TO_CLIP, NULL, 0, 0, (APTR)SCP_TO_CLIP,
                NM_END, NULL, NULL, 0, 0, NULL
        };
        static struct NewMenu ChatMenu[] = {
                NM_TITLE, (STRPTR)MSG_CHAT, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)MENU_JOIN_CHANNEL, NULL, 0, 0, (APTR)SCP_IRC,
                  NM_ITEM, (STRPTR)GAD_TO_WWW, NULL, 0, 0, (APTR)SCP_WWW,
                NM_END, NULL, NULL, 0, 0, NULL
        };

        if( FirstTime ) {

            FirstTime = FALSE;

            LocalizeMenus( UserMenu );
            LocalizeMenus( WWWMenu  );
            LocalizeMenus( FTPMenu  );
            LocalizeMenus( ChatMenu );
        }

        set( obj, MUIA_NList_Quiet, TRUE );

        quiet = TRUE;

        data->Type = tag->ti_Data;

        if( data->ContextMenu ) {
            MUI_DisposeObject( data->ContextMenu );
            data->ContextMenu = NULL;
        }

        switch( data->Type ) {

            case CME_USER:
                data->ContextMenu = MUI_MakeObject( MUIO_MenustripNM, UserMenu, 0 );
                break;

            case CME_WWW:
                data->ContextMenu = MUI_MakeObject( MUIO_MenustripNM, WWWMenu, 0 );
                break;

            case CME_FTP:
                data->ContextMenu = MUI_MakeObject( MUIO_MenustripNM, FTPMenu, 0 );
                break;

            case CME_CHAT:
                data->ContextMenu = MUI_MakeObject( MUIO_MenustripNM, ChatMenu, 0 );
                break;
        }

        set( obj, MUIA_ContextMenu, data->ContextMenu );
    }

    if( tag = FindTagItem( MUIA_EntryList_Group, msg->ops_AttrList )) {

        if( tag->ti_Data != MUIV_Listtree_Active_Off ) {
            struct CMUser  *user;

            data->Group = (struct CMGroup *) ((struct MUIS_Listtree_TreeNode  *)tag->ti_Data )->tn_User;

            if(!( quiet )) {
                set( obj, MUIA_NList_Quiet, TRUE );
                quiet = TRUE;
            }

            DoMethod( obj, MUIM_NList_Clear );

            while( user = (struct CMUser *) RemHead(( struct List * )&data->Group->Entries ))
                DoMethod( obj, MUIM_NList_InsertSingle, user, MUIV_NList_Insert_Bottom );

            set( obj, MUIA_NList_Active, MUIV_NList_Active_Top );
        }
    }

    if( quiet )
        set( obj, MUIA_NList_Quiet, FALSE );

    return( DoSuperMethodA( cl, obj, (Msg)msg ));
}
///
/// EntryList_Get
static ULONG EntryList_Get( struct IClass *cl, Object *obj, struct opGet *msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );

    switch ( msg->opg_AttrID ) {

        case MUIA_EntryList_Type:
            *msg->opg_Storage = data->Type;
            return( TRUE );
    }

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// EntryList_Setup
static ULONG EntryList_Setup( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );

    if(!( DoSuperMethodA( cl, obj, (Msg) msg )))
        return( FALSE );

    MUI_RequestIDCMP( obj, IDCMP_RAWKEY );

    if( data->RexxPort ) {

        data->RexxHandler.ihn_Object         = obj;
        data->RexxHandler.ihn_stuff.ihn_sigs = ( 1L << data->RexxPort->mp_SigBit ) | ( 1L << MyLibPort->mp_SigBit );
        data->RexxHandler.ihn_Method         = MUIM_EntryList_HandleARexx;

        DoMethod( _app( obj ), MUIM_Application_AddInputHandler, &data->RexxHandler );
    }

    return( TRUE );
}
///
/// EntryList_Cleanup
static ULONG EntryList_Cleanup( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );

    MUI_RejectIDCMP( obj, IDCMP_RAWKEY );

    if( data->RexxPort )
        DoMethod( _app( obj ), MUIM_Application_RemInputHandler, &data->RexxHandler );

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///
/// EntryList_HandleInput
static ULONG EntryList_HandleInput( struct IClass *cl, Object *obj, struct MUIP_HandleInput *msg )
{

    if( msg->imsg ) {

        switch( msg->imsg->Class ) {

            case IDCMP_RAWKEY:
                switch( msg->imsg->Code ) {

                    case 0x46:   // DEL
                        DoMethod( obj, MUIM_EntryList_Remove );
                        return( 0 );
                }
                break;
        }
    }

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///
/// EntryList_ContextMenuChoice
static ULONG EntryList_ContextMenuChoice( struct IClass *cl, Object *obj, struct MUIP_ContextMenuChoice *msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );
    ULONG                   scp;

    scp = muiUserData( msg->item );

    if( scp == SCP_TO_CLIP )
        DoMethod( obj, MUIM_EntryList_CopyToClip );
    else
        DoMethod( obj, MUIM_EntryList_StartScript, scp );

    return( 0 );
}
///
/// EntryList_Import
static ULONG EntryList_Import( struct IClass *cl, Object *obj, struct MUIP_Import *msg )
{
    ULONG   id;

    if( id = muiNotifyData( obj )->mnd_ObjectID ) {
        struct OrderPrefs  *data;

        if( data = (struct OrderPrefs *) DoMethod( msg->dataspace, MUIM_Dataspace_Find, id ))
            Order = *data;
    }

    return( 0 );
}
///
/// EntryList_Export
static ULONG EntryList_Export( struct IClass *cl, Object *obj, struct MUIP_Export *msg )
{
    ULONG   id;

    if( id = muiNotifyData( obj )->mnd_ObjectID )
        DoMethod( msg->dataspace, MUIM_Dataspace_Add, &Order, sizeof( Order ), id );

    return( 0 );
}
///
/// EntryList_Remove
static ULONG EntryList_Remove( struct IClass *cl, Object *obj, Msg msg )
{
    struct CMUser  *entry;
    LONG            pos;

    set( obj, MUIA_NList_Quiet, TRUE );

    pos = MUIV_NList_NextSelected_Start;

    DoMethod( obj, MUIM_NList_NextSelected, &pos );

    while( pos != MUIV_NList_NextSelected_End ) {

        DoMethod( obj, MUIM_NList_GetEntry, pos, &entry );

        CloseEditWindow( _app( obj ), entry );

        CM_FreeEntry( entry );

        DoMethod( obj, MUIM_NList_NextSelected, &pos );

        Flags &= ~SAVED;
    }

    DoMethod( obj, MUIM_NList_Remove, MUIV_NList_Remove_Selected );

    DoMethod( obj, MUIM_NList_Select, MUIV_NList_Select_All, MUIV_NList_Select_Off, NULL );

    set( obj, MUIA_NList_Quiet, FALSE );

    return( 0 );
}
///
/// EntryList_Clone
static ULONG EntryList_Clone( struct IClass *cl, Object *obj, Msg msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );
    struct CMUser          *from;
    LONG                    pos;
    struct MinList          toadd;

    pos = MUIV_NList_NextSelected_Start;

    DoMethod( obj, MUIM_NList_NextSelected, &pos );

    NewList(( struct List * )&toadd );

    while( pos != MUIV_NList_NextSelected_End ) {
        struct CMUser  *to;
        ULONG           size;

        DoMethod( obj, MUIM_NList_GetEntry, pos, &from );

        switch( from->Type ) {

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
        }

#ifndef NETCONNECT
        if(( RegNumber == 0 ) && ( xget( obj, MUIA_NList_Entries ) >= 10 )) {
            Object *wnd;

            if( wnd = NewObject( CL_AboutWindow->mcc_Class, NULL,
                                 MUIA_AboutWindow_More,  TRUE,
                                 MUIA_AboutWindow_Limit, TRUE,
                                 TAG_DONE )) {

                DoMethod( _app( obj ), OM_ADDMEMBER, wnd );

                set( wnd, MUIA_Window_Open, TRUE );
            }

            break;

        } else
#endif
        if( to = CM_AllocEntry( from->Type )) {

            CopyMem( from, to, size );

            AddTail(( struct List * )&toadd, ( struct Node * )to );
        }

        DoMethod( obj, MUIM_NList_NextSelected, &pos );

        Flags &= ~SAVED;
    }

    while( from = (struct CMUser *) RemHead(( struct List * )&toadd ))
        DoMethod( obj, MUIM_NList_InsertSingle, from, MUIV_NList_Insert_Active );

    return( 0 );
}
///
/// EntryList_DoubleClick
static ULONG EntryList_DoubleClick( struct IClass *cl, Object *obj, Msg msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );

    if( AcceptEntry & CMGE_MULTISELECT ) {
        struct MinList *list;

        if( list = CM_AllocEntry( CME_LIST )) {
            LONG    pos;

            pos = MUIV_NList_NextSelected_Start;

            DoMethod( obj, MUIM_NList_Select, MUIV_NList_Select_Active, MUIV_NList_Select_On, NULL );

            DoMethod( obj, MUIM_NList_NextSelected, &pos );

            while( pos != MUIV_NList_NextSelected_End ) {
                struct CMUser  *from, *to;
                ULONG           mask, size;

                DoMethod( obj, MUIM_NList_GetEntry, pos, &from );

                switch( from->Type ) {

                    case CME_USER:
                        mask = CMGE_USER;
                        size = sizeof( struct CMUser );
                        break;

                    case CME_WWW:
                        mask = CMGE_WWW;
                        size = sizeof( struct CMWWW );
                        break;

                    case CME_FTP:
                        mask = CMGE_FTP;
                        size = sizeof( struct CMFTP );
                        break;

                    case CME_CHAT:
                        mask = CMGE_CHAT;
                        size = sizeof( struct CMChat );
                        break;
                }

                if(!( AcceptEntry & mask )) {
                    CM_FreeList( list );
                    return( 0 );
                }

                if( to = CM_AllocEntry( from->Type )) {

                    CopyMem( from, to, size );

                    AddTail(( struct List * )list, ( struct Node * )to );
                }

                DoMethod( obj, MUIM_NList_NextSelected, &pos );
            }

            SendLibMsg( CMC_GET_ENTRY, list );

            AcceptEntry = 0;
        }

    } else {
        struct CMUser  *entry;

        DoMethod( obj, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &entry );

        if( entry ) {

            if( AcceptEntry ) {
                ULONG   mask = 0, size;

                switch( entry->Type ) {

                    case CME_USER:
                        mask = CMGE_USER;
                        size = sizeof( struct CMUser );
                        break;

                    case CME_WWW:
                        mask = CMGE_WWW;
                        size = sizeof( struct CMWWW );
                        break;

                    case CME_FTP:
                        mask = CMGE_FTP;
                        size = sizeof( struct CMFTP );
                        break;

                    case CME_CHAT:
                        mask = CMGE_CHAT;
                        size = sizeof( struct CMChat );
                        break;
                }

                if( AcceptEntry & mask ) {
                    struct CMUser  *newentry;

                    if( newentry = CM_AllocEntry( entry->Type ))
                        CopyMem( entry, newentry, size );

                    SendLibMsg( CMC_GET_ENTRY, newentry );

                    AcceptEntry = 0;
                }

            } else switch( Action ) {

                case ACTION_EDIT:
                    DoMethod( _app( obj ), MUIM_Application_PushMethod, obj, 1, MUIM_EntryList_Edit );
                    break;

                case ACTION_SCRIPT:
                case ACTION_SCRIPT_CLOSE:
                case ACTION_SCRIPT_ICONIFY: {
                    struct CMUser  *entry;

                    DoMethod( obj, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &entry );

                    if( entry ) {
                        Object *app;

                        app = _app( obj );

                        if(( entry->Type == CME_WWW ) || ( entry->Type == CME_FTP )) {

                            DoMethod( app, MUIM_Application_PushMethod, obj, 2, MUIM_EntryList_StartScript, ( entry->Type == CME_WWW ) ? SCP_WWW :
                                                                                                            ( UseDOpus ? SCP_DOPUSFTP : SCP_FTP ));

                            switch( Action ) {

                                case ACTION_SCRIPT_CLOSE:
                                    DoMethod( app, MUIM_Application_PushMethod, _win( obj ), 2, MUIM_MainWindow_Close, FALSE );
                                    break;

                                case ACTION_SCRIPT_ICONIFY:
                                    set( app, MUIA_Application_Iconified, TRUE );
                                    break;
                            }

                        } else
                            DoMethod( app, MUIM_Application_PushMethod, obj, 1, MUIM_EntryList_Edit );
                    }
                }   break;
            }
        }
    }

    return( 0 );
}
///
/// EntryList_StartScript
static ULONG EntryList_StartScript( struct IClass *cl, Object *obj, struct MUIP_EntryList_StartScript *msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );
    struct CMUser          *entry;

    DoMethod( obj, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &entry );

    if( entry ) {
        TEXT            Command[1024];
        BPTR            lock;
        STRPTR          MyPort;
        struct RexxMsg *rxmsg;
        ULONG           script;

        Command[0] = '\0';

        if( lock = Lock( "PROGDIR:", ACCESS_READ )) {

            NameFromLock( lock, Command, sizeof( Command ));

            UnLock( lock );
        }

        switch( msg->Script ) {

            case SCP_BUSINESS_FAX:
                script = SCP_FAX;
                break;

            case SCP_BUSINESS_PHONE:
                script = SCP_PHONE;
                break;

            case SCP_BUSINESS_EMAIL:
                script = SCP_EMAIL;
                break;

            default:
                script = msg->Script;
                break;
        }

        if(( script != SCP_PHONE ) && ( script != SCP_MOBILE )) {

            AddPart( Command, "Rexx", sizeof( Command ));
            AddPart( Command, Scripts[ script ], sizeof( Command ));

            if( msg->Script != SCP_DOPUSFTP )
                strcat( Command, " \"" );
        }

        switch( entry->Type ) {

            case CME_USER:
                switch( msg->Script ) {

                    case SCP_WWW:
                        strcat( Command, entry->WWW );
                        strcat( Command, "\" \"" );
                        strcat( Command, WWWPath );
                        break;

                    case SCP_FTP:
                        strcat( Command, entry->FTP );
                        strcat( Command, "\" \"" );
                        strcat( Command, FTPPath );
                        break;

                    case SCP_DOPUSFTP:
                        strcat( Command, " " );
                        strcat( Command, UseLister ? "1" : "0" );

                        if( UseLister ) {
                            strcat( Command, " " );
                            strcat( Command, Lister );
                        }
                        break;

                    case SCP_EMAIL:
                    case SCP_BUSINESS_EMAIL:
                        strcat( Command, ( msg->Script == SCP_EMAIL ) ? entry->EMail : entry->BEMail );
                        strcat( Command, "\" \"" );
                        strcat( Command, MailPath );
                        break;

                    case SCP_PHONE:
                    case SCP_MOBILE:
                    case SCP_BUSINESS_PHONE: {
                        STRPTR  phone;

                        switch( msg->Script ) {

                            case SCP_PHONE:
                                phone = entry->Phone;
                                break;

                            case SCP_MOBILE:
                                phone = entry->Mobile;
                                break;

                            case SCP_BUSINESS_PHONE:
                                phone = entry->BPhone;
                                break;
                        }

                        if( UseDialler ) {

                            DoMethod( _win( obj ), MUIM_MainWindow_Dial, phone );

                            return( 0 );

                        } else {

                            sprintf( Command, DialProg, phone );

                            SystemTags( Command,
                                        SYS_Asynch,   TRUE,
                                        SYS_Input,    Open( "NIL:", MODE_NEWFILE ),
                                        SYS_Output,   Open( "NIL:", MODE_NEWFILE ),
                                        TAG_END );

                            return( 0 );
                        }
                    }   break;

                    case SCP_FAX:
                        strcat( Command, entry->Fax );
                        break;

                    case SCP_BUSINESS_FAX:
                        strcat( Command, entry->BFax );
                        break;
                }
                break;

            case CME_WWW:
                if( UseOpenURL && OpenURLBase ) {

                    URL_OpenA((( struct CMWWW * )entry )->WWW, NULL );

                    return( 0 );

                } else {
                    strcat( Command, ((struct CMWWW *)entry )->WWW );
                    strcat( Command, "\" \"" );
                    strcat( Command, WWWPath );
                }
                break;

            case CME_FTP:
                switch( msg->Script ) {

                    case SCP_DOPUSFTP:

                        strcat( Command, " " );

                        if(((struct CMFTP *)entry )->Local[0] && ( entry->Flags & FTPF_LOCAL )) {

                            strcat( Command, "1 " );
                            strcat( Command, ((struct CMFTP *)entry )->Local );

                        } else if( UseLister ) {

                            strcat( Command, "1 " );
                            strcat( Command, Lister );

                        } else
                            strcat( Command, "0" );
                        break;

                    case SCP_FTP:
                        strcat( Command, ((struct CMFTP *)entry )->FTP );
                        strcat( Command, "\" \"" );
                        strcat( Command, FTPPath );
                        break;

                    case SCP_WWW:
                        strcat( Command, ((struct CMFTP *)entry )->FTP );
                        strcat( Command, "\" \"" );
                        strcat( Command, WWWPath );
                        break;
                }
                break;

            case CME_CHAT:
                switch( msg->Script ) {

                    case SCP_IRC:
                        strcat( Command, ((struct CMChat *)entry )->Server );
                        strcat( Command, "\" \"" );
                        strcat( Command, ((struct CMChat *)entry )->Channel );
                        strcat( Command, "\" \"" );
                        strcat( Command, IRCPath );
                        break;

                    case SCP_WWW:
                        strcat( Command, ((struct CMChat *)entry )->WWW );
                        strcat( Command, "\" \"" );
                        strcat( Command, WWWPath );
                        break;
                }
                break;
        }

        if( msg->Script != SCP_DOPUSFTP )
            strcat( Command, "\"" );

        get( _app( obj ), MUIA_Application_Base, &MyPort );

        if( rxmsg = CreateRexxMsg( data->RexxPort, "REXX", MyPort )) {

            if( rxmsg->rm_Args[0] = CreateArgstring( Command, 1 + strlen( Command ))) {
                struct MsgPort *dest_port;

                rxmsg->rm_Action = RXCOMM;

                Forbid();

                if( dest_port = FindPort( "REXX" ))
                    PutMsg( dest_port, (struct Message *)rxmsg );

                Permit();

                if( dest_port )
                    data->RexxOut += 1;
                else {
                    DeleteArgstring( rxmsg->rm_Args[0] );
                    DeleteRexxMsg( rxmsg );
                }
            }
        }
    }

    return( 0 );
}
///
/// EntryList_HandleARexx
static ULONG EntryList_HandleARexx( struct IClass *cl, Object *obj, Msg msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );
    struct RexxMsg         *RxMsg;
    struct CMMessage       *cmmsg;

    while( RxMsg = (struct RexxMsg *) GetMsg( data->RexxPort )) {

        DeleteArgstring( RxMsg->rm_Args[0] );
        DeleteRexxMsg( RxMsg );

        data->RexxOut -= 1;
    }

    while( cmmsg = (struct CMMessage *) GetMsg( MyLibPort )) {

        if( cmmsg->Command == CMC_GET_ENTRY )
            AcceptEntry = cmmsg->Data;

        ReplyMsg(( struct Message * )cmmsg );
    }

    return( 0 );
}
///
/// EntryList_Edit
static ULONG EntryList_Edit( struct IClass *cl, Object *obj, Msg msg )
{
    struct CMUser  *entry;
    LONG            pos;

    pos = MUIV_NList_NextSelected_Start;

    DoMethod( obj, MUIM_NList_NextSelected, &pos );

    while( pos != MUIV_NList_NextSelected_End ) {

        DoMethod( obj, MUIM_NList_GetEntry, pos, &entry );

        if(!( ShowEditWindow( _app( obj ), entry ))) {
            Object *Body;
            ULONG   Title, ID;

            switch( entry->Type ) {

                case CME_USER:
                    Title = WND_EDIT_USER;
                    Body  = NewObject( CL_Edit->mcc_Class, NULL, TAG_DONE );
                    ID    = MAKE_ID( 'E', 'D', 'U', 'S' );
                    break;

                case CME_WWW:
                    Title = WND_EDIT_WWW;
                    Body  = NewObject( CL_EditWWW->mcc_Class, NULL, TAG_DONE );
                    ID    = MAKE_ID( 'E', 'D', 'W', 'W' );
                    break;

                case CME_FTP:
                    Title = WND_EDIT_FTP;
                    Body  = NewObject( CL_EditFTP->mcc_Class, NULL, TAG_DONE );
                    ID    = MAKE_ID( 'E', 'D', 'F', 'T' );
                    break;

                case CME_CHAT:
                    Title = WND_EDIT_CHAT;
                    Body  = NewObject( CL_EditIRC->mcc_Class, NULL, TAG_DONE );
                    ID    = MAKE_ID( 'E', 'D', 'C', 'H' );
                    break;
            }

            if( Body ) {
                Object *wnd;

                set( Body, MUIA_Edit_Entry, entry );

                if( wnd = NewObject( CL_EditWindow->mcc_Class, NULL,
                                     MUIA_EditWindow_Title,     Title,
                                     MUIA_EditWindow_ID,        ID,
                                     MUIA_EditWindow_Object,    Body,
                                     MUIA_EditWindow_List,      obj,
                                     MUIA_EditWindow_Virtual,   entry->Type == CME_USER,
                                     TAG_DONE )) {

                    DoMethod( _app( obj ), OM_ADDMEMBER, wnd );

                    DoMethod( wnd, MUIM_EditWindow_Open, entry );
                }
            }
        }

        DoMethod( obj, MUIM_NList_NextSelected, &pos );
    }

    return( 0 );
}
///
/// EntryList_CopyToClip
static ULONG EntryList_CopyToClip( struct IClass *cl, Object *obj, Msg msg )
{
    struct EntryListData   *data = INST_DATA( cl, obj );
    struct CMUser          *entry;

    DoMethod( obj, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &entry );

    if( entry ) {
        TEXT                string[1024];
        struct IFFHandle   *iff;

        switch( entry->Type ) {

            case CME_WWW:
                strcpy( string, ((struct CMWWW *)entry )->WWW );
                break;

            case CME_FTP:
                strcpy( string, ((struct CMFTP *)entry )->FTP );
                break;

            default:
                return( 0 );
        }

        if( iff = AllocIFF() ) {

            if( iff->iff_Stream = (ULONG) OpenClipboard( 0 )) {

                InitIFFasClip( iff );

                if(!( OpenIFF( iff, IFFF_WRITE ))) {

                    PushChunk( iff, ID_FTXT, ID_FORM, IFFSIZE_UNKNOWN );
                    PushChunk( iff, 0,       ID_CHRS, IFFSIZE_UNKNOWN );

                    WriteChunkBytes( iff, string, strlen( string ));

                    PopChunk( iff );
                    PopChunk( iff );

                    CloseIFF( iff );
                }

                CloseClipboard(( struct ClipboardHandle * )iff->iff_Stream );
            }

            FreeIFF( iff );
        }
    }

    return( 0 );
}
///


/// EditWindow_Dispatcher
ULONG SAVEDS ASM EditWindow_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( EditWindow_New( cl, obj, (APTR)msg ));

        case OM_GET:
            return( EditWindow_Get( cl, obj, (APTR)msg ));

        case MUIM_EditWindow_Open:
            return( EditWindow_Open( cl, obj, (APTR)msg ));

        case MUIM_EditWindow_Close:
            return( EditWindow_Close( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// EditWindow_New
static ULONG EditWindow_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditWindowData   temp = { 0 };
    Object                 *Ok, *Cancel, *contents;
    struct TagItem         *tag;
    ULONG                   title, ID;
    BOOL                    virt;

    if( tag = FindTagItem( MUIA_EditWindow_Object, msg->ops_AttrList ))
        temp.Body = (Object *) tag->ti_Data;
    else
        return( 0 );

    title = GetTagData( MUIA_EditWindow_Title, 0, msg->ops_AttrList );
    ID    = GetTagData( MUIA_EditWindow_ID,    0, msg->ops_AttrList );

    if( tag = FindTagItem( MUIA_EditWindow_List, msg->ops_AttrList ))
        temp.List = (Object *) tag->ti_Data;
    else
        return( 0 );

    contents = temp.Body;

    if( virt = GetTagData( MUIA_EditWindow_Virtual, FALSE, msg->ops_AttrList )) {

        contents = ScrollgroupObject,
                        MUIA_Scrollgroup_UseWinBorder,  TRUE,
                        MUIA_Scrollgroup_Contents,      VirtgroupObject,
                                                            Child, contents,
                                                        End,
                   End;
    }

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            MUIA_Window_Title,                      CatCompArray[ title ].cca_Str,
                            MUIA_Window_ID,                         ID,
                            MUIA_Window_UseRightBorderScroller,     virt,
                            MUIA_Window_UseBottomBorderScroller,    virt,
                            PubScreen[0] ? MUIA_Window_PublicScreen : TAG_IGNORE, PubScreen,

                            WindowContents, VGroup,

                                Child, contents,

                                Child, HGroup, MUIA_Group_SameSize, TRUE,
                                    Child, Ok = MakeButton( GAD_OK, -1 ),
                                    Child, HSpace( 0 ),
                                    Child, Cancel = MakeButton( GAD_CANCEL, -1 ),
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct EditWindowData  *data = INST_DATA( cl, obj );

        *data = temp;

        set( obj, MUIA_Window_ActiveObject, Ok );

        DoMethod( obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_EditWindow_Close, FALSE );

        /*  Gadgets */
        DoMethod( Ok, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_EditWindow_Close, TRUE );
        DoMethod( Cancel, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_EditWindow_Close, FALSE );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// EditWindow_Get
static ULONG EditWindow_Get( struct IClass *cl, Object *obj, struct opGet *msg )
{
    struct EditWindowData  *data = INST_DATA( cl, obj );

    switch ( msg->opg_AttrID ) {

        case MUIA_EditWindow_Entry:
            *msg->opg_Storage = xget( data->Body, MUIA_Edit_Entry );
            return( TRUE );
    }

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// EditWindow_Open
static ULONG EditWindow_Open( struct IClass *cl, Object *obj, struct MUIP_EditWindow_Open *msg )
{
    struct EditWindowData  *data = INST_DATA( cl, obj );

    set( data->Body, MUIA_Edit_Entry, msg->Entry );

    set( obj, MUIA_Window_Open, TRUE );

    return( 0 );
}
///
/// EditWindow_Close
static ULONG EditWindow_Close( struct IClass *cl, Object *obj, struct MUIP_EditWindow_Close *msg )
{
    struct EditWindowData  *data = INST_DATA( cl, obj );
    struct CMUser          *entry;

    get( data->Body, MUIA_Edit_Entry, &entry );

    if( msg->Ok ) {
        ULONG   i;

        DoMethod( data->Body, MUIM_Edit_Update );

        for( i = 0;; i++ ) {
            struct CMUser  *e;

            DoMethod( data->List, MUIM_NList_GetEntry, i, &e );

            if( e == NULL )
                break;

            if( e == entry ) {
                DoMethod( data->List, MUIM_NList_Redraw, i );
                break;
            }
        }

        entry->Flags &= ~ENTRY_NEW;

        Flags &= ~SAVED;

    } else if( entry->Flags & ENTRY_NEW ) {
        ULONG   i;
        BOOL    found = FALSE;

        for( i = 0;; i++ ) {
            struct CMUser  *e;

            DoMethod( data->List, MUIM_NList_GetEntry, i, &e );

            if( e == NULL )
                break;

            if( e == entry ) {
                DoMethod( data->List, MUIM_NList_Remove, i );
                found = TRUE;
                break;
            }
        }

        if(!( found ))  // it's not displayed now, so it's linked in a list
            Remove(( struct Node * )entry );

        CM_FreeEntry( entry );
    }

    RemoveWindow( obj );

    return( 0 );
}
///

/// ShowEditWindow
static BOOL ShowEditWindow( Object *AppObj, APTR Entry )
{
    struct List    *list;
    Object         *anchor, *wnd;

    get( AppObj, MUIA_Application_WindowList, &list );

    anchor = (Object *) list->lh_Head;

    while( wnd = NextObject( &anchor ))
        if(( APTR )xget( wnd, MUIA_EditWindow_Entry ) == Entry ) {

            DoMethod( wnd, MUIM_Window_ToFront );

            set( wnd, MUIA_Window_Activate, TRUE );

            return( TRUE );
        }

    return( FALSE );
}
///
/// CloseEditWindow
static void CloseEditWindow( Object *AppObj, APTR Entry )
{
    struct List    *list;
    Object         *anchor, *wnd;

    get( AppObj, MUIA_Application_WindowList, &list );

    anchor = (Object *) list->lh_Head;

    while( wnd = NextObject( &anchor ))
        if(( APTR )xget( wnd, MUIA_EditWindow_Entry ) == Entry ) {
            DoMethod( wnd, MUIM_EditWindow_Close, FALSE );
            return;
        }
}
///
