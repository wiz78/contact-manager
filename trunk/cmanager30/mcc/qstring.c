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
#include <devices/inputevent.h>

/***********************************************************************/

struct data
{
    struct MUI_EventHandlerNode eh;
    Object                      *list;
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperMethodA(cl,obj,(Msg)msg))
    {
        register struct data *data = INST_DATA(cl,obj);

        data->list = (Object *)GetTagData(MUIA_QString_AttachedList,NULL,msg->ops_AttrList);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl, obj);
    struct TagItem          *tags;
    register struct TagItem *tag;

    for (tags = msg->ops_AttrList; tag = NextTagItem(&tags); )
    {
        register ULONG tidata = tag->ti_Data;

        switch (tag->ti_Tag)
        {
            case MUIA_QString_AttachedList:
                data->list = (Object *)tidata;
                break;
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***************************************************************************/

static ULONG
mShow(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    memset(&data->eh,0,sizeof(data->eh));
    data->eh.ehn_Class  = cl;
    data->eh.ehn_Object = obj;
    data->eh.ehn_Events = IDCMP_RAWKEY;
    DoMethod(_win(obj),MUIM_Window_AddEventHandler,&data->eh);

    return TRUE;
}

/***********************************************************************/

static ULONG
mHide(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    DoMethod(_win(obj),MUIM_Window_RemEventHandler,&data->eh);

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mHandleEvent(struct IClass *cl,Object *obj,struct MUIP_HandleEvent *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (msg->imsg && (msg->imsg->Class==IDCMP_RAWKEY))
    {
        Object *o;

        get(_win(obj),MUIA_Window_ActiveObject,&o);

        if (o==obj)
        {
            register LONG active = 0, sactive = TRUE; //gcc

            switch (msg->muikey)
            {
                case MUIKEY_UP:
                    active = MUIV_NList_Active_Up;
                    break;

                case MUIKEY_DOWN:
                    active = MUIV_NList_Active_Down;
                    break;

                case MUIKEY_PAGEUP:
                    active = MUIV_NList_Active_PageUp;
                    break;

                case MUIKEY_PAGEDOWN:
                    active = MUIV_NList_Active_PageDown;
                    break;

                case MUIKEY_TOP:
                    active = MUIV_NList_Active_Top;
                    break;

                case MUIKEY_BOTTOM:
                    active = MUIV_NList_Active_Bottom;
                    break;

                case MUIKEY_NONE:
                {
                    register WORD mx, my;

                    mx = msg->imsg->MouseX;
                    my = msg->imsg->MouseY;

                    msg->imsg->MouseX = _mleft(data->list);
                    msg->imsg->MouseY = _mtop(data->list);

                    DoMethodA(data->list,(Msg)msg);

                    msg->imsg->MouseX = mx;
                    msg->imsg->MouseY = my;

                    sactive = FALSE;

                    break;
                }

                default:
                    sactive = FALSE;
                    break;
            }

            if (sactive) set(data->list,MUIA_NList_Active,active);
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:           return mNew(cl,obj,(APTR)msg);
        case OM_SET:           return mSets(cl,obj,(APTR)msg);

        case MUIM_HandleEvent: return mHandleEvent(cl,obj,(APTR)msg);
        case MUIM_Show:        return mShow(cl,obj,(APTR)msg);
        case MUIM_Hide:        return mHide(cl,obj,(APTR)msg);

        default:               return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initQString(void)
{
    return (ULONG)(lib_QString = MUI_CreateCustomClass(NULL,NULL,lib_RootString,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeQString(void)
{
    MUI_DeleteCustomClass(lib_QString);
    lib_QString = NULL;
}

/***********************************************************************/

