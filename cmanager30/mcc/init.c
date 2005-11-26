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

static void
finishInit(void)
{
    ObtainSemaphore(&lib_sem);

    if (!(lib_flags & BASEFLG_FinishInit))
    {
        register ULONG res = FALSE;

        if (GetVar("Vapor/CManager_Path",lib_CMPath,sizeof(lib_CMPath),0)>0)
        {
            register STRPTR ptr;

            if (ptr = PathPart(lib_CMPath)) *ptr = '\0';

            res = TRUE;
        }
        else
        {
            for (;;)
            {
                if (getDir(NULL,CM_GetString(Msg_GetDBDir),lib_CMPath,TRUE))
                {
                    REGARRAY UBYTE cm[256];
                    register BPTR  f;

                    strcpy(cm,lib_CMPath);
                    AddPart(cm,"CManager",sizeof(cm));

                    UnLock(CreateDir("ENV:CManager"));
                    UnLock(CreateDir("ENVARC:CManager"));

                    if (f = Open("ENVARC:Vapor/CManager_Path",MODE_NEWFILE))
                    {
                        Write(f,cm,strlen(cm));
                        Close(f);
                    }

                    if (f = Open("ENV:Vapor/CManager_Path",MODE_NEWFILE))
                    {
                        Write(f,cm,strlen(cm));
                        Close(f);
                    }

                    res = TRUE;
                    break;
                }

                if (!MUI_Request(NULL,NULL,0,"CManager",CM_GetString(Msg_NOMUINoDirWarningGads),CM_GetString(Msg_NOMUINoDirWarningMsg)))
                    break;
            }
        }

        if (res) lib_flags |= BASEFLG_FinishInit;
    }

    ReleaseSemaphore(&lib_sem);
}

/***********************************************************************/

