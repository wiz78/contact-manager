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
    struct CMTelnet *Current;

    Object          *CMObj;
    Object          *Host;
    Object          *Port;
    Object          *SSH;
    Object          *Login;
    Object          *Password;
    Object          *Alias;
    Object          *EMail;
    Object          *Comment;
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data temp;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl, obj,
            MUIA_HelpNode,   "CMTELNET",
            MUIA_Frame,      MUIV_Frame_Group,
            MUIA_Background, MUII_GroupBack,

            Child, ColGroup(2),

                Child, olabel2(Msg_Edit_TelnetHost),
                Child, HGroup,
                    Child, temp.Host = ostring(128,Msg_Edit_TelnetHost,0),

                    Child, olabel2(Msg_Edit_TelnetPort),
                    Child, temp.Port = ointeger(12,Msg_Edit_TelnetPort,0),

                    Child, olabel1(Msg_Edit_TelnetSSH),
                    Child, temp.SSH = ocheck(Msg_Edit_TelnetSSH,0),
                End,

                Child, olabel2(Msg_Edit_TelnetLogin),
                Child, HGroup,
                    Child, temp.Login = ostring(128, Msg_Edit_TelnetLogin,0),

                    Child, olabel2(Msg_Edit_TelnetPass),
                    Child, temp.Password = osstring(60, Msg_Edit_TelnetPass,0),
                End,

                Child, olabel2(Msg_Edit_TelnetAlias),
                Child, temp.Alias = ostring(40,Msg_Edit_TelnetAlias,0),

                Child, olabel2(Msg_Edit_TelnetEMail),
                Child, temp.EMail = ostring(128,Msg_Edit_TelnetEMail,0),

                Child, ofreelabel2(Msg_Edit_TelnetComment),
                Child, temp.Comment = osscroll(512,Msg_Edit_TelnetComment,0),
            End,

            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->CMObj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList);

        set(temp.Port, MUIA_Weight, 30);
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
        data->Current = CMTELNET(tag->ti_Data);

        set(data->Host,MUIA_Textinput_Contents,data->Current->Host);
        set(data->Login,MUIA_Textinput_Contents,data->Current->Login);
        set(data->Password, MUIA_Textinput_Contents,data->Current->Password);
        set(data->Alias,MUIA_Textinput_Contents,data->Current->Alias);
        set(data->EMail,MUIA_Textinput_Contents,data->Current->EMail);
        set(data->Comment,MUIA_Textinput_Contents,data->Current->Comment);
        set(data->Port,MUIA_Textinput_Integer,  data->Current->Port);

        set(data->SSH,MUIA_Selected,data->Current->Flags & CMTELNETFLG_SSH);
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
        case MUIA_CManager_ActiveEditObject: *msg->opg_Storage = (ULONG)data->Host; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mUpdate(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data     *data = INST_DATA(cl,obj);
    register struct CMTelnet *entry;

    entry = data->Current;

    getstr(data->Host,entry->Host);
    getstr(data->Login,entry->Login);
    getstr(data->Password,entry->Password);
    getstr(data->Alias,entry->Alias);
    getstr(data->EMail,entry->EMail);
    getstr(data->Comment,entry->Comment);

    entry->Port = xget(data->Port, MUIA_Textinput_Integer);

    entry->Flags &= ~CMTELNETFLG_SSH;

    if (xget(data->SSH,MUIA_Selected)) entry->Flags |= CMTELNETFLG_SSH;

    return 0;
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

        default:               return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initEditTelnet(void)
{
    return (ULONG)(lib_EditTelnet = MUI_CreateCustomClass(NULL,MUIC_Group,NULL,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeEditTelnet(void)
{
    MUI_DeleteCustomClass(lib_EditTelnet);
    lib_EditTelnet = NULL;
}

/***********************************************************************/
