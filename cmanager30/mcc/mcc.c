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
#include <dos/dostags.h>
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/***************************************************************************/

struct data
{
    /* External objects */
    Object                         *App;
    Object                         *Win;

    /* Objects */
    Object                         *ButtonsBar;

    Object                         *SuperMainGroup;
    Object                         *MainGroup;

    /* Groups objects */
    Object                         *GroupsGroup;
    Object                         *GroupsListview;
    Object                         *GroupsList;
    Object                         *GroupsCtrl;
    Object                         *GroupName;
    Object                         *AddGroup;
    Object                         *RemGroup;
    Object                         *GroupUp;
    Object                         *GroupDown;

    Object                         *Balance1;
    Object                         *Balance2;

    /* Entries objects */
    Object                         *SuperEntriesGroup;
    Object                         *InnerEntriesGroup;
    Object                         *EntriesGroup;
    Object                         *Locate;
    Object                         *ChangesFlag;
    Object                         *ChangedFlag;
    Object                         *EntriesList;

    /* Quick objects */
    Object                         *QuickGroup;
    Object                         *QuickUserName;
    Object                         *QuickUserLastName;
    Object                         *QuickUserEMail;
    Object                         *QuickWWWName;
    Object                         *QuickWWWWWW;
    Object                         *QuickFTPName;
    Object                         *QuickFTPFTP;
    Object                         *QuickChatChannel;
    Object                         *QuickChatServer;
    Object                         *QuickTelnetHost;
    Object                         *QuickTelnetPort;
    Object                         *QuickTelnetLogin;
    Object                         *QuickTelnetPass;

    Object                         *Card;

    /* Window objects */
    Object                         *PrefsWin;
    Object                         *SearchWin;
    Object                         *LoginWin;
    Object                         *MiniMailWin;

    /* Data and prefs */
    struct CMData                  *CMData;
    struct DateStamp               HLDates;
    char                           User[64];
    char                           Path[256];
    char                           tUser[64];
    char                           tPath[256];

    /* Active values */
    struct MUI_NListtree_TreeNode  *LastActive;
    LONG                           LastType;
    LONG                           SortCol0;

    /* Preferences */
    struct CMPrefs                 Prefs;
    struct VisualPrefs             LastSavedVisualPrefs;

    /* Data changes notification */
    int                            NotiSig;
    struct MsgPort                 NotiPort;
    struct NotifyRequest           NotifyReq;
    struct MUI_InputHandlerNode    Handler;

    /* Doubleclick stuff */
    struct Hook                    *DoubleClickHook;
    Object                         *DoubleClickObject;

    /* Flags */
    ULONG                          Flags;
};

enum
{
    FLG_HideUsers       = 1<<0,
    FLG_HideWWW         = 1<<1,
    FLG_HideFTP         = 1<<2,
    FLG_HideChat        = 1<<3,
    FLG_HideTelnet      = 1<<4,

    FLG_Setup           = 1<<5,
    FLG_SettingUp       = 1<<6,
    FLG_DataSaved       = 1<<7,

    FLG_Notified        = 1<<8,
    FLG_GrabbedLists    = 1<<9,
    FLG_AtHome          = 1<<10,
    FLG_NoBar           = 1<<11,
    FLG_NoGroups        = 1<<12,
    FLG_NoMenu          = 1<<13,
    FLG_QuickDisabled   = 1<<14,
    FLG_Disposing       = 1<<15,
    FLG_ExternalChanged = 1<<16,
    FLG_Special         = 1<<17,
    FLG_Quit            = 1<<18,

    FLG_Rem             = 1<<19,

    FLG_NoQuick         = 1<<20,
    FLG_DisableQuick    = 1<<21,
    FLG_NoCard          = 1<<22,
    FLG_DisableCard     = 1<<23,
};

/***************************************************************************/

static ULONG
request(Object *obj,struct data *data,ULONG type,STRPTR text,STRPTR gads,...)
{
    REGARRAY char   buf[512], gbuf[512];
    register STRPTR s, c;
    register Object *win, *g, *o, *aobj, *o1, *o2, *osp;
    register ULONG  active, notify, res = FALSE;
    register int    i;
#ifdef __MORPHOS__
    va_list         va;

    va_start(va,gads);
    VNewRawDoFmt(text,0,buf,va);
    va_end(va);
#else
    vsprintf(buf,text,&gads+1);
#endif

    if (!(win = MWinObject,
        MUIA_BWin_Background,       MUII_RequesterBack,
        MUIA_Window_Title,          CM_GetString(Msg_Req_Title),
        MUIA_Window_NoMenus,        TRUE,
        MUIA_Window_RefWindow,      data->Win,
        MUIA_Window_CloseGadget,    FALSE,
        WindowContents, VGroup,
            MUIA_Background,        MUII_RequesterBack,
            MUIA_Group_VertSpacing, 8,
            Child, TextObject,
                TextFrame,
                InnerSpacing(8,8),
                MUIA_Background,    MUII_TextBack,
                MUIA_Text_PreParse, MUIX_C,
                MUIA_Text_Contents, buf,
            End,
            Child, g = HGroup, End,
        End,
    End)) goto end;

    strcpy(gbuf,gads);

    o = NULL; /* To make SAS-C happy */

    for (aobj = o1 = o2 = osp = NULL, active = notify = 0, i = 1, s = c = gbuf; *c; c++)
    {
        if (notify)
        {
            register Object *sp;

            if (sp = wspace(10))
            {
                DoMethod(g,OM_ADDMEMBER,sp);
                if (!osp) osp = sp;
            }

            DoMethod(o,MUIM_Notify,MUIA_Pressed,FALSE,data->App,7,MUIM_Application_PushMethod,obj,4,MUIM_CManager_RequesterResult,win,type,i);
            i++;
            notify = 0;
        }

        if (*c=='*')
        {
            active = 1;
            s = c+1;
            continue;
        }

        if (*c!='|') continue;

        *c = 0;

        if (!(o = MUI_MakeObject(MUIO_Button,s))) goto end;
        set(o,MUIA_CycleChain,TRUE);
        DoMethod(g,OM_ADDMEMBER,o);
        if (!o1) o1 = o;
        else if (!o2) o2 = o;
        s = c+1;

        if (active)
        {
            aobj = o;
            active = 0;
        }

        notify = 1;
    }

    if (s!=c)
    {
        if (!(o = MUI_MakeObject(MUIO_Button,s))) goto end;

        set(o,MUIA_CycleChain,TRUE);
        DoMethod(g,OM_ADDMEMBER,o);
        if (!o1) o1 = o;
        else if (!o2) o2 = o;
        if (active) aobj = o;

        notify = 1;
    }

    if (notify)
        DoMethod(o,MUIM_Notify,MUIA_Pressed,FALSE,data->App,7,MUIM_Application_PushMethod,obj,4,MUIM_CManager_RequesterResult,win,type,0);

    if (i<3)
        if (i==2)
        {
            register Object *sp1;

            if (sp1 = wspace(70))
            {
                DoMethod(g,OM_ADDMEMBER,sp1);
                DoMethod(g,MUIM_Group_Sort,o1,sp1,osp,o2,NULL);
            }
        }
        else
            if (i==1)
            {
                register Object *sp1;

                if (sp1 = wspace(100))
                {
                    register Object *sp2;

                    if (sp2 = wspace(100))
                    {
                        DoMethod(g,OM_ADDMEMBER,sp1);
                        DoMethod(g,OM_ADDMEMBER,sp2);
                        DoMethod(g,MUIM_Group_Sort,sp1,o1,sp2,NULL);
                    }
                    else MUI_DisposeObject(sp1);
                }
            }
            else goto end;

    DoMethod(win,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,data->App,7,MUIM_Application_PushMethod,obj,4,MUIM_CManager_RequesterResult,win,type,0);

    sleepApp(obj,TRUE);

    DoMethod(data->App,OM_ADDMEMBER,win);
    if (aobj) set(win,MUIA_Window_ActiveObject,aobj);

    if (!openWindow(data->App,win))
    {
        DoMethod(data->App,OM_REMMEMBER,win);
        goto end;
    }

    res = TRUE;

end:

    if (!res)
    {
        if (win) MUI_DisposeObject(win);
        DoMethod(data->App,MUIM_Application_PushMethod,obj,4,MUIM_CManager_RequesterResult,NULL,type,0);
    }

    return res;
}

/***************************************************************************/

static void
extractDateStamp(STRPTR File,struct DateStamp *ds,ULONG wait)
{
    register int i;

    memset(ds,0,sizeof(*ds));

    for (i = 8; i; i--)
    {
        register BPTR lock;

        if (lock = Lock(File,ACCESS_READ))
        {
            struct FileInfoBlock *fib;

            if (fib = AllocDosObject(DOS_FIB,NULL))
    	    {
                if (Examine(lock,fib))
    	        {
                    copymem(ds,&fib->fib_Date,sizeof(struct DateStamp));
            	    i = 0;
            	}

	        FreeDosObject(DOS_FIB,fib);
            }

            UnLock(lock);
        }

        if (i && wait) Delay(10);
        else break;
    }
}

/***************************************************************************/

static void
startDOSNotify(Object *obj,struct data *data)
{
    if (data->CMData && data->CMData->Path[0] && !(data->Flags & FLG_Notified) && (data->Flags & FLG_Setup))
    {
        extractDateStamp(data->CMData->Path,&data->HLDates,FALSE);

        data->Handler.ihn_Object  = obj;
        data->Handler.ihn_Signals = 1L<<data->NotiSig;
        data->Handler.ihn_Flags   = 0;
        data->Handler.ihn_Method  = MUIM_CManager_HandleNotify;
        DoMethod(data->App,MUIM_Application_AddInputHandler,&data->Handler);

        memset(&data->NotifyReq,0,sizeof(data->NotifyReq));
        data->NotifyReq.nr_Name                 = data->CMData->Path;
        data->NotifyReq.nr_Flags                = NRF_SEND_MESSAGE;
        data->NotifyReq.nr_stuff.nr_Msg.nr_Port = &data->NotiPort;
        StartNotify(&data->NotifyReq);

        data->Flags |= FLG_Notified;
    }
}

/***************************************************************************/

static void
stopDOSNotify(Object *obj,struct data *data)
{
    if (data->Flags & FLG_Notified)
    {
        EndNotify(&data->NotifyReq);
        DoMethod(data->App,MUIM_Application_RemInputHandler,&data->Handler);
        data->Flags &= ~FLG_Notified;
    }
}

/***************************************************************************/

static void
updateVisualPrefs(struct data *data)
{
    struct ListPrefs *lp;

    get(data->EntriesList,MUIA_EntryList_ListPrefs,&lp);
    copymem(&data->Prefs.VisualPrefs.ListPrefs,lp,sizeof(struct ListPrefs));

    data->Prefs.VisualPrefs.ListPrefs.TreeW = xget(data->GroupsGroup,MUIA_HorizWeight);
    data->Prefs.VisualPrefs.ListPrefs.ListW = xget(data->SuperEntriesGroup,MUIA_HorizWeight);

    if (!(data->Flags & FLG_NoCard))
    {
        data->Prefs.VisualPrefs.ListPrefs.ListCW = xget(data->InnerEntriesGroup,MUIA_VertWeight);
        data->Prefs.VisualPrefs.ListPrefs.CardCW = xget(data->Card,MUIA_VertWeight);
    }

    if (memcmp(&data->Prefs.VisualPrefs,&data->LastSavedVisualPrefs,sizeof(data->Prefs.VisualPrefs)))
    {
        copymem(&data->LastSavedVisualPrefs,&data->Prefs.VisualPrefs,sizeof(data->LastSavedVisualPrefs));
        loadPrefs(data->User,&data->Prefs);
        copymem(&data->Prefs.VisualPrefs,&data->LastSavedVisualPrefs,sizeof(data->Prefs.VisualPrefs));
        savePrefs(data->User,&data->Prefs);
    }
}

/***************************************************************************/

static void
setStatus(struct data *data)
{
    set(data->ChangesFlag,MUIA_Lamp_Off,data->Flags & FLG_DataSaved);
    set(data->ChangedFlag,MUIA_Lamp_Off,!(data->Flags & FLG_ExternalChanged));
    if (data->ButtonsBar) set((Object *)DoMethod(data->ButtonsBar,MUIM_TheBar_GetObject,B_SAVE),MUIA_Disabled,data->Flags & FLG_DataSaved);
}

/***************************************************************************/

