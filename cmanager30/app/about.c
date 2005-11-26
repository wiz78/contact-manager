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

#include "cm.h"
#include <mui/BWin_mcc.h>
#include <mui/Urltext_mcc.h>
#define USE_CMANAGER_BODY
#define USE_CMANAGER_COLORS
#include "CManager_iff.h"
#include "CManager_rev.h"

/***********************************************************************/

#define CManagerImage\
    BodychunkObject,\
        MUIA_FixWidth,              CMANAGER_WIDTH,\
        MUIA_FixHeight,             CMANAGER_HEIGHT,\
        MUIA_Bitmap_Width,          CMANAGER_WIDTH,\
        MUIA_Bitmap_Height,         CMANAGER_HEIGHT,\
        MUIA_Bodychunk_Depth,       CMANAGER_DEPTH,\
        MUIA_Bodychunk_Body,        CManager_body,\
        MUIA_Bodychunk_Compression, CMANAGER_COMPRESSION,\
        MUIA_Bodychunk_Masking,     CMANAGER_MASKING,\
        MUIA_Bitmap_SourceColors,   CManager_colors,\
        MUIA_Bitmap_Transparent,    0

#define ovfixspace RectangleObject, MUIA_FixHeightTxt, " ", End
#define ohfixspace RectangleObject, MUIA_FixWidthTxt, " ", End
#define otextitem  TextObject, MUIA_Text_Contents, "-", MUIA_Text_SetMax, TRUE, End

/***********************************************************************/

Object *
olabel(ULONG id)
{
    return Label(CM_GetString(id));
}

/****************************************************************************/

Object *
ollabel(ULONG id)
{
    return LLabel(CM_GetString(id));
}

/***********************************************************************/

Object *
ourltext(STRPTR url,STRPTR text)
{
    return UrltextObject,
        MUIA_Urltext_Text,           text,
        MUIA_Urltext_Url,            url,
        MUIA_Urltext_SetMax,         FALSE,
    End;
}

/***********************************************************************/

static Object *
othirdMUI(Object **mui)
{
    return HGroup,
        MUIA_Group_HorizSpacing, 0,
        Child, ohfixspace,
        Child, otextitem,
        Child, ohfixspace,
        Child, *mui = TextObject,
            ButtonFrame,
            MUIA_Background,     MUII_ButtonBack,
            MUIA_InputMode,      MUIV_InputMode_RelVerify,
            MUIA_Font,           MUIV_Font_Button,
            MUIA_ControlChar,    'm',
            MUIA_CycleChain,     TRUE,
            MUIA_Text_Contents,  "_MUI",
            MUIA_Text_PreParse,  MUIX_C,
            MUIA_Text_HiCharIdx, '_',
            MUIA_Text_SetMax,    TRUE,
        End,
        Child, ollabel(Msg_About_OfCourse),
        Child, HSpace(0),
    End;
}

/***********************************************************************/

