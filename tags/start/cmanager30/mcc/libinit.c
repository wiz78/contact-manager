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

#include "class.h"
#include "CManager.mcc_rev.h"

/****************************************************************************/

UBYTE                   lib_name[] = PRG;
UBYTE	                lib_ver[] = VSTRING;
UBYTE                   lib_fullName[] = PRGNAME;
ULONG                   lib_version = VERSION;
ULONG                   lib_revision = REVISION;

struct ExecBase         *SysBase = NULL;
struct DosLibrary       *DOSBase = NULL;
struct IntuitionBase    *IntuitionBase = NULL;
struct Library          *UtilityBase = NULL;
struct GfxBase          *GfxBase = NULL;
struct Library          *DataTypesBase = NULL;
struct Library          *CyberGfxBase = NULL;

struct Library          *IFFParseBase = NULL;
struct RxsLib           *RexxSysBase = NULL;
struct Library          *CManagerBase = NULL;
struct Library          *LocaleBase = NULL;
struct Library          *MUIMasterBase = NULL;

struct Library          *GenesisBase = NULL;
struct Library          *OpenURLBase = NULL;
struct Library          *CodesetsBase = NULL;

struct SignalSemaphore  lib_sem = {0};
struct Library          *lib_base = NULL;
ULONG                   lib_segList = NULL;
ULONG                   lib_flags = 0;

struct SignalSemaphore  lib_poolSem = {0};
APTR                    lib_pool = NULL;

UBYTE                   lib_CMPath[256];
struct Locale           *lib_locale = NULL;
struct MinList          lib_countries;

struct MUI_CustomClass  *lib_RootScrollgroup = NULL;
struct MUI_CustomClass  *lib_RootString = NULL;
struct MUI_CustomClass  *lib_RootScroll = NULL;
struct MUI_CustomClass  *lib_RootBar = NULL;

struct MUI_CustomClass  *lib_PopupCodesets = NULL;
struct MUI_CustomClass  *lib_Popasl = NULL;
struct MUI_CustomClass  *lib_QString = NULL;
struct MUI_CustomClass  *lib_Pic = NULL;
struct MUI_CustomClass  *lib_DelWarning = NULL;
struct MUI_CustomClass  *lib_EntryList = NULL;
struct MUI_CustomClass  *lib_GroupList = NULL;
struct MUI_CustomClass  *lib_EditWindow = NULL;
struct MUI_CustomClass  *lib_EditUser = NULL;
struct MUI_CustomClass  *lib_EditWWW = NULL;
struct MUI_CustomClass  *lib_EditFTP = NULL;
struct MUI_CustomClass  *lib_EditIRC = NULL;
struct MUI_CustomClass  *lib_EditTelnet = NULL;
struct MUI_CustomClass  *lib_MatherWin = NULL;
struct MUI_CustomClass  *lib_DialWindow = NULL;
struct MUI_CustomClass  *lib_SearchWindow = NULL;
struct MUI_CustomClass  *lib_LoginWindow = NULL;
struct MUI_CustomClass  *lib_PrefsWindow = NULL;
struct MUI_CustomClass  *lib_Class = NULL;
struct MUI_CustomClass  *lib_miniMailWindow = NULL;
struct MUI_CustomClass  *lib_Card = NULL;
struct MUI_CustomClass  *lib_Lamp = NULL;
struct MUI_CustomClass  *lib_Popph = NULL;

/****************************************************************************/

#ifdef __MORPHOS__
#include <exec/resident.h>

static struct Library *initLib ( struct Library *base , BPTR segList , struct ExecBase *sys );
static struct Library *openLib ( void );
static ULONG expungeLib ( void );
static ULONG closeLib ( void );
static ULONG nil (void );
static ULONG first(void) __attribute((unused));

/****************************************************************************/

static ULONG
first(void)
{
    return -1;
}

/****************************************************************************/

#pragma pack(2)
struct dataTable
{
    UBYTE ln_Type_Init[4];
    UBYTE ln_Pri_Init[4];
    UBYTE ln_Name_Init[2];
    ULONG ln_Name_Content;
    UBYTE lib_Flags_Init[4];
    UBYTE lib_Version_Init[2];
        UWORD lib_Version_Content;
    UBYTE lib_Revision_Init[2];
        UWORD lib_Revision_Content;
    UBYTE lib_IdString_Init[2];
    ULONG lib_IdString_Content;
    UWORD EndMark;
};
#pragma pack()

