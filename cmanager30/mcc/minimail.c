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
#include <mui/MiniMailer_mcc.h>

/***********************************************************************/

struct data
{
    Object *app;
    Object *miniMail;
    Object *cm;

    UBYTE  file[256];

    ULONG  flags;
};

/***********************************************************************/

static ULONG
mNew( struct IClass *cl, Object *obj, struct opSet *msg)
{
    register Object         *miniMail;
    register struct TagItem *attrs = msg->ops_AttrList;

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode,      "CMQUICKEMAIL",
            MUIA_Window_Title,  CM_GetString(Msg_MiniMailWin_Title),
            MUIA_Window_ID,     MAKE_ID('c','m','m','m'),
            MUIA_Window_Height, MUIV_Window_Height_Scaled,

            WindowContents, miniMail = MiniMailerObject,
                MUIA_MiniMailer_MinVer, 18,
            End,

            TAG_MORE,attrs))
    {
        register struct data *data = INST_DATA(cl,obj);
        REGARRAY UBYTE       buf[256];
        register STRPTR      user;

        data->miniMail = miniMail;

        data->app = (Object *)GetTagData(MUIA_CManager_AppObj,NULL,attrs);
        data->cm  = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,attrs);
        user      = (STRPTR)GetTagData(MUIA_MiniMail_User,(ULONG)"",attrs);

        strcpy(data->file,lib_CMPath);
        if (*user) sprintf(buf,"%s/MiniMail.prefs",user);
        else strcpy(buf,"MiniMail.prefs");
        AddPart(data->file,buf,sizeof(data->file));

        DoMethod(miniMail,MUIM_MiniMailer_LoadPrefs,data->file);

        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,2,MUIM_CManager_WinClose,0);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mDispose( struct IClass *cl, Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    DoMethod(data->miniMail,MUIM_MiniMailer_SavePrefs,data->file);

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mGet( struct IClass *cl, Object *obj, struct opGet *msg)
{
    switch (msg->opg_AttrID)
    {
        case MUIA_CManager_IsCMWin: *msg->opg_Storage = TRUE; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mSets( struct IClass *cl, Object *obj, struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    struct TagItem          *tags;
    register struct TagItem *tag;

    for (tags = msg->ops_AttrList; tag = NextTagItem(&tags); )
    {
        register ULONG tidata = tag->ti_Data;

        switch (tag->ti_Tag)
        {
            case MUIA_MiniMail_Entry:
            {
                switch (CMENTRY(tidata)->Type)
                {
                    case CMEntry_User:
                        set(data->miniMail,MUIA_MiniMailer_To,*CMUSER(tidata)->EMail ? CMUSER(tidata)->EMail : CMUSER(tidata)->BEMail);
                        break;

                    case CMEntry_WWW:
                        set(data->miniMail,MUIA_MiniMailer_To,CMWWW(tidata)->EMail);
                        break;

                    case CMEntry_FTP:
                        set(data->miniMail,MUIA_MiniMailer_To,CMFTP(tidata)->EMail);
                        break;

                    case CMEntry_Chat:
                        set(data->miniMail,MUIA_MiniMailer_To,CMCHAT(tidata)->EMail);
                        break;

                    case CMEntry_Telnet:
                        set(data->miniMail,MUIA_MiniMailer_To,CMTELNET(tidata)->EMail);
                        break;
                }
            }
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mClose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    SetSuperAttrs(cl,obj,MUIA_Window_Open,FALSE,TAG_DONE);
    DoMethod(_app(obj),MUIM_Application_PushMethod,data->cm,2,MUIM_CManager_DisposeObj,obj);

    return 0;
}

/***********************************************************************/

static ULONG
mTranslate(struct IClass *cl,Object *obj,struct MUIP_MiniMailer_Translate *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMPrefs       *prefs;

    get(data->cm,MUIA_CManager_Prefs,&prefs);
    stccpy(msg->codesetName,prefs->CodesetName,msg->codesetNameLen);

    return (ULONG)msg->text;
}

/***********************************************************************/

static ULONG
mFreeTranslated(struct IClass *cl,Object *obj,Msg msg)
{
    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                          return mNew(cl,obj,(APTR)msg);
        case OM_DISPOSE:                      return mDispose(cl,obj,(APTR)msg);
        case OM_GET:                          return mGet(cl,obj,(APTR)msg);
        case OM_SET:                          return mSets(cl,obj,(APTR)msg);
        case MUIM_CManager_WinClose:          return mClose(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_Translate:       return mTranslate(cl,obj,(APTR)msg);
        case MUIM_MiniMailer_FreeTranslated:  return mFreeTranslated(cl,obj,(APTR)msg);
        default:                              return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initMiniMailWindow(void)
{
    return (ULONG)(lib_miniMailWindow = MUI_CreateCustomClass(NULL,NULL,lib_MatherWin,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeMiniMailWindow(void)
{
    MUI_DeleteCustomClass(lib_miniMailWindow);
    lib_miniMailWindow = NULL;
}

/***********************************************************************/
