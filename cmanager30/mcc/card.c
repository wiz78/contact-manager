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
#include <MUI/NFloattext_mcc.h>
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/***********************************************************************/

static struct MUI_CustomClass *titleClass = NULL;
#define titleObject NewObject(titleClass->mcc_Class,NULL

/****************************************************************************/

struct titleData
{
    STRPTR             text;
    LONG               textLen;
    LONG               miny;
    struct MUI_PenSpec *titlesPen;
    struct MUI_PenSpec *titlesBackPen;
    LONG               tpen;
    LONG               bpen;
    ULONG              flags;
};

enum
{
    TFLG_Setup = 1<<0,
};

/***********************************************************************/

static ULONG
mTitleNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperNew(cl,obj,TAG_MORE,msg->ops_AttrList))
    {
        msg->MethodID = OM_SET;
        DoMethodA(obj,(Msg)msg);
        msg->MethodID = OM_NEW;
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mTitleSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct titleData *data = INST_DATA(cl,obj);
    register struct TagItem   *tag;
    struct TagItem            *tstate;
    register ULONG            redraw = FALSE, res;

    for (tstate = msg->ops_AttrList; tag = NextTagItem(&tstate); )
    {
        register ULONG tidata = tag->ti_Data;

        #define PREFS(t) ((struct CMPrefs *)(t))

        switch (tag->ti_Tag)
        {
            case MUIA_Text_Contents:
                if (data->text = (STRPTR)tidata) data->textLen = strlen((STRPTR)tidata);
                if (data->flags & TFLG_Setup) redraw = TRUE;
                break;

            case MUIA_CManager_Prefs:
                data->titlesPen     = &PREFS(tidata)->TitlesPen;
                data->titlesBackPen = &PREFS(tidata)->TitlesBackPen;

                if (data->flags & TFLG_Setup)
                {
                    MUI_ReleasePen(muiRenderInfo(obj),data->bpen);
                    MUI_ReleasePen(muiRenderInfo(obj),data->tpen);

                    data->tpen = MUI_ObtainPen(muiRenderInfo(obj),data->titlesPen,0);
                    data->bpen = MUI_ObtainPen(muiRenderInfo(obj),data->titlesBackPen,0);

                    redraw = TRUE;
                }

                break;
        }
    }

    res = DoSuperMethodA(cl,obj,(APTR)msg);

    if (redraw)
    {
        Object *p;

        superget(cl,obj,MUIA_Parent,&p);

        MUI_Redraw(obj,MADF_DRAWOBJECT);
    }

    return res;
}

/***********************************************************************/

static ULONG
mTitleSetup(struct IClass *cl,Object *obj,struct MUIP_Setup *msg)
{
    register struct titleData *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,(APTR)msg)) return FALSE;

    data->tpen = MUI_ObtainPen(muiRenderInfo(obj),data->titlesPen,0);
    data->bpen = MUI_ObtainPen(muiRenderInfo(obj),data->titlesBackPen,0);

    data->flags |= TFLG_Setup;

    return TRUE;
}

/***********************************************************************/

static  ULONG
mTitleCleanup(struct IClass *cl,Object *obj,struct MUIP_Setup *msg)
{
    register struct titleData *data = INST_DATA(cl,obj);

    MUI_ReleasePen(muiRenderInfo(obj),data->bpen);
    MUI_ReleasePen(muiRenderInfo(obj),data->tpen);

    data->flags &= ~TFLG_Setup;

    return DoSuperMethodA(cl,obj,(APTR)msg);
}

/***********************************************************************/

#define XDELTA 2
#define YDELTA 2

static ULONG
mTitleAskMinMax(struct IClass *cl,Object *obj,struct MUIP_AskMinMax *msg)
{
    register struct titleData *data = INST_DATA(cl,obj);
    struct RastPort           rp;
    struct TextExtent         te;
    register WORD             w, h;

    DoSuperMethodA(cl,obj,(APTR)msg);

    if (data->text)
    {
        copymem(&rp,&_screen(obj)->RastPort,sizeof(rp));
        SetFont(&rp,_font(obj));

        TextExtent(&rp,data->text,data->textLen,&te);
        data->miny = te.te_Extent.MinY;

        w = te.te_Width+XDELTA;
        h = te.te_Height+YDELTA;
    }
    else
    {
        w = 0;
        h = 0;
    }

    msg->MinMaxInfo->MinWidth  += w;
    msg->MinMaxInfo->MinHeight += h;
    msg->MinMaxInfo->DefWidth  += w;
    msg->MinMaxInfo->DefHeight += h;

    msg->MinMaxInfo->MaxWidth  += MBQ_MUI_MAXMAX;
    msg->MinMaxInfo->MaxHeight += h;

    return 0;
}

/***********************************************************************/

static ULONG
mTitleDraw(struct IClass *cl,Object *obj,struct MUIP_Draw *msg)
{
    register struct titleData *data = INST_DATA(cl,obj);

    DoSuperMethodA(cl,obj,(APTR)msg);

    if (msg->flags & MADF_DRAWOBJECT)
    {
        if (data->text)
        {
            struct RastPort rp;
            register WORD   x = _mleft(obj)+XDELTA, y = _mtop(obj)-data->miny+YDELTA;

            copymem(&rp,_rp(obj),sizeof(rp));

            SetAPen(&rp,MUIPEN(data->bpen));
            RectFill(&rp,_left(obj),_top(obj),_right(obj),_bottom(obj));

            SetFont(&rp,_font(obj));
            SetAPen(&rp,MUIPEN(data->tpen));
            Move(&rp,x,y);
            Text(&rp,data->text,data->textLen);
        }
    }

    return 0;
}

/***********************************************************************/

