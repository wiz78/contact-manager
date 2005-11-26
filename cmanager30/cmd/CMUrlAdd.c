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
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/iffparse.h>
#include <proto/muimaster.h>
#include <proto/openurl.h>
#include <proto/CManager.h>

#include <clib/alib_protos.h>
#include <datatypes/textclass.h>
#include <libraries/mui.h>

#include <string.h>
#include <stdio.h>

#include "CMURLAdd_rev.h"

/***************************************************************************/

#ifdef __MORPHOS__
long __stack = 8192;
#else
long __stack = 16000;
#endif
char __ver[] = VERSTAG;

struct IntuitionBase *IntuitionBase = NULL;
struct Library	     *UtilityBase = NULL;
struct Library       *IFFParseBase = NULL;
struct Library 	     *MUIMasterBase = NULL;
struct Library       *OpenURLBase = NULL;
struct Library       *CManagerBase = NULL;

/***************************************************************************/

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#ifndef __MORPHOS__
#define SAVEDS   __saveds
#define ASM      __asm
#define REG(x,p) register __ ## x p
#endif

/***********************************************************************/

#define MUIA_CMURL_URL        (TAG_USER+1)
#define MUIA_CMURL_EMail      (TAG_USER+2)

#define MUIM_CMURL_OpenURL    (TAG_USER+1)
#define MUIM_CMURL_AddToCM    (TAG_USER+2)
#define MUIM_CMURL_CopyToClip (TAG_USER+3)

struct MUIP_CMURL_AddToCM
{
    ULONG  MethodID;
    ULONG  name;
};

enum
{
    MUIV_CMURL_AddToCM_Name_Wait,
    MUIV_CMURL_AddToCM_Name_Get,
};

#define mainGroupObject NewObject(mainGroupClass->mcc_Class,NULL

/***********************************************************************/

#ifndef __MORPHOS__
ULONG
DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
    return DoSuperMethod(cl,obj,OM_NEW,&tag1,NULL);
}
#endif

/***********************************************************************/

Object *
owspace(ULONG weight)
{
    return RectangleObject,
    	MUIA_Weight, weight,
    End;
}

/***********************************************************************/

#ifdef __MORPHOS__
void
asprintf(STRPTR buf,STRPTR fmt,...)
{
    va_list va;

    va_start(va,fmt);
    VNewRawDoFmt(fmt,0,buf,va);
    va_end(va);
}
#else
UWORD fmtfunc[] = { 0x16c0, 0x4e75 };

void
asprintf(STRPTR to,STRPTR fmt,...)
{
    RawDoFmt(fmt,&fmt+1,(APTR)fmtfunc,to);
}
#endif

/***********************************************************************/

struct data
{
    Object *cm;
    Object *name;
    Object *add;
    STRPTR URL;
    ULONG  flags;
};

enum
{
    FLG_EMail = 1<<0,
};

/***********************************************************************/