static void
setBarPrefs(struct IClass *cl,Object *obj,struct data *data,ULONG obPos,ULONG nbPos)
{
    if (data->ButtonsBar)
    {
        if (!(data->Flags & FLG_NoBar))
        {
            if (data->Flags & FLG_Setup)
            {
                if ((nbPos & SBPos_TopMode) || (obPos & SBPos_TopMode)) DoSuperMethod(cl,obj,MUIM_Group_InitChange);
                if ((nbPos & SBPos_LeftMode)  || (obPos & SBPos_LeftMode)) DoMethod(data->SuperMainGroup,MUIM_Group_InitChange);
                if ((nbPos & SBPos_InnerMode) || (obPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,MUIM_Group_InitChange);
            }

            if (nbPos!=obPos)
            {
                if (obPos & SBPos_TopMode)
                {
                    if (!(nbPos & SBPos_TopMode)) DoSuperMethod(cl,obj,OM_REMMEMBER,data->ButtonsBar);
                }
                else
                    if (obPos & SBPos_LeftMode)
                    {
                        if (!(nbPos & SBPos_LeftMode)) DoMethod(data->SuperMainGroup,OM_REMMEMBER,data->ButtonsBar);
                    }
                    else
                        if (obPos & SBPos_InnerMode)
                        {
                            if (!(nbPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,OM_REMMEMBER,data->ButtonsBar);
                        }

                switch (nbPos)
                {
                    case SBPos_Top:
                        if (!(obPos & SBPos_TopMode)) DoSuperMethod(cl,obj,OM_ADDMEMBER,data->ButtonsBar);
                        DoSuperMethod(cl,obj,MUIM_Group_Sort,data->ButtonsBar,data->SuperMainGroup,NULL);
                        break;

                    case SBPos_Bottom:
                        if (!(obPos & SBPos_TopMode)) DoSuperMethod(cl,obj,OM_ADDMEMBER,data->ButtonsBar);
                        DoSuperMethod(cl,obj,MUIM_Group_Sort,data->SuperMainGroup,data->ButtonsBar,NULL);
                        break;

                    case SBPos_Left:
                        if (!(obPos & SBPos_LeftMode)) DoMethod(data->SuperMainGroup,OM_ADDMEMBER,data->ButtonsBar);
                        DoMethod(data->SuperMainGroup,MUIM_Group_Sort,data->ButtonsBar,data->MainGroup,NULL);
                        break;

                    case SBPos_Right:
                        if (!(obPos & SBPos_LeftMode)) DoMethod(data->SuperMainGroup,OM_ADDMEMBER,data->ButtonsBar);
                        DoMethod(data->SuperMainGroup,MUIM_Group_Sort,data->MainGroup,data->ButtonsBar,NULL);
                        break;

                    case SBPos_InnerTop:
                        if (!(obPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,OM_ADDMEMBER,data->ButtonsBar);
                        DoMethod(data->SuperEntriesGroup,MUIM_Group_Sort,data->ButtonsBar,data->EntriesGroup,NULL);
                        break;

                    case SBPos_InnerBottom:
                        if (!(obPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,OM_ADDMEMBER,data->ButtonsBar);
                        DoMethod(data->SuperEntriesGroup,MUIM_Group_Sort,data->EntriesGroup,data->ButtonsBar,NULL);
                        break;
                }
            }
        }

        SetAttrs(data->ButtonsBar,
            MUIA_Group_Forward,     FALSE,
            MUIA_Group_Horiz,       (nbPos & SBPos_Horiz) ? TRUE : FALSE,
            MUIA_TheBar_ViewMode,   data->Prefs.VisualPrefs.ButtonsBarPrefs.ViewMode,
            MUIA_TheBar_BarPos,     data->Prefs.VisualPrefs.ButtonsBarPrefs.ButtonsPos,
            MUIA_TheBar_LabelPos,   data->Prefs.VisualPrefs.ButtonsBarPrefs.LabelPosition,
            MUIA_TheBar_Borderless, (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Borderless) ? TRUE : FALSE,
            MUIA_TheBar_Sunny,      (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Sunny) ? TRUE : FALSE,
            MUIA_TheBar_Raised,     (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Raised) ? TRUE : FALSE,
            MUIA_TheBar_Scaled,     (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Scaled) ? TRUE : FALSE,
            MUIA_TheBar_EnableKeys, (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Underscore) ? TRUE : FALSE,
            MUIA_TheBar_BarSpacer,  (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_BarSpacer) ? TRUE : FALSE,
            MUIA_TheBar_Frame,      (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Frame) ? TRUE : FALSE,
            MUIA_TheBar_DragBar,    (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_DragBar) ? TRUE : FALSE,
            TAG_DONE);

        DoMethod(data->ButtonsBar,MUIM_TheBar_SetAttr,B_SORT,MUIV_TheBar_Attr_Hide,!(data->Prefs.Options & OPT_ManualSort));

        if ((data->Flags & FLG_Setup) && !(data->Flags & FLG_NoBar))
        {
            if ((nbPos & SBPos_InnerMode) || (obPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,MUIM_Group_ExitChange);
            if ((nbPos & SBPos_LeftMode) || (obPos & SBPos_LeftMode)) DoMethod(data->SuperMainGroup,MUIM_Group_ExitChange);
            if ((nbPos & SBPos_TopMode) || (obPos & SBPos_TopMode)) DoSuperMethod(cl,obj,MUIM_Group_ExitChange);
        }
    }
}

/***************************************************************************/

static ULONG
checkMCCs(struct data *data)
{
    return (ULONG)((xget(data->GroupsListview,MUIA_Version)>=19) &&
                   (xget(data->EntriesList,MUIA_Version)>=20) &&
                   (xget(data->GroupsList,MUIA_Version)>=18) &&
                   (xget(data->Locate,MUIA_Version)>=29));
}

/***************************************************************************/

struct cmPackTags
{
    struct Hook *dcHook;
    Object      *dcObj;
    ULONG       noquick;
    ULONG       dquick;
    ULONG       nocard;
    ULONG       dcard;
    ULONG       flags;
};

static ULONG cmTable[] =
{
    PACK_STARTTABLE(CMTAG(0)),

    PACK_ENTRY(CMTAG(0),MUIA_CManager_AppDoubleClick,cmPackTags,dcHook,PKCTRL_LONG|PKCTRL_PACKONLY),
    PACK_ENTRY(CMTAG(0),MUIA_CManager_DoubleClickObject,cmPackTags,dcObj,PKCTRL_LONG|PKCTRL_PACKONLY),

    PACK_ENTRY(CMTAG(0),MUIA_CManager_NoQuick,cmPackTags,noquick,PKCTRL_LONG|PKCTRL_PACKONLY),
    PACK_ENTRY(CMTAG(0),MUIA_CManager_DisableQuick,cmPackTags,dquick,PKCTRL_LONG|PKCTRL_PACKONLY),
    PACK_ENTRY(CMTAG(0),MUIA_CManager_NoCard,cmPackTags,nocard,PKCTRL_LONG|PKCTRL_PACKONLY),
    PACK_ENTRY(CMTAG(0),MUIA_CManager_DisableCard,cmPackTags,dcard,PKCTRL_LONG|PKCTRL_PACKONLY),

    PACK_LONGBIT(CMTAG(0),MUIA_CManager_AtHome,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_AtHome),
    PACK_LONGBIT(CMTAG(0),MUIA_CManager_NoBar,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_NoBar),
    PACK_LONGBIT(CMTAG(0),MUIA_CManager_NoGroupsCtrl,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_NoGroups),
    PACK_LONGBIT(CMTAG(0),MUIA_CManager_NoMenu,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_NoMenu),
    PACK_LONGBIT(CMTAG(0),MUIA_CManager_HideUsers,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_HideUsers),
    PACK_LONGBIT(CMTAG(0),MUIA_CManager_HideWWW,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_HideWWW),
    PACK_LONGBIT(CMTAG(0),MUIA_CManager_HideFTP,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_HideFTP),
    PACK_LONGBIT(CMTAG(0),MUIA_CManager_HideChat,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_HideChat),
    PACK_LONGBIT(CMTAG(0),MUIA_CManager_HideTelnet,cmPackTags,flags,PKCTRL_BIT|PKCTRL_PACKONLY,FLG_HideTelnet),

    PACK_ENDTABLE
};

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data             temp;
    struct cmPackTags       pt;
    register Object         *Entries;
    register struct TagItem *attrs = msg->ops_AttrList;

    memset(&temp,0,sizeof(temp));

    if ((temp.NotiSig = AllocSignal(-1))<0) return 0;

    memset(&pt,0,sizeof(pt));
    pt.noquick = pt.nocard = TRUE;
    PackStructureTags(&pt,cmTable,attrs);

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Group_VertSpacing, 2,

            ((pt.flags & FLG_NoBar) ? TAG_IGNORE : Child), ((pt.flags & FLG_NoBar) ? 0 : (temp.ButtonsBar = RootBarObject, End)),

            Child, temp.SuperMainGroup = HGroup,
                MUIA_Group_Spacing, 2,

                Child, temp.MainGroup = HGroup,
                    MUIA_Group_Spacing, 2,

                    Child, temp.GroupsGroup = VGroup,
                        MUIA_HelpNode,    "CMGROUPS",
                        MUIA_HorizWeight, DEF_TreeW,

                        Child, temp.GroupsListview = NListviewObject,
                            MUIA_CycleChain,      TRUE,
                            MUIA_NListview_NList, temp.GroupsList = GroupListObject, End,
                        End,

                        Child, temp.GroupsCtrl = VGroup,
                            MUIA_ShowMe,            !(pt.flags & FLG_NoGroups),
                            MUIA_Group_VertSpacing, 1,

                            Child, ColGroup(2),
                                MUIA_Group_Spacing, 1,

                                Child, temp.GroupName = ostring(80,0,Msg_Gad_GroupNameHelp),
                                Child, temp.GroupUp = oibutton(IBT_Up,Msg_Gad_GroupUpHelp),

                                Child, HGroup,
                                    MUIA_Group_HorizSpacing,  1,
                                    MUIA_Group_SameSize,      TRUE,
                                    Child, temp.AddGroup = obutton(Msg_Gad_AddGroup,Msg_Gad_AddGroupHelp),
                                    Child, temp.RemGroup = obutton(Msg_Gad_RemoveGroup,Msg_Gad_RemoveGroupHelp),
                                End,
                                Child, temp.GroupDown = oibutton(IBT_Down,Msg_Gad_GroupDownHelp),
                            End,
                        End,
                    End,

                    Child, temp.Balance1 = BalanceObject, End,

                    Child, temp.SuperEntriesGroup = VGroup,
                        MUIA_HorizWeight,   DEF_ListW,
                        MUIA_Group_Spacing, 2,

                        Child, temp.EntriesGroup = VGroup,
                            MUIA_HelpNode, "CMLIST",

                            Child, temp.InnerEntriesGroup = VGroup,
                                MUIA_VertWeight, DEF_ListCW,
                                (lib_flags & BASEFLG_NoEntriesFrame) ? TAG_IGNORE : MUIA_Frame, MUIV_Frame_Group,

                                Child, HGroup,
                                    Child, olabel2(Msg_Gad_Locate),
                                    Child, temp.Locate = oqstring(NULL,80,Msg_Gad_Locate,Msg_Gad_LocateHelp),

                                    Child, wspace(20),

                                    Child, Entries = TextObject,
                                        MUIA_Text_PreParse, MUIX_R,
                                    End,

                                    Child, RectangleObject, MUIA_FixWidthTxt, "-", End,

                                    Child, HGroup,
                                        MUIA_Group_Spacing, 2,
                                        Child, temp.ChangesFlag = LampObject, End,
                                        Child, temp.ChangedFlag = LampObject, End,
                                    End,
                                End,

                                Child, NListviewObject,
                                    MUIA_CycleChain,      TRUE,
                                    MUIA_NListview_NList, temp.EntriesList = EntryListObject,
                                        MUIA_CManager_AtHome,  (pt.flags & FLG_AtHome),
                                        MUIA_EntryList_NoMenu, (pt.flags & FLG_NoMenu),
                                    End,
                                End,
                            End,
                        End,
                    End,
                End,
            End,
            TAG_MORE,attrs))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->Flags = pt.flags|FLG_DataSaved;

        if (!checkMCCs(data))
        {
            CoerceMethod(cl->cl_Super,obj,OM_DISPOSE);
            return 0;
        }

        INITPORT(&data->NotiPort,data->NotiSig);

        data->DoubleClickHook   = pt.dcHook;
        data->DoubleClickObject = pt.dcObj;

        data->LastActive = MUIV_NListtree_Active_Off;
        data->LastType   = -1;

        setDefaultPrefs(&data->Prefs);
        if (data->Flags & FLG_NoBar) data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags |= SBFLG_Hide;
        if (data->Flags & FLG_NoQuick) data->Prefs.VisualPrefs.Flags |= VPFLG_NoQuick;
        if (data->Flags & FLG_NoCard) data->Prefs.VisualPrefs.Flags |= VPFLG_NoCard;
        copymem(&data->LastSavedVisualPrefs,&data->Prefs.VisualPrefs,sizeof(&data->LastSavedVisualPrefs));

        set(temp.GroupName,MUIA_Textinput_AttachedList,data->GroupsList);
        set(temp.QuickTelnetPort,MUIA_Weight,30);

        DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_CManager_CMObj,obj,data->GroupsList,data->EntriesList,data->ButtonsBar,NULL);
        DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_CManager_Prefs,&data->Prefs,data->GroupsList,data->EntriesList,data->Card,NULL);

        SetAttrs(temp.Locate,MUIA_QString_AttachedList,   data->EntriesList,
                             MUIA_Textinput_RemainActive, FALSE,
                             MUIA_Textinput_AdvanceOnCR,  TRUE,
                             TAG_DONE);

        DoMethod(Entries,MUIM_SetAsString,MUIA_Text_Contents,CM_GetString(Msg_Entries),0);

        if (data->ButtonsBar)
        {
            DoMethod(_sbbut(data->ButtonsBar,B_ADD),MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_CManager_AddItem);
            DoMethod(_sbbut(data->ButtonsBar,B_CLONE),MUIM_Notify,MUIA_Pressed,FALSE,data->EntriesList,1,MUIM_EntryList_Clone);
            DoMethod(_sbbut(data->ButtonsBar,B_EDIT),MUIM_Notify,MUIA_Pressed,FALSE,data->EntriesList,1,MUIM_EntryList_Edit);
            DoMethod(_sbbut(data->ButtonsBar,B_REMOVE),MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_CManager_RemSelected);
            DoMethod(_sbbut(data->ButtonsBar,B_SEARCH),MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_CManager_Search);
            DoMethod(_sbbut(data->ButtonsBar,B_SORT),MUIM_Notify,MUIA_Pressed,FALSE,data->EntriesList,1,MUIM_EntryList_Sort);
            DoMethod(_sbbut(data->ButtonsBar,B_SAVE),MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_SaveData,NULL);
            DoMethod(_sbbut(data->ButtonsBar,B_PREFS),MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_PrefsWindow,MUIV_PrefsWindow_ActivePage_NoPage);
            if (GenesisBase) DoMethod(_sbbut(data->ButtonsBar,B_LOGIN),MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_CManager_LoginWindow);
        }

        DoMethod(temp.GroupsList,MUIM_Notify,MUIA_NListtree_Active,MUIV_EveryTime,obj,2,MUIM_CManager_SetGroup,MUIV_TriggerValue);
        DoMethod(temp.GroupName,MUIM_Notify,MUIA_Textinput_Contents,MUIV_EveryTime,obj,1,MUIM_CManager_GetGroup);
        DoMethod(temp.GroupUp,MUIM_Notify,MUIA_Timer,MUIV_EveryTime,data->GroupsList,2,MUIM_GroupList_Arrow,TRUE);
        DoMethod(temp.GroupDown,MUIM_Notify,MUIA_Timer,MUIV_EveryTime,data->GroupsList,2,MUIM_GroupList_Arrow,FALSE);
        DoMethod(temp.AddGroup,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_AddGroup,NULL);
        DoMethod(temp.RemGroup,MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_CManager_RemGroup);

        DoMethod(temp.EntriesList,MUIM_Notify,MUIA_NList_DoubleClick,MUIV_EveryTime,obj,1,MUIM_CManager_OnDoubleClick);
        DoMethod(temp.EntriesList,MUIM_Notify,MUIA_NList_Active,MUIV_EveryTime,obj,1,MUIM_CManager_ChangeQuick);
        DoMethod(temp.EntriesList,MUIM_Notify,MUIA_NList_Entries,MUIV_EveryTime,Entries,4,MUIM_SetAsString,MUIA_Text_Contents,CM_GetString(Msg_Entries),MUIV_TriggerValue);
        DoMethod(temp.EntriesList,MUIM_Notify,MUIA_NList_TitleMark,MUIV_EveryTime,obj,3,MUIM_Set,MUIA_CManager_SortCol0,MUIV_TriggerValue);

        DoMethod(temp.Locate,MUIM_Notify,MUIA_Textinput_Contents,MUIV_EveryTime,temp.EntriesList,2,MUIM_EntryList_Locate,MUIV_TriggerValue);

        if (pt.dquick)
        {
            data->Flags |= FLG_DisableQuick|FLG_NoQuick;
        }
        else
        {
            if (pt.noquick) data->Flags |= FLG_NoQuick;
        }

        if (pt.dcard)
        {
            data->Flags |= FLG_DisableCard|FLG_NoCard;
        }
        else
        {
            if (pt.nocard) data->Flags |= FLG_NoCard;
        }

        if (!(data->Flags & FLG_NoQuick) || !(data->Flags & FLG_NoCard))
        {
            SetAttrs(obj,(data->Flags & FLG_NoQuick) ? TAG_IGNORE : MUIA_CManager_NoQuick, FALSE,
                         (data->Flags & FLG_NoCard)  ? TAG_IGNORE : MUIA_CManager_NoCard,  FALSE,
                         TAG_DONE);
        }
    }
    else FreeSignal(temp.NotiSig);

    return (ULONG)obj;
}
/***************************************************************************/

