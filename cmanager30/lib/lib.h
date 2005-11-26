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

#ifndef _LIB_H
#define _LIB_H

#define __NOLIBBASE__
#define __USE_SYSBASE

#define USE_INLINE_STDARG
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/graphics.h>
#include <proto/genesis.h>
#include <proto/intuition.h>
#include <proto/locale.h>
#include <proto/codesets.h>
#include <proto/asyncio.h>

#include <clib/alib_protos.h>
#include <clib/debug_protos.h>
#include <dos/rdargs.h>
#include <dos/dostags.h>
#include <libraries/mui.h>
#include <libraries/locale.h>
#include <libraries/CManager.h>

#include <macros.h>
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

#ifndef __MORPHOS__
#include <dos.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <macros.h>

#include "base.h"

/***********************************************************************/

extern UBYTE LIBNAME[];

/***********************************************************************/

struct CMHandle
{
    struct MsgPort *MyPort;
    struct MsgPort *CMPort;
    STRPTR         CMARexx;
};

/***********************************************************************/

#include "lib_protos.h"

/***********************************************************************/

#endif /* _LIB_H */