#define FMT "Select the action to perform on \33b%s"

ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object *ourl, *cm, *clip, *cancel0, *name, *back, *add, *cancel1;
    register STRPTR URL, str;

    URL = (STRPTR)GetTagData(MUIA_CMURL_URL,NULL,msg->ops_AttrList);
    if (!URL || !*URL) return 0;

    if (!(str = AllocVec(strlen(FMT)+strlen(URL)+1,MEMF_ANY))) return 0;
    asprintf(str,FMT,URL);

    if (obj = (Object *)DoSuperNew(cl,obj,
    	    MUIA_Group_PageMode, TRUE,

	    Child, VGroup,
            	Child, FloattextObject,
                    TextFrame,
                    MUIA_Background, MUII_TextBack,
                    MUIA_Floattext_Text, str,
                End,

                Child, HGroup,
                    Child, cm = MUI_MakeObject(MUIO_Button,">C_Manager"),
                    Child, owspace(15),
                    Child, ourl = MUI_MakeObject(MUIO_Button,"_OpenURL"),
                    Child, owspace(15),
                    Child, clip = MUI_MakeObject(MUIO_Button,">C_lipboard"),
                    Child, owspace(15),
                    Child, cancel0 = MUI_MakeObject(MUIO_Button,"_Cancel"),
	        End,
	    End,

            Child, VGroup,
            	Child, VGroup,
		    GroupFrame,
    	            MUIA_Background, MUII_GroupBack,
                    Child, HGroup,
            	        Child, Label2("_Name"),
                        Child, name = StringObject,
		            StringFrame,
                            MUIA_ControlChar, 	     (ULONG)'n',
		    	    MUIA_CycleChain, 	     TRUE,
			    MUIA_String_AdvanceOnCR, TRUE,
			    MUIA_String_MaxLen,	     81,
        	        End,
	            End,
                    Child, VSpace(0),
                End,
                Child, HGroup,
                    Child, add = MUI_MakeObject(MUIO_Button,"_Add"),
                    Child, owspace(30),
                    Child, back = MUI_MakeObject(MUIO_Button,"_Back"),
                    Child, owspace(30),
                    Child, cancel1 = MUI_MakeObject(MUIO_Button,"_Cancel"),
	        End,
	    End,

            TAG_MORE, (ULONG)msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

	data->cm   = cm;
	data->name = name;
	data->add  = add;
	data->URL  = URL;

	if (GetTagData(MUIA_CMURL_EMail,FALSE,msg->ops_AttrList))
	    data->flags = FLG_EMail;

	DoSuperMethod(cl,obj,MUIM_MultiSet,MUIA_CycleChain,TRUE,
    	    ourl, cm, clip, cancel0, back, add, cancel1, NULL);

    	DoMethod(cm,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CMURL_AddToCM,MUIV_CMURL_AddToCM_Name_Wait);
    	DoMethod(ourl,MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_CMURL_OpenURL),
    	DoMethod(clip,MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_CMURL_CopyToClip),
    	DoMethod(cancel0,MUIM_Notify,MUIA_Pressed,FALSE,MUIV_Notify_Application,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);
    	DoMethod(back,MUIM_Notify,MUIA_Pressed,FALSE,obj,3,MUIM_Set,MUIA_Group_ActivePage,0);
    	DoMethod(add,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_CMURL_AddToCM,MUIV_CMURL_AddToCM_Name_Get);
    	DoMethod(name,MUIM_Notify,MUIA_String_Acknowledge,TRUE,obj,2,MUIM_CMURL_AddToCM,MUIV_CMURL_AddToCM_Name_Get);
    	DoMethod(cancel1,MUIM_Notify,MUIA_Pressed,FALSE,MUIV_Notify_Application,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);
    }

    FreeVec(str);

    return (ULONG)obj;
}

/***********************************************************************/

ULONG
mShow(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    set(_win(obj),MUIA_Window_ActiveObject,data->cm);

    return TRUE;
}

/***********************************************************************/

ULONG
mOpenURL(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!URL_OpenA(data->URL,NULL)) DisplayBeep(_screen(obj));
    DoMethod(_app(obj),MUIM_Application_PushMethod,_app(obj),2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);

    return 0;
}

/***********************************************************************/

ULONG
mCopyToClip(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data      *data = INST_DATA(cl,obj);
    register struct IFFHandle *iff;
    register ULONG	      res = FALSE;

    if (iff = AllocIFF())
    {
        if (iff->iff_Stream = (ULONG)OpenClipboard(0))
        {
            InitIFFasClip(iff);

            if (!(OpenIFF(iff,IFFF_WRITE)))
            {
                register ULONG len = strlen(data->URL);

                if (!PushChunk(iff,ID_FTXT,ID_FORM,IFFSIZE_UNKNOWN) &&
                    !PushChunk(iff,ID_FTXT,ID_CHRS,len) &&
                    (WriteChunkBytes(iff,data->URL,len)==len) &&
                    !PopChunk(iff)) res = TRUE;

                CloseIFF(iff);
            }

            CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);
        }

        FreeIFF(iff);
    }

    if (!res) DisplayBeep(_screen(obj));

    DoMethod(_app(obj),MUIM_Application_PushMethod,_app(obj),2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);

    return 0;
}

