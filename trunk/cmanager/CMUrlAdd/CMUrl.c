/***************************************************************************
    revision             : $Id: CMUrl.c,v 1.1.1.1 2003-01-06 15:55:12 tellini Exp $
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
#include <exec/types.h>                 //  exec
#include <exec/memory.h>
#include <exec/libraries.h>
#include <dos/dos.h>                    //  dos
#include <dos/rdargs.h>
#include <dos/dostags.h>
#include <iffp/ftxt.h>                  //  iffp
#include <libraries/reqtools.h>         //  libraries
#include <libraries/openurl.h>
#include <clib/alib_protos.h>           //  clib
#include <clib/openurl_protos.h>
#include <clib/reqtools_protos.h>
#include <proto/exec.h>                 //  proto
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/iffparse.h>
#include <pragmas/openurl_pragmas.h>    //  pragmas
#include <pragmas/reqtools_pragmas.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <extrafuncs.h>

#define sprintf mysprintf

#include "/CManager.h"

#include "/Library/CManager_protos.h"
#include "/Library/CManager_lib_lib.h"
///
/// Prototypes
static void AddToCM( STRPTR );
static void CopyToClip( STRPTR );
///
/// Data
struct Library     *CManagerBase;
struct Library     *ReqToolsBase;
struct Library     *OpenURLBase;
struct Library     *IFFParseBase;

static TEXT Version[] = "$VER: CMUrlAdd 1.1 (21.9.99) ©1999-2003 by Simone Tellini <wiz@vapor.com>";
///


/// main
int main( int argc, char *argv[] )
{
    if( argc && ( ReqToolsBase = OpenLibrary( "reqtools.library", 38 ))) {

        if( OpenURLBase = OpenLibrary( "openurl.library", 0 )) {

            if( CManagerBase = OpenLibrary( "CManager.library", 2 )) {

                if( IFFParseBase = OpenLibrary( "iffparse.library", 36 )) {
                    static ULONG    tags[] = { RTEZ_ReqTitle, (ULONG)"CMAddUrl - ©1999 Simone Tellini",
                                               RT_Underscore, '_',
                                               TAG_DONE };
                    ULONG           action;
                    TEXT            body[512];

                    sprintf( body, "Select the action to perform on:\n%s", argv[1] );

                    action = rtEZRequestA( body,
                                           "_Open URL|» _CManager|» C_lipboard|_Nothing",
                                           NULL, NULL,
                                           (struct TagItem *)tags );

                    switch( action ) {

                        case 1:
                            URL_OpenA( argv[1], NULL );
                            break;

                        case 2:
                            AddToCM( argv[1] );
                            break;

                        case 3:
                            CopyToClip( argv[1] );
                            break;
                    }

                    CloseLibrary( IFFParseBase );
                }

                CloseLibrary( CManagerBase );
            }

            CloseLibrary( OpenURLBase );
        }

        CloseLibrary( ReqToolsBase );
    }

    return( 0 );
}
///


/// AddToCM
static void AddToCM( STRPTR site )
{
    ULONG           type;
    struct CMWWW   *entry;

    type = ( strncmp( site, "ftp://", 6 ) == 0 ) ? CME_FTP : CME_WWW;

    if( entry = CM_AllocEntry( type )) {

        switch( type ) {

            case CME_WWW:
                stccpy( entry->WWW, site, sizeof( entry->WWW ));
                break;

            case CME_FTP:
                stccpy(((struct CMFTP *)entry )->FTP, site, sizeof(((struct CMFTP *)entry )->FTP ));
                break;
        }

        stccpy( entry->Name, site, sizeof( entry->Name ));

        rtGetStringA( entry->Name, sizeof( entry->Name ), "Record name", NULL, NULL );

        CM_AddEntry( entry );

        CM_FreeEntry( entry );
    }
}
///
/// CopyToClip
static void CopyToClip( STRPTR string )
{
    struct IFFHandle   *iff;

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
///
