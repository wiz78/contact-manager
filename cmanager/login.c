/***************************************************************************
    revision             : $Id: login.c,v 1.1.1.1 2003-01-06 15:53:27 tellini Exp $
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
#include <libraries/mui.h>              // libraries
#include <libraries/genesis.h>
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <proto/genesis.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/genesis_pragmas.h>

#include <mymacros.h>

#include "login.h"
#include "mui.h"
#include "CManager.h"
#include "locale.h"
///
/// Prototypes
/*
static ULONG LoginWindow_New( struct IClass *, Object *, struct opSet * );
static ULONG LoginWindow_Close( struct IClass *, Object *, struct MUIP_LoginWindow_Close * );
static ULONG LoginWindow_Result( struct IClass *, Object *, Msg );

static void SAVEDS ASM User_ObjStrHook( REG( a2 ) Object *, REG( a1 ) Object * );
*/
///
/// Data
struct MUI_CustomClass *CL_LoginWindow;
///



/// GetUser
struct UserData *MyGetUser( Object *AppObj )
{

    return( GetUser( 0 ));
/*
    Object             *wnd;
    struct UserData    *ret = NULL;

    if( wnd = NewObject( CL_LoginWindow->mcc_Class, NULL, TAG_DONE )) {
        ULONG   sigs = 0;

        DoMethod( AppObj, OM_ADDMEMBER, wnd );

        set( wnd, MUIA_Window_Open, TRUE );

        while( DoMethod( AppObj, MUIM_Application_NewInput, &sigs ) != MUIV_Application_ReturnID_Quit ) {
            if( sigs ) {
                ULONG   signals;

                signals = Wait( SIGBREAKF_CTRL_C | sigs );

                if( signals & SIGBREAKF_CTRL_C )
                    break;
            }
        }

        ret = (struct UserData *) DoMethod( wnd, MUIM_LoginWindow_Result );

        RemoveWindow( wnd );
    }

    return( ret );
*/
}
///

#ifdef NOT_USED
/// LoginWindow_Dispatcher
ULONG SAVEDS ASM LoginWindow_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( LoginWindow_New( cl, obj, (APTR)msg ));

        case MUIM_LoginWindow_Close:
            return( LoginWindow_Close( cl, obj, (APTR)msg ));

        case MUIM_LoginWindow_Result:
            return( LoginWindow_Result( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// LoginWindow_New
static ULONG LoginWindow_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct LoginWindowData  temp = { 0 };
    Object                 *Ok, *Cancel, *Pop, *Users;
    static struct Hook      List_ObjStrHook = {
                                { NULL, NULL },
                                (HOOKFUNC)User_ObjStrHook,
                                NULL, NULL
                            };

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            MUIA_Window_Title,      "CManager Login",
                            MUIA_Window_ID,         MAKE_ID( 'L', 'O', 'G', 'I' ),
                            MUIA_Window_LeftEdge,   MUIV_Window_LeftEdge_Centered,
                            MUIA_Window_TopEdge,    MUIV_Window_TopEdge_Centered,
                            MUIA_Window_NoMenus,    TRUE,

                            WindowContents, VGroup,

                                Child, HGroup,
                                    Child, MakeLabel1( GAD_USERNAME ),
                                    Child, Pop = PopobjectObject,
                                            MUIA_Popstring_String,      temp.User = MakeString( 40, GAD_USERNAME, -1 ),
                                            MUIA_Popstring_Button,      PopButton( MUII_PopUp ),
                                            MUIA_Popobject_ObjStrHook,  &List_ObjStrHook,
                                            MUIA_Popobject_Object,      Users = ListviewObject,
                                                    MUIA_Listview_List, ListObject,
                                                        InputListFrame,
                                                        MUIA_List_ConstructHook,    MUIV_List_ConstructHook_String,
                                                        MUIA_List_DestructHook,     MUIV_List_DestructHook_String,
                                                    End,
                                            End,
                                    End,
                                End,

                                Child, HGroup,
                                    Child, Ok     = MakeButton( GAD_OK,     -1 ),
                                    Child, HSpace( 0 ),
                                    Child, Cancel = MakeButton( GAD_CANCEL, -1 ),
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct LoginWindowData *data = INST_DATA( cl, obj );
        ULONG                   i;

        *data = temp;

        for( i = 0;; i++ ) {
            struct UserData    *ud;

            if(!( ud = GetUser( i )))
                break;

            DoMethod( Users, MUIM_List_InsertSingle, ud->ud_Name, MUIV_List_Insert_Bottom );
        }

        if( i == 1 ) {
            struct UserData    *ud;

            ud = GetUser( 0 );

            set( temp.User, MUIA_String_Contents, ud->ud_Name );
        }

        set( obj, MUIA_Window_ActiveObject, temp.User );

        /*  Window  */
        DoMethod( obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_LoginWindow_Close, FALSE );

        /*  Gadgets */
        DoMethod( Users, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, Pop, 2, MUIM_Popstring_Close, TRUE );

        DoMethod( Ok,     MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_LoginWindow_Close, TRUE  );
        DoMethod( Cancel, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_LoginWindow_Close, FALSE );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// LoginWindow_Close
static ULONG LoginWindow_Close( struct IClass *cl, Object *obj, struct MUIP_LoginWindow_Close *msg )
{
    struct LoginWindowData *data = INST_DATA( cl, obj );

    data->Ok = msg->Ok;

    set( obj, MUIA_Window_Open, FALSE );

    DoMethod(( Object * )xget( obj, MUIA_ApplicationObject ), MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit );

    return( 0 );
}
///
/// LoginWindow_Result
static ULONG LoginWindow_Result( struct IClass *cl, Object *obj, Msg msg )
{
    struct LoginWindowData *data = INST_DATA( cl, obj );
    struct UserData        *ud = NULL;

    if( data->Ok )
        ud = GetUserByName(( STRPTR )xget( data->User, MUIA_String_Contents ));

    return(( ULONG )ud );
}
///

/// User_ObjStrHook
static void SAVEDS ASM User_ObjStrHook( REG( a2 ) Object *list, REG( a1 ) Object *str )
{
    STRPTR  entry;

    DoMethod( list, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &entry );

    set( str, MUIA_String_Contents, entry );
}
///
#endif
