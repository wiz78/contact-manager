/***************************************************************************
    revision             : $Id: main.c,v 1.1.1.1 2003-01-06 15:53:52 tellini Exp $
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
#define ASL_V38_NAMES_ONLY
#define CATCOMP_NUMBERS

#include <exec/types.h>                 // exec
#include <exec/execbase.h>
#include <exec/libraries.h>
#include <exec/memory.h>
#include <dos/dos.h>                    // dos
#include <dos/rdargs.h>
#include <dos/notify.h>
#include <workbench/workbench.h>        // workbench
#include <libraries/mui.h>              // libraries
#include <libraries/asl.h>
#include <libraries/gadtools.h>
#include <libraries/genesis.h>
#include <mui/nlistview_mcc.h>          // mui
#include <mui/nlist_mcc.h>
#include <mui/textinput_mcc.h>
#include <mui/listtree_mcc.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/locale.h>
#include <proto/icon.h>
#include <proto/genesis.h>

#include <clib/muimaster_protos.h>      // protos
#include <clib/vupdate_protos.h>
#include <clib/alib_protos.h>
#include <pragmas/muimaster_pragmas.h>  // pragmas
#include <pragmas/genesis_pragmas.h>
#include <pragmas/vupdate_pragmas.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <mymacros.h>
#include <extrafuncs.h>

#include "CManager.h"
#include "main.h"
#include "mui.h"
#include "MCC/entrylist.h"
#include "MCC/grouplist.h"
#include "rexx.h"
#include "import.h"
#include "about.h"
#include "prefs.h"
#include "locale.h"

#include "Library/CManagerMsg.h"
#include "Library/CManager_protos.h"
#include "Library/CManager_lib_lib.h"

#include "MCC/CManager_mcc.h"

#include "CManager_rev.h"
///
/// Defines
#define NEWSTACK_SIZE       (32 * 1024)

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d)    ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

enum { IMPORT_V = 0, IMPORT_THOR, IMPORT_IB, IMPORT_AWEB, IMPORT_MD, IMPORT_YAM,
       IMPORT_AMFTP, IMPORT_STFAX, IMPORT_AMIRC, IMPORT_DOPUSFTP, IMPORT_DFA,
       IMPORT_PINT, IMPORT_GOODNEWS, IMPORT_STRICQ, IMPORT_USER_CSV };

enum { EXPORT_URL_HTML = 0, EXPORT_WWW_HTML, EXPORT_FTP_HTML,
       EXPORT_USER_CSV };

enum { LOAD_NEWUSER = 0, LOAD_SAMEUSER, LOAD_RELOAD };
///
/// Prototypes
extern unsigned char Version[];

BOOL            GetFile( Object *, struct MainWindowData *, STRPTR, STRPTR, BOOL );

static void     Setup( void );
static void     CleanUp( ULONG );
static void     ParseArguments( void );
static BOOL     Login( Object * );
static void     SetNames( void );
static void     ChangeDir( void );

int             main( int, char ** );
static void     RealMain( void );

static ULONG                MainWindow_New( struct IClass *, Object *, struct opSet * );
static ULONG                MainWindow_Dispose( struct IClass *, Object *, struct opSet * );
static ULONG                MainWindow_Open( struct IClass *, Object *, struct opSet * );
static ULONG                MainWindow_Close( struct IClass *, Object *, struct MUIP_MainWindow_Close * );
static ULONG                MainWindow_Load( struct IClass *, Object *, struct MUIP_MainWindow_Load * );
static ULONG                MainWindow_Import( struct IClass *, Object *, struct MUIP_MainWindow_Import * );
static ULONG                MainWindow_Export( struct IClass *cl, Object *obj, struct MUIP_MainWindow_Export *msg );
static ULONG                MainWindow_About( struct IClass *, Object *, Msg );

static void                 LoadCountries( void );
static void                 FreeCountries( void );

static void SAVEDS          CM_ClickHook( REG( a0 ) struct Hook *hook, REG( a1 ) struct CMUser *entry );
static void                 GetMaskSize( struct CMUser *entry, ULONG *mask, ULONG *size );
///
/// ARexx commands
static struct Hook  GetEntry_RX = {
                            { NULL, NULL },
                            (HOOKFUNC)GetEntryRexxed,
                            NULL, NULL
                    };

static struct MUI_Command ARexxCmds[] =
{
    { "GETENTRY", "STEM/A", 1, &GetEntry_RX },
    { NULL, NULL, 0, NULL }
};
///
/// Data
struct Library         *MUIMasterBase;
struct Library         *LocaleBase;
struct Library         *UtilityBase;
struct IntuitionBase   *IntuitionBase;
struct Library         *IconBase;
struct Library         *CManagerBase;
struct Library         *GenesisBase;
struct Library         *DataTypesBase;
struct Library         *OpenURLBase;
struct Library         *IFFParseBase;
struct Library         *VUPBase;

UBYTE                   Flags = SAVED;

static struct StackSwapStruct   StackSwapper;
static struct Process          *MyTask;

#ifdef _DCC
extern struct WBStartup *_WBMsg;
#else
static struct WBStartup *_WBMsg;
#endif

extern struct ExecBase *SysBase;

static APTR                 vuphandle;

static struct DiskObject   *SleepIcon;
static BPTR                 old_Dir, DirLock;

TEXT            PubScreen[256];

static TEXT     CurrentUser[41];
static TEXT     Password[60];
static TEXT     DataName[256];

static TEXT     LayoutFile[] = "PROGDIR:CM.layout";
static ULONG    TreeGroupWeight = 80, ListGroupWeight = 100;

struct Locale  *Locale;

struct MsgPort *LibPort;
struct MsgPort *MyLibPort;

struct OrderPrefs   Order = { TRUE, FALSE };

struct MinList  Countries = {(struct MinNode *)&Countries.mlh_Tail, NULL, (struct MinNode *)&Countries.mlh_Head };

ULONG           AcceptEntry;

Object         *MainWin;
///


/// FPutString
void FPutString( BPTR file, STRPTR str )
{
    UWORD   len;

    len = strlen( str );

    FPutC( file, len );

    FWrite( file, str, len, 1 );
}
///
/// FGetString
void FGetString( BPTR file, STRPTR str )
{
    UWORD   len;

    len = FGetC( file );

    FRead( file, str, len, 1 );

    str[ len ] = '\0';
}
///

/// Setup
static void Setup( void )
{
    BPTR    file;
    APTR    wndptr;

    if(!( IntuitionBase = (struct IntuitionBase *) OpenLibrary( "intuition.library", 37 )))
        CleanUp( 20 );

    if(!( MUIMasterBase = OpenLibrary( "muimaster.library", MUIMASTER_VMIN )))
        CleanUp( 20 );

    if(!( UtilityBase = OpenLibrary( "utility.library", 37 )))
        CleanUp( 20 );

    if(!( IconBase = OpenLibrary( "icon.library", 36 )))
        CleanUp( 20 );

    if(!( LocaleBase = OpenLibrary( "locale.library", 38 )))
        CleanUp( 20 );

    if(!( CManagerBase = OpenLibrary( "CManager.library", 8 )))
        CleanUp( 20 );

    if(!( IFFParseBase = OpenLibrary( "iffparse.library", 36 )))
        CleanUp( 20 );

    if( VUPBase = OpenLibrary( "vapor_update.library", 1 ))
        vuphandle = VUP_BeginCheckUpdate( 100, ( VERSION << 24 ) | ( REVISION << 16 ), VERS" ("DATE")" );

    wndptr               = MyTask->pr_WindowPtr;
    MyTask->pr_WindowPtr = (APTR)-1;

    DataTypesBase  = OpenLibrary( "datatypes.library",  37 );
    GenesisBase    = OpenLibrary( GENESISNAME,          2  );
    OpenURLBase    = OpenLibrary( "openurl.library",    1  );

    MyTask->pr_WindowPtr = wndptr;

    if(!( MyLibPort = CreateMsgPort()))
        CleanUp( 20 );

    if(!( InitClasses() ))
        CleanUp( 20 );

    Locale = OpenLocale( NULL );

    if( file = Open( LayoutFile, MODE_OLDFILE )) {

        FRead( file, &TreeGroupWeight, sizeof( ULONG ), 1 );
        FRead( file, &ListGroupWeight, sizeof( ULONG ), 1 );

        Close( file );
    }

    ParseArguments();
}
///
/// Cleaup
static void CleanUp( ULONG RetCode )
{
    if( SleepIcon )
        FreeDiskObject( SleepIcon );

    if( old_Dir )
        CurrentDir( old_Dir );

    if( DirLock )
        UnLock( DirLock );

    CloseLocale( Locale );

    ExitClasses();

    if( MyLibPort ) {
        struct Message *msg;

        while( msg = GetMsg( MyLibPort ))
            ReplyMsg( msg );

        DeleteMsgPort( MyLibPort );
    }

    if( Flags & STACK_SWAPPED ) {
        StackSwap( &StackSwapper );
        FreeMem( StackSwapper.stk_Lower, NEWSTACK_SIZE );
    }

    if( VUPBase ) {
        VUP_Quit( vuphandle );
        CloseLibrary( VUPBase );
    }

    if( IFFParseBase   )    CloseLibrary( IFFParseBase   );
    if( CManagerBase   )    CloseLibrary( CManagerBase   );
    if( OpenURLBase    )    CloseLibrary( OpenURLBase    );
    if( GenesisBase    )    CloseLibrary( GenesisBase    );
    if( IconBase       )    CloseLibrary( IconBase       );
    if( MUIMasterBase  )    CloseLibrary( MUIMasterBase  );
    if( DataTypesBase  )    CloseLibrary( DataTypesBase  );
    if( LocaleBase     )    CloseLibrary( LocaleBase     );
    if( UtilityBase    )    CloseLibrary( UtilityBase    );
    if( IntuitionBase  )    CloseLibrary((struct Library *)IntuitionBase );

    exit( RetCode );
}
///

/// Parse Arguments
enum {
    ARG_FILE = 0, ARG_PUBSCREEN, ARG_PORT, ARG_NOLOGIN,
    ARG_USER, ARG_PASSWORD,
    NUM_ARGS
};

static void ParseArguments( void )
{
    struct WBArg       *Args;
    struct RDArgs      *Arguments;
    struct DiskObject  *icon;
    ULONG               ArgArray[ NUM_ARGS ];
    TEXT                MyName[256];
    BPTR                f;

    memset( ArgArray, 0, sizeof( ArgArray ));

    if (!( _WBMsg )) {
        TEXT    buffer[256];

        NameFromLock( (BPTR)GetProgramDir(), MyName, sizeof( MyName ));
        GetProgramName( buffer, sizeof( buffer ));
        AddPart( MyName, buffer, sizeof( MyName ));

        if( Arguments = ReadArgs( "FILE,PUBSCREEN/K,PORT/K/N,NOLOGIN/S,USER/K,PASSWORD/K",
                                  (LONG *)ArgArray, NULL )) {

            if( ArgArray[ ARG_FILE ]) {
                stccpy( DataName, (STRPTR)ArgArray[ ARG_FILE ], sizeof( DataName ));
                Flags |= LOAD_FILE;
            }

            if( ArgArray[ ARG_PUBSCREEN ])
                stccpy( PubScreen, (STRPTR)ArgArray[ ARG_PUBSCREEN ], sizeof( PubScreen ));

            if( ArgArray[ ARG_PORT ]) {

                LibPort = *((struct MsgPort **)ArgArray[ ARG_PORT ]);

#ifdef DEBUG
                kprintf( "\nStarting CManager\n" );
#endif
                SendLibMsg( CMC_PORT, MyLibPort );

                Flags |= SEND_AREXX;
            }

            if( ArgArray[ ARG_NOLOGIN ])
                Flags |= NO_LOGIN;

            if( ArgArray[ ARG_USER ]) {
                stccpy( CurrentUser, (STRPTR)ArgArray[ ARG_USER ], sizeof( CurrentUser ));
                Flags |= FIXED_LOGIN;
            }

            if( ArgArray[ ARG_PASSWORD ])
                stccpy( Password, (STRPTR)ArgArray[ ARG_PASSWORD ], sizeof( Password ));
        }

        icon = GetDiskObject( "PROGDIR:CManager" );

    } else {
        BPTR    old_dir;

        Args = _WBMsg->sm_ArgList;

        old_dir = CurrentDir( Args[0].wa_Lock );

        NameFromLock( Args[0].wa_Lock, MyName, sizeof( MyName ));
        AddPart( MyName, Args[0].wa_Name, sizeof( MyName ));

        if( icon = GetDiskObject( Args[0].wa_Name )) {
            STRPTR  value;

            if( value = FindToolType( icon->do_ToolTypes, "FILE" )) {
                stccpy( DataName, value, sizeof( DataName ));
                Flags |= LOAD_FILE;
            }

            if( FindToolType( icon->do_ToolTypes, "PUBSCREEN" ))
                stccpy( PubScreen, value, sizeof( PubScreen ));

            if( FindToolType( icon->do_ToolTypes, "NOLOGIN" ))
                Flags |= NO_LOGIN;

            if( value = FindToolType( icon->do_ToolTypes, "USER" )) {
                stccpy( CurrentUser, value, sizeof( CurrentUser ));
                Flags |= FIXED_LOGIN;
            }

            if( value = FindToolType( icon->do_ToolTypes, "PASSWORD" ))
                stccpy( Password, value, sizeof( Password ));
        }

        CurrentDir( old_dir );
    }

    if( icon )
        SleepIcon = icon;

    if(!( f = Lock( "ENV:Vapor/CManager_path", ACCESS_READ ))) {

        UnLock( CreateDir( "ENV:Vapor" ));
        UnLock( CreateDir( "ENVARC:Vapor" ));

        if( f = Open( "ENVARC:Vapor/CManager_path", MODE_NEWFILE )) {
            Write( f, MyName, strlen( MyName ));
            Close( f );
        }

        if( f = Open( "ENV:Vapor/CManager_path", MODE_NEWFILE )) {
            Write( f, MyName, strlen( MyName ));
            Close( f );
        }

    } else
        UnLock( f );
}
///
/// Login
static BOOL Login( Object *App )
{
    if( GenesisBase && (!( Flags & NO_LOGIN ))) {
        struct User    *ud = NULL;

        if(!( Flags & FIXED_LOGIN ))
            ud = GetGlobalUser();

        if(!( ud )) {
            TEXT    buf[41];

            GetUserName( 0, buf, 40 );

            ud = GetUser(( Flags & FIXED_LOGIN ) ? CurrentUser : buf, Password[0] ? Password : NULL, NULL, 0 );
        }

        if( ud ) {

            SetGlobalUser( ud );

            strcpy( CurrentUser, ud->us_name );

            SetNames();

            FreeUser( ud );

        } else
            return( FALSE );
    }

    ChangeDir();

    return( TRUE );
}
///
/// SetNames
static void SetNames( void )
{
    sprintf( DataName,  "%s.data",    CurrentUser );
    sprintf( PrefsFile, "%s.prefs",   CurrentUser );
}
///
/// ChangeDir
static void ChangeDir( void )
{
    static TEXT Dir[256] = "PROGDIR:";

    if( CurrentUser[0] )
        sprintf( Dir, "PROGDIR:%s", CurrentUser );

    if( DirLock ) {

        CurrentDir( old_Dir );

        UnLock( DirLock );
    }

    if(!( DirLock = Lock( Dir, ACCESS_READ )))
        DirLock = CreateDir( Dir );

    if( DirLock )
        old_Dir = CurrentDir( DirLock );
}
///

/// main
void wbmain( void *Msg )
{
#ifndef _DCC
    _WBMsg = Msg;
#endif

    main( 0, NULL );
}

int main( int argc, char *argv[] )
{
    if( SysBase->LibNode.lib_Version < 37 ) {

        if( IntuitionBase = (struct IntuitionBase *) OpenLibrary( "intuition.library", 0 )) {
            static UBYTE AlertData[] = "\0\214\020Contact Manager requires kickstart v37+ !!!\0\0";

            DisplayAlert( RECOVERY_ALERT, AlertData, 30 );

            CloseLibrary(( struct Library * )IntuitionBase );
        }

        exit( 30 );
    }

    MyTask = (struct Process *) FindTask( NULL );

    if(((ULONG)MyTask->pr_Task.tc_SPUpper - (ULONG)MyTask->pr_Task.tc_SPLower ) < NEWSTACK_SIZE ) {

        if(!( StackSwapper.stk_Lower = AllocMem( NEWSTACK_SIZE, 0L )))
            CleanUp( 20 );

        StackSwapper.stk_Upper   = (ULONG)StackSwapper.stk_Lower + NEWSTACK_SIZE;
        StackSwapper.stk_Pointer = (APTR)StackSwapper.stk_Upper;

        StackSwap( &StackSwapper );
        Flags |= STACK_SWAPPED;
    }

    RealMain();
}
///
/// RealMain
static void RealMain( void )
{
    Object *AppObj;

    Setup();

    if( AppObj = NewObject( CL_MyApplication->mcc_Class, NULL,

                        MUIA_Application_Title,         "Contact Manager",
                        MUIA_Application_Version,       &Version[1],
                        MUIA_Application_Copyright,     "Copyright ©1998-2000 by Simone Tellini",
                        MUIA_Application_Author,        "Simone Tellini <wiz@vapor.com>",
                        MUIA_Application_Description,   CM_GetString( MSG_APP_DESCRIPTION ),
                        MUIA_Application_Base,          "CONTACTMAN",
                        MUIA_Application_Commands,      ARexxCmds,
                        MUIA_Application_DiskObject,    SleepIcon,

                        SubWindow, MainWin = NewObject( CL_MainWindow->mcc_Class, NULL,
                            PubScreen[0] ? MUIA_Window_PublicScreen : TAG_IGNORE,   PubScreen,
                        End,
                   End ) {

        struct MainWindowData  *data; 

        if( Flags & SEND_AREXX )
            SendLibMsg( CMC_AREXX, (APTR)xget( AppObj, MUIA_Application_Base ));

        GetEntry_RX.h_Data = INST_DATA( CL_MainWindow->mcc_Class, MainWin );

        if( Login( AppObj )) {
            ULONG   sigs = 0;

            LoadPrefs( NULL );

            DoMethod( AppObj, MUIM_Application_Load, MUIV_Application_Load_ENV );

            DoMethod( MainWin, MUIM_MainWindow_Open );

            while( DoMethod( AppObj, MUIM_Application_NewInput, &sigs ) != MUIV_Application_ReturnID_Quit ) {
                if( sigs ) {

                    sigs = Wait( sigs | SIGBREAKF_CTRL_C );

                    if( sigs & SIGBREAKF_CTRL_C )
                        break;
                }
            }

            DoMethod( AppObj, MUIM_Application_Save, MUIV_Application_Save_ENV    );
            DoMethod( AppObj, MUIM_Application_Save, MUIV_Application_Save_ENVARC );
        }

        if( AcceptEntry )
            SendLibMsg( CMC_GET_ENTRY, NULL );

        data = INST_DATA( CL_MainWindow->mcc_Class, MainWin );

        DoMethod( data->CMObj, MUIM_CManager_Cleanup );

        MUI_DisposeObject( AppObj );
    }

    CleanUp( 0 );
}
///

/// MainWindow_Dispatcher
ULONG SAVEDS ASM MainWindow_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( MainWindow_New( cl, obj, (APTR)msg ));

        case OM_DISPOSE:
            return( MainWindow_Dispose( cl, obj, (APTR)msg ));

        case MUIM_MainWindow_Open:
            return( MainWindow_Open( cl, obj, (APTR)msg ));

        case MUIM_MainWindow_Close:
            return( MainWindow_Close( cl, obj, (APTR)msg ));

        case MUIM_MainWindow_Load:
            return( MainWindow_Load( cl, obj, (APTR)msg ));

        case MUIM_MainWindow_Import:
            return( MainWindow_Import( cl, obj, (APTR)msg ));

        case MUIM_MainWindow_Export:
            return( MainWindow_Export( cl, obj, (APTR)msg ));

        case MUIM_MainWindow_About:
            return( MainWindow_About( cl, obj, (APTR)msg ));

        case MUIM_MainWindow_Prefs:
            return( MainWindow_Prefs( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// MainWindow_New
static ULONG MainWindow_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct MainWindowData   temp = { 0 };
    enum { M_OPEN = 1, M_SAVE, M_SAVE_AS, M_ABOUT, M_ABOUT_MUI, M_QUIT, M_SEARCH,
           M_IMPORT_V, M_IMPORT_THOR, M_IMPORT_IB, M_IMPORT_AWEB, M_IMPORT_AMFTP,
           M_IMPORT_YAM, M_IMPORT_MD, M_IMPORT_STFAX, M_IMPORT_AMIRC, M_IMPORT_DOPUSFTP,
           M_PREFS, M_PREFS_MUI, M_IMPORT_DFA, M_IMPORT_PINT, M_IMPORT_GOODNEWS,
           M_IMPORT_STRICQ, M_EXPORT_URL_HTML, M_EXPORT_WWW_HTML, M_EXPORT_FTP_HTML,
           M_EXPORT_USER_CSV, M_IMPORT_USER_CSV };
    static struct NewMenu   MainNewMenu[] = {
                NM_TITLE, (STRPTR)MENU_PROJECT, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)MENU_OPEN, (STRPTR)KEY_OPEN, 0, 0, (APTR)M_OPEN,
                  NM_ITEM, (STRPTR)MENU_SAVE, (STRPTR)KEY_SAVE, 0, 0, (APTR)M_SAVE,
                  NM_ITEM, (STRPTR)MENU_SAVE_AS, (STRPTR)KEY_SAVE_AS, 0, 0, (APTR)M_SAVE_AS,
                  NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)MENU_ABOUT, (STRPTR)KEY_ABOUT, 0, 0, (APTR)M_ABOUT,
                  NM_ITEM, (STRPTR)MENU_ABOUT_MUI, NULL, 0, 0, (APTR)M_ABOUT_MUI,
                  NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)MENU_QUIT, (STRPTR)KEY_QUIT, 0, 0, (APTR)M_QUIT,
                NM_TITLE, (STRPTR)MENU_OPTIONS, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)MENU_SEARCH, (STRPTR)NULL, 0, 0, (APTR)M_SEARCH,
                  NM_ITEM, (STRPTR)MENU_IMPORT, (STRPTR)NULL, 0, 0, NULL,
                    NM_SUB, (STRPTR)MENU_V_HOTLIST, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_V,
                    NM_SUB, (STRPTR)MENU_IB_HOTLIST, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_IB,
                    NM_SUB, (STRPTR)MENU_AWEB_HOTLIST, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_AWEB,
                    NM_SUB, (STRPTR)MENU_AMFTP_PROFILES, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_AMFTP,
                    NM_SUB, (STRPTR)MENU_DOPUS_FTP, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_DOPUSFTP,
                    NM_SUB, (STRPTR)MENU_THOR_USERS, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_THOR,
                    NM_SUB, (STRPTR)MENU_MD_ADDRESSES, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_MD,
                    NM_SUB, (STRPTR)MENU_YAM_ADDRESSES, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_YAM,
                    NM_SUB, (STRPTR)MENU_STRICQ_USERS, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_STRICQ,
                    NM_SUB, (STRPTR)MENU_DFA_ADDRESSES, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_DFA,
                    NM_SUB, (STRPTR)MENU_UMS_PINT, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_PINT,
                    NM_SUB, (STRPTR)MENU_UMS_GOODNEWS, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_GOODNEWS,
                    NM_SUB, (STRPTR)MENU_STFAX_PHONEBOOK, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_STFAX,
                    NM_SUB, (STRPTR)MENU_AMIRC_SERVERS, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_AMIRC,
                    NM_SUB, (STRPTR)MENU_EXPORT_USERS_CSV, (STRPTR)NULL, 0, 0, (APTR)M_IMPORT_USER_CSV,
                  NM_ITEM, (STRPTR)MENU_EXPORT, (STRPTR)NULL, 0, 0, NULL,
                    NM_SUB, (STRPTR)MENU_EXPORT_USERS_CSV, (STRPTR)NULL, 0, 0, (APTR)M_EXPORT_USER_CSV,
                    NM_SUB, (STRPTR)MENU_EXPORT_URL_HTML, (STRPTR)NULL, 0, 0, (APTR)M_EXPORT_URL_HTML,
                    NM_SUB, (STRPTR)MENU_EXPORT_WWW_HTML, (STRPTR)NULL, 0, 0, (APTR)M_EXPORT_WWW_HTML,
                    NM_SUB, (STRPTR)MENU_EXPORT_FTP_HTML, (STRPTR)NULL, 0, 0, (APTR)M_EXPORT_FTP_HTML,
                NM_TITLE, (STRPTR)MENU_SETTINGS, NULL, 0, 0, NULL,
                  NM_ITEM, (STRPTR)MENU_CMANAGER, (STRPTR)NULL, 0, 0, (APTR)M_PREFS,
                  NM_ITEM, (STRPTR)MENU_MUI, (STRPTR)NULL, 0, 0, (APTR)M_PREFS_MUI,
                NM_END, NULL, NULL, 0, 0L, NULL
        };
    Object     *strip;

    LocalizeMenus( MainNewMenu );

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            MUIA_Window_Title,      "Contact Manager ©1998-2000 by Simone Tellini",
                            MUIA_Window_ID,         MAKE_ID( 'M', 'A', 'I', 'N' ),
                            MUIA_Window_Menustrip,  strip = MUI_MakeObject( MUIO_MenustripNM, MainNewMenu, 0 ),

                            WindowContents, VGroup,

                                Child, temp.CMObj = CManagerObject,
                                    MUIA_ObjectID,  MAKE_ID( 'C', 'M', 'D', 'B' ),
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct MainWindowData  *data = INST_DATA( cl, obj );

        *data = temp;

        data->ClickHook.h_Entry = (HOOKFUNC) CM_ClickHook;
        data->ClickHook.h_Data  = obj;

//        set( obj, MUIA_Window_DefaultObject, Tree );

        /*  Window  */

        DoMethod( obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_MainWindow_Close, FALSE );

        /*  Gadgets */

        /*  Menus   */
        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_ABOUT_MUI ),
                  MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
                  MUIV_Notify_Application, 2, MUIM_Application_AboutMUI, obj );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_ABOUT ),
                  MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
                  obj, 1, MUIM_MainWindow_About );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_QUIT ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Close, FALSE );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_OPEN ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Load, LOAD_SAMEUSER );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_SAVE ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, temp.CMObj, 3, MUIM_CManager_SaveData, FALSE, NULL );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_SAVE_AS ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, temp.CMObj, 3, MUIM_CManager_SaveData, TRUE, NULL );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_SEARCH ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, temp.CMObj, 1, MUIM_CManager_Search );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_V ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_V );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_IB ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_IB );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_AWEB ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_AWEB );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_MD ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_MD );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_AMFTP ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_AMFTP );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_DOPUSFTP ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_DOPUSFTP );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_THOR ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_THOR );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_YAM ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_YAM );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_STRICQ ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_STRICQ );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_DFA ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_DFA );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_PINT ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_PINT );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_GOODNEWS ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_GOODNEWS );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_STFAX ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_STFAX );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_AMIRC ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_AMIRC );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_IMPORT_USER_CSV ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Import, IMPORT_USER_CSV );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_PREFS ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 1, MUIM_MainWindow_Prefs );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_PREFS_MUI ),
                  MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime,
                  MUIV_Notify_Application, 2, MUIM_Application_OpenConfigWindow, 0 );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_EXPORT_URL_HTML ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Export, EXPORT_URL_HTML );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_EXPORT_WWW_HTML ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Export, EXPORT_WWW_HTML );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_EXPORT_FTP_HTML ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Export, EXPORT_FTP_HTML );

        DoMethod(( Object * )DoMethod( strip, MUIM_FindUData, M_EXPORT_USER_CSV ),
                 MUIM_Notify, MUIA_Menuitem_Trigger, MUIV_EveryTime, obj, 2, MUIM_MainWindow_Export, EXPORT_USER_CSV );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// MainWindow_Dispose
