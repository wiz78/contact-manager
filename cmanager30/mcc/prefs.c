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
#include <exec/execbase.h>
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/***********************************************************************/

static struct MUI_CustomClass *listerClass = NULL;
#define listerObject   NewObject(listerClass->mcc_Class,NULL

#define IMAGES 7

#ifdef __MORPHOS__
static STRPTR images[] =
{
    "Options",
    "Locale",
    "TheBar",
    "Macros",
    "Phone",
    "Hotlists",
    "Card",
};
#endif

static ULONG MainRegisterLabelsIDs[] =
{
    Msg_PrefsWin_Settings_Options,
    Msg_PrefsWin_Settings_Locale,
    Msg_PrefsWin_Settings_ButtonsBar,
    Msg_PrefsWin_TitleScripts,
    Msg_PrefsWin_TitleDialer,
    Msg_PrefsWin_TitleHotlists,
    Msg_PrefsWin_TitleCard,
    0
};
static STRPTR MainRegisterLabels[IDSSIZE(MainRegisterLabelsIDs)];

struct listerData
{
    #ifdef __MORPHOS__
    Object *objs[IMAGES];
    APTR   *imgs[IMAGES];
    #endif
    UBYTE  title[256];
    ULONG  flags;
};

enum
{
    LFLG_NoImages = 1<<0,
};

static ULONG
mListerNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Frame,              MUIV_Frame_InputList,
            MUIA_ContextMenu,        NULL,
            TAG_MORE, msg->ops_AttrList))
    {
        #ifdef __MORPHOS__
        struct listerData *data = INST_DATA(cl,obj);
    	UBYTE 		  imagePath[256];
        #endif
    	register int 	  i;

	#ifdef __MORPHOS__
        strcpy(imagePath,lib_CMPath);
    	AddPart(imagePath,DEF_ImagePath,sizeof(imagePath));
        #endif

        for (i = 0; i<IMAGES; i++)
        {
            #ifdef __MORPHOS__
            if (!(data->flags & LFLG_NoImages))
            {
    	        if (!(data->flags & LFLG_NoImages))
                {
                    AddPart(imagePath,images[i],sizeof(imagePath));

                    data->objs[i] = PicObject, MUIA_Pic_File, imagePath, MUIA_Pic_Transparent, TRUE, MUIA_Pic_Preload, TRUE, End;
                    if (!data->objs[i]) data->flags |= LFLG_NoImages;

                    *PathPart(imagePath) = 0;
            	}
            }
            #endif

            DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,i+1,MUIV_NList_Insert_Bottom);
        }
    }

    return (ULONG)obj;
}

#ifdef __MORPHOS__
static ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    struct listerData *data = INST_DATA(cl,obj);
    register int      i;

    for (i = 0; i<IMAGES; i++)
	if (data->objs[i]) MUI_DisposeObject(data->objs[i]);
        else break;

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

static ULONG
mListerSetup(struct IClass *cl,Object *obj,Msg msg)
{
    struct listerData *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    if (!(data->flags & LFLG_NoImages))
    {
    	int i;

        superset(cl,obj,MUIA_NList_Quiet,TRUE);

        for (i = 0; i<IMAGES; i++)
        {
            data->imgs[i] = (APTR)DoSuperMethod(cl,obj,MUIM_NList_CreateImage,data->objs[i],0);
            if (!data->imgs[i])
            {
            	data->flags |= LFLG_NoImages;
                break;
            }
        }

        superset(cl,obj,MUIA_NList_Quiet,FALSE);
    }

    return TRUE;
}

static ULONG
mListerCleanup(struct IClass *cl,Object *obj,Msg msg)
{
    struct listerData *data = INST_DATA(cl,obj);
    int		      i;

    for (i = 0; i<IMAGES; i++)
        if (data->imgs[i])
        {
            DoSuperMethod(cl,obj,MUIM_NList_DeleteImage,data->imgs[i]);
            data->imgs[i] = NULL;
	}
        else break;

    return DoSuperMethodA(cl,obj,msg);
}
#endif

static ULONG
mListerAskMinMax(struct IClass *cl,Object *obj,struct MUIP_AskMinMax *msg)
{
    #ifdef __MORPHOS__
    struct listerData 	       *data = INST_DATA(cl,obj);
    #endif
    struct RastPort            rp;
    struct TextExtent 	       te;
    register struct MUI_MinMax *mi;
    register int	       i;
    register UWORD	       sw, w, iw;

    DoSuperMethodA(cl,obj,(APTR)msg);

    memcpy(&rp,&_screen(obj)->RastPort,sizeof(rp));
    SetFont(&rp,_font(obj));
    
    #ifdef __MORPHOS__
    TextExtent(&rp," ",1,&te);
    sw = te.te_Width;
    #else
    sw = 0;
    #endif

    for (i = w = iw = 0; i<IMAGES; i++)
    {
    	STRPTR s = MainRegisterLabels[i];

        TextExtent(&rp,s,strlen(s),&te);

	if (w<te.te_Width) w = te.te_Width;

        #ifdef __MORPHOS__
        if (!(data->flags & LFLG_NoImages))
        {
            UWORD x;

            x = xget(data->objs[i],MUIA_Pic_Width);
            if (iw<x) iw = x;
	}
    	#endif
    }

    mi = msg->MinMaxInfo;

    mi->MinWidth += sw+w+iw;
    mi->DefWidth = mi->MaxWidth = mi->MinWidth;

    return 0;
}

/***********************************************************************/

static ULONG
mListerConstruct(struct IClass *cl,Object *obj,struct MUIP_NList_Construct *msg)
{
    return (ULONG)msg->entry;
}

static ULONG
mListerDestruct(struct IClass *cl,Object *obj,struct MUIP_NList_Destruct *msg)
{
    return 0;
}

static ULONG
mListerDisplay(struct IClass *cl,Object *obj,struct MUIP_NList_Display *msg)
{
    #ifdef __MORPHOS__
    struct listerData *data = INST_DATA(cl,obj);
    #endif
    ULONG 	      id = (ULONG)msg->entry;
    STRPTR	      s;

    id--;
    s = MainRegisterLabels[id];
    
    #ifdef __MORPHOS__
    if (data->flags & LFLG_NoImages) stccpy(data->title,s,sizeof(data->title));
    else snprintf(data->title,sizeof(data->title),"\33O[%08lx] %s",(ULONG)data->imgs[id],(ULONG)s);

    *msg->strings = data->title;
    #else
    *msg->strings = s;
    #endif

    return 0;
}

