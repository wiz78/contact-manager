/***************************************************************************
    revision             : $Id: search.c,v 1.1.1.1 2003-01-06 16:00:56 tellini Exp $
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
#include <exec/memory.h>
#include <intuition/intuition.h>        // intuition
#include <dos/dos.h>                    // dos
#include <mui/nlistview_mcc.h>          // mui
#include <mui/nlist_mcc.h>
#include <mui/listtree_mcc.h>
#include <libraries/mui.h>              // libraries
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/utility_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/utility_pragmas.h>

#include <mymacros.h>

#include "/CManager.h"
#include "mui.h"
#include "search.h"
#include "/locale.h"

#include "CManager_mcc.h"
#include "CManager_private.h"

#include "/Library/CManager_protos.h"
#include "/Library/CManager_lib_lib.h"
///
/// Prototypes
#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d)    ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

enum { SEARCH_CURRENT = 0, SEARCH_TOP, SEARCH_BACKWARDS };

struct EntryStore {
        struct EntryStore  *Succ, *Pred;
        struct CMGroup     *Entry;
};

static ULONG SearchWindow_New( struct IClass *, Object *, struct opSet * );
static ULONG SearchWindow_Open( struct IClass *, Object *, struct MUIP_SearchWindow_Open * );
static ULONG SearchWindow_Close( struct IClass *, Object *, Msg );
static ULONG SearchWindow_Search( struct IClass *, Object *, struct MUIP_SearchWindow_Search * );

static BOOL MatchStr( STRPTR, STRPTR );
static BOOL Search( ULONG, STRPTR, struct EntryStore **, struct CMUser ** );

static struct MUIS_Listtree_TreeNode *FindGroup( Object *, struct CMGroup *, APTR );
static ULONG FindItem( Object *, APTR );
static void AddToList( Object *, struct MinList *, APTR, ULONG, struct CMData * );
///
/// Data
struct MUI_CustomClass *CL_SearchWindow;
///


/// CManager_Search
ULONG CManager_Search( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    Object                 *wnd;

    if( wnd = NewObject( CL_SearchWindow->mcc_Class, 0, TAG_DONE )) {

        DoMethod(( Object * )xget( obj, MUIA_ApplicationObject ), OM_ADDMEMBER, wnd );

        DoMethod( wnd, MUIM_SearchWindow_Open, obj );
    }

    return( 0 );
}
///

/// SearchWindow_Dispatcher
ULONG SAVEDS ASM SearchWindow_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( SearchWindow_New( cl, obj, (APTR)msg ));

        case MUIM_SearchWindow_Open:
            return( SearchWindow_Open( cl, obj, (APTR)msg ));

        case MUIM_SearchWindow_Close:
            return( SearchWindow_Close( cl, obj, (APTR)msg ));

        case MUIM_SearchWindow_Search:
            return( SearchWindow_Search( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// SearchWindow_New
static ULONG SearchWindow_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct SearchWindowData     temp = { 0 };
    Object                     *Search, *Top, *Back, *Cancel;

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            MUIA_Window_Title,      CM_GetString( MENU_SEARCH ),
                            MUIA_Window_ID,         MAKE_ID( 'S', 'R', 'C', 'H' ),

                            WindowContents, VGroup,

                                Child, HGroup,
                                    Child, MakeLabel1( GAD_SEARCH_FOR ),
                                    Child, temp.String = MakeString( 128, GAD_SEARCH_FOR, -1 ),
                                End,

                                Child, HGroup,
                                    Child, MakeLabel1( MSG_USERS ),
                                    Child, temp.Users = MakeCheck( MSG_USERS, -1 ),
                                    Child, HSpace( 0 ),
                                    Child, MakeLabel1( MSG_WWW ),
                                    Child, temp.WWW = MakeCheck( MSG_WWW, -1 ),
                                    Child, HSpace( 0 ),
                                    Child, MakeLabel1( MSG_FTP ),
                                    Child, temp.FTP = MakeCheck( MSG_FTP, -1 ),
                                    Child, HSpace( 0 ),
                                    Child, MakeLabel1( MSG_CHAT ),
                                    Child, temp.Chat = MakeCheck( MSG_CHAT, -1 ),
                                End,

                                Child, HGroup,
                                    Child, Search = MakeButton( GAD_SEARCH, -1 ),
                                    Child, Top    = MakeButton( GAD_FROM_TOP, -1 ),
                                    Child, Back   = MakeButton( GAD_BACKWARDS, -1 ),
                                    Child, Cancel = MakeButton( GAD_CANCEL, -1 ),
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct SearchWindowData  *data = INST_DATA( cl, obj );

        *data = temp;

        set( obj, MUIA_Window_ActiveObject, temp.String );

        set( temp.Users, MUIA_Selected, TRUE );
        set( temp.WWW,   MUIA_Selected, TRUE );
        set( temp.FTP,   MUIA_Selected, TRUE );

        DoMethod( obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 1, MUIM_SearchWindow_Close );

        /*  Gadgets */
        DoMethod( Search, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_SearchWindow_Search, SEARCH_CURRENT   );
        DoMethod( Top,    MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_SearchWindow_Search, SEARCH_TOP       );
        DoMethod( Back,   MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_SearchWindow_Search, SEARCH_BACKWARDS );

        DoMethod( Cancel, MUIM_Notify, MUIA_Pressed, FALSE, obj, 1, MUIM_SearchWindow_Close );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// SearchWindow_Open
