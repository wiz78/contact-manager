/***************************************************************************
    revision             : $Id: prefs.c,v 1.1.1.1 2003-01-06 15:54:37 tellini Exp $
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
#include <exec/memory.h>
#include <exec/execbase.h>
#include <intuition/intuition.h>        // intuition
#include <dos/dos.h>                    // dos
#include <libraries/mui.h>              // libraries
#include <libraries/locale.h>
#include <libraries/asl.h>
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/locale_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/locale_pragmas.h>

#include <mymacros.h>

#include "CManager.h"
#ifndef CMANAGER_MCC
#include "import.h"
#include "main.h"
#include "mui.h"
#else
#include "MCC/dial.h"
#endif
#include "prefs.h"
#include "locale.h"

#include "MCC/CManager_mcc.h"

#include "Library/CManager_protos.h"
#include "Library/CManager_lib_lib.h"

#include "/SpeedBar/SpeedBar_mcc.h"
#include "/SpeedBar/SpeedBarCfg/SpeedBarCfg_mcc.h"
///
/// Prototypes
#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d)    ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#ifndef CMANAGER_MCC

extern struct ExecBase *SysBase;

static ULONG    PrefsWindow_New( struct IClass *, Object *, struct opSet * );
static ULONG    PrefsWindow_Open( struct IClass *, Object *, struct MUIP_PrefsWindow_Open * );
static ULONG    PrefsWindow_Close( struct IClass *, Object *, struct MUIP_PrefsWindow_Close * );
static ULONG    PrefsWindow_GetCountry( struct IClass *, Object *, Msg );
static ULONG    PrefsWindow_SetString( struct IClass *, Object *, struct MUIP_PrefsWindow_SetString * );
static ULONG    PrefsWindow_SetDialerGroup( struct IClass *, Object *, Msg );

static void SAVEDS ASM Device_ObjStrHook( REG( a2 ) Object *, REG( a1 ) Object * );
static void SAVEDS ASM Modem_ObjStrHook( REG( a2 ) Object *, REG( a1 ) Object * );

static void     SavePrefs( STRPTR );
static void     SaveDates( void );
static void     ReadStamps( void );
static BOOL     FileNew( Object *, STRPTR, struct DateStamp * );
static void     ReImport( Object *, ULONG, struct CMGroup *, void ( * )( BPTR, struct CMGroup *, BOOL ));
static void     ReImport2( Object *App, Object *CMObj, ULONG Hot, ULONG Type );

#else

extern void FGetString( BPTR, STRPTR );

#endif
///
/// Data
#ifndef CMANAGER_MCC
struct MUI_CustomClass *CL_PrefsWindow;

static Object          *PrefsWnd;
#endif

TEXT    Country[80];
TEXT    CountryCode[9];

BOOL    SilentScan;
BOOL    UseOpenURL;

#ifndef CMANAGER_MCC
TEXT    PrefsFile[256] = "CManager.prefs";

TEXT    SerDevice[256] = "serial.device";
UBYTE   SerUnit = 0;
BOOL    SerShared = FALSE;
TEXT    SerInit[256] = "AT&F";
TEXT    DialCommand[30] = "ATDT";
UWORD   RedialDelay = 30;
UWORD   RedialAttempts;
BOOL    LogCalls;
#endif

TEXT    Hotlists[ NUM_HOTLISTS ][256];

UWORD   Action;

static struct DateStamp Stamps[ NUM_HOTLISTS ];
static TEXT             DatesFile[] = "Hotlist.stamps";

BOOL    DontMoveCols = TRUE, SaveOnExit, SaveOnChanges;
BOOL    UseDOpus, UseLister;
TEXT    Lister[256];

UWORD   UseDialler = 1;

static UWORD    ModemType = 0;

static UBYTE    WWWProg = 0, FTPProg = 0, MailProg = 0, IRCProg = 0;

TEXT    WWWPath[256], FTPPath[256], MailPath[256], IRCPath[256];

TEXT    WWWScript[256]  = "SendV.rexx";
TEXT    FTPScript[256]  = "SendAmFTP.rexx";
TEXT    MailScript[256] = "SendMD.rexx";
TEXT    IRCScript[256]  = "SendAmIRC.rexx";
TEXT    DialProg[256]   = "Rx SendPhone.rexx \"%s\"";

struct MUIS_SpeedBarCfg_Config  ToolbarCfg = {
                                    MUIV_SpeedBar_ViewMode_Text,
                                    0
                                };
///


#ifndef CMANAGER_MCC
/// MainWindow_Prefs
ULONG MainWindow_Prefs( struct IClass *cl, Object *obj, Msg msg )
{
    struct MainWindowData  *data = INST_DATA( cl, obj );

    if( PrefsWnd ) {

        set( PrefsWnd, MUIA_Window_Activate, TRUE );

        DoMethod( PrefsWnd, MUIM_Window_ToFront );

    } else if( PrefsWnd = NewObject( CL_PrefsWindow->mcc_Class, NULL, TAG_DONE )) {

        DoMethod( _app( obj ), OM_ADDMEMBER, PrefsWnd );

        DoMethod( PrefsWnd, MUIM_PrefsWindow_Open, data->CMObj );
    }

    return( 0 );
}
///

/// PrefsWindow_Dispatcher
ULONG SAVEDS ASM PrefsWindow_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( PrefsWindow_New( cl, obj, (APTR)msg ));

        case MUIM_PrefsWindow_Open:
            return( PrefsWindow_Open( cl, obj, (APTR)msg ));

        case MUIM_PrefsWindow_Close:
            return( PrefsWindow_Close( cl, obj, (APTR)msg ));

        case MUIM_PrefsWindow_GetCountry:
            return( PrefsWindow_GetCountry( cl, obj, (APTR)msg ));

        case MUIM_PrefsWindow_SetString:
            return( PrefsWindow_SetString( cl, obj, (APTR)msg ));

        case MUIM_PrefsWindow_SetDialerGroup:
            return( PrefsWindow_SetDialerGroup( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// PrefsWindow_New
static ULONG PrefsWindow_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct PrefsWindowData  temp = { 0 };
    static STRPTR           Labels[] = {
                                (STRPTR)MENU_SETTINGS,
                                (STRPTR)MSG_PROGRAMS,
                                (STRPTR)MSG_PATHS,
                                (STRPTR)MSG_DIALLER,
                                (STRPTR)MSG_DOPUS_FTP,
                                NULL
                            };
    static STRPTR           Actions[] = {
                                (STRPTR)MSG_EDIT_ENTRIES,
                                (STRPTR)MSG_START_SCRIPT,
                                (STRPTR)MSG_SCRIPT_AND_CLOSE,
                                (STRPTR)MSG_SCRIPT_AND_ICONIFY,
                                NULL
                            };
    static STRPTR           WWWProgs[] = {
                                "Voyager",
                                "IBrowse",
                                "AWeb",
                                "---",
                                NULL
                            };
    static STRPTR           MailProgs[] = {
                                "Microdot II",
                                "THOR",
                                "YAM",
                                "---",
                                NULL
                            };
    static STRPTR           FTPProgs[] = {
                                "AmFTP",
                                "---",
                                NULL
                            };
    static STRPTR           IRCProgs[] = {
                                "AmIRC",
                                "---",
                                NULL
                            };
    static STRPTR           WWWScripts[] = {
                                "SendV.rexx",
                                "SendIB.rexx",
                                "SendAWeb.rexx",
                                ""
                            };
    static STRPTR           FTPScripts[] = {
                                "SendAmFTP.rexx",
                                ""
                            };
    static STRPTR           MailScripts[] = {
                                "SendMD.rexx",
                                "SendThor.rexx",
                                "SendYAM.rexx",
                                ""
                            };
    static STRPTR           IRCScripts[] = {
                                "SendAmIRC.rexx",
                                ""
                            };
    static STRPTR           DialerOptions[] = {
                                (STRPTR)MSG_USE_EXT_DIALER,
                                (STRPTR)GAD_USE_DIALLER,
                                NULL
                            };
    static BOOL             FirstTime = TRUE;
    static struct Hook      Dev_ObjStrHook = {
                                { NULL, NULL },
                                (HOOKFUNC)Device_ObjStrHook,
                                NULL, NULL
                            };
    static struct Hook      Mod_ObjStrHook = {
                                { NULL, NULL },
                                (HOOKFUNC)Modem_ObjStrHook,
                                NULL, NULL
                            };
    Object                 *Save, *Use, *Cancel, *GetCountry, *DevList, *PopDev, *PopModem;
    Object                 *wwwgroup;

    if( FirstTime ) {
        FirstTime = FALSE;
        LocalizeArray( Labels        );
        LocalizeArray( Actions       );
        LocalizeArray( DialerOptions );
    }

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            MUIA_Window_Title,      CM_GetString( MENU_SETTINGS ),
                            MUIA_Window_ID,         MAKE_ID( 'P', 'R', 'F', 'S' ),
                            PubScreen[0] ? MUIA_Window_PublicScreen : TAG_IGNORE, PubScreen,

                            WindowContents, VGroup,

                                Child, RegisterGroup( Labels ),

                                    /*              Settings                */
                                    Child, VGroup,
                                        Child, VSpace( 0 ),

                                        Child, ColGroup( 2 ),

                                            GroupFrameT( CM_GetString( MSG_LOCALE )),

                                            Child, MakeLabel1( MSG_COUNTRY ),
                                            Child, temp.Country = MakeString( 80, MSG_COUNTRY, HELP_COUNTRY ),

                                            Child, MakeLabel1( GAD_COUNTRY_CODE ),
                                            Child, HGroup,
                                                Child, temp.CountryCode = MakeString( 8, GAD_COUNTRY_CODE, HELP_COUNTRY_CODE ),
                                                Child, GetCountry = MakeButton( GAD_FROM_LOCALE, -1 ),
                                            End,
                                        End,

                                        Child, ColGroup( 2 ),

                                            GroupFrameT( CM_GetString( MSG_MISC )),

                                            Child, MakeLabel1( GAD_SILENT_SCAN ),
                                            Child, HGroup,
                                                Child, temp.SilentScan = MakeCheck( GAD_SILENT_SCAN, HELP_SILENT_SCAN ),
                                                Child, HSpace( 0 ),
                                                Child, MakeLabel1( GAD_OPEN_URL ),
                                                Child, temp.OpenURL = MakeCheck( GAD_OPEN_URL, -1 ),
                                            End,

                                            Child, MakeLabel1( MSG_ON_DOUBLE_CLICK ),
                                            Child, temp.DoubleClick = MakeCycle( Actions, MSG_ON_DOUBLE_CLICK ),

                                            Child, MakeLabel1( GAD_DONT_MOVE_COLUMNS ),
                                            Child, HGroup,
                                                Child, temp.MoveCols = MakeCheck( GAD_DONT_MOVE_COLUMNS, HELP_DONT_MOVE_COLUMNS ),
                                                Child, HSpace( 0 ),
                                                Child, MakeLabel1( GAD_SAVE_ON_EXIT ),
                                                Child, temp.SaveOnExit = MakeCheck( GAD_SAVE_ON_EXIT, HELP_SAVE_ON_EXIT ),
                                            End,

                                            Child, HSpace( 0 ),
                                            Child, HGroup,
                                                Child, HSpace( 0 ),
                                                Child, MakeLabel1( GAD_SAVE_ON_CHANGES ),
                                                Child, temp.SaveOnChanges = MakeCheck( GAD_SAVE_ON_CHANGES, HELP_SAVE_ON_CHANGES ),
                                            End,
                                        End,

                                        Child, VGroup,

                                            GroupFrameT( CM_GetString( MSG_TOOLBAR )),

                                            Child, temp.ToolbarCfg = SpeedBarCfgObject,
                                                MUIA_SpeedBarCfg_Config,    &ToolbarCfg,
                                            End,
                                        End,

                                        Child, VSpace( 0 ),
                                    End,

                                    /*              Programs                */
                                    Child, VGroup,
                                        Child, VSpace( 0 ),

                                        Child, wwwgroup = ColGroup( 2 ),

                                            MUIA_Group_VertSpacing, 0,

                                            Child, MakeLabel1( GAD_WWW ),
                                            Child, temp.WWWProg = MakeCycle( WWWProgs, GAD_WWW ),

                                            Child, MakeLabel1( MSG_SCRIPT ),
                                            Child, MakeFile( &temp.WWWScript, 256, MSG_SCRIPT, -1 ),

                                            Child, MakeLabel1( MSG_PATH ),
                                            Child, MakeFile( &temp.WWWPath, 256, MSG_PATH, -1 ),
                                        End,

                                        Child, ColGroup( 2 ),

                                            MUIA_Group_VertSpacing, 0,

                                            Child, MakeLabel1( GAD_FTP ),
                                            Child, temp.FTPProg = MakeCycle( FTPProgs, GAD_FTP ),

                                            Child, MakeLabel1( MSG_SCRIPT ),
                                            Child, MakeFile( &temp.FTPScript, 256, MSG_SCRIPT, -1 ),

                                            Child, MakeLabel1( MSG_PATH ),
                                            Child, MakeFile( &temp.FTPPath, 256, MSG_PATH, -1 ),
                                        End,

                                        Child, ColGroup( 2 ),

                                            MUIA_Group_VertSpacing, 0,

                                            Child, MakeLabel1( GAD_MAILER ),
                                            Child, temp.MailProg = MakeCycle( MailProgs, GAD_MAILER ),

                                            Child, MakeLabel1( MSG_SCRIPT ),
                                            Child, MakeFile( &temp.MailScript, 256, MSG_SCRIPT, -1 ),

                                            Child, MakeLabel1( MSG_PATH ),
                                            Child, MakeFile( &temp.MailPath, 256, MSG_PATH, -1 ),
                                        End,

                                        Child, ColGroup( 2 ),

                                            MUIA_Group_VertSpacing, 0,

                                            Child, MakeLabel1( GAD_IRC ),
                                            Child, temp.IRCProg = MakeCycle( IRCProgs, GAD_IRC ),

                                            Child, MakeLabel1( MSG_SCRIPT ),
                                            Child, MakeFile( &temp.IRCScript, 256, MSG_SCRIPT, -1 ),

                                            Child, MakeLabel1( MSG_PATH ),
                                            Child, MakeFile( &temp.IRCPath, 256, MSG_PATH, -1 ),
                                        End,

                                        Child, VSpace( 0 ),
                                    End,

                                    /*              Hotlists                */
                                    Child, ColGroup( 2 ),

                                        Child, MakeLabel1( GAD_V ),
                                        Child, MakeFile( &temp.Voyager, 256, GAD_V, -1 ),

                                        Child, MakeLabel1( GAD_IB ),
                                        Child, MakeFile( &temp.IB, 256, GAD_IB, -1 ),

                                        Child, MakeLabel1( GAD_AWEB ),
                                        Child, MakeFile( &temp.AWeb, 256, GAD_AWEB, -1 ),

                                        Child, MakeLabel1( GAD_MD2 ),
                                        Child, MakeFile( &temp.MD2, 256, GAD_MD2, -1 ),

                                        Child, MakeLabel1( GAD_YAM ),
                                        Child, MakeFile( &temp.YAM, 256, GAD_YAM, -1 ),

                                        Child, MakeLabel1( GAD_STRICQ_USERS ),
                                        Child, MakeFile( &temp.STRICQ, 256, GAD_STRICQ_USERS, -1 ),

                                        Child, MakeLabel1( GAD_GOODNEWS ),
                                        Child, MakeFile( &temp.GoodNews, 256, GAD_GOODNEWS, -1 ),

                                        Child, MakeLabel1( GAD_UMS ),
                                        Child, MakeFile( &temp.PINT, 256, GAD_UMS, -1 ),

                                        Child, MakeLabel1( GAD_DFA ),
                                        Child, MakeFile( &temp.DFA, 256, GAD_DFA, -1 ),

                                        Child, MakeLabel1( GAD_AMFTP ),
                                        Child, MakeFile( &temp.AmFTP, 256, GAD_AMFTP, -1 ),

                                        Child, MakeLabel1( GAD_STFAX ),
                                        Child, MakeFile( &temp.STFax, 256, GAD_STFAX, -1 ),

                                        Child, MakeLabel1( GAD_AMIRC ),
                                        Child, MakeFile( &temp.AmIRC, 256, GAD_AMIRC, -1 ),

                                        Child, MakeLabel1( GAD_DOPUSFTP ),
                                        Child, MakeFile( &temp.DOpus, 256, GAD_DOPUSFTP, -1 ),
                                    End,

                                    /*              Dialer                  */
                                    Child, VGroup,
                                        Child, VSpace( 0 ),

                                        Child, temp.UseDialler = MakeCycle( DialerOptions, 0 ),

                                        Child, temp.ExtDialGroup = VGroup,

                                            MUIA_Group_Spacing, 0,

                                            Child, HGroup,
                                                Child, MakeLabel1( GAD_PROGRAM ),
                                                Child, MakeFile( &temp.DialProg, 256, GAD_PROGRAM, -1 ),
                                            End,

                                            Child, TextObject,
                                                MUIA_Font,              MUIV_Font_Tiny,
                                                MUIA_Text_Contents,     CM_GetString( MSG_DIALER_PLACEHOLDERS ),
                                                MUIA_Text_PreParse,     "\033c",
                                            End,
                                        End,

                                        Child, temp.DialGroup = ColGroup( 2 ),

                                            MUIA_ShowMe,    FALSE,

                                            Child, MakeLabel1( GAD_DEVICE ),
                                            Child, PopDev = PopobjectObject,
                                                    MUIA_Popstring_String,      temp.DialDevice = MakeString( 256, GAD_DEVICE, -1 ),
                                                    MUIA_Popstring_Button,      PopButton( MUII_PopUp ),
                                                    MUIA_Popobject_ObjStrHook,  &Dev_ObjStrHook,
                                                    MUIA_Popobject_Object,      DevList = ListviewObject,
                                                            MUIA_Listview_List, ListObject,
                                                                InputListFrame,
                                                                MUIA_List_ConstructHook,    MUIV_List_ConstructHook_String,
                                                                MUIA_List_DestructHook,     MUIV_List_DestructHook_String,
                                                            End,
                                                    End,
                                            End,

                                            Child, MakeLabel1( GAD_UNIT ),
                                            Child, HGroup,
                                                Child, temp.DialUnit = MakeInteger( 3, GAD_UNIT, -1 ),
                                                Child, MakeLabel1( GAD_SHARED ),
                                                Child, temp.DialShared = MakeCheck( GAD_SHARED, -1 ),
                                            End,

                                            Child, MakeLabel1( GAD_MODEM_TYPE ),
                                            Child, PopModem = PopobjectObject,
                                                    MUIA_Popstring_String,      temp.ModemName = TextObject,
                                                                                                        TextFrame,
                                                                                                        MUIA_Background,    MUII_TextBack,
                                                                                                End,
                                                    MUIA_Popstring_Button,      PopButton( MUII_PopUp ),
                                                    MUIA_Popobject_ObjStrHook,  &Mod_ObjStrHook,
                                                    MUIA_Popobject_Object,      temp.ModemList = ListviewObject,
                                                            MUIA_Listview_List, ListObject,
                                                                InputListFrame,
                                                                MUIA_List_ConstructHook,    MUIV_List_ConstructHook_String,
                                                                MUIA_List_DestructHook,     MUIV_List_DestructHook_String,
                                                            End,
                                                    End,
                                            End,

                                            Child, MakeLabel1( GAD_INIT_STRING ),
                                            Child, temp.DialInit = MakeString( 256, GAD_INIT_STRING, -1 ),

                                            Child, MakeLabel1( GAD_DIAL_COMMAND ),
                                            Child, temp.DialCommand = MakeString( 30, GAD_DIAL_COMMAND, -1 ),

                                            Child, MakeLabel1( GAD_REDIAL_DELAY ),
                                            Child, temp.RedialDelay = MakeSlider( GAD_REDIAL_DELAY, 0, 120 ),

                                            Child, MakeLabel1( GAD_REDIAL_ATTEMPTS ),
                                            Child, temp.RedialAttempts = MakeSlider( GAD_REDIAL_ATTEMPTS, 0, 99 ),

                                            Child, MakeLabel1( GAD_LOG_CALLS ),
                                            Child, HGroup,
                                                Child, temp.LogCalls = MakeCheck( GAD_LOG_CALLS, -1 ),
                                                Child, HSpace( 0 ),
                                            End,
                                        End,

                                        Child, VSpace( 0 ),
                                    End,

                                    /*              DOpus FTP               */
                                    Child, VGroup,
                                        Child, VSpace( 0 ),

                                        Child, ColGroup( 2 ),

                                            Child, MakeLabel1( MSG_DEFAULT_FTP ),
                                            Child, HGroup,
                                                Child, temp.UseDOpus = MakeCheck( MSG_DEFAULT_FTP, -1 ),
                                                Child, HSpace( 0 ),
                                            End,

                                            Child, MakeLabel1( MSG_LOCAL_LISTER ),
                                            Child, HGroup, MUIA_Group_Spacing, 0,
                                                Child, temp.UseLister = MakeCheck( MSG_LOCAL_LISTER, -1 ),
                                                Child, MakeFile( &temp.Lister, 256, MSG_LOCAL_LISTER, -1 ),
                                            End,
                                        End,

                                        Child, VSpace( 0 ),
                                    End,
                                End,

                                Child, HGroup,
                                    Child, Save = MakeButton( GAD_SAVE, -1 ),
                                    Child, HSpace( 0 ),
                                    Child, Use  = MakeButton( GAD_USE, -1 ),
                                    Child, HSpace( 0 ),
                                    Child, Cancel = MakeButton( GAD_CANCEL, -1 ),
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct PrefsWindowData *data = INST_DATA( cl, obj );
        BPTR                    lock;
        struct Node            *dev;

        *data = temp;

        set( temp.DoubleClick, MUIA_ShortHelp, CM_GetString( HELP_DOUBLE_CLICK ));

        set( temp.Lister, MUIA_Disabled, TRUE );

        if(!( LocaleBase ))
            set( GetCountry, MUIA_Disabled, TRUE );

        /*      Fill the device list    */

        if( lock = Lock( "DEVS:", ACCESS_READ )) {
            BPTR                OldDir;
            struct AnchorPath  *anchorpath;

            OldDir = CurrentDir( lock );

            if( anchorpath = (struct AnchorPath *)AllocMem( sizeof( struct AnchorPath ) ,MEMF_CLEAR )) {
                ULONG   error;

                error = MatchFirst( "#?.device", anchorpath );

                while( error == 0 ) {

                    DoMethod( DevList, MUIM_List_InsertSingle, anchorpath->ap_Info.fib_FileName, MUIV_List_Insert_Sorted );

                    error = MatchNext( anchorpath );
                }

                MatchEnd( anchorpath );

                FreeMem( anchorpath, sizeof( struct AnchorPath ));
            }

            CurrentDir( OldDir );

            UnLock( lock );
        }

        Forbid();   // let's add resident devices

        for( dev = SysBase->DeviceList.lh_Head; dev->ln_Succ; dev = dev->ln_Succ ) {
            ULONG   i;
            BOOL    add;

            add = TRUE;

            for( i = 0;; i++ ) {
                STRPTR  entry;

                DoMethod( DevList, MUIM_List_GetEntry, i, &entry );

                if( entry == NULL )
                    break;

                if( strcmp( entry, dev->ln_Name ) == 0 ) {
                    add = FALSE;
                    break;
                }
            }

            if( add )
                DoMethod( DevList, MUIM_List_InsertSingle, dev->ln_Name, MUIV_List_Insert_Sorted );
        }

        Permit();

        /*          Fill the modem type list    */

        if( lock = Open( "PROGDIR:Modems.txt", MODE_OLDFILE )) {
            TEXT    buf[512];

            while( FGets( lock, buf, sizeof( buf ) - 1 )) {
                struct CSource  cs;
                TEXT            modem[256];

                cs.CS_Buffer = buf;
                cs.CS_Length = strlen( buf );
                cs.CS_CurChr = 0;

                ReadItem( modem,  255, &cs );

                DoMethod( temp.ModemList, MUIM_List_InsertSingle, modem, MUIV_List_Insert_Bottom );
            }

            Close( lock );
        }

        /*  Window  */
        DoMethod( obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_PrefsWindow_Close, 0 );

        /*  Gadgets */
        DoMethod( GetCountry, MUIM_Notify, MUIA_Pressed, FALSE, obj, 1, MUIM_PrefsWindow_GetCountry );
        DoMethod( temp.OpenURL, MUIM_Notify, MUIA_Selected, MUIV_EveryTime, wwwgroup, 3, MUIM_Set, MUIA_Disabled, MUIV_TriggerValue );

        DoMethod( temp.WWWProg,  MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, obj, 4, MUIM_PrefsWindow_SetString, temp.WWWScript,  WWWScripts,  MUIV_TriggerValue );
        DoMethod( temp.FTPProg,  MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, obj, 4, MUIM_PrefsWindow_SetString, temp.FTPScript,  FTPScripts,  MUIV_TriggerValue );
        DoMethod( temp.MailProg, MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, obj, 4, MUIM_PrefsWindow_SetString, temp.MailScript, MailScripts, MUIV_TriggerValue );
        DoMethod( temp.IRCProg,  MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, obj, 4, MUIM_PrefsWindow_SetString, temp.IRCScript,  IRCScripts,  MUIV_TriggerValue );

        DoMethod( temp.UseLister, MUIM_Notify, MUIA_Selected, MUIV_EveryTime, temp.Lister, 3, MUIM_Set, MUIA_Disabled, MUIV_NotTriggerValue );

        DoMethod( temp.UseDialler, MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, obj, 1, MUIM_PrefsWindow_SetDialerGroup );
        DoMethod( DevList, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, PopDev, 2, MUIM_Popstring_Close, TRUE );
        DoMethod( temp.ModemList, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, PopModem, 2, MUIM_Popstring_Close, TRUE );

        DoMethod( Save,   MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_PrefsWindow_Close, 2 );
        DoMethod( Use,    MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_PrefsWindow_Close, 1 );
        DoMethod( Cancel, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_PrefsWindow_Close, 0 );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// PrefsWindow_Open
