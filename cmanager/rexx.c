/***************************************************************************
    revision             : $Id: rexx.c,v 1.1.1.1 2003-01-06 15:54:48 tellini Exp $
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


#include <exec/types.h>                 // exec
#include <exec/memory.h>
#include <dos/dos.h>                    // dos
#include <dos/rdargs.h>
#include <dos/dosextens.h>
#include <rexx/storage.h>               // rexx
#include <rexx/errors.h>
#include <mui/nlist_mcc.h>              // mui
#include <libraries/mui.h>              // libraries
#include <clib/exec_protos.h>           // protos
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>
#include <clib/rexxsyslib_protos.h>
#include <clib/alib_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/dos_pragmas.h>
#include <pragmas/intuition_pragmas.h>
#include <pragmas/rexxsyslib_pragmas.h>

#include <mymacros.h>
#include <extrafuncs.h>

#include "CManager.h"
#include "main.h"
#include "mui.h"
#include "MCC/CManager_mcc.h"
///
/// Prototypes
static void SetStem( struct RexxMsg *, STRPTR, ULONG, STRPTR, STRPTR );
///

/// GetEntry
LONG SAVEDS ASM GetEntryRexxed( REG( a0 ) struct Hook *Hook, REG( a1 ) ULONG *ArgArray, REG( a2 ) Object *App )
{
    struct CMUser          *entry;
    struct MainWindowData  *data;
    TEXT                    buffer[512], var[10];
    ULONG                   len;
    struct RexxMsg         *rxmsg;
    static TEXT             rx_true[] = "1", rx_false[] = "0";

    data = (struct MainWindowData *) Hook->h_Data;

    DoMethod(( Object * )xget( data->CMObj, MUIA_CManager_ListObject ), MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &entry );

    if( entry == NULL )
        return( 10 );

    get( App, MUIA_Application_RexxMsg, &rxmsg );

    stccpy( buffer, (STRPTR)ArgArray[0], sizeof( buffer ));

    len = strlen( buffer );

    switch( entry->Type ) {

        case CME_USER:
            SetStem( rxmsg, buffer, len, ".TYPE",                "USER"             );
            SetStem( rxmsg, buffer, len, ".NAME",                entry->Name        );
            SetStem( rxmsg, buffer, len, ".LASTNAME",            entry->LastName    );
            SetStem( rxmsg, buffer, len, ".ADDRESS",             entry->Address     );
            SetStem( rxmsg, buffer, len, ".CITY",                entry->City        );
            SetStem( rxmsg, buffer, len, ".COUNTRY",             entry->Country     );
            SetStem( rxmsg, buffer, len, ".ZIP",                 entry->ZIP         );
            SetStem( rxmsg, buffer, len, ".ALIAS",               entry->Alias       );
            SetStem( rxmsg, buffer, len, ".COMMENT",             entry->Comment     );
            SetStem( rxmsg, buffer, len, ".EMAIL",               entry->EMail       );
            SetStem( rxmsg, buffer, len, ".WWW",                 entry->WWW         );
            SetStem( rxmsg, buffer, len, ".FTP",                 entry->FTP         );
            SetStem( rxmsg, buffer, len, ".PHONE",               entry->Phone       );
            SetStem( rxmsg, buffer, len, ".FAX",                 entry->Fax         );
            SetStem( rxmsg, buffer, len, ".MOBILE",              entry->Mobile      );
            SetStem( rxmsg, buffer, len, ".ICQ",                 entry->ICQ         );
            SetStem( rxmsg, buffer, len, ".BUSINESS.COMPANY",    entry->Company     );
            SetStem( rxmsg, buffer, len, ".BUSINESS.JOBTITLE",   entry->BJobTitle   );
            SetStem( rxmsg, buffer, len, ".BUSINESS.OFFICE",     entry->BOffice     );
            SetStem( rxmsg, buffer, len, ".BUSINESS.ADDRESS",    entry->BAddress    );
            SetStem( rxmsg, buffer, len, ".BUSINESS.CITY",       entry->BCity       );
            SetStem( rxmsg, buffer, len, ".BUSINESS.COUNTRY",    entry->BCountry    );
            SetStem( rxmsg, buffer, len, ".BUSINESS.ZIP",        entry->BZIP        );
            SetStem( rxmsg, buffer, len, ".BUSINESS.DEPARTMENT", entry->BDepartment );
            SetStem( rxmsg, buffer, len, ".BUSINESS.PHONE",      entry->BPhone      );
            SetStem( rxmsg, buffer, len, ".BUSINESS.FAX",        entry->BFax        );
            SetStem( rxmsg, buffer, len, ".BUSINESS.EMAIL",      entry->BEMail      );
            break;

        case CME_WWW:
            SetStem( rxmsg, buffer, len, ".TYPE",       "WWW"                              );
            SetStem( rxmsg, buffer, len, ".NAME",       ((struct CMWWW *) entry )->Name    );
            SetStem( rxmsg, buffer, len, ".COMMENT",    ((struct CMWWW *) entry )->Comment );
            SetStem( rxmsg, buffer, len, ".WWW",        ((struct CMWWW *) entry )->WWW     );
            break;

        case CME_FTP:
            SetStem( rxmsg, buffer, len, ".TYPE",       "FTP"                               );
            SetStem( rxmsg, buffer, len, ".NAME",       ((struct CMFTP *) entry )->Name     );
            SetStem( rxmsg, buffer, len, ".COMMENT",    ((struct CMFTP *) entry )->Comment  );
            SetStem( rxmsg, buffer, len, ".FTP",        ((struct CMFTP *) entry )->FTP      );
            SetStem( rxmsg, buffer, len, ".USERNAME",   ((struct CMFTP *) entry )->Username );
            SetStem( rxmsg, buffer, len, ".PASSWORD",   ((struct CMFTP *) entry )->Password );

            SetStem( rxmsg, buffer, len, ".ADVANCED",   (((struct CMFTP *)entry )->Flags & FTPF_ADVANCED ) ? rx_true : rx_false );
            SetStem( rxmsg, buffer, len, ".QUIET",      (((struct CMFTP *)entry )->Flags & FTPF_QUIET ) ? rx_true : rx_false );
            SetStem( rxmsg, buffer, len, ".COMPRESS",   (((struct CMFTP *)entry )->Flags & FTPF_COMPRESS ) ? rx_true : rx_false );
            SetStem( rxmsg, buffer, len, ".ANONYMOUS",  (((struct CMFTP *)entry )->Flags & FTPF_ANONYMOUS ) ? rx_true : rx_false );

            sprintf( var, "%ld", ((struct CMFTP *) entry )->Port );
            SetStem( rxmsg, buffer, len, ".PORT", var );

            sprintf( var, "%ld", ((struct CMFTP *) entry )->Retries );
            SetStem( rxmsg, buffer, len, ".RETRIES", var );
            break;

        case CME_CHAT:
            SetStem( rxmsg, buffer, len, ".TYPE",       "CHAT"                                 );
            SetStem( rxmsg, buffer, len, ".CHANNEL",    ((struct CMChat *) entry )->Channel    );
            SetStem( rxmsg, buffer, len, ".SERVER",     ((struct CMChat *) entry )->Server     );
            SetStem( rxmsg, buffer, len, ".MAINTAINER", ((struct CMChat *) entry )->Maintainer );
            SetStem( rxmsg, buffer, len, ".NICK",       ((struct CMChat *) entry )->Nick       );
            SetStem( rxmsg, buffer, len, ".WWW",        ((struct CMChat *) entry )->WWW        );
            SetStem( rxmsg, buffer, len, ".PASSWORD",   ((struct CMChat *) entry )->Password   );
            break;
    }

    return( 0L );
}
///

/// SetStem
static void SetStem( struct RexxMsg *rxmsg, STRPTR buffer, ULONG len, STRPTR stem, STRPTR var )
{
    strcat( buffer, stem );

    SetRexxVar(( struct Message * )rxmsg, buffer, var, strlen( var ));

    buffer[ len ] = '\0';
}
///