static
ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    data->Flags |= FLG_DataSaved|FLG_Disposing;
    DoMethod(obj,MUIM_CManager_FreeData);

    if (data->NotiSig>=0) FreeSignal(data->NotiSig);

    if ((data->Flags & FLG_NoBar) && data->ButtonsBar)
        MUI_DisposeObject(data->ButtonsBar);

    updateVisualPrefs(data);

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***************************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_CManager_Changed:         *msg->opg_Storage = (data->Flags & FLG_DataSaved) ? FALSE : TRUE; return TRUE;
        case MUIA_CManager_Prefs:           *msg->opg_Storage = (ULONG)&data->Prefs; return TRUE;
        case MUIA_CManager_ButtonsBarPrefs: *msg->opg_Storage = (ULONG)&data->Prefs.VisualPrefs.ButtonsBarPrefs; return TRUE;
        case MUIA_CManager_CMData:          *msg->opg_Storage = (ULONG)data->CMData; return TRUE;
        case MUIA_CManager_ListObject:      *msg->opg_Storage = (ULONG)data->EntriesList; return TRUE;
        case MUIA_CManager_TreeObject:      *msg->opg_Storage = (ULONG)data->GroupsList; return TRUE;
        case MUIA_CManager_Path:            *msg->opg_Storage = (ULONG)lib_CMPath; return TRUE;
        case MUIA_CManager_Version:         *msg->opg_Storage = (ULONG)lib_fullName; return TRUE;
        case MUIA_CManager_ButtonsBar:      *msg->opg_Storage = (ULONG)data->ButtonsBar; return TRUE;
        case MUIA_CManager_NoMenu:          *msg->opg_Storage = (data->Flags & FLG_NoMenu) ? TRUE : FALSE; return TRUE;
        case MUIA_CManager_NoBar:           *msg->opg_Storage = (data->Flags & FLG_NoBar) ? TRUE : FALSE; return TRUE;
        case MUIA_CManager_NoQuick:         *msg->opg_Storage = (data->Flags & FLG_NoQuick) ? TRUE : FALSE; return TRUE;
        case MUIA_CManager_NoCard:          *msg->opg_Storage = (data->Flags & FLG_NoCard) ? TRUE : FALSE; return TRUE;
        case MUIA_CManager_NoGroupsCtrl:    *msg->opg_Storage = (data->Flags & FLG_NoGroups) ? TRUE : FALSE; return TRUE;
        case MUIA_CManager_Quit:            *msg->opg_Storage = (data->Flags & FLG_Quit) ? TRUE : FALSE; return TRUE;

        case MUIA_Version:                  *msg->opg_Storage = lib_version; return TRUE;
        case MUIA_Revision:                 *msg->opg_Storage = lib_revision; return TRUE;

        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***************************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    struct TagItem          *tags, *nmt = NULL, *nbt = NULL, *ngt = NULL, *nqt = NULL, *nct = NULL;
    register struct TagItem *tag;
    register ULONG          res;

    for (tags = msg->ops_AttrList; tag = NextTagItem(&tags); )
    {
        register ULONG tidata = tag->ti_Data;

        switch (tag->ti_Tag)
        {
            case MUIA_CManager_Changed:
                if (tidata) data->Flags &= ~FLG_DataSaved;
                else data->Flags &= ~FLG_ExternalChanged;
                setStatus(data);
                break;

            case MUIA_CManager_SortCol0:
                data->SortCol0 = tidata;
                break;

            case MUIA_CManager_AppDoubleClick:
                data->DoubleClickHook = (struct Hook *)tidata;
                break;

            case MUIA_CManager_DoubleClickObject:
                data->DoubleClickObject = (Object *)tidata;
                break;

            case MUIA_CManager_NoMenu:
                if (BOOLSAME(data->Flags & FLG_NoMenu,tidata))
                {
                    tag->ti_Tag = TAG_IGNORE;
                    nmt = tag;
                }
                else
                {
                    if (tidata) data->Flags |= FLG_NoMenu;
                    else data->Flags &= ~FLG_NoMenu;

                    set(data->EntriesList,MUIA_EntryList_NoMenu,tidata);
                }
                break;

            case MUIA_CManager_NoBar:
                if (!data->ButtonsBar || BOOLSAME(data->Flags & FLG_NoBar,tidata))
                {
                    tag->ti_Tag = TAG_IGNORE;
                    nbt = tag;
                }
                else
                {
                    register ULONG bPos = data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition;

                    if (data->Flags & FLG_Setup)
                    {
                        if (bPos & SBPos_TopMode) DoSuperMethod(cl,obj,MUIM_Group_InitChange);
                        else if (bPos & SBPos_LeftMode) DoMethod(data->SuperMainGroup,MUIM_Group_InitChange);
                             else if (bPos & SBPos_InnerMode) DoMethod(data->SuperEntriesGroup,MUIM_Group_InitChange);

                        sleepApp(obj,TRUE);
                    }

                    if (tidata)
                    {
                        if (bPos & SBPos_TopMode) DoSuperMethod(cl,obj,OM_REMMEMBER,data->ButtonsBar);
                        else if (bPos & SBPos_LeftMode) DoMethod(data->SuperMainGroup,OM_REMMEMBER,data->ButtonsBar);
                             else if (bPos & SBPos_InnerMode) DoMethod(data->SuperEntriesGroup,OM_REMMEMBER,data->ButtonsBar);

                        data->Flags |= FLG_NoBar;
                        data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags |= SBFLG_Hide;
                    }
                    else
                    {
                        switch (bPos)
                        {
                            case SBPos_Top:
                                DoSuperMethod(cl,obj,OM_ADDMEMBER,data->ButtonsBar);
                                DoSuperMethod(cl,obj,MUIM_Group_Sort,data->ButtonsBar,data->SuperMainGroup,NULL);
                                break;

                            case SBPos_Bottom:
                                DoSuperMethod(cl,obj,OM_ADDMEMBER,data->ButtonsBar);
                                DoSuperMethod(cl,obj,MUIM_Group_Sort,data->SuperMainGroup,data->ButtonsBar,NULL);
                                break;

                            case SBPos_Left:
                                DoMethod(data->SuperMainGroup,OM_ADDMEMBER,data->ButtonsBar);
                                DoMethod(data->SuperMainGroup,MUIM_Group_Sort,data->ButtonsBar,data->MainGroup,NULL);
                                break;

                            case SBPos_Right:
                                DoMethod(data->SuperMainGroup,OM_ADDMEMBER,data->ButtonsBar);
                                DoMethod(data->SuperMainGroup,MUIM_Group_Sort,data->MainGroup,data->ButtonsBar,NULL);
                                break;

                            case SBPos_InnerTop:
                                DoMethod(data->SuperEntriesGroup,OM_ADDMEMBER,data->ButtonsBar);
                                DoMethod(data->SuperEntriesGroup,MUIM_Group_Sort,data->ButtonsBar,data->EntriesGroup,NULL);
                                break;

                            case SBPos_InnerBottom:
                                DoMethod(data->SuperEntriesGroup,OM_ADDMEMBER,data->ButtonsBar);
                                DoMethod(data->SuperEntriesGroup,MUIM_Group_Sort,data->EntriesGroup,data->ButtonsBar,NULL);
                                break;
                        }

                        data->Flags &= ~FLG_NoBar;
                        data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags &= ~SBFLG_Hide;
                    }

                    if (data->Flags & FLG_Setup)
                    {
                        if (bPos & SBPos_TopMode) DoSuperMethod(cl,obj,MUIM_Group_ExitChange);
                        else if (bPos & SBPos_LeftMode) DoMethod(data->SuperMainGroup,MUIM_Group_ExitChange);
                             else if (bPos & SBPos_InnerMode) DoMethod(data->SuperEntriesGroup,MUIM_Group_ExitChange);

                        sleepApp(obj,FALSE);
                    }
                }
                break;

            case MUIA_CManager_NoGroupsCtrl:
                if (BOOLSAME(data->Flags & FLG_NoGroups,tidata))
                {
                    tag->ti_Tag = TAG_IGNORE;
                    ngt = tag;
                }
                else
                {
                    if (tidata) data->Flags |= FLG_NoGroups;
                    else data->Flags &= ~FLG_NoGroups;

                    set(data->GroupsCtrl,MUIA_ShowMe,!tidata);
                }
                break;

	        case MUIA_CManager_NoQuick:
                if ((data->Flags & FLG_DisableQuick) || BOOLSAME(data->Flags & FLG_NoQuick,tidata))
                {
                    tag->ti_Tag = TAG_IGNORE;
                    nqt = tag;
                }
        		else
                {
                    if (tidata)
                    {
                        DoMethod(data->InnerEntriesGroup,MUIM_Group_InitChange);
                        DoMethod(data->InnerEntriesGroup,OM_REMMEMBER,data->QuickGroup);
                        DoMethod(data->InnerEntriesGroup,MUIM_Group_ExitChange);
                        MUI_DisposeObject(data->QuickGroup);

                        data->Flags |= FLG_NoQuick;
                        data->Prefs.VisualPrefs.Flags |= VPFLG_NoQuick;
                    }
                    else
                    {
                        data->QuickGroup = VGroup,
                            MUIA_Group_PageMode, TRUE,

                            Child, ColGroup(2),
                                Child, olabel2(Msg_Quick_UserFirst),
                                Child, HGroup,
                                    Child, data->QuickUserName = oqstring(data->EntriesList,80,Msg_Quick_UserFirst,0),
                                    Child, olabel2(Msg_Quick_UserLast),
                                    Child, data->QuickUserLastName = oqstring(data->EntriesList,80,Msg_Quick_UserLast,0),
                                End,
                                Child, olabel2(Msg_Quick_UserEMail),
                                Child, data->QuickUserEMail = oqstring(data->EntriesList,128,Msg_Quick_UserEMail,0),
                            End,

                            Child, ColGroup(2),
                                Child, olabel2(Msg_Quick_WWWName),
                                Child, data->QuickWWWName = oqstring(data->EntriesList,60,Msg_Quick_WWWName,0),
                                Child, olabel2(Msg_Quick_WWWHost),
                                Child, data->QuickWWWWWW = oqstring(data->EntriesList,256,Msg_Quick_WWWHost,0),
                            End,

                            Child, ColGroup(2),
                                Child, olabel2(Msg_Quick_FTPName),
                                Child, data->QuickFTPName = oqstring(data->EntriesList,80,Msg_Quick_FTPName,0),
                                Child, olabel2(Msg_Quick_FTPHost),
                                Child, data->QuickFTPFTP = oqstring(data->EntriesList,256,Msg_Quick_FTPHost,0),
                            End,

                            Child, ColGroup(2),
                                Child, olabel2(Msg_Quick_ChatChannel),
                                Child, data->QuickChatChannel = oqstring(data->EntriesList,128,Msg_Quick_ChatChannel,0),
                                Child, olabel2(Msg_Quick_ChatServer),
                                Child, data->QuickChatServer = oqstring(data->EntriesList,128,Msg_Quick_ChatServer,0),
                            End,

                            Child, ColGroup(2),
                                Child, olabel2(Msg_Quick_TelnetHost),
                                Child, HGroup,
                                    Child, data->QuickTelnetHost = oqstring(data->EntriesList,128,Msg_Quick_TelnetHost,0),
                                    Child, olabel2(Msg_Quick_TelnetPort),
                                    Child, data->QuickTelnetPort = oqinteger(data->EntriesList,12,Msg_Quick_TelnetPort,0),
                                End,
                                Child, olabel2(Msg_Quick_TelnetLogin),
                                Child, HGroup,
                                    Child, data->QuickTelnetLogin = oqstring(data->EntriesList,128,Msg_Quick_TelnetLogin,0),
                                    Child, olabel2(Msg_Quick_TelnetPass),
                                    Child, data->QuickTelnetPass = oqsstring(data->EntriesList,60,Msg_Quick_TelnetPass,0),
                                End,
                            End,
                        End;

                        if (data->QuickGroup)
                        {
                            DoMethod(data->InnerEntriesGroup,MUIM_Group_InitChange);
                            DoMethod(data->InnerEntriesGroup,OM_ADDMEMBER,data->QuickGroup);
                            DoMethod(data->InnerEntriesGroup,MUIM_Group_ExitChange);

                            DoMethod(data->QuickUserName,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickUserLastName,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickUserEMail,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickWWWName,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickWWWWWW,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickFTPName,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickFTPFTP,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickChatChannel,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickChatServer,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickTelnetHost,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickTelnetPort,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickTelnetLogin,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);
                            DoMethod(data->QuickTelnetPass,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,1,MUIM_CManager_QuickChange);

                            data->Flags &= ~FLG_NoQuick;
                            data->Prefs.VisualPrefs.Flags &= ~VPFLG_NoQuick;

                            DoMethod(obj,MUIM_CManager_ChangeQuick);
                        }
                        else
                        {
                            tag->ti_Tag = TAG_IGNORE;
                            nqt = tag;
                        }
                    }
                }
                break;

            case MUIA_CManager_NoCard:
                if ((data->Flags & FLG_DisableCard) || BOOLSAME(data->Flags & FLG_NoCard,tidata))
                {
                    tag->ti_Tag = TAG_IGNORE;
                    nct = tag;
                }
                else
                {
                    if (tidata)
                    {
                        DoMethod(data->EntriesGroup,MUIM_Group_InitChange);

                        DoMethod(data->EntriesGroup,OM_REMMEMBER,data->Card);
                        MUI_DisposeObject(data->Card);

                        DoMethod(data->EntriesGroup,OM_REMMEMBER,data->Balance2);
                        MUI_DisposeObject(data->Balance2);

                        DoMethod(data->EntriesGroup,MUIM_Group_ExitChange);

                        data->Flags |= FLG_NoCard;
                        data->Prefs.VisualPrefs.Flags |= VPFLG_NoCard;
                        data->Card = NULL;
                    }
                    else
                    {
                        data->Balance2 = BalanceObject, End;
                        if (data->Balance2)
                        {
                            data->Card = CardObject, MUIA_CManager_CMObj, obj, End;
                            if (data->Card)
                            {
                                set(data->Card,MUIA_CManager_Prefs,&data->Prefs);
                                DoMethod(data->EntriesGroup,MUIM_Group_InitChange);

                                DoMethod(data->EntriesGroup,OM_ADDMEMBER,data->Balance2);
                                DoMethod(data->EntriesGroup,OM_ADDMEMBER,data->Card);
                                DoMethod(data->EntriesGroup,MUIM_Group_Sort,data->InnerEntriesGroup,
                                                                            data->Balance2,
                                                                            data->Card,
                                                                            NULL);

                                set(data->Card,MUIA_Weight,data->Prefs.VisualPrefs.ListPrefs.CardCW);
                                set(data->InnerEntriesGroup,MUIA_Weight,data->Prefs.VisualPrefs.ListPrefs.ListCW);

                                DoMethod(data->EntriesGroup,MUIM_Group_ExitChange);

                                data->Flags &= ~FLG_NoCard;
                                data->Prefs.VisualPrefs.Flags &= ~VPFLG_NoCard;

                                DoMethod(obj,MUIM_CManager_ChangeQuick);
                            }
                            else
                            {
                                MUI_DisposeObject(data->Balance2);
                                tag->ti_Tag = TAG_IGNORE;
                                nct = tag;
                            }
                        }
                    }
                }
                break;
        }
    }

    res = DoSuperMethodA(cl,obj,(Msg)msg);

    if (nmt) nmt->ti_Tag = MUIA_CManager_NoMenu;
    if (nbt) nbt->ti_Tag = MUIA_CManager_NoBar;
    if (ngt) ngt->ti_Tag = MUIA_CManager_NoGroupsCtrl;
    if (nqt) nqt->ti_Tag = MUIA_CManager_NoQuick;
    if (nct) nct->ti_Tag = MUIA_CManager_NoCard;

    return res;
}

