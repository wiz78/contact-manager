/***************************************************************************
    revision             : $Id: prefs.h,v 1.1.1.1 2003-01-06 15:54:38 tellini Exp $
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

#ifndef PREFS_H
#define PREFS_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

#ifdef String
#undef String
#endif

/// Methods
#define MUIM_PrefsWindow_Open           (TAGBASE + 1)
#define MUIM_PrefsWindow_Close          (TAGBASE + 2)
#define MUIM_PrefsWindow_GetCountry     (TAGBASE + 3)
#define MUIM_PrefsWindow_SetString      (TAGBASE + 4)
#define MUIM_PrefsWindow_SetDialerGroup (TAGBASE + 5)
///
/// Structs
struct PrefsWindowData {
        Object         *Voyager;        //  Hotlists
        Object         *AmFTP;
        Object         *MD2;
        Object         *YAM;
        Object         *DFA;
        Object         *PINT;
        Object         *GoodNews;
        Object         *STRICQ;
        Object         *AWeb;
        Object         *IB;
        Object         *STFax;
        Object         *AmIRC;
        Object         *DOpus;          //  Main
        Object         *SilentScan;
        Object         *OpenURL;
        Object         *Country;
        Object         *CountryCode;
        Object         *DoubleClick;
        Object         *MoveCols;
        Object         *SaveOnExit;
        Object         *SaveOnChanges;
        Object         *ToolbarCfg;
        Object         *UseLister;      //  DOpus
        Object         *Lister;
        Object         *UseDOpus;
        Object         *UseDialler;     //  Dialler
        Object         *DialDevice;
        Object         *DialUnit;
        Object         *DialShared;
        Object         *DialInit;
        Object         *RedialDelay;
        Object         *RedialAttempts;
        Object         *DialCommand;
        Object         *ModemList;
        Object         *ModemName;
        Object         *LogCalls;
        Object         *WWWProg;        //  Programs
        Object         *WWWScript;
        Object         *WWWPath;
        Object         *FTPProg;
        Object         *FTPScript;
        Object         *FTPPath;
        Object         *MailProg;
        Object         *MailScript;
        Object         *MailPath;
        Object         *IRCProg;
        Object         *IRCScript;
        Object         *IRCPath;
        Object         *DialGroup;
        Object         *ExtDialGroup;
        Object         *DialProg;
        Object         *CMObj;
};

struct MUIP_PrefsWindow_Open {
        ULONG           MethodID;
        Object         *CMObj;
};

struct MUIP_PrefsWindow_Close {
        ULONG           MethodID;
        ULONG           Ok;
};

struct MUIP_PrefsWindow_SetString {
        ULONG           MethodID;
        Object         *Obj;
        STRPTR         *Strings;
        ULONG           Index;
};
///
/// Externs
extern struct MUI_CustomClass  *CL_PrefsWindow;

enum { HL_IB = 0, HL_V, HL_AWEB, HL_AMFTP, HL_MD2, HL_STFAX, HL_YAM, HL_AMIRC,
       HL_DOPUS, HL_DFA, HL_PINT, HL_GOODNEWS, HL_STRICQ, NUM_HOTLISTS };

extern TEXT     Hotlists[ NUM_HOTLISTS ][256];

enum { ACTION_EDIT = 0, ACTION_SCRIPT, ACTION_SCRIPT_CLOSE, ACTION_SCRIPT_ICONIFY };

extern UWORD    Action;

extern TEXT     Country[80];
extern TEXT     CountryCode[9];

#ifndef CMANAGER_MCC
extern TEXT     PrefsFile[256];
#endif

extern BOOL     UseOpenURL;
extern BOOL     UseDOpus, UseLister;
extern TEXT     Lister[256];

extern BOOL     DontMoveCols, SaveOnExit, SaveOnChanges;

extern UWORD    UseDialler;
extern TEXT     DialProg[256];

extern TEXT     WWWPath[256], FTPPath[256], MailPath[256], IRCPath[256];
extern TEXT     WWWScript[256], FTPScript[256], MailScript[256], IRCScript[256];

extern struct MUIS_SpeedBarCfg_Config  ToolbarCfg;

extern void     LoadPrefs( STRPTR );
extern void     ScanHotlists( Object *, Object *, struct CMData * );

extern ULONG MainWindow_Prefs( struct IClass *, Object *, Msg );
extern ULONG SAVEDS ASM PrefsWindow_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///

#endif /* PREFS_H */