/***********************************************************************/

ULONG
mAddToCM(struct IClass *cl,Object *obj,struct MUIP_CMURL_AddToCM *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct CMEntry *entry;
    register ULONG          type;
    register STRPTR	    URL;
    STRPTR		    name;

    URL  = data->URL;

    if (msg->name==MUIV_CMURL_AddToCM_Name_Wait)
    {
    	SetSuperAttrs(cl,obj,MUIA_Group_ActivePage,1,TAG_DONE);
        set(_win(obj),MUIA_Window_ActiveObject,data->add);
	return 0;
    }

    if (msg->name==MUIV_CMURL_AddToCM_Name_Get)
    {
        get(data->name,MUIA_String_Contents,&name);
        if (!name || !*name) name = data->URL;
    }

    if (data->flags & FLG_EMail) type = CMEntry_User;
    else
    	if (!strnicmp(URL,"mailto:",7)) type = CMEntry_User;
    	else if (!strnicmp(URL,"ftp://",6)) type = CMEntry_FTP;
	     else type = CMEntry_WWW;

    if (entry = CM_AllocEntry(type))
    {
        switch (type)
        {
            case CMEntry_User:
                stccpy(CMUSER(entry)->EMail,URL,sizeof(CMUSER(entry)->EMail));
                break;

            case CMEntry_WWW:
                stccpy(CMWWW(entry)->WWW,URL,sizeof(CMWWW(entry)->WWW));
                break;

            case CMEntry_FTP:
                stccpy(CMFTP(entry)->FTP,URL,sizeof(CMFTP(entry)->FTP));
                break;
        }

        stccpy(CMUSER(entry)->Name,name,sizeof(CMUSER(entry)->Name));

        CM_AddEntry(entry);
        CM_FreeEntry(entry);
    }
    else DisplayBeep(_screen(obj));

    DoMethod(_app(obj),MUIM_Application_PushMethod,_app(obj),2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);

    return 0;
}

/***********************************************************************/