M_DISP(titleDispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:         return mTitleNew(cl,obj,(APTR)msg);
        case OM_SET:         return mTitleSets(cl,obj,(APTR)msg);
        case MUIM_Setup:     return mTitleSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup:   return mTitleCleanup(cl,obj,(APTR)msg);
        case MUIM_AskMinMax: return mTitleAskMinMax(cl,obj,(APTR)msg);
        case MUIM_Draw:      return mTitleDraw(cl,obj,(APTR)msg);
        default:             return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(titleDispatcher)

/***********************************************************************/

static ULONG
initTitle(void)
{
    return (ULONG)(titleClass = MUI_CreateCustomClass(NULL,MUIC_Area,NULL,sizeof(struct titleData),DISP(titleDispatcher)));
}

/***********************************************************************/

static void
disposeTitle(void)
{
    MUI_DeleteCustomClass(titleClass);
    titleClass = NULL;
}

/***********************************************************************/

struct data
{
    struct CMEntry *entry;

    Object *cmobj;
    Object *vg;
    Object *sg;

    Object *title;

    Object *miniature;

    Object *gcontact;
    Object *gphone;
    Object *ghome;
    Object *gwork;
    Object *gnote;

    Object *contact;
    Object *phone;
    Object *home;
    Object *work;
    Object *note;

    Object *scontact;
    Object *sphone;
    Object *shome;
    Object *swork;
    Object *snote;

    struct CMPrefs       *prefs;
    struct MUI_ImageSpec back;
    ULONG                miniatureSize;
#ifdef __MORPHOS__
    struct MUI_FrameSpec miniatureFrame;
#endif

    APTR   menu;

    ULONG  flags;
};

enum
{
    FLG_Title         = 1<<0,

    FLG_Contact       = 1<<1,
    FLG_Phone         = 1<<2,
    FLG_Home          = 1<<3,
    FLG_Work          = 1<<4,
    FLG_Note          = 1<<5,

    FLG_SContact      = 1<<6,
    FLG_SPhone        = 1<<7,
    FLG_SHome         = 1<<8,
    FLG_SWork         = 1<<9,
    FLG_SNote         = 1<<10,

    FLG_ShowMiniature = 1<<11,
    FLG_FirstPref     = 1<<12,
};

/***********************************************************************/

#define hvspace RectangleObject, MUIA_Weight, 100, End

static ULONG tnMenuIDs[] =
{
    Msg_Card_Menu_Title,
        Msg_Card_Menu_Hide,
    	Msg_Menu_Configure
};

static struct NewMenu tnMenu[] =
{
    MTITLE(Msg_Card_Menu_Title),
        MITEM(Msg_Card_Menu_Hide),
        MITEM(Msg_Menu_Configure),
    MEND
};

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data temp;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode,      "CMCARD",
            MUIA_Group_Spacing, 2,

            MUIA_Scrollgroup_Contents, temp.vg = VirtgroupObject,
                MUIA_Frame,         MUIV_Frame_Virtual,
                MUIA_Group_Spacing, 2,

                Child, temp.sg = ColGroup(2),
                    MUIA_Group_Spacing, 2,

                    Child, temp.gcontact = VGroup,
                        Child, temp.scontact = hvspace,
                    End,

                    Child, temp.gphone = VGroup,
                        Child, temp.sphone = hvspace,
                    End,

                    Child, temp.ghome = VGroup,
                        Child, temp.shome = hvspace,
                    End,

                    Child, temp.gwork = VGroup,
                        Child, temp.swork = hvspace,
                    End,

                    Child, temp.gnote = VGroup,
                        Child, temp.snote = hvspace,
                    End,

                    Child, hvspace,
                End,
            End,

            TAG_MORE,msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->flags |= FLG_SContact|FLG_SPhone|FLG_SHome|FLG_SWork|FLG_SNote;

        data->cmobj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList);

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

    if (data->flags & FLG_Contact)
        MUI_DisposeObject(data->scontact);

    if (data->flags & FLG_Phone)
        MUI_DisposeObject(data->sphone);

    if (data->flags & FLG_Home)
        MUI_DisposeObject(data->shome);

    if (data->flags & FLG_Work)
        MUI_DisposeObject(data->swork);

    if (data->flags & FLG_Note)
        MUI_DisposeObject(data->snote);

    if (data->menu) MUI_DisposeObject(data->menu);

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static Object *
ourl(struct data *data,STRPTR url,STRPTR text)
{
    register Object *o = UrltextObject,
        MUIA_Urltext_Url,                      url,
        text ? MUIA_Urltext_Text : TAG_IGNORE, text,
        MUIA_Urltext_SetMax ,                  FALSE,
    End;

    if (o)
    {
        SetAttrs(o,MUIA_Urltext_MouseOverPen, &data->prefs->URLOverPen,
                   MUIA_Urltext_MouseOutPen,  &data->prefs->URLPen,
                   MUIA_Urltext_VisitedPen,   &data->prefs->URLPen,
                   TAG_DONE);
    }

    return o;
}

/***********************************************************************/

static Object *
ofloat(struct data *data,STRPTR text)
{
    return NFloattextObject,
        NoFrame,
    	//MUIA_List_ScrollerPos, MUIV_List_ScrollerPos_None,
	    MUIA_ContextMenu,      NULL,
        //MUIA_Background,       &data->prefs->CardBack,
        MUIA_Floattext_Text,   text,
    End;

	/*return VGroup,
    	MUIA_Group_Spacing, 0,
        Child, TextObject,
    	    MUIA_Text_Contents,  text,
	    End,
        Child, VSpace(0),
	End;*/
}

/***********************************************************************/