static Object *
othird(ULONG stuff,STRPTR author,STRPTR url)
{
    register Object *g, *o[8];

    if (g = HGroup,
            MUIA_Group_HorizSpacing, 0,
            Child, o[0] = ohfixspace,
            Child, o[1] = otextitem,
            Child, o[2] = ohfixspace,
            Child, o[3] = ollabel(stuff),
            Child, o[4] = ohfixspace,
            Child, o[5] = HSpace(0),
        End)
    {
        register Object *u;

        if (author && *author)
            if (url && *url) u = ourltext(url,author);
            else u = LLabel(author);
        else u = NULL;

        if (u)
        {
            DoMethod(g,OM_ADDMEMBER,u);
            DoMethod(g,MUIM_Group_Sort,o[0],o[1],o[2],o[3],o[4],u,o[5],NULL);
        }
    }

    return g;
}

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object         *g, *ig, *olib, *omcc, *o[9];
    Object                  *mui;
    register struct TagItem *attrs = msg->ops_AttrList;
    register ULONG          nwin = !(mainFlags & MFLG_BWin);

    if (obj = (Object *)DoSuperNew(cl,obj,
        MUIA_HelpNode,                       "CMWRID",
        MUIA_Window_ID,                      MAKE_ID('c','m','a','b'),
        MUIA_Window_Title,                   CM_GetString(Msg_About_WinTitle),
        MUIA_Window_LeftEdge,                MUIV_Window_LeftEdge_Centered,
        MUIA_Window_TopEdge,                 MUIV_Window_TopEdge_Centered,
        MUIA_Window_UseRightBorderScroller,  nwin,
        MUIA_Window_UseBottomBorderScroller, nwin,
        MUIA_BWin_Borders,                   TRUE,

        WindowContents, VGroup,
            Child, MUI_NewObject(nwin ? MUIC_Scrollgroup : MUIC_Group,
                MUIA_Scrollgroup_UseWinBorder, nwin,
                MUIA_Scrollgroup_FreeHoriz,    nwin,

                nwin ? MUIA_Scrollgroup_Contents : Child, VirtgroupObject,

                    Child, HGroup,
                        MUIA_Group_HorizSpacing, 4,
                        Child, HSpace(0),
                        Child, CManagerImage, End,
                        Child, HSpace(10),
                        Child, VGroup,
                            MUIA_Group_VertSpacing, 0,
                            Child, VSpace(0),
                            Child, TextObject,
                                MUIA_Font, MUIV_Font_Big,
                                MUIA_Text_Contents, "Contact Manager",
                                MUIA_Text_PreParse, MUIX_B,
                            End,

                            Child, TextObject,
                                MUIA_Font, MUIV_Font_Big,
                                MUIA_Text_Contents, CM_GetString(Msg_App_Copyright),
                                //MUIA_Text_PreParse, MUIX_B,
                            End,
                            Child, VSpace(0),
                        End,
                        Child, HSpace(0),
                    End,

                    //Child, ovfixspace,

                    Child, TextObject,
                        MUIA_Text_Contents, CM_GetString(Msg_About_Descr),
                        MUIA_Text_PreParse, MUIX_C,
                    End,

                    Child, ovfixspace,

                    Child, MUI_MakeObject(MUIO_BarTitle,CM_GetString(Msg_About_Info)),

                    Child, HGroup,
                        Child, HSpace(0),
                        Child, ig = ColGroup(2),

                            Child, olabel(Msg_About_Authors),
                            Child, HGroup,
                                MUIA_Group_HorizSpacing, 0,
                                Child, ourltext("https://tellini.info","Simone Tellini"),
                                Child, ohfixspace,
                                Child, ourltext("mailto:alforan@tin.it","Alfonso Ranieri"),
                                Child, HSpace(0),
                            End,

                            /* Authors
                            Child, VGroup,
                                Child, HGroup, Child, HSpace(0), Child, olabel(Msg_About_Authors), End,
                                Child, VSpace(0),
                            End,
                            Child, VGroup,
                                Child, HGroup, Child, ourltext("mailto:alforan@tin.it","Alfonso Ranieri"), Child, HSpace(0), End,
                                Child, HGroup, Child, ourltext("mailto:wiz@vapor.com","Simone Tellini"), Child, HSpace(0), End,
                            End, */

                            /* Support */
                            Child, olabel(Msg_About_Support),
                            Child, ourltext("http://alfie.altervista.org",NULL),

                            /* Versions */
                            Child, VGroup,
                                Child, HGroup, Child, HSpace(0), Child, olabel(Msg_About_Version), End,
                                Child, VSpace(0),
                            End,
                            Child, VGroup,
                                Child, HGroup, Child, LLabel(PRGNAME), Child, HSpace(0), End,
                                Child, HGroup, Child, olib = LLabel(""), Child, HSpace(0), End,
                                Child, HGroup, Child, omcc = LLabel(""), Child, HSpace(0), End,
                            End,
                        End,
                        Child, HSpace(0),
                    End,

                    Child, ovfixspace,

                    Child, MUI_MakeObject(MUIO_BarTitle,CM_GetString(Msg_About_ThirdParts)),

                    Child, HGroup,
                        Child, HSpace(0),
                        Child, g = VGroup,
                            Child, o[0] = othird(Msg_About_NList,"Gilles Masson","mailto:masson@iutsoph.unice.fr"),
                            Child, o[1] = othird(Msg_About_NListtree,"Carsten Scholling","mailto:cs@aphaso.de"),
                            Child, o[2] = othird(Msg_About_Textinput,"Oliver Wagner","mailto:owagner@vapor.com"),
                            Child, o[3] = othird(Msg_About_GroupsImages,"Luca Longone","mailto:hexaae@tiscalinet.it"),
                            Child, o[4] = othirdMUI(&mui),
                        End,
                        Child, HSpace(0),
                    End,
                End,
            End,
        End,
        TAG_MORE,attrs))
    {
        register Object *app, *space, *cm;
        struct CMData   *cmdata;
        register STRPTR s;

        /* Set Database file name */
        cm = (Object *)GetTagData(MUIA_CMApp_CM,NULL,attrs);
        cmdata = NULL;
        get(cm,MUIA_CManager_CMData,&cmdata);
        if (cmdata && *cmdata->Path)
        {
            register Object *l;

            if (l = olabel(Msg_About_Database))
            {
                register Object *n;

                if (n = LLabel(cmdata->Path))
                {
                    DoMethod(ig,OM_ADDMEMBER,l);
                    DoMethod(ig,OM_ADDMEMBER,n);
                }
                else MUI_DisposeObject(l);
            }
        }

        /* Set Translation info */
        if ((s = CM_GetString(Msg_About_Translation)) && *s)
        {
            register Object *to;

            if (to = othird(Msg_About_Translation,CM_GetString(Msg_About_Translator),CM_GetString(Msg_About_TranslatorURL)))
            {
                DoMethod(g,OM_ADDMEMBER,to);
                DoMethod(g,MUIM_Group_Sort,o[0],o[1],o[2],o[3],to,o[4],NULL);
            }
        }

        if (space = ovfixspace) DoMethod(g,OM_ADDMEMBER,space);

        /* Set Versions */
        s = (STRPTR)CM_GetOption(CMGETOPTION_Version);
        set(olib,MUIA_Text_Contents,s);

        s = (STRPTR)GetTagData(MUIA_CManager_Version,NULL,attrs);
        set(omcc,MUIA_Text_Contents,s);

        app = (Object *)GetTagData(MUIA_Aboutmui_Application,NULL,attrs);
        DoMethod(mui,MUIM_Notify,MUIA_Pressed,0,app,1,MUIM_Application_AboutMUI);
        DoMethod(app,OM_ADDMEMBER,obj);
    }

    return (ULONG)obj;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW: return mNew(cl,obj,(APTR)msg);
        default:     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initCMAboutClass(void)
{
    if (mainFlags & MFLG_BWin) CMAboutClass = MUI_CreateCustomClass(NULL,MUIC_BWin,NULL,0,DISP(dispatcher));
    if (!CMAboutClass) CMAboutClass = MUI_CreateCustomClass(NULL,MUIC_Window,NULL,0,DISP(dispatcher));

    return (ULONG)CMAboutClass;
}

/***********************************************************************/
