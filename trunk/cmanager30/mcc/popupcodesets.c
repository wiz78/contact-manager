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

M_HOOK(window,Object *pop,Object *win)
{
    set(win,MUIA_Window_DefaultObject,pop);

    return 0;
}

/***********************************************************************/

M_HOOK(open,Object *list,Object *str)
{
    register STRPTR s;
    STRPTR          x;
    register int    i;

    s = (STRPTR)xget(str,MUIA_Textinput_Contents);

    for (i = 0; ;i++)
    {
        DoMethod(list,MUIM_List_GetEntry,i,&x);
        if (!x)
        {
            set(list,MUIA_List_Active,MUIV_List_Active_Off);
            break;
        }
        else
            if (!stricmp(x,s))
            {
                set(list,MUIA_List_Active,i);
                break;
            }
    }

    return TRUE;
}

/***********************************************************************/

M_HOOK(close,Object *list,Object *str)
{
    STRPTR e;

    DoMethod(list,MUIM_List_GetEntry,MUIV_List_GetEntry_Active,&e);
    set(str,MUIA_Textinput_Contents,e);

    return 0;
}

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object *bt, *lv, *l;

    if (obj = (Object *)DoSuperNew(cl,obj,

            MUIA_Popstring_String, ostring(256,GetTagData(MUIA_PopupCodesets_Key    ,0,msg->ops_AttrList),0),
            MUIA_Popstring_Button, bt = MUI_MakeObject(MUIO_PopButton,MUII_PopUp),

            MUIA_Popobject_Object, lv = ListviewObject,
                MUIA_Listview_List, l = ListObject,
                    MUIA_Frame,              MUIV_Frame_InputList,
                    MUIA_Background,         MUII_ListBack,
                    MUIA_List_AutoVisible,   TRUE,
                    MUIA_List_ConstructHook, MUIV_List_ConstructHook_String,
                    MUIA_List_DestructHook,  MUIV_List_DestructHook_String,
                End,
            End,
            MUIA_Popobject_WindowHook, &window_hook,
            MUIA_Popobject_StrObjHook, &open_hook,
            MUIA_Popobject_ObjStrHook, &close_hook,

            TAG_MORE,msg->ops_AttrList))
    {
        register STRPTR *array;

        set(bt,MUIA_CycleChain,TRUE);
        DoMethod(lv,MUIM_Notify,MUIA_Listview_DoubleClick,TRUE,obj,2,MUIM_Popstring_Close,TRUE);

        if (array = CodesetsSupportedA(NULL))
        {
            DoMethod(l,MUIM_List_Insert,array,-1,MUIV_List_Insert_Sorted);
            CodesetsFreeA(array,NULL);
        }
        else superset(cl,obj,MUIA_Disabled,TRUE);
    }

    return (ULONG)obj;
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
initPopupCodesets(void)
{
    return (ULONG)(lib_PopupCodesets = MUI_CreateCustomClass(NULL,MUIC_Popobject,NULL,0,DISP(dispatcher)));
}

/***********************************************************************/

void
disposePopupCodesets(void)
{
    MUI_DeleteCustomClass(lib_PopupCodesets);
    lib_PopupCodesets = NULL;
}

/***********************************************************************/