static ULONG SearchWindow_Open( struct IClass *cl, Object *obj, struct MUIP_SearchWindow_Open *msg )
{
    struct SearchWindowData    *data = INST_DATA( cl, obj );

    data->CMObj = msg->CMObj;

    set( obj, MUIA_Window_Open, TRUE );

    return( 0 );
}
///
/// SearchWindow_Close
static ULONG SearchWindow_Close( struct IClass *cl, Object *obj, Msg msg )
{
    struct SearchWindowData    *data = INST_DATA( cl, obj );
    Object                     *AppObj;

    set( obj, MUIA_Window_Open, FALSE );

    get( obj, MUIA_ApplicationObject, &AppObj );

    DoMethod( AppObj, OM_REMMEMBER, obj );

    DoMethod( AppObj, MUIM_Application_PushMethod, data->CMObj, 2, MUIM_CManager_DisposeObj, obj );

    return( 0 );
}
///
/// SearchWindow_Search
static ULONG SearchWindow_Search( struct IClass *cl, Object *obj, struct MUIP_SearchWindow_Search *msg )
{
    struct SearchWindowData        *data = INST_DATA( cl, obj );
    struct MUIS_Listtree_TreeNode  *tn;
    struct CMUser                  *entry = NULL;
    TEXT                            String[128];
    UBYTE                          *ptr, which;
    struct EntryStore              *start;
    struct MinList                  groups;
    Object                         *Tree, *List;

    which = 0;

    if( xget( data->Users, MUIA_Selected ))
        which |= CMGE_USER;

    if( xget( data->WWW, MUIA_Selected ))
        which |= CMGE_WWW;

    if( xget( data->FTP, MUIA_Selected ))
        which |= CMGE_FTP;

    if( xget( data->Chat, MUIA_Selected ))
        which |= CMGE_CHAT;

    if(!( which ))
        return( 0 );

    DoMethod( data->CMObj, MUIM_CManager_GrabLists );

    get( data->CMObj, MUIA_CManager_TreeObject, &Tree );
    get( data->CMObj, MUIA_CManager_ListObject, &List );

    NewList(( struct List * )&groups );

    AddToList( Tree, &groups, NULL, which, (struct CMData *)xget( data->CMObj, MUIA_CManager_CMData ));

    if(( msg->Type == SEARCH_CURRENT ) || ( msg->Type == SEARCH_BACKWARDS )) {
        BOOL    getcurrent = FALSE;

        start = ( msg->Type == SEARCH_CURRENT ) ?
                (struct EntryStore *) groups.mlh_Head : (struct EntryStore *) groups.mlh_TailPred;

        if( tn = (struct MUIS_Listtree_TreeNode *)
                    DoMethod( Tree, MUIM_Listtree_GetEntry,
                              MUIV_Listtree_GetEntry_ListNode_Active,
                              MUIV_Listtree_GetEntry_Position_Active, 0 )) {

            if( msg->Type == SEARCH_CURRENT ) {
                struct EntryStore  *tmp;

                for( tmp = start; tmp->Succ; tmp = tmp->Succ )
                    if( tmp->Entry == (struct CMGroup *) tn->tn_User ) {
                        start      = tmp;
                        getcurrent = TRUE;
                        break;
                    }

            } else {
                struct EntryStore  *tmp;

                for( tmp = start; tmp->Pred; tmp = tmp->Pred )
                    if( tmp->Entry == (struct CMGroup *) tn->tn_User ) {
                        start      = tmp;
                        getcurrent = TRUE;
                        break;
                    }
            }
        }

        if( getcurrent ) {

            DoMethod( List, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &entry );

            if( msg->Type == SEARCH_BACKWARDS ) {

                if( entry )
                    entry = entry->Prec;

                if( !entry || !entry->Prec )
                    if( start != (struct EntryStore *) groups.mlh_Head ) {
                        start = start->Pred;
                        entry = (struct CMUser *) start->Entry->Entries.mlh_TailPred;
                    }

            } else {

                if( entry )
                    entry = entry->Succ;

                if( !entry || !entry->Succ )
                    if( start->Succ->Succ ) {
                        start = start->Succ;
                        entry = (struct CMUser *) start->Entry->Entries.mlh_Head;
                    }
            }

        } else {

            if( msg->Type == SEARCH_BACKWARDS )
                entry = (struct CMUser *) start->Entry->Entries.mlh_TailPred;
            else
                entry = (struct CMUser *) start->Entry->Entries.mlh_Head;
        }
    }

    if(( msg->Type == SEARCH_TOP ) || ( start == NULL )) {

        start = (struct EntryStore *) groups.mlh_Head;

        if( msg->Type == SEARCH_BACKWARDS )
            entry = (struct CMUser *) start->Entry->Entries.mlh_TailPred;
        else
            entry = (struct CMUser *) start->Entry->Entries.mlh_Head;
    }

    getstr( data->String, String );

    ptr = String;

    while( *ptr )
        *ptr++ = ToLower( *ptr );

    if( Search( msg->Type, String, &start, &entry )) {
        struct MUIS_Listtree_TreeNode  *tn, *tn2;

        tn2 = tn = FindGroup( Tree, start->Entry, NULL );

        while( tn2 = (struct MUIS_Listtree_TreeNode *)
                        DoMethod( Tree, MUIM_Listtree_GetEntry,
                                  tn2,
                                  MUIV_Listtree_GetEntry_Position_Parent, 0 ))
            DoMethod( Tree, MUIM_Listtree_Open, tn2, tn2, 0 );

        set( Tree, MUIA_Listtree_Active, tn );

        set( List, MUIA_NList_Active, FindItem( List, entry ));
    }

    while( start = (struct EntryStore *) RemTail(( struct List * )&groups ))
        FreeMem( start, sizeof( struct EntryStore ));

    return( 0 );
}
///

