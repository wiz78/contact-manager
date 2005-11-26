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

#ifndef _CLASS_H
#define _CLASS_H

#define __NOLIBBASE__
#define __USE_SYSBASE

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/locale.h>
#include <proto/graphics.h>
#include <proto/datatypes.h>
#include <proto/CManager.h>
#include <proto/rexxsyslib.h>
#include <proto/iffparse.h>
#include <proto/genesis.h>
#include <proto/openurl.h>
#include <proto/cybergraphics.h>
#include <proto/codesets.h>

#undef USE_INLINE_STDARG
#include <proto/intuition.h>
#include <proto/muimaster.h>

#include <clib/alib_protos.h>
#include <clib/debug_protos.h>

#include <datatypes/pictureclass.h>
#include <utility/pack.h>
#include <libraries/mui.h>
#include <libraries/asl.h>
#include <libraries/gadtools.h>
#include <graphics/gfxmacros.h>
#include <cybergraphx/cybergraphics.h>
#include <prefs/prefhdr.h>

#include <mui/MUIundoc.h>
#include <mui/nlistview_mcc.h>
#include <mui/nlist_mcc.h>
#include <mui/NListtree_mcc.h>
#include <mui/textinput_mcc.h>
#include <mui/TheBar_mcc.h>
#include <mui/Urltext_mcc.h>
#include <mui/BWin_mcc.h>
#include <mui/CManager_mcc.h>
#include <mui/Calendar_mcc.h>


#ifndef __MORPHOS__
#include <dos.h>
#endif

#include <string.h>
#include <stdlib.h>

#include "base.h"
#include <macros.h>

/***********************************************************************/

extern UBYTE LIBNAME[];

/***********************************************************************/

#define MINSTACKSIZE 8192

/***********************************************************************/

struct placeHolder
{
    UBYTE  flag;
    STRPTR value;
};

/***********************************************************************/
/*
** NewMouse events
*/

#ifndef NM_WHEEL_UP
#define NM_WHEEL_UP      0x7a
#endif

#ifndef NM_WHEEL_DOWN
#define NM_WHEEL_DOWN    0x7b
#endif

/***********************************************************************/
/*
** Groups button
*/

enum
{
    IBT_Up,
    IBT_Down,
};

/***********************************************************************/
/*
** MUI macros
*/

/* Classes */
#define PopupCodesetsObject   NewObject(lib_PopupCodesets->mcc_Class,NULL
#define PopaObject            NewObject(lib_Popasl->mcc_Class,NULL
#define RootScrollgroupObject NewObject(lib_RootScrollgroup->mcc_Class,NULL
#define RootStringObject      NewObject(lib_RootString->mcc_Class,NULL
#define RootScrollObject      NewObject(lib_RootScroll->mcc_Class,NULL
#define RootBarObject         NewObject(lib_RootBar->mcc_Class,NULL
#define QStringObject         NewObject(lib_QString->mcc_Class,NULL
#define PicObject             NewObject(lib_Pic->mcc_Class,NULL
#define DelWarningObject      NewObject(lib_DelWarning->mcc_Class,NULL
#define EntryListObject       NewObject(lib_EntryList->mcc_Class,NULL
#define GroupListObject       NewObject(lib_GroupList->mcc_Class,NULL
#define DialWindowObject      NewObject(lib_DialWindow->mcc_Class,NULL
#define EditUserObject        NewObject(lib_EditUser->mcc_Class,NULL
#define EditWWWObject         NewObject(lib_EditWWW->mcc_Class,NULL
#define EditFTPObject         NewObject(lib_EditFTP->mcc_Class,NULL
#define EditIRCObject         NewObject(lib_EditIRC->mcc_Class,NULL
#define EditWindowObject      NewObject(lib_EditWindow->mcc_Class,NULL
#define AboutWindowObject     NewObject(lib_AboutWindow->mcc_Class,NULL
#define EditTelnetObject      NewObject(lib_EditTelnet->mcc_Class,NULL
#define SearchWindowObject    NewObject(lib_SearchWindow->mcc_Class,NULL
#define LoginWindowObject     NewObject(lib_LoginWindow->mcc_Class,NULL
#define MiniMailWindowObject  NewObject(lib_miniMailWindow->mcc_Class,NULL
#define PrefsWindowObject     NewObject(lib_PrefsWindow->mcc_Class,NULL
#define MWinObject            NewObject(lib_MatherWin->mcc_Class,NULL
#define CardObject            NewObject(lib_Card->mcc_Class,NULL
#define LampObject            NewObject(lib_Lamp->mcc_Class,NULL
#define PopphObject           NewObject(lib_Popph->mcc_Class,NULL

/* Menus */
#define MTITLE(d)           {NM_TITLE,NULL,0,0,0,(APTR)(d)}
#define MITEM(d)            {NM_ITEM,NULL,0,0,0,(APTR)(d)}
#define MTITEM(d)           {NM_ITEM,NULL,0,CHECKIT|MENUTOGGLE,0,(APTR)(d)}
#define MEITEM(d,e)         {NM_ITEM,NULL,0,CHECKIT,(LONG)(e),(APTR)(d)}
#define MBAR                {NM_ITEM,(STRPTR)NM_BARLABEL,0,0,0,NULL}
#define MSUBITEM(d)         {NM_SUB,NULL,0,0,0,(APTR)(d)}
#define MESUBITEM(d,e)      {NM_SUB,NULL,0,CHECKIT,(LONG)(e),(APTR)(d)}
#define MSUBBAR             {NM_SUB,(STRPTR)NM_BARLABEL,0,0,0,NULL}
#define MEND                {NM_END,NULL,0,0,0,NULL}

/* Various */
#define wspace(w)            RectangleObject, MUIA_Weight, w, End
#define fwspace()            RectangleObject, MUIA_FixWidthTxt, "-", End
#define fhspace()            RectangleObject, MUIA_FixHeightTxt, "-", End
#define _sbbut(sb,id)        ((Object *)DoMethod((Object *)(sb),MUIM_TheBar_GetObject,(ULONG)(id)))

enum
{
    B_ADD,
    B_CLONE,
    B_EDIT,
    B_REMOVE,
    B_SORT,
    B_SAVE,
    B_SEARCH,
    B_PREFS,
    B_LOGIN,
};

/***********************************************************************/
/*
** Some constant and default
*/

#define DEF_TreeW               60
#define DEF_ListW              160

#define DEF_ListCW             160
#define DEF_CardCW              60

#define DEF_DataFile           "Default.data"
#define DEF_PrefsFile          "CManager.prefs"
#define DEF_HotlistStampsFile  "Hotlist.stamps"
#define DEF_ImagePath          "Images"
#define DEF_ModemsFile         "Modems.txt"
#define DEF_CountriesFile      "CountryCodes.txt"
#define DEF_DelLimit           20

/***********************************************************************/

#include "class_protos.h"

/***********************************************************************/

#endif /* _CLASS_H */
