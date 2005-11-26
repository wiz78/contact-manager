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

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif

/***************************************************************************/

extern UBYTE                   lib_name[];
extern UBYTE                   lib_fullName[];
extern ULONG                   lib_version;
extern ULONG                   lib_revision;

extern struct ExecBase         *SysBase;
extern struct DosLibrary       *DOSBase;
extern struct Library          *UtilityBase;
extern struct IntuitionBase    *IntuitionBase;
extern struct GfxBase          *GfxBase;
extern struct Library          *GenesisBase;
extern struct Library          *LocaleBase;
extern struct Library          *IFFParseBase;
extern struct Library          *CodesetsBase;
extern struct Library          *AsyncIOBase;

extern struct SignalSemaphore  lib_sem;
extern struct Library          *lib_base;
extern ULONG                   lib_segList;
extern struct SignalSemaphore  lib_poolSem;
extern APTR                    lib_pool;
extern ULONG                   lib_flags;
extern struct Catalog          *lib_cat;
extern UBYTE                   lib_CMPath[256];

/***************************************************************************/

enum
{
    BASEFLG_Init         = 1<<0,
    BASEFLG_AvoidGenesis = 1<<1,
    BASEFLG_AvoidAsyncIO = 1<<2,
    BASEFLG_AsyncIO      = 1<<3,
};

/***************************************************************************/