static Object *
otitle(struct data *data,STRPTR text,STRPTR image,ULONG size)
{
    if (image)
    {
        register Object *miniature;

        miniature = PicObject,
#ifdef __MORPHOS__
                (lib_flags & BASEFLG_MUI20) ? TAG_IGNORE : MUIA_InnerLeft,   1,
                (lib_flags & BASEFLG_MUI20) ? TAG_IGNORE : MUIA_InnerTop,    1,
                (lib_flags & BASEFLG_MUI20) ? TAG_IGNORE : MUIA_InnerRight,  1,
                (lib_flags & BASEFLG_MUI20) ? TAG_IGNORE : MUIA_InnerBottom, 1,
                MUIA_Frame, 	        (lib_flags & BASEFLG_MUI20) ? (ULONG)&data->miniatureFrame : MUIV_Frame_String,
#else
                MUIA_InnerLeft,   1,
                MUIA_InnerTop,    1,
                MUIA_InnerRight,  1,
                MUIA_InnerBottom, 1,
                MUIA_Frame, 	        MUIV_Frame_String,
#endif
                MUIA_Pic_File,          image,
                MUIA_Pic_Preload,       TRUE,
                MUIA_Pic_ScaledMaxSize, size,
        End;

        if (data->miniature = miniature)
        {
            return HGroup,
                Child, VGroup,
                    Child, TextObject,
                        MUIA_Text_Contents, text,
                        //MUIA_Background,    &data->prefs->CardBack,
                        MUIA_Font,          MUIV_Font_Big,
                    End,
                    Child, wspace(1),
                End,
                Child, miniature,
            End;
        }
    }

    return TextObject,
		MUIA_Text_Contents, text,
        //MUIA_Background,    &data->prefs->CardBack,
        MUIA_Font,          MUIV_Font_Big,
    End;
}

/***********************************************************************/

static Object *
otitletext(struct data *data,ULONG text)
{
    return titleObject,
        MUIA_Text_Contents,  CM_GetString(text),
        MUIA_CManager_Prefs, data->prefs,
    End;
}

/***********************************************************************/

enum
{
    CHANGE_Contact = 1<<0,
    CHANGE_Phone   = 1<<1,
    CHANGE_Home    = 1<<2,
    CHANGE_Work    = 1<<3,
    CHANGE_Note    = 1<<4,
};

