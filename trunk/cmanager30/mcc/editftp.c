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
    struct CMFTP *Current;

    Object       *CMObj;
    Object       *Name;
    Object       *FTP;
    Object       *Alias;
    Object       *EMail;
    Object       *Comment;
    Object       *Advanced;
    Object       *AdvancedGroup;
    Object       *Port;
    Object       *Username;
    Object       *Password;
    Object       *IsADT;
    Object       *Anonymous;
    Object       *Quiet;
    Object       *Retries;
    Object       *Compression;
    Object       *List;
    Object       *UseLocal;
    Object       *Local;
    Object       *WinType;
    Object       *ShowFTPOut;
    Object       *ShowHidden;
    Object       *UseProxy;
    Object       *SendNOOP;
};

/***********************************************************************/

static ULONG LabelsFTPIDs[] =
{
    Msg_Edit_FTPSettingsTitle,
    Msg_Edit_FTPAdvancedTitle,
    0
};
static STRPTR LabelsFTP[IDSSIZE(LabelsFTPIDs)];

static ULONG WinTypesIDs[] =
{
    Msg_Edit_FTPWinTypeDual,
    Msg_Edit_FTPWinTypeSingle,
    0
};
static STRPTR WinTypes[IDSSIZE(WinTypesIDs)];

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data      temp;
    register Object  *Local, *bt;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode,   "CMFTP",
            Child, RegisterGroup(LabelsFTP),
                MUIA_CycleChain, TRUE,

                /* Settings */
                Child, ColGroup(2),
                    MUIA_Frame, MUIV_Frame_Virtual,

                    Child, olabel2(Msg_Edit_FTPName),
                    Child, temp.Name = ostring(80,Msg_Edit_FTPName,0),

                    Child, olabel2(Msg_Edit_FTPHost),
                    Child, HGroup,
                        MUIA_Group_Spacing, 1,
                        Child, temp.FTP = ostring(256,Msg_Edit_FTPHost,0),
                        Child, bt = MUI_MakeObject(MUIO_PopButton,MUII_PopUp),
                    End,

                    Child, olabel2(Msg_Edit_FTPAlias),
                    Child, temp.Alias = ostring(40,Msg_Edit_FTPAlias,0),

                    Child, olabel2(Msg_Edit_FTPEMail),
                    Child, temp.EMail = ostring(128,Msg_Edit_FTPEMail,0),

                    Child, olabel2(Msg_Edit_FTPLocal),
                    Child, HGroup, MUIA_Group_Spacing, 2,
                        Child, temp.UseLocal = ocheck(Msg_Edit_FTPLocalUse,0),
                        Child, Local = ofile(&temp.Local,256,Msg_Edit_FTPLocal,0,TRUE),
                    End,

                    Child, ofreelabel2(Msg_Edit_FTPComment),
                    Child, temp.Comment = osscroll(512,Msg_Edit_FTPComment,0),
                End,

                /* Advanced */
                Child, VGroup,

                    Child, HGroup,
                        Child, HSpace(0),
                        Child, temp.Advanced = ocheck(Msg_Edit_FTPAdvanced,0),
                        Child, olabel1(Msg_Edit_FTPAdvanced),
                        Child, HSpace(0),
                    End,

                    Child, temp.AdvancedGroup = VGroup,
                        MUIA_Frame,    MUIV_Frame_Virtual,
                        MUIA_Disabled, TRUE,

                        Child, ColGroup(2),

                            Child, olabel2(Msg_Edit_FTPUser),
                            Child, temp.Username = ostring(60,Msg_Edit_FTPUser,0),

                            Child, olabel2(Msg_Edit_FTPPass),
                            Child, temp.Password = osstring(60,Msg_Edit_FTPPass,0),

                            Child, olabel2(Msg_Edit_FTPPort),
                            Child, HGroup,
                                Child, temp.Port = ointeger(12,Msg_Edit_FTPPort,0),
                                Child, olabel2(Msg_Edit_FTPRetries),
                                Child, temp.Retries = oslider(Msg_Edit_FTPRetries,0,0,50),
                            End,

                            Child, olabel2(Msg_Edit_FTPWinType),
                            Child, temp.WinType = ocycle(WinTypes,Msg_Edit_FTPWinType,0),
                        End,

                        Child, fhspace(),

                        Child, ColGroup(10),

                            Child, HSpace(0),
                            Child, temp.IsADT = ocheck(Msg_Edit_FTPADT,0),
                            Child, ollabel1(Msg_Edit_FTPADT),
                            Child, HSpace(0),
                            Child, temp.Anonymous = ocheck(Msg_Edit_FTPAnon,0),
                            Child, ollabel1(Msg_Edit_FTPAnon),
                            Child, HSpace(0),
                            Child, temp.Quiet = ocheck(Msg_Edit_FTPQuiet,0),
                            Child, ollabel1(Msg_Edit_FTPQuiet),
                            Child, HSpace(0),

                            Child, HSpace(0),
                            Child, temp.Compression = ocheck(Msg_Edit_FTPCompression,0),
                            Child, ollabel1(Msg_Edit_FTPCompression),
                            Child, HSpace(0),
                            Child, temp.ShowFTPOut = ocheck(Msg_Edit_FTPOutput,0),
                            Child, ollabel1(Msg_Edit_FTPOutput),
                            Child, HSpace(0),
                            Child, temp.ShowHidden = ocheck(Msg_Edit_FTPHidden,0),
                            Child, ollabel1(Msg_Edit_FTPHidden),
                            Child, HSpace(0),

                            Child, HSpace(0),
                            Child, temp.SendNOOP = ocheck(Msg_Edit_FTPNOOPs,0),
                            Child, ollabel1(Msg_Edit_FTPNOOPs),
                            Child, HSpace(0),
                            Child, temp.UseProxy = ocheck(Msg_Edit_FTPUseProxy,0),
                            Child, ollabel1(Msg_Edit_FTPUseProxy),
                            Child, HSpace(0),
                            Child, HSpace(0),
                            Child, HSpace(0),
                            Child, HSpace(0),
                        End,

                        Child, VSpace(0),
                    End,
                End,
            End,

            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->CMObj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList);

        DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Disabled,TRUE,temp.Username,temp.Password,temp.Anonymous,NULL);

        DoMethod(temp.Advanced,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,2,MUIM_Edit_FTPAdvanced,MUIV_TriggerValue);
        DoMethod(temp.Anonymous,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,temp.Username,3,MUIM_Set, MUIA_Disabled,MUIV_TriggerValue);
        DoMethod(temp.Anonymous,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,temp.Password,3,MUIM_Set, MUIA_Disabled, MUIV_TriggerValue);

        //DoMethod(temp.FTP,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,3,MUIM_Edit_URL,temp.FTP,URL_FTP);

        if (OpenURLBase) DoMethod(bt,MUIM_Notify,MUIA_Pressed,TRUE,obj,2,MUIM_Edit_GoURL,data->FTP);
        else set(bt,MUIA_ShowMe,0);
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
        register ULONG flags, eflags;

        data->Current = CMFTP(tag->ti_Data);

        flags  = data->Current->Flags;
        eflags = data->Current->ExtraFlags;

        set(data->Name,MUIA_Textinput_Contents,data->Current->Name);
        set(data->Comment,MUIA_Textinput_Contents,data->Current->Comment);
        set(data->FTP,MUIA_Textinput_Contents,data->Current->FTP);
        set(data->Username,MUIA_Textinput_Contents,data->Current->Username);
        set(data->Password,MUIA_Textinput_Contents,data->Current->Password);
        set(data->Port,MUIA_Textinput_Integer,data->Current->Port);
        set(data->Retries,MUIA_Numeric_Value,data->Current->Retries);
        set(data->Local,MUIA_Textinput_Contents,data->Current->Local);
        set(data->Alias,MUIA_Textinput_Contents,data->Current->Alias);
        set(data->EMail,MUIA_Textinput_Contents,data->Current->EMail);
        set(data->WinType,MUIA_Cycle_Active,data->Current->WindowStyle);

        set(data->Advanced,MUIA_Selected,flags & CMFTPFLG_Advanced);
        set(data->Quiet,MUIA_Selected,flags & CMFTPFLG_Quiet);
        set(data->Anonymous,MUIA_Selected,flags & CMFTPFLG_Anonymous);
        set(data->Compression,MUIA_Selected,flags & CMFTPFLG_Compress);
        set(data->UseLocal,MUIA_Selected,flags & CMFTPFLG_Local);
        set(data->UseProxy,MUIA_Selected,flags & CMFTPFLG_Proxy);
        set(data->IsADT,MUIA_Selected,flags & CMFTPFLG_ADT);
        set(data->ShowHidden,MUIA_Selected,eflags & CMFTPEFLG_ShowHidden);
        set(data->ShowFTPOut,MUIA_Selected,eflags & CMFTPEFLG_ShowFTPOutput);
        set(data->SendNOOP,MUIA_Selected,eflags & CMFTPEFLG_Noops);
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
    register struct CMFTP *entry;

    entry = data->Current;

    getstr(data->Name,entry->Name);
    getstr(data->Comment,entry->Comment);
    getstr(data->FTP,entry->FTP);
    getstr(data->Username,entry->Username);
    getstr(data->Password,entry->Password);
    getstr(data->Local,entry->Local);
    getstr(data->Alias,entry->Alias);
    getstr(data->EMail,entry->EMail);

    entry->Port        = xget(data->Port,MUIA_Textinput_Integer);
    entry->Retries     = xget(data->Retries,MUIA_Numeric_Value);
    entry->WindowStyle = xget(data->WinType,MUIA_Cycle_Active);

    entry->Flags &= ~(CMFTPFLG_Advanced | CMFTPFLG_Quiet | CMFTPFLG_Anonymous | CMFTPFLG_Compress | CMFTPFLG_ADT | CMFTPFLG_Local | CMFTPFLG_Proxy);
    entry->ExtraFlags &= ~(CMFTPEFLG_ShowFTPOutput | CMFTPEFLG_Noops | CMFTPEFLG_ShowHidden);

    if (xget(data->Advanced, MUIA_Selected)) entry->Flags |= CMFTPFLG_Advanced;
    if (xget(data->Quiet, MUIA_Selected)) entry->Flags |= CMFTPFLG_Quiet;
    if (xget(data->Anonymous, MUIA_Selected)) entry->Flags |= CMFTPFLG_Anonymous;
    if (xget(data->Compression, MUIA_Selected)) entry->Flags |= CMFTPFLG_Compress;
    if (xget(data->IsADT, MUIA_Selected)) entry->Flags |= CMFTPFLG_ADT;
    if (xget(data->UseLocal, MUIA_Selected)) entry->Flags |= CMFTPFLG_Local;
    if (xget(data->UseProxy, MUIA_Selected)) entry->Flags |= CMFTPFLG_Proxy;
    if (xget(data->ShowHidden, MUIA_Selected)) entry->ExtraFlags |= CMFTPEFLG_ShowHidden;
    if (xget(data->ShowFTPOut, MUIA_Selected)) entry->ExtraFlags |= CMFTPEFLG_ShowFTPOutput;
    if (xget(data->SendNOOP, MUIA_Selected)) entry->ExtraFlags |= CMFTPEFLG_Noops;

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

