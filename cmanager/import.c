/***************************************************************************
    revision             : $Id: import.c,v 1.1.1.1 2003-01-06 15:52:56 tellini Exp $
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

#include "STFaxLib_protos.h"
#include "STFax_lib_lib.h"
#include "phonebook.h"

#include "CManager.h"
#include "main.h"
#include "import.h"
#include "MUI.h"
#include "locale.h"

#include "Library/CManager_protos.h"
#include "Library/CManager_lib_lib.h"
///
/// Prototypes
#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d)    ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

static void OldImportSTFax( BPTR file, struct CMGroup *Group, BOOL Filter );

static struct Library *OpenBRLibrary( LONG );

static ULONG ListWindow_New( struct IClass *, Object *, struct opSet * );
static ULONG ListWindow_Open( struct IClass *, Object *, struct MUIP_ListWindow_Open * );
static ULONG ListWindow_Close( struct IClass *, Object *, struct MUIP_ListWindow_Close * );
static ULONG ListWindow_Result( struct IClass *, Object *, Msg );

static ULONG SAVEDS ASM List_DispHook( REG( a2 ) STRPTR *, REG( a1 ) struct Node * );

static struct CMGroup  *CheckGroup( struct CMGroup *, struct CMGroup * );
static struct CMWWW    *CheckWWW( struct CMGroup *, struct CMWWW * );
static struct CMFTP    *CheckFTP( struct CMGroup *, struct CMFTP * );
static struct CMUser   *CheckUser( struct CMGroup *, struct CMUser * );
static struct CMChat   *CheckChat( struct CMGroup *, struct CMChat * );

static LONG my_rand( ULONG * );

struct profile {
        char label[ 40 ];
        char host[ 128 ];
        char dummy4[ 32 ];
        char password[ 64 ];
        char defdir[ 120 ];
        char dummy[ 8 ];
        char localdir[ 120 ];
        char port[ 6 ];
        UBYTE compress;
        UBYTE saveldir;
        UBYTE is_anon;
        UBYTE retries;
        UBYTE savedir;
        UBYTE quiet_login;
        UBYTE is_adt;
        char username[ 64 ];
        UBYTE dummy3; // ACHTUNG siehe profile.c sizeof - 1
};
///
/// Data
struct MUI_CustomClass *CL_ListWindow;

static struct Library *BBSReadBase;
static struct Library *STFaxBase;
///


/// ImportAmFTP
void ImportAmFTP( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    UWORD           version;
    ULONG           install;
    struct profile  pf;

    Read( File, &version, 2 ); // Version auslesen
    Read( File, &install, 4 ); // Ablaufzeit auslesen

    if( version > 2 )
        return;

    while( Read( File, &pf, sizeof( pf )  - 1 ) == sizeof( pf ) - 1 ) {
        BYTE           *p;
        ULONG           j, seed;
        struct CMFTP   *ftp;

        seed = (ULONG)-1;

        p = (BYTE *) &pf;

        for( j = 0; j < sizeof( pf ); j++ )
            *p++ ^= my_rand( &seed );

        if( *(pf.port) == 0 )
            strcpy( pf.port, "21" );

        if( ftp = CM_AllocEntry( CME_FTP )) {
            ULONG   len;
            UBYTE  *ptr;

            strcpy( ftp->Name,      pf.label    );
            strcpy( ftp->FTP,       pf.host     );
            strcpy( ftp->Username,  pf.username );
            strcpy( ftp->Password,  pf.password );

            len = strlen( ftp->FTP ) - 1;

            if( ftp->FTP[ len ] == '/' )
                ftp->FTP[ len ] = '\0';

            ftp->Port    = atoi( pf.port );
            ftp->Retries = pf.retries;

            ftp->Flags = FTPF_ADVANCED;

            if( pf.compress )
                ftp->Flags |= FTPF_COMPRESS;

            if( pf.quiet_login )
                ftp->Flags |= FTPF_QUIET;

            if( pf.is_adt )
                ftp->Flags |= FTPF_ADT;

            if( pf.is_anon )
                ftp->Flags |= FTPF_ANONYMOUS;

            ptr = pf.defdir;

            if( *ptr == '/' )
                ptr++;

            AddPart( ftp->FTP, ptr, sizeof( ftp->FTP ));

            if(( Filter == FALSE ) || ( CheckFTP( Group, ftp ) == NULL )) {

                AddTail(( struct List * )&Group->Entries, ( struct Node * )ftp );

                Flags &= ~SAVED;

            } else
                CM_FreeEntry( ftp );
        }
    }
}
///

/// ImportTHOR
BOOL ImportTHOR( Object *AppObj, struct CMGroup *Group )
{
    BOOL            ret = FALSE;

    if( BBSReadBase = OpenBRLibrary( 3 )) {
        struct List    *bbslist;

        if( bbslist = GetBBSList() ) {
            Object *wnd;

            if( wnd = NewObject( CL_ListWindow->mcc_Class, NULL, TAG_DONE )) {
                ULONG               sigs = 0L;
                struct BBSListItem *bbs;

                DoMethod( AppObj, OM_ADDMEMBER, wnd );

                DoMethod( wnd, MUIM_ListWindow_Open, bbslist );

                while( DoMethod( AppObj, MUIM_Application_NewInput, &sigs ) != MUIV_Application_ReturnID_Quit ) {
                    if( sigs ) {

                        sigs = Wait( SIGBREAKF_CTRL_C | sigs );

                        if( sigs & SIGBREAKF_CTRL_C )
                            break;
                    }
                }

                bbs = (struct BBSListItem *) DoMethod( wnd, MUIM_ListWindow_Result );

                RemoveWindow( wnd );

                if( bbs ) {
                    ULONG   i;

                    for( i = bbs->bl_Internal->bi_FirstUser; i <= bbs->bl_Internal->bi_LastUser; i++ ) {
                        APTR            thoruser;
                        ULONG           flags;
                        struct TagItem *tags;

                        if( thoruser = ReadBRUserTags( bbs, i,
                                                       RBRUSR_UserTagsPtr,  &tags,
                                                       RBRUSR_Flags,        &flags,
                                                       TAG_DONE )) {
                            struct CMUser  *user;

                            if( tags == NULL )
                                continue;

                            if( user = CM_AllocEntry( CME_USER )) {
                                struct TagItem *tag;

                                AddTail(( struct List * )&Group->Entries, ( struct Node * )user );

                                ret = TRUE;

                                while( tag = NextTagItem( &tags )) {
                                    STRPTR  ptr;

                                    switch( tag->ti_Tag ) {

                                        case BRUSR_Name:
                                            stccpy( user->Name, (STRPTR)tag->ti_Data, sizeof( user->Name ));

                                            if( ptr = strrchr( user->Name, ' ' )) {

                                                stccpy( user->LastName, ptr + 1, sizeof( user->LastName ));

                                                *ptr = '\0';
                                            }
                                            break;

                                        case BRUSR_Address:
                                            stccpy( user->EMail, (STRPTR)tag->ti_Data, sizeof( user->EMail ));
                                            break;

                                        case BRUSR_Alias:
                                            stccpy( user->Alias, (STRPTR)tag->ti_Data, sizeof( user->Alias ));
                                            break;

                                        case BRUSR_Comment:
                                            stccpy( user->Comment, *((STRPTR *)tag->ti_Data ), sizeof( user->Comment ));
                                            break;
                                    }
                                }

                                Flags &= ~SAVED;
                            }

                            FreeBRObject( thoruser );
                        }
                    }
                }
            }

            FreeBRObject( bbslist );
        }

        CloseLibrary( BBSReadBase );
    }

    return( ret );
}
///
/// ImportMD
void ImportMD( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT            buffer[1024];
    struct CMGroup *CurGroup;

    FGets( File, buffer, sizeof( buffer ) - 1 );

    if( stricmp( buffer, "MicroDot - Bookmarks.mcc\n" ) != 0 )
        return;

    CurGroup = Group;

    while( FGets( File, buffer, sizeof( buffer ) - 1 )) {
        struct CMUser  *user;
        UBYTE          *ptr;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( buffer[0] == (UBYTE)'«' ) {
            CurGroup = CM_GetParent( Group, CurGroup );
            continue;
        }

        if( buffer[0] == (UBYTE)'»' ) {
            struct CMGroup *ng;

            if( ng = CM_AllocEntry( CME_GROUP )) {
                struct CMGroup *group;

                ng->Flags = GRPF_OPEN;

                stccpy( ng->Name, &buffer[1], sizeof( ng->Name ));

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

        if( user = CM_AllocEntry( CME_USER )) {
            UBYTE  *ptr;

            ptr = strchr( buffer, '|' );

            stccpy( user->EMail, ptr + 1, sizeof( user->EMail ));

            *ptr = '\0';

            stccpy( user->Name, buffer, sizeof( user->Name ));

            if( ptr = strrchr( user->Name, ' ' )) {

                stccpy( user->LastName, ptr + 1, sizeof( user->LastName ));

                *ptr = '\0';
            }

            if(( Filter == FALSE ) || ( CheckUser( Group, user ) == NULL ))
                AddTail(( struct List * )&CurGroup->Entries, ( struct Node * )user );
            else
                CM_FreeEntry( user );
        }
    }

    Flags &= ~SAVED;
}
///
/// ImportYAM
void ImportYAM( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT            buffer[1024];
    struct CMGroup *CurGroup;

    FGets( File, buffer, sizeof( buffer ) - 1 );

    if(( stricmp( buffer, "YAB2 - YAM Addressbook V2\n" ) != 0 ) &&
       ( stricmp( buffer, "YAB3 - YAM Addressbook\n" ) != 0 ))
        return;

    CurGroup = Group;

    while( FGets( File, buffer, sizeof( buffer ) - 1 )) {
        struct CMUser  *user;
        UBYTE          *ptr;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( stricmp( buffer, "@ENDGROUP" ) == 0 ) {
            CurGroup = CM_GetParent( Group, CurGroup );
            continue;
        }

        if( strncmp( buffer, "@GROUP", 6 ) == 0 ) {
            struct CMGroup *ng;

            if( ng = CM_AllocEntry( CME_GROUP )) {
                struct CMGroup *group;

                ng->Flags = GRPF_OPEN;

                FGets( File, buffer, sizeof( buffer ) - 1 );

                stccpy( ng->Name, buffer, sizeof( ng->Name ));

                if( ptr = strchr( ng->Name, '\n' ))
                    *ptr = '\0';

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

        if( strncmp( buffer, "@USER ", 6 ) == 0 ) {

            if( user = CM_AllocEntry( CME_USER )) {

                stccpy( user->Alias, &buffer[6], sizeof( user->Alias ));

                FGets( File, buffer, sizeof( buffer ) - 1 );
                stccpy( user->EMail, buffer, sizeof( user->EMail ));

                FGets( File, buffer, sizeof( buffer ) - 1 );
                stccpy( user->Name, buffer, sizeof( user->Name ));

                FGets( File, buffer, sizeof( buffer ) - 1 );
                stccpy( user->Comment, buffer, sizeof( user->Comment ));

                if( ptr = strchr( user->EMail, '\n' ))
                    *ptr = '\0';

                if( ptr = strchr( user->Name, '\n' ))
                    *ptr = '\0';

                if( ptr = strchr( user->Comment, '\n' ))
                    *ptr = '\0';

                if( ptr = strrchr( user->Name, ' ' )) {

                    stccpy( user->LastName, ptr + 1, sizeof( user->LastName ));

                    *ptr = '\0';
                }

                if(( Filter == FALSE ) || ( CheckUser( Group, user ) == NULL ))
                    AddTail(( struct List * )&CurGroup->Entries, ( struct Node * )user );
                else
                    CM_FreeEntry( user );
            }

            while( FGets( File, buffer, sizeof( buffer ) - 1 ))
                if( stricmp( buffer, "@ENDUSER\n" ) == 0 )
                    break;
        }
    }

    Flags &= ~SAVED;
}
///
/// ImportDFA
void ImportDFA( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT    buffer[1024];

    FGets( File, buffer, sizeof( buffer ) - 1 );

    if( strncmp( buffer, "DFAddress", 9 ) != 0 )
        return;

    while( FGets( File, buffer, sizeof( buffer ) - 1 )) {
        struct CMUser  *user;
        UBYTE          *ptr;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( user = CM_AllocEntry( CME_USER )) {

            stccpy( user->Name, buffer, sizeof( user->Name ));

            FGets( File, user->LastName,  sizeof( user->LastName ) - 1 );
            FGets( File, user->Address,   sizeof( user->Address  ) - 1 );
            FGets( File, user->ZIP,       sizeof( user->ZIP      ) - 1 );
            FGets( File, user->City,      sizeof( user->City     ) - 1 );
            FGets( File, buffer,          sizeof( buffer         ) - 1 );
            FGets( File, buffer,          sizeof( buffer         ) - 1 );

            sprintf( user->Comment, "Birthday: %s\n", buffer );

            FGets( File, user->Phone,  sizeof( user->Phone   ) - 1 );
            FGets( File, user->EMail,  sizeof( user->EMail   ) - 1 );
            FGets( File, buffer,       sizeof( buffer        ) - 1 );
            FGets( File, buffer,       sizeof( buffer        ) - 1 );
            FGets( File, buffer,       sizeof( buffer        ) - 1 );

            strcat( user->Comment, buffer );

            FGets( File, buffer,        sizeof( buffer         ) - 1 );
            FGets( File, buffer,        sizeof( buffer         ) - 1 );
            FGets( File, user->Fax,     sizeof( user->Fax      ) - 1 );
            FGets( File, buffer,        sizeof( buffer         ) - 1 );
            FGets( File, buffer,        sizeof( buffer         ) - 1 );
            FGets( File, user->Country, sizeof( user->Country  ) - 1 );
            FGets( File, buffer,        sizeof( buffer         ) - 1 );

            if( ptr = strchr( user->LastName, '\n' ))
                *ptr = '\0';

            if( ptr = strchr( user->Address, '\n' ))
                *ptr = '\0';

            if( ptr = strchr( user->ZIP, '\n' ))
                *ptr = '\0';

            if( ptr = strchr( user->City, '\n' ))
                *ptr = '\0';

            if( ptr = strchr( user->Phone, '\n' ))
                *ptr = '\0';

            if( ptr = strchr( user->EMail, '\n' ))
                *ptr = '\0';

            if( ptr = strchr( user->Fax, '\n' ))
                *ptr = '\0';

            if( ptr = strchr( user->Country, '\n' ))
                *ptr = '\0';

            if(( Filter == FALSE ) || ( CheckUser( Group, user ) == NULL ))
                AddTail(( struct List * )&Group->Entries, ( struct Node * )user );
            else
                CM_FreeEntry( user );
        }
    }

    Flags &= ~SAVED;
}
///
/// ImportPINT
void ImportPINT( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT            buffer[1024];
    struct CMGroup *CurGroup;
    BOOL            inlist = FALSE;

    FGets( File, buffer, sizeof( buffer ) - 1 );    // $VER:
    FGets( File, buffer, sizeof( buffer ) - 1 );    // empty line

    CurGroup = Group;

    while( FGets( File, buffer, sizeof( buffer ) - 1 )) {
        struct CMUser  *user;
        UBYTE          *ptr;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( inlist && ( buffer[0] == '\0' )) {
            CurGroup = CM_GetParent( Group, CurGroup );
            inlist   = FALSE;
            continue;
        }

        if( stricmp( buffer, "ENDGROUP" ) == 0 ) {
            CurGroup = CM_GetParent( Group, CurGroup );
            FGets( File, buffer, sizeof( buffer ) - 1 );    // empty line
            continue;
        }

        if( stricmp( buffer, "LIST" ) == 0 )
            inlist = TRUE;

        if(( stricmp( buffer, "LIST" ) == 0 ) || ( stricmp( buffer, "GROUP" ) == 0 )) {
            struct CMGroup *ng;

            if( ng = CM_AllocEntry( CME_GROUP )) {
                struct CMGroup *group;

                ng->Flags = GRPF_OPEN;

                FGets( File, ng->Name, sizeof( ng->Name ) - 1 );

                if( ptr = strchr( ng->Name, '\n' ))
                    *ptr = '\0';

                FGets( File, buffer, sizeof( buffer ) - 1 );    // comment

                if(!( inlist ))
                    FGets( File, buffer, sizeof( buffer ) - 1 );    // empty line

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

        if( user = CM_AllocEntry( CME_USER )) {

            stccpy( user->Name, buffer, sizeof( user->Name ));

            if( ptr = strrchr( user->Name, ' ' )) {

                stccpy( user->LastName, ptr + 1, sizeof( user->LastName ));

                *ptr = '\0';
            }

            if(!( inlist )) {

                FGets( File, user->Comment, sizeof( user->Comment ) - 1 );

                if( ptr = strchr( user->Comment, '\n' ))
                    *ptr = '\0';
            }

            FGets( File, user->EMail,   sizeof( user->EMail ) - 1 );

            if( ptr = strchr( user->EMail, '\n' ))
                *ptr = '\0';

            if(( Filter == FALSE ) || ( CheckUser( Group, user ) == NULL ))
                AddTail(( struct List * )&CurGroup->Entries, ( struct Node * )user );
            else
                CM_FreeEntry( user );
        }

        if(!( inlist ))
            while( FGets( File, buffer, sizeof( buffer ) - 1 ))
                if( buffer[0] == '\n' )
                    break;
    }

    Flags &= ~SAVED;
}
///
/// ImportGoodNews
void ImportGoodNews( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT            buffer[1024];
    struct CMGroup *CurGroup;
    BOOL            inlist = FALSE;

    CurGroup = Group;

    while( FGets( File, buffer, sizeof( buffer ) - 1 )) {
        struct CMUser  *user;
        UBYTE          *ptr;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( buffer[0] == '\0' ) {

            if( inlist ) {
                CurGroup = CM_GetParent( Group, CurGroup );
                inlist   = FALSE;
            }

            continue;
        }

        if( stricmp( buffer, "GROUP" ) == 0 ) {
            struct CMGroup *ng;

            inlist = TRUE;

            if( ng = CM_AllocEntry( CME_GROUP )) {
                struct CMGroup *group;

                ng->Flags = GRPF_OPEN;

                FGets( File, ng->Name, sizeof( ng->Name ) - 1 );

                if( ptr = strchr( ng->Name, '\n' ))
                    *ptr = '\0';

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

        if( user = CM_AllocEntry( CME_USER )) {

            stccpy( user->Name, buffer, sizeof( user->Name ));

            if( ptr = strrchr( user->Name, ' ' )) {

                stccpy( user->LastName, ptr + 1, sizeof( user->LastName ));

                *ptr = '\0';
            }

            if(!( inlist )) {

                FGets( File, user->Comment, sizeof( user->Comment ) - 1 );

                if( ptr = strchr( user->Comment, '\n' ))
                    *ptr = '\0';
            }

            FGets( File, user->EMail,   sizeof( user->EMail ) - 1 );

            if( ptr = strchr( user->EMail, '\n' ))
                *ptr = '\0';

            if(( Filter == FALSE ) || ( CheckUser( Group, user ) == NULL ))
                AddTail(( struct List * )&CurGroup->Entries, ( struct Node * )user );
            else
                CM_FreeEntry( user );
        }

        if(!( inlist ))
            while( FGets( File, buffer, sizeof( buffer ) - 1 ))
                if( buffer[0] == '\n' )
                    break;
    }

    Flags &= ~SAVED;
}
///
/// ImportSTRICQ
void ImportSTRICQ( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    BPTR    dir;

    if( dir = ParentOfFH( File )) {
        BPTR    olddir;
        TEXT    uin[40];

        olddir = CurrentDir( dir );

        while( FGets( File, uin, sizeof( uin ) - 1 )) {
            BPTR    userfile;
            TEXT    name[256];
            UBYTE  *ptr;

            if( ptr = strchr( uin, '\n' ))
                *ptr = '\0';

            sprintf( name, "Users/%s", uin );

            if( userfile = Open( name, MODE_OLDFILE )) {
                struct CMUser  *user;

                if( user = CM_AllocEntry( CME_USER )) {
                    TEXT    buffer[256];

                    stccpy( user->ICQ, uin, sizeof( user->ICQ ));

                    while( FGets( userfile, buffer, sizeof( buffer ) - 1 )) {

                        if( ptr = strchr( uin, '\n' ))
                            *ptr = '\0';

                        if( strncmp( buffer, "NICK", 4 ) == 0 )
                            stccpy( user->Alias, &buffer[ 5 ], sizeof( user->Alias ));

                        else if( strncmp( buffer, "FIRST", 5 ) == 0 )
                            stccpy( user->Name, &buffer[ 6 ], sizeof( user->Name ));

                        else if( strncmp( buffer, "LAST", 4 ) == 0 )
                            stccpy( user->LastName, &buffer[ 5 ], sizeof( user->LastName ));

                        else if( strncmp( buffer, "EMAIL", 5 ) == 0 )
                            stccpy( user->EMail, &buffer[ 6 ], sizeof( user->EMail ));

                        else if( strncmp( buffer, "CITY", 4 ) == 0 )
                            stccpy( user->City, &buffer[ 5 ], sizeof( user->City ));

                        else if( strncmp( buffer, "STATE", 5 ) == 0 )
                            stccpy( user->Country, &buffer[ 6 ], sizeof( user->Country ));

                        else if( strncmp( buffer, "PHONE", 5 ) == 0 )
                            stccpy( user->Phone, &buffer[ 6 ], sizeof( user->Phone ));

                        else if( strncmp( buffer, "HOMEPAGE", 8 ) == 0 )
                            stccpy( user->WWW, &buffer[ 9 ], sizeof( user->WWW ));

                        else if( strncmp( buffer, "COMMENT", 7 ) == 0 ) {

                            Flush( userfile );

                            Read( userfile, user->Comment, sizeof( user->Comment ) - 1 );

                            user->Comment[ strlen( user->Comment ) ] = '\0';
                        }
                    }

                    if(( Filter == FALSE ) || ( CheckUser( Group, user ) == NULL ))
                        AddTail(( struct List * )&Group->Entries, ( struct Node * )user );
                    else
                        CM_FreeEntry( user );
                }

                Close( userfile );
            }
        }

        CurrentDir( olddir );

        UnLock( dir );

        Flags &= ~SAVED;
    }
}
///

/// ImportSTFax
void ImportSTFax( BPTR file, struct CMGroup *Group, BOOL Filter )
{
    if( STFaxBase = OpenLibrary( "STFax.library", 1 )) {
        TEXT            name[256];
        struct MinList  numbers;

        NameFromFH( file, name, sizeof( name ));

        NewList(( struct List * )&numbers );

        if( STF_LoadPhonebook( &numbers, name )) {
            struct PBEntry *e;

            for( e = (struct PBEntry *)numbers.mlh_Head; e->Node.ln_Succ; e = (struct PBEntry *)e->Node.ln_Succ ) {
                struct CMUser  *user;

                if( e->Node.ln_Type != PBT_ENTRY )
                    continue;

                if( user = CM_AllocEntry( CME_USER )) {
                    STRPTR  ptr;

                    stccpy( user->Name,    e->Name,    sizeof( user->Name    ));
                    stccpy( user->Fax,     e->Number,  sizeof( user->Fax     ));
                    stccpy( user->Phone,   e->Phone,   sizeof( user->Phone   ));
                    stccpy( user->Comment, e->Note,    sizeof( user->Comment ));
                    stccpy( user->Address, e->Address, sizeof( user->Address ));
                    stccpy( user->City,    e->City,    sizeof( user->City    ));
                    stccpy( user->Country, e->Country, sizeof( user->Country ));
                    stccpy( user->ZIP,     e->ZIP,     sizeof( user->ZIP     ));
                    stccpy( user->Mobile,  e->Mobile,  sizeof( user->Mobile  ));
                    stccpy( user->Company, e->Company, sizeof( user->Company ));
                    stccpy( user->EMail,   e->EMail,   sizeof( user->EMail   ));

                    if( ptr = strrchr( user->Name, ' ' )) {

                        stccpy( user->LastName, ptr + 1, sizeof( user->LastName ));

                        *ptr = '\0';
                    }

                    if(( Filter == FALSE ) || ( CheckUser( Group, user ) == NULL )) {

                        AddTail(( struct List * )&Group->Entries, ( struct Node * )user );

                        Flags &= ~SAVED;

                    } else
                        CM_FreeEntry( user );
                }
            }

            STF_FreePhonebook( &numbers );
        }

        CloseLibrary( STFaxBase );

    } else
        OldImportSTFax( file, Group, Filter );
}
///
/// OldImportSTFax
#define PB_EXTENDED     (1 << 4)

static void OldImportSTFax( BPTR file, struct CMGroup *Group, BOOL Filter )
{
    UBYTE   f;

    while(( f = FGetC( file )) == 0 ) {
        struct CMUser  *user;

        if( user = CM_AllocEntry( CME_USER )) {
            UBYTE   flags;
            TEXT    buf[256];
            STRPTR  ptr;

            flags = FGetC( file );

            FGetString( file, user->Fax     );
            FGetString( file, user->Name    );
            FGetString( file, user->Comment );

            if( ptr = strrchr( user->Name, ' ' )) {

                stccpy( user->LastName, ptr + 1, sizeof( user->LastName ));

                *ptr = '\0';
            }

            FGetString( file, buf );
            FGetString( file, buf );
            FGetString( file, buf );

            if( flags & PB_EXTENDED ) {

                FGetString( file, user->Phone );

                if( FGetC( file )) {

                    FGetString( file, buf );

                    FGetC( file );  // more data?
                }
            }

            if(( Filter == FALSE ) || ( CheckUser( Group, user ) == NULL )) {

                AddTail(( struct List * )&Group->Entries, ( struct Node * )user );

                Flags &= ~SAVED;

            } else
                CM_FreeEntry( user );
        }
    }
}
///

/// ImportAmIRC
void ImportAmIRC( BPTR File, struct CMGroup *Group, BOOL Filter )
{
    TEXT            buffer[1024];
    struct CMGroup *CurGroup;

    CurGroup = Group;

    while( FGets( File, buffer, sizeof( buffer ) - 1 )) {
        UBYTE  *ptr;

        if( ptr = strchr( buffer, '\n' ))
            *ptr = '\0';

        if( strncmp( buffer, "SERVER", 6 ) == 0 ) {
            struct CMChat  *entry;

            if( strncmp( &buffer[7], "<<", 2 ) == 0 ) {
                CurGroup = CM_GetParent( Group, CurGroup );
                continue;
            }

            if( strncmp( &buffer[7], ">>", 2 ) == 0 ) {
                struct CMGroup *ng;

                if( ng = CM_AllocEntry( CME_GROUP )) {
                    struct CMGroup *group;

                    ng->Flags = GRPF_OPEN;

                    stccpy( ng->Name, &buffer[9], sizeof( ng->Name ));

                    if( ptr = strrchr( ng->Name, ',' ))
                        *ptr = '\0';

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

            if( entry = CM_AllocEntry( CME_CHAT )) {
                STRPTR  ptr, ptr2;

                ptr  = &buffer[7];
                ptr2 = strchr( ptr, ',' );

                *ptr2 = '\0';

                stccpy( entry->Server, ptr, sizeof( entry->Server ));

                ptr  = strchr( ptr2 + 1, ',' ) + 1;
                ptr2 = strchr( ptr, ',' );

                *ptr2 = '\0';

                stccpy( entry->Channel, ptr, sizeof( entry->Channel ));

                ptr = strchr( ptr2 + 1, ',' ) + 1;

                stccpy( entry->Comment, ptr, sizeof( entry->Comment ));

                if(( Filter == FALSE ) || ( CheckChat( Group, entry ) == NULL )) {
                    AddTail(( struct List * )&CurGroup->Entries, ( struct Node * )entry );
                    Flags &= ~SAVED;
                } else
                    CM_FreeEntry( entry );
            }
        }
    }
}
///

/// ImportDOpusFTP
enum {
    DOFTP_ANON = 0, DOFTP_USERACCT, DOFTP_NAME, DOFTP_HOST,
    DOFTP_ADDR, DOFTP_PORT, DOFTP_DIR, DOFTP_USER, DOFTP_PASS,
    DOFTP_NUM_ARGS
};

void ImportDOpusFTP( BPTR file, struct CMGroup *Group, BOOL Filter )
{
    struct RDArgs  *args;

    if( args = AllocDosObject( DOS_RDARGS, NULL )) {
        TEXT    buffer[1024];
        BOOL    ok = FALSE;

        while( FGets( file, buffer, sizeof( buffer ) - 1 )) {

            if( strncmp( buffer, "# FTP accounts", 14 ) == 0 ) {
                ok = TRUE;
                break;
            }

            if( strncmp( buffer, "# Anonymous FTP sites", 21 ) == 0 ) {
                ok = TRUE;
                break;
            }
        }

        if( ok ) while( FGets( file, buffer, sizeof( buffer ) - 1 )) {
            struct RDArgs  *rdargs;
            UBYTE          *ptr;
            LONG            ArgArray[ DOFTP_NUM_ARGS ];

            if( ptr = strchr( buffer, '\n' ))
                *ptr = '\0';

            if(( buffer[0] == '\0' ) || ( buffer[0] == '#' ))
                continue;

            memset(( char * )ArgArray, 0, sizeof( ArgArray ));

            strcat( buffer, "\12" );

            args->RDA_Source.CS_Buffer = buffer;
            args->RDA_Source.CS_Length = strlen( buffer );
            args->RDA_Source.CS_CurChr = 0;
            args->RDA_DAList           = NULL;
            args->RDA_Buffer           = NULL;
            args->RDA_BufSiz           = 0L;
            args->RDA_Flags           |= RDAF_NOPROMPT;

            if( rdargs = ReadArgs( "ANONYMOUS=ANON/S,USERACCOUNT=ACCT/S,ALIAS=NAME/K,HOST,"
                                   "ADDRESS=ADDR/K,PORT/N,PATH=DIR,USER/K,PASSWORD=PASS/K",
                                   ArgArray, args )) {
                struct CMFTP   *ftp;
                ULONG           len;

                if( ftp = CM_AllocEntry( CME_FTP )) {

                    if( ArgArray[ DOFTP_NAME ])
                        stccpy( ftp->Name, (STRPTR)ArgArray[ DOFTP_NAME ], sizeof( ftp->Name ));

                    if( ArgArray[ DOFTP_HOST ])
                        stccpy( ftp->FTP, (STRPTR)ArgArray[ DOFTP_HOST ], sizeof( ftp->FTP ) - 1 );

                    if( ArgArray[ DOFTP_USER ])
                        stccpy( ftp->Username, (STRPTR)ArgArray[ DOFTP_USER ], sizeof( ftp->Username ));

                    if( ArgArray[ DOFTP_PASS ])
                        stccpy( ftp->Password, (STRPTR)ArgArray[ DOFTP_PASS ], sizeof( ftp->Password ));

                    len = strlen( ftp->FTP ) - 1;

                    if( ftp->FTP[ len ] == '/' )
                        ftp->FTP[ len ] = '\0';

                    if( ArgArray[ DOFTP_PORT ])
                        ftp->Port = *((ULONG *)ArgArray[ DOFTP_PORT ]);

                    if( ArgArray[ DOFTP_DIR ])
                        AddPart( ftp->FTP, (STRPTR)ArgArray[ DOFTP_DIR ], sizeof( ftp->FTP ));

                    ftp->Flags = FTPF_ADVANCED;

                    if( ArgArray[ DOFTP_ANON ])
                        ftp->Flags |= FTPF_ANONYMOUS;


                    if(( Filter == FALSE ) || ( CheckFTP( Group, ftp ) == NULL )) {

                        AddTail(( struct List * )&Group->Entries, ( struct Node * )ftp );

                        Flags &= ~SAVED;

                    } else
                        CM_FreeEntry( ftp );
                }

                FreeArgs( rdargs );
            }
        }

        FreeDosObject( DOS_RDARGS, args );
    }
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

//  AmFTP Support
/// my_rand
static LONG my_rand( ULONG *seed )
{
    LONG    ret, a;

    a     = (LONG)*seed;    // MOVE.L  0040(A4),D0
    ret   = a;              // MOVE.L  D0,D1
    ret <<= 3;              // ASL.L   #3,D1
    ret  -= a;              // SUB.L   D0,D1
    ret <<= 3;              // ASL.L   #3,D1
    ret  += a;              // ADD.L   D0,D1
    ret  += ret;            // ADD.L   D1,D1
    ret  += a;              // ADD.L   D0,D1
    ret <<= 4;              // ASL.L   #4,D1
    ret  -= a;              // SUB.L   D0,D1
    ret  += ret;            // ADD.L   D1,D1
    ret  -= a;              // SUB.L   D0,D1
    ret  += 0x00000E60;     // ADDI.L  #00000e60,D1
    ret  &= 0x7FFFFFFF;     // ANDI.L  #7fffffff,D1
    a     = ret;            // MOVE.L  D1,D0
    a    -= 1;              // SUBQ.L  #1,D0
    *seed = (ULONG)a;       // MOVE.L  D0,0040(A4)

    return( ret );          // MOVE.L  D1,D0
}
///


//  THOR Support
/// OpenBRLibrary
static struct Library *OpenBRLibrary( LONG version )
{
    TEXT            envbuf[256];
    struct Library *BBSReadBase = NULL;

    if( GetVar( ENV_THORPATH, envbuf, 256, GVF_GLOBAL_ONLY )) {

        if( AddPart( envbuf, "libs/" BBSREADNAME, 256 )) {
            BBSReadBase = OpenLibrary( envbuf, version );
        }
    }

    if(!( BBSReadBase ))
        BBSReadBase = OpenLibrary( BBSREADNAME, version );

    return( BBSReadBase );
}
///

/// ListWindow_Dispatcher
ULONG SAVEDS ASM ListWindow_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( ListWindow_New( cl, obj, (APTR)msg ));

        case MUIM_ListWindow_Open:
            return( ListWindow_Open( cl, obj, (APTR)msg ));

        case MUIM_ListWindow_Close:
            return( ListWindow_Close( cl, obj, (APTR)msg ));

        case MUIM_ListWindow_Result:
            return( ListWindow_Result( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// ListWindow_New
static ULONG ListWindow_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct ListWindowData   temp = { 0 };
    static struct Hook      DispHook = {
                { NULL, NULL },
                (HOOKFUNC)List_DispHook,
                NULL, NULL
    };
    Object                 *Ok, *Cancel;

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            MUIA_Window_Title,      CM_GetString( MENU_THOR_USERS ),
                            MUIA_Window_ID,         MAKE_ID( 'L', 'I', 'S', 'T' ),

                            WindowContents, VGroup,

                                Child, temp.List = NListviewObject,
                                    MUIA_CycleChain,        TRUE,
                                    MUIA_NListview_NList,   NListObject,
                                        InputListFrame,
                                        MUIA_NList_AutoVisible,     TRUE,
                                        MUIA_NList_DisplayHook,     &DispHook,
                                    End,
                                End,

                                Child, HGroup, MUIA_Group_SameSize, TRUE,
                                    Child, Ok = MakeButton( GAD_OK, -1 ),
                                    Child, HSpace( 0 ),
                                    Child, Cancel = MakeButton( GAD_CANCEL, -1 ),
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct ListWindowData  *data = INST_DATA( cl, obj );

        *data = temp;

        set( obj, MUIA_Window_DefaultObject, temp.List );

        DoMethod( obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_ListWindow_Close, FALSE );

        /*  Gadgets */
        DoMethod( temp.List, MUIM_Notify, MUIA_NList_DoubleClick, TRUE, obj, 2, MUIM_ListWindow_Close, TRUE );

        DoMethod( Ok, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_ListWindow_Close, TRUE );
        DoMethod( Cancel, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_ListWindow_Close, FALSE );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// ListWindow_Open