static void
renderCard(struct IClass *cl,Object *obj,struct data *data,struct CMEntry *entry)
{
    register Object *g = NULL;
    register ULONG  changes = 0;

    data->miniature = NULL;

    DoMethod(data->vg,MUIM_Group_InitChange);

    if (data->flags & FLG_Title)
    {
        DoMethod(data->vg,OM_REMMEMBER,data->title);
        MUI_DisposeObject(data->title);

        data->flags &= ~FLG_Title;
    }

    if (data->flags & FLG_Contact)
    {
        DoMethod(data->gcontact,MUIM_Group_InitChange);
        changes |= CHANGE_Contact;

        DoMethod(data->gcontact,OM_REMMEMBER,data->contact);
        MUI_DisposeObject(data->contact);

        data->flags &= ~FLG_Contact;
    }

    if (data->flags & FLG_Phone)
    {
        DoMethod(data->gphone,MUIM_Group_InitChange);
        changes |= CHANGE_Phone;

        DoMethod(data->gphone,OM_REMMEMBER,data->phone);
        MUI_DisposeObject(data->phone);

        data->flags &= ~FLG_Phone;
    }

    if (data->flags & FLG_Home)
    {
        DoMethod(data->ghome,MUIM_Group_InitChange);
        changes |= CHANGE_Home;

        DoMethod(data->ghome,OM_REMMEMBER,data->home);
        MUI_DisposeObject(data->home);

        data->flags &= ~FLG_Home;
    }

    if (data->flags & FLG_Work)
    {
        DoMethod(data->gwork,MUIM_Group_InitChange);
        changes |= CHANGE_Work;

        DoMethod(data->gwork,OM_REMMEMBER,data->work);
        MUI_DisposeObject(data->work);

        data->flags &= ~FLG_Work;
    }

    if (data->flags & FLG_Note)
    {
        DoMethod(data->gnote,MUIM_Group_InitChange);
        changes |= CHANGE_Note;

        DoMethod(data->gnote,OM_REMMEMBER,data->note);
        MUI_DisposeObject(data->note);

        data->flags &= ~FLG_Note;
    }

    if (!entry) goto end;

    switch (entry->Type)
    {
        case CMEntry_User:
        {
            REGARRAY UBYTE  buf[512];
            register Object *o;
            register ULONG  size, len, address;

            snprintf(buf,sizeof(buf),CM_GetString(Msg_Card_Title),CMUSER(entry)->VisualName);
            o = data->title = otitle(data,buf,((data->flags & FLG_ShowMiniature) && *CMUSER(entry)->ImageFile) ? CMUSER(entry)->ImageFile : NULL,data->prefs->MiniatureSize);
            if (!o) goto fail;
            DoMethod(data->vg,OM_ADDMEMBER,o);
            data->flags |= FLG_Title;
            DoMethod(data->vg,MUIM_Group_Sort,o,data->sg,NULL);

            if (*CMUSER(entry)->Name  || *CMUSER(entry)->LastName ||
                *CMUSER(entry)->Alias || *CMUSER(entry)->EMail)
            {
                if (!(changes & CHANGE_Contact))
                {
                    DoMethod(data->gcontact,MUIM_Group_InitChange);
                    changes |= CHANGE_Contact;
                }

                g = data->contact = VGroup,
                    Child, otitletext(data,Msg_Card_Contact),
                End;
                if (!g) goto fail;

                if (*CMUSER(entry)->Name || *CMUSER(entry)->LastName ||
                    *CMUSER(entry)->Alias)
                {
		            register ULONG nl = FALSE;

                    size = sizeof(buf);
                    len = 0;

                    if (*CMUSER(entry)->Name || *CMUSER(entry)->LastName)
                    {
                        if (*CMUSER(entry)->Name) len = snprintf(buf,size,"%s: %s %s",CM_GetString(Msg_Card_Name),CMUSER(entry)->Name,CMUSER(entry)->LastName);
                        else len = snprintf(buf,size,"%s: %s",CM_GetString(Msg_Card_Name),CMUSER(entry)->LastName);
                        nl = TRUE;
                    }

                    if (*CMUSER(entry)->Alias)
                    {
                        if (nl) len += snprintf(buf+len,size-len,"\n");
                    	snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Alias),CMUSER(entry)->Alias);
                    }

                    o = ofloat(data,buf);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (*CMUSER(entry)->EMail)
                {
                    sprintf(buf,"mailto:%s",CMUSER(entry)->EMail);
                    o = ourl(data,buf,CMUSER(entry)->EMail);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (data->flags & FLG_SContact)
                {
                    DoMethod(data->gcontact,OM_REMMEMBER,data->scontact);
                    data->flags &= ~FLG_SContact;
                }

                DoMethod(data->gcontact,OM_ADDMEMBER,g);
                data->flags |= FLG_Contact;
            }

            if (*CMUSER(entry)->Phone  || *CMUSER(entry)->Fax ||
                *CMUSER(entry)->Mobile || *CMUSER(entry)->BPhone ||
                *CMUSER(entry)->BFax   || *CMUSER(entry)->BMobile)
            {
		    	register ULONG nl = FALSE;

                if (!(changes & CHANGE_Phone))
                {
                    DoMethod(data->gphone,MUIM_Group_InitChange);
                    changes |= CHANGE_Phone;
                }

                g = data->phone = VGroup,
                    Child, otitletext(data,Msg_Card_Phone),
                End;
                if (!g) goto fail;

                size = sizeof(buf);
                len = 0;

                if (*CMUSER(entry)->Phone)
                {
                    len = snprintf(buf,size,"%s: %s",CM_GetString(Msg_Card_Phone_Home),CMUSER(entry)->Phone);
                    nl = TRUE;
                }

                if (*CMUSER(entry)->Fax)
                {
                    if (nl) len += snprintf(buf+len,size-len,"\n");
                    len += snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Phone_Fax),CMUSER(entry)->Fax);
                    nl = TRUE;
                }

                if (*CMUSER(entry)->Mobile)
                {
                    if (nl) len += snprintf(buf+len,size-len,"\n");
                    len += snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Phone_Mobile),CMUSER(entry)->Mobile);
                    nl = TRUE;
                }

                if (*CMUSER(entry)->BPhone)
                {
                    if (nl) len += snprintf(buf+len,size-len,"\n");
                    len += snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Phone_Work),CMUSER(entry)->BPhone);
                	nl = TRUE;
                }

                if (*CMUSER(entry)->BFax)
                {
                    if (nl) len += snprintf(buf+len,size-len,"\n");
                    len += snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Phone_WorkFax),CMUSER(entry)->BFax);
                	nl = TRUE;
                }

                if (*CMUSER(entry)->BMobile)
                {
                    if (nl) len += snprintf(buf+len,size-len,"\n");
                    snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Phone_WorkMobile),CMUSER(entry)->BMobile);
                }

                o = ofloat(data,buf);
                if (!o) goto fail;
                DoMethod(g,OM_ADDMEMBER,o);

                if (data->flags & FLG_SPhone)
                {
                    DoMethod(data->gphone,OM_REMMEMBER,data->sphone);
                    data->flags &= ~FLG_SPhone;
                }

                DoMethod(data->gphone,OM_ADDMEMBER,g);
                data->flags |= FLG_Phone;
            }

            address = *CMUSER(entry)->Address || *CMUSER(entry)->City ||
                      *CMUSER(entry)->ZIP     || *CMUSER(entry)->Country;

            if (address || *CMUSER(entry)->WWW)
            {
                if (!(changes & CHANGE_Home))
                {
                    DoMethod(data->ghome,MUIM_Group_InitChange);
                    changes |= CHANGE_Home;
                }

                g = data->home = VGroup,
                    Child, otitletext(data,Msg_Card_Home),
                End;
                if (!g) goto fail;

                if (address)
                {
                    register Object *gm, *b;
                	ULONG			nl = FALSE, zip = FALSE;

                    gm = HGroup,
                        Child, b = TextObject,
                            MUIA_CycleChain,    TRUE,
                            MUIA_Font,          MUIV_Font_Button,
                            MUIA_InputMode,     MUIV_InputMode_RelVerify,
                            ButtonFrame,
                            MUIA_Background,    MUII_ButtonBack,
                            MUIA_Text_Contents, CM_GetString(Msg_Card_GetMap),
                            MUIA_ControlChar,   getKeyCharID(Msg_Card_GetMap),
                            MUIA_Text_HiCharIdx,(ULONG)'_',
                            MUIA_Text_PreParse, MUIX_C,
                            MUIA_Text_SetMax,   TRUE,
                        End,
                        Child, HSpace(0),
                    End;
                    if (!gm) goto fail;

                    size = sizeof(buf);
                    len = 0;

                    if (*CMUSER(entry)->Address)
                    {
                        len = snprintf(buf,size,CMUSER(entry)->Address);
                        nl = TRUE;
                    }

                    if (*CMUSER(entry)->ZIP)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        len += snprintf(buf+len,size-len,"%s",CMUSER(entry)->ZIP);
                        nl = zip = TRUE;
                    }

                    if (*CMUSER(entry)->City)
                    {
	                    if (zip) len += snprintf(buf+len,size-len," ");
                        else if (nl) len += snprintf(buf+len,size-len,"\n");
                        len += snprintf(buf+len,size-len,"%s",CMUSER(entry)->City);
                        nl = TRUE;
                    }

                    if (*CMUSER(entry)->Country)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        snprintf(buf+len,size-len,"%s",CMUSER(entry)->Country);
                        nl = TRUE;
                    }

                    o = HGroup,
                            Child, ofloat(data,buf),
                            Child, VGroup,
                                MUIA_Weight, 0,
                                Child, gm,
                                Child, wspace(1),
                            End,
                    End;
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);

                    DoMethod(b,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CManager_GetMap,MUIV_CManager_GetMap_Home);
                }

                if (*CMUSER(entry)->WWW)
                {
                    register STRPTR u;

                    if (strlen(CMUSER(entry)->WWW)>61)
                    {
                        strncpy(u = buf,CMUSER(entry)->WWW,61);
                        buf[61] = buf[62] = buf[63] = '.';
                        buf[64] = 0;
                    }
                    else u = CMUSER(entry)->WWW;

                    o = ourl(data,CMUSER(entry)->WWW,u);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (data->flags & FLG_SHome)
                {
                    DoMethod(data->ghome,OM_REMMEMBER,data->shome);
                    data->flags &= ~FLG_SHome;
                }

                DoMethod(data->ghome,OM_ADDMEMBER,g);
                data->flags |= FLG_Home;
            }

            address = *CMUSER(entry)->Company     || *CMUSER(entry)->BAddress ||
                      *CMUSER(entry)->BCity       || *CMUSER(entry)->BCountry ||
                      *CMUSER(entry)->BZIP        || *CMUSER(entry)->BPhone ||
                      *CMUSER(entry)->BFax        || *CMUSER(entry)->BJobTitle ||
                      *CMUSER(entry)->BDepartment || *CMUSER(entry)->BOffice ||
                      *CMUSER(entry)->BMobile;

            if (address || *CMUSER(entry)->BEMail)
            {
                if (!(changes & CHANGE_Work))
                {
                    DoMethod(data->gwork,MUIM_Group_InitChange);
                    changes |= CHANGE_Work;
                }

                g = data->work = VGroup,
                    Child, otitletext(data,Msg_Card_Work),
                End;
                if (!g) goto fail;

                if (address)
                {
                    register Object *gm, *b;
                	ULONG			nl = FALSE, zip = FALSE;

                    gm = HGroup,
                        Child, b = TextObject,
                            MUIA_CycleChain,    TRUE,
                            MUIA_Font,          MUIV_Font_Button,
                            MUIA_InputMode,     MUIV_InputMode_RelVerify,
                            ButtonFrame,
                            MUIA_Background,    MUII_ButtonBack,
                            MUIA_Text_Contents, CM_GetString(Msg_Card_GetMap),
                            MUIA_ControlChar,   getKeyCharID(Msg_Card_GetMap),
                            MUIA_Text_HiCharIdx,(ULONG)'_',
                            MUIA_Text_PreParse, MUIX_C,
                            MUIA_Text_SetMax,   TRUE,
                        End,
                        Child, HSpace(0),
                    End;
                    if (!gm) goto fail;

                    size = sizeof(buf);
                    len = 0;

                    if (*CMUSER(entry)->BJobTitle)
                    {
                        len = snprintf(buf,size,CMUSER(entry)->BJobTitle);
                        nl = TRUE;
                    }

                    if (*CMUSER(entry)->BDepartment)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        len += snprintf(buf+len,size-len,"%s",CMUSER(entry)->BDepartment);
                        nl = TRUE;
                    }

                    if (*CMUSER(entry)->Company)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        len += snprintf(buf+len,size-len,"%s",CMUSER(entry)->Company);
                        nl = TRUE;
                    }

                    if (*CMUSER(entry)->BAddress)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        len += snprintf(buf+len,size-len,"%s",CMUSER(entry)->BAddress);
                        nl = TRUE;
                    }

                    if (*CMUSER(entry)->BZIP)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        len += snprintf(buf+len,size-len,"%s",CMUSER(entry)->BZIP);
                        nl = zip = TRUE;
                    }

                    if (*CMUSER(entry)->BCity)
                    {
	                    if (zip) len += snprintf(buf+len,size-len," ");
	                    else if (nl) len += snprintf(buf+len,size-len,"\n");
                        len += snprintf(buf+len,size-len,"%s",CMUSER(entry)->BCity);
                        nl = TRUE;
                    }

                    if (*CMUSER(entry)->BCountry)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        snprintf(buf+len,size-len,"%s",CMUSER(entry)->BCountry);
                        nl = TRUE;
                    }

                    o = HGroup,
                            Child, ofloat(data,buf),
                            Child, VGroup,
                                MUIA_Weight, 0,
                                Child, gm,
                                Child, wspace(1),
                            End,
                    End;
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                    DoMethod(b,MUIM_Notify,MUIA_Pressed,0,obj,2,MUIM_CManager_GetMap,MUIV_CManager_GetMap_Job);
                }

                if (*CMUSER(entry)->BEMail)
                {
                    sprintf(buf,"mailto:%s",CMUSER(entry)->BEMail);
                    o = ourl(data,buf,CMUSER(entry)->BEMail);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (data->flags & FLG_SWork)
                {
                    DoMethod(data->gwork,OM_REMMEMBER,data->swork);
                    data->flags &= ~FLG_SWork;
                }

                DoMethod(data->gwork,OM_ADDMEMBER,g);
                data->flags |= FLG_Work;
            }

            goto end;
        }

        case CMEntry_WWW:
        {
            REGARRAY UBYTE  buf[512];
            register Object *o;
            register ULONG  size, len;

            snprintf(buf,sizeof(buf),CM_GetString(Msg_Card_Title),*CMWWW(entry)->Alias ? CMWWW(entry)->Alias : CMWWW(entry)->Name);
            o = data->title = otitle(data,buf,NULL,0);
            if (!o) goto fail;
            DoMethod(data->vg,OM_ADDMEMBER,o);
            data->flags |= FLG_Title;
            DoMethod(data->vg,MUIM_Group_Sort,o,data->sg,NULL);

            if (*CMWWW(entry)->Name  || *CMWWW(entry)->Alias ||
                *CMWWW(entry)->WWW   || *CMWWW(entry)->EMail)
            {
                if (!(changes & CHANGE_Contact))
                {
                    DoMethod(data->gcontact,MUIM_Group_InitChange);
                    changes |= CHANGE_Contact;
                }

                g = data->contact = VGroup,
                    Child, otitletext(data,Msg_Card_Contact),
                End;
                if (!g) goto fail;

                if (*CMWWW(entry)->Name || *CMWWW(entry)->Alias)
                {
                    ULONG nl = FALSE;

                    size = sizeof(buf);
                    len = 0;

                    if (*CMWWW(entry)->Name)
                    {
                        len = snprintf(buf,size,"%s: %s",CM_GetString(Msg_Card_Name),CMWWW(entry)->Name);
                        nl = TRUE;
                    }

                    if (*CMWWW(entry)->Alias)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Alias),CMWWW(entry)->Alias);
                    }

                    o = ofloat(data,buf);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (*CMWWW(entry)->WWW)
                {
                    register STRPTR u;

                    if (strlen(CMWWW(entry)->WWW)>61)
                    {
                        strncpy(u = buf,CMWWW(entry)->WWW,61);
                        buf[61] = buf[62] = buf[63] = '.';
                        buf[64] = 0;
                    }
                    else u = CMWWW(entry)->WWW;

                    o = ourl(data,CMWWW(entry)->WWW,u);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (*CMWWW(entry)->EMail)
                {
                    sprintf(buf,"mailto:%s",CMWWW(entry)->EMail);
                    o = ourl(data,buf,CMWWW(entry)->EMail);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (data->flags & FLG_SContact)
                {
                    DoMethod(data->gcontact,OM_REMMEMBER,data->scontact);
                    data->flags &= ~FLG_SContact;
                }

                DoMethod(data->gcontact,OM_ADDMEMBER,g);
                data->flags |= FLG_Contact;
            }

            goto end;
        }

        case CMEntry_FTP:
        {
            REGARRAY UBYTE  buf[512];
            register Object *o;
            register ULONG  size, len;

            snprintf(buf,sizeof(buf),CM_GetString(Msg_Card_Title),*CMFTP(entry)->Alias ? CMFTP(entry)->Alias : CMFTP(entry)->Name);
            o = data->title = otitle(data,buf,NULL,0);
            if (!o) goto fail;
            DoMethod(data->vg,OM_ADDMEMBER,o);
            data->flags |= FLG_Title;
            DoMethod(data->vg,MUIM_Group_Sort,o,data->sg,NULL);

            if (*CMFTP(entry)->Name  || *CMFTP(entry)->Alias ||
                *CMFTP(entry)->FTP   || *CMFTP(entry)->EMail)
            {
                if (!(changes & CHANGE_Contact))
                {
                    DoMethod(data->gcontact,MUIM_Group_InitChange);
                    changes |= CHANGE_Contact;
                }

                g = data->contact = VGroup,
                        Child, otitletext(data,Msg_Card_Contact),
                End;
                if (!g) goto fail;

                if (*CMFTP(entry)->Name || *CMFTP(entry)->Alias)
                {
					ULONG nl = FALSE;

                    size = sizeof(buf);
                    len = 0;

                    if (*CMFTP(entry)->Name)
                    {
                        len = snprintf(buf,size,"%s: %s",CM_GetString(Msg_Card_Name),CMFTP(entry)->Name);
                        nl = TRUE;
                    }

                    if (*CMFTP(entry)->Alias)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Alias),CMFTP(entry)->Alias);
                        nl = TRUE;
                    }

                    o = ofloat(data,buf);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (*CMFTP(entry)->FTP)
                {
                    register STRPTR u;

                    if (strlen(CMFTP(entry)->FTP)>61)
                    {
                        strncpy(u = buf,CMFTP(entry)->FTP,61);
                        buf[61] = buf[62] = buf[63] = '.';
                        buf[64] = 0;
                    }
                    else u = CMFTP(entry)->FTP;

                    o = ourl(data,CMFTP(entry)->FTP,u);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (*CMFTP(entry)->EMail)
                {
                    sprintf(buf,"mailto:%s",CMFTP(entry)->EMail);
                    o = ourl(data,buf,CMFTP(entry)->EMail);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (data->flags & FLG_SContact)
                {
                    DoMethod(data->gcontact,OM_REMMEMBER,data->scontact);
                    data->flags &= ~FLG_SContact;
                }

                DoMethod(data->gcontact,OM_ADDMEMBER,g);
                data->flags |= FLG_Contact;
            }

            goto end;
        }

        case CMEntry_Chat:
        {
            REGARRAY UBYTE  buf[512];
            register Object *o;
            register ULONG  size, len;

            snprintf(buf,sizeof(buf),CM_GetString(Msg_Card_Title),CMCHAT(entry)->Channel);
            o = data->title = otitle(data,buf,NULL,0);
            if (!o) goto fail;
            DoMethod(data->vg,OM_ADDMEMBER,o);
            data->flags |= FLG_Title;
            DoMethod(data->vg,MUIM_Group_Sort,o,data->sg,NULL);

            if (*CMCHAT(entry)->Channel || *CMCHAT(entry)->Server ||
                *CMCHAT(entry)->WWW     || *CMCHAT(entry)->EMail)
            {
                if (!(changes & CHANGE_Contact))
                {
                    DoMethod(data->gcontact,MUIM_Group_InitChange);
                    changes |= CHANGE_Contact;
                }

                g = data->contact = VGroup,
                    Child, otitletext(data,Msg_Card_Contact),
                End;
                if (!g) goto fail;

                if (*CMCHAT(entry)->Channel || *CMCHAT(entry)->Server)
                {
                    ULONG nl = FALSE;

                    size = sizeof(buf);
                    len = 0;

                    if (*CMCHAT(entry)->Channel)
                    {
                        len = snprintf(buf,size,"%s: %s",CM_GetString(Msg_Card_Channel),CMCHAT(entry)->Channel);
                    	nl = TRUE;
                    }

                    if (*CMCHAT(entry)->Server)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Server),CMCHAT(entry)->Server);
                    }

                    o = ofloat(data,buf);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (*CMCHAT(entry)->EMail)
                {
                    sprintf(buf,"mailto:%s",CMCHAT(entry)->EMail);
                    o = ourl(data,buf,CMCHAT(entry)->EMail);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (*CMCHAT(entry)->WWW)
                {
                    register STRPTR u;

                    if (strlen(CMCHAT(entry)->WWW)>61)
                    {
                        strncpy(u = buf,CMCHAT(entry)->WWW,61);
                        buf[61] = buf[62] = buf[63] = '.';
                        buf[64] = 0;
                    }
                    else u = CMCHAT(entry)->WWW;

                    o = ourl(data,CMCHAT(entry)->WWW,u);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (data->flags & FLG_SContact)
                {
                    DoMethod(data->gcontact,OM_REMMEMBER,data->scontact);
                    data->flags &= ~FLG_SContact;
                }

                DoMethod(data->gcontact,OM_ADDMEMBER,g);
                data->flags |= FLG_Contact;
            }

            goto end;
        }

        case CMEntry_Telnet:
        {
            REGARRAY UBYTE  buf[512];
            register Object *o;

            snprintf(buf,sizeof(buf),CM_GetString(Msg_Card_Title),CMTELNET(entry)->Alias);
            o = data->title = otitle(data,buf,NULL,0);
            if (!o) goto fail;
            DoMethod(data->vg,OM_ADDMEMBER,o);
            data->flags |= FLG_Title;
            DoMethod(data->vg,MUIM_Group_Sort,o,data->sg,NULL);

            if (*CMTELNET(entry)->Host || *CMTELNET(entry)->EMail || *CMTELNET(entry)->Alias)
            {
                if (!(changes & CHANGE_Contact))
                {
                    DoMethod(data->gcontact,MUIM_Group_InitChange);
                    changes |= CHANGE_Contact;
                }

                g = data->contact = VGroup,
                    Child, otitletext(data,Msg_Card_Contact),
                End;
                if (!g) goto fail;

                if (*CMTELNET(entry)->Host || *CMTELNET(entry)->Alias)
                {
                    register ULONG  size, len;
                    ULONG			nl = FALSE;

                    size = sizeof(buf);
                    len = 0;

                    if (*CMTELNET(entry)->Host)
                    {
                        len = snprintf(buf,sizeof(buf),"%s: %s",CM_GetString(Msg_Card_Host),CMTELNET(entry)->Host);
                    	nl = FALSE;
                    }

                    if (*CMTELNET(entry)->Alias)
                    {
	                    if (nl) len += snprintf(buf+len,size-len,"\n");
                        snprintf(buf+len,size-len,"%s: %s",CM_GetString(Msg_Card_Host),CMTELNET(entry)->Alias);
                    }

                    o = ofloat(data,buf);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (*CMTELNET(entry)->EMail)
                {
                    sprintf(buf,"mailto:%s",CMTELNET(entry)->EMail);
                    o = ourl(data,buf,CMTELNET(entry)->EMail);
                    if (!o) goto fail;
                    DoMethod(g,OM_ADDMEMBER,o);
                }

                if (data->flags & FLG_SContact)
                {
                    DoMethod(data->gcontact,OM_REMMEMBER,data->scontact);
                    data->flags &= ~FLG_SContact;
                }

                DoMethod(data->gcontact,OM_ADDMEMBER,g);
                data->flags |= FLG_Contact;
            }

            goto end;
        }
    }