/// MatchStr
static BOOL MatchStr( STRPTR String, STRPTR Pattern )
{
    UBYTE  *ptr;
    TEXT    buffer[256];

    ptr = buffer;

    while( *String )
        *ptr++ = ToLower( *String++ );

    *ptr = '\0';

    if( strstr( buffer, Pattern ))
        return( TRUE );

    return( FALSE );
}
///
/// Search
static BOOL Search( ULONG Type, STRPTR String, struct EntryStore **StartGroup, struct CMUser **StartEntry )
{
    struct EntryStore  *group;
    struct CMUser      *entry;

    group = *StartGroup;
    entry = *StartEntry;

    while(( Type == SEARCH_BACKWARDS ) ? group->Pred : group->Succ ) {

        if( entry ) for( ; ( Type == SEARCH_BACKWARDS ) ? entry->Prec : entry->Succ; entry = ( Type == SEARCH_BACKWARDS ) ? entry->Prec : entry->Succ ) {
            BOOL    found = FALSE;

            switch( entry->Type ) {

                case CME_USER:

                    if( MatchStr( entry->Name, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->Address, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->City, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->Country, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->ZIP, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->Phone, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->Fax, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->EMail, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->WWW, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->FTP, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr( entry->Comment, String ))
                        found = TRUE;
                    break;

                case CME_WWW:

                    if( MatchStr((( struct CMWWW * )entry )->Name, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMWWW * )entry )->WWW, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMWWW * )entry )->Comment, String ))
                        found = TRUE;

                    break;

                case CME_FTP:

                    if( MatchStr((( struct CMFTP * )entry )->Name, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMFTP * )entry )->FTP, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMFTP * )entry )->Comment, String ))
                        found = TRUE;

                    break;

                case CME_CHAT:

                    if( MatchStr((( struct CMChat * )entry )->Channel, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMChat * )entry )->Server, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMChat * )entry )->Maintainer, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMChat * )entry )->Nick, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMChat * )entry )->WWW, String )) {
                        found = TRUE;
                        break;
                    }

                    if( MatchStr((( struct CMChat * )entry )->Comment, String ))
                        found = TRUE;

                    break;
            }

            if( found ) {

                *StartGroup = group;
                *StartEntry = entry;

                return( TRUE );
            }
        }

        group = ( Type == SEARCH_BACKWARDS ) ? group->Pred : group->Succ;

        if(( Type == SEARCH_BACKWARDS ) ? group->Pred : group->Succ )
            if( Type == SEARCH_BACKWARDS )
                entry = (struct CMUser *) group->Entry->Entries.mlh_TailPred;
            else
                entry = (struct CMUser *) group->Entry->Entries.mlh_Head;
    }

    return( FALSE );
}
///

