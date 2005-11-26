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

#include "lib.h"

/***********************************************************************/

void
freeBase(void)
{
    if (AsyncIOBase)
    {
        CloseLibrary(AsyncIOBase);
        AsyncIOBase = NULL;
    }

    if (GenesisBase)
    {
        CloseLibrary(GenesisBase);
        GenesisBase = NULL;
    }

    if (CodesetsBase)
    {
        CloseLibrary(CodesetsBase);
        CodesetsBase = NULL;
    }

    if (IFFParseBase)
    {
        CloseLibrary((struct Library *)IFFParseBase);
        IFFParseBase = NULL;
    }

    if (LocaleBase)
    {
        if (lib_cat)
        {
            CloseCatalog(lib_cat);
            lib_cat = NULL;
        }

        CloseLibrary((struct Library *)LocaleBase);
        LocaleBase = NULL;
    }

    if (lib_pool)
    {
        DeletePool(lib_pool);
        lib_pool = NULL;
    }

    if (UtilityBase)
    {
        CloseLibrary(UtilityBase);
        UtilityBase = NULL;
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

    lib_flags &= ~(BASEFLG_Init|BASEFLG_AvoidGenesis|BASEFLG_AvoidAsyncIO|BASEFLG_AsyncIO);
}

/***********************************************************************/

ULONG
initBase(void)
{
    if ((DOSBase = (struct DosLibrary *)OpenLibrary("dos.library",37)) &&
        (UtilityBase = OpenLibrary("utility.library",37)) &&
        (GfxBase = (struct GfxBase *)OpenLibrary("graphics.library",37)) &&
        (IFFParseBase = OpenLibrary("iffparse.library",37)) &&
        (CodesetsBase = OpenLibrary("codesets.library",5)) &&
        (lib_pool = CreatePool(MEMF_ANY,sizeof(struct CMUser)*50,sizeof(struct CMUser)*50)))
    {
        REGARRAY UBYTE buf[256];
        LONG     val = TRUE;

        if ((GetVar("CManager/AvoidAsyncIO",buf,sizeof(buf),GVF_GLOBAL_ONLY)>0) &&
            (StrToLong(buf,&val)>0) && val) lib_flags |= BASEFLG_AvoidAsyncIO;
        else
            if (AsyncIOBase = OpenLibrary("asyncio.library",0))
                lib_flags |= BASEFLG_AsyncIO;

        if ((GetVar("CManager/AvoidGenesis",buf,sizeof(buf),GVF_GLOBAL_ONLY)>0) &&
            (StrToLong(buf,&val)>0) && val) lib_flags |= BASEFLG_AvoidGenesis;
        else
        {
            register struct Process *this;
            register APTR           win;

            this = (struct Process *)FindTask(NULL);
            win = this->pr_WindowPtr;
            this->pr_WindowPtr = (APTR)-1;

            GenesisBase = OpenLibrary(GENESISNAME,2);

            this->pr_WindowPtr = win;
        }

        if (GetVar("Vapor/CManager_Path",lib_CMPath,sizeof(lib_CMPath),0)>0)
        {
            register STRPTR ptr;

            if (ptr = PathPart(lib_CMPath)) *ptr = '\0';
        }

        initStrings();

        lib_flags |= BASEFLG_Init;

        return TRUE;
    }

    freeBase();

    return FALSE;
}

/***********************************************************************/
