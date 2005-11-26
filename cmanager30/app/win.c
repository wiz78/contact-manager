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

#include "cm.h"
#include <mui/BWin_mcc.h>

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct TagItem   *attrs = msg->ops_AttrList;
    register struct parseArgs *pa;

    pa = (struct parseArgs *)GetTagData(MUIA_CMApp_ParseArgs,NULL,attrs);

    return (ULONG)DoSuperNew(cl,obj,
            MUIA_Window_ID,    MAKE_ID('M','A','I','N'),
            pa->pubscreen[0] ? MUIA_Window_PublicScreen : TAG_IGNORE, pa->pubscreen,
            MUIA_BWin_Borders, TRUE,
            TAG_MORE, attrs);
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
initCMWinClass(void)
{
    if (mainFlags & MFLG_BWin) CMWinClass = MUI_CreateCustomClass(NULL,MUIC_BWin,NULL,0,DISP(dispatcher));
    if (!CMWinClass) CMWinClass = MUI_CreateCustomClass(NULL,MUIC_Window,NULL,0,DISP(dispatcher));

    return (ULONG)CMWinClass;
}

/***********************************************************************/