#ifdef __MORPHOS__
ULONG dispatcher(void)
{
    struct IClass *cl = (struct IClass *)REG_A0;
    Object        *obj = (Object *)REG_A2;
    Msg            msg  = (Msg)REG_A1;
#else
ULONG ASM SAVEDS dispatcher(REG(a0,struct IClass *cl),REG(a2,Object *obj),REG(a1,Msg msg))
{
#endif

    switch(msg->MethodID)
    {
        case OM_NEW:          	    return mNew(cl,obj,(APTR)msg);
        case MUIM_Show:    	    return mShow(cl,obj,(APTR)msg);
        case MUIM_CMURL_OpenURL:    return mOpenURL(cl,obj,(APTR)msg);
        case MUIM_CMURL_AddToCM:    return mAddToCM(cl,obj,(APTR)msg);
        case MUIM_CMURL_CopyToClip: return mCopyToClip(cl,obj,(APTR)msg);

        default:              	    return DoSuperMethodA(cl,obj,msg);
    }
}

#ifdef __MORPHOS__
static struct EmulLibEntry dispatcherTrap = {TRAP_LIB,0,(void *)&dispatcher};
#endif

/***********************************************************************/

#ifndef MUIA_Application_UsedClasses
#define MUIA_Application_UsedClasses 0x8042E9A7
#endif

#define TEMPLATE "URL/A,MAIL/S"

STRPTR usedClasses[] = {NULL};

int
main(int argc,char **argv)
{
    register struct RDArgs *ra;
    LONG          	   arg[8] = {0};
    register int           res = 0;

    if (ra = ReadArgs(TEMPLATE,arg,NULL))
    {
	if (IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library",36))
	{
	    if (UtilityBase = OpenLibrary("utility.library",36))
            {
	        if (IFFParseBase = OpenLibrary("iffparse.library",36))
                {
	            if (OpenURLBase = OpenLibrary("openurl.library",0))
                    {
		        if (CManagerBase = OpenLibrary("CManager.library",2))
                        {
                            if (MUIMasterBase = OpenLibrary("muimaster.library",19))
	                    {
	                    	register struct MUI_CustomClass *mainGroupClass;

			        #ifdef __MORPHOS__
		                if (mainGroupClass = MUI_CreateCustomClass(NULL,MUIC_Group,NULL,sizeof(struct data),(APTR)&dispatcherTrap))
			        #else
		                if (mainGroupClass = MUI_CreateCustomClass(NULL,MUIC_Group,NULL,sizeof(struct data),dispatcher))
			        #endif
        	                {
                    		    Object *app, *win;

	            	            if (app = ApplicationObject,
                        	            MUIA_Application_Title,        PRG,
		                            MUIA_Application_Version,      VERSTAG+1,
		                            MUIA_Application_Copyright,    "Copyright 1998-2005 by Ranieri/Tellini",
		                            MUIA_Application_Author,       "Alfonso Ranieri <alforan@tin.it>",
		                            MUIA_Application_Description,  "CManager utility.",
		                            MUIA_Application_Base,         "CMURLADD",
				            MUIA_Application_UsedClasses,  usedClasses,

		                            SubWindow, win = WindowObject,
                	                        MUIA_Window_Title, PRG,
                            		        WindowContents, mainGroupObject,
                                                    MUIA_CMURL_URL,   arg[0],
                                                    MUIA_CMURL_EMail, arg[1],
                                                End,
					    End,
				        End)
				    {
		                        ULONG sigs = 0;

		                        DoMethod(win,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,MUIV_Notify_Application,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);

		                        set(win,MUIA_Window_Open,TRUE);

		                        while (DoMethod(app,MUIM_Application_NewInput,&sigs)!=MUIV_Application_ReturnID_Quit)
		                        {
		                            if (sigs)
		                            {
		                                sigs = Wait(sigs | SIGBREAKF_CTRL_C);
		                                if (sigs & SIGBREAKF_CTRL_C) break;
		                            }
		                        }

		                        MUI_DisposeObject(app);

		                        res = RETURN_OK;
		                    }
                                    else
		                    {
		                        printf("%s: can't create application\n",argv[0]);
		                        res = RETURN_FAIL;
                    		    }
			        }

                    	        MUI_DeleteCustomClass(mainGroupClass);
	                    }
                	    else
	                    {
                	        printf("%s: can't create custom class\n",argv[0]);
	                        res = RETURN_FAIL;
                	    }

                            CloseLibrary(MUIMasterBase);
		        }
		        else
	                {
                	    printf("%s: Can't open muimaster.library ver 19 or higher\n",argv[0]);
	                    res = RETURN_ERROR;
	                }

                        CloseLibrary(OpenURLBase);
                    }
		    else
	            {
                        printf("%s: Can't open muimaster.library ver 19 or higher\n",argv[0]);
	                res = RETURN_ERROR;
	            }

                    CloseLibrary(IFFParseBase);
                }
	        else
	        {
                    printf("%s: Can't open iffparse.library ver 36 or higher\n",argv[0]);
		    res = RETURN_ERROR;
	        }

                CloseLibrary(UtilityBase);
            }
	    else
	    {
                printf("%s: Can't open utility.library ver 36 or higher\n",argv[0]);
	        res = RETURN_ERROR;
	    }

            CloseLibrary((struct Library *)IntuitionBase);
	}
	else
	{
    	    printf("%s: Can't open intuition.library ver 36 or higher\n",argv[0]);
	    res = RETURN_ERROR;
	}

    	FreeArgs(ra);
    }
    else
    {
    	PrintFault(IoErr(),argv[0]);
	res = RETURN_FAIL;
    }

    return res;
}

/***********************************************************************/
