/***************************************************************************
    revision             : $Id: MCCTest.c,v 1.1.1.1 2003-01-06 16:00:23 tellini Exp $
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
#include <exec/types.h>             //  exec
#include <exec/libraries.h>
#include <exec/memory.h>
#include <dos/dos.h>                //  dos
#include <libraries/mui.h>          //  libraries
#include <clib/muimaster_protos.h>  //  clib
#include <clib/alib_protos.h>
#include <proto/exec.h>             //  proto
#include <proto/dos.h>
#include <proto/muimaster.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "CManager_mcc.h"
///
/// Data
struct Library *MUIMasterBase;
///

/// main
int main( int argc, char *argv[] )
{
    Object *AppObj, *MainWin, *obj;

    if(!( MUIMasterBase = OpenLibrary( "muimaster.library", MUIMASTER_VMIN )))
        return( 20 );

    if( AppObj = ApplicationObject,

                        MUIA_Application_Title,         "CMTest",
                        MUIA_Application_Version,       "$VER: CMTest 1.0 (17.1.99)",
                        MUIA_Application_Copyright,     "Copyright ©1999-2003 by Simone Tellini",
                        MUIA_Application_Author,        "Simone Tellini <wiz@vapor.com>",
                        MUIA_Application_Description,   "CManager.mcc test program",
                        MUIA_Application_Base,          "CMTEST",

                        SubWindow, MainWin = WindowObject,

                            MUIA_Window_ID,             MAKE_ID( 'M', 'A', 'I', 'N' ),
                            MUIA_Window_Title,          "CMTest - ©1999 Simone Tellini",

                            WindowContents, VGroup,

                                Child, obj = CManagerObject,
                                    MUIA_CManager_HideFTP,          TRUE,
                                End,
                            End,
                        End,
                   End ) {
        ULONG   sigs = 0;

        DoMethod( MainWin, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, MUIV_Notify_Application, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit );

        // you MUST call the following
        DoMethod( obj, MUIM_CManager_LoadData, NULL, NULL );

        set( MainWin, MUIA_Window_Open, TRUE );

        while( DoMethod( AppObj, MUIM_Application_NewInput, &sigs ) != MUIV_Application_ReturnID_Quit ) {
            if( sigs ) {

                sigs = Wait( sigs | SIGBREAKF_CTRL_C );

                if( sigs & SIGBREAKF_CTRL_C )
                    break;
            }
        }

        get( obj, MUIA_CManager_Changed, &sigs );

        if( sigs )
            DoMethod( obj, MUIM_CManager_SaveData, TRUE, NULL );

        MUI_DisposeObject( AppObj );
    }

    CloseLibrary( MUIMasterBase );

    return( 0 );
}
///
