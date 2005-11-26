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

/***********************************************************************/

#ifndef __MORPHOS__
ULONG
DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
    return DoSuperMethod(cl,obj,OM_NEW,&tag1,NULL);
}
#endif

/***********************************************************************/

ULONG
openWindow(Object *app,Object *win)
{
    ULONG v;

    if (win)
    {
        set(win,MUIA_Window_Open,TRUE);
        get(win,MUIA_Window_Open,&v);
        if (!v) get(app,MUIA_Application_Iconified,&v);
    }
    else v = FALSE;

    if (!v) DisplayBeep(0);

    return v;
}

/***********************************************************************/

void
localizeMenus(struct NewMenu *menu)
{
    while (menu->nm_Type!=NM_END )
    {
        if (menu->nm_Label!=NM_BARLABEL)
            menu->nm_Label = CM_GetString((ULONG)menu->nm_Label);

        menu++;
    }
}

/***********************************************************************/

#ifndef __MORPHOS__
static UWORD fmtfunc[] = { 0x16c0, 0x4e75 };
void STDARGS
msprintf(STRPTR buf,STRPTR fmt,...)
{
    RawDoFmt(fmt,&fmt+1,(APTR)fmtfunc,buf);
}
#else

void
msprintf(STRPTR buf,STRPTR fmt,...)
{
    va_list va;

    va_start(va,fmt);
    VNewRawDoFmt(fmt,0,buf,va);
    va_end(va);
}
#endif

/***********************************************************************/

