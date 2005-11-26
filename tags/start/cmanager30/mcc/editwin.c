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
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/***********************************************************************/

struct data
{
    Object *Body;
    Object *List;
    Object *CMObj;
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data             temp;
    register Object         *Ok, *Cancel;
    register struct TagItem *attrs = msg->ops_AttrList, *tag;

    memset(&temp,0,sizeof(temp));

    if (tag = FindTagItem(MUIA_EditWindow_Object,attrs)) temp.Body = (Object *)tag->ti_Data;
    else return 0;

    if (tag = FindTagItem(MUIA_EditWindow_List,attrs)) temp.List = (Object *)tag->ti_Data;
    else return 0;

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Window_Title, CM_GetString(GetTagData(MUIA_EditWindow_Title,0,attrs)),
            MUIA_Window_ID,    GetTagData(MUIA_EditWindow_ID,0,attrs),

            WindowContents, VGroup,
                Child, temp.Body,
                Child, HGroup,
                    Child, Ok = obutton(Msg_Edit_OK,0),
                    Child, wspace(200),
                    Child, Cancel = obutton(Msg_Edit_Cancel,0),
                End,
            End,

        TAG_MORE, attrs))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->CMObj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,attrs);

        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,2,MUIM_CManager_WinClose,FALSE);

        DoMethod(Ok,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_WinClose,TRUE);
        DoMethod(Cancel,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_WinClose,FALSE);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_EditWindow_Entry:     *msg->opg_Storage = xget(data->Body,MUIA_Edit_Entry); return TRUE;
        case MUIA_EditWindow_IsEditWin: *msg->opg_Storage = TRUE; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mOpen(struct IClass *cl,Object *obj,struct MUIP_EditWindow_Open *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    set(data->Body,MUIA_Edit_Entry,msg->Entry);
    set(obj,MUIA_Window_Open,TRUE);

    return 0;
}

/***********************************************************************/

static ULONG
mClose(struct IClass *cl,Object *obj,struct MUIP_CManager_WinClose *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMEntry       *entry;
    Object               *AppObj;

    set(obj,MUIA_Window_Open,FALSE);
    get(data->Body,MUIA_Edit_Entry,&entry);

    if (msg->mode)
    {
        register ULONG i;

        DoMethod(data->Body,MUIM_Edit_Update);

        for (i = 0; ; i++)
        {
            struct CMEntry *e;

            DoMethod(data->List,MUIM_NList_GetEntry,i,&e);
            if (e==NULL) break;

            if (e==entry)
            {
                DoMethod(data->List,MUIM_NList_Redraw,i);
                DoMethod(data->List,MUIM_NList_Sort);
                DoMethod(data->CMObj,MUIM_CManager_ChangeQuick);
                break;
            }
        }

        entry->Flags &= ~CMENTRYFLG_New;

        set(data->CMObj,MUIA_CManager_Changed,TRUE);
    }
    else
        if (entry->Flags & CMENTRYFLG_New)
        {
            register ULONG i;
            register ULONG  found = FALSE;

            for (i = 0;; i++)
            {
                struct CMEntry *e;

                DoMethod(data->List,MUIM_NList_GetEntry,i,&e);
                if (e==NULL) break;

                if (e==entry)
                {
                    DoMethod(data->List,MUIM_NList_Remove,i);
                    found = TRUE;
                    break;
                }
            }

            // it's not displayed now, so it's linked in a list
            if (!(found)) Remove((struct Node *)entry);

            CM_FreeEntry(entry);
        }

    superget(cl,obj,MUIA_ApplicationObject,&AppObj);
    DoMethod(AppObj,MUIM_Application_PushMethod,data->CMObj,2,MUIM_CManager_DisposeObj,obj);

    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:                 return mNew(cl,obj,(APTR)msg);
        case OM_GET:                 return mGet(cl,obj,(APTR)msg);

        case MUIM_EditWindow_Open:   return mOpen(cl,obj,(APTR)msg);
        case MUIM_CManager_WinClose: return mClose(cl,obj,(APTR)msg);

        default:                     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initEditWindow(void)
{
    return (ULONG)(lib_EditWindow = MUI_CreateCustomClass(NULL,NULL,lib_MatherWin,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeEditWindow(void)
{
    MUI_DeleteCustomClass(lib_EditWindow);
    lib_EditWindow = NULL;
}

/***********************************************************************/
