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

#define CATCOMP_ARRAY
#include "lib.h"

/***********************************************************************/

static struct Catalog *
openCat(STRPTR name,ULONG minVer,ULONG minRev)
{
    register struct Catalog *cat;

    if (cat = OpenCatalogA(NULL,name,NULL))
    {
        register ULONG ver = cat->cat_Version;

        if ((ver<minVer) ? TRUE : ((ver==minVer) ? (cat->cat_Revision<minRev) : FALSE))
        {
            CloseCatalog(cat);
            cat = NULL;
        }
    }

    return cat;
}

/***********************************************************************/

void
initStrings(void)
{
    if ((LocaleBase = OpenLibrary("locale.library",36)) &&
        (lib_cat = openCat("CManager.catalog",33,0)))
    {
        register struct CatCompArrayType *cca;
        register int                     cnt;

        for (cnt = (sizeof(CatCompArray)/sizeof(struct CatCompArrayType))-1, cca = (struct CatCompArrayType *)CatCompArray+cnt;
             cnt>=0;
             cnt--, cca--)
        {
            register STRPTR s;

            if (s = GetCatalogStr(lib_cat,cca->cca_ID,cca->cca_Str)) cca->cca_Str = s;
        }
    }
}

/***********************************************************************/

#ifdef __MORPHOS__
STRPTR CM_GetString(ULONG id)
#else
STRPTR SAVEDS ASM CM_GetString(REG(d0,ULONG id))
#endif
{
    register int low, high;

    for (low = 0, high = (sizeof(CatCompArray)/sizeof(struct CatCompArrayType))-1; low<=high; )
    {
        register int                     mid = (low+high)>>1, cond;
        register struct CatCompArrayType *cca = (struct CatCompArrayType *)CatCompArray+mid;

        if ((cond = id-cca->cca_ID)==0)
            return lib_cat ? GetCatalogStr(lib_cat,id,cca->cca_Str) : cca->cca_Str;

        if (cond<0) high = mid-1;
        else low = mid+1;
    }

    return (STRPTR)"";
}

#ifdef __MORPHOS__
STRPTR LIB_GetString(void)
{
    return CM_GetString((ULONG)REG_D0);
}
#endif

/***********************************************************************/
