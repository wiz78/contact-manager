/***************************************************************************
    revision             : $Id: export.c,v 1.1 2003-01-18 17:10:17 tellini Exp $
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

#define CATCOMP_NUMBERS

#include <exec/types.h>                 // exec
#include <exec/memory.h>
#include <dos/dos.h>                    // dos
#include <mui/nlist_mcc.h>              // mui
#include <libraries/mui.h>              // libraries
#include <clib/exec_protos.h>           // protos
#include <clib/dos_protos.h>
#include <clib/utility_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/alib_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/dos_pragmas.h>
#include <pragmas/utility_pragmas.h>
#include <pragmas/muimaster_pragmas.h>

#include <extrafuncs.h>
#include <mymacros.h>

#include "/CManager.h"
#include "CManager_mcc.h"
#include "CManager_private.h"
#include "export.h"
#include "MUI.h"
#include "/locale.h"

#include "/Library/CManager_protos.h"
#include "/Library/CManager_lib_lib.h"
///
/// Prototypes
static void ExportURLasHTML( struct CMData *CMData, BPTR file, ULONG Flags );
static void ExportWWWasHTML( struct CMGroup *Group, BPTR file, ULONG Flags );
static void ExportFTPasHTML( struct CMGroup *Group, BPTR file, ULONG Flags );

static void ExportUsersAsCSV( struct CMGroup *Group, BPTR file, ULONG Flags );
static void WriteCSV( BPTR file, STRPTR string, BOOL comma );
///
/// Data
static TEXT HTMLExportHeader[] = "<HTML>\n"
                             "<HEAD><TITLE>CManager database</TITLE></HEAD>\n"
                             "<BODY>\n"
                             "<H2><P ALIGN=CENTER>CManager database</P></H2>\n"
                             "<HR>\n";

static TEXT HTMLExportFooter[] = "<HR>\n"
                             "<P ALIGN=CENTER><FONT SIZE=-1>"
                             "Created with <A HREF=\"http://www.vapor.com/cmanager/\">Contact Manager</A>"
                             " by <A HREF=\"mailto:wiz@vapor.com\">Simone Tellini</A>."
                             "</FONT></P>\n"
                             "</BODY>\n"
                             "</HTML>\n";
///


/// CManager_ExportDB
ULONG CManager_ExportDB( struct IClass *cl, Object *obj, struct MUIP_CManager_Export *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    BPTR                    file;
    Object                 *win;

    get( obj, MUIA_WindowObject, &win );

    if( win )
        set( win, MUIA_Window_Sleep, TRUE );

    DoMethod( obj, MUIM_CManager_GrabLists );

    if( file = Open( msg->File, MODE_NEWFILE )) {

        switch( msg->Type ) {

            case MUIV_CManager_Export_HTML_URLs:
                ExportURLasHTML( data->CMData, file, msg->Flags );
                break;

            case MUIV_CManager_Export_HTML_WWW:
                ExportWWWasHTML( data->CMData->WWWs, file, msg->Flags );
                break;

            case MUIV_CManager_Export_HTML_FTP:
                ExportFTPasHTML( data->CMData->FTPs, file, msg->Flags );
                break;

            case MUIV_CManager_Export_CSV_Users:
                ExportUsersAsCSV( data->CMData->Users, file, msg->Flags );
                break;
        }

        Close( file );
    }

    if( win )
        set( win, MUIA_Window_Sleep, FALSE );

    return( 0 );
}
///

/// ExportURLasHTML
static void ExportURLasHTML( struct CMData *CMData, BPTR file, ULONG Flags )
{
    FPuts( file, HTMLExportHeader );

    FPuts( file, "<CENTER><H3>Web sites</H3></CENTER><P>\n" );

    ExportWWWasHTML( CMData->WWWs, file, Flags | MUIV_CManager_Export_NoHead );

    FPuts( file, "<CENTER><H3>FTP sites</H3></CENTER><P>\n" );

    ExportFTPasHTML( CMData->FTPs, file, Flags | MUIV_CManager_Export_NoHead );

    FPuts( file, HTMLExportFooter );
}
///
/// ExportWWWasHTML
static void ExportWWWasHTML( struct CMGroup *Group, BPTR file, ULONG Flags )
{
    struct CMWWW   *www;

    if(!( Flags & MUIV_CManager_Export_NoHead ))
        FPuts( file, HTMLExportHeader );

    FPrintf( file, "<B>%s</B><UL>\n", Group->Name );

    for( www = (struct CMWWW *)Group->Entries.mlh_Head; www->Succ; www = www->Succ ) {

        if( !www->WWW[0] )
            continue;

        FPuts( file, "<LI><A HREF=\"" );

        if( strncmp( www->WWW, "http://", 7 ) &&
            strncmp( www->WWW, "file://", 7 ) &&
            strncmp( www->WWW, "ftp://",  6 ) &&
            strncmp( www->WWW, "gopher:", 7 ))
            FPuts( file, "http://" );

        FPrintf( file, "%s\">%s</A>\n", www->WWW, www->Name[0] ? www->Name : "- No Name -" );
    }

    for( Group = (struct CMGroup *)Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ ) {
        FPuts( file, "<LI>" );
        ExportWWWasHTML( Group, file, Flags | MUIV_CManager_Export_NoHead );
    }

    FPuts( file, "</UL>\n" );

    if(!( Flags & MUIV_CManager_Export_NoHead ))
        FPuts( file, HTMLExportFooter );
}
///
/// ExportFTPasHTML
static void ExportFTPasHTML( struct CMGroup *Group, BPTR file, ULONG Flags )
{
    struct CMFTP   *ftp;

    if(!( Flags & MUIV_CManager_Export_NoHead ))
        FPuts( file, HTMLExportHeader );

    FPrintf( file, "<B>%s</B><UL>\n", Group->Name );

    for( ftp = (struct CMFTP *)Group->Entries.mlh_Head; ftp->Succ; ftp = ftp->Succ ) {

        if( !ftp->FTP[0] )
            continue;

        FPuts( file, "<LI><A HREF=\"" );

        if( strncmp( ftp->FTP, "ftp://", 6 ))
            FPuts( file, "ftp://" );

        FPrintf( file, "%s\">%s</A>\n", ftp->FTP, ftp->Name[0] ? ftp->Name : "- No Name -" );
    }

    for( Group = (struct CMGroup *)Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ ) {
        FPuts( file, "<LI>" );
        ExportFTPasHTML( Group, file, Flags | MUIV_CManager_Export_NoHead );
    }

    FPuts( file, "</UL>\n" );

    if(!( Flags & MUIV_CManager_Export_NoHead ))
        FPuts( file, HTMLExportFooter );
}
///

/// ExportUsersAsCSV
static void ExportUsersAsCSV( struct CMGroup *Group, BPTR file, ULONG Flags )
{
    struct CMUser  *entry;

    if(!( Flags & MUIV_CManager_Export_NoHead )) {
        WriteCSV( file, "Last Name",    FALSE );
        WriteCSV( file, "First Name",   TRUE  );
        WriteCSV( file, "Address",      TRUE  );
        WriteCSV( file, "City",         TRUE  );
        WriteCSV( file, "Country",      TRUE  );
        WriteCSV( file, "ZIP",          TRUE  );
        WriteCSV( file, "Comment",      TRUE  );
        WriteCSV( file, "Alias",        TRUE  );
        WriteCSV( file, "Phone",        TRUE  );
        WriteCSV( file, "Fax",          TRUE  );
        WriteCSV( file, "Mobile",       TRUE  );
        WriteCSV( file, "EMail",        TRUE  );
        WriteCSV( file, "WWW",          TRUE  );
        WriteCSV( file, "FTP",          TRUE  );
        WriteCSV( file, "ICQ",          TRUE  );
        WriteCSV( file, "Company",      TRUE  );
        WriteCSV( file, "Job Title",    TRUE  );
        WriteCSV( file, "Department",   TRUE  );
        WriteCSV( file, "Office",       TRUE  );
        WriteCSV( file, "Work Address", TRUE  );
        WriteCSV( file, "Work City",    TRUE  );
        WriteCSV( file, "Work Country", TRUE  );
        WriteCSV( file, "Work ZIP",     TRUE  );
        WriteCSV( file, "Work Phone",   TRUE  );
        WriteCSV( file, "Work Fax",     TRUE  );
        WriteCSV( file, "Work Mobile",  TRUE  );
        WriteCSV( file, "Work EMail",   TRUE  );
        FPuts( file, "\n" );
    }

    for( entry = (struct CMUser *)Group->Entries.mlh_Head; entry->Succ; entry = entry->Succ ) {
        WriteCSV( file, entry->LastName,    FALSE );
        WriteCSV( file, entry->Name,        TRUE  );
        WriteCSV( file, entry->Address,     TRUE  );
        WriteCSV( file, entry->City,        TRUE  );
        WriteCSV( file, entry->Country,     TRUE  );
        WriteCSV( file, entry->ZIP,         TRUE  );
        WriteCSV( file, entry->Comment,     TRUE  );
        WriteCSV( file, entry->Alias,       TRUE  );
        WriteCSV( file, entry->Phone,       TRUE  );
        WriteCSV( file, entry->Fax,         TRUE  );
        WriteCSV( file, entry->Mobile,      TRUE  );
        WriteCSV( file, entry->EMail,       TRUE  );
        WriteCSV( file, entry->WWW,         TRUE  );
        WriteCSV( file, entry->FTP,         TRUE  );
        WriteCSV( file, entry->ICQ,         TRUE  );
        WriteCSV( file, entry->Company,     TRUE  );
        WriteCSV( file, entry->BJobTitle,   TRUE  );
        WriteCSV( file, entry->BDepartment, TRUE  );
        WriteCSV( file, entry->BOffice,     TRUE  );
        WriteCSV( file, entry->BAddress,    TRUE  );
        WriteCSV( file, entry->BCity,       TRUE  );
        WriteCSV( file, entry->BCountry,    TRUE  );
        WriteCSV( file, entry->BZIP,        TRUE  );
        WriteCSV( file, entry->BPhone,      TRUE  );
        WriteCSV( file, entry->BFax,        TRUE  );
        WriteCSV( file, entry->BMobile,     TRUE  );
        WriteCSV( file, entry->BEMail,      TRUE  );
        FPuts( file, "\n" );
    }

    for( Group = (struct CMGroup *)Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ )
        ExportUsersAsCSV( Group, file, Flags | MUIV_CManager_Export_NoHead );
}
///
/// WriteCSV
static void WriteCSV( BPTR file, STRPTR string, BOOL comma )
{
    STRPTR  quote;

    if( comma )
        FPuts( file, "," );

    if( quote = strchr( string, ',' ))
        FPuts( file, "\"" );

    while( string && *string ) {
        STRPTR  ptr;

        if( ptr = strchr( string, '\"' )) {

            FWrite( file, string, ptr - string + 1, 1 );
            FPuts( file, "\"" );

            ptr++;

        } else if( ptr = strchr( string, '\n' )) {

            FWrite( file, string, ptr - string + 1, 1 );
            FPuts( file, " " );

            ptr++;

        } else
            FPuts( file, string );

        string = ptr;
    }

    if( quote )
        FPuts( file, "\"" );
}
///