/***************************************************************************/

static ULONG
mSetup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    data->App = _app(obj);
    data->Win = _win(obj);

    if (!(DoSuperMethodA(cl,obj,msg)))
    {
        stopDOSNotify(obj,data);
        data->App = data->Win = NULL;

        return FALSE;
    }

    data->Flags |= FLG_SettingUp|FLG_Setup;

    if (data->CMData && !(data->Flags & FLG_Notified))
        startDOSNotify(obj,data);

    data->Flags &= ~FLG_SettingUp;

    /* NList.mcc bug */
    set(data->EntriesList,MUIA_NList_First,MUIV_NList_First_Top);
    set(data->Win,MUIA_Window_DefaultObject,data->EntriesList);

    return TRUE;
}

/***************************************************************************/

static ULONG
mCleanup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    stopDOSNotify(obj,data);

    //if (!(data->Flags & FLG_AtHome))
        DoMethod(obj,MUIM_CManager_Cleanup,FALSE);

    data->App = data->Win = NULL;

    data->Flags &= ~FLG_Setup;

    return DoSuperMethodA(cl,obj,msg);
}

/***************************************************************************/

static ULONG
mChangeQuick(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMEntry       *entry;

    DoMethod(data->EntriesList,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
    if (!(data->Flags & FLG_NoQuick))
    {
        if (entry)
        {
            switch (entry->Type)
            {
                case CMEntry_User:
                    set(data->QuickUserName,MUIA_Textinput_Contents,CMUSER(entry)->Name);
                    set(data->QuickUserLastName,MUIA_Textinput_Contents,CMUSER(entry)->LastName);
                    set(data->QuickUserEMail,MUIA_Textinput_Contents,CMUSER(entry)->EMail);
                    break;

                case CMEntry_WWW:
                    set(data->QuickWWWName,MUIA_Textinput_Contents,CMWWW(entry)->Name);
                    set(data->QuickWWWWWW,MUIA_Textinput_Contents,CMWWW(entry)->WWW);
                    break;

                case CMEntry_FTP:
                    set(data->QuickFTPName,MUIA_Textinput_Contents,CMFTP(entry)->Name);
                    set(data->QuickFTPFTP,MUIA_Textinput_Contents,CMFTP(entry)->FTP);
                    break;

                case CMEntry_Chat:
                    set(data->QuickChatChannel,MUIA_Textinput_Contents,CMCHAT(entry)->Channel);
                    set(data->QuickChatServer,MUIA_Textinput_Contents,CMCHAT(entry)->Server);
                    break;

                case CMEntry_Telnet:
                    set(data->QuickTelnetHost,MUIA_Textinput_Contents,CMTELNET(entry)->Host);
                    set(data->QuickTelnetPort,MUIA_Textinput_Integer,CMTELNET(entry)->Port);
                    set(data->QuickTelnetLogin,MUIA_Textinput_Contents,CMTELNET(entry)->Login);
                    set(data->QuickTelnetPass,MUIA_Textinput_Contents,CMTELNET(entry)->Password);
                    break;
            }
        }
        else
        {
            ULONG a;

            get(data->QuickGroup,MUIA_Group_ActivePage,&a);

            switch (a)
            {
                case IDX_User:
                    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Textinput_Contents,NULL,
                        data->QuickUserName,
                        data->QuickUserLastName,
                        data->QuickUserEMail,
                        NULL);
                    break;

                case IDX_WWW:
                    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Textinput_Contents,NULL,
                        data->QuickWWWName,
                        data->QuickWWWWWW,
                        NULL);
                    break;

                case IDX_FTP:
                    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Textinput_Contents,NULL,
                        data->QuickFTPName,
                        data->QuickFTPFTP,
                        NULL);
                    break;

                case IDX_Chat:
                    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Textinput_Contents,NULL,
                        data->QuickChatChannel,
                        data->QuickChatServer,
                        NULL);
                    break;

                case IDX_Telnet:
                    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Textinput_Contents,NULL,
                        data->QuickTelnetHost,
                        data->QuickTelnetPort,
                        data->QuickTelnetLogin,
                        data->QuickTelnetPass,
                        NULL);
                    break;
            }
        }

        if (!BOOLSAME(!entry,data->Flags & FLG_QuickDisabled))
        {
            DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Disabled,!entry,
                data->QuickUserName,
                data->QuickUserLastName,
                data->QuickUserEMail,
                data->QuickWWWName,
                data->QuickWWWWWW,
                data->QuickFTPName,
                data->QuickFTPFTP,
                data->QuickChatChannel,
                data->QuickChatServer,
                data->QuickTelnetHost,
                data->QuickTelnetPort,
                data->QuickTelnetLogin,
                data->QuickTelnetPass,
                NULL);
        }
    }

    if (!(data->Flags & FLG_NoCard))
        set(data->Card,MUIA_Card_Entry,entry);

    if (!BOOLSAME(!entry,data->Flags & FLG_QuickDisabled))
    {
        if (data->ButtonsBar)
            DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Disabled,!entry,
                DoMethod(data->ButtonsBar,MUIM_TheBar_GetObject,B_CLONE),
                DoMethod(data->ButtonsBar,MUIM_TheBar_GetObject,B_EDIT),
                DoMethod(data->ButtonsBar,MUIM_TheBar_GetObject,B_REMOVE),
                NULL);

        if (entry) data->Flags &= ~FLG_QuickDisabled;
        else data->Flags |= FLG_QuickDisabled;
    }

    return 0;
}

/***************************************************************************/

static ULONG
mQuickChange(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMEntry       *entry;

    DoMethod(data->EntriesList,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
    if (entry)
    {
        switch (entry->Type)
        {
            case CMEntry_User:
                getstr(data->QuickUserName,CMUSER(entry)->Name);
                getstr(data->QuickUserLastName,CMUSER(entry)->LastName);
                getstr(data->QuickUserEMail,CMUSER(entry)->EMail);
                break;

            case CMEntry_WWW:
                getstr(data->QuickWWWName,CMWWW(entry)->Name);
                getstr(data->QuickWWWWWW,CMWWW(entry)->WWW);
                break;

            case CMEntry_FTP:
                getstr(data->QuickFTPName,CMFTP(entry)->Name);
                getstr(data->QuickFTPFTP,CMFTP(entry)->FTP);
                break;

            case CMEntry_Chat:
                getstr(data->QuickChatChannel,CMCHAT(entry)->Channel);
                getstr(data->QuickChatServer,CMCHAT(entry)->Server);
                break;

            case CMEntry_Telnet:
                getstr(data->QuickTelnetHost,CMTELNET(entry)->Host);
                CMTELNET(entry)->Port = xget(data->QuickTelnetPort,MUIA_Textinput_Integer);
                getstr(data->QuickTelnetLogin,CMTELNET(entry)->Login);
                getstr(data->QuickTelnetPass,CMTELNET(entry)->Password);
                break;
        }

        DoMethod(data->EntriesList,MUIM_NList_Sort);
        DoMethod(data->EntriesList,MUIM_NList_Redraw,MUIV_NList_Redraw_Active);

        set(obj,MUIA_CManager_Changed,TRUE);
    }

    return 0;
}

/***************************************************************************/

static ULONG
mHandleNotify(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct Message *nmsg;
    register int            res = 0;

    while (nmsg = GetMsg(&data->NotiPort))
    {
        res++;
        ReplyMsg(nmsg);
    }

    if (res)
    {
        if (!(data->Prefs.Options & OPT_NtCheckChanges))
        {
            struct DateStamp ds;

            extractDateStamp(data->CMData->Path,&ds,TRUE);
            if (CompareDates(&ds,&data->HLDates)!=0)
            {
                stopDOSNotify(obj,data);

                if (data->Prefs.Options & OPT_AutoReload)
                {
                    data->Flags |= FLG_DataSaved;
                    DoMethod(obj,MUIM_CManager_LoadData,data->User,data->Path);
                }
                else request(obj,data,MUIV_CManager_Requester_Reload,CM_GetString(Msg_Req_ChangedMsg),CM_GetString(Msg_Req_ChangedGads));
            }
        }

        data->Flags |= FLG_ExternalChanged;
        setStatus(data);
    }

    return (ULONG)(res ? TRUE : FALSE);
}

/***************************************************************************/

static ULONG
mSetGroup(struct IClass *cl,Object *obj,struct MUIP_CManager_SetGroup *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct CMGroup *entry;

    if ((msg->Node!=MUIV_NListtree_Active_Off) && (msg->Node!=data->LastActive))
    {
        register struct MUI_NListtree_TreeNode *tn;
        register struct CMGroup                *group;
        register ULONG                         type = 0, isnew; //gcc

        DoMethod(data->GroupsList,MUIM_GroupList_RebuildList,data->EntriesList,data->LastActive);
        data->LastActive = msg->Node;

        set(data->EntriesList,MUIA_NList_Quiet,MUIV_NList_Quiet_Visual);

        tn = (struct MUI_NListtree_TreeNode *)DoMethod(data->GroupsList,MUIM_GroupList_GetRoot,msg->Node);
        group = CMGROUP(tn->tn_User);

        if (group==data->CMData->Users) type = CMEntry_User;
        else if (group==data->CMData->WWWs) type = CMEntry_WWW;
             else if (group==data->CMData->FTPs) type = CMEntry_FTP;
                  else if (group==data->CMData->Chat) type = CMEntry_Chat;
                       else if (group==data->CMData->Telnet) type = CMEntry_Telnet;

        set(data->EntriesList,MUIA_EntryList_Type,type);

        isnew = data->LastType!=type;
        if (isnew) data->LastType = type;

        entry = CMGROUP(msg->Node->tn_User);

        DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Disabled,entry->Type==CMEntry_Section,
            data->GroupName,
            data->GroupUp,
            data->GroupDown,
            NULL);

        if (entry->Type!=CMEntry_Section) nnset(data->GroupName,MUIA_Textinput_Contents,entry->Name);
        else nnset(data->GroupName,MUIA_Textinput_Contents,NULL);

        DoMethod(data->EntriesList,MUIM_NList_Clear);

        SetAttrs(data->EntriesList,MUIA_EntryList_Group, msg->Node,
                                   MUIA_NList_Active,    MUIV_NList_Active_Top,
                                   MUIA_NList_First,     MUIV_NList_First_Top, /* NList.mcc bug */
                                   MUIA_NList_Quiet,     FALSE,
                                   TAG_DONE);


        if (isnew)
        {
            register ULONG a = 0; //gcc

            switch (type)
            {
                case CMEntry_User:   a = 0; break;
                case CMEntry_WWW:    a = 1; break;
                case CMEntry_FTP:    a = 2; break;
                case CMEntry_Chat:   a = 3; break;
                case CMEntry_Telnet: a = 4; break;
            }

            if (!(data->Flags & FLG_NoQuick))
                set(data->QuickGroup,MUIA_Group_ActivePage,a);
        }
    }

    return 0;
}

/***************************************************************************/

static ULONG
mGetGroup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data                   *data = INST_DATA(cl,obj);
    register struct MUI_NListtree_TreeNode *tn;

    if (tn = (struct MUI_NListtree_TreeNode *)xget(data->GroupsList,MUIA_NListtree_Active))
    {
        getstr(data->GroupName,CMGROUP(tn->tn_User)->Name);
        DoMethod(data->GroupsList,MUIM_NListtree_Redraw,MUIV_NListtree_Redraw_Active,0);
        set(obj,MUIA_CManager_Changed,TRUE);
    }

    return 0;
}

/***************************************************************************/