#ifdef __MORPHOS__
ULONG query(void)
{
    LONG which = (LONG)REG_D0;
#else
ULONG SAVEDS ASM query(REG(d0,LONG which))
{
#endif

    switch (which)
    {
        case 0:
            finishInit();
            return (ULONG)lib_Class;

        default:
            return 0;
    }
}

/****************************************************************************/

void
freeBase(void)
{
    if (MUIMasterBase)
    {
        if (lib_miniMailWindow) disposeMiniMailWindow();
        if (lib_PopupCodesets) disposePopupCodesets();
        if (lib_Popasl) disposePopasl();
        if (lib_QString) disposeQString();
        if (lib_Pic) disposePic();
        if (lib_DelWarning) disposeDelWarning();
        if (lib_EntryList) disposeEntryList();
        if (lib_GroupList) disposeGroupList();
        if (lib_EditUser) disposeEditUser();
        if (lib_EditWWW) disposeEditWWW();
        if (lib_EditFTP) disposeEditFTP();
        if (lib_EditIRC) disposeEditIRC();
        if (lib_EditTelnet) disposeEditTelnet();
        if (lib_EditWindow) disposeEditWindow();
        if (lib_DialWindow) disposeDialWindow();
        if (lib_SearchWindow) disposeSearchWindow();
        if (lib_LoginWindow) disposeLoginWindow();
        if (lib_PrefsWindow) disposePrefsWindow();
        if (lib_RootScroll) disposeRootScroll();
        if (lib_RootString) disposeRootString();
        if (lib_RootBar) disposeRootBar();
        if (lib_Lamp) disposeLamp();
        if (lib_Popph) disposePopph();
        if (lib_Card) disposeCard();
        if (lib_RootScrollgroup) disposeRootScrollgroup();
        if (lib_MatherWin) disposeMatherWin();
        if (lib_Class) disposeMCC();

        CloseLibrary(MUIMasterBase);
        MUIMasterBase = NULL;
    }

    if (CyberGfxBase)
    {
        CloseLibrary(CyberGfxBase);
        CyberGfxBase = NULL;
    }

    if (GenesisBase)
    {
        CloseLibrary(GenesisBase);
        GenesisBase = NULL;
    }

    if (OpenURLBase)
    {
        CloseLibrary(OpenURLBase);
        OpenURLBase = NULL;
    }

    if (lib_pool)
    {
        DeletePool(lib_pool);
        lib_pool = NULL;
    }

    if (CodesetsBase)
    {
        CloseLibrary(CodesetsBase);
        CodesetsBase = NULL;
    }

    if (LocaleBase)
    {
        if (lib_locale)
        {
            CloseLocale(lib_locale);
            lib_locale = NULL;
        }

        CloseLibrary((struct Library *)LocaleBase);
        LocaleBase = NULL;
    }

    if (CManagerBase)
    {
        CloseLibrary(CManagerBase);
        CManagerBase = NULL;
    }

    if (RexxSysBase)
    {
        CloseLibrary((struct Library *)RexxSysBase);
        RexxSysBase = NULL;
    }

    if (IFFParseBase)
    {
        CloseLibrary((struct Library *)IFFParseBase);
        IFFParseBase = NULL;
    }

    if (DataTypesBase)
    {
        CloseLibrary(DataTypesBase);
        DataTypesBase = NULL;
    }

    if (UtilityBase)
    {
        CloseLibrary(UtilityBase);
        UtilityBase = NULL;
    }

    if (IntuitionBase)
    {
        CloseLibrary((struct Library *)IntuitionBase);
        IntuitionBase = NULL;
    }

    if (GfxBase)
    {
        CloseLibrary((struct Library *)GfxBase);
        GfxBase = NULL;
    }

    if (DOSBase)
    {
        CloseLibrary((struct Library *)DOSBase);
        DOSBase = NULL;
    }

    lib_flags &= ~(BASEFLG_Init|BASEFLG_AvoidGenesis|BASEFLG_BWin|BASEFLG_Countries|BASEFLG_NoEntriesFrame|BASEFLG_GroupsInputFrame|BASEFLG_MUI20);
}

/***********************************************************************/

ULONG
initBase(void)
{
    register struct Process *this;
    register struct Library *vtk;
    register APTR           win;
    register ULONG          res = FALSE;

    NEWLIST(&lib_countries);

    this = (struct Process *)FindTask(NULL);
    win = this->pr_WindowPtr;
    this->pr_WindowPtr = (APTR)-1;

    if ((vtk = OpenLibrary("vapor_toolkit.library",15)) &&
        (DOSBase = (struct DosLibrary *)OpenLibrary("dos.library",37)) &&
        (IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library",37)) &&
        (UtilityBase = OpenLibrary("utility.library",37)) &&
        (GfxBase = (struct GfxBase *)OpenLibrary("graphics.library",37)) &&
        (DataTypesBase = OpenLibrary("datatypes.library",37)) &&
        (IFFParseBase = OpenLibrary("iffparse.library",37)) &&
        (RexxSysBase = (struct RxsLib *)OpenLibrary("rexxsyslib.library",36)) &&
        (RexxSysBase = (struct RxsLib *)OpenLibrary("rexxsyslib.library",36)) &&
        (MUIMasterBase = OpenLibrary("muimaster.library",19)) &&
        (CManagerBase  = OpenLibrary("CManager.library",34)) &&
        (LocaleBase = OpenLibrary("locale.library",37)) &&
        (CodesetsBase = OpenLibrary("codesets.library",5)) &&
        (lib_pool = CreatePool(MEMF_ANY,2048,1024)))
    {
        REGARRAY UBYTE buf[16];
        LONG           val = TRUE;

        if ((GetVar("CManager/UseBWin",buf,sizeof(buf),GVF_GLOBAL_ONLY)<0) ||
            (StrToLong(buf,&val)==0)) val = FALSE;
        if (val) lib_flags |= BASEFLG_BWin;

        if ((GetVar("CManager/NoEntriesFrame",buf,sizeof(buf),GVF_GLOBAL_ONLY)<0) ||
            (StrToLong(buf,&val)==0)) val = FALSE;
        if (val) lib_flags |= BASEFLG_NoEntriesFrame;

        if ((GetVar("CManager/GroupsInputFrame",buf,sizeof(buf),GVF_GLOBAL_ONLY)<0) ||
            (StrToLong(buf,&val)==0)) val = FALSE;
        if (val) lib_flags |=  BASEFLG_GroupsInputFrame;

        if (CM_GetOption(CMGETOPTION_AvoidGenesis)) lib_flags |= BASEFLG_AvoidGenesis;
        else GenesisBase = OpenLibrary(GENESISNAME,2);

        if (initMatherWin() &&
            initRootScrollgroup() &&
            initRootString() &&
            initRootBar() &&
            initQString() &&
            initPic() &&
            initCard() &&
            initEntryList() &&
            initGroupList() &&
            initLamp() &&
            initMCC())
        {
            OpenURLBase = OpenLibrary("openurl.library",4);

            if (GfxBase->LibNode.lib_Version>=39)
                CyberGfxBase = OpenLibrary("cybergraphics.library",41);

            lib_locale = OpenLocale(NULL);

            if (MUIMasterBase->lib_Version>=MUIVER20)
            {
                lib_flags |= BASEFLG_MUI20;

                if (MUIMasterBase->lib_Version>MUIVER20 || MUIMasterBase->lib_Revision>=5341)
                    lib_flags |= BASEFLG_MUI4;
            }

            res = TRUE;
        }
    }

    CloseLibrary(vtk);

    this->pr_WindowPtr = win;

    if (res)
    {
        lib_flags |= BASEFLG_Init;
    }
    else freeBase();

    return res;
}

/***********************************************************************/
