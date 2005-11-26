/*
**
** Contact Manager - An Amiga Centralized Bookmarks System
**
** Copyright 1999-2006 by Simone [wiz] Tellini <simone@tellini.info>
** Copyright 2003-2006 by Alfonso [alfie] Ranieri <alforan@tin.it>
** All Rights Are Reserved.
**
** Destributed Under The Terms Of The LGPL II
**
**
**/

#ifndef _CM_H
#define _CM_H

#define __USE_SYSBASE

#define USE_INLINE_STDARG
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/locale.h>
#include <proto/icon.h>
#include <proto/CManager.h>
#include <proto/genesis.h>
#include <proto/rexxsyslib.h>

#undef USE_INLINE_STDARG
#include <proto/intuition.h>
#include <proto/muimaster.h>

#include <clib/alib_protos.h>

#include <libraries/mui.h>
#include <libraries/asl.h>
#include <libraries/gadtools.h>

#include <mui/muiundoc.h>
#include <mui/NList_mcc.h>
#include <mui/CManager_mcc.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include <macros.h>
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/***********************************************************************/

#ifdef __MORPHOS__
extern struct IntuitionBase   *IntuitionBase;
#endif
extern struct Library         *UtilityBase;
extern struct Library         *IconBase;
extern struct Library         *MUIMasterBase;
extern struct Library         *CManagerBase;
extern struct Library         *GenesisBase;

extern struct MUI_CustomClass *CMAppClass;
extern struct MUI_CustomClass *CMWinClass;
extern struct MUI_CustomClass *CMAboutClass;

extern struct WBStartup       *_WBenchMsg;
extern UBYTE                  __ver[];

extern ULONG                  mainFlags;

enum
{
    MFLG_BWin = 1<<0,
};

/***********************************************************************/
/*
** Tags
*/

#define _TAGBASE  ((int)0xF76B0000)
#define _CMTAG(n) (_TAGBASE+(int)(n))

/***********************************************************************/
/*
** CMApp class
*/

/* methods */

#define MUIM_CMApp_DisposeWin   _CMTAG(0)
#define MUIM_CMApp_Import       _CMTAG(1)
#define MUIM_CMApp_Export       _CMTAG(2)
#define MUIM_CMApp_ScanHotlists _CMTAG(3)
#define MUIM_CMApp_Message      _CMTAG(4)
#define MUIM_CMApp_Open         _CMTAG(5)
#define MUIM_CMApp_Changed      _CMTAG(6)
#define MUIM_CMApp_SaveAs       _CMTAG(7)
#define MUIM_CMApp_About        _CMTAG(8)
#define MUIM_CMApp_Close        _CMTAG(10)

/* structures */

struct MUIP_CMApp_DisposeWin
{
    ULONG  MethodID;
    Object *win;
};

struct MUIP_CMApp_Import
{
    ULONG MethodID;
    ULONG type;
};

struct MUIP_CMApp_Export
{
    ULONG MethodID;
    ULONG type;
};

struct MUIP_CMApp_Close
{
    ULONG MethodID;
    ULONG mode;
};

enum
{
    MUIV_CMApp_Close_Quit,
    MUIV_CMApp_Close_Check,
    MUIV_CMApp_Close_Save,
};

/* Attributes */
#define MUIA_CMApp_Win       _CMTAG(0)
#define MUIA_CMApp_CM        _CMTAG(1)
#define MUIA_CMApp_ParseArgs _CMTAG(2)
#define MUIA_CMApp_Port      _CMTAG(3)

/***********************************************************************/
/*
** CMAbout class
*/

/***********************************************************************/
/*
** Macros
**/

#define MTITLE(t)      {NM_TITLE,(STRPTR)(t),0,0,0,(APTR)(t)}
#define MITEM(t)       {NM_ITEM,(STRPTR)(t),0,0,0,(APTR)(t)}
#define MTITEM(t)      {NM_ITEM,(STRPTR)(t),0,CHECKIT|MENUTOGGLE,0,(APTR)(t)}
#define MBAR           {NM_ITEM,(STRPTR)NM_BARLABEL,0,0,0,NULL}
#define MSUBITEM(t)    {NM_SUB,(STRPTR)(t),0,0,0,(APTR)(t)}
#define MSUBBAR        {NM_SUB,(STRPTR)NM_BARLABEL,0,0,0,NULL}
#define MEND           {NM_END,NULL,0,0,0,NULL}

#define CMAppObject   NewObject(CMAppClass->mcc_Class, NULL
#define CMWinObject   NewObject(CMWinClass->mcc_Class, NULL
#define CMAboutObject NewObject(CMAboutClass->mcc_Class, NULL

/***********************************************************************/
/*
** Arguments
*/

struct parseArgs
{
    UBYTE          file[256];
    UBYTE          pubscreen[256];
    struct MsgPort *port;
    UBYTE          user[41];
    UBYTE          password[60];
    ULONG          flags;
};

/* flags */
enum
{
    PAFLG_File      = 1<<0,
    PAFLG_Pubscreen = 1<<1,
    PAFLG_Port      = 1<<2,
    PAFLG_NoLogin   = 1<<3,
    PAFLG_User      = 1<<4,
    PAFLG_Password  = 1<<6,

    PAFLG_HideUser   = 1<<12,
    PAFLG_HideWWW    = 1<<13,
    PAFLG_HideFTP    = 1<<14,
    PAFLG_HideChat   = 1<<15,
    PAFLG_HideTelnet = 1<<16,
};

/***********************************************************************/

#define MINCMVER 30

/***********************************************************************/

#include "cm_protos.h"

/***********************************************************************/

#endif /* _CM_H */