static ULONG MainWindow_Dispose( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct MainWindowData  *data = INST_DATA( cl, obj );

    if( data->Req )
        MUI_FreeAslRequest( data->Req );

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///
/// MainWindow_Open
static ULONG MainWindow_Open( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct MainWindowData  *data = INST_DATA( cl, obj );

    data->Req = MUI_AllocAslRequest( ASL_FileRequest, NULL );

    if(( Flags & LOAD_FILE ) && DataName[0] )
        DoMethod( data->CMObj, MUIM_CManager_LoadData, NULL, DataName );
    else
        DoMethod( data->CMObj, MUIM_CManager_LoadData, CurrentUser[0] ? CurrentUser : NULL, NULL );

    DoMethod( data->CMObj, MUIM_CManager_GrabLists );

    ScanHotlists(( Object * )xget( obj, MUIA_ApplicationObject ), data->CMObj, (struct CMData *)xget( data->CMObj, MUIA_CManager_CMData ));

    DoMethod( data->CMObj, MUIM_CManager_ReinsertLists );

    if( SaveOnChanges )
        DoMethod( data->CMObj, MUIM_Notify, MUIA_CManager_Changed, TRUE,
                  data->CMObj, 3, MUIM_CManager_SaveData, FALSE, NULL );

    set( obj, MUIA_Window_Open, TRUE );

    return( 0 );
}
///
/// MainWindow_Close
static ULONG MainWindow_Close( struct IClass *cl, Object *obj, struct MUIP_MainWindow_Close *msg )
{
    struct MainWindowData  *data = INST_DATA( cl, obj );

    if( xget( data->CMObj, MUIA_CManager_Changed )) {

        set( obj, MUIA_Window_Sleep, TRUE );

        switch( SaveOnExit ? 1 : MUI_RequestA( _app( obj ), obj, 0, NULL,
                                               CM_GetString( ANS_SAVE_QUIT_CANCEL ),
                                               CM_GetString( MSG_NOT_SAVED ), NULL )) {

            case 0:
                set( obj, MUIA_Window_Sleep, FALSE );
                return( 0 );

            case 1:
                DoMethod( data->CMObj, MUIM_CManager_SaveData, FALSE, NULL );
                break;
        }
    }

    DoMethod(( Object * )xget( obj, MUIA_ApplicationObject ), MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit );

    return( 0 );
}
///
/// MainWindow_Load
static ULONG MainWindow_Load( struct IClass *cl, Object *obj, struct MUIP_MainWindow_Load *msg )
{
    struct MainWindowData  *data = INST_DATA( cl, obj );
    BOOL                    ok;
    TEXT                    DataName[256];
    struct CMData          *cm;

    get( data->CMObj, MUIA_CManager_CMData, &cm );

    strcpy( DataName, cm->Path );

    if( msg->Type == LOAD_SAMEUSER )
        ok = GetFile( obj, data, DataName, NULL, FALSE );

    if( ok ) {

        set( obj, MUIA_Window_Sleep, TRUE );

        if( msg->Type == LOAD_SAMEUSER )
            DoMethod( data->CMObj, MUIM_CManager_LoadData, NULL, DataName );
        else
            DoMethod( data->CMObj, MUIM_CManager_LoadData, CurrentUser[0] ? CurrentUser : NULL, NULL );

        if( msg->Type == LOAD_NEWUSER ) {

            LoadPrefs( NULL );

            DoMethod( data->CMObj, MUIM_CManager_GrabLists );

            ScanHotlists( _app( obj ), data->CMObj, (struct CMData *)xget( data->CMObj, MUIA_CManager_CMData ));

            DoMethod( data->CMObj, MUIM_CManager_ReinsertLists );
        }

        set( obj, MUIA_Window_Sleep, FALSE );
    }

    return( 0 );
}
///
/// MainWindow_Import
static ULONG MainWindow_Import( struct IClass *cl, Object *obj, struct MUIP_MainWindow_Import *msg )
{
    struct MainWindowData          *data = INST_DATA( cl, obj );
    TEXT                            name[256];
    struct CMData                  *cm;
    BOOL                            done = TRUE;

    get( data->CMObj, MUIA_CManager_CMData, &cm );

    set( obj, MUIA_Window_Sleep, TRUE );

    DoMethod( data->CMObj, MUIM_CManager_GrabLists );

    if( msg->From == IMPORT_THOR ) {

        ImportTHOR( _app( obj ), cm->Users );

        done = FALSE;

    } else {
        STRPTR  pattern = NULL, fname;

        switch( msg->From ) {

            case IMPORT_V:
                pattern = "Bookmarks.html";
                fname   = Hotlists[ HL_V ];
                break;

            case IMPORT_IB:
                pattern = "#?.html";
                fname   = Hotlists[ HL_IB ];
                break;

            case IMPORT_AWEB:
                pattern = "#?.hotlist";
                fname   = Hotlists[ HL_AWEB ];
                break;

            case IMPORT_AMFTP:
                pattern = "#?.AmFTPProfiles";
                fname   = Hotlists[ HL_AMFTP ];
                break;

            case IMPORT_MD:
                pattern = "#?.AddressBook";
                fname   = Hotlists[ HL_MD2 ];
                break;

            case IMPORT_YAM:
                pattern = "#?.AddressBook";
                fname   = Hotlists[ HL_YAM ];
                break;

            case IMPORT_STFAX:
                pattern = "#?";
                fname   = Hotlists[ HL_STFAX ];
                break;

            case IMPORT_AMIRC:
                pattern = "#?.AmIRCfg";
                fname   = Hotlists[ HL_AMIRC ];
                break;

            case IMPORT_DOPUSFTP:
                pattern = "#?.config";
                fname   = Hotlists[ HL_DOPUS ];
                break;

            case IMPORT_DFA:
                pattern = "#?.dfa";
                fname   = Hotlists[ HL_DFA ];
                break;

            case IMPORT_PINT:
                pattern = "#?.addresses";
                fname   = Hotlists[ HL_PINT ];
                break;

            case IMPORT_GOODNEWS:
                pattern = "#?.addresses";
                fname   = Hotlists[ HL_GOODNEWS ];
                break;

            case IMPORT_STRICQ:
                pattern = "Contacts#?";
                fname   = Hotlists[ HL_STRICQ ];
                break;

            case IMPORT_USER_CSV:
                pattern = "#?.csv";
                fname   = "";
                break;
        }

        strcpy( name, fname );

        if( GetFile( obj, data, name, pattern, FALSE )) {
            BPTR    file;
            ULONG   type;

            switch( msg->From ) {

                case IMPORT_V:
                    type = MUIV_CManager_Import_Voyager;
                    break;

                case IMPORT_IB:
                    type = MUIV_CManager_Import_IB;
                    break;

                case IMPORT_AWEB:
                    type = MUIV_CManager_Import_AWeb;
                    break;

                case IMPORT_USER_CSV:
                    type = MUIV_CManager_Import_Users_CSV;
                    break;

                default:
                    done = FALSE;
                    break;
            }

            if( done ) {

                DoMethod( data->CMObj, MUIM_CManager_Import, type, name, MUIV_CManager_Import_Filter );

            } else {

                if( file = Open( name, MODE_OLDFILE )) {

                    switch( msg->From ) {

                        case IMPORT_AMFTP:
                            ImportAmFTP( file, cm->FTPs, FALSE );
                            break;

                        case IMPORT_MD:
                            ImportMD( file, cm->Users, FALSE );
                            break;

                        case IMPORT_YAM:
                            ImportYAM( file, cm->Users, FALSE );
                            break;

                        case IMPORT_DFA:
                            ImportDFA( file, cm->Users, FALSE );
                            break;

                        case IMPORT_PINT:
                            ImportPINT( file, cm->Users, FALSE );
                            break;

                        case IMPORT_GOODNEWS:
                            ImportGoodNews( file, cm->Users, FALSE );
                            break;

                        case IMPORT_STRICQ:
                            ImportSTRICQ( file, cm->Users, FALSE );
                            break;

                        case IMPORT_STFAX:
                            ImportSTFax( file, cm->Users, FALSE );
                            break;

                        case IMPORT_AMIRC:
                            ImportAmIRC( file, cm->Chat, FALSE );
                            break;

                        case IMPORT_DOPUSFTP:
                            ImportDOpusFTP( file, cm->FTPs, FALSE );
                            break;
                    }

                    Close( file );

                } else
                    DisplayBeep( NULL );
            }
        }
    }

    if( !done ) // not needed if the .mcc did the import
        DoMethod( data->CMObj, MUIM_CManager_ReinsertLists );

    set( obj, MUIA_Window_Sleep, FALSE );

    return( 0 );
}
///
/// MainWindow_Export
static ULONG MainWindow_Export( struct IClass *cl, Object *obj, struct MUIP_MainWindow_Export *msg )
{
    struct MainWindowData          *data = INST_DATA( cl, obj );
    TEXT                            name[256];
    STRPTR                          pattern = NULL;
    ULONG                           type;

    name[0] = '\0';

    switch( msg->Type ) {

        case EXPORT_URL_HTML:
            pattern = "#?.html";
            type    = MUIV_CManager_Export_HTML_URLs;
            break;

        case EXPORT_WWW_HTML:
            pattern = "#?.html";
            type    = MUIV_CManager_Export_HTML_WWW;
            break;

        case EXPORT_FTP_HTML:
            pattern = "#?.html";
            type    = MUIV_CManager_Export_HTML_FTP;
            break;

        case EXPORT_USER_CSV:
            pattern = "#?.csv";
            type    = MUIV_CManager_Export_CSV_Users;
            break;
    }

    if( GetFile( obj, data, name, pattern, TRUE ))
        DoMethod( data->CMObj, MUIM_CManager_Export, type, name, 0 );

    return( 0 );
}
///
/// MainWindow_About
static ULONG MainWindow_About( struct IClass *cl, Object *obj, Msg msg )
{
    Object *wnd;

    if( wnd = NewObject( CL_AboutWindow->mcc_Class, NULL, TAG_DONE )) {

        DoMethod( _app( obj ), OM_ADDMEMBER, wnd );

        set( wnd, MUIA_Window_Open, TRUE );
    }

    return( 0 );
}
///

/// GetFile
BOOL GetFile( Object *Win, struct MainWindowData *data, STRPTR File, STRPTR Pattern, BOOL SaveMode )
{
    BOOL    ok = FALSE;

    if( data->Req ) {
        TEXT    dir[256];
        APTR    scr;

        strcpy( dir, File );

        *( PathPart( dir )) = '\0';

        get( Win, MUIA_Window_Screen, &scr );

        if( MUI_AslRequestTags( data->Req,
                                ASLFR_DoPatterns,                                TRUE,
                                ASLFR_DoSaveMode,                                SaveMode,
                                Pattern ? ASLFR_InitialPattern : TAG_IGNORE,     Pattern,
                                File[0] ? ASLFR_InitialFile : TAG_IGNORE,        FilePart( File ),
                                File[0] ? ASLFR_InitialDrawer : TAG_IGNORE,      dir,
                                scr ? ASLFR_Screen : TAG_IGNORE,                 scr,
                                TAG_DONE )) {

            strcpy( File, data->Req->fr_Drawer );
            AddPart( File, data->Req->fr_File, 256 );

            ok = TRUE;
        }
    }

    return( ok );
}
///

/// LoadCountries
static void LoadCountries( void )
{
    BPTR    file;

    if( file = Open( "PROGDIR:CountryCodes.txt", MODE_OLDFILE )) {
        TEXT    buf[1024];

        while( FGets( file, buf, sizeof( buf ) - 1 )) {
            struct CSource      cs;
            struct CMCountry   *country;

            cs.CS_Buffer = buf;
            cs.CS_Length = strlen( buf );
            cs.CS_CurChr = 0;

            if( country = AllocMem( sizeof( struct CMCountry ), MEMF_CLEAR )) {

                AddTail(( struct List * )&Countries, ( struct Node * )country );

                ReadItem( country->Country,  80, &cs );
                ReadItem( country->Country2, 80, &cs );
                ReadItem( country->Code,     8,  &cs );

            } else
                break;
        }

        Close( file );
    }
}
///
/// FreeCountries
static void FreeCountries( void )
{
    struct CMCountry   *country;

    while( country = (struct CMCountry *) RemTail(( struct List * )&Countries ))
        FreeMem( country, sizeof( struct CMCountry ));
}
///

/// SendLibMsg
void SendLibMsg( ULONG Command, APTR Data )
{
    struct CMMessage    msg = { 0 };

#ifdef DEBUG
    kprintf( "CManager - SendLibMsg( %ld, %08lx );\n", Command, Data );
#endif

    msg.Msg.mn_ReplyPort = MyLibPort;
    msg.Msg.mn_Length    = sizeof( msg );
    msg.Command          = Command;
    msg.Data             = (ULONG)Data;

    PutMsg( LibPort, (struct Message *)&msg );

#ifdef DEBUG
    kprintf( "CManager - SendLibMsg() - WaitPort()\n" );
#endif

    do {
        WaitPort( MyLibPort );
    } while(!( GetMsg( MyLibPort )));

#ifdef DEBUG
    kprintf( "CManager - SendLibMsg() - done\n" );
#endif
}
///

/// CM_ClickHook
static void SAVEDS CM_ClickHook( REG( a0 ) struct Hook *hook, REG( a1 ) struct CMUser *entry )
{
    Object                 *obj = (Object *) hook->h_Data;
    struct MainWindowData  *data = INST_DATA( CL_MainWindow->mcc_Class, obj );

    if( AcceptEntry & CMGE_MULTISELECT ) {
        struct MinList *list;
        Object         *listobj;

        get( data->CMObj, MUIA_CManager_ListObject, &listobj );

        if( list = CM_AllocEntry( CME_LIST )) {
            LONG    pos;

            pos = MUIV_NList_NextSelected_Start;

            DoMethod( listobj, MUIM_NList_Select, MUIV_NList_Select_Active, MUIV_NList_Select_On, NULL );

            DoMethod( listobj, MUIM_NList_NextSelected, &pos );

            while( pos != MUIV_NList_NextSelected_End ) {
                struct CMUser  *from, *to;
                ULONG           mask, size;

                DoMethod( listobj, MUIM_NList_GetEntry, pos, &from );

                GetMaskSize( from, &mask, &size );

                if(!( AcceptEntry & mask )) {
                    CM_FreeList( list );
                    return;
                }

                if( to = CM_AllocEntry( from->Type )) {

                    CopyMem( from, to, size );

                    AddTail(( struct List * )list, ( struct Node * )to );
                }

                DoMethod( listobj, MUIM_NList_NextSelected, &pos );
            }

            SendLibMsg( CMC_GET_ENTRY, list );

            AcceptEntry = 0;

            set( data->CMObj, MUIA_CManager_AppDoubleClick, NULL );
        }

    } else if( entry &&  AcceptEntry ) {
        ULONG   mask = 0, size;

        GetMaskSize( entry, &mask, &size );

        if( AcceptEntry & mask ) {
            struct CMUser  *newentry;

            if( newentry = CM_AllocEntry( entry->Type ))
                CopyMem( entry, newentry, size );

            SendLibMsg( CMC_GET_ENTRY, newentry );

            AcceptEntry = 0;

            set( data->CMObj, MUIA_CManager_AppDoubleClick, NULL );
        }
    }
}
///
/// GetMaskSize
static void GetMaskSize( struct CMUser *entry, ULONG *mask, ULONG *size )
{
    switch( entry->Type ) {

        case CME_USER:
            *mask = CMGE_USER;
            *size = sizeof( struct CMUser );
            break;

        case CME_WWW:
            *mask = CMGE_WWW;
            *size = sizeof( struct CMWWW );
            break;

        case CME_FTP:
            *mask = CMGE_FTP;
            *size = sizeof( struct CMFTP );
            break;

        case CME_CHAT:
            *mask = CMGE_CHAT;
            *size = sizeof( struct CMChat );
            break;
    }
}
///