static ULONG PrefsWindow_Open( struct IClass *cl, Object *obj, struct MUIP_PrefsWindow_Open *msg )
{
    struct PrefsWindowData *data = INST_DATA( cl, obj );
    STRPTR                  str;

    data->CMObj = msg->CMObj;

    set( data->IB,       MUIA_String_Contents, Hotlists[ HL_IB       ]);
    set( data->Voyager,  MUIA_String_Contents, Hotlists[ HL_V        ]);
    set( data->AmFTP,    MUIA_String_Contents, Hotlists[ HL_AMFTP    ]);
    set( data->MD2,      MUIA_String_Contents, Hotlists[ HL_MD2      ]);
    set( data->STFax,    MUIA_String_Contents, Hotlists[ HL_STFAX    ]);
    set( data->YAM,      MUIA_String_Contents, Hotlists[ HL_YAM      ]);
    set( data->STRICQ,   MUIA_String_Contents, Hotlists[ HL_STRICQ   ]);
    set( data->PINT,     MUIA_String_Contents, Hotlists[ HL_PINT     ]);
    set( data->GoodNews, MUIA_String_Contents, Hotlists[ HL_GOODNEWS ]);
    set( data->DFA,      MUIA_String_Contents, Hotlists[ HL_DFA      ]);
    set( data->AWeb,     MUIA_String_Contents, Hotlists[ HL_AWEB     ]);
    set( data->AmIRC,    MUIA_String_Contents, Hotlists[ HL_AMIRC    ]);
    set( data->DOpus,    MUIA_String_Contents, Hotlists[ HL_DOPUS    ]);

    set( data->Country,     MUIA_String_Contents, Country     );
    set( data->CountryCode, MUIA_String_Contents, CountryCode );

    set( data->SilentScan,     MUIA_Selected,     SilentScan    );
    set( data->OpenURL,        MUIA_Selected,     UseOpenURL    );
    set( data->DoubleClick,    MUIA_Cycle_Active, Action        );
    set( data->MoveCols,       MUIA_Selected,     DontMoveCols  );
    set( data->SaveOnExit,     MUIA_Selected,     SaveOnExit    );
    set( data->SaveOnChanges,  MUIA_Selected,     SaveOnChanges );

    set( data->UseDOpus,    MUIA_Selected,          UseDOpus  );
    set( data->UseLister,   MUIA_Selected,          UseLister );
    set( data->Lister,      MUIA_String_Contents,   Lister    );

    set( data->UseDialler,      MUIA_Cycle_Active,      UseDialler     );
    set( data->DialDevice,      MUIA_String_Contents,   SerDevice      );
    set( data->DialUnit,        MUIA_String_Integer,    SerUnit        );
    set( data->DialShared,      MUIA_Selected,          SerShared      );
    set( data->DialInit,        MUIA_String_Contents,   SerInit        );
    set( data->DialCommand,     MUIA_String_Contents,   DialCommand    );
    set( data->RedialDelay,     MUIA_Slider_Level,      RedialDelay    );
    set( data->RedialAttempts,  MUIA_Slider_Level,      RedialAttempts );
    set( data->DialProg,        MUIA_String_Contents,   DialProg       );
    set( data->LogCalls,        MUIA_Selected,          LogCalls       );
    set( data->ModemList,       MUIA_List_Active,       ModemType      );

    DoMethod( data->ModemList, MUIM_List_GetEntry, ModemType, &str );

    set( data->ModemName, MUIA_Text_Contents, str );

    set( data->WWWProg,  MUIA_Cycle_Active, WWWProg  );
    set( data->FTPProg,  MUIA_Cycle_Active, FTPProg  );
    set( data->MailProg, MUIA_Cycle_Active, MailProg );
    set( data->IRCProg,  MUIA_Cycle_Active, IRCProg  );

    set( data->WWWPath,  MUIA_String_Contents, WWWPath  );
    set( data->FTPPath,  MUIA_String_Contents, FTPPath  );
    set( data->MailPath, MUIA_String_Contents, MailPath );
    set( data->IRCPath,  MUIA_String_Contents, IRCPath  );

    set( data->WWWScript,  MUIA_String_Contents, WWWScript  );
    set( data->FTPScript,  MUIA_String_Contents, FTPScript  );
    set( data->MailScript, MUIA_String_Contents, MailScript );
    set( data->IRCScript,  MUIA_String_Contents, IRCScript  );

    set( obj, MUIA_Window_Open, TRUE );

    return( 0 );
}
///
/// PrefsWindow_Close
static ULONG PrefsWindow_Close( struct IClass *cl, Object *obj, struct MUIP_PrefsWindow_Close *msg )
{
    struct PrefsWindowData *data = INST_DATA( cl, obj );

    if( msg->Ok ) {

        getstr( data->IB,       Hotlists[ HL_IB       ]);
        getstr( data->Voyager,  Hotlists[ HL_V        ]);
        getstr( data->AmFTP,    Hotlists[ HL_AMFTP    ]);
        getstr( data->MD2,      Hotlists[ HL_MD2      ]);
        getstr( data->STFax,    Hotlists[ HL_STFAX    ]);
        getstr( data->GoodNews, Hotlists[ HL_GOODNEWS ]);
        getstr( data->PINT,     Hotlists[ HL_PINT     ]);
        getstr( data->YAM,      Hotlists[ HL_YAM      ]);
        getstr( data->STRICQ,   Hotlists[ HL_STRICQ   ]);
        getstr( data->DFA,      Hotlists[ HL_DFA      ]);
        getstr( data->AWeb,     Hotlists[ HL_AWEB     ]);
        getstr( data->AmIRC,    Hotlists[ HL_AMIRC    ]);
        getstr( data->DOpus,    Hotlists[ HL_DOPUS    ]);

        getstr( data->Country,     Country     );
        getstr( data->CountryCode, CountryCode );

        SilentScan    = xget( data->SilentScan,    MUIA_Selected     );
        UseOpenURL    = xget( data->OpenURL,       MUIA_Selected     );
        Action        = xget( data->DoubleClick,   MUIA_Cycle_Active );
        DontMoveCols  = xget( data->MoveCols,      MUIA_Selected     );
        SaveOnExit    = xget( data->SaveOnExit,    MUIA_Selected     );
        SaveOnChanges = xget( data->SaveOnChanges, MUIA_Selected     );

        UseDOpus  = xget( data->UseDOpus,  MUIA_Selected );
        UseLister = xget( data->UseLister, MUIA_Selected );

        memcpy( &ToolbarCfg, (APTR)xget( data->ToolbarCfg, MUIA_SpeedBarCfg_Config ), sizeof( ToolbarCfg ));

        getstr( data->Lister, Lister );

        UseDialler = xget( data->UseDialler, MUIA_Cycle_Active );

        getstr( data->DialDevice, SerDevice );

        SerUnit        = xget( data->DialUnit,       MUIA_String_Integer );
        SerShared      = xget( data->DialShared,     MUIA_Selected       );
        ModemType      = xget( data->ModemList,      MUIA_List_Active    );
        RedialDelay    = xget( data->RedialDelay,    MUIA_Slider_Level   );
        RedialAttempts = xget( data->RedialAttempts, MUIA_Slider_Level   );
        LogCalls       = xget( data->LogCalls,       MUIA_Selected       );

        getstr( data->DialInit,    SerInit     );
        getstr( data->DialCommand, DialCommand );
        getstr( data->DialProg,    DialProg    );

        WWWProg  = xget( data->WWWProg,  MUIA_Cycle_Active );
        FTPProg  = xget( data->FTPProg,  MUIA_Cycle_Active );
        MailProg = xget( data->MailProg, MUIA_Cycle_Active );
        IRCProg  = xget( data->IRCProg,  MUIA_Cycle_Active );

        getstr( data->WWWScript,  WWWScript  );
        getstr( data->FTPScript,  FTPScript  );
        getstr( data->MailScript, MailScript );
        getstr( data->IRCScript,  IRCScript  );

        getstr( data->WWWPath,  WWWPath  );
        getstr( data->FTPPath,  FTPPath  );
        getstr( data->MailPath, MailPath );
        getstr( data->IRCPath,  IRCPath  );

        if( msg->Ok == 2 ) {

            ReadStamps();

            SaveDates();

            SavePrefs( PrefsFile );

            DoMethod( data->CMObj, MUIM_CManager_LoadPrefs, PrefsFile );

        } else {
            static TEXT     prefs[] = "T:CM.prefs";

            SavePrefs( prefs );

            DoMethod( data->CMObj, MUIM_CManager_LoadPrefs, prefs );

            DeleteFile( prefs );
        }

        DoMethod( data->CMObj, MUIM_KillNotify, MUIA_CManager_Changed );

        if( SaveOnChanges )
            DoMethod( data->CMObj, MUIM_Notify, MUIA_CManager_Changed, TRUE,
                      data->CMObj, 3, MUIM_CManager_SaveData, FALSE, NULL );
    }

    RemoveWindow( obj );

    PrefsWnd = NULL;

    return( 0 );
}
///
/// PrefsWindow_GetCountry
static ULONG PrefsWindow_GetCountry( struct IClass *cl, Object *obj, Msg msg )
{
    struct PrefsWindowData *data = INST_DATA( cl, obj );
    struct Locale          *locale;

    if( locale = OpenLocale( NULL )) {
        struct CMCountry   *c;

        set( data->CountryCode, MUIA_String_Integer, locale->loc_TelephoneCode );

        for( c = (struct CMCountry *)Countries.mlh_Head; c->Succ; c = c->Succ )
            if( locale->loc_TelephoneCode == atoi( c->Code )) {
                set( data->Country, MUIA_String_Contents, c->Country );
                break;
            }

        CloseLocale( locale );
    }

    return( 0 );
}
///
/// PrefsWindow_SetString
static ULONG PrefsWindow_SetString( struct IClass *cl, Object *obj, struct MUIP_PrefsWindow_SetString *msg )
{
    set( msg->Obj, MUIA_String_Contents, msg->Strings[ msg->Index ]);

    return( 0 );
}
///
/// PrefsWindow_SetDialerGroup
static ULONG PrefsWindow_SetDialerGroup( struct IClass *cl, Object *obj, Msg msg )
{
    struct PrefsWindowData *data = INST_DATA( cl, obj );
    ULONG                   which;
    Object                 *parent;

    get( data->UseDialler, MUIA_Cycle_Active, &which );

    get( data->DialGroup, MUIA_Parent, &parent );

    DoMethod( parent, MUIM_Group_InitChange );

    if( which ) {

        set( data->DialGroup,    MUIA_ShowMe, TRUE  );
        set( data->ExtDialGroup, MUIA_ShowMe, FALSE );

    } else {

        set( data->DialGroup,    MUIA_ShowMe, FALSE );
        set( data->ExtDialGroup, MUIA_ShowMe, TRUE  );
    }

    DoMethod( parent, MUIM_Group_ExitChange );

    return( 0 );
}
///