/// FindGroup
static struct MUIS_Listtree_TreeNode *FindGroup( Object *Tree, struct CMGroup *Group, APTR Node )
{
    struct MUIS_Listtree_TreeNode  *tn;
    ULONG                           i = 0;

    while( tn = (struct MUIS_Listtree_TreeNode *)
           DoMethod( Tree, MUIM_Listtree_GetEntry, Node, i++, MUIV_Listtree_GetEntry_Flags_SameLevel )) {

        struct MUIS_Listtree_TreeNode  *tn2;
        ULONG                           i2 = 0;

        if(( struct CMGroup * )tn->tn_User == Group )
            return( tn );

        while( tn2 = (struct MUIS_Listtree_TreeNode *)DoMethod( Tree, MUIM_Listtree_GetEntry, tn, i2++, MUIV_Listtree_GetEntry_Flags_SameLevel )) {
            struct MUIS_Listtree_TreeNode  *ret;
            struct CMGroup                 *group2;

            group2 = (struct CMGroup *) tn2->tn_User;

            if( group2 == Group )
                return( tn2 );

            if( ret = FindGroup( Tree, Group, tn2 ))
                return( ret );
        }
    }
}
///
/// FindItem
static ULONG FindItem( Object *List, APTR Entry )
{
    ULONG   i;

    for( i = 0;; i++ ) {
        struct CMGroup *node;

        DoMethod( List, MUIM_NList_GetEntry, i, &node );

        if( node == Entry )
            break;
    }

    return( i );
}
///
/// AddToList
static void AddToList( Object *Tree, struct MinList *List, APTR Node, ULONG Flags, struct CMData *cm )
{
    struct MUIS_Listtree_TreeNode  *tn;
    ULONG                           i = 0;

    while( tn = (struct MUIS_Listtree_TreeNode *)
           DoMethod( Tree, MUIM_Listtree_GetEntry, Node, i++, MUIV_Listtree_GetEntry_Flags_SameLevel )) {

        struct MUIS_Listtree_TreeNode  *tn2;
        struct EntryStore              *entry;
        ULONG                           i2 = 0;

        if((( APTR )tn->tn_User == cm->Users ) || (( APTR )tn->tn_User == cm->WWWs  ) || (( APTR )tn->tn_User == cm->FTPs  ) || (( APTR )tn->tn_User == cm->Chat ))
            if(!((( Flags & CMGE_USER ) && (( APTR )tn->tn_User == cm->Users )) ||
                 (( Flags & CMGE_WWW  ) && (( APTR )tn->tn_User == cm->WWWs  )) ||
                 (( Flags & CMGE_FTP  ) && (( APTR )tn->tn_User == cm->FTPs  )) ||
                 (( Flags & CMGE_CHAT ) && (( APTR )tn->tn_User == cm->Chat  ))))
                continue;

        if(!( entry = AllocMem( sizeof( struct EntryStore ), MEMF_ANY )))
            break;

        entry->Entry = (struct CMGroup *) tn->tn_User;

        AddTail(( struct List * )List, ( struct Node * )entry );

        while( tn2 = (struct MUIS_Listtree_TreeNode *)DoMethod( Tree, MUIM_Listtree_GetEntry, tn, i2++, MUIV_Listtree_GetEntry_Flags_SameLevel )) {
            struct EntryStore  *entry2;

            if( entry2 = AllocMem( sizeof( struct EntryStore ), MEMF_ANY )) {

                entry2->Entry = (struct CMGroup *) tn2->tn_User;

                AddTail(( struct List * )List, ( struct Node * )entry2 );

                AddToList( Tree, List, tn2, Flags, cm );
            }
        }
    }
}
///
