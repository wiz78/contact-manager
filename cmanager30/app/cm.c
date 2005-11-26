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

#define CATCOMP_STRINGS
#include "cm.h"
#include "CManager_rev.h"

/***********************************************************************/

#ifdef __MORPHOS__
struct IntuitionBase   *IntuitionBase;
#endif

struct Library         *UtilityBase;
struct Library         *IconBase;
struct Library         *MUIMasterBase;
struct Library         *CManagerBase;
struct Library         *GenesisBase;

struct MUI_CustomClass *CMAppClass;
struct MUI_CustomClass *CMWinClass;
struct MUI_CustomClass *CMAboutClass;

long                   __stack = 256000;
UBYTE                  __ver[] = VERSTAG;

ULONG                  mainFlags;

/***********************************************************************/

static void
requester(ULONG err)
{
    register STRPTR title, text, gads;

    if (CManagerBase)
    {
        title = CM_GetString(MSG_ErrReqTitle);
        text  = CM_GetString(err);
        gads  = CM_GetString(MSG_NOMUIErrReqGadget);
    }
    else
    {
        title = MSG_ErrReqTitle_STR;
        text  = Msg_Error_NoApp_STR;
        gads  = MSG_NOMUIErrReqGadget_STR;
    }

    if (MUIMasterBase)
    {
        register Object *app = ApplicationObject,
            MUIA_Application_Title,          "Contact Manager",
            MUIA_Application_Base,           "CONTACTMAN",
            MUIA_Application_UseCommodities, FALSE,
            MUIA_Application_UseRexx,        FALSE,
        End;

        MUI_RequestA(app,NULL,0,title,gads,text,NULL);

        if (app) MUI_DisposeObject(app);
    }
    else
    {
        struct EasyStruct es;

        es.es_StructSize   = sizeof(struct EasyStruct);
        es.es_Flags        = 0;
        es.es_Title        = title;
        es.es_TextFormat   = text;
        es.es_GadgetFormat = gads;

        EasyRequestArgs(NULL,&es,NULL,NULL);
    }
}

/***********************************************************************/

ULONG
checkUser(STRPTR user)
{
    register ULONG found;
    register int   i;

    for (found = FALSE, i = 0; ; i++)
    {
        REGARRAY UBYTE name[256];

        if (!GetUserName(i,name,sizeof(name))) break;

        if (!stricmp(name,user))
        {
            found = TRUE;
            break;
        }
    }

    return found;
}

/***********************************************************************/

static ULONG
login(struct parseArgs *pa)
{
    register ULONG res = RETURN_OK;

    if (GenesisBase && !(pa->flags & PAFLG_NoLogin))
    {
        register struct User *ud;

        ud = (pa->flags & PAFLG_User) ? NULL : GetGlobalUser();

        if (!ud)
        {
            REGARRAY UBYTE buf[41];

            if (pa->flags & PAFLG_User)
            {
                if (!checkUser(pa->user)) return RETURN_ERROR;
            }
            else GetUserName(0,buf,sizeof(buf)-1);

            ud = GetUser((pa->flags & PAFLG_User) ? pa->user : buf,(pa->flags & PAFLG_Password) ? pa->password : NULL,NULL,0);
        }

        if (ud)
        {
            SetGlobalUser(ud);
            strcpy(pa->user,ud->us_name);
            FreeUser(ud);
        }
        else res = RETURN_ERROR;
    }

    pa->flags &= ~PAFLG_User;

    return res;
}

/***********************************************************************/