static ULONG
mFTPAdvanced(struct IClass *cl,Object *obj,struct MUIP_Edit_FTPAdvanced *msg)
{
    register struct data  *data = INST_DATA(cl,obj);

    if (msg->val)
    {
        ULONG val;

        set(data->AdvancedGroup,MUIA_Disabled,FALSE);

        get(data->Anonymous,MUIA_Selected,&val);
        set(data->Username,MUIA_Disabled,val);
        set(data->Password,MUIA_Disabled,val);
    }
    else set(data->AdvancedGroup,MUIA_Disabled,TRUE);

    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:                return mNew(cl,obj,(APTR)msg);
        case OM_SET:                return mSets(cl,obj,(APTR)msg);
        case OM_GET:                return mGet(cl,obj,(APTR)msg);

        case MUIM_Edit_FTPAdvanced: return mFTPAdvanced(cl,obj,(APTR)msg);
        case MUIM_Edit_Update:      return mUpdate(cl,obj,(APTR)msg);
        case MUIM_Edit_URL:         return mEditURL(cl,obj,(APTR)msg);
        case MUIM_Edit_GoURL:       return mEditGoURL(cl,obj,(APTR)msg);

        default:                    return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initEditFTP(void)
{
    if (lib_EditFTP = MUI_CreateCustomClass(NULL,MUIC_Group,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        localizeArray(LabelsFTP,LabelsFTPIDs);
        localizeArray(WinTypes,WinTypesIDs);

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
disposeEditFTP(void)
{
    MUI_DeleteCustomClass(lib_EditFTP);
    lib_EditFTP = NULL;
}

/**********************************************************************/