M_DISP(listerDispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW: 	   	       return mListerNew(cl,obj,(APTR)msg);
    
        #ifdef __MORPHOS__
        case OM_DISPOSE:       	   return mDispose(cl,obj,(APTR)msg);

        case MUIM_Setup:   	       return mListerSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup: 	       return mListerCleanup(cl,obj,(APTR)msg);
        #endif

        case MUIM_AskMinMax:   	   return mListerAskMinMax(cl,obj,(APTR)msg);

        case MUIM_NList_Construct: return mListerConstruct(cl,obj,(APTR)msg);
        case MUIM_NList_Destruct:  return mListerDestruct(cl,obj,(APTR)msg);
        case MUIM_NList_Display:   return mListerDisplay(cl,obj,(APTR)msg);

        default:     	   	       return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(listerDispatcher)

static ULONG
initListerClass(void)
{
    if (listerClass = MUI_CreateCustomClass(NULL,MUIC_NList,NULL,sizeof(struct listerData),DISP(listerDispatcher)))
    {
        localizeArray(MainRegisterLabels,MainRegisterLabelsIDs);

        return TRUE;
    }

    return FALSE;
}

static void
disposeListerClass(void)
{
    if (listerClass)
    {
    	MUI_DeleteCustomClass(listerClass);
	listerClass = NULL;
    }
}

/**************************************************************************/

struct modemItem
{
    STRPTR name;
    STRPTR init;
    STRPTR dialPrefix;
    STRPTR dialSuffix;
};

struct page
{
    Object *obj;
    ULONG  index;
};

struct data
{
    struct CMPrefs BackPrefs;
    struct CMPrefs *Prefs;
    struct CMPrefs *TestPrefs;

    Object         *App;
    Object         *CMObj;
    Object         *Lister;
    Object         *SettingsLister;
    Object         *DialerLister;

    Object         *Pager;
    struct page    Pages[16];
    ULONG          PagesIndexCounter;

    /* Settings */
    Object         *Country;
    Object         *CountryCode;
    Object         *CodesetName;
    Object         *DoubleClick;
    Object         *GroupsStatus;
    Object         *DelWarning;
    Object         *SilentScan;
    Object         *IgnoreLast;
    Object         *OpenURL;
    Object         *AutoReload;
    Object         *NtCheckChanges;
    Object         *SaveOnExit;
    Object         *SilentExit;
    Object         *SaveOnChanges;
    Object         *EditNew;
    Object         *BoldSections;
    Object         *EmptyNodes;
    Object         *SectionsImages;
    Object         *GroupsImages;
    Object         *ClosedGroups;
    Object         *BoldTitles;
    Object         *NoListBar;
    Object         *RightMouseActive;
    Object         *ManualSort;
    Object         *BarPos;
    Object         *ButtonsPos;
    Object         *ViewMode;
    Object         *LabelPos;
    Object         *Borderless;
    Object         *Sunny;
    Object         *Raised;
    Object         *Scaled;
    Object         *Underscore;
    Object         *BarSpacer;
    #ifndef __MORPHOS__
    Object         *Frame;
    #endif
    Object         *DragBar;

    /* Programs */
    Object         *MailProg;
    Object         *MailScript;
    Object         *MailPath;

    Object         *WWWProg;
    Object         *WWWScript;
    Object         *WWWPath;

    Object         *FTPProg;
    Object         *FTPScript;
    Object         *FTPPath;
    Object         *UseLocal;
    Object         *Local;

    Object         *ChatProg;
    Object         *ChatScript;
    Object         *ChatPath;

    Object         *FaxProg;
    Object         *FaxScript;
    Object         *FaxPath;

    Object         *TelnetProg;
    Object         *TelnetScript;
    Object         *TelnetPath;

    Object         *UseSavedProg;
    Object         *SavedProg;

    /* Hotlists */
    Object         *Voyager;
    Object         *IB;
    Object         *AWeb;
    Object         *Netscape;
    Object         *MD2;
    Object         *SimpleMail;
    Object         *YAM;
    Object         *STRICQ;
    Object         *GoodNews;
    Object         *PINT;
    Object         *DFA;
    Object         *STFax;
    Object         *AmFTP;
    Object         *DOpus;
    Object         *AmIRC;

    /* Dialer */
    Object         *UseDialler;
    Object         *DialProg;
    Object         *SerDevice;
    Object         *DialUnit;
    Object         *DialShared;
    Object         *DialInit;
    Object         *RedialDelay;
    Object         *RedialAttempts;
    Object         *DialPrefix;
    Object         *DialSuffix;
    Object         *ModemList;
    Object         *ModemName;

    Object         *AslArray[32];

#ifdef __MORPHOS__
    struct EmulLibEntry modemsObjStrTrap;
#endif
    struct Hook     modemsObjStrHook;

    /* Card */
    Object         *cardBack;
    Object         *titlesPen;
    Object         *titlesBackPen;
    Object         *URLPen;
    Object         *URLOverPen;
    //Object         *cardFont;
    //Object         *titlesFont;
    Object         *showMiniature;
    Object         *miniatureSize;
#ifdef __MORPHOS__
    Object         *miniatureFrame;
#endif

    ULONG          Flags;
};

enum
{
    FLG_Page      = 1<<0,
    FLG_TestPrefs = 1<<1,
};

/***********************************************************************/

static void
prefsToGadgets(struct data *data,struct CMPrefs *prefs,ULONG page,ULONG all)
{
    if (((all || page==MUIV_PrefsWindow_ActivePage_Options)) && data->Pages[MUIV_PrefsWindow_ActivePage_Options].obj)
    {
        /* Options */
        nnset(data->DelWarning,MUIA_Slider_Level,prefs->DelWarning);
        nnset(data->DoubleClick,MUIA_Cycle_Active,prefs->Action);
        nnset(data->GroupsStatus,MUIA_Cycle_Active,prefs->GroupsStatus);
        nnset(data->SilentScan,MUIA_Selected,prefs->Options & OPT_SilentScan);
        nnset(data->OpenURL,MUIA_Selected,prefs->Options & OPT_UseOpenURL);
        nnset(data->AutoReload,MUIA_Selected,prefs->Options & OPT_AutoReload);
        nnset(data->SaveOnExit,MUIA_Selected,prefs->Options & OPT_SaveOnExit);
        nnset(data->SilentExit,MUIA_Selected,prefs->Options & OPT_SilentExit);
        nnset(data->SaveOnChanges,MUIA_Selected,prefs->Options & OPT_SaveOnChanges);
        nnset(data->ManualSort,MUIA_Selected,prefs->Options & OPT_ManualSort);
        nnset(data->BoldSections,MUIA_Selected,prefs->Options & OPT_BoldSections);
        nnset(data->EmptyNodes,MUIA_Selected,prefs->Options  & OPT_EmptyNodes);
        nnset(data->SectionsImages,MUIA_Selected,prefs->Options  & OPT_SectionsImages);
        nnset(data->GroupsImages,MUIA_Selected,prefs->Options  & OPT_GroupsImages);
        nnset(data->ClosedGroups,MUIA_Selected,prefs->Options  & OPT_ClosedGroups);
        nnset(data->BoldTitles,MUIA_Selected,prefs->Options & OPT_BoldTitles);
        nnset(data->NoListBar,MUIA_Selected,prefs->Options & OPT_NoListBar);
        nnset(data->RightMouseActive,MUIA_Selected,prefs->Options & OPT_RightMouseActive);
        nnset(data->NtCheckChanges,MUIA_Selected,prefs->Options & OPT_NtCheckChanges);
        nnset(data->EditNew,MUIA_Selected,prefs->Options & OPT_EditNew);
        nnset(data->UseSavedProg,MUIA_Selected,prefs->Options & OPT_UseSavedProg);
        nnset(data->IgnoreLast,MUIA_Selected,prefs->Options & OPT_IgnoreLast);
    }

    if (((all || page==MUIV_PrefsWindow_ActivePage_Locale)) && data->Pages[MUIV_PrefsWindow_ActivePage_Locale].obj)
    {
        /* Locale */
        nnset(data->Country,MUIA_Textinput_Contents,prefs->Country);
        nnset(data->CountryCode,MUIA_Textinput_Contents,prefs->CountryCode);
        nnset(data->CodesetName,MUIA_Textinput_Contents,prefs->CodesetName);
    }

    if (((all || page==MUIV_PrefsWindow_ActivePage_ButtonsBar)) && data->Pages[MUIV_PrefsWindow_ActivePage_ButtonsBar].obj)
    {
        register ULONG v;

        /* TheBar */
        switch (prefs->VisualPrefs.ButtonsBarPrefs.BarPosition)
        {
            case SBPos_Bottom:      v = 1; break;
            case SBPos_Left:        v = 2; break;
            case SBPos_Right:       v = 3; break;
            case SBPos_InnerTop:    v = 4; break;
            case SBPos_InnerBottom: v = 5; break;
            default:                v = 0; break;
        }
        nnset(data->BarPos,MUIA_Cycle_Active,v);
        nnset(data->ButtonsPos,MUIA_Cycle_Active,prefs->VisualPrefs.ButtonsBarPrefs.ButtonsPos);
        nnset(data->ViewMode,MUIA_Cycle_Active,prefs->VisualPrefs.ButtonsBarPrefs.ViewMode);
        nnset(data->LabelPos,MUIA_Cycle_Active,prefs->VisualPrefs.ButtonsBarPrefs.LabelPosition);
        nnset(data->Raised,MUIA_Selected,prefs->VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Raised);
        nnset(data->Borderless,MUIA_Selected,(prefs->VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Borderless) ? TRUE : FALSE);
        nnset(data->Sunny,MUIA_Selected,prefs->VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Sunny);
        nnset(data->Scaled,MUIA_Selected,prefs->VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Scaled);
        nnset(data->Underscore,MUIA_Selected,prefs->VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Underscore);
        nnset(data->BarSpacer,MUIA_Selected,prefs->VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_BarSpacer);
        #ifndef __MORPHOS__
        nnset(data->Frame,MUIA_Selected,prefs->VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_Frame);
        #endif
        nnset(data->DragBar,MUIA_Selected,prefs->VisualPrefs.ButtonsBarPrefs.Flags & SBFLG_DragBar);
    }

    if (((all || page==MUIV_PrefsWindow_ActivePage_Programs)) && data->Pages[MUIV_PrefsWindow_ActivePage_Programs].obj)
    {
        /* Programs */
        nnset(data->MailProg,MUIA_Cycle_Active,prefs->MailProg);
        nnset(data->MailScript,MUIA_Textinput_Contents,prefs->MailScript);
        nnset(data->MailPath,MUIA_Textinput_Contents,prefs->MailPath);

        nnset(data->WWWProg,MUIA_Cycle_Active,prefs->WWWProg);
        nnset(data->WWWScript,MUIA_Textinput_Contents,prefs->WWWScript);
        nnset(data->WWWPath,MUIA_Textinput_Contents,prefs->WWWPath);

        nnset(data->FTPProg,MUIA_Cycle_Active,prefs->FTPProg);
        nnset(data->FTPScript,MUIA_Textinput_Contents,prefs->FTPScript);
        nnset(data->FTPPath,MUIA_Textinput_Contents,prefs->FTPPath);
        nnset(data->UseLocal,MUIA_Selected,prefs->FTPOptions & FTPOPT_UseLocal);
        nnset(data->Local,MUIA_Textinput_Contents,prefs->Local);

        nnset(data->ChatProg,MUIA_Cycle_Active,prefs->ChatProg);
        nnset(data->ChatScript,MUIA_Textinput_Contents,prefs->ChatScript);
        nnset(data->ChatPath,MUIA_Textinput_Contents,prefs->ChatPath);

        nnset(data->TelnetProg,MUIA_Cycle_Active,prefs->TelnetProg);
        nnset(data->TelnetScript,MUIA_Textinput_Contents,prefs->TelnetScript);
        nnset(data->TelnetPath,MUIA_Textinput_Contents,prefs->TelnetPath);

        nnset(data->FaxProg,MUIA_Cycle_Active,prefs->FaxProg);
        nnset(data->FaxScript,MUIA_Textinput_Contents,prefs->FaxScript);
        nnset(data->FaxPath,MUIA_Textinput_Contents,prefs->FaxPath);

        nnset(data->SavedProg,MUIA_String_Contents,prefs->SavedProg);
    }

    if (((all || page==MUIV_PrefsWindow_ActivePage_Dialer)) && data->Pages[MUIV_PrefsWindow_ActivePage_Dialer].obj)
    {
        struct modemItem *item;

        /* Dialer */
        nnset(data->UseDialler,MUIA_Cycle_Active,(prefs->DialOptions & DIALOPT_ExternalDialer) ? 0 : 1);
        nnset(data->SerDevice,MUIA_Textinput_Contents,prefs->SerDevice);
        nnset(data->DialUnit,MUIA_Textinput_Integer, prefs->SerUnit);
        nnset(data->DialShared,MUIA_Selected,prefs->DialOptions & DIALOPT_SerShared);
        nnset(data->DialInit,MUIA_Textinput_Contents,prefs->SerInit);
        nnset(data->DialPrefix,MUIA_Textinput_Contents,prefs->DialPrefix);
        nnset(data->DialSuffix,MUIA_Textinput_Contents,prefs->DialSuffix);
        nnset(data->RedialDelay,MUIA_Slider_Level,prefs->RedialDelay);
        nnset(data->RedialAttempts,MUIA_Slider_Level,prefs->RedialAttempts);
        nnset(data->DialProg,MUIA_Textinput_Contents,prefs->DialProg);
        nnset(data->ModemList,MUIA_NList_Active,prefs->ModemType);
        DoMethod(data->ModemList,MUIM_NList_GetEntry,prefs->ModemType,&item);
        nnset(data->ModemName,MUIA_Text_Contents,item->name);
    }

    if (((all || page==MUIV_PrefsWindow_ActivePage_Hotlists)) && data->Pages[MUIV_PrefsWindow_ActivePage_Hotlists].obj)
    {
        /* Hots */
        nnset(data->IB,MUIA_Textinput_Contents,prefs->Hotlists[HL_IB]);
        nnset(data->Voyager,MUIA_Textinput_Contents,prefs->Hotlists[HL_V]);
        nnset(data->AmFTP,MUIA_Textinput_Contents,prefs->Hotlists[HL_AMFTP]);
        nnset(data->MD2,MUIA_Textinput_Contents,prefs->Hotlists[HL_MD2]);
        nnset(data->STFax,MUIA_Textinput_Contents,prefs->Hotlists[HL_STFAX]);
        nnset(data->SimpleMail,MUIA_Textinput_Contents,prefs->Hotlists[HL_SIMPLEMAIL]);
        nnset(data->YAM,MUIA_Textinput_Contents,prefs->Hotlists[HL_YAM]);
        nnset(data->STRICQ,MUIA_Textinput_Contents,prefs->Hotlists[HL_STRICQ]);
        nnset(data->PINT,MUIA_Textinput_Contents,prefs->Hotlists[HL_PINT]);
        nnset(data->GoodNews,MUIA_Textinput_Contents,prefs->Hotlists[HL_GOODNEWS]);
        nnset(data->DFA,MUIA_Textinput_Contents,prefs->Hotlists[HL_DFA]);
        nnset(data->AWeb,MUIA_Textinput_Contents,prefs->Hotlists[HL_AWEB]);
        nnset(data->AmIRC,MUIA_Textinput_Contents,prefs->Hotlists[HL_AMIRC]);
        nnset(data->DOpus,MUIA_Textinput_Contents,prefs->Hotlists[HL_DOPUS]);
        nnset(data->Netscape,MUIA_Textinput_Contents,prefs->Hotlists[HL_NETSCAPE]);
    }

    if (((all || page==MUIV_PrefsWindow_ActivePage_Card)) && data->Pages[MUIV_PrefsWindow_ActivePage_Card].obj)
    {
        /* Card */
        nnset(data->cardBack,MUIA_Imagedisplay_Spec,&prefs->CardBack);
        nnset(data->titlesPen,MUIA_Pendisplay_Spec,&prefs->TitlesPen);
        nnset(data->titlesBackPen,MUIA_Pendisplay_Spec,&prefs->TitlesBackPen);
        nnset(data->URLPen,MUIA_Pendisplay_Spec,&prefs->URLPen);
        nnset(data->URLOverPen,MUIA_Pendisplay_Spec,&prefs->URLOverPen);
        nnset(data->showMiniature,MUIA_Selected,prefs->CardOptions & COPT_ShowMiniature);
        nnset(data->miniatureSize,MUIA_Slider_Level,prefs->MiniatureSize);
        #ifdef __MORPHOS__
    	if (lib_flags & BASEFLG_MUI20) set(data->miniatureFrame,MUIA_Framedisplay_Spec,&prefs->MiniatureFrame);
        #endif
        //set(data->cardFont,MUIA_Textinput_Contents,prefs->CardFont);
        //set(data->titlesFont,MUIA_Textinput_Contents,prefs->TitlesFont);
    }
}

/***********************************************************************/

static void
gadgetsToPrefs(struct data *data,struct CMPrefs *prefs)
{
    copymem(prefs,data->Prefs,sizeof(*prefs));

    if (data->Pages[MUIV_PrefsWindow_ActivePage_Options].obj)
    {
        prefs->DelWarning   = xget(data->DelWarning,MUIA_Slider_Level);
        prefs->Action       = xget(data->DoubleClick,MUIA_Cycle_Active);
        prefs->GroupsStatus = xget(data->GroupsStatus,MUIA_Cycle_Active);

        prefs->Options = (xget(data->SilentScan,       MUIA_Selected) ? OPT_SilentScan       : 0) |
                         (xget(data->OpenURL,          MUIA_Selected) ? OPT_UseOpenURL       : 0) |
                         (xget(data->AutoReload,       MUIA_Selected) ? OPT_AutoReload       : 0) |
                         (xget(data->SaveOnExit,       MUIA_Selected) ? OPT_SaveOnExit       : 0) |
                         (xget(data->SilentExit,       MUIA_Selected) ? OPT_SilentExit       : 0) |
                         (xget(data->SaveOnChanges,    MUIA_Selected) ? OPT_SaveOnChanges    : 0) |
                         (xget(data->BoldSections,     MUIA_Selected) ? OPT_BoldSections     : 0) |
                         (xget(data->EmptyNodes,       MUIA_Selected) ? OPT_EmptyNodes       : 0) |
                         (xget(data->SectionsImages,   MUIA_Selected) ? OPT_SectionsImages   : 0) |
                         (xget(data->GroupsImages,     MUIA_Selected) ? OPT_GroupsImages     : 0) |
                         (xget(data->ClosedGroups,     MUIA_Selected) ? OPT_ClosedGroups     : 0) |
                         (xget(data->BoldTitles,       MUIA_Selected) ? OPT_BoldTitles       : 0) |
                         (xget(data->NoListBar,        MUIA_Selected) ? OPT_NoListBar        : 0) |
                         (xget(data->RightMouseActive, MUIA_Selected) ? OPT_RightMouseActive : 0) |
                         (xget(data->NtCheckChanges,   MUIA_Selected) ? OPT_NtCheckChanges   : 0) |
                         (xget(data->ManualSort,       MUIA_Selected) ? OPT_ManualSort       : 0) |
                         (xget(data->EditNew,          MUIA_Selected) ? OPT_EditNew          : 0) |
                         (xget(data->UseSavedProg,     MUIA_Selected) ? OPT_UseSavedProg     : 0) |
                         (xget(data->IgnoreLast,       MUIA_Selected) ? OPT_IgnoreLast       : 0);
    }

    if (data->Pages[MUIV_PrefsWindow_ActivePage_Locale].obj)
    {
        getstr(data->Country,prefs->Country);
        getstr(data->CountryCode,prefs->CountryCode);
        getstr(data->CodesetName,prefs->CodesetName);
        prefs->Codeset = CodesetsFindA(prefs->CodesetName,NULL);
    }

    if (data->Pages[MUIV_PrefsWindow_ActivePage_ButtonsBar].obj)
    {
        register ULONG v = 0; //gcc

        switch (xget(data->BarPos,MUIA_Cycle_Active))
        {
            case 0: v = SBPos_Top;         break;
            case 1: v = SBPos_Bottom;      break;
            case 2: v = SBPos_Left;        break;
            case 3: v = SBPos_Right;       break;
            case 4: v = SBPos_InnerTop;    break;
            case 5: v = SBPos_InnerBottom; break;
        }
        prefs->VisualPrefs.ButtonsBarPrefs.BarPosition = v;
        prefs->VisualPrefs.ButtonsBarPrefs.ButtonsPos = xget(data->ButtonsPos,MUIA_Cycle_Active);
        prefs->VisualPrefs.ButtonsBarPrefs.ViewMode = xget(data->ViewMode,MUIA_Cycle_Active);
        prefs->VisualPrefs.ButtonsBarPrefs.LabelPosition = xget(data->LabelPos,MUIA_Cycle_Active);
        prefs->VisualPrefs.ButtonsBarPrefs.Flags = (xget(data->Borderless,MUIA_Selected) ? SBFLG_Borderless : 0) |
                                                   (xget(data->Sunny,MUIA_Selected) ? SBFLG_Sunny  : 0) |
                                                   (xget(data->Raised,MUIA_Selected) ? SBFLG_Raised : 0) |
                                                   (xget(data->Scaled,MUIA_Selected) ? SBFLG_Scaled : 0) |
                                                   (xget(data->Underscore,MUIA_Selected) ? SBFLG_Underscore : 0) |
                                                   (xget(data->BarSpacer,MUIA_Selected) ? SBFLG_BarSpacer : 0)  |
						   #ifndef __MORPHOS__
                                                   (xget(data->Frame,MUIA_Selected) ? SBFLG_Frame : 0) |
                                                   #endif
                                                   (xget(data->DragBar,MUIA_Selected) ? SBFLG_DragBar : 0);
    }

    if (data->Pages[MUIV_PrefsWindow_ActivePage_Programs].obj)
    {
        prefs->MailProg = xget(data->MailProg,MUIA_Cycle_Active);
        getstr(data->MailScript,prefs->MailScript);
        getstr(data->MailPath,prefs->MailPath);

        prefs->WWWProg = xget(data->WWWProg, MUIA_Cycle_Active);
        getstr(data->WWWScript,prefs->WWWScript);
        getstr(data->WWWPath,prefs->WWWPath);

        prefs->FTPProg = xget(data->FTPProg,MUIA_Cycle_Active);
        getstr(data->FTPScript,prefs->FTPScript);
        getstr(data->FTPPath,prefs->FTPPath);
        prefs->FTPOptions = xget(data->UseLocal,MUIA_Selected) ? FTPOPT_UseLocal : 0;
        getstr(data->Local,prefs->Local);

        prefs->ChatProg = xget(data->ChatProg, MUIA_Cycle_Active);
        getstr(data->ChatScript,prefs->ChatScript);
        getstr(data->ChatPath,prefs->ChatPath);

        prefs->TelnetProg = xget(data->TelnetProg, MUIA_Cycle_Active);
        getstr(data->TelnetScript,prefs->TelnetScript);
        getstr(data->TelnetPath,prefs->TelnetPath);

        prefs->FaxProg = xget(data->FaxProg, MUIA_Cycle_Active);
        getstr(data->FaxScript,prefs->FaxScript);
        getstr(data->FaxPath,prefs->FaxPath);

        strcpy(prefs->SavedProg,(STRPTR)xget(data->SavedProg,MUIA_String_Contents));
    }

    if (data->Pages[MUIV_PrefsWindow_ActivePage_Dialer].obj)
    {
        prefs->DialOptions = (xget(data->UseDialler,MUIA_Cycle_Active) ? 0 : DIALOPT_ExternalDialer) |
                            (xget(data->DialShared,MUIA_Selected) ? DIALOPT_SerShared : 0);

        getstr(data->DialProg,prefs->DialProg);
        getstr(data->SerDevice,prefs->SerDevice);
        getstr(data->DialInit,prefs->SerInit);
        getstr(data->DialPrefix,prefs->DialPrefix);
        getstr(data->DialSuffix,prefs->DialSuffix);

        prefs->SerUnit        = xget(data->DialUnit,MUIA_Textinput_Integer);
        prefs->ModemType      = xget(data->ModemList,MUIA_NList_Active);
        prefs->RedialDelay    = xget(data->RedialDelay,MUIA_Slider_Level);
        prefs->RedialAttempts = xget(data->RedialAttempts,MUIA_Slider_Level);
    }

    if (data->Pages[MUIV_PrefsWindow_ActivePage_Hotlists].obj)
    {
        getstr(data->IB,prefs->Hotlists[HL_IB]);
        getstr(data->Voyager,prefs->Hotlists[HL_V]);
        getstr(data->AWeb,prefs->Hotlists[HL_AWEB]);
        getstr(data->Netscape,prefs->Hotlists[HL_NETSCAPE]);
        getstr(data->AmFTP,prefs->Hotlists[HL_AMFTP]);
        getstr(data->MD2,prefs->Hotlists[HL_MD2]);
        getstr(data->STFax,prefs->Hotlists[HL_STFAX]);
        getstr(data->SimpleMail,prefs->Hotlists[HL_SIMPLEMAIL]);
        getstr(data->YAM,prefs->Hotlists[HL_YAM]);
        getstr(data->AmIRC,prefs->Hotlists[HL_AMIRC]);
        getstr(data->DOpus,prefs->Hotlists[HL_DOPUS]);
        getstr(data->DFA,prefs->Hotlists[HL_DFA]);
        getstr(data->PINT,prefs->Hotlists[HL_PINT]);
        getstr(data->GoodNews,prefs->Hotlists[HL_GOODNEWS]);
        getstr(data->STRICQ,prefs->Hotlists[HL_STRICQ]);
    }

    if (data->Pages[MUIV_PrefsWindow_ActivePage_Card].obj)
    {
        APTR ptr;

        get(data->cardBack,MUIA_Imagedisplay_Spec,&ptr);
        if (ptr) stccpy((STRPTR)&prefs->CardBack,ptr,sizeof(prefs->CardBack));

        get(data->titlesPen,MUIA_Pendisplay_Spec,&ptr);
        if (ptr) stccpy((STRPTR)&prefs->TitlesPen,ptr,sizeof(prefs->TitlesPen));

        get(data->titlesBackPen,MUIA_Pendisplay_Spec,&ptr);
        if (ptr) stccpy((STRPTR)&prefs->TitlesBackPen,ptr,sizeof(prefs->TitlesBackPen));

        get(data->URLPen,MUIA_Pendisplay_Spec,&ptr);
        if (ptr) stccpy((STRPTR)&prefs->URLPen,ptr,sizeof(prefs->URLPen));

        get(data->URLOverPen,MUIA_Pendisplay_Spec,&ptr);
        if (ptr) stccpy((STRPTR)&prefs->URLOverPen,ptr,sizeof(prefs->URLOverPen));

        prefs->CardOptions = xget(data->showMiniature,MUIA_Selected) ? COPT_ShowMiniature : 0;
        prefs->MiniatureSize = xget(data->miniatureSize,MUIA_Slider_Level);

        #ifdef __MORPHOS__
    	if (lib_flags & BASEFLG_MUI20)
    	{
            get(data->miniatureFrame,MUIA_Framedisplay_Spec,&ptr);
        	if (ptr) stccpy((STRPTR)&prefs->MiniatureFrame,ptr,sizeof(prefs->MiniatureFrame));
        }
        #endif

        //getstr(data->cardFont,prefs->CardFont);
        //getstr(data->titlesFont,prefs->TitlesFont);
    }
}

/***********************************************************************/

M_HOOK(deviceObjStr,Object *list,Object *str)
{
    STRPTR entry;

    DoMethod(list,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
    set(str,MUIA_Textinput_Contents,entry);

    return 0;
}

/***********************************************************************/

M_HOOK(modemsCon,APTR pool,struct modemItem *item)
{
    register struct modemItem *entry;
    register int              len, nlen, ilen, dplen, dslen;

    len = sizeof(struct modemItem);

    len += (nlen = strlen(item->name)+1);

    if (item->init) len += (ilen = strlen(item->init)+1);
    else ilen = 0;

    if (item->dialPrefix) len += (dplen = strlen(item->dialPrefix)+1);
    else dplen = 0;

    if (item->dialSuffix) len += (dslen = strlen(item->dialSuffix)+1);
    else dslen = 0;

    if (entry = allocVecPooled(pool,len))
    {
        register STRPTR next = (STRPTR)entry+sizeof(struct modemItem);

        memset(entry,0,sizeof(*entry));

        entry->name = next;
        strcpy(entry->name,item->name);
        next += nlen;

        if (ilen)
        {
            entry->init = next;
            strcpy(entry->init,item->init);
            next += ilen;
        }

        if (dplen)
        {
            entry->dialPrefix = next;
            strcpy(entry->dialPrefix,item->dialPrefix);
            next += dplen;
        }

        if (dslen)
        {
            entry->dialSuffix = next;
            strcpy(entry->dialSuffix,item->dialSuffix);
        }
    }

    return (LONG)entry;
}

/***********************************************************************/

M_HOOK(modemsDes,APTR pool,struct modemItem *item)
{
    freeVecPooled(pool,item);

    return 0;
}

/***********************************************************************/

M_HOOK(modemsSort,struct modemItem *item2,struct modemItem *item1)
{
    return strcmp(item1->name,item2->name);
}

/***************************************************************************/

M_HOOK(modemsDisp,STRPTR *array,struct modemItem *item)
{
    *array = item->name;

    return 0;
}

/***********************************************************************/

#ifdef __MORPHOS__
static void modemsObjStrFun(void)
{
    struct Hook *hook = (struct Hook *)REG_A0;
    Object  *list = (Object *)REG_A2;
    Object  *str  = (Object *)REG_A1;

#else
static void SAVEDS ASM modemsObjStrFun(REG(a0,struct Hook *hook),REG(a2,Object *list),REG(a1,Object *str))
{
#endif

    register struct data *data = hook->h_Data;
    struct modemItem     *item;

    DoMethod(list,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&item);
    set(str,MUIA_Text_Contents,item->name ? item->name : (STRPTR)"");
    set(data->DialInit,MUIA_Text_Contents,item->init ? item->init : (STRPTR)"");
    set(data->DialPrefix,MUIA_Text_Contents,item->dialPrefix ? item->dialPrefix : (STRPTR)"ATDT");
    set(data->DialSuffix,MUIA_Text_Contents,item->dialSuffix ? item->dialSuffix : (STRPTR)"\\r");
}

/***********************************************************************/

static ULONG SettingsRegisterLabelsIDs[] =
{
    Msg_PrefsWin_Settings_Options,
    Msg_PrefsWin_Settings_Locale,
    Msg_PrefsWin_Settings_ButtonsBar,
    0
};
static STRPTR SettingsRegisterLabels[IDSSIZE(SettingsRegisterLabelsIDs)];

static ULONG BarPossIDs[] =
{
    Msg_PrefsWin_Settings_BarPositionTop,
    Msg_PrefsWin_Settings_BarPositionBottom,
    Msg_PrefsWin_Settings_BarPositionLeft,
    Msg_PrefsWin_Settings_BarPositionRight,
    Msg_PrefsWin_Settings_BarPositionInnertTop,
    Msg_PrefsWin_Settings_BarPositionInnerBottom,
    0
};
static STRPTR BarPoss[IDSSIZE(BarPossIDs)];

static ULONG ButtonsPossIDs[] =
{
    Msg_PrefsWin_Settings_ButtonsPosition_Left,
    Msg_PrefsWin_Settings_ButtonsPosition_Center,
    Msg_PrefsWin_Settings_ButtonsPosition_Right,
    0
};
static STRPTR ButtonsPoss[IDSSIZE(ButtonsPossIDs)];

static ULONG ViewModesIDs[] =
{
    Msg_PrefsWin_Settings_TextGfx,
    Msg_PrefsWin_Settings_Gfx,
    Msg_PrefsWin_Settings_Text,
    0
};
static STRPTR ViewModes[IDSSIZE(ViewModesIDs)];

static ULONG LabelPosIDs[] =
{
    Msg_PrefsWin_Settings_LabelPosition_Bottom,
    Msg_PrefsWin_Settings_LabelPosition_Top,
    Msg_PrefsWin_Settings_LabelPosition_Right,
    Msg_PrefsWin_Settings_LabelPosition_Left,
    0
};
static STRPTR LabelPos[IDSSIZE(LabelPosIDs)];

static ULONG ActionsIDs[] =
{
    Msg_PrefsWin_Settings_DCEdit,
    Msg_PrefsWin_Settings_DCScript,
    Msg_PrefsWin_Settings_DCScriptQuit,
    Msg_PrefsWin_Settings_DCScriptHide,
    0
};
static STRPTR Actions[IDSSIZE(ActionsIDs)];

static ULONG GroupsStatussIDs[] =
{
    Msg_PrefsWin_Settings_GroupsStatusRemember,
    Msg_PrefsWin_Settings_GroupsStatusOpened,
    Msg_PrefsWin_Settings_GroupsStatusClosed,
    0
};
static STRPTR GroupsStatuss[IDSSIZE(GroupsStatussIDs)];

static ULONG DialerOptionsIDs[] =
{
    Msg_PrefsWin_Dialer_DExternal,
    Msg_PrefsWin_Dialer_DInternal,
    0
};
static STRPTR DialerOptions[IDSSIZE(DialerOptionsIDs)];

static STRPTR SavedProgPlaceHoldersSyms[] =
{
    "%ld",
    NULL
};

static ULONG SavedProgPlaceHoldersIDs[] =
{
    Msg_PrefsWin_Programs_SavedProgPopData,
    0
};
static STRPTR SavedProgPlaceHolders[IDSSIZE(SavedProgPlaceHoldersIDs)];

static STRPTR DialerProgPlaceHoldersSyms[] =
{
    "%s",
    NULL
};

static ULONG DialerProgPlaceHoldersIDs[] =
{
    Msg_PrefsWin_Dialer_ProgramPopNumber,
    0
};
static STRPTR DialerProgPlaceHolders[IDSSIZE(DialerProgPlaceHoldersIDs)];

static STRPTR WWWProgs[] =
{
    "AWeb",
    "Voyager",
    "IBrowse",
    "---",
    NULL
};

static STRPTR WWWScripts[] =
{
    "SendAWeb.rexx",
    "SendV.rexx",
    "SendIB.rexx",
    ""
};

static STRPTR MailProgs[] =
{
    "SimpleMail",
    "YAM",
    "Microdot II",
    "THOR",
    "---",
    NULL
};

static STRPTR MailScripts[] =
{
    "SendSimpleMail.rexx",
    "SendYAM.rexx",
    "SendMD.rexx",
    "SendThor.rexx",
    ""
};

static STRPTR FTPProgs[] =
{
    "AmFTP",
    "DOpus",
    "---",
    NULL
};

static STRPTR FTPScripts[] =
{
    "SendAmFTP.rexx",
    "SendDOpusFTP.rexx",
    ""
};

static STRPTR ChatProgs[] =
{
    "AmIRC",
    "---",
    NULL
};

static STRPTR ChatScripts[] =
{
    "SendAmIRC.rexx",
    ""
};

static STRPTR FaxProgs[] =
{
    "STFax",
    "---",
    NULL
};

static STRPTR FaxScripts[] =
{
    "SendSTFax.rexx",
    ""
};

static STRPTR TelnetProgs[] =
{
    "AmTelnet",
    "---",
    NULL
};

static STRPTR TelnetScripts[] =
{
    "SendAmTelnet.rexx",
    ""
};

#define INNER 4

static Object *
otitle(ULONG id)
{
    return HGroup,
        Child, RectangleObject, MUIA_Rectangle_HBar, TRUE, MUIA_Weight, 10, End,
        Child, TextObject, MUIA_Text_SetMax, TRUE, MUIA_Text_Contents, CM_GetString(id), End,
        Child, RectangleObject, MUIA_Rectangle_HBar, TRUE, End,
    End;
}

static Object *
makePage(Object *obj,struct data *data,ULONG page)
{
    register Object *o;
    register int    asl = 0;

    if (o = data->Pages[page].obj)
    {
        set(data->Pager,MUIA_Group_ActivePage,data->Pages[page].index);

        return o;
    }

    DoMethod(data->Pager,MUIM_Group_InitChange);
    sleepApp(obj,TRUE);

    switch (page)
    {
        case MUIV_PrefsWindow_ActivePage_Options:
            o = VGroup,
                MUIA_InnerLeft,   INNER,
                MUIA_InnerTop,    INNER,
                MUIA_InnerRight,  INNER,
                MUIA_InnerBottom, INNER,
                VirtualFrame,
                MUIA_Background,  MUII_PageBack,

        		Child, HGroup,
                    Child, olabel2(Msg_PrefsWin_Settings_DelWarning),
                    Child, data->DelWarning = DelWarningObject, End,
                End,

		        Child, HGroup,
                    Child, olabel2(Msg_PrefsWin_Settings_DoubleClick),
                    Child, data->DoubleClick = ocycle(Actions,Msg_PrefsWin_Settings_DoubleClick,Msg_PrefsWin_Settings_DoubleClickHelp),
                    Child, olabel2(Msg_PrefsWin_Settings_GroupsStatus),
                    Child, data->GroupsStatus = ocycle(GroupsStatuss,Msg_PrefsWin_Settings_GroupsStatus,Msg_PrefsWin_Settings_GroupsStatusHelp),
                End,

                Child, RootScrollgroupObject,
                    MUIA_Scrollgroup_FreeHoriz, FALSE,
                    MUIA_Scrollgroup_Contents, VirtgroupObject,
                        MUIA_Frame, MUIV_Frame_Virtual,
                        Child, HGroup,
                            Child, ColGroup(2),
                                Child, data->SaveOnExit = ocheck(Msg_PrefsWin_Settings_SaveOnExit,Msg_PrefsWin_Settings_SaveOnExitHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_SaveOnExit),
                                Child, data->SaveOnChanges = ocheck(Msg_PrefsWin_Settings_SaveOnChanges,Msg_PrefsWin_Settings_SaveOnChangesHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_SaveOnChanges),
                                Child, data->SilentExit = ocheck(Msg_PrefsWin_Settings_SilentExit,Msg_PrefsWin_Settings_SilentExitHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_SilentExit),
                                Child, data->AutoReload = ocheck(Msg_PrefsWin_Settings_AutoReload,Msg_PrefsWin_Settings_AutoReloadHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_AutoReload),
                                Child, data->NtCheckChanges = ocheck(Msg_PrefsWin_Settings_NtCheckChanges,Msg_PrefsWin_Settings_NtCheckChangesHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_NtCheckChanges),
                                Child, data->SilentScan = ocheck(Msg_PrefsWin_Settings_SilentScan,Msg_PrefsWin_Settings_SilentScanHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_SilentScan),
                                Child, data->IgnoreLast = ocheck(Msg_PrefsWin_Settings_IgnoreLast,Msg_PrefsWin_Settings_IgnoreLastHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_IgnoreLast),
                                Child, data->ManualSort = ocheck(Msg_PrefsWin_Settings_ManualSort,Msg_PrefsWin_Settings_ManualSortHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_ManualSort),
                                Child, data->ClosedGroups = ocheck(Msg_PrefsWin_Settings_ClosedGroups,Msg_PrefsWin_Settings_ClosedGroupsHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_ClosedGroups),

                                Child, wspace(20),
                                Child, wspace(20),

                                Child, data->OpenURL = ocheck(Msg_PrefsWin_Settings_UseOpenURL,Msg_PrefsWin_Settings_UseOpenURLHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_UseOpenURL),
                                Child, data->EditNew = ocheck(Msg_PrefsWin_Settings_EditNew,Msg_PrefsWin_Settings_EditNewHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_EditNew),

                                Child, wspace(20),
                                Child, wspace(20),

                                Child, data->RightMouseActive = ocheck(Msg_PrefsWin_Settings_RightMouseActive,Msg_PrefsWin_Settings_RightMouseActiveHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_RightMouseActive),
                                Child, data->BoldTitles = ocheck(Msg_PrefsWin_Settings_BoldTitles,Msg_PrefsWin_Settings_BoldTitlesHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_BoldTitles),
                                Child, data->NoListBar = ocheck(Msg_PrefsWin_Settings_NoListBar,Msg_PrefsWin_Settings_NoListBarHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_NoListBar),
                                Child, data->BoldSections = ocheck(Msg_PrefsWin_Settings_BoldSections,Msg_PrefsWin_Settings_BoldSectionsHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_BoldSections),
                                Child, data->EmptyNodes = ocheck(Msg_PrefsWin_Settings_EmptyNodes,Msg_PrefsWin_Settings_EmptyNodesHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_EmptyNodes),
                                Child, data->SectionsImages = ocheck(Msg_PrefsWin_Settings_SectionsImages,Msg_PrefsWin_Settings_SectionsImagesHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_SectionsImages),
                                Child, data->GroupsImages = ocheck(Msg_PrefsWin_Settings_GroupsImages,Msg_PrefsWin_Settings_GroupsImagesHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_GroupsImages),
                            End,
                            Child, HSpace(0),
                        End,
                    End,
                End,
            End;

            DoMethod(data->DelWarning,MUIM_Notify,MUIA_Slider_Level,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->DoubleClick,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->GroupsStatus,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,obj,1,TAG_USER);

            DoMethod(data->SaveOnExit,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->SaveOnChanges,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->SilentExit,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->AutoReload,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->NtCheckChanges,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->SilentScan,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->IgnoreLast,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->ManualSort,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->ClosedGroups,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);

            DoMethod(data->OpenURL,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->EditNew,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);

            DoMethod(data->RightMouseActive,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->BoldTitles,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->NoListBar,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->BoldSections,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->EmptyNodes,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->SectionsImages,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->GroupsImages,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);


            break;

        case MUIV_PrefsWindow_ActivePage_Locale:
        {
            register Object *GetCountry;

            o = VGroup,
                MUIA_InnerLeft,   INNER,
                MUIA_InnerTop,    INNER,
                MUIA_InnerRight,  INNER,
                MUIA_InnerBottom, INNER,
                VirtualFrame,
                MUIA_Background,  MUII_PageBack,

                Child, ColGroup(2),
                    Child, olabel2(Msg_PrefsWin_Settings_Country),
                    Child, data->Country = ostring(80,Msg_PrefsWin_Settings_Country,Msg_PrefsWin_Settings_CountryHelp),

                    Child, olabel2(Msg_PrefsWin_Settings_CountryCode),
                    Child, HGroup,
                        Child, data->CountryCode = ostring(8,Msg_PrefsWin_Settings_CountryCode,Msg_PrefsWin_Settings_CountryCodeHelp),
                        Child, GetCountry = obutton(Msg_PrefsWin_Settings_GetFromLocale,Msg_PrefsWin_Settings_GetFromLocaleHelp),
                    End,

                    Child, olabel2(Msg_PrefsWin_Settings_Codesets),
                    Child, data->CodesetName = PopupCodesetsObject,
                        MUIA_ShortHelp, CM_GetString(Msg_PrefsWin_Settings_CodesetsHelp),
                        MUIA_PopupCodesets_Key, Msg_PrefsWin_Settings_Codesets,
                    End,
                End,
                Child, VSpace(0),
            End;

            if (o)
            {
                if (LocaleBase) DoMethod(GetCountry,MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_PrefsWindow_GetCountry);
                else set(GetCountry,MUIA_Disabled,TRUE);
            }

            break;
        }

        case MUIV_PrefsWindow_ActivePage_ButtonsBar:
            o = VGroup,
                MUIA_InnerLeft,   INNER,
                MUIA_InnerTop,    INNER,
                MUIA_InnerRight,  INNER,
                MUIA_InnerBottom, INNER,
                VirtualFrame,
                MUIA_Background,  MUII_PageBack,

                Child, ColGroup(4),
                    Child, olabel2(Msg_PrefsWin_Settings_BarPosition),
                    Child,data->BarPos = ocycle(BarPoss,Msg_PrefsWin_Settings_BarPosition,Msg_PrefsWin_Settings_BarPositionHelp),
                    Child, olabel2(Msg_PrefsWin_Settings_ButtonsPosition),
                    Child,data->ButtonsPos = ocycle(ButtonsPoss,Msg_PrefsWin_Settings_ButtonsPosition,Msg_PrefsWin_Settings_ButtonsPositionHelp),
                    Child, olabel2(Msg_PrefsWin_Settings_ViewMode),
                    Child,data->ViewMode = ocycle(ViewModes,Msg_PrefsWin_Settings_ViewMode,Msg_PrefsWin_Settings_ViewModeHelp),
                    Child, olabel2(Msg_PrefsWin_Settings_LabelPosition),
                    Child,data->LabelPos = ocycle(LabelPos,Msg_PrefsWin_Settings_LabelPosition,Msg_PrefsWin_Settings_LabelPositionHelp),
                End,

                Child, RootScrollgroupObject,
                    MUIA_Scrollgroup_FreeHoriz, FALSE,
                    MUIA_Scrollgroup_Contents, VirtgroupObject,
                        MUIA_Frame, MUIV_Frame_Virtual,
                        Child, HGroup,
                            Child, ColGroup(2),
                                Child, data->Borderless = ocheck(Msg_PrefsWin_Settings_Borderless,Msg_PrefsWin_Settings_BorderlessHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_Borderless),
                                Child, data->Sunny = ocheck(Msg_PrefsWin_Settings_Sunny,Msg_PrefsWin_Settings_SunnyHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_Sunny),
                                Child, data->Raised = ocheck(Msg_PrefsWin_Settings_Raised,Msg_PrefsWin_Settings_RaisedHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_Raised),
                                Child, data->Scaled = ocheck(Msg_PrefsWin_Settings_Scaled,Msg_PrefsWin_Settings_ScaledHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_Scaled),
                                Child, data->Underscore = ocheck(Msg_PrefsWin_Settings_Underscore,Msg_PrefsWin_Settings_UnderscoreHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_Underscore),
                                Child, data->BarSpacer = ocheck(Msg_PrefsWin_Settings_BarSpacer,Msg_PrefsWin_Settings_BarSpacerHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_BarSpacer),
                				#ifndef __MORPHOS__
                                Child, data->Frame = ocheck(Msg_PrefsWin_Settings_Frame,Msg_PrefsWin_Settings_FrameHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_Frame),
                                #endif
                                Child, data->DragBar = ocheck(Msg_PrefsWin_Settings_DragBar,Msg_PrefsWin_Settings_DragBarHelp),
                                Child, ollabel1(Msg_PrefsWin_Settings_DragBar),
                            End,
                            Child, HSpace(0),
                        End,
                        Child, VSpace(0),
                    End,
                End,
            End;

            if (o)
            {
                set(data->Raised,MUIA_Disabled,TRUE);
                DoMethod(data->Borderless,MUIM_Notify,MUIA_Selected,FALSE,data->Raised,3,MUIM_Set,MUIA_Selected,FALSE);
                DoMethod(data->Borderless,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,data->Raised,3,MUIM_Set,MUIA_Disabled,MUIV_NotTriggerValue);


                DoMethod(data->BarPos,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->ButtonsPos,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->ViewMode,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->LabelPos,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);

                DoMethod(data->Borderless,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->Sunny,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->Raised,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->Scaled,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->Underscore,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->BarSpacer,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
                DoMethod(data->DragBar,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,(ULONG)obj,1,TAG_USER);
            }

            break;

        case MUIV_PrefsWindow_ActivePage_Programs:
        {
            Object *wwwgroup, *mailgroup, *mscript, *mpath, *wscript, *wpath, *lister;

            o = RootScrollgroupObject,
                MUIA_Scrollgroup_FreeHoriz, FALSE,
                    MUIA_Scrollgroup_Contents, VirtgroupObject,
                    MUIA_InnerLeft,   INNER,
                    MUIA_InnerTop,    INNER,
                    MUIA_InnerRight,  INNER,
                    MUIA_InnerBottom, INNER,
                    MUIA_Frame, MUIV_Frame_Virtual,
                    MUIA_Background, MUII_PageBack,

                    Child, otitle(Msg_PrefsWin_Programs_MailerTitle),
                    Child, mailgroup = ColGroup(2),
                        Child, olabel2(Msg_PrefsWin_Programs_Mailer),
                        Child, data->MailProg = ocycle(MailProgs,Msg_PrefsWin_Programs_Mailer,Msg_PrefsWin_Programs_MailerHelp),
                        Child, olabel2(Msg_PrefsWin_Programs_MailerScript),
                        Child, mscript = data->AslArray[asl++] = ofile(&data->MailScript,256,Msg_PrefsWin_Programs_MailerScript,Msg_PrefsWin_Programs_MailerScriptHelp,FALSE),
                        Child, olabel2(Msg_PrefsWin_Programs_MailerPath),
                        Child, mpath = data->AslArray[asl++] = ofile(&data->MailPath,256,Msg_PrefsWin_Programs_MailerPath,Msg_PrefsWin_Programs_MailerPathHelp,FALSE),
                    End,

                    Child, fhspace(),

                    Child, otitle(Msg_PrefsWin_Programs_WWWTitle),
                    Child, wwwgroup = ColGroup(2),
                        Child, olabel2(Msg_PrefsWin_Programs_WWW),
                        Child, data->WWWProg = ocycle(WWWProgs,Msg_PrefsWin_Programs_WWW,Msg_PrefsWin_Programs_WWWHelp),
                        Child, olabel2(Msg_PrefsWin_Programs_WWWScript),
                        Child, wscript = data->AslArray[asl++] = ofile(&data->WWWScript,256,Msg_PrefsWin_Programs_WWWScript,Msg_PrefsWin_Programs_WWWScriptHelp,FALSE),
                        Child, olabel2(Msg_PrefsWin_Programs_WWWPath),
                        Child, wpath = data->AslArray[asl++] = ofile(&data->WWWPath,256,Msg_PrefsWin_Programs_WWWPath,Msg_PrefsWin_Programs_WWWPathHelp,FALSE),
                    End,

                    Child, fhspace(),

                    Child, otitle(Msg_PrefsWin_Programs_FTPTitle),
                    Child, ColGroup(2),
                        Child, olabel2(Msg_PrefsWin_Programs_FTP),
                        Child, data->FTPProg = ocycle(FTPProgs,Msg_PrefsWin_Programs_FTP,Msg_PrefsWin_Programs_FTPHelp),
                        Child, olabel2(Msg_PrefsWin_Programs_FTPScript),
                        Child, data->AslArray[asl++] = ofile(&data->FTPScript,256,Msg_PrefsWin_Programs_FTPScript,Msg_PrefsWin_Programs_FTPScriptHelp,FALSE),
                        Child, olabel2(Msg_PrefsWin_Programs_FTPPath),
                        Child, data->AslArray[asl++] = ofile(&data->FTPPath,256,Msg_PrefsWin_Programs_FTPPath,Msg_PrefsWin_Programs_FTPPathHelp,FALSE),
                        Child, olabel2(Msg_PrefsWin_Programs_FTPLocal),
                        Child, HGroup,
                            MUIA_Group_Spacing, 2,
                            Child, data->UseLocal = ocheck(Msg_PrefsWin_Programs_FTPLocalUse,Msg_PrefsWin_Programs_FTPLocalUseHelp),
                            Child, lister = data->AslArray[asl++] = ofile(&data->Local,256,Msg_PrefsWin_Programs_FTPLocal,Msg_PrefsWin_Programs_FTPLocalHelp,TRUE),
                            //Child, olabel1(Msg_PrefsWin_Programs_FTPLocalUse),
                        End,
                    End,

                    Child, fhspace(),

                    Child, otitle(Msg_PrefsWin_Programs_ChatTitle),
                    Child, ColGroup(2),
                        Child, olabel2(Msg_PrefsWin_Programs_Chat),
                        Child, data->ChatProg = ocycle(ChatProgs,Msg_PrefsWin_Programs_Chat,Msg_PrefsWin_Programs_ChatHelp),
                        Child, olabel2(Msg_PrefsWin_Programs_ChatScript),
                        Child, data->AslArray[asl++] = ofile(&data->ChatScript,256,Msg_PrefsWin_Programs_ChatScript,Msg_PrefsWin_Programs_ChatScriptHelp,FALSE),
                        Child, olabel2(Msg_PrefsWin_Programs_ChatPath),
                        Child, data->AslArray[asl++] = ofile(&data->ChatPath,256,Msg_PrefsWin_Programs_ChatPath,Msg_PrefsWin_Programs_ChatPathHelp,FALSE),
                    End,

                    Child, fhspace(),

                    Child, otitle(Msg_PrefsWin_Programs_TelnetTitle),
                    Child, ColGroup(2),
                        Child, olabel2(Msg_PrefsWin_Programs_Telnet),
                        Child, data->TelnetProg = ocycle(TelnetProgs,Msg_PrefsWin_Programs_Telnet,Msg_PrefsWin_Programs_TelnetHelp),
                        Child, olabel2(Msg_PrefsWin_Programs_TelnetScript),
                        Child, data->AslArray[asl++] = ofile(&data->TelnetScript,256,Msg_PrefsWin_Programs_TelnetScript,Msg_PrefsWin_Programs_TelnetScriptHelp,FALSE),
                        Child, olabel2(Msg_PrefsWin_Programs_TelnetPath),
                        Child, data->AslArray[asl++] = ofile(&data->TelnetPath,256,Msg_PrefsWin_Programs_TelnetPath,Msg_PrefsWin_Programs_TelnetPathHelp,FALSE),
                    End,

                    Child, fhspace(),

                    Child, otitle(Msg_PrefsWin_Programs_FaxTitle),
                    Child, ColGroup(2),
                        Child, olabel2(Msg_PrefsWin_Programs_Fax),
                        Child, data->FaxProg = ocycle(FaxProgs,Msg_PrefsWin_Programs_Fax,Msg_PrefsWin_Programs_FaxHelp),
                        Child, olabel2(Msg_PrefsWin_Programs_FaxScript),
                        Child, data->AslArray[asl++] = ofile(&data->FaxScript,256,Msg_PrefsWin_Programs_FaxScript,Msg_PrefsWin_Programs_FaxScriptHelp,FALSE),
                        Child, olabel2(Msg_PrefsWin_Programs_FaxPath),
                        Child, data->AslArray[asl++] = ofile(&data->FaxPath,256,Msg_PrefsWin_Programs_FaxPath,Msg_PrefsWin_Programs_FaxPathHelp,FALSE),
                    End,

                    Child, fhspace(),

                    Child, otitle(Msg_PrefsWin_Programs_SavedProgTitle),
                    Child, HGroup,
                        Child, olabel2(Msg_PrefsWin_Programs_SavedProg),
                        Child, HGroup,
                            MUIA_ShortHelp, CM_GetString(Msg_PrefsWin_Programs_SavedProgHelp),
                            MUIA_Group_Spacing, 2,
                            Child, data->UseSavedProg = ocheck(Msg_PrefsWin_Programs_SavedProgUse,Msg_PrefsWin_Programs_SavedProgUseHelp),
                            Child, data->SavedProg = PopphObject,
                                MUIA_Popph_Syms,   SavedProgPlaceHoldersSyms,
                                MUIA_Popph_Names,  SavedProgPlaceHolders,
                                MUIA_Popph_Key,    Msg_PrefsWin_Programs_SavedProg,
                                MUIA_Popph_MaxLen, 256,
                                MUIA_Popph_Asl,    TRUE,
                            End,
                            //Child, olabel1(Msg_PrefsWin_Programs_SavedProgUse),
                        End,
                    End,

                End,
            End;

            if (o)
            {
                DoMethod(data->WWWProg,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,obj,4,MUIM_PrefsWindow_SetString,data->WWWScript,WWWScripts,MUIV_TriggerValue);
                DoMethod(data->FTPProg,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,obj,4,MUIM_PrefsWindow_SetString,data->FTPScript,FTPScripts,MUIV_TriggerValue);
                DoMethod(data->MailProg,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,obj,4,MUIM_PrefsWindow_SetString,data->MailScript,MailScripts,MUIV_TriggerValue);
                DoMethod(data->ChatProg,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,obj,4,MUIM_PrefsWindow_SetString,data->ChatScript,ChatScripts,MUIV_TriggerValue);
                DoMethod(data->FaxProg,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,obj,4,MUIM_PrefsWindow_SetString,data->FaxScript,FaxScripts,MUIV_TriggerValue);
                DoMethod(data->TelnetProg,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,obj,4,MUIM_PrefsWindow_SetString,data->TelnetScript,TelnetScripts,MUIV_TriggerValue);
            }

            break;
        }

        case MUIV_PrefsWindow_ActivePage_Dialer:
        {
            register Object *DevList, *PopDev, *PopModem, *bt;

            o = VGroup,
                MUIA_InnerLeft,   INNER,
                MUIA_InnerTop,    INNER,
                MUIA_InnerRight,  INNER,
                MUIA_InnerBottom, INNER,
                VirtualFrame,
                MUIA_Background,  MUII_PageBack,

                Child, HGroup,
                    Child, olabel2(Msg_PrefsWin_Dialer_Dialer),
                    Child, data->UseDialler = ocycle(DialerOptions,Msg_PrefsWin_Dialer_Dialer,Msg_PrefsWin_Dialer_DialerHelp),
                End,

                Child, data->DialerLister = VGroup,
                    MUIA_Frame,          MUIV_Frame_Virtual,
                    MUIA_Group_PageMode, TRUE,

                    /* External */
                    Child, VGroup,
                        Child, ColGroup(2),
                            Child, olabel2(Msg_PrefsWin_Dialer_Program),
                            Child, data->DialProg = PopphObject,
                                MUIA_ShortHelp,    CM_GetString(Msg_PrefsWin_Dialer_ProgramHelp),
                                MUIA_Popph_Syms,   DialerProgPlaceHoldersSyms,
                                MUIA_Popph_Names,  DialerProgPlaceHolders,
                                MUIA_Popph_Key,    Msg_PrefsWin_Dialer_Program,
                                MUIA_Popph_MaxLen, 256,
                                MUIA_Popph_Asl,    TRUE,
                            End,
                        End,
                        Child, VSpace(0),
                    End, /* External */

                    /* Internal */
                    Child, VGroup,

                        Child, ColGroup(2),
                            Child, olabel2(Msg_PrefsWin_Dialer_Device),
                            Child, HGroup,
                                Child, PopDev = PopobjectObject,
                                    MUIA_ShortHelp,            CM_GetString(Msg_PrefsWin_Dialer_DeviceHelp),
                                    MUIA_Popstring_String,     data->SerDevice = ostring(256,Msg_PrefsWin_Dialer_Device,0),
                                    MUIA_Popstring_Button,     PopButton(MUII_PopUp),
                                    MUIA_Popobject_ObjStrHook, &deviceObjStr_hook,
                                    MUIA_Popobject_Object,     NListviewObject,
                                        MUIA_NListview_NList, DevList = NListObject,
                                            MUIA_ContextMenu,         NULL,
                                            MUIA_NList_ConstructHook, MUIV_NList_ConstructHook_String,
                                            MUIA_NList_DestructHook,  MUIV_NList_DestructHook_String,
                                        End,
                                    End,
                                End,

                                Child, olabel2(Msg_PrefsWin_Dialer_Unit),
                                Child, data->DialUnit = ointeger(3,Msg_PrefsWin_Dialer_Unit,Msg_PrefsWin_Dialer_UnitHelp),
                                Child, olabel1(Msg_PrefsWin_Dialer_Shared),
                                Child, data->DialShared = ocheck(Msg_PrefsWin_Dialer_Shared,Msg_PrefsWin_Dialer_SharedHelp),
                            End,

                            Child, olabel2(Msg_PrefsWin_Dialer_ModemType),
                            Child, PopModem = PopobjectObject,
                                MUIA_ShortHelp,        CM_GetString(Msg_PrefsWin_Dialer_ModemTypeHelp),
                                MUIA_Popstring_String, data->ModemName = TextObject,
                                    TextFrame,
                                    MUIA_Background,   MUII_TextBack,
                                End,
                                MUIA_Popstring_Button,     bt = PopButton(MUII_PopUp),
                                MUIA_Popobject_Object,     NListviewObject,
                                    MUIA_NListview_NList, data->ModemList = NListObject,
                                        MUIA_ContextMenu,         NULL,
                                        MUIA_NList_ConstructHook, &modemsCon_hook,
                                        MUIA_NList_DestructHook,  &modemsDes_hook,
                                        MUIA_NList_CompareHook,   &modemsSort_hook,
                                        MUIA_NList_DisplayHook,   &modemsDisp_hook,
                                    End,
                                End,
                            End,

                            Child, olabel2(Msg_PrefsWin_Dialer_ModemInit),
                            Child, data->DialInit = ostring(256,Msg_PrefsWin_Dialer_ModemInit,Msg_PrefsWin_Dialer_ModemInitHelp),

                            Child, olabel2(Msg_PrefsWin_Dialer_DialPrefix),
                            Child, data->DialPrefix = ostring(30,Msg_PrefsWin_Dialer_DialPrefix,Msg_PrefsWin_Dialer_DialPrefixHelp),

                            Child, olabel2(Msg_PrefsWin_Dialer_DialSuffix),
                            Child, data->DialSuffix = ostring(30,Msg_PrefsWin_Dialer_DialSuffix,Msg_PrefsWin_Dialer_DialSuffixHelp),

                            Child, olabel2(Msg_PrefsWin_Dialer_RedialDelay),
                            Child, data->RedialDelay = oslider(Msg_PrefsWin_Dialer_RedialDelay,Msg_PrefsWin_Dialer_RedialDelayHelp,0,120),

                            Child, olabel2(Msg_PrefsWin_Dialer_RedialAttempts),
                            Child, data->RedialAttempts = oslider(Msg_PrefsWin_Dialer_RedialAttempts,Msg_PrefsWin_Dialer_RedialAttemptsHelp,0,99),

                        End,
                        Child, VSpace(0),
                    End, /* Internal */
                End,
            End;

            if (o)
            {
                REGARRAY UBYTE       buf[512];
                register BPTR        lock;
                register struct Node *dev;

                /* Fill the device list */
                if (lock = Lock("DEVS:",ACCESS_READ))
                {
                    register struct AnchorPath *anchorpath;
                    register BPTR              OldDir;

                    OldDir = CurrentDir(lock);

                    if (anchorpath = (struct AnchorPath *)allocArbitratePooled(sizeof(struct AnchorPath)))
                    {
                        register ULONG error;

                        memset(anchorpath,0,sizeof(*anchorpath));

                        error = MatchFirst("#?.device",anchorpath);

                        while (error==0)
                        {
                            DoMethod(DevList,MUIM_NList_InsertSingle,anchorpath->ap_Info.fib_FileName,MUIV_NList_Insert_Sorted);
                            error = MatchNext(anchorpath);
                        }

                        MatchEnd(anchorpath);
                        freeArbitratePooled(anchorpath,sizeof(struct AnchorPath));
                    }

                    CurrentDir(OldDir);
                    UnLock(lock);
                }

                /* Add the resident devices */
                Forbid();

                for (dev = SysBase->DeviceList.lh_Head; dev->ln_Succ; dev = dev->ln_Succ)
                {
                    register ULONG  add;
                    register ULONG i;

                    add = TRUE;

                    for (i = 0; ; i++)
                    {
                        STRPTR  entry;

                        DoMethod(DevList,MUIM_NList_GetEntry,i,&entry);
                        if (entry==NULL) break;

                        if (strcmp(entry,dev->ln_Name)==0)
                        {
                            add = FALSE;
                            break;
                        }
                    }

                    if (add) DoMethod(DevList,MUIM_NList_InsertSingle,dev->ln_Name,MUIV_NList_Insert_Sorted);
                }

                Permit();

                /* Fill the modem type list */

                #ifdef __MORPHOS__
            	data->modemsObjStrTrap.Trap      = TRAP_LIB;
            	data->modemsObjStrTrap.Extension = 0;
            	data->modemsObjStrTrap.Func      = (APTR)modemsObjStrFun;

            	data->modemsObjStrHook.h_Entry   = (HOOKFUNC)&data->modemsObjStrTrap;
                #else
                data->modemsObjStrHook.h_Entry = (HOOKFUNC)modemsObjStrFun;
                #endif
                data->modemsObjStrHook.h_Data  = data;
                set(PopModem,MUIA_Popobject_ObjStrHook,&data->modemsObjStrHook);

                strcpy(buf,lib_CMPath);
                AddPart(buf,DEF_ModemsFile,sizeof(buf));
                if (lock = Open(buf,MODE_OLDFILE))
                {
                    struct modemItem *generic = NULL;

                    while (FGets(lock,buf,sizeof(buf)-1))
                    {
                        struct RDArgs    ra;
                        struct modemItem item = {0};
                        REGARRAY UBYTE   wbuf[512];

                        if (*buf=='#' || *buf=='\n') continue;

                        ra.RDA_Source.CS_Buffer  = buf;
                        ra.RDA_Source.CS_Length  = strlen(buf);
                        ra.RDA_Source.CS_CurChr  = 0;
                        ra.RDA_DAList            = NULL;
                        ra.RDA_Buffer            = wbuf;
                        ra.RDA_BufSiz            = sizeof(wbuf);
                        ra.RDA_Flags             = RDAF_NOALLOC|RDAF_NOPROMPT;
                        if (ReadArgs("NAME/A,INIT,DIALPREFIX,DIALSUFFIX",(LONG *)&item,&ra))
                        {
                            DoMethod(data->ModemList,MUIM_NList_InsertSingle,&item,MUIV_NList_Insert_Sorted);

                            if (!stricmp(item.name,"Generic"))
                            {
                                LONG ip;

                                get(data->ModemList,MUIA_NList_InsertPosition,&ip);
                                if (ip>=0) DoMethod(data->ModemList,MUIM_NList_GetEntry,ip,&generic);
                            }
                        }
                    }

                    if (generic)
                    {
                        LONG gpos = MUIV_NList_GetPos_Start;

                        DoMethod(data->ModemList,MUIM_NList_GetPos,generic,&gpos);
                        if (gpos>=0) DoMethod(data->ModemList,MUIM_NList_Move,gpos,MUIV_NList_Move_Top);
                    }

                    Close(lock);
                }
                else
                {
                    struct modemItem item;

                    item.name = "Generic";
                    item.init = "AT&F&D2\r";
                    item.dialPrefix = "ATDT";
                    item.dialSuffix = "\\r";

                    DoMethod(data->ModemList,MUIM_NList_InsertSingle,&item,MUIV_NList_Insert_Top);
                }

                set(data->DialUnit,MUIA_Weight,20);
                set(bt,MUIA_ControlChar,getKeyChar(CM_GetString(Msg_PrefsWin_Dialer_ModemType)));

                DoMethod(data->UseDialler,MUIM_Notify,MUIA_Cycle_Active,MUIV_EveryTime,data->DialerLister,3,MUIM_Set,MUIA_Group_ActivePage,MUIV_TriggerValue);
                DoMethod(DevList,MUIM_Notify,MUIA_NList_DoubleClick,MUIV_EveryTime,PopDev,2,MUIM_Popstring_Close,TRUE);
                DoMethod(data->ModemList,MUIM_Notify,MUIA_NList_DoubleClick,MUIV_EveryTime,PopModem,2,MUIM_Popstring_Close,TRUE);
            }

            break;
        }

        case MUIV_PrefsWindow_ActivePage_Hotlists:
        {
            Object *hotsFill, *hotsClear;

            o = VGroup,
                MUIA_InnerLeft,   INNER,
                MUIA_InnerTop,    INNER,
                MUIA_InnerRight,  INNER,
                MUIA_InnerBottom, INNER,
                VirtualFrame,
                MUIA_Background,  MUII_PageBack,

                Child, RootScrollgroupObject,
                    MUIA_Scrollgroup_FreeHoriz, FALSE,
                    MUIA_Scrollgroup_Contents, VirtgroupObject,
                        MUIA_Frame, MUIV_Frame_Virtual,
                        MUIA_Background, MUII_PageBack,

                        Child, ColGroup(2),

                            /* WWW */
                            Child, olabel2(Msg_PrefsWin_Hotlists_V),
                            Child, data->AslArray[asl++] = ofile(&data->Voyager,256,Msg_PrefsWin_Hotlists_V,Msg_PrefsWin_Hotlists_VHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_IB),
                            Child, data->AslArray[asl++] = ofile(&data->IB,256,Msg_PrefsWin_Hotlists_IB,Msg_PrefsWin_Hotlists_IBHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_AWeb),
                            Child, data->AslArray[asl++] = ofile(&data->AWeb,256,Msg_PrefsWin_Hotlists_AWeb,Msg_PrefsWin_Hotlists_AWebHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_Netscape),
                            Child, data->AslArray[asl++] = ofile(&data->Netscape,256,Msg_PrefsWin_Hotlists_Netscape,Msg_PrefsWin_Hotlists_NetscapeHelp,FALSE),

                            Child, wspace(20),
                            Child, wspace(20),

                            /* Users */
                            Child, olabel2(Msg_PrefsWin_Hotlists_SimpleMail),
                            Child, data->AslArray[asl++] = ofile(&data->SimpleMail,256,Msg_PrefsWin_Hotlists_SimpleMail,Msg_PrefsWin_Hotlists_SimpleMailHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_MD2),
                            Child, data->AslArray[asl++] = ofile(&data->MD2,256,Msg_PrefsWin_Hotlists_MD2,Msg_PrefsWin_Hotlists_MD2Help,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_YAM),
                            Child, data->AslArray[asl++] = ofile(&data->YAM,256,Msg_PrefsWin_Hotlists_YAM,Msg_PrefsWin_Hotlists_YAMHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_STRICQ),
                            Child, data->AslArray[asl++] = ofile(&data->STRICQ,256,Msg_PrefsWin_Hotlists_STRICQ,Msg_PrefsWin_Hotlists_STRICQHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_GoodNews),
                            Child, data->AslArray[asl++] = ofile(&data->GoodNews,256,Msg_PrefsWin_Hotlists_GoodNews,Msg_PrefsWin_Hotlists_GoodNewsHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_PINT),
                            Child, data->AslArray[asl++] = ofile(&data->PINT,256,Msg_PrefsWin_Hotlists_PINT,Msg_PrefsWin_Hotlists_PINTHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_DFA),
                            Child, data->AslArray[asl++] = ofile(&data->DFA,256,Msg_PrefsWin_Hotlists_DFA,Msg_PrefsWin_Hotlists_DFAHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_STFax),
                            Child, data->AslArray[asl++] = ofile( &data->STFax, 256, Msg_PrefsWin_Hotlists_STFax,Msg_PrefsWin_Hotlists_STFaxHelp,FALSE),

                            Child, wspace(20),
                            Child, wspace(20),

                            /* FTP */
                            Child, olabel2(Msg_PrefsWin_Hotlists_AmFTP),
                            Child, data->AslArray[asl++] = ofile(&data->AmFTP,256,Msg_PrefsWin_Hotlists_AmFTP,Msg_PrefsWin_Hotlists_AmFTPHelp,FALSE),

                            Child, olabel2(Msg_PrefsWin_Hotlists_DOpus),
                            Child, data->AslArray[asl++] = ofile(&data->DOpus,256,Msg_PrefsWin_Hotlists_DOpus,Msg_PrefsWin_Hotlists_DOpusHelp,FALSE),

                            Child, wspace(20),
                            Child, wspace(20),

                            /* IRC */
                            Child, olabel2(Msg_PrefsWin_Hotlists_AmIRC),
                            Child, data->AslArray[asl] = ofile(&data->AmIRC,256,Msg_PrefsWin_Hotlists_AmIRC,Msg_PrefsWin_Hotlists_AmIRCHelp,FALSE),
                        End,
                    End,
                End,
                Child, HGroup,
                    Child, hotsFill = obutton(Msg_PrefsWin_HotsFill,Msg_PrefsWin_HotsFillHelp),
                    Child, wspace(300),
                    Child, hotsClear = obutton(Msg_PrefsWin_HotsClear,Msg_PrefsWin_HotsClearHelp),
                End,
            End;

            if (o)
            {
                DoMethod(hotsFill,MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_PrefsWindow_HotsFill);
                DoMethod(hotsClear,MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_PrefsWindow_HotsClear);
            }

            break;
        }

        case MUIV_PrefsWindow_ActivePage_Card:
        {
            o = VGroup,
                MUIA_InnerLeft,   INNER,
                MUIA_InnerTop,    INNER,
                MUIA_InnerRight,  INNER,
                MUIA_InnerBottom, INNER,
                VirtualFrame,
                MUIA_Background,  MUII_PageBack,

                /*Child, ColGroup(2),
                    Child, olabel2(Msg_PrefsWin_CardFont),
                    Child, data->AslArray[asl++] = ofont(&data->cardFont,256,Msg_PrefsWin_CardFont,Msg_PrefsWin_CardFontHelp),
                    Child, olabel2(Msg_PrefsWin_TitlesFont),
                    Child, data->AslArray[asl++] = ofont(&data->titlesFont,256,Msg_PrefsWin_TitlesFont,Msg_PrefsWin_TitlesFontHelp),
                End,*/

                Child, ColGroup(4),

                    Child, oflabel(Msg_PrefsWin_TitlesPen),
                    Child, data->titlesPen = opoppen(Msg_PrefsWin_TitlesPen,Msg_PrefsWin_TitlesPenTitle,Msg_PrefsWin_TitlesPenHelp),

                    Child, oflabel(Msg_PrefsWin_TitlesBackPen),
                    Child, data->titlesBackPen = opoppen(Msg_PrefsWin_TitlesBackPen,Msg_PrefsWin_TitlesBackPenTitle,Msg_PrefsWin_TitlesBackPenHelp),

                    Child, oflabel(Msg_PrefsWin_URLPen),
                    Child, data->URLPen = opoppen(Msg_PrefsWin_URLPen,Msg_PrefsWin_URLPenTitle,Msg_PrefsWin_URLPenHelp),

                    Child, oflabel(Msg_PrefsWin_URLOverPen),
                    Child, data->URLOverPen = opoppen(Msg_PrefsWin_URLOverPen,Msg_PrefsWin_URLOverPenTitle,Msg_PrefsWin_URLOverPenHelp),

                    Child, oflabel(Msg_PrefsWin_CardBack),
                    Child, data->cardBack = opopimage(MUIV_Imageadjust_Type_Background,Msg_PrefsWin_CardBack,Msg_PrefsWin_CardBackTitle,Msg_PrefsWin_CardBackHelp),

#ifdef __MORPHOS__
                    Child, (lib_flags & BASEFLG_MUI20) ? oflabel(Msg_PrefsWin_MiniatureFrame) : HSpace(0),
                    Child, (lib_flags & BASEFLG_MUI20) ? (data->miniatureFrame = opopframe(Msg_PrefsWin_MiniatureFrame,Msg_PrefsWin_MiniatureFrameTitle,Msg_PrefsWin_MiniatureFrameHelp)) : HSpace(0) ,
#else
		    Child, HSpace(0),
                    Child, HSpace(0),
#endif
                End,

                Child, RectangleObject, MUIA_Rectangle_HBar, TRUE, MUIA_Weight, 0, End,

                Child, HGroup,
                    Child, ollabel1(Msg_PrefsWin_ShowMiniature),
                    Child, data->showMiniature = ocheck(Msg_PrefsWin_ShowMiniature,Msg_PrefsWin_ShowMiniatureHelp),
                    Child, fwspace(),
                    Child, ollabel1(Msg_PrefsWin_MiniatureSize),
                    Child, data->miniatureSize = oslider(Msg_PrefsWin_MiniatureSize,Msg_PrefsWin_MiniatureSizeHelp,32,256),
                End,

                Child, VSpace(0),

            End;

            DoMethod(data->cardBack,MUIM_Notify,MUIA_Imagedisplay_Spec,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->titlesPen,MUIM_Notify,MUIA_Pendisplay_Spec,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->titlesBackPen,MUIM_Notify,MUIA_Pendisplay_Spec,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->URLPen,MUIM_Notify,MUIA_Pendisplay_Spec,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->URLOverPen,MUIM_Notify,MUIA_Pendisplay_Spec,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->showMiniature,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,obj,1,TAG_USER);
            DoMethod(data->miniatureSize,MUIM_Notify,MUIA_Slider_Level,MUIV_EveryTime,obj,1,TAG_USER);
            #ifdef __MORPHOS__
            DoMethod(data->miniatureFrame,MUIM_Notify,MUIA_Framedisplay_Spec,MUIV_EveryTime,obj,1,TAG_USER);
            #endif
            break;
        }

        default:
            o = NULL;
            break;
    }

    if (o)
    {
        data->Pages[page].obj   = o;
        data->Pages[page].index = data->PagesIndexCounter++;
        prefsToGadgets(data,data->Prefs,page,FALSE);

        DoMethod(data->Pager,OM_ADDMEMBER,o);
        set(data->Pager,MUIA_Group_ActivePage,data->Pages[page].index);
    }

    sleepApp(obj,FALSE);
    DoMethod(data->Pager,MUIM_Group_ExitChange);

    return o;
}

/***********************************************************************/

static ULONG
mNew( struct IClass *cl, Object *obj, struct opSet *msg)
{
    struct data             temp;
    register Object         *Save,*Use,*Cancel,*Apply;
    register struct TagItem *attrs = msg->ops_AttrList;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode,      "CMPREFS",
            MUIA_Window_Title,  CM_GetString(Msg_PrefsWin_Title),
            MUIA_Window_ID,     MAKE_ID('c','m','p','r'),
            MUIA_Window_Height, MUIV_Window_Height_Scaled,

            WindowContents, VGroup,

                Child, HGroup,
                    MUIA_Group_Spacing, 4,

                    Child, temp.Lister = NListviewObject,
                        MUIA_NListview_Vert_ScrollBar, MUIV_NListview_VSB_Always,
                        MUIA_NListview_NList, listerObject, End,
                    End,

                    Child, temp.Pager = VGroup,
                        MUIA_Group_PageMode, TRUE,
                    End,
                End,

                Child, HGroup,
                    Child, Save = obutton(Msg_PrefsWin_Save,Msg_PrefsWin_SaveHelp),
                    Child, wspace(30),
                    Child, Use  = obutton(Msg_PrefsWin_Use,Msg_PrefsWin_UseHelp),
                    Child, wspace(30),
                    Child, Apply = obutton(Msg_PrefsWin_Apply,Msg_PrefsWin_ApplyHelp),
                    Child, wspace(30),
                    Child, Cancel = obutton(Msg_PrefsWin_Cancel,Msg_PrefsWin_CancelHelp),
                End,
            End,

            TAG_MORE,attrs))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->App = (Object *)GetTagData(MUIA_CManager_AppObj,NULL,attrs);
        data->CMObj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,attrs);
        data->Prefs = (struct CMPrefs *)GetTagData(MUIA_CManager_Prefs,NULL,attrs);

        SetSuperAttrs(cl,obj,MUIA_Window_DefaultObject,data->Lister,TAG_DONE);

        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,2,MUIM_CManager_WinClose,MUIV_PrefsWindow_Close_Cancel);
        DoMethod(data->Lister,MUIM_Notify,MUIA_NList_Active,MUIV_EveryTime,obj,3,MUIM_Set,MUIA_PrefsWindow_ActivePage,MUIV_TriggerValue);
        DoMethod(Save,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_WinClose,MUIV_PrefsWindow_Close_Save);
        DoMethod(Use,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_WinClose,MUIV_PrefsWindow_Close_Use);
        DoMethod(Apply,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_WinClose,MUIV_PrefsWindow_Close_Apply);
        DoMethod(Cancel,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_WinClose,MUIV_PrefsWindow_Close_Cancel);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mGet( struct IClass *cl, Object *obj, struct opGet *msg)
{
    //register struct data *data = INST_DATA(cl,obj);

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
            case MUIA_PrefsWindow_ActivePage:
                if (tidata==MUIV_PrefsWindow_ActivePage_NoPage)
                    if (!(data->Flags & FLG_Page)) tidata = MUIV_PrefsWindow_ActivePage_Options;
                    else break;

                if (makePage(obj,data,tidata))
                {
                    nnset(data->Lister,MUIA_NList_Active,tidata);
                    data->Flags |= FLG_Page;
                }

                break;

            case MUIA_CManager_Prefs:
                prefsToGadgets(data,(struct CMPrefs *)tidata,0,TRUE);
                break;

            case MUIA_PrefsWindow_BarPosition:
                if (data->Pages[MUIV_PrefsWindow_ActivePage_ButtonsBar].obj)
                {
                    register ULONG v;

                    switch (tag->ti_Data)
                    {
                        case SBPos_Bottom:      v = 1; break;
                        case SBPos_Left:        v = 2; break;
                        case SBPos_Right:       v = 3; break;
                        case SBPos_InnerTop:    v = 4; break;
                        case SBPos_InnerBottom: v = 5; break;
                        default:                v = 0; break;
                    }
                    set(data->BarPos,MUIA_Cycle_Active,v);
                }
                break;
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mSetup( struct IClass *cl, Object *obj, Msg msg)
{
    register struct data    *data = INST_DATA(cl,obj);

    if (!(data->Flags & FLG_Page)) return FALSE;

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mClose( struct IClass *cl, Object *obj, struct MUIP_CManager_WinClose *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (msg->mode!=MUIV_PrefsWindow_Close_Apply)
        superset(cl,obj,MUIA_Window_Open,FALSE);

    if ((data->Flags & FLG_TestPrefs) || (msg->mode!=MUIV_PrefsWindow_Close_Cancel))
    {
        struct CMPrefs prefs, *p;

        sleepApp(obj,TRUE);

        if ((data->Flags & FLG_TestPrefs) && (msg->mode==MUIV_PrefsWindow_Close_Cancel))
            p = &data->BackPrefs;
        else
        {
            gadgetsToPrefs(data,p = &prefs);

            if ((data->Flags & FLG_TestPrefs) && (msg->mode==MUIV_PrefsWindow_Close_Apply))
                copymem(&data->BackPrefs,p,sizeof(data->BackPrefs));
        }

        DoMethod(data->CMObj,MUIM_CManager_SetPrefs,p,((msg->mode==MUIV_PrefsWindow_Close_Save) ? MUIV_CManager_SetPrefs_Save : 0)|MUIV_CManager_SetPrefs_OneWay);
        DoMethod(data->CMObj,MUIM_KillNotify,MUIA_CManager_Changed);
        if (p->Options & OPT_SaveOnChanges) DoMethod(data->CMObj,MUIM_Notify,MUIA_CManager_Changed,TRUE,data->CMObj,3,MUIM_CManager_SaveData,FALSE,NULL);

        sleepApp(obj,FALSE);
    }

    if (msg->mode!=MUIV_PrefsWindow_Close_Apply)
    {
        //DoMethod(data->App,OM_REMMEMBER,obj);
        DoMethod(data->App,MUIM_Application_PushMethod,data->CMObj,2,MUIM_CManager_DisposeObj,obj);
    }

    return 0;
}

/***********************************************************************/

static ULONG
mTestPrefs(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMPrefs       prefs;

    sleepApp(obj,TRUE);

    if (!(data->Flags & FLG_TestPrefs))
    {
        copymem(&data->BackPrefs,data->Prefs,sizeof(data->BackPrefs));
        data->Flags |= FLG_TestPrefs;
    }

    gadgetsToPrefs(data,&prefs);

    DoMethod(data->CMObj,MUIM_CManager_SetPrefs,&prefs,MUIV_CManager_SetPrefs_OneWay);

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/


static ULONG
mGetContry( struct IClass *cl, Object *obj, Msg msg)
{
    struct data   *data = INST_DATA(cl,obj);
    struct Locale *locale;

    if (LocaleBase && (locale = OpenLocale(NULL)))
    {
        register struct CMCountry *c;

        set(data->CountryCode,MUIA_Textinput_Integer,locale->loc_TelephoneCode);

        for (c = (struct CMCountry *)lib_countries.mlh_Head; c->Succ; c = c->Succ)
            if (locale->loc_TelephoneCode == atoi(c->Code))
            {
                set(data->Country,MUIA_Textinput_Contents,c->Country);
                break;
            }

        CloseLocale(locale);
    }

    return 0;
}

/***********************************************************************/

static ULONG
mSetString( struct IClass *cl, Object *obj, struct MUIP_PrefsWindow_SetString *msg)
{
    set(msg->Obj,MUIA_Textinput_Contents,msg->Strings[msg->Index]);

    return 0;
}

/***********************************************************************/

static ULONG
mHotsFill( struct IClass *cl, Object *obj, Msg msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    REGARRAY UBYTE          buf[256];
    register STRPTR         s;
    register struct Process *this;
    register APTR           win;
    register BPTR           lock;

    this = (struct Process *)FindTask(NULL);
    win = this->pr_WindowPtr;
    this->pr_WindowPtr = (APTR)-1;

    /* AWeb */
    if (lock = Lock("AWeb:aweb.hotlist",SHARED_LOCK)) s = "AWeb:aweb.hotlist";
    else if (lock = Lock("AWeb3:aweb.hotlist",SHARED_LOCK)) s = "AWeb3:aweb.hotlist";
         else if (lock = Lock("AWeb_APL:aweb.hotlist",SHARED_LOCK)) s = "AWeb_APL:aweb.hotlist";
              else s = NULL;
    if (s)
    {
        UnLock(lock);
        set(data->AWeb,MUIA_Textinput_Contents,s);
    }

    /* YAM */
    if (lock = Lock("YAM:.addressbook",SHARED_LOCK))
    {
        s = "YAM:.addressbook";
        UnLock(lock);
    }
    else s = NULL;
    set(data->YAM,MUIA_Textinput_Contents,s);

    /* STFax */
    if (lock = Lock("STFax:Phonebook",SHARED_LOCK))
    {
        s = "STFax:Phonebook";
        UnLock(lock);
    }
    else s = NULL;
    set(data->STFax,MUIA_Textinput_Contents,s);

    /* AmIRC */
    if ((GetVar("Vapor/AmIRC_LASTUSEDDIR",buf,sizeof(buf),GVF_GLOBAL_ONLY)>0) && AddPart(buf,"Default.AmIRCfg",sizeof(buf)))
        s = buf;
    else s = NULL;
    set(data->AmIRC,MUIA_Textinput_Contents,s);

    this->pr_WindowPtr = win;

    return 0;
}

/***********************************************************************/

static ULONG
mHotsClear( struct IClass *cl, Object *obj, Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_Textinput_Contents,NULL,
        data->Voyager,
        data->IB,
        data->AWeb,
        data->Netscape,
        data->SimpleMail,
        data->MD2,
        data->YAM,
        data->STRICQ,
        data->GoodNews,
        data->PINT,
        data->DFA,
        data->STFax,
        data->AmFTP,
        data->DOpus,
        data->AmIRC,
        NULL);

    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                          return mNew(cl,obj,(APTR)msg);
        case OM_GET:                          return mGet(cl,obj,(APTR)msg);
        case OM_SET:                          return mSets(cl,obj,(APTR)msg);

        case MUIM_Window_Setup:               return mSetup(cl,obj,(APTR)msg);

        case MUIM_CManager_WinClose:          return mClose(cl,obj,(APTR)msg);
        case TAG_USER:                        return mTestPrefs(cl,obj,(APTR)msg);
        case MUIM_PrefsWindow_GetCountry:     return mGetContry(cl,obj,(APTR)msg);
        case MUIM_PrefsWindow_SetString:      return mSetString(cl,obj,(APTR)msg);
        case MUIM_PrefsWindow_HotsFill:       return mHotsFill(cl,obj,(APTR)msg);
        case MUIM_PrefsWindow_HotsClear:      return mHotsClear(cl,obj,(APTR)msg);

        default:                              return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initPrefsWindow(void)
{
    if (initListerClass())
    {
        if (lib_PrefsWindow = MUI_CreateCustomClass(NULL,NULL,lib_MatherWin,sizeof(struct data),DISP(dispatcher)))
        {
            localizeArray(SettingsRegisterLabels,SettingsRegisterLabelsIDs);
            localizeArray(BarPoss,BarPossIDs),
            localizeArray(ButtonsPoss,ButtonsPossIDs);
            localizeArray(ViewModes,ViewModesIDs);
            localizeArray(LabelPos,LabelPosIDs),
            localizeArray(Actions,ActionsIDs);
            localizeArray(GroupsStatuss,GroupsStatussIDs);
            localizeArray(DialerOptions,DialerOptionsIDs);
            localizeArray(SavedProgPlaceHolders,SavedProgPlaceHoldersIDs);
            localizeArray(DialerProgPlaceHolders,DialerProgPlaceHoldersIDs);

            loadCountries();

            return TRUE;
        }

        disposeListerClass();
    }

    return FALSE;
}

/***********************************************************************/

void
disposePrefsWindow(void)
{
    MUI_DeleteCustomClass(lib_PrefsWindow);
    lib_PrefsWindow = NULL;
    disposeListerClass();
}

/***********************************************************************/
