/***************************************************************************
    revision             : $Id: import.c,v 1.1.1.1 2003-01-06 15:59:29 tellini Exp $
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
#include <mui/nlistview_mcc.h>
#include <libraries/mui.h>              // libraries
#include <libraries/bbsread.h>
#include <clib/exec_protos.h>           // protos
#include <clib/dos_protos.h>
#include <clib/bbsread_protos.h>
#include <clib/utility_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/alib_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/dos_pragmas.h>
#include <pragmas/bbsread_pragmas.h>
#include <pragmas/utility_pragmas.h>
#include <pragmas/muimaster_pragmas.h>

#include <extrafuncs.h>
#include <mymacros.h>

#include "/CManager.h"
#include "CManager_mcc.h"
#include "CManager_private.h"
//#include "main.h"
//#include "import.h"
#include "MUI.h"
#include "/locale.h"

#include "/Library/CManager_protos.h"
#include "/Library/CManager_lib_lib.h"
///
/// Prototypes
#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d)    ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

static void ImportVoyager( BPTR File, struct CMGroup *Group, BOOL Filter );
static void ImportIB( BPTR File, struct CMGroup *Group, BOOL Filter );
static void ImportAWeb( BPTR File, struct CMGroup *Group, BOOL Filter );
static void ImportUserCSV( BPTR file, struct CMGroup *Group, BOOL Filter );

static STRPTR GetCSVToken( STRPTR str, STRPTR dest, ULONG maxlen );

static struct CMGroup  *CheckGroup( struct CMGroup *Group, struct CMGroup *Entry );
static struct CMWWW    *CheckWWW( struct CMGroup *Group, struct CMWWW *Entry );
static struct CMFTP    *CheckFTP( struct CMGroup *Group, struct CMFTP *Entry );
static struct CMUser   *CheckUser( struct CMGroup *Group, struct CMUser *Entry );
static struct CMChat   *CheckChat( struct CMGroup *Group, struct CMChat *Entry );
///
/// Data
///


/// CManager_ImportDB
ULONG CManager_ImportDB( struct IClass *cl, Object *obj, struct MUIP_CManager_Import *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    BPTR                    file;

    DoMethod( obj, MUIM_CManager_GrabLists );

    if( file = Open( msg->File, MODE_OLDFILE )) {

        switch( msg->Type ) {

            case MUIV_CManager_Import_Voyager:
                ImportVoyager( file, data->CMData->WWWs, ( msg->Flags & MUIV_CManager_Import_Filter ) ? TRUE : FALSE );
                break;

            case MUIV_CManager_Import_IB:
                ImportIB( file, data->CMData->WWWs, ( msg->Flags & MUIV_CManager_Import_Filter ) ? TRUE : FALSE );
                break;

            case MUIV_CManager_Import_AWeb:
                ImportAWeb( file, data->CMData->WWWs, ( msg->Flags & MUIV_CManager_Import_Filter ) ? TRUE : FALSE );
                break;

            case MUIV_CManager_Import_Users_CSV:
                ImportUserCSV( file, data->CMData->Users, ( msg->Flags & MUIV_CManager_Import_Filter ) ? TRUE : FALSE );
                break;
        }

        Close( file );
    }

    DoMethod( obj, MUIM_CManager_ReinsertLists );

    nnset( obj, MUIA_CManager_Changed, TRUE );

    DoMethod( obj, MUIM_CManager_SaveData, FALSE, NULL );

    return( 0 );
}
///

/// ImportVoyager
static void ImportVoyager( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT            buffer[1024];
    ULONG           ULs = 0;
    struct CMGroup *CurGroup;

    while( FGets( File, buffer, sizeof( buffer ) - 1 ))
        if( stricmp( buffer, "<UL>\n" ) == 0 ) {
            ULs = 1;
            break;
        }

    CurGroup = Group;

    while( ULs && FGets( File, buffer, sizeof( buffer ) - 1 )) {
        STRPTR  ptr;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( stricmp( buffer, "</UL>" ) == 0 ) {

            CurGroup  = CM_GetParent( Group, CurGroup );
            ULs      -= 1;

            continue;
        }

        if( strncmp( buffer, "<LI>", 4 ))
            continue;

        if( ptr = strstr( buffer, "<UL>" )) {
            struct CMGroup *ng;

            *ptr = '\0';

            if( ng = CM_AllocEntry( CME_GROUP )) {
                TEXT            buf2[256];
                struct CMGroup *group;

                ng->Flags = GRPF_OPEN;

                if( strncmp( &buffer[4], "<B>", 3 ) == 0 )
                    stccpy( buf2, &buffer[7], strlen( &buffer[7] ) - 3 );
                else
                    stccpy( buf2, &buffer[4], sizeof( buf2 ));

                stccpy( ng->Name, buf2, sizeof( ng->Name ));

                if(( Filter == FALSE ) || (( group = CheckGroup( Group, ng )) == NULL )) {

                    AddTail(( struct List * )&CurGroup->SubGroups, ( struct Node * )ng );

                    CurGroup = ng;

                } else {

                    CurGroup = group;

                    CM_FreeEntry( ng );
                }

                ULs += 1;
            }

            continue;
        }

        if( strncmp( &buffer[4], "<A HREF=\"", 9 ) == 0 ) {
            struct CMWWW   *www;

            if( www = CM_AllocEntry( CME_WWW  )) {
                UBYTE  *ptr2;

                ptr = strchr( &buffer[13], '>' ) + 1;

                if( ptr2 = strstr( ptr, "</A>" ))
                    *ptr2 = '\0';

                stccpy( www->Name, ptr, sizeof( www->Name ));

                ptr[-2] = '\0';

                stccpy( www->WWW, &buffer[13], sizeof( www->WWW  ));

                if(( Filter == FALSE ) || ( CheckWWW( Group, www ) == NULL ))
                    AddTail(( struct List * )&CurGroup->Entries, ( struct Node * )www );
                else
                    CM_FreeEntry( www );
            }
        }
    }
}
///
/// ImportIB
static void ImportIB( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT            buffer[1024];
    ULONG           ULs = 0;
    struct CMGroup *CurGroup;

    while( FGets( File, buffer, sizeof( buffer ) - 1 ))
        if( stricmp( buffer, "<UL>\n" ) == 0 ) {
            ULs = 1;
            break;
        }

    CurGroup = Group;

    while( ULs && FGets( File, buffer, sizeof( buffer ) - 1 )) {
        UBYTE  *ptr, *buf;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( stricmp( buffer, "</UL>" ) == 0 ) {

            CurGroup  = CM_GetParent( Group, CurGroup );
            ULs      -= 1;

            continue;
        }

        buf = buffer;

        if( strncmp( buf, "<LI", 3 ))
            continue;

        buf = strchr( buf, '>' ) + 1;

        if( ptr = strstr( buffer, "<UL>" )) {
            struct CMGroup *ng;

            *ptr = '\0';

            if( ng = CM_AllocEntry( CME_GROUP )) {
                UBYTE          *from;
                struct CMGroup *group;

                ng->Flags = GRPF_OPEN;

                from = buf;

                if( strncmp( from, "<B>", 3 ) == 0 ) {

                    from += 3;

                    if( ptr = strstr( from, "</B>" ))
                        *ptr = '\0';
                }

                stccpy( ng->Name, from, sizeof( ng->Name ));

                if(( Filter == FALSE ) || (( group = CheckGroup( Group, ng )) == NULL )) {

                    AddTail(( struct List * )&CurGroup->SubGroups, ( struct Node * )ng );

                    CurGroup = ng;

                } else {

                    CurGroup = group;

                    CM_FreeEntry( ng );
                }

                ULs += 1;
            }

            continue;
        }

        if( strncmp( buf, "<A HREF=\"", 9 ) == 0 ) {
            struct CMWWW   *www;

            buf += 9;

            if( www = CM_AllocEntry( CME_WWW )) {
                UBYTE  *ptr2;

                ptr = strchr( buf, '>' ) + 1;

                if( ptr2 = strstr( ptr, "</A>" ))
                    *ptr2 = '\0';

                stccpy( www->Name, ptr, sizeof( www->Name ));

                *strchr( buf, '"' ) = '\0';

                stccpy( www->WWW, buf, sizeof( www->WWW  ));

                if(( Filter == FALSE ) || ( CheckWWW( Group, www ) == NULL ))
                    AddTail(( struct List * )&CurGroup->Entries, ( struct Node * )www );
                else
                    CM_FreeEntry( www );
            }
        }
    }
}
///
/// ImportAWeb
static void ImportAWeb( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT            buffer[1024];
    struct CMGroup *CurGroup;

    FGets( File, buffer, sizeof( buffer ) - 1 );

    if( stricmp( buffer, "@AWeb hotlist\n" ) != 0 )
        return;

    CurGroup = Group;

    while( FGets( File, buffer, sizeof( buffer ) - 1 )) {
        struct CMWWW   *www;
        UBYTE          *ptr;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( stricmp( buffer, "@ENDGROUP" ) == 0 ) {
            CurGroup = CM_GetParent( Group, CurGroup );
            continue;
        }

        if( strncmp( buffer, "@GROUP", 6 ) == 0 ) {
            struct CMGroup *ng;
            UBYTE          *buf;

            buf = strchr( buffer, ' ' ) + 1;

            if( ng = CM_AllocEntry( CME_GROUP )) {
                struct CMGroup *group;

                ng->Flags = GRPF_OPEN;

                stccpy( ng->Name, buf, sizeof( ng->Name ));

                if(( Filter == FALSE ) || (( group = CheckGroup( Group, ng )) == NULL )) {

                    AddTail(( struct List * )&CurGroup->SubGroups, ( struct Node * )ng );

                    CurGroup = ng;

                } else {

                    CurGroup = group;

                    CM_FreeEntry( ng );
                }
            }

            continue;
        }


        if( www = CM_AllocEntry( CME_WWW )) {

            stccpy( www->WWW, buffer, sizeof( www->WWW ));

            FGets( File, buffer, sizeof( buffer ) - 1 );

            stccpy( www->Name, buffer, sizeof( www->Name ));

            if( ptr = strchr( www->Name, '\n' ))
                *ptr = '\0';

            if(( Filter == FALSE ) || ( CheckWWW( Group, www ) == NULL ))
                AddTail(( struct List * )&CurGroup->Entries, ( struct Node * )www );
            else
                CM_FreeEntry( www );
        }
    }
}
///

/// ImportUserCSV
static void ImportUserCSV( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT    buffer[1024];

    FGets( File, buffer, sizeof( buffer ) - 1 ); // skip headers

    while( FGets( File, buffer, sizeof( buffer ) - 1 )) {
        STRPTR          ptr;
        struct CMUser  *entry;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( entry = CM_AllocEntry( CME_USER )) {

            ptr = buffer;

            ptr = GetCSVToken( ptr, entry->LastName,    sizeof( entry->LastName    ));
            ptr = GetCSVToken( ptr, entry->Name,        sizeof( entry->Name        ));
            ptr = GetCSVToken( ptr, entry->Address,     sizeof( entry->Address     ));
            ptr = GetCSVToken( ptr, entry->City,        sizeof( entry->City        ));
            ptr = GetCSVToken( ptr, entry->Country,     sizeof( entry->Country     ));
            ptr = GetCSVToken( ptr, entry->ZIP,         sizeof( entry->ZIP         ));
            ptr = GetCSVToken( ptr, entry->Comment,     sizeof( entry->Comment     ));
            ptr = GetCSVToken( ptr, entry->Alias,       sizeof( entry->Alias       ));
            ptr = GetCSVToken( ptr, entry->Phone,       sizeof( entry->Phone       ));
            ptr = GetCSVToken( ptr, entry->Fax,         sizeof( entry->Fax         ));
            ptr = GetCSVToken( ptr, entry->Mobile,      sizeof( entry->Mobile      ));
            ptr = GetCSVToken( ptr, entry->EMail,       sizeof( entry->EMail       ));
            ptr = GetCSVToken( ptr, entry->WWW,         sizeof( entry->WWW         ));
            ptr = GetCSVToken( ptr, entry->FTP,         sizeof( entry->FTP         ));
            ptr = GetCSVToken( ptr, entry->ICQ,         sizeof( entry->ICQ         ));
            ptr = GetCSVToken( ptr, entry->Company,     sizeof( entry->Company     ));
            ptr = GetCSVToken( ptr, entry->BJobTitle,   sizeof( entry->BJobTitle   ));
            ptr = GetCSVToken( ptr, entry->BDepartment, sizeof( entry->BDepartment ));
            ptr = GetCSVToken( ptr, entry->BOffice,     sizeof( entry->BOffice     ));
            ptr = GetCSVToken( ptr, entry->BAddress,    sizeof( entry->BAddress    ));
            ptr = GetCSVToken( ptr, entry->BCity,       sizeof( entry->BCity       ));
            ptr = GetCSVToken( ptr, entry->BCountry,    sizeof( entry->BCountry    ));
            ptr = GetCSVToken( ptr, entry->BZIP,        sizeof( entry->BZIP        ));
            ptr = GetCSVToken( ptr, entry->BPhone,      sizeof( entry->BPhone      ));
            ptr = GetCSVToken( ptr, entry->BFax,        sizeof( entry->BFax        ));
            ptr = GetCSVToken( ptr, entry->BMobile,     sizeof( entry->BMobile     ));
            ptr = GetCSVToken( ptr, entry->BEMail,      sizeof( entry->BEMail      ));

            if(( Filter == FALSE ) || ( CheckUser( Group, entry ) == NULL ))
                AddTail(( struct List * )&Group->Entries, ( struct Node * )entry );
            else
                CM_FreeEntry( entry );
        }
    }
}
///

/// GetCSVToken
static STRPTR GetCSVToken( STRPTR str, STRPTR dest, ULONG maxlen )
{
    BOOL    first = TRUE, quoted = FALSE, lastquote = FALSE, done = FALSE;

    maxlen--; // ending '\0'

    while( str && *str && !done ) {
        UBYTE   ch, out;

        ch  = *str++;
        out = '\0';

        switch( ch ) {

            case '"':
                if( first )
                    quoted = TRUE;
                else if( lastquote )
                    out = '"';
                else if( quoted && ( *str == ',' ))
                    quoted = FALSE;
                else
                    lastquote = TRUE;
                break;

            case ',':
                if( quoted )
                    out = ',';
                else
                    done = TRUE;
                break;

            default:
                out = ch;
                break;
        }

        if( out && maxlen ) {
            *dest++ = out;
            maxlen--;
        }

        first = FALSE;
    }

    *dest = '\0';

    return( str );
}
///

/// CheckGroup
static struct CMGroup *CheckGroup( struct CMGroup *Group, struct CMGroup *Entry )
{
    struct CMGroup *node;

    for( node = (struct CMGroup *)Group->SubGroups.mlh_Head; node->Succ; node = node->Succ ) {
        struct CMGroup *node2;

        if( stricmp( node->Name, Entry->Name ) == 0 )
            return( node );

        if( node2 = CheckGroup( node, Entry ))
            return( node2 );
    }

    return( NULL );
}
///
/// CheckWWW
static struct CMWWW *CheckWWW( struct CMGroup *Group, struct CMWWW *Entry )
{
    struct CMWWW   *node;

    for( node = (struct CMWWW *)Group->Entries.mlh_Head; node->Succ; node = node->Succ )
        if( stricmp( Entry->WWW, node->WWW ) == 0 )
            return( node );

    for( Group = (struct CMGroup *)Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ )
        if( node = CheckWWW( Group, Entry ))
            return( node );

    return( NULL );
}
///
/// CheckFTP
static struct CMFTP *CheckFTP( struct CMGroup *Group, struct CMFTP *Entry )
{
    struct CMFTP   *node;

    for( node = (struct CMFTP *)Group->Entries.mlh_Head; node->Succ; node = node->Succ )
        if( stricmp( Entry->FTP, node->FTP ) == 0 )
            return( node );

    for( Group = (struct CMGroup *)Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ )
        if( node = CheckFTP( Group, Entry ))
            return( node );

    return( NULL );
}
///
/// CheckUser
static struct CMUser *CheckUser( struct CMGroup *Group, struct CMUser *Entry )
{
    struct CMUser  *node;
    TEXT            this_user[256];

    strcpy( this_user, Entry->Name );

    if( Entry->LastName[0] ) {
        strcat( this_user, " " );
        strcat( this_user, Entry->LastName );
    }

    for( node = (struct CMUser *)Group->Entries.mlh_Head; node->Succ; node = node->Succ ) {
        TEXT    user[256];

        user[0] = '\0';

        if( node->Name[0] ) {

            strcpy( user, node->Name );

            if( node->LastName[0] )
                strcat( user, " " );
        }

        if( node->LastName[0] )
            strcat( user, node->LastName );

        if( stricmp( this_user, user ) == 0 )
            return( node );
    }

    for( Group = (struct CMGroup *)Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ )
        if( node = CheckUser( Group, Entry ))
            return( node );

    return( NULL );
}
///
/// CheckChat
static struct CMChat *CheckChat( struct CMGroup *Group, struct CMChat *Entry )
{
    struct CMChat  *node;

    for( node = (struct CMChat *)Group->Entries.mlh_Head; node->Succ; node = node->Succ )
        if(( stricmp( Entry->Server, node->Server ) == 0 ) && ( stricmp( Entry->Channel, node->Channel ) == 0 ))
            return( node );

    for( Group = (struct CMGroup *)Group->SubGroups.mlh_Head; Group->Succ; Group = Group->Succ )
        if( node = CheckChat( Group, Entry ))
            return( node );

    return( NULL );
}
///

