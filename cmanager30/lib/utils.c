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
#include <proto/iffparse.h>
#include <prefs/prefhdr.h>

/***********************************************************************/

APTR
allocVecPooled(APTR pool,ULONG size)
{
    register ULONG *mem;

    if (mem = AllocPooled(pool,size = size+sizeof(ULONG)))
        *mem++ = size;

    return mem;
}

/****************************************************************************/

void
freeVecPooled(APTR pool,APTR mem)
{
    FreePooled(pool,(LONG *)mem-1,*((LONG *)mem-1));
}

/****************************************************************************/

APTR
allocArbitratePooled(ULONG size)
{
    register APTR mem;

    ObtainSemaphore(&lib_poolSem);
    mem = AllocPooled(lib_pool,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

APTR
freeArbitratePooled(APTR mem,ULONG size)
{
    ObtainSemaphore(&lib_poolSem);
    FreePooled(lib_pool,mem,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

APTR
allocArbitrateVecPooled(ULONG size)
{
    register APTR mem;

    ObtainSemaphore(&lib_poolSem);
    mem = allocVecPooled(lib_pool,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

APTR
freeArbitrateVecPooled(APTR mem)
{
    ObtainSemaphore(&lib_poolSem);
    freeVecPooled(lib_pool,mem);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

#ifndef __MORPHOS__
static UWORD fmtfunc[] = { 0x16c0, 0x4e75 };

void STDARGS
sprintf(STRPTR buf,STRPTR fmt,...)
{
    RawDoFmt(fmt,&fmt+1,(APTR)fmtfunc,buf);
}
#else
void
sprintf(STRPTR buf,STRPTR fmt,...)
{
    va_list va;

    va_start(va,fmt);
    VNewRawDoFmt(fmt,0,buf,va);
    va_end(va);
}
#endif

/***********************************************************************/

#define DEF_PrefsFile "CManager.prefs"
#define ID_CODESET    MAKE_ID('L','C','S','T')
#define CMANAGER_PREFS_VERSION 4

ULONG
userCodeset(STRPTR user,STRPTR cbuf,ULONG clen)
{
    REGARRAY UBYTE            fileName[256], buf[256];
    register struct IFFHandle *iffh;
    register ULONG            res = FALSE;

    strcpy(fileName,lib_CMPath);
    if (user && *user) sprintf(buf,"%s/%s.prefs",user,user);
    else strcpy(buf,DEF_PrefsFile);
    AddPart(fileName,buf,sizeof(fileName));

    if (iffh = AllocIFF())
    {
        if (iffh->iff_Stream = Open(fileName,MODE_OLDFILE))
        {
            InitIFFasDOS(iffh);

            if (!OpenIFF(iffh,IFFF_READ))
            {
                struct PrefHeader           prhd;
                register struct ContextNode *cn;
                register ULONG              l;

                if (StopChunk(iffh,ID_PREF,ID_PRHD)) goto fail;

                if (StopChunk(iffh,ID_PREF,ID_CODESET)) goto fail;

                if (ParseIFF(iffh,IFFPARSE_SCAN)) goto fail;

                if (!(cn = CurrentChunk(iffh))) goto fail;

                if ((cn->cn_Type!=ID_PREF) || (cn->cn_ID!=ID_PRHD) ||
                    (cn->cn_Size!=sizeof(struct PrefHeader))) goto fail;

                if (ReadChunkBytes(iffh,&prhd,cn->cn_Size)!=cn->cn_Size) goto fail;

                if (prhd.ph_Version>CMANAGER_PREFS_VERSION) goto fail;

                for (;;)
                {
                    register ULONG error;

                    error = ParseIFF(iffh,IFFPARSE_SCAN);
                    if (error==IFFERR_EOF) break;
                    else if (error) goto fail;

                    if (!(cn = CurrentChunk(iffh))) goto fail;

                    if (cn->cn_Type!=ID_PREF) continue;

                    if ((cn->cn_ID==ID_CODESET) && ((l = cn->cn_Size)<clen))
                    {
                        if (ReadChunkBytes(iffh,cbuf,l)!=l) goto fail;
                        cbuf[l] = 0;
                        break;
                    }
                }

                res = TRUE;

                fail: CloseIFF(iffh);
            }

            Close(iffh->iff_Stream);
        }

        FreeIFF(iffh);
    }

    return res;
}

/**************************************************************************/

int
countNodes(struct MinList *List)
{
    register struct MinNode *node, *succ;
    register int            num;

    for (node = List->mlh_Head, num = 0; succ = node->mln_Succ; node = succ) ++num;

    return num;
}

/***********************************************************************/