static ULONG ListWindow_Open( struct IClass *cl, Object *obj, struct MUIP_ListWindow_Open *msg )
{
    struct ListWindowData  *data = INST_DATA( cl, obj );
    struct Node            *node;

    for( node = msg->List->lh_Head; node->ln_Succ; node = node->ln_Succ )
        DoMethod( data->List, MUIM_NList_InsertSingle, node, MUIV_NList_Insert_Bottom );

    set( obj, MUIA_Window_Open, TRUE );

    return( 0 );
}
///
/// ListWindow_Close
static ULONG ListWindow_Close( struct IClass *cl, Object *obj, struct MUIP_ListWindow_Close *msg )
{
    struct ListWindowData  *data = INST_DATA( cl, obj );

    if( msg->Ok )
        DoMethod( data->List, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &data->Node );

    DoMethod( _app( obj ), MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit );

    return( 0 );
}
///
/// ListWindow_Result
static ULONG ListWindow_Result( struct IClass *cl, Object *obj, Msg msg )
{
    struct ListWindowData  *data = INST_DATA( cl, obj );

    return(( ULONG )data->Node );
}
///

/// List_DispHook
static ULONG SAVEDS ASM List_DispHook( REG( a2 ) STRPTR *array, REG( a1 ) struct Node *entry )
{
    *array = entry->ln_Name;

    return( 0 );
}
///

