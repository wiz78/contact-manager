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
    Object          *this;
    Object          *cmobj;
    APTR            menu;

    ULONG           flags;
};

enum
{
    FLG_TextOnly = 1<<0,
};

/***********************************************************************/

static ULONG tnMenuIDs[] =
{
    Msg_Bar_Menu_Title,
    Msg_Bar_Menu_ViewMode_TextGfx,
    Msg_Bar_Menu_ViewMode_Gfx,
    Msg_Bar_Menu_ViewMode_Text,
    0,
    Msg_Bar_Menu_Borderless,
    Msg_Bar_Menu_Sunny,
    Msg_Bar_Menu_Raised,
    0,
    Msg_Menu_Configure
};

static struct NewMenu tnMenu[] =
{
    MTITLE(Msg_Bar_Menu_Title),
        MEITEM(Msg_Bar_Menu_ViewMode_TextGfx,2+4),
        MEITEM(Msg_Bar_Menu_ViewMode_Gfx,1+4),
        MEITEM(Msg_Bar_Menu_ViewMode_Text,1+2),
        MBAR,
        MTITEM(Msg_Bar_Menu_Borderless),
        MTITEM(Msg_Bar_Menu_Sunny),
        MTITEM(Msg_Bar_Menu_Raised),
        MBAR,
        MITEM(Msg_Menu_Configure),
    MEND
};

static ULONG buttonsIDs[] =
{
    Msg_Gad_Add,    Msg_Gad_AddHelp,
    Msg_Gad_Clone,  Msg_Gad_CloneHelp,
    Msg_Gad_Edit,   Msg_Gad_EditHelp,
    Msg_Gad_Remove, Msg_Gad_RemoveHelp,
    Msg_Gad_Sort,   Msg_Gad_SortHelp,
    Msg_Gad_Save,   Msg_Gad_SaveHelp,
    Msg_Gad_Search, Msg_Gad_SearchHelp,
    0,		    0,
    Msg_Gad_Prefs,  Msg_Gad_PrefsHelp,
    Msg_Gad_Login,  Msg_Gad_LoginHelp
};

static struct MUIS_TheBar_Button buttons[] =
{
    {0, B_ADD},
    {1, B_CLONE},
    {2, B_EDIT},
    {3, B_REMOVE},
    {4, B_SORT, NULL, NULL, MUIV_TheBar_ButtonFlag_Hide},
    {5, B_SAVE},
    {6, B_SEARCH},
    {MUIV_TheBar_ButtonSpacer,-1},
    {7, B_PREFS},
    {8, B_LOGIN},
    {MUIV_TheBar_End}
};

static STRPTR pics[] =
{
    "Add",
    "Clone",
    "Edit",
    "Remove",
    "Sort",
    "Save",
    "Search",
    "Prefs",
    "Login",
    NULL
};

static STRPTR spics[] =
{
    "Add_S",
    "Clone_S",
    "Edit_S",
    "Remove_S",
    "Sort_S",
    "Save_S",
    "Search_S",
    "Prefs_S",
    "Login_S",
    NULL
};

static STRPTR dpics[] =
{
    "Add_G",
    "Clone_G",
    "Edit_G",
    "Remove_G",
    "Sort_G",
    "Save_G",
    "Search_G",
    "Prefs_G",
    "Login_G",
    NULL
};

static  ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    REGARRAY UBYTE picsDrawer[256];

    strcpy(picsDrawer,lib_CMPath);
    AddPart(picsDrawer,DEF_ImagePath,sizeof(picsDrawer));

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Group_Horiz,       	  TRUE,
            MUIA_TheBar_MinVer,     	  19,
            MUIA_TheBar_Buttons,    	  buttons,
            MUIA_TheBar_PicsDrawer, 	  picsDrawer,
            MUIA_TheBar_Pics,       	  pics,
            MUIA_TheBar_SelPics,    	  spics,
            MUIA_TheBar_DisPics,    	  dpics,
            MUIA_TheBar_DragBar,    	  TRUE,
            MUIA_TheBar_Frame,      	  TRUE,
	    MUIA_TheBar_IgnoreAppareance, TRUE,
	    MUIA_TheBar_ForceWindowActivity, TRUE,
            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);
        ULONG                tonly;

        data->this = obj;

        superget(cl,obj,MUIA_TheBar_TextOnly,&tonly);
        if (tonly) data->flags |= FLG_TextOnly;

        if (data->menu = MUI_MakeObject(MUIO_MenustripNM,tnMenu,0))
        {
            if (tonly)
            {
                set((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_Bar_Menu_ViewMode_TextGfx),MUIA_Menuitem_Enabled,FALSE);
                set((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_Bar_Menu_ViewMode_Gfx),MUIA_Menuitem_Enabled,FALSE);
                set((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_Bar_Menu_ViewMode_Text),MUIA_Menuitem_Enabled,FALSE);
            }

            SetSuperAttrs(cl,obj,MUIA_ContextMenu,TRUE,TAG_DONE);
    	}
    }

    return (ULONG)obj;
}

