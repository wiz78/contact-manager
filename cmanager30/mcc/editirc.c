/*
**
** Contact Manager - An Amiga Centralized Bookmarks System
**
** Copyright 1999-2006 by Simone [wiz] Tellini https://tellini.info/
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
    struct CMChat *Current;

    Object        *CMObj;
    Object        *Channel;
    Object        *Server;
    Object        *Maintainer;
    Object        *Nick;
    Object        *Comment;
    Object        *EMail;
    Object        *WWW;
    Object        *Password;

};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data temp;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl, obj,
            MUIA_HelpNode,   "CMCHAT",
            MUIA_Frame,      MUIV_Frame_Group,
            MUIA_Background, MUII_GroupBack,

            Child, ColGroup(2),

                Child, olabel2(Msg_Edit_ChatChannel),
                Child, HGroup,
                    Child, temp.Channel = ostring(128,Msg_Edit_ChatChannel,0),
                    Child, olabel2(Msg_Edit_ChatPass),
                    Child, temp.Password = osstring(60,Msg_Edit_ChatPass,0),
                End,

                Child, olabel2(Msg_Edit_ChatServer),
                Child, temp.Server = ostring(128,Msg_Edit_ChatServer,0),

                Child, olabel2(Msg_Edit_ChatNick),
                Child, HGroup,
                    Child, temp.Nick = ostring(20,Msg_Edit_ChatNick,0),
                    Child, olabel2(Msg_Edit_ChatMantainer),
                    Child, temp.Maintainer = ostring(80,Msg_Edit_ChatMantainer,0),
                End,

                Child, olabel2(Msg_Edit_ChatEMail),
                Child, temp.EMail = ostring(128,Msg_Edit_ChatEMail,0),

                Child, olabel2(Msg_Edit_ChatWWW),
                Child, temp.WWW = ostring(256,Msg_Edit_ChatWWW,0),

                Child, ofreelabel2(Msg_Edit_ChatComment),
                Child, temp.Comment = osscroll(512,Msg_Edit_ChatComment,0),
            End,

            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->CMObj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList);

        DoMethod(temp.WWW, MUIM_Notify, MUIA_Textinput_Acknowledge, MUIV_EveryTime, obj, 3, MUIM_Edit_URL, temp.WWW, URL_WWW);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct TagItem *tag;

    if (tag = FindTagItem(MUIA_Edit_Entry, msg->ops_AttrList))
    {
        data->Current = CMCHAT(tag->ti_Data);

        set(data->Channel,MUIA_Textinput_Contents, data->Current->Channel);
        set(data->Server,MUIA_Textinput_Contents, data->Current->Server);
        set(data->Maintainer,MUIA_Textinput_Contents, data->Current->Maintainer);
        set(data->Nick,MUIA_Textinput_Contents, data->Current->Nick);
        set(data->Comment,MUIA_Textinput_Contents, data->Current->Comment);
        set(data->WWW,MUIA_Textinput_Contents, data->Current->WWW);
        set(data->EMail,MUIA_Textinput_Contents, data->Current->EMail);
        set(data->Password,MUIA_Textinput_Contents, data->Current->Password);
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_Edit_Entry:                *msg->opg_Storage = (ULONG)data->Current; return TRUE;
        case MUIA_CManager_ActiveEditObject: *msg->opg_Storage = (ULONG)data->Channel; return TRUE;

        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mUpdate(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data   *data = INST_DATA(cl,obj);
    register struct CMChat *entry;

    entry = data->Current;

    getstr(data->Channel,entry->Channel);
    getstr(data->Server,entry->Server);
    getstr(data->Maintainer,entry->Maintainer);
    getstr(data->Nick,entry->Nick);
    getstr(data->Comment,entry->Comment);
    getstr(data->WWW,entry->WWW);
    getstr(data->EMail,entry->EMail);
    getstr(data->Password,entry->Password);

    return 0;
}

/***********************************************************************/

static ULONG
mEditURL(struct IClass *cl,Object *obj,struct MUIP_Edit_URL *msg)
{
    return editURLFun(msg->URL,msg->Type);
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:           return mNew(cl,obj,(APTR)msg);
        case OM_SET:           return mSets(cl,obj,(APTR)msg);
        case OM_GET:           return mGet(cl,obj,(APTR)msg);

        case MUIM_Edit_Update: return mUpdate(cl,obj,(APTR)msg);
        case MUIM_Edit_URL:    return mEditURL(cl,obj,(APTR)msg);

        default:               return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initEditIRC(void)
{
    return (ULONG)(lib_EditIRC = MUI_CreateCustomClass(NULL,MUIC_Group,NULL,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeEditIRC(void)
{
    MUI_DeleteCustomClass(lib_EditIRC);
    lib_EditIRC = NULL;
}

/***********************************************************************/