/// Device_ObjStrHook
static void SAVEDS ASM Device_ObjStrHook( REG( a2 ) Object *list, REG( a1 ) Object *str )
{
    STRPTR  entry;

    DoMethod( list, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &entry );

    set( str, MUIA_String_Contents, entry );
}
///
/// Modem_ObjStrHook
static void SAVEDS ASM Modem_ObjStrHook( REG( a2 ) Object *list, REG( a1 ) Object *str )
{
    STRPTR  entry;

    DoMethod( list, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &entry );

    set( str, MUIA_Text_Contents, entry );
}
///

/// SavePrefs
static void SavePrefs( STRPTR PrefsFile )
{
    BPTR    file;

    if( file = Open( PrefsFile, MODE_NEWFILE )) {
        ULONG   i;

        FPutString( file, Country     );
        FPutString( file, CountryCode );

        for( i = 0; i < 8; i++ )
            FPutString( file, Hotlists[ i ]);

        FWrite( file, &SilentScan, sizeof( SilentScan ), 1 );
        FWrite( file, &Action,     sizeof( Action     ), 1 );

        FPutString( file, Hotlists[ HL_DOPUS ]);

        FWrite( file, &UseDOpus,  sizeof( UseDOpus  ), 1 );
        FWrite( file, &UseLister, sizeof( UseLister ), 1 );

        FPutString( file, Lister );

        FWrite( file, &UseOpenURL, sizeof( UseOpenURL ), 1 );

        FWrite( file, &UseDialler, sizeof( UseDialler ), 1 );

        FPutString( file, SerDevice   );
        FPutString( file, SerInit     );
        FPutString( file, DialCommand );

        FPutC( file, SerUnit );

        FWrite( file, &SerShared, sizeof( SerShared ), 1 );

        FPutC( file, WWWProg  );
        FPutC( file, FTPProg  );
        FPutC( file, MailProg );

        FPutString( file, WWWPath  );
        FPutString( file, FTPPath  );
        FPutString( file, MailPath );

        FPutString( file, WWWScript  );
        FPutString( file, FTPScript  );
        FPutString( file, MailScript );

        FPutC( file, IRCProg );
        FPutString( file, IRCPath   );
        FPutString( file, IRCScript );

        FPutString( file, DialProg );

        FWrite( file, &RedialDelay,    sizeof( RedialDelay    ), 1 );
        FWrite( file, &RedialAttempts, sizeof( RedialAttempts ), 1 );

        FWrite( file, &ModemType, sizeof( ModemType ), 1 );

        FPutString( file, Hotlists[ HL_DFA ]);

        FPutC( file, SaveOnExit );

        FPutString( file, Hotlists[ HL_PINT     ]);
        FPutString( file, Hotlists[ HL_GOODNEWS ]);
        FPutString( file, Hotlists[ HL_STRICQ   ]);

        FPutC( file, DontMoveCols  );
        FPutC( file, SaveOnChanges );

        FWrite( file, &ToolbarCfg.ViewMode, sizeof( ToolbarCfg.ViewMode ), 1 );
        FWrite( file, &ToolbarCfg.Flags,    sizeof( ToolbarCfg.Flags    ), 1 );

        Close( file );
    }
}
///
#endif
/// LoadPrefs
void LoadPrefs( STRPTR prefs )
{
    BPTR    file;

#ifndef CMANAGER_MCC
    if( file = Open( prefs ? prefs : PrefsFile, MODE_OLDFILE )) {
#else
    if( file = Open( prefs, MODE_OLDFILE )) {
#endif
        ULONG   i;

        FGetString( file, Country     );
        FGetString( file, CountryCode );

        for( i = 0; i < 8; i++ )
            FGetString( file, Hotlists[ i ]);

        FRead( file, &SilentScan, sizeof( SilentScan ), 1 );
        FRead( file, &Action,     sizeof( Action     ), 1 );

        FGetString( file, Hotlists[ HL_DOPUS ]);

        FRead( file, &UseDOpus,  sizeof( UseDOpus  ), 1 );
        FRead( file, &UseLister, sizeof( UseLister ), 1 );

        FGetString( file, Lister );

        FRead( file, &UseOpenURL, sizeof( UseOpenURL ), 1 );

        FRead( file, &UseDialler, sizeof( UseDialler ), 1 );

        FGetString( file, SerDevice   );
        FGetString( file, SerInit     );
        FGetString( file, DialCommand );

        SerUnit = FGetC( file );

        FRead( file, &SerShared, sizeof( SerShared ), 1 );

        WWWProg  = FGetC( file );
        FTPProg  = FGetC( file );
        MailProg = FGetC( file );

        FGetString( file, WWWPath  );
        FGetString( file, FTPPath  );
        FGetString( file, MailPath );

        FGetString( file, WWWScript  );
        FGetString( file, FTPScript  );
        FGetString( file, MailScript );

        IRCProg = FGetC( file );

        FGetString( file, IRCPath   );
        FGetString( file, IRCScript );

        FGetString( file, DialProg );

        FRead( file, &RedialDelay,    sizeof( RedialDelay    ), 1 );
        FRead( file, &RedialAttempts, sizeof( RedialAttempts ), 1 );

        FRead( file, &ModemType, sizeof( ModemType ), 1 );

        FGetString( file, Hotlists[ HL_DFA ]);

        SaveOnExit = FGetC( file );

        FGetString( file, Hotlists[ HL_PINT     ]);
        FGetString( file, Hotlists[ HL_GOODNEWS ]);
        FGetString( file, Hotlists[ HL_STRICQ   ]);

        DontMoveCols  = FGetC( file );
        SaveOnChanges = FGetC( file );

        FRead( file, &ToolbarCfg.ViewMode, sizeof( ToolbarCfg.ViewMode ), 1 );
        FRead( file, &ToolbarCfg.Flags,    sizeof( ToolbarCfg.Flags    ), 1 );

        Close( file );
    }

#ifndef CMANAGER_MCC
    if( file = Open( DatesFile, MODE_OLDFILE )) {

        Read( file, &Stamps, sizeof( Stamps ));

        Close( file );
    }
#endif
}
///

#ifndef CMANAGER_MCC
/// SaveDates
static void SaveDates( void )
{
    BPTR    file;

    if( file = Open( DatesFile, MODE_NEWFILE )) {

        Write( file, Stamps, sizeof( Stamps ));

        Close( file );
    }
}
///

/// ScanHotlists
void ScanHotlists( Object *App, Object *CMObj, struct CMData *cm )
{
    ReImport2( App, CMObj, HL_IB,   MUIV_CManager_Import_IB      );
    ReImport2( App, CMObj, HL_V,    MUIV_CManager_Import_Voyager );
    ReImport2( App, CMObj, HL_AWEB, MUIV_CManager_Import_AWeb    );

    ReImport( App, HL_AMFTP, cm->FTPs, ImportAmFTP );

    ReImport( App, HL_MD2,      cm->Users, ImportMD       );
    ReImport( App, HL_YAM,      cm->Users, ImportYAM      );
    ReImport( App, HL_DFA,      cm->Users, ImportDFA      );
    ReImport( App, HL_PINT,     cm->Users, ImportPINT     );
    ReImport( App, HL_GOODNEWS, cm->Users, ImportGoodNews );
    ReImport( App, HL_STRICQ,   cm->Users, ImportSTRICQ   );

    ReImport( App, HL_STFAX, cm->Users, ImportSTFax );

    ReImport( App, HL_AMIRC, cm->Chat, ImportAmIRC );

    ReImport( App, HL_DOPUS, cm->FTPs, ImportDOpusFTP );

    ReadStamps();
    SaveDates();
}
///
/// ReadStamps
static void ReadStamps( void )
{
    struct FileInfoBlock   *fib;

    if( fib = AllocDosObject( DOS_FIB, NULL )) {
        ULONG   i;

        for( i = 0; i < NUM_HOTLISTS; i++ ) {

            if( Hotlists[ i ][ 0 ]) {
                BPTR    lock;

                if( lock = Lock( Hotlists[ i ], ACCESS_READ )) {

                    Examine( lock, fib );

                    CopyMem( &fib->fib_Date, &Stamps[ i ], sizeof( struct DateStamp ));

                    UnLock( lock );
                }
            }
        }

        FreeDosObject( DOS_FIB, fib );
    }
}
///
/// FileNew
static BOOL FileNew( Object *App, STRPTR Name, struct DateStamp *Stamp )
{
    BOOL    ret = FALSE;

    if( Name[0] ) {
        struct FileInfoBlock   *fib;

        if( fib = AllocDosObject( DOS_FIB, NULL )) {
            BPTR    lock;

            if( lock = Lock( Name, ACCESS_READ )) {

                Examine( lock, fib );

                if( CompareDates( Stamp, &fib->fib_Date ) > 0 )
                    ret = SilentScan ? TRUE :
                          MUI_Request( App, NULL, 0, NULL,
                                       CM_GetString( ANS_OK_CANCEL       ),
                                       CM_GetString( MSG_HOTLIST_CHANGED ),
                                       FilePart( Name ));

                UnLock( lock );
            }

            FreeDosObject( DOS_FIB, fib );
        }
    }

    return( ret );
}
///
/// ReImport
static void ReImport( Object *App, ULONG Hot, struct CMGroup *Group, void ( *Import )( BPTR, struct CMGroup *, BOOL ))
{
    if( FileNew( App, Hotlists[ Hot ], &Stamps[ Hot ])) {
        BPTR    file;

        if( file = Open( Hotlists[ Hot ], MODE_OLDFILE )) {

            Import( file, Group, TRUE );

            Close( file );
        }
    }
}
///
/// ReImport2
static void ReImport2( Object *App, Object *CMObj, ULONG Hot, ULONG Type )
{
    if( FileNew( App, Hotlists[ Hot ], &Stamps[ Hot ]))
        DoMethod( CMObj, MUIM_CManager_Import, Type, Hotlists[ Hot ], MUIV_CManager_Import_Filter );
}
///
#endif