#ifdef __MORPHOS__
int
realMain(int argc,char **argv)
#else
int
main(int argc,char **argv)
#endif
{
    register struct Process *this;
    register APTR           win;
    register ULONG          err = Msg_Error_NoMem;
    register int            res = RETURN_FAIL;

    #ifdef __MORPHOS__
    if (!(IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library",37)))
    return RETURN_ERROR;
    #endif

    this = (struct Process *)FindTask(NULL);
    win = this->pr_WindowPtr;
    this->pr_WindowPtr = (APTR)-1;

    MUIMasterBase = OpenLibrary("muimaster.library",19);
    CManagerBase = OpenLibrary("CManager.library",34);

    if (MUIMasterBase && CManagerBase)
    {
        if (UtilityBase = OpenLibrary("utility.library",37))
        {
            if (IconBase = OpenLibrary("icon.library",37))
            {
                if (!CM_GetOption(CMGETOPTION_AvoidGenesis)) GenesisBase = OpenLibrary(GENESISNAME,2);

                if (initCMAppClass())
                {
                    REGARRAY UBYTE buf[16];
                    LONG          val = TRUE;

                    if ((GetVar("CManager/UseBWin",buf,sizeof(buf),GVF_GLOBAL_ONLY)<0) ||
                        (StrToLong(buf,&val)==0)) val = FALSE;
                    if (val) mainFlags |= MFLG_BWin;

                    if (initCMWinClass())
                    {
                        struct parseArgs pa;

                        memset(&pa,0,sizeof(pa));

                        if (!(res = parseArgs(&pa)))
                        {
                            register Object *app;

                            if (app = CMAppObject, MUIA_CMApp_ParseArgs, &pa, End)
                            {
                                if (!(res = login(&pa)))
                                {
                                    Object *win, *cm;

                                    get(app,MUIA_CMApp_Win,&win);
                                    get(app,MUIA_CMApp_CM,&cm);

                                    if (pa.flags & PAFLG_File) DoMethod(cm,MUIM_CManager_LoadData,NULL,pa.file);
                                    else if (pa.flags & PAFLG_User) DoMethod(cm,MUIM_CManager_LoadData,pa.user,NULL);
                                         else DoMethod(cm,MUIM_CManager_LoadData,NULL,NULL);

                                    DoMethod(app,MUIM_CMApp_ScanHotlists);

                                    if (openWindow(app,win))
                                    {
                                        ULONG sigs = 0;

                                        for (;;)
                                        {
                                            if (DoMethod(app,MUIM_Application_NewInput,&sigs)==MUIV_Application_ReturnID_Quit)
                                            {
                                                ULONG val;

                                                get(app,MUIA_Application_ForceQuit,&val);
                                                if (!val) get(cm,MUIA_CManager_Quit,&val);
                                                if (val) break;

                                                DoMethod(app,MUIM_CMApp_Close,MUIV_CMApp_Close_Check);
                                            }

                                            if (sigs && ((sigs = Wait(sigs|SIGBREAKF_CTRL_C)) & SIGBREAKF_CTRL_C))
                                                break;
                                        }

                                        res = RETURN_OK;
                                        err = 0;
                                    }
                                    else
                                    {
                                        res = RETURN_FAIL;
                                        err = Msg_Error_NoWin;
                                    }
                                }
                                else err = Msg_Error_CantLogin;

                                MUI_DisposeObject(app);
                            }
                            else
                            {
                                res = RETURN_FAIL;
                                err = Msg_Error_NoApp;
                            }
                        }
                        else err = 0;

                        MUI_DeleteCustomClass(CMWinClass);

                        if (CMAboutClass) MUI_DeleteCustomClass(CMAboutClass);
                    }

                    MUI_DeleteCustomClass(CMAppClass);
                }

                if (GenesisBase) CloseLibrary(GenesisBase);

                CloseLibrary(IconBase);
            }

            CloseLibrary(UtilityBase);
        }
    }
    else err = Msg_Error_NoApp;

    if (err) requester(err);

    if (CManagerBase) CloseLibrary(CManagerBase);
    if (MUIMasterBase) CloseLibrary(MUIMasterBase);

    this->pr_WindowPtr = win;

    #ifdef __MORPHOS__
    CloseLibrary((struct Library *)IntuitionBase);
    #endif

    return res;
}

/***********************************************************************/

#ifdef __MORPHOS__
#define MIN68KSTACKSIZE 64000

int
main(int argc,char **argv)
{
    struct Task *me = FindTask(NULL);
    ULONG       size;

    if (NewGetTaskAttrsA(me,&size,sizeof(size),TASKINFOTYPE_STACKSIZE_M68K,NULL))
    {
        if (size<MIN68KSTACKSIZE)
        {
            struct StackSwapStruct *sss;

            if (sss = AllocVec(sizeof(*sss)+MIN68KSTACKSIZE,MEMF_PUBLIC))
            {
                int res;

                sss->stk_Lower   = sss+1;
                sss->stk_Upper   = (ULONG)(((UBYTE *)(sss + 1))+MIN68KSTACKSIZE);
                sss->stk_Pointer = (APTR)sss->stk_Upper;
                StackSwap(sss);
                res = realMain(argc,argv);
                StackSwap(sss);
                FreeVec(sss);

                return res;
            }
        }

        return realMain(argc,argv);
    }

    return RETURN_ERROR;
}

#endif

/***********************************************************************/

