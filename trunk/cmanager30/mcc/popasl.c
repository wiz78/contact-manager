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

/***********************************************************************/

static ULONG
mPopStringOpen( struct IClass *cl, Object *obj, Msg msg)
{
    //register struct data *data = INST_DATA(cl,obj);
    register ULONG       res;
    ULONG                active;

    res = DoSuperMethodA(cl,obj,(Msg)msg);

    superget(cl,obj,MUIA_Popasl_Active,&active);
    if (active) set(_win(obj),MUIA_Window_Sleep,TRUE);

    return res;
}

/***********************************************************************/

static ULONG
mPopStringClose( struct IClass *cl, Object *obj, Msg msg)
{
    //register struct data *data = INST_DATA(cl,obj);

    set(_win(obj),MUIA_Window_Sleep,FALSE);

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case MUIM_Popstring_Open:  return mPopStringOpen(cl,obj,(APTR)msg);
        case MUIM_Popstring_Close: return mPopStringClose(cl,obj,(APTR)msg);

        default:                   return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initPopasl(void)
{
    return (ULONG)(lib_Popasl = MUI_CreateCustomClass(NULL,MUIC_Popasl,NULL,0,DISP(dispatcher)));
}

/***********************************************************************/

void
disposePopasl(void)
{
    MUI_DeleteCustomClass(lib_Popasl);
    lib_Popasl = NULL;
}

/***********************************************************************/
