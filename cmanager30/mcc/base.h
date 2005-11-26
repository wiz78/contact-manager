/*
**
** Contact Manager - An Amiga Centralized Bookmarks System
**
** Copyright 1999-2006 by Simone [wiz] Tellini https://tellini.info/
** Copyright 2003-2006 by Alfonso [alfie] Ranieri <alforan@tin.it>
** All Rights Are Reserved.
**
** Destributed Under The Terms Of The LGPL II
**
**
**/

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

/***************************************************************************/

extern UBYTE                   lib_name[];
extern UBYTE                   lib_fullName[];
extern ULONG                   lib_version;
extern ULONG                   lib_revision;

extern struct ExecBase         *SysBase;
extern struct DosLibrary       *DOSBase;
extern struct IntuitionBase    *IntuitionBase;
extern struct Library          *UtilityBase;
extern struct GfxBase          *GfxBase;
extern struct Library          *DataTypesBase;
extern struct Library          *CyberGfxBase;

extern struct Library          *IFFParseBase;
extern struct RxsLib           *RexxSysBase;
extern struct Library          *CManagerBase;
extern struct Library          *LocaleBase;
extern struct Library          *MUIMasterBase;

extern struct Library          *GenesisBase;
extern struct Library          *OpenURLBase;
extern struct Library          *CodesetsBase;

extern struct SignalSemaphore  lib_sem;
extern struct Library          *lib_base;
extern ULONG                   lib_segList;
extern ULONG                   lib_flags;

extern struct SignalSemaphore  lib_poolSem;
extern APTR                    lib_pool;

extern UBYTE                   lib_CMPath[256];
extern struct Locale           *lib_locale;
extern struct MinList          lib_countries;

extern struct MUI_CustomClass  *lib_RootScrollgroup;
extern struct MUI_CustomClass  *lib_RootString;
extern struct MUI_CustomClass  *lib_RootScroll;
extern struct MUI_CustomClass  *lib_RootBar;

extern struct MUI_CustomClass  *lib_PopupCodesets;
extern struct MUI_CustomClass  *lib_Popasl;
extern struct MUI_CustomClass  *lib_QString;
extern struct MUI_CustomClass  *lib_Pic;
extern struct MUI_CustomClass  *lib_DelWarning;
extern struct MUI_CustomClass  *lib_EntryList;
extern struct MUI_CustomClass  *lib_GroupList;
extern struct MUI_CustomClass  *lib_EditWindow;
extern struct MUI_CustomClass  *lib_EditUser;
extern struct MUI_CustomClass  *lib_EditWWW;
extern struct MUI_CustomClass  *lib_EditFTP;
extern struct MUI_CustomClass  *lib_EditIRC;
extern struct MUI_CustomClass  *lib_EditTelnet;
extern struct MUI_CustomClass  *lib_MatherWin;
extern struct MUI_CustomClass  *lib_DialWindow;
extern struct MUI_CustomClass  *lib_SearchWindow;
extern struct MUI_CustomClass  *lib_LoginWindow;
extern struct MUI_CustomClass  *lib_PrefsWindow;
extern struct MUI_CustomClass  *lib_Class;
extern struct MUI_CustomClass  *lib_popupCodesets;
extern struct MUI_CustomClass  *lib_miniMailWindow;
extern struct MUI_CustomClass  *lib_Card;
extern struct MUI_CustomClass  *lib_Lamp;
extern struct MUI_CustomClass  *lib_Popph;

/***************************************************************************/

enum
{
    BASEFLG_Init             = 1<<0,
    BASEFLG_MUI20            = 1<<1,
    BASEFLG_MUI4             = 1<<2,
    BASEFLG_Countries        = 1<<3,
    BASEFLG_AvoidGenesis     = 1<<4,
    BASEFLG_BWin             = 1<<5,
    BASEFLG_GroupsInputFrame = 1<<6,
    BASEFLG_NoEntriesFrame   = 1<<7,
    BASEFLG_FinishInit       = 1<<8,
};

/***************************************************************************/
