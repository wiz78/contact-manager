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
    struct CMWWW *Current;

    Object       *CMObj;
    Object       *Name;
    Object       *WWW;
    Object       *Comment;
    Object       *WebMaster;
    Object       *EMail;
    Object       *Alias;
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data     temp;
    register Object *bt;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode,   "CMWWW",
            MUIA_Frame,      MUIV_Frame_Group,
            MUIA_Background, MUII_GroupBack,

            Child, ColGroup(2),

                Child, olabel2(Msg_Edit_WWWName),
                Child, temp.Name = ostring(60,Msg_Edit_WWWName,0),

                Child, olabel2(Msg_Edit_WWWHost),
                Child, HGroup,
                    MUIA_Group_Spacing, 1,
                    Child, temp.WWW = ostring(256,Msg_Edit_WWWHost,0),
                    Child, bt = MUI_MakeObject(MUIO_PopButton,MUII_PopUp),
                End,

                Child, olabel2(Msg_Edit_WWWAlias ),
                Child, temp.Alias = ostring(40,Msg_Edit_WWWAlias,0),

                Child, olabel2(Msg_Edit_WWWMaster),
                Child, temp.WebMaster = ostring(80,Msg_Edit_WWWMaster,0),

                Child, olabel2(Msg_Edit_WWWEMail),
                Child, temp.EMail = ostring(128,Msg_Edit_WWWEMail,0),

                Child, ofreelabel2(Msg_Edit_WWWComment),
                Child, temp.Comment = osscroll(512,Msg_Edit_WWWComment,0),
            End,

            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->CMObj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList);

        DoMethod(data->WWW,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,3,MUIM_Edit_URL,temp.WWW,URL_WWW);

        if (OpenURLBase) DoMethod(bt,MUIM_Notify,MUIA_Pressed,TRUE,obj,2,MUIM_Edit_GoURL,data->WWW);
        else set(bt,MUIA_ShowMe,0);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register struct TagItem     *tag;

    if (tag = FindTagItem(MUIA_Edit_Entry,msg->ops_AttrList))
    {
        data->Current = CMWWW(tag->ti_Data);

        set(data->Name,MUIA_Textinput_Contents, data->Current->Name);
        set(data->Comment,MUIA_Textinput_Contents, data->Current->Comment);
        set(data->WWW,MUIA_Textinput_Contents, data->Current->WWW);
        set(data->Alias,MUIA_Textinput_Contents, data->Current->Alias);
        set(data->WebMaster,MUIA_Textinput_Contents, data->Current->WebMaster);
        set(data->EMail,MUIA_Textinput_Contents, data->Current->EMail);
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
        case MUIA_CManager_ActiveEditObject: *msg->opg_Storage = (ULONG)data->Name; return TRUE;

        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mUpdate(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data  *data = INST_DATA(cl,obj);
    register struct CMWWW *entry;

    entry = data->Current;

    getstr(data->Name,entry->Name);
    getstr(data->Comment,entry->Comment);
    getstr(data->WWW,entry->WWW);
    getstr(data->WebMaster,entry->WebMaster);
    getstr(data->EMail,entry->EMail);
    getstr(data->Alias,entry->Alias);

    return 0;
}

/***********************************************************************/

static ULONG
mEditURL(struct IClass *cl,Object *obj,struct MUIP_Edit_URL *msg)
{
    return editURLFun(msg->URL,msg->Type);
}

/***********************************************************************/

static ULONG
mEditGoURL(struct IClass *cl,Object *obj,struct MUIP_Edit_GoURL *msg)
{
    return editGoURLFun((STRPTR)xget(msg->URL,MUIA_Textinput_Contents));
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
        case MUIM_Edit_GoURL:  return mEditGoURL(cl,obj,(APTR)msg);

        default:               return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initEditWWW(void)
{
    return (ULONG)(lib_EditWWW = MUI_CreateCustomClass(NULL,MUIC_Group,NULL,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeEditWWW(void)
{
    MUI_DeleteCustomClass(lib_EditWWW);
    lib_EditWWW = NULL;
}

/***********************************************************************/
