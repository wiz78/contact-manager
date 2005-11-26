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
#include <mui/BWin_mcc.h>

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    return (ULONG)DoSuperNew(cl,obj,
            MUIA_BWin_Borders,TRUE,
            TAG_MORE, (ULONG)msg->ops_AttrList);
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW: return mNew(cl,obj,(APTR)msg);
        default:     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initMatherWin(void)
{
    if (lib_flags & BASEFLG_BWin)
        lib_MatherWin = MUI_CreateCustomClass(NULL,MUIC_BWin,NULL,0,DISP(dispatcher));

    if (!lib_MatherWin) lib_MatherWin = MUI_CreateCustomClass(NULL,MUIC_Window,NULL,0,DISP(dispatcher));

    return (ULONG)(lib_MatherWin);
}

/***********************************************************************/

void
disposeMatherWin(void)
{
    MUI_DeleteCustomClass(lib_MatherWin);
    lib_MatherWin = NULL;
}

/***********************************************************************/