fail:
    MUI_DisposeObject(g);

end:
    if (!(data->flags & FLG_Contact) && !(data->flags & FLG_SContact))
    {
        if (!(changes & CHANGE_Contact))
        {
            DoMethod(data->gcontact,MUIM_Group_InitChange);
            changes |= CHANGE_Contact;
        }

        DoMethod(data->gcontact,OM_ADDMEMBER,data->scontact);

        data->flags |= FLG_SContact;
    }

    if (!(data->flags & FLG_Phone) && !(data->flags & FLG_SPhone))
    {
        if (!(changes & CHANGE_Phone))
        {
            DoMethod(data->gphone,MUIM_Group_InitChange);
            changes |= CHANGE_Phone;
        }

        DoMethod(data->gphone,OM_ADDMEMBER,data->sphone);

        data->flags |= FLG_SPhone;
    }

    if (!(data->flags & FLG_Home) && !(data->flags & FLG_SHome))
    {
        if (!(changes & CHANGE_Home))
        {
            DoMethod(data->ghome,MUIM_Group_InitChange);
            changes |= CHANGE_Home;
        }

        DoMethod(data->ghome,OM_ADDMEMBER,data->shome);

        data->flags |= FLG_SHome;
    }

    if (!(data->flags & FLG_Work) && !(data->flags & FLG_SWork))
    {
        if (!(changes & CHANGE_Work))
        {
            DoMethod(data->gwork,MUIM_Group_InitChange);
            changes |= CHANGE_Work;
        }

        DoMethod(data->gwork,OM_ADDMEMBER,data->swork);

        data->flags |= FLG_SWork;
    }

    if (data->flags & FLG_Note && !(data->flags & FLG_SNote))
    {
        if (!(changes & CHANGE_Note))
        {
            DoMethod(data->gnote,MUIM_Group_InitChange);
            changes |= CHANGE_Note;
        }

        DoMethod(data->gnote,OM_ADDMEMBER,data->snote);

        data->flags |= FLG_SNote;
    }

    if (changes & CHANGE_Contact) DoMethod(data->gcontact,MUIM_Group_ExitChange);
    if (changes & CHANGE_Phone)   DoMethod(data->gphone,MUIM_Group_ExitChange);
    if (changes & CHANGE_Home)    DoMethod(data->ghome,MUIM_Group_ExitChange);
    if (changes & CHANGE_Work)    DoMethod(data->gwork,MUIM_Group_ExitChange);
    if (changes & CHANGE_Note)    DoMethod(data->gnote,MUIM_Group_ExitChange);

    DoMethod(data->vg,MUIM_Group_ExitChange);
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct TagItem *tag;
    struct TagItem          *tags;
    register ULONG          res, win = FALSE, render = FALSE;

    for (tags = msg->ops_AttrList; tag = NextTagItem(&tags); )
    {
        register ULONG tidata = tag->ti_Data;

        switch (tag->ti_Tag)
        {
            case MUIA_Card_Entry:
                data->entry = (struct CMEntry *)tidata;
                render = TRUE;
                break;

            case MUIA_CManager_Prefs:
                data->prefs = PREFS(tidata);

                if (!(data->flags & FLG_FirstPref))
                {
                    copymem(&data->back,&PREFS(tidata)->CardBack,sizeof(data->back));
                    #ifdef __MORPHOS__
                    copymem(&data->miniatureFrame,&PREFS(tidata)->MiniatureFrame,sizeof(data->miniatureFrame));
                    #endif
                    data->flags |= FLG_FirstPref;
                }
                else
                {
                    if (memcmp(&data->back,&PREFS(tidata)->CardBack,sizeof(data->back)))
                    {
                    	copymem(&data->back,&PREFS(tidata)->CardBack,sizeof(data->back));
                        win = TRUE;
                    }

                    #ifdef __MORPHOS__
                    if (memcmp(&data->miniatureFrame,&PREFS(tidata)->MiniatureFrame,sizeof(data->miniatureFrame)))
                    {
                    	copymem(&data->miniatureFrame,&PREFS(tidata)->MiniatureFrame,sizeof(data->miniatureFrame));
                	    if (data->miniature) win = TRUE;
                    }
                    #endif
                }

                if (!BOOLSAME(data->flags & FLG_ShowMiniature,PREFS(tidata)->CardOptions & COPT_ShowMiniature))
                {
                    if (PREFS(tidata)->CardOptions & COPT_ShowMiniature)
                        data->flags |= FLG_ShowMiniature;
                    else data->flags &= ~FLG_ShowMiniature;

                    render = TRUE;
                }

                if (data->miniatureSize!=PREFS(tidata)->MiniatureSize)
                {
                    data->miniatureSize = PREFS(tidata)->MiniatureSize;

                    render = TRUE;
                }

                SetAttrs(data->vg,MUIA_Urltext_MouseOverPen, &data->prefs->URLOverPen,
                                  MUIA_Urltext_MouseOutPen,  &data->prefs->URLPen,
                                  MUIA_Urltext_VisitedPen,   &data->prefs->URLPen,
                                  TAG_DONE);

                SetAttrs(data->vg,MUIA_Background,&data->prefs->CardBack,
                		          MUIA_Group_Forward, FALSE,
                                  TAG_DONE);

                SetAttrs(obj,MUIA_Background,&data->prefs->CardBack,
                		     MUIA_Group_Forward, FALSE,
                             TAG_DONE);

                render = TRUE;
                break;
        }
    }

    res = DoSuperMethodA(cl,obj,(Msg)msg);

    if (win && !(lib_flags & BASEFLG_MUI4)) DoMethod(_app(obj),MUIM_Application_PushMethod,_win(obj),3,MUIM_Set,MUIA_Window_Open,FALSE);
    if (render) renderCard(cl,obj,data,data->entry);
    if (win && !(lib_flags & BASEFLG_MUI4)) DoMethod(_app(obj),MUIM_Application_PushMethod,_win(obj),3,MUIM_Set,MUIA_Window_Open,TRUE);

    return res;
}