static const APTR funcTable[] =
{
    (APTR)   FUNCARRAY_32BIT_NATIVE,
    (APTR)   openLib,
    (APTR)   closeLib,
    (APTR)   expungeLib,
    (APTR)   nil,
        (APTR)   query,
    (APTR)   -1
};

static const struct dataTable dataTable =
{
    0xa0,8,  NT_LIBRARY,0,
    0xa0,9,  0,0,
    0x80,10, (ULONG)lib_name,
    0xa0,14, LIBF_SUMUSED|LIBF_CHANGED,0,
    0x90,20, VERSION,
    0x90,22, REVISION,
    0x80,24, (ULONG)lib_ver,
    0
};

static const ULONG initTable[] =
{
    sizeof(struct Library),
    (ULONG)funcTable,
    (ULONG)&dataTable,
    (ULONG)initLib
};

const struct Resident romTag =
{
    RTC_MATCHWORD,
    (struct Resident *)&romTag,
    (struct Resident *)&romTag+1,
    RTF_AUTOINIT|RTF_PPC|RTF_EXTENDED,
    VERSION,
    NT_LIBRARY,
    0,
    (char *)lib_name,
    (char *)lib_ver,
    (APTR)initTable,
    REVISION,
        NULL
};

const ULONG __abox__    = 1;
#endif

/****************************************************************************/

#ifdef __MORPHOS__
static struct Library *initLib(struct Library *base,BPTR segList,struct ExecBase *sys)
#else
struct Library *SAVEDS ASM initLib(REG(a0,ULONG segList),REG(a6,APTR sys),REG(d0, struct Library *base))
#endif
{

#define SysBase sys
    InitSemaphore(&lib_sem);
    InitSemaphore(&lib_poolSem);
#undef SysBase

    SysBase     = sys;
    lib_segList = segList;

    return lib_base = base;
}

/****************************************************************************/

#ifdef __MORPHOS__
static struct Library *openLib(void)
#else
struct Library * SAVEDS ASM openLib(REG(a6,struct Library *base))
#endif
{
#ifdef __MORPHOS__
    struct Library *base = (struct Library *)REG_A6;
#endif
    register struct Library *res;

    ObtainSemaphore(&lib_sem);

    base->lib_OpenCnt++;
    base->lib_Flags &= ~LIBF_DELEXP;

    if (!(lib_flags & BASEFLG_Init) && !initBase())
    {
        base->lib_OpenCnt--;
        res = NULL;
    }
    else res = base;

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG expungeLib(void)
#else
ULONG SAVEDS ASM expungeLib(REG(a6,struct Library *base))
#endif
{
#ifdef __MORPHOS__
    struct Library *base = (struct Library *)REG_A6;
#endif
    register ULONG res;

    ObtainSemaphore(&lib_sem);

    if (!base->lib_OpenCnt)
    {
        Remove((struct Node *)base);
        FreeMem((UBYTE *)base-base->lib_NegSize,base->lib_NegSize+base->lib_PosSize);

        res = lib_segList;
    }
    else
    {
        base->lib_Flags |= LIBF_DELEXP;
        res = NULL;
    }

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG closeLib(void)
#else
ULONG SAVEDS ASM closeLib(REG(a6,struct Library *base))
#endif
{
#ifdef __MORPHOS__
    struct Library *base = (struct Library *)REG_A6;
#endif
    register ULONG res = NULL;

    ObtainSemaphore(&lib_sem);

    if (!--base->lib_OpenCnt)
    {
        freeBase();

        if (base->lib_Flags & LIBF_DELEXP)
	{
            Remove((struct Node *)base);
    	    FreeMem((UBYTE *)base-base->lib_NegSize,base->lib_NegSize+base->lib_PosSize);

            res = lib_segList;
	}
    }

    ReleaseSemaphore(&lib_sem);

    return res;
}

/****************************************************************************/

#ifdef __MORPHOS__
static ULONG nil(void)
{
    return 0;
}
#endif

/****************************************************************************/

