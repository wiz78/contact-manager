/***************************************************************************
    revision             : $Id: grouplist.c,v 1.1.1.1 2003-01-06 15:51:48 tellini Exp $
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

#include "CManager.h"
#include "grouplist.h"
#include "mui.h"
///
/// Prototypes
static ULONG GroupList_DragQuery( struct IClass *, Object *, struct MUIP_DragQuery * );
static ULONG GroupList_DragBegin( struct IClass *, Object *, Msg );
static ULONG GroupList_DragDrop( struct IClass *, Object *, struct MUIP_DragDrop * );
static ULONG GroupList_DragReport( struct IClass *, Object *, struct MUIP_DragReport * );

struct CMGroup *GetRoot( Object *, struct MUIS_Listtree_TreeNode * );

extern UBYTE    Flags;
///
/// Data
struct MUI_CustomClass *CL_GroupList;
///

/// GroupList_Dispatcher
ULONG SAVEDS ASM GroupList_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case MUIM_DragQuery:
            return( GroupList_DragQuery( cl, obj, (APTR)msg ));

        case MUIM_DragBegin:
            return( GroupList_DragBegin( cl, obj, (APTR)msg ));

        case MUIM_DragDrop:
            return( GroupList_DragDrop( cl, obj, (APTR)msg ));

        case MUIM_DragReport:
            return( GroupList_DragReport( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///

/// GroupList_DragQuery
static ULONG GroupList_DragQuery( struct IClass *cl, Object *obj, struct MUIP_DragQuery *msg )
{

    if( msg->obj != obj )
        return( MUIV_DragQuery_Accept );

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///
/// GroupList_DragBegin
static ULONG GroupList_DragBegin( struct IClass *cl, Object *obj, Msg msg )
{
    struct GroupListData   *data = INST_DATA( cl, obj );

    data->Node = NULL;

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///
/// GroupList_DragDrop
static ULONG GroupList_DragDrop( struct IClass *cl, Object *obj, struct MUIP_DragDrop *msg )
{
    struct GroupListData   *data = INST_DATA( cl, obj );

    if( msg->obj == obj ) {
        struct MUIS_Listtree_TreeNode  *tn;
        struct CMGroup                 *from, *to;

        tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( obj, MUIM_Listtree_GetEntry,
                          MUIV_Listtree_GetEntry_ListNode_Active,
                          MUIV_Listtree_GetEntry_Position_Active, 0 );

        from = GetRoot( obj, tn );

        if( data->Node == NULL )
            return( 0 );

        to = (struct CMGroup *) data->Node->tn_User;

        if(( to == from ) && ( data->Flags != MUIV_Listtree_TestPos_Result_Flags_Onto ))
            return( 0 );

        if((( to == &Users ) || ( to == &WWWs ) || ( to == &FTPs ) || ( to == &Chat )) &&
            ( data->Flags != MUIV_Listtree_TestPos_Result_Flags_Onto ))
            return( 0 );

        to = GetRoot( obj, data->Node );

        if( from != to )
            return( 0 );

        Flags &= ~SAVED;

    } else {
        UBYTE                           fromtype;
        struct CMGroup                 *to;
        LONG                            pos;
        struct MUIS_Listtree_TreeNode  *tn;

        if( data->Node == NULL )
            return( 0 );

        tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( obj, MUIM_Listtree_GetEntry,
                          MUIV_Listtree_GetEntry_ListNode_Active,
                          MUIV_Listtree_GetEntry_Position_Active, 0 );

        if( tn == data->Node )
            return( 0 );

        to = GetRoot( obj, data->Node );

        if( to == &Users )
            fromtype = CME_USER;

        else if( to == &WWWs )
            fromtype = CME_WWW;

        else if( to == &FTPs )
            fromtype = CME_FTP;

        else if( to == &Chat )
            fromtype = CME_CHAT;

        pos = MUIV_NList_NextSelected_Start;

        DoMethod( msg->obj, MUIM_NList_NextSelected, &pos );

        while( pos != MUIV_NList_NextSelected_End ) {
            struct CMUser  *entry;

            DoMethod( msg->obj, MUIM_NList_GetEntry, pos, &entry );

            if( entry->Type == fromtype ) {

                AddTail(( struct List * )&((struct CMGroup *) data->Node->tn_User )->Entries, ( struct Node * )entry );

            } else
                break;

            DoMethod( msg->obj, MUIM_NList_NextSelected, &pos );
        }

        DoMethod( msg->obj, MUIM_NList_Remove, MUIV_NList_Remove_Selected );

        Flags &= ~SAVED;

        return( 0 );
    }

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///
/// GroupList_DragReport
static ULONG GroupList_DragReport( struct IClass *cl, Object *obj, struct MUIP_DragReport *msg )
{
    struct GroupListData                *data = INST_DATA( cl, obj );
    struct MUIS_Listtree_TestPos_Result result;

    if(!( msg->update ))
        return( MUIV_DragReport_Refresh );

    DoMethod( obj, MUIM_Listtree_TestPos, msg->x, msg->y, &result );

    if( data->Node != result.tpr_TreeNode ) {

        data->Node  = result.tpr_TreeNode;
        data->Flags = result.tpr_Flags;

        if( msg->obj != obj ) {

            DoMethod( obj, MUIM_Listtree_SetDropMark, result.tpr_ListEntry,
                           MUIV_Listtree_SetDropMark_Values_Onto );

            return( 0 );
        }
    }

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///

/// GetRoot
struct CMGroup *GetRoot( Object *Tree, struct MUIS_Listtree_TreeNode *tn )
{
    while(( tn->tn_User != (APTR)&Users ) &&
          ( tn->tn_User != (APTR)&WWWs ) &&
          ( tn->tn_User != (APTR)&FTPs ) &&
          ( tn->tn_User != (APTR)&Chat ))
        if(!( tn = (struct MUIS_Listtree_TreeNode *)
                    DoMethod( Tree, MUIM_Listtree_GetEntry,
                              tn,
                              MUIV_Listtree_GetEntry_Position_Parent, 0 )))
            return( NULL );

    return(( struct CMGroup * )tn->tn_User );
}
///
