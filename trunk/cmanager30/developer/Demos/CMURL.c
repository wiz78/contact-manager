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

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/muimaster.h>

#include <libraries/mui.h>
#include <libraries/CManager.h>

#include <clib/alib_protos.h>

#include <mui/CManager_mcc.h>

#include <stdio.h>
#include <string.h>

/***************************************************************************/

#ifndef __MMORPHOS__
#define SAVEDS __saveds
#define ASM __asm
#define STDARGS __stdargs
#define REG(x) register __ ## x
#endif

#define MAKE_ID(a,b,c,d) ((ULONG)(a)<<24|(ULONG)(b)<<16|(ULONG)(c)<<8|(ULONG)(d))

/***************************************************************************/

struct data
{
    Object *pop;     /* The Popstring object                */
    Object *str;     /* The String object of the Popostring */
    Object *win;     /* The Window to pop up                */
    Object *cm;      /* The CManager object                 */
    ULONG  load;     /* Flag, if 0 load data in cm          */
};

/***************************************************************************/

#ifdef __MORPHOS__
long           __stack = 128000;
#else
long           __stack = 64000;
#endif

struct IntuitionBase *IntuitionBase = NULL;
struct Library 	     *MUIMasterBase = NULL;

/***********************************************************************/

#ifdef __MORPHOS__
ULONG
openFun(void)
{
    struct Hook *hook = (struct Hook *)REG_A0;
//    Object *pop       = (Object *)REG_A2;
//    Object **str      = (Object **)REG_A1;
#else
ULONG ASM SAVEDS
openFun(REG(a0) struct Hook *hook,REG(a2) Object *pop,REG(a1) Object **str)
{
#endif
    register struct data *data = hook->h_Data;

    if (!data->load)
    {
        DoMethod(data->cm,MUIM_CManager_LoadData,NULL,NULL);
        data->load = 1;
    }

    set(data->win,MUIA_Window_Open,TRUE);

    return TRUE;
}

#ifdef __MORPHOS__
static struct EmulLibEntry openTrap = {TRAP_LIB,0,(void (*)(void))openFun};
static struct Hook openHook = {0,0,(HOOKFUNC)&openTrap};
#else
static struct Hook openHook = {0,0,(HOOKFUNC)openFun};
#endif

/***************************************************************************/

struct closeMode
{
    Object *str;
    LONG   success;
};

#ifdef __MORPHOS__
ULONG
closeFun(void)
{
    struct Hook *hook = (struct Hook *)REG_A0;
//    Object *pop = (Object *)REG_A2;
//    struct closeMode *close = (struct closeMode *)REG_A1;
#else
ULONG ASM SAVEDS
closeFun(REG(a0) struct Hook *hook,REG(a2) Object *pop,REG(a1) struct closeMode *close)
{
#endif
    register struct data *data = hook->h_Data;

    set(data->win,MUIA_Window_Open,FALSE);

    return TRUE;
}

#ifdef __MORPHOS__
static struct EmulLibEntry closeTrap = {TRAP_LIB,0,(void (*)(void))closeFun};
static struct Hook closeHook = {0,0,(HOOKFUNC)&closeTrap};
#else
static struct Hook closeHook = {0,0,(HOOKFUNC)closeFun};
#endif

/***************************************************************************/
/*
** Set the string contents and close the pop
*/

#ifdef __MORPHOS__
ULONG
dcFun(void)
{
    struct Hook *hook = (struct Hook *)REG_A0;
//    Object *cm = (Object *)REG_A2;
    struct CMWWW *entry = (struct CMWWW *)REG_A1;
#else
ULONG SAVEDS ASM
dcFun(REG(a0) struct Hook *hook,REG(a2) Object *cm,REG(a1) struct CMWWW *entry)
{
#endif
    register struct data *data = hook->h_Data;
    register STRPTR      s;

    switch (entry->Type)
    {
        case CMEntry_WWW:
            s = entry->WWW;
            break;

        case CMEntry_FTP:
            s = ((struct CMFTP *)entry)->FTP;
    }

    set(data->str,MUIA_String_Contents,s);
    DoMethod(data->pop,MUIM_Popstring_Close,TRUE);

    return 0;
}

#ifdef __MORPHOS__
static struct EmulLibEntry dcTrap = {TRAP_LIB,0,(void (*)(void))dcFun};
static struct Hook dcHook = {0,0,(HOOKFUNC)&dcTrap};
#else
static struct Hook dcHook = {0,0,(HOOKFUNC)dcFun};
#endif

/***************************************************************************/

int
main(int argc,char **argv)
{
    register int res = RETURN_ERROR;

    if (IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library",37))
    {
        if (MUIMasterBase = OpenLibrary("muimaster.library",19))
        {
            struct data data;
            Object      *app, *win;

            if (app = ApplicationObject,
                    MUIA_Application_Title,       "CMURL",
                    MUIA_Application_Version,     "$VER: CMURL 1.2 (13.2.2004)",
                    MUIA_Application_Copyright,   "Copyright 2003 by Alfonso Ranieri",
                    MUIA_Application_Author,      "Alfonso Ranieri <alforan@tin.it>",
                    MUIA_Application_Description, "CManager.mcc example",
                    MUIA_Application_Base,        "CMURL",

                    SubWindow, win = WindowObject,
                        MUIA_Window_ID,    MAKE_ID('M','A','I','N'),
                        MUIA_Window_Title, "CMURL",

                        WindowContents, VGroup,
                            Child, HGroup,
                                Child, Label1("_URL"),
                                Child, data.pop = PopstringObject,
                                    MUIA_Popstring_Button, PopButton(MUII_PopUp),
                                    MUIA_Popstring_String, data.str = StringObject,
                                        StringFrame,
                                        MUIA_String_MaxLen, 256,
                                        MUIA_ControlChar,      'u',
                                    End,
                                    MUIA_Popstring_OpenHook,   &openHook,
                                    MUIA_Popstring_CloseHook,  &closeHook,
                                End,
                            End,
                        End,
                    End,

                    SubWindow, data.win = WindowObject,
                        MUIA_Window_ID,    MAKE_ID('P','O','P','W'),
                        MUIA_Window_Title, "Select an URL...",
                        WindowContents, data.cm = CManagerObject,
                            MUIA_CManager_AppDoubleClick, &dcHook,
                            MUIA_CManager_HideUsers,      TRUE,
                            MUIA_CManager_HideChat,       TRUE,
                            MUIA_CManager_HideTelnet,     TRUE,
                            MUIA_CManager_NoMenu,         TRUE,
                        End,
                    End,
                End)
            {
                ULONG signals;

                data.load = 0;

            	openHook.h_Data  = &data;
            	closeHook.h_Data = &data;
            	dcHook.h_Data    = &data;

                DoMethod(win,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,MUIV_Notify_Application,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);
                DoMethod(data.win,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,data.pop,2,MUIM_Popstring_Close,FALSE);

                set(win,MUIA_Window_Open,TRUE);

                for (signals = 0; DoMethod(app,MUIM_Application_NewInput,&signals)!=MUIV_Application_ReturnID_Quit; )
                    if (signals && ((signals = Wait(signals | SIGBREAKF_CTRL_C)) & SIGBREAKF_CTRL_C)) break;

                MUI_DisposeObject(app);

                res = RETURN_OK;
            }
            else
            {
                Printf("%s: can't create application\n",argv[0]);
                res = RETURN_FAIL;
            }

            CloseLibrary(MUIMasterBase);
        }
        else Printf("%s: Can't open muimaster.library ver 19 or higher\n",argv[0]);
	
        CloseLibrary(MUIMasterBase);
    }
    else Printf("%s: Can't open intuition.library ver 37 or higher\n",argv[0]);

    return res;
}

/***********************************************************************/