/***********************************************************************/

static ULONG
mContextMenuChoice(struct IClass *cl,Object *obj,struct MUIP_ContextMenuChoice *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register Object 	 *cmobj = data->cmobj;

    if (muiUserData(msg->item)==Msg_Menu_Configure) DoMethod(cmobj,MUIM_CManager_PrefsWindow,MUIV_PrefsWindow_ActivePage_Card);
    else if (muiUserData(msg->item)==Msg_Card_Menu_Hide) DoMethod(_app(cmobj),MUIM_Application_PushMethod,data->cmobj,3,MUIM_Set,MUIA_CManager_NoCard,TRUE);

    return 0;
}

/***********************************************************************/

static ULONG
mContextMenuBuild(struct IClass *cl,Object *obj,struct MUIP_NList_ContextMenuBuild *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    return (ULONG)data->menu;
}

/***********************************************************************/

static ULONG
mGetMap(struct IClass *cl,Object *obj,struct MUIP_CManager_GetMap *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch (data->entry->Type)
    {
        case CMEntry_User:
        {
            switch (msg->type)
            {
                case MUIV_CManager_GetMap_Home:
                    return getMap(CMUSER(data->entry)->Address,
                                  CMUSER(data->entry)->Country,
                                  CMUSER(data->entry)->City,
                                  CMUSER(data->entry)->ZIP);

                case MUIV_CManager_GetMap_Job:
                    return getMap(CMUSER(data->entry)->BAddress,
                                  CMUSER(data->entry)->BCountry,
                                  CMUSER(data->entry)->BCity,
                                  CMUSER(data->entry)->BZIP);
            }
        }
    }

    return FALSE;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                   return mNew(cl,obj,(APTR)msg);
        case OM_DISPOSE:               return mDispose(cl,obj,(APTR)msg);
        case OM_SET:                   return mSets(cl,obj,(APTR)msg);

        case MUIM_ContextMenuBuild:    return mContextMenuBuild(cl,obj,(APTR)msg);
        case MUIM_ContextMenuChoice:   return mContextMenuChoice(cl,obj,(APTR)msg);

        case MUIM_CManager_GetMap:     return mGetMap(cl,obj,(APTR)msg);

        default:                       return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initCard(void)
{
    if (initTitle() &&
       (lib_Card = MUI_CreateCustomClass(NULL,NULL,lib_RootScrollgroup,sizeof(struct data),DISP(dispatcher))))
    {
        localizeMenus(tnMenu,tnMenuIDs);

        return TRUE;
    }

    disposeTitle();

    return FALSE;
}

/***********************************************************************/

void
disposeCard(void)
{
    disposeTitle();
    MUI_DeleteCustomClass(lib_Card);
    lib_Card = NULL;
}

/***********************************************************************/