/***********************************************************************/

static  ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->menu) MUI_DisposeObject(data->menu);

    return DoSuperMethodA(cl,obj,msg);

}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register Object      *cmobj;

    if (cmobj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList))
        data->cmobj = cmobj;

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***************************************************************************/

static ULONG
mContextMenuChoice(struct IClass *cl,Object *obj,struct MUIP_ContextMenuChoice *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register Object 	 *cmobj = data->cmobj;
    register ULONG	 id = muiUserData(msg->item);

    if (id==Msg_Menu_Configure) DoMethod(cmobj,MUIM_CManager_PrefsWindow,MUIV_PrefsWindow_ActivePage_ButtonsBar);
    else
    {
        struct ButtonsBarPrefs *oprefs, prefs;

        get(cmobj,MUIA_CManager_ButtonsBarPrefs,&oprefs),
        copymem(&prefs,oprefs,sizeof(prefs));

        switch (id)
        {
            case Msg_Bar_Menu_ViewMode_TextGfx:
                prefs.ViewMode = MUIV_TheBar_ViewMode_TextGfx;
                break;

            case Msg_Bar_Menu_ViewMode_Gfx:
                prefs.ViewMode = MUIV_TheBar_ViewMode_Gfx;
                break;

            case Msg_Bar_Menu_ViewMode_Text:
                prefs.ViewMode = MUIV_TheBar_ViewMode_Text;
                break;

            case Msg_Bar_Menu_Borderless:
                if (prefs.Flags & SBFLG_Borderless) prefs.Flags &= ~SBFLG_Borderless;
                else prefs.Flags |= SBFLG_Borderless;
                break;

            case Msg_Bar_Menu_Raised:
                if (prefs.Flags & SBFLG_Raised) prefs.Flags &= ~SBFLG_Raised;
                else prefs.Flags |= SBFLG_Raised;
                break;

            case Msg_Bar_Menu_Sunny:
                if (prefs.Flags & SBFLG_Sunny) prefs.Flags &= ~SBFLG_Sunny;
                else prefs.Flags |= SBFLG_Sunny;
                break;
        }

        /* ?!?!? */
        if (!(prefs.Flags & SBFLG_Borderless)) prefs.Flags &= ~SBFLG_Raised;

        DoMethod(cmobj,MUIM_CManager_SetButtonsBarPrefs,&prefs);
    }

    return 0;
}

/***********************************************************************/

static ULONG
mContextMenuBuild(struct IClass *cl,Object *obj,struct MUIP_NList_ContextMenuBuild *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->menu)
    {
        struct ButtonsBarPrefs *prefs;
        register ULONG         vm = 0, bl; //gcc
        ULONG                  dm;

        get(data->cmobj,MUIA_CManager_ButtonsBarPrefs,&prefs);

        if (!(data->flags & FLG_TextOnly)) vm = prefs->ViewMode;
        bl = prefs->Flags & SBFLG_Borderless;
        get(obj,MUIA_TheBar_DisMode,&dm);

        if (!(data->flags & FLG_TextOnly))
        {
            if (vm==MUIV_TheBar_ViewMode_TextGfx) vm = Msg_Bar_Menu_ViewMode_TextGfx;
            else if (vm==MUIV_TheBar_ViewMode_Gfx) vm = Msg_Bar_Menu_ViewMode_Gfx;
                 else vm = Msg_Bar_Menu_ViewMode_Text;

            set((Object *)DoMethod(data->menu,MUIM_FindUData,vm),MUIA_Menuitem_Checked,TRUE);
        }

        set((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_Bar_Menu_Borderless),MUIA_Menuitem_Checked,prefs->Flags & SBFLG_Borderless);
        SetAttrs((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_Bar_Menu_Sunny),MUIA_Menuitem_Checked,prefs->Flags & SBFLG_Sunny,MUIA_Menuitem_Enabled,dm!=MUIV_TheBar_DisMode_Sunny,TAG_DONE);
        SetAttrs((Object *)DoMethod(data->menu,MUIM_FindUData,Msg_Bar_Menu_Raised),MUIA_Menuitem_Enabled,bl,MUIA_Menuitem_Checked,bl && (prefs->Flags & SBFLG_Raised),TAG_DONE);

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
initRootBar(void)
{
    if (lib_RootBar = MUI_CreateCustomClass(NULL,MUIC_TheBarVirt,NULL,sizeof(struct data),DISP(dispatcher)))
    {
    	localizeMenus(tnMenu,tnMenuIDs);
        localizeButtonsBar(buttons,buttonsIDs);

        if (!GenesisBase) buttons[9].img = MUIV_TheBar_End;

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
disposeRootBar(void)
{
    MUI_DeleteCustomClass(lib_RootBar);
    lib_RootBar = NULL;
}

/***********************************************************************/