static ULONG
mLoadData(struct IClass *cl,Object *obj,struct MUIP_CManager_LoadData *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       res = FALSE;

    sleepApp(obj,TRUE);

    stopDOSNotify(obj,data);

    if (!(data->Flags & FLG_DataSaved))
    {
        if (msg->User) stccpy(data->tUser,msg->User,sizeof(data->tUser));
        else *data->tUser = 0;

        if (msg->Path) stccpy(data->tPath,msg->Path,sizeof(data->tPath));
        else *data->tPath = 0;

        request(obj,data,MUIV_CManager_Requester_SaveChanges,CM_GetString(Msg_Req_NotSavedMsg),CM_GetString(Msg_Req_NotSavedGads));

        sleepApp(obj,FALSE);

        return TRUE;
    }

    DoMethod(obj,MUIM_CManager_Cleanup,TRUE);
    DoMethod(obj,MUIM_CManager_FreeData);

    *data->User = *data->Path = 0;

    if (msg->User) stccpy(data->User,msg->User,sizeof(data->User));
    if (msg->Path) stccpy(data->Path,msg->Path,sizeof(data->Path));

    if (!*data->User && !*data->Path)
    {
        if (data->CMData = (struct CMData *)CM_LoadData(CMA_LoadData_Current,TRUE,CMA_LoadData_Codeset,(ULONG)data->Prefs.Codeset,TAG_DONE))
        {
            strcpy(data->User,data->CMData->CurrentUser);
            res = TRUE;
        }
        else goto end;
    }

    if (!data->CMData)
    {
        if (data->CMData = CM_AllocCMData())
        {
            REGARRAY UBYTE file[256];

            if (*data->Path) stccpy(data->CMData->Path,data->Path,sizeof(data->CMData->Path));
            else
            {
                stccpy(data->CMData->Path,lib_CMPath,sizeof(data->CMData->Path));
                sprintf(file,"%s/%s.data",data->User,data->User);
                AddPart(data->CMData->Path,file,sizeof(data->CMData->Path));
            }

            res = CM_LoadData(CMA_LoadData_File,(ULONG)data->CMData->Path,CMA_LoadData_CMData,(ULONG)data->CMData,CMA_LoadData_User,(ULONG)data->CMData->CurrentUser,CMA_LoadData_Codeset,(ULONG)data->Prefs.Codeset,TAG_DONE);
        }
    }

    if (data->CMData)
    {
        register ULONG flags;

        DoMethod(obj,MUIM_CManager_LoadPrefs);

        data->Flags |= FLG_DataSaved;

        set(obj,MUIA_CManager_Changed,FALSE);
        set(data->GroupsList,MUIA_GroupList_CMData,data->CMData);

        flags = 0;
        if (data->Flags & FLG_HideUsers)  flags |= MUIV_GroupList_HideUsers;
        if (data->Flags & FLG_HideWWW)    flags |= MUIV_GroupList_HideWWW;
        if (data->Flags & FLG_HideFTP)    flags |= MUIV_GroupList_HideFTP;
        if (data->Flags & FLG_HideChat)   flags |= MUIV_GroupList_HideChat;
        if (data->Flags & FLG_HideTelnet) flags |= MUIV_GroupList_HideTelnet;

        DoMethod(data->GroupsList,MUIM_GroupList_InsertGroups,data->CMData,flags);

        startDOSNotify(obj,data);
    }

end:

    sleepApp(obj,FALSE);

    return res;
}

/***************************************************************************/

static ULONG
mReload(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    REGARRAY UBYTE       User[64], Path[256];

    if (data->User) stccpy(User,data->User,sizeof(User));
    else *User = 0;

    if (data->Path) stccpy(Path,data->Path,sizeof(Path));
    else *Path = 0;

    data->Flags |= FLG_DataSaved;

    return DoMethod(obj,MUIM_CManager_LoadData,User,Path);
}

/***************************************************************************/

static ULONG
mSaveData(struct IClass *cl,Object *obj,struct MUIP_CManager_SaveData *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    REGARRAY UBYTE       backup[256];

    if ((data->Flags & FLG_DataSaved) && (!msg->Path || !msg->Path[0]))
        return FALSE;

    stopDOSNotify(obj,data);

    if (!data->CMData)
    {
        if (data->CMData = CM_AllocCMData())
        {
            stccpy(data->CMData->CurrentUser,data->User,sizeof(data->CMData->CurrentUser));
            stccpy(data->CMData->Path,lib_CMPath,sizeof(data->CMData->Path));
            AddPart(data->CMData->Path,DEF_DataFile,sizeof(data->CMData->Path));
            set(data->GroupsList,MUIA_GroupList_CMData,data->CMData);
        }
        else return FALSE;
    }

    DoMethod(obj,MUIM_CManager_GrabLists);

    if (msg->Path && msg->Path[0])
        stccpy(data->CMData->Path,msg->Path,sizeof(data->CMData->Path));

    sprintf(backup,"%s.bak",data->CMData->Path);
    DeleteFile(backup);
    Rename(data->CMData->Path,backup);

    if (*data->User) createUserDir(data->User);
    CM_SaveData(CMA_SaveData_File,(ULONG)data->CMData->Path,CMA_SaveData_CMData,(ULONG)data->CMData,CMA_SaveData_User,(ULONG)data->User,CMA_SaveData_Codeset,(ULONG)data->Prefs.Codeset,TAG_DONE);

    if ((data->Prefs.Options & OPT_UseSavedProg) && data->Prefs.SavedProg[0])
    {
        REGARRAY UBYTE              buf[256];
        register STRPTR		    cmd;
        REGARRAY struct placeHolder ph[1];

        ph[0].flag  = 'd';
        ph[0].value = data->CMData->Path;

        if (cmd = expandPlaceHolders(data->Prefs.SavedProg,ph,buf,sizeof(buf),1))
        {
            SystemTags(cmd,SYS_Asynch,   TRUE,
                           SYS_Input,    Open("NIL:", MODE_NEWFILE),
                           SYS_Output,   Open("NIL:", MODE_NEWFILE),
                           NP_StackSize, 8192,
                           TAG_END);

            if (cmd!=buf) freeArbitrateVecPooled(cmd);
        }
    }

    startDOSNotify(obj,data);

    data->Flags |= FLG_DataSaved;

    set(obj,MUIA_CManager_Changed,FALSE);

    return TRUE;
}

/***************************************************************************/

static ULONG
mFreeData(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    stopDOSNotify(obj,data);

    if (data->CMData)
    {
        DoMethod(obj,MUIM_CManager_GrabLists);
        CM_FreeData(data->CMData);
        data->CMData = NULL;

        set(data->GroupsList,MUIA_NListtree_Quiet,TRUE);
        set(data->EntriesList,MUIA_NList_Quiet,MUIV_NList_Quiet_Visual);

        DoMethod(data->GroupsList,MUIM_GroupList_Clear);
        DoMethod(data->EntriesList,MUIM_NList_Clear);

        set(data->EntriesList,MUIA_NList_Quiet,FALSE);
        set(data->GroupsList,MUIA_NListtree_Quiet,FALSE);

        if (!(data->Flags & FLG_Disposing))
            data->LastActive = MUIV_NListtree_Active_Off;
    }

    return TRUE;
}

/***************************************************************************/

static ULONG
mLoadPrefs(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       obPos = data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition, nbPos;

    sleepApp(obj,TRUE);

    loadPrefs(data->User,&data->Prefs);
    copymem(&data->LastSavedVisualPrefs,&data->Prefs.VisualPrefs,sizeof(data->LastSavedVisualPrefs));
    nbPos = data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition;

    if (data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Hide)
    {
        data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition = obPos;
        set(obj,MUIA_CManager_NoBar,TRUE);
        data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition = nbPos;
    }

    setBarPrefs(cl,obj,data,obPos,nbPos);

    if (data->Flags & FLG_Setup) DoMethod(data->MainGroup,MUIM_Group_InitChange);
    set(data->GroupsGroup,MUIA_HorizWeight,data->Prefs.VisualPrefs.ListPrefs.TreeW);
    set(data->SuperEntriesGroup,MUIA_HorizWeight,data->Prefs.VisualPrefs.ListPrefs.ListW);
    if (data->Flags & FLG_Setup) DoMethod(data->MainGroup,MUIM_Group_ExitChange);

    if (!(data->Flags & FLG_NoCard))
    {
        if (data->Flags & FLG_Setup) DoMethod(data->EntriesGroup,MUIM_Group_InitChange);
        set(data->InnerEntriesGroup,MUIA_VertWeight,data->Prefs.VisualPrefs.ListPrefs.ListCW);
        set(data->Card,MUIA_VertWeight,data->Prefs.VisualPrefs.ListPrefs.CardCW);
        if (data->Flags & FLG_Setup) DoMethod(data->EntriesGroup,MUIM_Group_ExitChange);
    }

    SetAttrs(obj,MUIA_CManager_NoQuick,data->Prefs.VisualPrefs.Flags & VPFLG_NoQuick,
                 MUIA_CManager_NoCard,data->Prefs.VisualPrefs.Flags & VPFLG_NoCard,
                 TAG_DONE);

    if (!(data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Hide)) set(obj,MUIA_CManager_NoBar,FALSE);

    if (data->Flags & FLG_Setup) set(data->EntriesList,MUIA_NList_Quiet,TRUE);
    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_CManager_Prefs,&data->Prefs,data->GroupsList,data->EntriesList,data->Card,NULL);
    if (data->Flags & FLG_Setup) set(data->EntriesList,MUIA_NList_Quiet,FALSE);

    if (data->PrefsWin) set(data->PrefsWin,MUIA_CManager_Prefs,&data->Prefs);

    sleepApp(obj,FALSE);

    return 0;
}

/***************************************************************************/

static ULONG
mSetPrefs(struct IClass *cl,Object *obj,struct MUIP_CManager_SetPrefs *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       obPos, nbPos;

    if (msg->flags & MUIV_CManager_SetPrefs_SaveCurrent)
    {
        struct ListPrefs *lp;

        get(data->EntriesList,MUIA_EntryList_ListPrefs,&lp);
        copymem(&data->Prefs.VisualPrefs.ListPrefs,lp,sizeof(struct ListPrefs));

        data->Prefs.VisualPrefs.ListPrefs.TreeW = xget(data->GroupsGroup,MUIA_HorizWeight);
        data->Prefs.VisualPrefs.ListPrefs.ListW = xget(data->SuperEntriesGroup,MUIA_HorizWeight);

        if (!(data->Flags & FLG_NoCard))
        {
            data->Prefs.VisualPrefs.ListPrefs.ListCW = xget(data->InnerEntriesGroup,MUIA_VertWeight);
            data->Prefs.VisualPrefs.ListPrefs.CardCW = xget(data->Card,MUIA_VertWeight);
        }

        savePrefs(data->User,&data->Prefs);
        copymem(&data->LastSavedVisualPrefs,&data->Prefs.VisualPrefs,sizeof(data->LastSavedVisualPrefs));

        return 0;
    }

    obPos = data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition;

    if (msg->flags & MUIV_CManager_SetPrefs_Full) copymem(&data->Prefs,msg->prefs,sizeof(data->Prefs));
    else
    {
        struct ListPrefs lprefs;

        copymem(&lprefs,&data->Prefs.VisualPrefs.ListPrefs,sizeof(lprefs));
        copymem(&data->Prefs,msg->prefs,sizeof(data->Prefs));
        copymem(&data->Prefs.VisualPrefs.ListPrefs,&lprefs,sizeof(data->Prefs.VisualPrefs.ListPrefs));
    }

    nbPos = data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition;

    if (msg->flags & MUIV_CManager_SetPrefs_Save)
    {
        REGARRAY UBYTE prefsFile[256];

        savePrefs(data->User,&data->Prefs);
        copymem(&data->LastSavedVisualPrefs,&data->Prefs.VisualPrefs,sizeof(data->LastSavedVisualPrefs));

        strcpy(prefsFile,lib_CMPath);
        if (data->User[0]) AddPart(prefsFile,data->User,sizeof(prefsFile));
        AddPart(prefsFile,DEF_HotlistStampsFile,sizeof(prefsFile));
        readStamps(&data->Prefs);
        saveDates(prefsFile,data->Prefs.Stamps);
    }

    setBarPrefs(cl,obj,data,obPos,nbPos);

    if (msg->flags & MUIV_CManager_SetPrefs_Full)
    {
        set(obj,MUIA_CManager_NoBar,(data->Prefs.VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Hide) ? TRUE : FALSE);

        if (data->Flags & FLG_Setup) DoMethod(data->MainGroup,MUIM_Group_InitChange);
        set(data->GroupsGroup,MUIA_HorizWeight,data->Prefs.VisualPrefs.ListPrefs.TreeW);
        set(data->SuperEntriesGroup,MUIA_HorizWeight,data->Prefs.VisualPrefs.ListPrefs.ListW);
        if (data->Flags & FLG_Setup) DoMethod(data->MainGroup,MUIM_Group_ExitChange);

        if (!(data->Flags & FLG_NoCard))
        {
            if (data->Flags & FLG_Setup) DoMethod(data->EntriesGroup,MUIM_Group_InitChange);
            set(data->InnerEntriesGroup,MUIA_VertWeight,data->Prefs.VisualPrefs.ListPrefs.ListCW);
            set(data->Card,MUIA_VertWeight,data->Prefs.VisualPrefs.ListPrefs.CardCW);
            if (data->Flags & FLG_Setup) DoMethod(data->EntriesGroup,MUIM_Group_ExitChange);
        }
    }

    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_CManager_Prefs,&data->Prefs,data->GroupsList,data->EntriesList,data->Card,NULL);

    if (data->PrefsWin && !(msg->flags & MUIV_CManager_SetPrefs_OneWay)) set(data->PrefsWin,MUIA_CManager_Prefs,&data->Prefs);

    return 0;
}

/***************************************************************************/

static ULONG
mLogin(struct IClass *cl,Object *obj,struct MUIP_CManager_Login *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register STRPTR      user = (msg->user && *msg->user) ? msg->user : NULL;

    if (user && GenesisBase && strcmp(user,data->User))
    {
        register ULONG found;
        register int   i;

        sleepApp(obj,TRUE);

        for (found = FALSE, i = 0; ; i++)
        {
            REGARRAY UBYTE buf[256];

            if (!GetUserName(i,buf,sizeof(buf))) break;

            if (!strcmp(user,buf))
            {
                found = TRUE;
                break;
            }
        }

        if (found)
        {
            register struct User *ud;

            if (ud = GetUser(user,NULL,NULL,0))
            {
                if (strcmp(ud->us_name,data->User))
                {
                    updateVisualPrefs(data);
                    DoMethod(obj,MUIM_CManager_LoadData,ud->us_name,NULL);
                }


                FreeUser(ud);
            }
        }

        sleepApp(obj,FALSE);
    }

    return 0;
}

/***************************************************************************/

static ULONG
mSetFreeCol(struct IClass *cl,Object *obj,struct MUIP_CManager_SetFreeCol *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch (msg->type)
    {
        case CMEntry_User:
            data->Prefs.VisualPrefs.ListPrefs.UserCols[msg->col] = msg->val;
            break;

        case CMEntry_WWW:
            data->Prefs.VisualPrefs.ListPrefs.WWWCols[msg->col] = msg->val;
            break;

        case CMEntry_FTP:
            data->Prefs.VisualPrefs.ListPrefs.FTPCols[msg->col] = msg->val;
            break;

        default:
            break;
    }

    return 0;
}

/***********************************************************************/

