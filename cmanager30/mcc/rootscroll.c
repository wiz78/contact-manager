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
    APTR    menu;
    ULONG   flags;
};

enum
{
    FLG_Disabled = 1<<0,
};

/***********************************************************************/

static ULONG tnMenuIDs[] =
{
    Msg_String_Menu_Title,
        Msg_String_Menu_Cut,
    	Msg_String_Menu_Copy,
    	Msg_String_Menu_Paste,
    	0,
    	Msg_String_Menu_External,
    	Msg_String_Menu_Insert,
    	0,
    	Msg_String_Menu_WordWrap
};

static struct NewMenu tnMenu[] =
{
    MTITLE(Msg_String_Menu_Title),
        MITEM(Msg_String_Menu_Cut),
        MITEM(Msg_String_Menu_Copy),
        MITEM(Msg_String_Menu_Paste),
        MBAR,
        MITEM(Msg_String_Menu_External),
        MITEM(Msg_String_Menu_Insert),
        MBAR,
        MTITEM(Msg_String_Menu_WordWrap),
    MEND
};

static  ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Textinput_MinVersion,   29,
            MUIA_Textinput_DefaultPopup, FALSE,
            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        if (GetTagData(MUIA_Disabled,FALSE,msg->ops_AttrList))
            data->flags |= FLG_Disabled;

    	data->menu = MUI_MakeObject(MUIO_MenustripNM,tnMenu,0);
        if (data->menu) SetSuperAttrs(cl,obj,MUIA_ContextMenu,TRUE,TAG_DONE);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->menu) MUI_DisposeObject(data->menu);

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static  ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct TagItem *tag;

    if (tag = FindTagItem(MUIA_Disabled,msg->ops_AttrList))
        if (tag->ti_Data) data->flags |= FLG_Disabled;
        else data->flags &= ~FLG_Disabled;

    /* ?!?!?!?!?!? Texinput bug */
    if (tag = FindTagItem(MUIA_ContextMenu,msg->ops_AttrList))
        tag->ti_Tag = TAG_IGNORE;

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mContextMenuChoice(struct IClass *cl,Object *obj,struct MUIP_ContextMenuChoice *msg)
{
    switch (muiUserData(msg->item))
    {
        case Msg_String_Menu_Cut:
            DoSuperMethod(cl,obj,MUIM_Textinput_DoCut);
            break;

        case Msg_String_Menu_Copy:
            DoSuperMethod(cl,obj,MUIM_Textinput_DoCopy);
            break;

        case Msg_String_Menu_Paste:
            DoSuperMethod(cl,obj,MUIM_Textinput_DoPaste);
            break;

        case Msg_String_Menu_External:
            DoSuperMethod(cl,obj,MUIM_Textinput_ExternalEdit);
            break;

        case Msg_String_Menu_Insert:
            DoSuperMethod(cl,obj,MUIM_Textinput_DoInsertFile);
            break;

        case Msg_String_Menu_WordWrap:
        {
            ULONG ww;

            superget(cl,obj,MUIA_Textinput_WordWrap,&ww);
            SetSuperAttrs(cl,obj,MUIA_Textinput_WordWrap,!ww,TAG_DONE);
            break;
        }
    }

    return 0;
}

/***********************************************************************/

static ULONG
mContextMenuBuild(struct IClass *cl,Object *obj,struct MUIP_NList_ContextMenuBuild *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->flags & FLG_Disabled) return NULL;

    if (data->menu)
    {
        ULONG ww;

        superget(cl,obj,MUIA_Textinput_WordWrap,&ww);


        set((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_String_Menu_WordWrap),MUIA_Menuitem_Checked,ww);
        return (ULONG)data->menu;
    }

    return NULL;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                 return mNew(cl,obj,(APTR)msg);
        case OM_DISPOSE:             return mDispose(cl,obj,(APTR)msg);
        case OM_SET:                 return mSets(cl,obj,(APTR)msg);

        case MUIM_ContextMenuBuild:  return mContextMenuBuild(cl,obj,(APTR)msg);
        case MUIM_ContextMenuChoice: return mContextMenuChoice(cl,obj,(APTR)msg);

        default:                     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initRootScroll(void)
{
    if (lib_RootScroll = MUI_CreateCustomClass(NULL,MUIC_Textinputscroll,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        localizeMenus(tnMenu,tnMenuIDs);

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
disposeRootScroll(void)
{
    MUI_DeleteCustomClass(lib_RootScroll);
    lib_RootScroll = NULL;
}

/***********************************************************************/
