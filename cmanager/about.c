/***************************************************************************
    revision             : $Id: about.c,v 1.1.1.1 2003-01-06 15:50:47 tellini Exp $
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
#include <dos/dostags.h>                // dos
#include <intuition/intuition.h>        // intuition
#include <libraries/mui.h>              // libraries
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/utility_protos.h>
#include <clib/dos_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/utility_pragmas.h>
#include <pragmas/dos_pragmas.h>

#ifdef NETCONNECT
#include <clib/nc_protos.h>
#include <pragmas/nc_pragmas.h>
#endif

#include <mymacros.h>

#include "about.h"
#include "mui.h"
#include "CManager.h"
#ifndef CMANAGER_MCC
#include "CManager_rev.h"
#else
#include "MCC/CManager_mcc.h"
#endif
#include "locale.h"

#include "Library/CManager_protos.h"
#include "Library/CManager_lib_lib.h"
///
/// Prototypes
static ULONG AboutWindow_New( struct IClass *, Object *, struct opSet * );
static ULONG AboutWindow_Close( struct IClass *, Object *, struct MUIP_AboutWindow_Close * );

extern unsigned int VersionNumber, RevisionNumber;
///
/// Data
struct MUI_CustomClass *CL_AboutWindow;
///

/// AboutWindow_Dispatcher
ULONG SAVEDS ASM AboutWindow_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( AboutWindow_New( cl, obj, (APTR)msg ));

        case MUIM_AboutWindow_Close:
            return( AboutWindow_Close( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// AboutWindow_New
static ULONG AboutWindow_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    Object *Ok;
    TEXT    body[1024];

    sprintf( body, CM_GetString( MSG_ABOUT ), VersionNumber, RevisionNumber );

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            MUIA_Window_Title,      CM_GetString( MENU_ABOUT ),
                            MUIA_Window_ID,         MAKE_ID( 'A', 'B', 'O', 'T' ),
                            MUIA_Window_LeftEdge,   MUIV_Window_LeftEdge_Centered,
                            MUIA_Window_TopEdge,    MUIV_Window_TopEdge_Centered,
                            MUIA_Window_NoMenus,    TRUE,
#ifndef CMANAGER_MCC
                            PubScreen[0] ? MUIA_Window_PublicScreen : TAG_IGNORE, PubScreen,
#endif

                            WindowContents, VGroup,

                                Child, TextObject,
                                    TextFrame,
                                    InnerSpacing( 8, 8 ),
                                    MUIA_Background,        MUII_TextBack,
                                    MUIA_Text_PreParse,     "\033c",
                                    MUIA_Text_Contents,     body,
                                End,

                                Child, HGroup,
                                    Child, Ok = MakeButton( GAD_OK, -1 ),
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {
#ifdef CMANAGER_MCC
        struct AboutData   *data = INST_DATA( cl, obj );
#endif

        DoMethod( obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_AboutWindow_Close, FALSE );

#ifdef CMANAGER_MCC
        data->CMObj = (Object *)GetTagData( MUIA_AboutWindow_CMObj, NULL, msg->ops_AttrList );
#endif

        set( obj, MUIA_Window_ActiveObject, Ok );

        /*  Gadgets */
        DoMethod( Ok, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_AboutWindow_Close, FALSE );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// AboutWindow_Close
static ULONG AboutWindow_Close( struct IClass *cl, Object *obj, struct MUIP_AboutWindow_Close *msg )
{
#ifdef CMANAGER_MCC
    struct AboutData   *data = INST_DATA( cl, obj );
    Object             *AppObj = _app( obj );
#endif

    set( obj, MUIA_Window_Open, FALSE );

    if( msg->More ) {
        Object *wnd;

        if( wnd = NewObject( CL_AboutWindow->mcc_Class, NULL,
                             MUIA_AboutWindow_More, TRUE,
                             TAG_DONE )) {

            DoMethod( _app( obj ), OM_ADDMEMBER, wnd );

            set( wnd, MUIA_Window_Open, TRUE );
        }
    }

#ifdef CMANAGER_MCC

    set( obj, MUIA_Window_Open, FALSE );

    DoMethod( AppObj, OM_REMMEMBER, obj );

    DoMethod( AppObj, MUIM_Application_PushMethod, data->CMObj, 2, MUIM_CManager_DisposeObj, obj );

#else

    RemoveWindow( obj );

#endif

    return( 0 );
}
///