static ULONG
mSetButtonsBarPrefs(struct IClass *cl,Object *obj,struct MUIP_CManager_SetButtonsBarPrefs *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       obPos = data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition;

    sleepApp(obj,TRUE);

    copymem(&data->Prefs.VisualPrefs.ButtonsBarPrefs,msg->prefs,sizeof(data->Prefs.VisualPrefs.ButtonsBarPrefs));
    setBarPrefs(cl,obj,data,obPos,data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition);
    if (data->PrefsWin) set(data->PrefsWin,MUIA_CManager_Prefs,&data->Prefs);

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mAddGroup(struct IClass *cl,Object *obj,struct MUIP_CManager_AddGroup *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct CMGroup *entry;

    if (!data->CMData) return 0;

    if (entry = CM_AllocEntry(CMEntry_Group))
    {
        register struct MUI_NListtree_TreeNode *tn;
        register struct CMGroup                *parent;

        entry->Flags = CMGROUPFLG_Open;

        tn = (struct MUI_NListtree_TreeNode *)xget(data->GroupsList,MUIA_NListtree_Active);
        parent = CMGROUP(tn->tn_User);

        AddTail((struct List *)&parent->SubGroups,(struct Node *)entry);

        if (msg->name)
        {
            strncpy(entry->Name,msg->name,sizeof(entry->Name));
            DoMethod(data->GroupsList,MUIM_NListtree_Redraw,MUIV_NListtree_Redraw_Active,0);
        }

         DoMethod(data->GroupsList,MUIM_NListtree_Insert,
            "M",entry,tn,MUIV_NListtree_Insert_PrevNode_Tail,
            TNF_LIST | MUIV_NListtree_Insert_Flag_Active);

        set(_win(obj),MUIA_Window_ActiveObject,data->GroupName);
        set(obj,MUIA_CManager_Changed,TRUE);
    }

    return 0;
}

/***************************************************************************/

static ULONG
mRemGroup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data                   *data = INST_DATA(cl,obj);
    register struct MUI_NListtree_TreeNode *tn;

    if (tn = (struct MUI_NListtree_TreeNode *)xget(data->GroupsList,MUIA_NListtree_Active))
    {
        if (CMUSER(tn->tn_User)->Type!=CMEntry_Section)
        {
            set(data->EntriesList,MUIA_NList_Quiet,MUIV_NList_Quiet_Visual);
            set(data->GroupsList,MUIA_NListtree_Quiet,TRUE);

            DoMethod(data->EntriesList,MUIM_NList_Clear);
            DoMethod(data->GroupsList,MUIM_NListtree_Remove,MUIV_NListtree_Remove_ListNode_Active,MUIV_NListtree_Remove_TreeNode_Active,0);

            set(data->EntriesList,MUIA_NList_Quiet,FALSE);
            set(data->GroupsList,MUIA_NListtree_Quiet,FALSE);

            set(obj,MUIA_CManager_Changed,TRUE);
        }
        else
        {
            if (data->Flags & FLG_Rem)
            {
                DoMethod(data->GroupsList,MUIM_NListtree_Remove,tn,MUIV_NListtree_Remove_TreeNode_All,0);
                DoMethod(data->EntriesList,MUIM_NList_Clear);
                DoMethod(data->GroupsList,MUIM_NListtree_Redraw,tn,0);
                set(obj,MUIA_CManager_Changed,TRUE);
            }
            else
            {
                REGARRAY UBYTE buf[64];

                data->Flags |= FLG_Rem;
                snprintf(buf,sizeof(buf),CM_GetString(Msg_Req_GroupDelWarningOne),1);
                request(obj,data,MUIV_CManager_Requester_GroupDelWarning,buf,CM_GetString(Msg_Req_GroupDelWarningGads));
            }
        }
    }

    return 0;
}

/***************************************************************************/

static ULONG
mAddItem(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data                   *data = INST_DATA(cl,obj);
    register struct MUI_NListtree_TreeNode *tn;

    if (!data->CMData) return 0;

    if (tn = (struct MUI_NListtree_TreeNode *)xget(data->GroupsList,MUIA_NListtree_Active))
    {
        register struct CMGroup *group;
        register struct CMEntry *entry;
        register UBYTE          type = 0; //gcc

        //if (!(tn->tn_Flags & TNF_OPEN)) DoMethod(data->GroupsList,MUIM_NListtree_Open,tn,tn,0);

        tn = (struct MUI_NListtree_TreeNode *)DoMethod(data->GroupsList,MUIM_GroupList_GetRoot,tn);
        group = CMGROUP(tn->tn_User);

        if (group==data->CMData->Users) type = CMEntry_User;
        else if (group==data->CMData->WWWs) type = CMEntry_WWW;
             else if (group==data->CMData->FTPs) type = CMEntry_FTP;
                  else if (group==data->CMData->Chat) type = CMEntry_Chat;
                       else if (group==data->CMData->Telnet) type = CMEntry_Telnet;

        if (entry = CM_AllocEntry(type))
        {
            switch (type)
            {
                case CMEntry_FTP:
                    CMFTP(entry)->Port = 21;
                    break;

                case CMEntry_Telnet:
                    CMTELNET(entry)->Port = 23;
                    break;
            }
            set(data->EntriesList,MUIA_NList_Quiet,MUIV_NList_Quiet_Visual);
            DoMethod(data->EntriesList,MUIM_NList_InsertSingle,entry,(data->Prefs.Options & OPT_ManualSort) ? MUIV_NList_Insert_Bottom : MUIV_NList_Insert_Sorted);
            SetAttrs(data->EntriesList,MUIA_NList_Active, xget(data->EntriesList,MUIA_NList_InsertPosition),
                                       MUIA_NList_Quiet,  FALSE,
                                       TAG_DONE);

            if (data->Prefs.Options & OPT_EditNew)
            {
                entry->Flags |= CMENTRYFLG_New;
                DoMethod(data->EntriesList,MUIM_EntryList_Edit);
            }
            else set(obj,MUIA_CManager_Changed,TRUE);
        }
    }

    return 0;
}

/***************************************************************************/

static ULONG
mAddEntry(struct IClass *cl,Object *obj,struct MUIP_CManager_AddEntry *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!data->CMData) return 0;

    if ((msg->Flags & MUIV_CManager_AddEntry_CurrentGroup) && (msg->Entry->Type==xget(data->EntriesList,MUIA_EntryList_Type)))
    {
        ULONG p;

        DoMethod(data->EntriesList,MUIM_NList_InsertSingle,msg->Entry,(data->Prefs.Options & OPT_ManualSort) ? MUIV_NList_Insert_Bottom : MUIV_NList_Insert_Sorted);
        get(data->EntriesList,MUIA_NList_InsertPosition,&p);
        set(data->EntriesList,MUIA_NList_Active,p);
    }
    else
    {
        struct MinList                *list = NULL; //gcc
        struct MUI_NListtree_TreeNode *tn;

        switch (msg->Entry->Type)
        {
            case CMEntry_User:
                list = &data->CMData->Users->Entries;
                break;

            case CMEntry_WWW:
                list = &data->CMData->WWWs->Entries;
                break;

            case CMEntry_FTP:
                list = &data->CMData->FTPs->Entries;
                break;

            case CMEntry_Chat:
                list = &data->CMData->Chat->Entries;
                break;

            case CMEntry_Telnet:
                list = &data->CMData->Telnet->Entries;
                break;
        }

        /* shall we add to the current group? */
        if ((tn = (struct MUI_NListtree_TreeNode *)xget(data->GroupsList,MUIA_NListtree_Active)) &&
            ((struct MinList *)tn->tn_User==list))
        {
            ULONG p;

            DoMethod(data->EntriesList,MUIM_NList_InsertSingle,msg->Entry,(data->Prefs.Options & OPT_ManualSort) ? MUIV_NList_Insert_Bottom : MUIV_NList_Insert_Sorted);
            get(data->EntriesList,MUIA_NList_InsertPosition,&p);
            set(data->EntriesList,MUIA_NList_Active,p);
        }
        else AddHead((struct List *)list, (struct Node *)msg->Entry); /* not in the visible list */
    }

    set(obj,MUIA_CManager_Changed,TRUE);

    DoMethod(obj,MUIM_CManager_SaveData,NULL);

    return 0;
}

/***************************************************************************/

static ULONG
mEditSelected(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!data->CMData) return 0;
    return DoMethod(data->EntriesList,MUIM_EntryList_Edit);
}

/***********************************************************************/

static ULONG
mRemSelected(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       l = data->Prefs.DelWarning;

    if (!data->CMData) return 0;

    if (l<=DEF_DelLimit)
    {
        ULONG sel;

        DoMethod(data->EntriesList,MUIM_NList_Select,MUIV_NList_Select_All,MUIV_NList_Select_Ask,&sel);
        if (sel>l)
        {
            REGARRAY UBYTE buf[64];

            snprintf(buf,sizeof(buf),CM_GetString((sel>1) ? Msg_Req_DelWarningMany : Msg_Req_DelWarningOne),sel);
            request(obj,data,MUIV_CManager_Requester_DelWarning,buf,CM_GetString(Msg_Req_DelWarningGads));
            return 0;
        }
    }

    DoMethod(data->EntriesList,MUIM_EntryList_Remove);

    return 0;
}

/***********************************************************************/

static ULONG
mOnDoubleClick(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    LONG                 dc;

    /* To avoid clicking on title to act like a doubleclick */
    get(data->EntriesList,MUIA_NList_DoubleClick,&dc);
    if (dc<0) return 0;

    if (data->DoubleClickHook || data->DoubleClickObject)
    {
        APTR entry;

        DoMethod(data->EntriesList,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);

        if (data->DoubleClickHook) CallHookPkt(data->DoubleClickHook,obj,entry);
        else DoMethod(data->DoubleClickObject,MUIM_CManager_DoubleClick,entry);

        return 0;
    }
    else return DoMethod(data->EntriesList,MUIM_EntryList_DoubleClick);
}

/***********************************************************************/

static ULONG
mGrabLists(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data                   *data = INST_DATA(cl,obj);
    register struct MUI_NListtree_TreeNode *tn;

    tn = (struct MUI_NListtree_TreeNode *)xget(data->GroupsList,MUIA_NListtree_Active);
    DoMethod(data->GroupsList,MUIM_GroupList_RebuildList,data->EntriesList,tn);
    DoMethod(data->GroupsList,MUIM_GroupList_RebuildTree,data->CMData,data->Flags);

    data->Flags |= FLG_GrabbedLists;

    return 0;
}

/***************************************************************************/

static ULONG
mReinsertLists(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->Flags & FLG_GrabbedLists)
    {
        register ULONG flags;

        flags = 0;
        if (data->Flags & FLG_HideUsers)  flags |= MUIV_GroupList_HideUsers;
        if (data->Flags & FLG_HideWWW)    flags |= MUIV_GroupList_HideWWW;
        if (data->Flags & FLG_HideFTP)    flags |= MUIV_GroupList_HideFTP;
        if (data->Flags & FLG_HideChat)   flags |= MUIV_GroupList_HideChat;
        if (data->Flags & FLG_HideTelnet) flags |= MUIV_GroupList_HideTelnet;

        set(data->GroupsList,MUIA_NListtree_Quiet,TRUE);
        set(data->EntriesList,MUIA_NList_Quiet,MUIV_NList_Quiet_Visual);

        DoMethod(data->EntriesList,MUIM_NList_Clear);

        data->LastActive = MUIV_NListtree_Active_Off;
        DoMethod(data->GroupsList,MUIM_GroupList_Clear);
        DoMethod(data->GroupsList,MUIM_GroupList_InsertGroups,data->CMData,flags);

        set(data->EntriesList,MUIA_NList_Quiet,FALSE);
        set(data->GroupsList,MUIA_NListtree_Quiet,FALSE);

        data->Flags &= ~FLG_GrabbedLists;
    }

    return 0;
}

/***************************************************************************/

void importATC(struct CMGroup *group,struct MUIS_CManager_IO *io);

