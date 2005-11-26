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
    Object *app;
    Object *cm;
    Object *string;
    Object *list;
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object         *string, *list;
    register struct TagItem *attrs = msg->ops_AttrList;

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode,     "CMLOGIN",
            MUIA_Window_Title, CM_GetString(Msg_UserWin_Title),
            MUIA_Window_ID,    MAKE_ID( 'c','m','l','o'),

            WindowContents, VGroup,

                Child, HGroup,
                    Child, olabel2(Msg_UserWin_User),
                    Child, string = oqstring(NULL,40,Msg_UserWin_User,0),
                End,

                Child, NListviewObject,
                    MUIA_CycleChain, TRUE,
                    MUIA_NListview_NList, list = NListObject,
                        MUIA_ContextMenu,         NULL,
                        MUIA_NList_ConstructHook, MUIV_NList_ConstructHook_String,
                        MUIA_NList_DestructHook,  MUIV_NList_DestructHook_String,
                    End,
                End,
            End,
            TAG_MORE, attrs))
    {
        register struct data *data = INST_DATA(cl,obj);
        register STRPTR      user = (STRPTR)GetTagData(MUIA_LoginWindow_User,NULL,attrs);
        register LONG        active = MUIV_NList_Active_Off;
        register ULONG       i;

        data->app = (Object *)GetTagData(MUIA_CManager_AppObj,NULL,attrs);
        data->cm  = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,attrs);

        data->string = string;
        data->list   = list;

        set(string,MUIA_QString_AttachedList,list);

        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,1,MUIM_CManager_WinClose);

        DoMethod(string,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,2,MUIM_LoginWindow_Acknowledge,MUIV_LoginWindow_Acknowledge_String);
        DoMethod(string,MUIM_Notify,MUIA_Textinput_Contents,MUIV_EveryTime,obj,2,MUIM_LoginWindow_ChangeActive,MUIV_TriggerValue);

        DoMethod(list,MUIM_Notify,MUIA_NList_DoubleClick,MUIV_EveryTime,obj,2,MUIM_LoginWindow_Acknowledge,MUIV_LoginWindow_Acknowledge_List);
        DoMethod(list,MUIM_Notify,MUIA_NList_Active,MUIV_EveryTime,obj,1,MUIM_LoginWindow_ActiveChange);

        for (i = 0; ; i++)
        {
            REGARRAY UBYTE name[256];

            if (!GetUserName(i,name,sizeof(name))) break;
            DoMethod(list,MUIM_NList_InsertSingle,name,MUIV_NList_Insert_Bottom);
            if ((active==MUIV_NList_Active_Off) && !strcmp(name,user)) active = i;
        }

        if (active!=MUIV_NList_Active_Off) set(list,MUIA_NList_Active,active);
        else set(string,MUIA_Textinput_Contents,user);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mSetup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    SetSuperAttrs(cl,obj,MUIA_Window_DefaultObject,data->list,TAG_DONE);

    return TRUE;
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    switch (msg->opg_AttrID)
    {
        case MUIA_CManager_IsCMWin: *msg->opg_Storage = TRUE; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mClose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    SetSuperAttrs(cl,obj,MUIA_Window_Open,FALSE,TAG_DONE);
    //DoMethod(data->app,OM_REMMEMBER,obj);
    DoMethod(data->app,MUIM_Application_PushMethod,data->cm,2,MUIM_CManager_DisposeObj,obj);

    return 0;
}

/***********************************************************************/

static ULONG
mAcknowledge(struct IClass *cl,Object *obj,struct MUIP_LoginWindow_Acknowledge *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    STRPTR               user;

    if (msg->mode==MUIV_LoginWindow_Acknowledge_String)
        get(data->string,MUIA_Textinput_Contents,&user);
    else DoMethod(data->list,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&user);

    DoMethod(data->cm,MUIM_CManager_Login,user,NULL);

    return 0;
}

/***********************************************************************/

static ULONG
mActiveChange(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    STRPTR               user;

    DoMethod(data->list,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&user);
    if (user) set(data->string,MUIA_Textinput_Contents,user);

    return 0;
}

/***********************************************************************/

static ULONG
mChangeActive(struct IClass *cl,Object *obj,struct MUIP_LoginWindow_ActiveChange *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register STRPTR      c = msg->contents;

    if (*c)
    {
        STRPTR       user;
        register int i, len;

        for (len = strlen(c), i = 0; ; i++)
        {
            DoMethod(data->list,MUIM_NList_GetEntry,i,&user);
            if (!user) break;

            if (!strncmp(c,user,len))
            {
                nnset(data->list,MUIA_NList_Active,i);
                break;
            }
        }
    }

    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:                        return mNew(cl,obj,(APTR)msg);
        case OM_GET:                        return mGet(cl,obj,(APTR)msg);

        case MUIM_Window_Setup:             return mSetup(cl,obj,(APTR)msg);

        case MUIM_CManager_WinClose:        return mClose(cl,obj,(APTR)msg);
        case MUIM_LoginWindow_Acknowledge:  return mAcknowledge(cl,obj,(APTR)msg);
        case MUIM_LoginWindow_ActiveChange: return mActiveChange(cl,obj,(APTR)msg);
        case MUIM_LoginWindow_ChangeActive: return mChangeActive(cl,obj,(APTR)msg);

        default:                            return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initLoginWindow(void)
{
    return (ULONG)(lib_LoginWindow = MUI_CreateCustomClass(NULL,NULL,lib_MatherWin,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeLoginWindow(void)
{
    MUI_DeleteCustomClass(lib_LoginWindow);
    lib_LoginWindow = NULL;
}

/***********************************************************************/
