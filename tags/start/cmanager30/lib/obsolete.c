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

#include "lib.h"

/***********************************************************************/

#ifdef __MORPHOS__
ULONG
OBSOLETE_CM_LoadData(void)
{
    /*STRPTR file	  = (STRPTR)REG_A0;
    struct CMData *data = (struct CMData *)REG_A1;
    STRPTR user		= (STRPTR)REG_A2;*/
#else
ULONG SAVEDS ASM
OBSOLETE_CM_LoadData(REG(a0,STRPTR file),REG(a1,struct CMData *data),REG(a2,STRPTR user))
{
#endif

    return FALSE;
}

/***********************************************************************/

#ifdef __MORPHOS__
ULONG
OBSOLETE_CM_LoadDataOld(void)
{
    /*STRPTR prefsName       = (STRPTR)REG_A0;
    struct CMDataOld *data = (struct CMDataOld *)REG_A1;
    STRPTR user            = (STRPTR)REG_A2;*/
#else
ULONG SAVEDS ASM
OBSOLETE_CM_LoadDataOld(REG(a0,STRPTR prefsName),REG(a1,struct CMDataOld *data),REG(a2,STRPTR user))
{
#endif

    return FALSE;
}

/***********************************************************************/

#ifdef __MORPHOS__
void
OBSOLETE_CM_SaveData(void)
{
    /*STRPTR file	  = (STRPTR)REG_A0;
    struct CMData *data = (struct CMData *)REG_A1;
    STRPTR user		= (STRPTR)REG_A2;*/
#else
void SAVEDS ASM
OBSOLETE_CM_SaveData(REG(a0,STRPTR file),REG(a1,struct CMData *data),REG(a2,STRPTR user))
{
#endif

}

/***********************************************************************/

#ifdef __MORPHOS__
void
OBSOLETE_CM_SaveDataOld(void)
{
    /*STRPTR prefsName       = (STRPTR)REG_A0;
    struct CMDataOld *data = (struct CMDataOld *)REG_A1;
    STRPTR user		   = (STRPTR)REG_A2;*/
#else
void SAVEDS ASM
OBSOLETE_CM_SaveDataOld(REG(a0,STRPTR prefsName),REG(a1,struct CMDataOld *data),REG(a2,STRPTR user))
{
#endif

}

/***********************************************************************/

#ifdef __MORPHOS__
void
OBSOLETE_CM_FreeDataOld(void)
{
    /*struct CMDataOld *data = (struct CMDataOld *)REG_A0;*/
#else
void SAVEDS ASM
OBSOLETE_CM_FreeDataOld(REG(a0,struct CMDataOld *data))
{
#endif

}

/***********************************************************************/

#ifdef __MORPHOS__
struct CMData *
OBSOLETE_CM_LoadCurrentUserData(void)
{
    /*ULONG askUser = (ULONG)REG_D0;*/
#else
struct CMData * SAVEDS ASM
OBSOLETE_CM_LoadCurrentUserData(REG(d0,ULONG askUser))
{
#endif

    return NULL;
}

/***********************************************************************/

#ifdef __MORPHOS__
void
OBSOLETE_CM_DeleteBitMap(void)
{
    /*struct BitMap *bm = (struct BitMap *)REG_A0;*/
#else
void SAVEDS ASM
OBSOLETE_CM_DeleteBitMap(REG(a0,struct BitMap *bm))
{
#endif

}

/***********************************************************************/

#ifdef __MORPHOS__
struct BitMap *
OBSOLETE_CM_CreateBitMap(void)
{
    /*ULONG width           = (ULONG)REG_D0;
    ULONG height          = (ULONG)REG_D1;
    ULONG depth           = (ULONG)REG_D2;
    ULONG flags           = (ULONG)REG_D3;
    struct BitMap *friend = (struct BitMap *)REG_A0;*/
#else
struct BitMap * SAVEDS ASM
OBSOLETE_CM_CreateBitMap(REG(d0,ULONG width),REG(d1,ULONG height),REG(d2,ULONG depth),REG(d3,ULONG flags),REG(a0,struct BitMap *friend))
{
#endif

    return NULL;
}

/**********************************************************************/