static ULONG
mImportDB(struct IClass *cl,Object *obj,struct MUIP_CManager_Import *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register BPTR        file;
    register STRPTR      name = NULL;

    sleepApp(obj,TRUE);

    DoMethod(obj,MUIM_CManager_GrabLists);

    if (file = Open(msg->File,MODE_OLDFILE))
    {
        struct MUIS_CManager_IO io;

        io.file    = file;
        io.codeset = data->Prefs.Codeset;
        io.flags   = msg->Flags | ((data->Prefs.Options & OPT_ClosedGroups) ? MUIV_CManager_Import_ClosedGroup : 0);
        io.added   = 0;

        switch (msg->Type)
        {
            case MUIV_CManager_Import_Voyager:
                importVoyager(data->CMData->WWWs,&io);
                name = "WWWS";
                break;

            case MUIV_CManager_Import_IB:
                importIB(data->CMData->WWWs,&io);
                name = "WWWS";
                break;

            case MUIV_CManager_Import_AWeb:
                importAWeb(data->CMData->WWWs,&io);
                name = "WWWS";
                break;

            case MUIV_CManager_Import_Netscape:
                importNetscape(data->CMData->WWWs,&io);
                name = "WWWS";
                break;

            case MUIV_CManager_Import_Users_CSV:
                importUserCSV(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_Users_CSVFullOE:
                importUserCSVFullOE(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_AmFTP:
                importAmFTP(data->CMData->FTPs,&io);
                name = "FTPS";
                break;

            case MUIV_CManager_Import_DOpusFTP:
                importDOpusFTP(data->CMData->FTPs,&io);
                name = "FTPS";
                break;

            case MUIV_CManager_Import_MD:
                importMD(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_YAM:
                importYAM(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_SimpleMail:
                importSimpleMail(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_DFA:
                importDFA(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_PINT:
                importPINT(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_GoodNews:
                importGoodNews(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_STRICQ:
                importSTRICQ(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_STFax:
                importSTFax(data->CMData->Users,&io);
                name = "USERS";
                break;

            case MUIV_CManager_Import_AmIRC:
                importAmIRC(data->CMData->Chat,&io);
                name = "CHATS";
                break;

            case MUIV_CManager_Import_ATC:
                importATC(data->CMData->FTPs,&io);
                name = "FTPS";
                break;
        }

        Close(file);
    }

    DoMethod(obj,MUIM_CManager_ReinsertLists);

    nnset(obj,MUIA_CManager_Changed,TRUE);

    if (name)
    {
        register struct MUI_NListtree_TreeNode *tn;

        if (tn = (struct MUI_NListtree_TreeNode *)DoMethod(data->GroupsList,MUIM_NListtree_FindName,MUIV_NListtree_FindName_ListNode_Root,name,0))
        {
            set(data->GroupsList,MUIA_NListtree_Active,tn);
        }
    }

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/

void exportATC(struct CMGroup *Group,struct MUIS_CManager_IO *io);

static ULONG
mExportDB(struct IClass *cl,Object *obj,struct MUIP_CManager_Export *msg )
{
    register struct data *data = INST_DATA(cl,obj);
    register BPTR        file;

    sleepApp(obj,TRUE);

    DoMethod(obj,MUIM_CManager_GrabLists);

    if (file = Open(msg->File,MODE_NEWFILE))
    {
        struct MUIS_CManager_IO io;

        io.file    = file;
        io.codeset = data->Prefs.Codeset;
        io.flags   = msg->Flags;
        io.added   = 0;

        switch (msg->Type)
        {
            case MUIV_CManager_Export_HTML_URLs:
                exportURLasHTML(data->CMData,&io);
                break;

            case MUIV_CManager_Export_HTML_WWW:
                exportWWWasHTML(data->CMData->WWWs,&io);
                break;

            case MUIV_CManager_Export_HTML_FTP:
                exportFTPasHTML(data->CMData->FTPs,&io);
                break;

            case MUIV_CManager_Export_CSV_Users:
                exportUsersAsCSV(data->CMData->Users,&io);
                break;

            case MUIV_CManager_Export_CSVFullOE_Users:
                exportUsersAsCSVFullOE(data->CMData->Users,&io);
                break;

            case MUIV_CManager_Export_AWeb:
                exportAWeb(data->CMData->WWWs,&io);
                break;

            case MUIV_CManager_Export_IBrowse:
                exportIBrowse(data->CMData->WWWs,&io);
                break;

            case MUIV_CManager_Export_YAM:
                exportYAM(data->CMData->Users,&io);
                break;

            case MUIV_CManager_Export_SimpleMail:
                exportSimpleMail(data->CMData->Users,&io);
                break;

            case MUIV_CManager_Export_Netscape:
                exportNetscape(data->CMData->WWWs,&io);
                break;

            case MUIV_CManager_Export_ATC:
                exportATC(data->CMData->FTPs,&io);
                break;
        }

        Close(file);
    }

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mScanHotlists(struct IClass *cl,Object *obj,struct MUIP_CManager_ScanHotlists *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       flags = msg->flags, mode = msg->mode;

    if (!data->CMData) return 0;

    if (data->Prefs.Options & OPT_SilentScan)
        mode = MUIV_CManager_ScanHotlists_Check|MUIV_CManager_ScanHotlists_Import|MUIV_CManager_ScanHotlists_SaveDates;

    if (mode & MUIV_CManager_ScanHotlists_Check)
    {
        register int i;

        for (flags = i = 0; i<NUM_HOTLISTS; i++)
            if (fileNew(data->Prefs.Hotlists[i],&data->Prefs.Stamps[i]))
                flags |= 1<<i;
    }

    if (mode & MUIV_CManager_ScanHotlists_Import)
    {
        register STRPTR file;
        register ULONG  type = 0; //gcc

        switch (flags)
        {
            case MUIV_CManager_ScanHotlists_IB:
                file = data->Prefs.Hotlists[HL_IB];
                type = MUIV_CManager_Import_IB;
                break;

            case MUIV_CManager_ScanHotlists_V:
                file = data->Prefs.Hotlists[HL_V];
                type = MUIV_CManager_Import_Voyager;
                break;

            case MUIV_CManager_ScanHotlists_AWEB:
                file = data->Prefs.Hotlists[HL_AWEB];
                type = MUIV_CManager_Import_AWeb;
                break;

            case MUIV_CManager_ScanHotlists_NETSCAPE:
                file = data->Prefs.Hotlists[HL_NETSCAPE];
                type = MUIV_CManager_Import_Netscape;
                break;

            case MUIV_CManager_ScanHotlists_AMFTP:
                file = data->Prefs.Hotlists[HL_AMFTP];
                type = MUIV_CManager_Import_AmFTP;
                break;

            case MUIV_CManager_ScanHotlists_MD2:
                file = data->Prefs.Hotlists[HL_MD2];
                type = MUIV_CManager_Import_MD;
                break;

            case MUIV_CManager_ScanHotlists_STFAX:
                file = data->Prefs.Hotlists[HL_STFAX];
                type = MUIV_CManager_Import_STFax;
                break;

            case MUIV_CManager_ScanHotlists_YAM:
                file = data->Prefs.Hotlists[HL_YAM];
                type = MUIV_CManager_Import_YAM;
                break;

            case MUIV_CManager_ScanHotlists_SIMPLEMAIL:
                file = data->Prefs.Hotlists[HL_SIMPLEMAIL];
                type = MUIV_CManager_Import_SimpleMail;
                break;

            case MUIV_CManager_ScanHotlists_AMIRC:
                file = data->Prefs.Hotlists[HL_AMIRC];
                type = MUIV_CManager_Import_AmIRC;
                break;

            case MUIV_CManager_ScanHotlists_DOPUS:
                file = data->Prefs.Hotlists[HL_DOPUS];
                type = MUIV_CManager_Import_DOpusFTP;
                break;

            case MUIV_CManager_ScanHotlists_DFA:
                file = data->Prefs.Hotlists[HL_DFA];
                type = MUIV_CManager_Import_DFA;
                break;

            case MUIV_CManager_ScanHotlists_PINT:
                file = data->Prefs.Hotlists[HL_PINT];
                type = MUIV_CManager_Import_PINT;
                break;

            case MUIV_CManager_ScanHotlists_GOODNEWS:
                file = data->Prefs.Hotlists[HL_GOODNEWS];
                type = MUIV_CManager_Import_GoodNews;
                break;

            case MUIV_CManager_ScanHotlists_STRICQ:
                file = data->Prefs.Hotlists[HL_STRICQ];
                type = MUIV_CManager_Import_STRICQ;
                break;

            default:
                file = NULL;
                break;
        }

        if (file) DoMethod(obj,MUIM_CManager_Import,type,file,MUIV_CManager_Import_AvoidDuplicate);

        flags = 0;
    }

    if (mode & MUIV_CManager_ScanHotlists_SaveDates)
    {
        REGARRAY UBYTE prefsFile[256];

        strcpy(prefsFile,lib_CMPath);
        if (data->User[0]) AddPart(prefsFile,data->User,sizeof(prefsFile));
        AddPart(prefsFile,DEF_HotlistStampsFile,sizeof(prefsFile));

        readStamps(&data->Prefs);
        saveDates(prefsFile,data->Prefs.Stamps);
    }

    return flags;
}

/***********************************************************************/

static ULONG
mSearch(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!data->CMData || !data->App) return 0;

    sleepApp(obj,TRUE);

    if (!data->SearchWin)
    {
        ObtainSemaphore(&lib_sem);

        if (lib_SearchWindow || initSearchWindow())
        {
            ReleaseSemaphore(&lib_sem);

            if (data->SearchWin = SearchWindowObject,
                    MUIA_Window_RefWindow,        data->Win,
                    MUIA_CManager_AppObj,         data->App,
                    MUIA_CManager_CMObj,          obj,
                    MUIA_CManager_AtHome,         data->Flags & FLG_AtHome,
                    MUIA_SearchWindow_HideUsers,  data->Flags & FLG_HideUsers,
                    MUIA_SearchWindow_HideWWW,    data->Flags & FLG_HideWWW,
                    MUIA_SearchWindow_HideFTP,    data->Flags & FLG_HideFTP,
                    MUIA_SearchWindow_HideChat,   data->Flags & FLG_HideChat,
                    MUIA_SearchWindow_HideTelnet, data->Flags & FLG_HideTelnet,
                End) DoMethod(data->App,OM_ADDMEMBER,data->SearchWin);
        }
        else ReleaseSemaphore(&lib_sem);
    }

    openWindow(data->App,data->SearchWin);

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mPrefsWindow(struct IClass *cl,Object *obj,struct MUIP_CManager_PrefsWindow *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       res = FALSE;

    if (data->CMData && data->App)
    {
        sleepApp(obj,TRUE);

        if (!data->PrefsWin)
        {
            ObtainSemaphore(&lib_sem);

            if ((lib_Popasl || initPopasl()) &&
                (lib_PopupCodesets || initPopupCodesets()) &&
                (lib_Popph || initPopph()) &&
                (lib_DelWarning || initDelWarning()) &&
                (lib_PrefsWindow || initPrefsWindow()))
            {
                ReleaseSemaphore(&lib_sem);

                if (data->PrefsWin = PrefsWindowObject,
                        MUIA_Window_RefWindow, data->Win,
                        MUIA_CManager_AppObj,  data->App,
                        MUIA_CManager_CMObj,   obj,
                        MUIA_CManager_Prefs,   &data->Prefs,
                        MUIA_CManager_AtHome,  data->Flags & FLG_AtHome,
                    End)
                {
                    DoMethod(data->App,OM_ADDMEMBER,data->PrefsWin);
                }
            }
            else ReleaseSemaphore(&lib_sem);
        }

        if (data->PrefsWin) set(data->PrefsWin,MUIA_PrefsWindow_ActivePage,msg->page);
        res = openWindow(data->App,data->PrefsWin);

        sleepApp(obj,FALSE);
    }

    return res;
}

/***********************************************************************/

static ULONG
mLoginWindow(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!data->App) return 0;

    sleepApp(obj,TRUE);

    if (GenesisBase && !data->LoginWin)
    {
        ObtainSemaphore(&lib_sem);

        if (lib_LoginWindow || initLoginWindow())
        {
            ReleaseSemaphore(&lib_sem);

            if (data->LoginWin = LoginWindowObject,
                    MUIA_CManager_AppObj,  data->App,
                    MUIA_CManager_CMObj,   obj,
                    MUIA_LoginWindow_User, data->User,
                    MUIA_Window_RefWindow, data->Win,
                    MUIA_CManager_AtHome,  data->Flags & FLG_AtHome,
                End) DoMethod(data->App,OM_ADDMEMBER,data->LoginWin);
        }
        else ReleaseSemaphore(&lib_sem);
    }

    openWindow(data->App,data->LoginWin);

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mMiniMailWindow(struct IClass *cl,Object *obj,struct MUIP_CManager_MiniMailWindow *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!data->App) return 0;

    sleepApp(obj,TRUE);

    if (!data->MiniMailWin)
    {
        ObtainSemaphore(&lib_sem);

        if (lib_miniMailWindow || initMiniMailWindow())
        {
            ReleaseSemaphore(&lib_sem);

            if (data->MiniMailWin = MiniMailWindowObject,
                    MUIA_CManager_AppObj,  data->App,
                    MUIA_CManager_CMObj,   obj,
                    MUIA_CManager_AtHome,  data->Flags & FLG_AtHome,
                    MUIA_MiniMail_User,    data->User,
                End) DoMethod(data->App,OM_ADDMEMBER,data->MiniMailWin);
        }
        else ReleaseSemaphore(&lib_sem);
    }

    if (data->MiniMailWin) set(data->MiniMailWin,MUIA_MiniMail_Entry,msg->entry);

    openWindow(data->App,data->MiniMailWin);

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mDisposeObj(struct IClass *cl,Object *obj,struct MUIP_CManager_DisposeObj *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register Object      *o = msg->Object;

    DoMethod(data->App,OM_REMMEMBER,o);
    MUI_DisposeObject(o);

    if (o==data->PrefsWin) data->PrefsWin = NULL;
    else if (o==data->SearchWin) data->SearchWin = NULL;
         else if (o==data->LoginWin) data->LoginWin = NULL;
              else if (o==data->MiniMailWin) data->MiniMailWin = NULL;

    return 0;
}

/***************************************************************************/

static void
closeAllCMWindows(struct data *data,Object *app,ULONG OnlyEdit)
{
    register ULONG loop;

    for (;;)
    {
        struct List     *l;
        register Object *win;
        Object          *mstate;

        loop = FALSE;

        for (get(app,MUIA_Application_WindowList,&l), mstate = (Object *)l->lh_Head; win = NextObject(&mstate); )
        {
            if (xget(win,MUIA_EditWindow_IsEditWin))
            {
   	        //NewRawDoFmt("Edit %lx\n",1,1,win);
                set(win,MUIA_Window_Open,FALSE);
                DoMethod(app,OM_REMMEMBER,win);
                MUI_DisposeObject(win);
                loop = TRUE;
                break;
            }

            if (!OnlyEdit && xget(win,MUIA_CManager_IsCMWin))
	    {
   	        //NewRawDoFmt("Other %lx\n",1,1,win);
                set(win,MUIA_Window_Open,FALSE);
                DoMethod(app,OM_REMMEMBER,win);
                MUI_DisposeObject(win);
                loop = TRUE;
                break;
            }
        }

        if (!loop) break;
    }

    data->PrefsWin    = NULL;
    data->SearchWin   = NULL;
    data->LoginWin    = NULL;
    data->MiniMailWin = NULL;
}

/***************************************************************************/

static ULONG
mCMCleanup(struct IClass *cl,Object *obj,struct MUIP_CManager_Cleanup *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->App) closeAllCMWindows(data,data->App,msg->OnlyEdit);

    return 0;
}

/***************************************************************************/

static ULONG
mClose(struct IClass *cl,Object *obj,struct MUIP_CManager_Close *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       quit = TRUE, save = FALSE;

    //DoMethod(obj,MUIM_CManager_Cleanup,FALSE);

    if (msg->mode==MUIV_CManager_Close_Quit)
    {
        if (!(data->Flags & FLG_DataSaved)) save = data->Prefs.Options & OPT_SaveOnExit;
    }
    else
    {
        if (!(data->Flags & FLG_DataSaved))
        {
            register ULONG dontAsk;

            if (msg->mode==MUIV_CManager_Close_Save) dontAsk = TRUE;
            else dontAsk = data->Prefs.Options & OPT_SaveOnExit;

            stopDOSNotify(obj,data);

            if (dontAsk) save = TRUE;
            else
                if (!(data->Prefs.Options & OPT_SilentExit))
                {
                    request(obj,data,MUIV_CManager_Requester_Close,CM_GetString(Msg_Req_NotSavedMsg),CM_GetString(Msg_Req_NotSavedGads));
                    quit = FALSE;
                }
        }
    }

    if (quit)
    {
        if (save) DoMethod(obj,MUIM_CManager_SaveData,NULL);
        data->Flags |= FLG_Quit;
        DoMethod(data->App,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);
    }

    return 0;
}

/***********************************************************************/

static ULONG
mRequesterResult(struct IClass *cl,Object *obj,struct MUIP_CManager_RequesterResult *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (msg->win)
    {
        set(msg->win,MUIA_Window_Open,FALSE);
        DoMethod(data->App,OM_REMMEMBER,msg->win);
        MUI_DisposeObject(msg->win);
    }

    switch (msg->type)
    {
        case MUIV_CManager_Requester_Reload:
            switch(msg->res)
            {
                case 1: /* Yes */
                    data->Flags |= FLG_DataSaved;
                    DoMethod(obj,MUIM_CManager_LoadData,data->User,data->Path);
                    break;

                default: /* No */
                    startDOSNotify(obj,data);
                    break;
            }
            break;

        case MUIV_CManager_Requester_SaveChanges:
            switch(msg->res)
            {
                case 1: /* Yes */
                    DoMethod(obj,MUIM_CManager_SaveData,NULL);
                    DoMethod(obj,MUIM_CManager_LoadData,data->tUser,data->tPath);
                    break;

                case 2: /* No */
                    data->Flags |= FLG_DataSaved;
                    DoMethod(obj,MUIM_CManager_LoadData,data->tUser,data->tPath);
                    break;

                default: /* Cancel */
                    startDOSNotify(obj,data);
                    break;
            }
            break;

        case MUIV_CManager_Requester_Close:
            switch(msg->res)
            {
                case 1: /* Yes */
                    data->Flags |= FLG_Quit;
                    DoMethod(obj,MUIM_CManager_SaveData,NULL);
                    DoMethod(data->App,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);
                    break;

                case 2: /* No */
                    data->Flags |= FLG_Quit;
                    DoMethod(data->App,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);
                    break;

                default: /* Cancel */
                    startDOSNotify(obj,data);
                    break;

            }
            break;

        case MUIV_CManager_Requester_DelWarning:
            switch(msg->res)
            {
                case 1: /* Yes */
                    DoMethod(data->EntriesList,MUIM_EntryList_Remove);
                    break;

                default: /* No */
                    startDOSNotify(obj,data);
                    break;
            }
            break;

        case MUIV_CManager_Requester_GroupDelWarning:
            switch(msg->res)
            {
                case 1: /* Yes */
                    DoMethod(obj,MUIM_CManager_RemGroup);
                    break;

                default: /* No */
                    startDOSNotify(obj,data);
                    break;
            }
            data->Flags &= ~FLG_Rem;
            break;
    }

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mDragQuery(struct IClass *cl,Object *obj,struct MUIP_DragQuery *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    return (ULONG)((msg->obj==data->ButtonsBar) ? MUIV_DragQuery_Accept : MUIV_DragQuery_Refuse);
}

/***********************************************************************/

static ULONG
mDragBegin(struct IClass *cl,Object *obj,struct MUIP_DragBegin *msg)
{
    //register struct data *data = INST_DATA(cl,obj);

    return 0;
}

/***********************************************************************/

#define DD_FACT   6
#define DD_INFACT 4

static ULONG
findPos(struct data *data,Object *obj,int obPos,ULONG mx,ULONG my)
{
    register ULONG bPos;
    register LONG  l, t, r, b, w, h, il, it, ib, ih;

    l = _mleft(obj);
    t = _mtop(obj);
    r = _mright(obj);
    b = _mbottom(obj);
    w = _mwidth(obj)/DD_FACT;
    h = _mheight(obj)/DD_FACT;

    il = _mleft(data->SuperEntriesGroup);
    it = _mtop(data->SuperEntriesGroup);
    ib = _mbottom(data->SuperEntriesGroup);
    ih = _mheight(data->SuperEntriesGroup)/DD_INFACT;

    bPos = 0;

    switch (obPos)
    {
        case SBPos_Left:
            if (mx<l+w) bPos = SBPos_Left;
            else if (mx>r-w) bPos = SBPos_Right;
                 else if (my<t+h) bPos = SBPos_Top;
                      else if (my>b-h) bPos = SBPos_Bottom;
                           else if (my<it+ih) bPos = SBPos_InnerTop;
                                else if (my>ib-ih) bPos = SBPos_InnerBottom;
            break;

        case SBPos_Right:
            if (mx>r-w) bPos = SBPos_Right;
            else if (mx<l+w) bPos = SBPos_Left;
                 else if (my<t+h) bPos = SBPos_Top;
                      else if (my>b-h) bPos = SBPos_Bottom;
                           else if (my<it+ih) bPos = SBPos_InnerTop;
                                else if (my>ib-ih) bPos = SBPos_InnerBottom;
            break;

        case SBPos_Top:
            if (my<t+h) bPos = SBPos_Top;
            else if (mx<l+w) bPos = SBPos_Left;
                 else if (mx>r-w) bPos = SBPos_Right;
                      else if (my>b-h) bPos = SBPos_Bottom;
                           else if (my<it+ih) bPos = SBPos_InnerTop;
                                else if (my>ib-ih) bPos = SBPos_InnerBottom;
            break;

        case SBPos_Bottom:
            if (my>b-h) bPos = SBPos_Bottom;
            else if (mx<l+w) bPos = SBPos_Left;
                 else if (mx>r-w) bPos = SBPos_Right;
                      else if (my<t+h) bPos = SBPos_Top;
                           else if (my<it+ih) bPos = SBPos_InnerTop;
                                else if (my>ib-ih) bPos = SBPos_InnerBottom;
            break;

        case SBPos_InnerTop:
            if ((my<it+ih) && (mx>il)) bPos = SBPos_InnerTop;
            else if (my<t+h) bPos = SBPos_Top;
                 else if (mx<l+w) bPos = SBPos_Left;
                      else if (mx>r-w) bPos = SBPos_Right;
                           else if (my>b-h) bPos = SBPos_Bottom;
                                else if (my>ib-ih) bPos = SBPos_InnerBottom;
            break;

        case SBPos_InnerBottom:
            if ((my>ib-ih) && (mx>il)) bPos = SBPos_InnerBottom;
            else if (my>b-h) bPos = SBPos_Bottom;
                 else if (mx<l+w) bPos = SBPos_Left;
                      else if (mx>r-w) bPos = SBPos_Right;
                           else if (my<t+h) bPos = SBPos_Top;
                                else if (my<it+ih) bPos = SBPos_InnerTop;
            break;

    }

    return (bPos==obPos) ? 0 : bPos;
}

/***********************************************************************/

static ULONG
mDragReport(struct IClass *cl,Object *obj,struct MUIP_DragReport *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       bPos, obPos;

    if (!msg->update) return MUIV_DragReport_Refresh;

    obPos = data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition;
    bPos = findPos(data,obj,obPos,msg->x,msg->y);

    if (bPos==0) return MUIV_DragReport_Continue;
    set(data->ButtonsBar,MUIA_TheBar_Limbo,TRUE);

    if ((bPos & SBPos_TopMode)   || (obPos & SBPos_TopMode))   DoSuperMethod(cl,obj,MUIM_Group_InitChange);
    if ((bPos & SBPos_LeftMode)  || (obPos & SBPos_LeftMode))  DoMethod(data->SuperMainGroup,MUIM_Group_InitChange);
    if ((bPos & SBPos_InnerMode) || (obPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,MUIM_Group_InitChange);

    if (obPos & SBPos_TopMode)
    {
        if (!(bPos & SBPos_TopMode)) DoSuperMethod(cl,obj,OM_REMMEMBER,data->ButtonsBar);
    }
    else
        if (obPos & SBPos_LeftMode)
        {
            if (!(bPos & SBPos_LeftMode)) DoMethod(data->SuperMainGroup,OM_REMMEMBER,data->ButtonsBar);
        }
        else
            if (obPos & SBPos_InnerMode)
            {
                if (!(bPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,OM_REMMEMBER,data->ButtonsBar);
            }

    switch (bPos)
    {
        case SBPos_Top:
            if (!(obPos & SBPos_TopMode)) DoSuperMethod(cl,obj,OM_ADDMEMBER,data->ButtonsBar);
            DoSuperMethod(cl,obj,MUIM_Group_Sort,data->ButtonsBar,data->SuperMainGroup,NULL);
            break;

        case SBPos_Bottom:
            if (!(obPos & SBPos_TopMode)) DoSuperMethod(cl,obj,OM_ADDMEMBER,data->ButtonsBar);
            DoSuperMethod(cl,obj,MUIM_Group_Sort,data->SuperMainGroup,data->ButtonsBar,NULL);
            break;

        case SBPos_Left:
            if (!(obPos & SBPos_LeftMode)) DoMethod(data->SuperMainGroup,OM_ADDMEMBER,data->ButtonsBar);
            DoMethod(data->SuperMainGroup,MUIM_Group_Sort,data->ButtonsBar,data->MainGroup,NULL);
            break;

        case SBPos_Right:
            if (!(obPos & SBPos_LeftMode)) DoMethod(data->SuperMainGroup,OM_ADDMEMBER,data->ButtonsBar);
            DoMethod(data->SuperMainGroup,MUIM_Group_Sort,data->MainGroup,data->ButtonsBar,NULL);
            break;

        case SBPos_InnerTop:
            if (!(obPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,OM_ADDMEMBER,data->ButtonsBar);
            DoMethod(data->SuperEntriesGroup,MUIM_Group_Sort,data->ButtonsBar,data->EntriesGroup,NULL);
            break;

        case SBPos_InnerBottom:
            if (!(obPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,OM_ADDMEMBER,data->ButtonsBar);
            DoMethod(data->SuperEntriesGroup,MUIM_Group_Sort,data->EntriesGroup,data->ButtonsBar,NULL);
            break;
    }

    if (!BOOLSAME(bPos & SBPos_Horiz,obPos & SBPos_Horiz))
        set(data->ButtonsBar,MUIA_Group_Horiz,(bPos & SBPos_Horiz) ? TRUE : FALSE);

    if ((bPos & SBPos_InnerMode) || (obPos & SBPos_InnerMode)) DoMethod(data->SuperEntriesGroup,MUIM_Group_ExitChange);
    if ((bPos & SBPos_LeftMode)  || (obPos & SBPos_LeftMode))  DoMethod(data->SuperMainGroup,MUIM_Group_ExitChange);
    if ((bPos & SBPos_TopMode)   || (obPos & SBPos_TopMode))   DoSuperMethod(cl,obj,MUIM_Group_ExitChange);

    set(data->ButtonsBar,MUIA_TheBar_Limbo,FALSE);

    data->Prefs.VisualPrefs.ButtonsBarPrefs.BarPosition = bPos;
    if (data->PrefsWin) set(data->PrefsWin,MUIA_PrefsWindow_BarPosition,bPos);

    return MUIV_DragReport_Continue;
}

/***********************************************************************/

static ULONG
mDragFinish(struct IClass *cl,Object *obj,struct MUIP_DragFinish *msg)
{
    //register struct data *data = INST_DATA(cl,obj);

    return 0;
}

/***********************************************************************/

static ULONG
mDragDrop(struct IClass *cl,Object *obj,struct MUIP_DragDrop *msg)
{
    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:                           return mNew(cl,obj,(APTR)msg);
        case OM_DISPOSE:                       return mDispose(cl,obj,(APTR)msg);
        case OM_GET:                           return mGet(cl,obj,(APTR)msg);
        case OM_SET:                           return mSets(cl,obj,(APTR)msg);

        case MUIM_Setup:                       return mSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup:                     return mCleanup(cl,obj,(APTR)msg);

        case MUIM_DragQuery:                   return mDragQuery(cl,obj,(APTR)msg);
        case MUIM_DragBegin:                   return mDragBegin(cl,obj,(APTR)msg);
        case MUIM_DragReport:                  return mDragReport(cl,obj,(APTR)msg);
        case MUIM_DragFinish:                  return mDragFinish(cl,obj,(APTR)msg);
        case MUIM_DragDrop:                    return mDragDrop(cl,obj,(APTR)msg);

        case MUIM_CManager_ChangeQuick:        return mChangeQuick(cl,obj,(APTR)msg);
        case MUIM_CManager_QuickChange:        return mQuickChange(cl,obj,(APTR)msg);

        case MUIM_CManager_HandleNotify:       return mHandleNotify(cl,obj,(APTR)msg);

        case MUIM_CManager_SetGroup:           return mSetGroup(cl,obj,(APTR)msg);
        case MUIM_CManager_GetGroup:           return mGetGroup(cl,obj,(APTR)msg);

        case MUIM_CManager_LoadData:           return mLoadData(cl,obj,(APTR)msg);
        case MUIM_CManager_Reload:             return mReload(cl,obj,(APTR)msg);
        case MUIM_CManager_SaveData:           return mSaveData(cl,obj,(APTR)msg);
        case MUIM_CManager_FreeData:           return mFreeData(cl,obj,(APTR)msg);
        case MUIM_CManager_LoadPrefs:          return mLoadPrefs(cl,obj,(APTR)msg);
        case MUIM_CManager_SetPrefs:           return mSetPrefs(cl,obj,(APTR)msg);
        case MUIM_CManager_Login:              return mLogin(cl,obj,(APTR)msg);
        case MUIM_CManager_SetButtonsBarPrefs: return mSetButtonsBarPrefs(cl,obj,(APTR)msg);
        case MUIM_CManager_SetFreeCol:         return mSetFreeCol(cl,obj,(APTR)msg);

        case MUIM_CManager_AddGroup:           return mAddGroup(cl,obj,(APTR)msg);
        case MUIM_CManager_RemGroup:           return mRemGroup(cl,obj,(APTR)msg);
        case MUIM_CManager_AddItem:            return mAddItem(cl,obj,(APTR)msg);
        case MUIM_CManager_AddEntry:           return mAddEntry(cl,obj,(APTR)msg);
        case MUIM_CManager_EditSelected:       return mEditSelected(cl,obj,(APTR)msg);
        case MUIM_CManager_RemSelected:        return mRemSelected(cl,obj,(APTR)msg);
        case MUIM_CManager_OnDoubleClick:      return mOnDoubleClick(cl,obj,(APTR)msg);
        case MUIM_CManager_GrabLists:          return mGrabLists(cl,obj,(APTR)msg);
        case MUIM_CManager_ReinsertLists:      return mReinsertLists(cl,obj,(APTR)msg);
        case MUIM_CManager_Import:             return mImportDB(cl,obj,(APTR)msg);
        case MUIM_CManager_Export:             return mExportDB(cl,obj,(APTR)msg);
        case MUIM_CManager_ScanHotlists:       return mScanHotlists(cl,obj,(APTR)msg);

        case MUIM_CManager_Search:             return mSearch(cl,obj,(APTR)msg);
        case MUIM_CManager_PrefsWindow:        return mPrefsWindow(cl,obj,(APTR)msg);
        case MUIM_CManager_LoginWindow:        return mLoginWindow(cl,obj,(APTR)msg);
        case MUIM_CManager_MiniMailWindow:     return mMiniMailWindow(cl,obj,(APTR)msg);

        case MUIM_CManager_DisposeObj:         return mDisposeObj(cl,obj,(APTR)msg);
        case MUIM_CManager_Cleanup:            return mCMCleanup(cl,obj,(APTR)msg);
        case MUIM_CManager_Close:              return mClose(cl,obj,(APTR)msg);

        case MUIM_CManager_RequesterResult:    return mRequesterResult(cl,obj,(APTR)msg);

        default:                               return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***************************************************************************/

ULONG
initMCC(void)
{
    if (lib_Class = MUI_CreateCustomClass(lib_base,MUIC_Group,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        if (lib_flags & BASEFLG_MUI20)
            lib_Class->mcc_Class->cl_ID = lib_name;

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
disposeMCC(void)
{
    MUI_DeleteCustomClass(lib_Class);
    lib_Class = NULL;
}

/***********************************************************************/

