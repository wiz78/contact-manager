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

#include "cm.h"
#include <mui/NListtree_mcc.h>

/***********************************************************************/

struct data
{
    Object                      *this;
    Object                      *win;
    Object                      *cm;
    Object                      *about;
    Object                      *aboutMUI;
    Object                      *entryList;

    struct DiskObject           *icon;
    struct FileRequester        *req;

    struct MsgPort              *reply;
    struct MsgPort              *port;
    struct MUI_InputHandlerNode ih;
    ULONG                       acceptEntry;

    UBYTE                       wtitle[64];
    UBYTE                       guide[256];

    ULONG                       flags;
};

enum
{
    FLG_handler = 1<<0,
};

/***********************************************************************/

static void
sendLibMsg(struct MsgPort *reply,struct MsgPort *to,ULONG command,APTR data)
{
    struct CMMessage msg;

    memset(&msg,0,sizeof(msg));

    msg.Msg.mn_ReplyPort = reply;
    msg.Msg.mn_Length    = sizeof(msg);
    msg.Command          = command;
    msg.Data             = (ULONG)data;

    PutMsg(to,(struct Message *)&msg);
    while (!GetMsg(reply)) WaitPort(reply);
}

/***********************************************************************/

static void
getMaskSize(struct CMEntry *entry,ULONG *mask,ULONG *size)
{
    switch (entry->Type)
    {
        case CMEntry_User:
            *mask = CMGETENTRY_User;
            *size = sizeof(struct CMUser);
            break;

        case CMEntry_WWW:
            *mask = CMGETENTRY_WWW;
            *size = sizeof(struct CMWWW);
            break;

        case CMEntry_FTP:
            *mask = CMGETENTRY_FTP;
            *size = sizeof(struct CMFTP);
            break;

        case CMEntry_Chat:
            *mask = CMGETENTRY_Chat;
            *size = sizeof(struct CMChat);
            break;

        case CMEntry_Telnet:
            *mask = CMGETENTRY_Telnet;
            *size = sizeof(struct CMTelnet);
            break;
    }
}

/***********************************************************************/

enum
{
    SSL_S, /* STRPTR */
    SSL_B, /* BOOL   */
    SSL_N, /* ULONG  */
};

static int
toUpperFun(int a)
{
    return toupper(a);
}

static void
makeName(STRPTR buf,STRPTR stem,STRPTR field,ULONG bufSize)
{
    register STRPTR t, f;
    register int    i;

    for (t = buf, f = stem, i = 2; (i<bufSize) && (*t++ = toUpperFun(*f++)); i++);

    if (i<bufSize)
    {
        *(t-1) = '.';
        i++;
    }

    for (f = field; (i<bufSize) && (*t++ = toUpperFun(*f++)); i++);

    *t = 0;
}

static ULONG
setStemList(struct RexxMsg *msg,STRPTR stem,...)
{
    va_list         ap;
    REGARRAY UBYTE  buf[512];
    register STRPTR field;

    va_start(ap,stem);

    while (field = va_arg(ap,STRPTR))
    {
        register ULONG  res, type, value;
        REGARRAY UBYTE  num[16];
    register STRPTR v = 0; // gcc

        type  = va_arg(ap,ULONG);
        value = va_arg(ap,ULONG);

        makeName(buf,stem,field,sizeof(buf));

        switch (type)
        {
            case SSL_S:
                v = (STRPTR)value;
                break;

            case SSL_B:
                v = value ? "1" : "0";
                break;

            case SSL_N:
                msprintf(num,"%ld",value);
                v = num;
                break;
        }

        if (res = SetRexxVar(msg,buf,v,strlen(v))) return res;
    }

    va_end(va);

    return 0;
}

/***********************************************************************/

M_HOOK(getEntry,Object *app,ULONG *args)
{
    register struct data *data = (struct data *)hook->h_Data;
    struct CMEntry       *entry;
    struct RexxMsg       *rxmsg;
    register ULONG       res = 0; // gcc

    DoMethod(data->entryList,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
    if (entry==NULL) return 10;

    get(data->this,MUIA_Application_RexxMsg,&rxmsg);

    switch (entry->Type)
    {
        case CMEntry_User:
            res = setStemList(rxmsg,(STRPTR)args[0],
                "TYPE",                SSL_S, "USER",
                "VISUALNAME",          SSL_S, CMUSER(entry)->VisualName,
                "NAME",                SSL_S, CMUSER(entry)->Name,
                "LASTNAME",            SSL_S, CMUSER(entry)->LastName,
                "ADDRESS",             SSL_S, CMUSER(entry)->Address,
                "CITY",                SSL_S, CMUSER(entry)->City,
                "COUNTRY",             SSL_S, CMUSER(entry)->Country,
                "ZIP",                 SSL_S, CMUSER(entry)->ZIP,
                "ALIAS",               SSL_S, CMUSER(entry)->Alias,
                "COMMENT",             SSL_S, CMUSER(entry)->Comment,
                "EMAIL",               SSL_S, CMUSER(entry)->EMail,
                "WWW",                 SSL_S, CMUSER(entry)->WWW,
                "FTP",                 SSL_S, CMUSER(entry)->FTP,
                "PHONE",               SSL_S, CMUSER(entry)->Phone,
                "FAX",                 SSL_S, CMUSER(entry)->Fax,
                "MOBILE",              SSL_S, CMUSER(entry)->Mobile,
                "ICQ",                 SSL_S, CMUSER(entry)->ICQ,
                "PGP",                 SSL_S, CMUSER(entry)->PGPUserID,
                "IMAGE",               SSL_S, CMUSER(entry)->ImageFile,
                "SEX",                 SSL_N, CMUSER(entry)->Sex,
                "BIRTHDAY",            SSL_N, CMUSER(entry)->Birthday,
                "USEBIRTHDAY",         SSL_B, CMUSER(entry)->Flags & CMUSERFLG_Birthday,
                "ANNIVERSARY",         SSL_N, CMUSER(entry)->Anniversary,
                "USEANNIVERSARY",      SSL_B, CMUSER(entry)->Flags & CMUSERFLG_Anniversary,
                "BUSINESS.COMPANY",    SSL_S, CMUSER(entry)->Company,
                "BUSINESS.JOBTITLE",   SSL_S, CMUSER(entry)->BJobTitle,
                "BUSINESS.OFFICE",     SSL_S, CMUSER(entry)->BOffice,
                "BUSINESS.ADDRESS",    SSL_S, CMUSER(entry)->BAddress,
                "BUSINESS.CITY",       SSL_S, CMUSER(entry)->BCity,
                "BUSINESS.COUNTRY",    SSL_S, CMUSER(entry)->BCountry,
                "BUSINESS.ZIP",        SSL_S, CMUSER(entry)->BZIP,
                "BUSINESS.DEPARTMENT", SSL_S, CMUSER(entry)->BDepartment,
                "BUSINESS.PHONE",      SSL_S, CMUSER(entry)->BPhone,
                "BUSINESS.FAX",        SSL_S, CMUSER(entry)->BFax,
                "BUSINESS.MOBILE",     SSL_S, CMUSER(entry)->BMobile,
                "BUSINESS.EMAIL",      SSL_S, CMUSER(entry)->BEMail,
                NULL);
            break;

        case CMEntry_WWW:
            res = setStemList(rxmsg,(STRPTR)args[0],
                "TYPE",                 SSL_S,    "WWW",
                "NAME",                 SSL_S,    CMWWW(entry)->Name,
                "WWW",                  SSL_S,    CMWWW(entry)->WWW,
                "COMMENT",              SSL_S,    CMWWW(entry)->Comment,
                "WEBMASTER",            SSL_S,    CMWWW(entry)->WebMaster,
                "EMAIL",                SSL_S,    CMWWW(entry)->EMail,
                "LASTMODIFIED.DAYS",    SSL_N,    CMWWW(entry)->LastModified.ds_Days,
                "LASTMODIFIED.MINUTE",  SSL_N,    CMWWW(entry)->LastModified.ds_Minute,
                "LASTMODIFIED.TICK",    SSL_N,    CMWWW(entry)->LastModified.ds_Tick,
                "LASTVISITED.DAYS",     SSL_N,    CMWWW(entry)->LastVisited.ds_Days,
                "LASTVISITED.MINUTE",   SSL_N,    CMWWW(entry)->LastVisited.ds_Minute,
                "LASTVISITED.TICK",     SSL_N,    CMWWW(entry)->LastVisited.ds_Tick,
                "ALIAS",                SSL_S,    CMWWW(entry)->Alias,
                NULL);
            break;

        case CMEntry_FTP:
            res = setStemList(rxmsg,(STRPTR)args[0],
                "TYPE",                 SSL_S,  "FTP",
                "NAME",                 SSL_S,  CMFTP(entry)->Name,
                "FTP",                  SSL_S,  CMFTP(entry)->FTP,
                "COMMENT",              SSL_S,  CMFTP(entry)->Comment,
                "USERNAME",             SSL_S,  CMFTP(entry)->Username,
                "PASSWORD",             SSL_S,  CMFTP(entry)->Password,
                "PORT",                 SSL_N,  CMFTP(entry)->Port,
                "RETRIES",              SSL_N,  CMFTP(entry)->Retries,
                "LOCAL",                SSL_S,  CMFTP(entry)->Local,
                "LASTMODIFIED.DAYS",    SSL_N,  CMFTP(entry)->LastModified.ds_Days,
                "LASTMODIFIED.MINUTE",  SSL_N,  CMFTP(entry)->LastModified.ds_Minute,
                "LASTMODIFIED.TICK",    SSL_N,  CMFTP(entry)->LastModified.ds_Tick,
                "LASTVISITED.DAYS",     SSL_N,  CMFTP(entry)->LastVisited.ds_Days,
                "LASTVISITED.MINUTE",   SSL_N,  CMFTP(entry)->LastVisited.ds_Minute,
                "LASTVISITED.TICK",     SSL_N,  CMFTP(entry)->LastVisited.ds_Tick,
                "ALIAS",                SSL_S,  CMFTP(entry)->Alias,
                "WINDOWSTYLE",          SSL_N,  CMFTP(entry)->WindowStyle,
                "EMAIL",                SSL_S,  CMFTP(entry)->EMail,
                "ADT",                  SSL_B,  CMFTP(entry)->Flags & CMFTPFLG_ADT,
                "USEPROXY",             SSL_B,  CMFTP(entry)->Flags & CMFTPFLG_Proxy,
                "USELOCAL",             SSL_B,  CMFTP(entry)->Flags & CMFTPFLG_Local,
                "ADVANCED",             SSL_B,  CMFTP(entry)->Flags & CMFTPFLG_Advanced,
                "QUIET",                SSL_B,  CMFTP(entry)->Flags & CMFTPFLG_Quiet,
                "COMPRESS",             SSL_B,  CMFTP(entry)->Flags & CMFTPFLG_Compress,
                "ANONYMOUS",            SSL_B,  CMFTP(entry)->Flags & CMFTPFLG_Anonymous,
                "SHOWOUTPUT",           SSL_B,  CMFTP(entry)->ExtraFlags & CMFTPEFLG_ShowFTPOutput,
                "SHOWHIDDENFILES",      SSL_B,  CMFTP(entry)->ExtraFlags & CMFTPEFLG_ShowHidden,
                "SENDNOOPS",            SSL_B,  CMFTP(entry)->ExtraFlags & CMFTPEFLG_Noops,
                NULL);
            break;

        case CMEntry_Chat:
            res = setStemList(rxmsg,(STRPTR)args[0],
                "TYPE",       SSL_S,    "CHAT",
                "CHANNEL",    SSL_S,    CMCHAT(entry)->Channel,
                "SERVER",     SSL_S,    CMCHAT(entry)->Server,
                "MAINTAINER", SSL_S,    CMCHAT(entry)->Maintainer,
                "NICK",       SSL_S,    CMCHAT(entry)->Nick,
                "WWW",        SSL_S,    CMCHAT(entry)->WWW,
                "COMMENT",    SSL_S,    CMCHAT(entry)->Comment,
                "PASSWORD",   SSL_S,    CMCHAT(entry)->Password,
                "EMAIL",      SSL_S,    CMCHAT(entry)->EMail,
                NULL);
            break;

        case CMEntry_Telnet:
            res = setStemList(rxmsg,(STRPTR)args[0],
                "TYPE",       SSL_S,    "TELNET",
                "HOST",       SSL_S,    CMTELNET(entry)->Host,
                "PORT",       SSL_N,    CMTELNET(entry)->Port,
                "LOGIN",      SSL_S,    CMTELNET(entry)->Login,
                "PASSWORD",   SSL_S,    CMTELNET(entry)->Password,
                "COMMENT",    SSL_S,    CMTELNET(entry)->Comment,
                "SSH",        SSL_B,    CMTELNET(entry)->Flags & CMTELNETFLG_SSH,
                "EMAIL",      SSL_S,    CMTELNET(entry)->EMail,
                NULL);
            break;
    }

    return res ? 20 : 0;
}

/***********************************************************************/

enum
{
    OPTS_USEFTPLOCAL,
    OPTS_FTPLOCAL,
};

M_HOOK(getAttr,Object *app,ULONG *args)
{
    register struct data *data = (struct data *)hook->h_Data;
    struct RexxMsg       *rxmsg;
    struct CMPrefs       *prefs;
    register STRPTR      val = NULL;
    register ULONG       attr;

    if (stricmp((STRPTR)args[0],"USEFTPLOCAL")==0) attr = OPTS_USEFTPLOCAL;
    else if (stricmp((STRPTR)args[0],"FTPLOCAL")==0) attr = OPTS_FTPLOCAL;
         else return 5;

    get(data->cm,MUIA_CManager_Prefs,&prefs),
    get(data->this,MUIA_Application_RexxMsg,&rxmsg);

    switch (attr)
    {
        case OPTS_USEFTPLOCAL:
            val = (prefs->FTPOptions & FTPOPT_UseLocal) ? "1" : "0";
            break;

        case OPTS_FTPLOCAL:
            val = prefs->Local;
            break;
    }

    set(data->this,MUIA_Application_RexxString,val);

    return 0;
}

/***********************************************************************/

static struct MUI_Command ARexxCmds[] =
{
    {"GETENTRY","STEM/A",1,&getEntry_hook,0},
    {"GETATTR","ATTR/A",1,&getAttr_hook,0},

    NULL
};

/***********************************************************************/

static STRPTR usedClasses[] =
{
    "BWin.mcc",
    "Calendar.mcc",
    "CManager.mcc",
    "Lamp.mcc ",
    "MiniMailer.mcc",
    "NListtree.mcc ",
    "NListviews.mcc ",
    "Popplaceholder.mcc",
    "Textinput.mcc",
    "TheBar.mcc",
    "Urltext.mcc",
    NULL
};

static struct NewMenu appMenu[] =
{
    MTITLE(Msg_Menu_Project),
        MITEM(Msg_Menu_About),
        MITEM(Msg_Menu_AboutMUI),
        MBAR,
        MITEM(Msg_Menu_Hide),
        MBAR,
        MITEM(Msg_Menu_User),
        MBAR,
        MITEM(Msg_Menu_SaveAndQuit),
        MITEM(Msg_Menu_ForceQuit),
        MITEM(Msg_Menu_Quit),

    MTITLE(Msg_Menu_Database),
        MITEM(Msg_Menu_Open),
        MITEM(Msg_Menu_Save),
        MITEM(Msg_Menu_SaveAs),
        MITEM(Msg_Menu_LastSaved),
        MBAR,
        MITEM(Msg_Menu_Add),
        MITEM(Msg_Menu_Clone),
        MITEM(Msg_Menu_Edit),
        MITEM(Msg_Menu_Remove),
        MBAR,
        MITEM(Msg_Menu_SelectAll),
        MITEM(Msg_Menu_SelectNone),
        MITEM(Msg_Menu_SelectToggle),
        MBAR,
        MITEM(Msg_Menu_Search),
        MBAR,
        MITEM(Msg_Menu_ScanHotlists),
        MITEM(Msg_Menu_Import),
            MSUBITEM(Msg_Menu_ImportSimpleMail),
            MSUBITEM(Msg_Menu_ImportYAM),
            MSUBITEM(Msg_Menu_ImportMD2),
            MSUBITEM(Msg_Menu_ImportSTRICQ),
            MSUBITEM(Msg_Menu_ImportDFA),
            MSUBITEM(Msg_Menu_ImportSTFax),
            MSUBITEM(Msg_Menu_ImportPINT),
            MSUBITEM(Msg_Menu_ImportGoodNews),
            MSUBITEM(Msg_Menu_ImportCSV),
            MSUBITEM(Msg_Menu_ImportCSVFullOE),
            MSUBBAR,
            MSUBITEM(Msg_Menu_ImportAWeb),
            MSUBITEM(Msg_Menu_ImportV),
            MSUBITEM(Msg_Menu_ImportIB),
            MSUBITEM(Msg_Menu_ImportNetscape),
            MSUBBAR,
            MSUBITEM(Msg_Menu_ImportAmFTP),
            MSUBITEM(Msg_Menu_ImportDOpus),
            MSUBITEM(Msg_Menu_ImportATC),
            MSUBBAR,
            MSUBITEM(Msg_Menu_ImportAmIRC),
        MITEM(Msg_Menu_Export),
            MSUBITEM(Msg_Menu_ExportSimpleMail),
            MSUBITEM(Msg_Menu_ExportYAM),
            MSUBITEM(Msg_Menu_ExportCSV),
            MSUBITEM(Msg_Menu_ExportCSVFullOE),
            MSUBBAR,
            MSUBITEM(Msg_Menu_ExportAWeb),
            MSUBITEM(Msg_Menu_ExportIBrowse),
            MSUBITEM(Msg_Menu_ExportNetscape),
            MSUBBAR,
            MSUBITEM(Msg_Menu_ExportURLHtml),
            MSUBITEM(Msg_Menu_ExportWWWHtml),
            MSUBBAR,
            MSUBITEM(Msg_Menu_ExportFTPHtml),
            MSUBITEM(Msg_Menu_ExportATC),

    MTITLE(Msg_Menu_Settings),
        MITEM(Msg_Menu_EditPrefs),
        MITEM(Msg_Menu_SavePrefs),
        MITEM(Msg_Menu_LastSavedPrefs),
        MBAR,
        MTITEM(Msg_Menu_NoBar),
        MTITEM(Msg_Menu_NoQuick),
        MTITEM(Msg_Menu_NoCard),
        MBAR,
        MITEM(Msg_Menu_MUI),

    MEND
};

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object           *strip, *win, *cm;
    register struct TagItem   *attrs = msg->ops_AttrList;
    struct parseArgs          *pa;
    register struct MsgPort   *port, *reply;

    pa = (struct parseArgs *)GetTagData(MUIA_CMApp_ParseArgs,NULL,attrs);

    if (pa->flags & PAFLG_Port)
    {
        if (!(reply = CreateMsgPort())) return 0;
        port = pa->port;
    }
    else port = reply = NULL; // gcc

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Application_Title,       "Contact Manager",
            MUIA_Application_Base,        "CONTACTMAN",
            MUIA_Application_Version,     &__ver[1],
            MUIA_Application_Copyright,   CM_GetString(Msg_App_Copyright),
            MUIA_Application_Description, CM_GetString(Msg_App_Description),
            MUIA_Application_Commands,    ARexxCmds,
            MUIA_Application_Menustrip,   strip = MUI_MakeObject(MUIO_MenustripNM,appMenu,MUIO_MenustripNM_CommandKeyCheck),
            MUIA_Application_UsedClasses, usedClasses,

            SubWindow, win = CMWinObject,
                MUIA_HelpNode,           "CMWIN",
                MUIA_CMApp_ParseArgs,    pa,
                //MUIA_Window_IsSubWindow, TRUE,

                WindowContents, cm = CManagerObject,
                    MUIA_CManager_AtHome,     TRUE,
                    MUIA_CManager_HideUsers,  pa->flags & PAFLG_HideUser,
                    MUIA_CManager_HideWWW,    pa->flags & PAFLG_HideWWW,
                    MUIA_CManager_HideFTP,    pa->flags & PAFLG_HideFTP,
                    MUIA_CManager_HideChat,   pa->flags & PAFLG_HideChat,
                    MUIA_CManager_HideTelnet, pa->flags & PAFLG_HideTelnet,

                End,
            End,

            TAG_MORE,attrs))
    {
        register struct data *data = INST_DATA(cl,obj);
        register Object      *o;
        STRPTR               v;
        ULONG                ver;

        /* Start setup istance data */

        data->this = obj;
        data->win  = win;
        data->cm   = cm;
        data->port = port;

        getEntry_hook.h_Data = data;
        getAttr_hook.h_Data  = data;

        get(obj,MUIA_Application_Base,&v);
        msprintf(data->wtitle,CM_GetString(Msg_Win_Title),v);
        set(win,MUIA_Window_Title,data->wtitle);

        /* Check version */
        get(cm,MUIA_Version,&ver);
        if (ver<MINCMVER)
        {
            if (data->reply = reply) sendLibMsg(reply,port,CMCOMMAND_Fail,NULL);
            CoerceMethod(cl,obj,OM_DISPOSE);
            return 0;
        }

        /* Finish setup istance data */

        data->req = MUI_AllocAslRequest(ASL_FileRequest,NULL);

        if (data->reply = reply)
        {
            data->ih.ihn_Object  = obj;
            data->ih.ihn_Signals = 1L<<reply->mp_SigBit;
            data->ih.ihn_Flags   = 0;
            data->ih.ihn_Method  = MUIM_CMApp_Message;
            DoMethod(obj,MUIM_Application_AddInputHandler,&data->ih);

            data->flags |= FLG_handler;

            sendLibMsg(data->reply,port,CMCOMMAND_Port,data->reply);
            sendLibMsg(data->reply,port,CMCOMMAND_ARexx,v);
        }

        if (data->icon = GetDiskObject("PROGDIR:CManager"))
            SetSuperAttrs(cl,obj,MUIA_Application_DiskObject,data->icon,TAG_DONE);

        get(cm,MUIA_CManager_ListObject,&data->entryList);

        /* Window notifies */
        DoMethod(win,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,2,
            MUIM_CMApp_Close,MUIV_CMApp_Close_Check);

        DoMethod(cm,MUIM_Notify,MUIA_CManager_Changed,TRUE,obj,1,MUIM_CMApp_Changed);

        /* Project notifies */
        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_About),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,1,MUIM_CMApp_About);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_AboutMUI),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_Application_AboutMUI,win);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Hide),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,3,MUIM_Set,MUIA_Application_Iconified,TRUE);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Quit),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Close,MUIV_CMApp_Close_Check);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_SaveAndQuit),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Close,MUIV_CMApp_Close_Save);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ForceQuit),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Close,MUIV_CMApp_Close_Quit);

        /* Database notifies */
        if (GenesisBase) DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_User),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,cm,1,MUIM_CManager_LoginWindow);
        else set((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_User),MUIA_Menuitem_Enabled,FALSE);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Open),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,1,MUIM_CMApp_Open);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Save),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,cm,2,MUIM_CManager_SaveData,NULL);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_SaveAs),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,1,MUIM_CMApp_SaveAs);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_LastSaved),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,cm,1,MUIM_CManager_Reload);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Add),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,cm,1,MUIM_CManager_AddItem);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Clone),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,data->entryList,1,MUIM_EntryList_Clone);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Edit),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,data->entryList,1,MUIM_EntryList_Edit);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Remove),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,data->entryList,1,MUIM_EntryList_Remove);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_SelectAll),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,data->entryList,3,MUIM_NList_Select,MUIV_NList_Select_All,MUIV_NList_Select_On,NULL);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_SelectNone),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,data->entryList,3,MUIM_NList_Select,MUIV_NList_Select_All,MUIV_NList_Select_Off,NULL);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_SelectToggle),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,data->entryList,3,MUIM_NList_Select,MUIV_NList_Select_All,MUIV_NList_Select_Toggle,NULL);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_Search),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,cm,1,MUIM_CManager_Search);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ScanHotlists),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,1,MUIM_CMApp_ScanHotlists);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportV),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_Voyager);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportIB),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_IB);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportAWeb),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_AWeb);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportNetscape),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_Netscape);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportAmFTP),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_AmFTP);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportDOpus),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_DOpusFTP);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportATC),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_ATC);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportMD2),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_MD);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportSimpleMail),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_SimpleMail);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportYAM),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_YAM);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportSTRICQ),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_STRICQ);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportDFA),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_DFA);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportPINT),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_PINT);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportGoodNews),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_GoodNews);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportSTFax),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_STFax);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportAmIRC),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_AmIRC);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportCSV),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_Users_CSV);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ImportCSVFullOE),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Import,MUIV_CManager_Import_Users_CSVFullOE);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportSimpleMail),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_SimpleMail);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportYAM),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_YAM);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportAWeb),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_AWeb);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportIBrowse),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_IBrowse);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportNetscape),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_Netscape);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportCSV),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_CSV_Users);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportCSVFullOE),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_CSVFullOE_Users);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportURLHtml),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_HTML_URLs);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportWWWHtml),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_HTML_WWW);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportFTPHtml),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_HTML_FTP);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_ExportATC),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_CMApp_Export,MUIV_CManager_Export_ATC);

        /* Settings notifies */
        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_EditPrefs),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,cm,2,MUIM_CManager_PrefsWindow,MUIV_PrefsWindow_ActivePage_NoPage);

        DoMethod(o = (Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_NoBar),MUIM_Notify,
            MUIA_Menuitem_Checked,MUIV_EveryTime,cm,3,MUIM_Set,MUIA_CManager_NoBar,MUIV_TriggerValue);
        DoMethod(cm,MUIM_Notify,MUIA_CManager_NoBar,MUIV_EveryTime,o,3,MUIM_Set,MUIA_Menuitem_Checked,MUIV_TriggerValue);

        o = (Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_NoQuick);
        set(o,MUIA_Menuitem_Checked,TRUE);
        DoMethod(o,MUIM_Notify,MUIA_Menuitem_Checked,MUIV_EveryTime,cm,3,MUIM_Set,MUIA_CManager_NoQuick,MUIV_TriggerValue);
        DoMethod(cm,MUIM_Notify,MUIA_CManager_NoQuick,MUIV_EveryTime,o,3,MUIM_Set,MUIA_Menuitem_Checked,MUIV_TriggerValue);

        o = (Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_NoCard);
        set(o,MUIA_Menuitem_Checked,TRUE);
        DoMethod(o,MUIM_Notify,MUIA_Menuitem_Checked,MUIV_EveryTime,cm,3,MUIM_Set,MUIA_CManager_NoCard,MUIV_TriggerValue);
        DoMethod(cm,MUIM_Notify,MUIA_CManager_NoCard,MUIV_EveryTime,o,3,MUIM_Set,MUIA_Menuitem_Checked,MUIV_TriggerValue);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_SavePrefs),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,cm,3,MUIM_CManager_SetPrefs,NULL,MUIV_CManager_SetPrefs_SaveCurrent);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_LastSavedPrefs),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,cm,1,MUIM_CManager_LoadPrefs);

        DoMethod((Object *)DoMethod(strip,MUIM_FindUData,Msg_Menu_MUI),MUIM_Notify,
            MUIA_Menuitem_Trigger,MUIV_EveryTime,obj,2,MUIM_Application_OpenConfigWindow,0);

        /* Setup online help */
        get(cm,MUIA_CManager_Path,&v);
        strcpy(data->guide,v);
        AddPart(data->guide,"Docs/CManager.guide",sizeof(data->guide));
        SetSuperAttrs(cl,obj,MUIA_Application_HelpFile,data->guide,TAG_DONE);
        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_Application_MenuHelp,MUIV_EveryTime,MUIV_Notify_Self,
            5,MUIM_Application_ShowHelp,win,data->guide,"CMMENUS",0);
    }
    else
    {
        if (reply)
        {
            sendLibMsg(reply,port,CMCOMMAND_Fail,NULL);
            DeleteMsgPort(reply);
        }
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data          *data = INST_DATA(cl,obj);
    register struct MsgPort       *reply = data->reply, *port = data->port;
    register struct DiskObject    *icon = data->icon;
    register struct FileRequester *req = data->req;
    register ULONG                res, acceptEntry = data->acceptEntry;

    if (data->flags & FLG_handler) DoMethod(obj,MUIM_Application_RemInputHandler,&data->ih);

    res = DoSuperMethodA(cl,obj,msg);

    if (icon) FreeDiskObject(icon);
    if (req) MUI_FreeAslRequest(req);

    if (reply)
    {
        register struct Message *cmsg;

        while (cmsg = GetMsg(reply)) ReplyMsg(cmsg);
        if (acceptEntry) sendLibMsg(reply,port,CMCOMMAND_GetEntry,NULL);
        DeleteMsgPort(reply);
    }

    return res;
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    register struct data *data = INST_DATA(cl, obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_CMApp_Win: *msg->opg_Storage = (ULONG)data->win; return TRUE;
        case MUIA_CMApp_CM:  *msg->opg_Storage = (ULONG)data->cm; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***************************************************************************/

static ULONG
mAbout(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,TRUE,TAG_DONE);

    if (!data->about)
    {
        if (CMAboutClass || initCMAboutClass())
        {
            STRPTR mccver;

            get(data->cm,MUIA_CManager_Version,&mccver);

            if (data->about = CMAboutObject,
                    MUIA_HelpNode,             "CMABOUT",
                    MUIA_Aboutmui_Application, obj,
                    MUIA_Window_RefWindow,     data->win,
                    MUIA_CManager_Version,     mccver,
                    MUIA_CMApp_CM,             data->cm,
                End)
                DoMethod(data->about,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,5,
                    MUIM_Application_PushMethod,obj,2,MUIM_CMApp_DisposeWin,data->about);
        }
    }

    openWindow(obj,data->about);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,FALSE,TAG_DONE);

    return 0;
}

/***********************************************************************/

static ULONG
mAboutMUI(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,TRUE,TAG_DONE);

    if (!data->aboutMUI)
    {
        if (data->aboutMUI = AboutmuiObject,
                MUIA_Aboutmui_Application, obj,
                MUIA_Window_RefWindow,     data->win,
                MUIA_HelpNode,             "CMMUI",
            End)
            DoMethod(data->aboutMUI,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,5,
                MUIM_Application_PushMethod,obj,2,MUIM_CMApp_DisposeWin,data->aboutMUI);
    }

    openWindow(obj,data->aboutMUI);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,FALSE,TAG_DONE);

    return 0;
}

/***********************************************************************/

static ULONG
mOpenMUIConfigWindow(struct IClass *cl,Object *obj,Msg msg)
{
    register ULONG res;

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,TRUE,TAG_DONE);
    res = DoSuperMethodA(cl,obj,msg);
    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,FALSE,TAG_DONE);

    return res;
}

/***********************************************************************/

static ULONG
mCMAppDisposeWin(struct IClass *cl,Object *obj,struct MUIP_CMApp_DisposeWin *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register Object      *win = msg->win;

    set(win,MUIA_Window_Open,FALSE);
    DoSuperMethod(cl,obj,OM_REMMEMBER,win);
    MUI_DisposeObject(win);

    if (win==data->about) data->about = NULL;
    else if (win==data->aboutMUI) data->aboutMUI = NULL;

    return 0;
}

/***********************************************************************/

static ULONG
mClose(struct IClass *cl,Object *obj,struct MUIP_CMApp_Close *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    DoMethod(data->cm,MUIM_CManager_Close,msg->mode);

    return 0;
}

/***********************************************************************/

#ifdef __MORPHOS__
void
reqIntuiFun(void)
{
    struct Hook 	*hook = (struct Hook *)REG_A0;
    struct IntuiMessage *imsg = (struct IntuiMessage *)REG_A1;
#else
void SAVEDS ASM
reqIntuiFun(REG(a0,struct Hook *hook),REG(a2,struct FileRequester req),REG(a1,struct IntuiMessage *imsg))
{
#endif
    if (imsg->Class==IDCMP_REFRESHWINDOW)
    	DoMethod(hook->h_Data,MUIM_Application_CheckRefresh);
}

#ifdef __MORPHOS__
static struct EmulLibEntry reqIntuiTrap = {TRAP_LIBNR,0,(void (*)(void))reqIntuiFun};
#endif

static ULONG
getFile(struct data *data,STRPTR file,ULONG fileSize,STRPTR pattern,ULONG saveMode)
{
    ULONG ok = FALSE;

    if (data->req)
    {
        struct Hook reqIntuiHook;
        TEXT  	    dir[256];

        #ifdef __MORPHOS__
        reqIntuiHook.h_Entry    = (HOOKFUNC)&reqIntuiTrap;
        #else
        reqIntuiHook.h_Entry    = (HOOKFUNC)reqIntuiFun;
	#endif
        reqIntuiHook.h_SubEntry = NULL;
    	reqIntuiHook.h_Data     = data->this;

        strcpy(dir,file);
        *(PathPart(dir)) = '\0';

        if (MUI_AslRequestTags(data->req,
                               ASLFR_Window,        				(ULONG)_window(data->cm),
                               ASLFR_PrivateIDCMP,  				TRUE,
	                       ASLFR_IntuiMsgFunc,  			        (ULONG)&reqIntuiHook,
                               ASLFR_Flags1,        				FRF_INTUIFUNC,
                               ASLFR_DoPatterns,                                TRUE,
                               ASLFR_DoSaveMode,                                saveMode,
                               pattern ? ASLFR_InitialPattern : TAG_IGNORE,     pattern,
                               file[0] ? ASLFR_InitialFile : TAG_IGNORE,        FilePart(file),
                               file[0] ? ASLFR_InitialDrawer : TAG_IGNORE,      dir,
                               TAG_DONE))
        {
            strcpy(file,data->req->fr_Drawer);
            AddPart(file,data->req->fr_File,fileSize);

            ok = TRUE;
        }
    }

    return ok;
}

/***********************************************************************/

static ULONG
mCMAppImport(struct IClass *cl,Object *obj,struct MUIP_CMApp_Import *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    REGARRAY UBYTE       file[256];
    register STRPTR      fname, pattern;
    struct CMPrefs       *prefs;

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,TRUE,TAG_DONE);

    get(data->cm,MUIA_CManager_Prefs,&prefs);

    switch (msg->type)
    {
        case MUIV_CManager_Import_Voyager:
            pattern = "Bookmarks.html";
            fname   = prefs->Hotlists[HL_V];
            break;

        case MUIV_CManager_Import_IB:
            pattern = "#?.html";
            fname   = prefs->Hotlists[HL_IB];
            break;

        case MUIV_CManager_Import_AWeb:
            pattern = "#?.hotlist";
            fname   = prefs->Hotlists[HL_AWEB];
            break;

        case MUIV_CManager_Import_Netscape:
            pattern = "#?.(htm|html)";
            fname   = "";
            break;

        case MUIV_CManager_Import_AmFTP:
            pattern = "#?.AmFTPProfiles";
            fname   = prefs->Hotlists[HL_AMFTP];
            break;

        case MUIV_CManager_Import_MD:
            pattern = "#?.AddressBook";
            fname   = prefs->Hotlists[HL_MD2];
            break;

        case MUIV_CManager_Import_YAM:
            pattern = "#?.AddressBook";
            fname   = prefs->Hotlists[HL_YAM];
            break;

        case MUIV_CManager_Import_SimpleMail:
            pattern = "#?.xml";
            fname   = prefs->Hotlists[HL_SIMPLEMAIL];
            break;

        case MUIV_CManager_Import_STFax:
            pattern = "#?";
            fname   = prefs->Hotlists[HL_STFAX];
            break;

        case MUIV_CManager_Import_AmIRC:
            pattern = "#?.AmIRCfg";
            fname   = prefs->Hotlists[HL_AMIRC];
            break;

        case MUIV_CManager_Import_DOpusFTP:
            pattern = "#?";
            fname   = prefs->Hotlists[HL_DOPUS];
            break;

        case MUIV_CManager_Import_ATC:
            pattern = "#?.profiles#?";
            fname   = "";
            break;

        case MUIV_CManager_Import_DFA:
            pattern = "#?.dfa";
            fname   = prefs->Hotlists[HL_DFA];
            break;

        case MUIV_CManager_Import_PINT:
            pattern = "#?.addresses";
            fname   = prefs->Hotlists[HL_PINT];
            break;

        case MUIV_CManager_Import_GoodNews:
            pattern = "#?.addresses";
            fname   = prefs->Hotlists[HL_GOODNEWS];
            break;

        case MUIV_CManager_Import_STRICQ:
            pattern = "Contacts#?";
            fname   = prefs->Hotlists[HL_STRICQ];
            break;

        default:
            pattern = "#?.csv";
            fname   = "";
            break;
    }

    strcpy(file,fname);

    if (getFile(data,file,sizeof(file),pattern,FALSE))
        DoMethod(data->cm,MUIM_CManager_Import,msg->type,file,MUIV_CManager_Import_AvoidDuplicate);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,FALSE,TAG_DONE);

    return 0;
}

/***********************************************************************/

static ULONG
mCMAppExport(struct IClass *cl,Object *obj,struct MUIP_CMApp_Export *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    REGARRAY UBYTE       file[256];
    register STRPTR      pattern = NULL; // gcc

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,TRUE,TAG_DONE);

    *file = 0;

    switch (msg->type)
    {
        case MUIV_CManager_Export_HTML_URLs:
            pattern = "#?.html";
            break;

        case MUIV_CManager_Export_HTML_WWW:
            pattern = "#?.html";
            break;

        case MUIV_CManager_Export_HTML_FTP:
            pattern = "#?.html";
            break;

        case MUIV_CManager_Export_CSV_Users:
        case MUIV_CManager_Export_CSVFullOE_Users:
            pattern = "#?.csv";
            break;

        case MUIV_CManager_Export_YAM:
            pattern = "#?.AddressBook";
            break;

        case MUIV_CManager_Export_SimpleMail:
            pattern = "#?.xml";
            break;

        case MUIV_CManager_Export_AWeb:
            pattern = "#?.hotlist";
            break;

        case MUIV_CManager_Export_IBrowse:
            pattern = "#?.html";
            break;

        case MUIV_CManager_Export_Netscape:
            pattern = "#?.htm";
            break;

        case MUIV_CManager_Export_ATC:
            pattern = "#?.profiles";
            break;
    }

    if (getFile(data,file,sizeof(file),pattern,TRUE))
        DoMethod(data->cm,MUIM_CManager_Export,msg->type,file,0);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,FALSE,TAG_DONE);

    return 0;
}

/***********************************************************************/

static ULONG
getHotName(STRPTR buf,ULONG id)
{
    register STRPTR f, t;

    switch (1<<id)
    {
        case MUIV_CManager_ScanHotlists_IB:         id = Msg_PrefsWin_Hotlists_IB;         break;
        case MUIV_CManager_ScanHotlists_V:          id = Msg_PrefsWin_Hotlists_V;          break;
        case MUIV_CManager_ScanHotlists_AWEB:       id = Msg_PrefsWin_Hotlists_AWeb;       break;
        case MUIV_CManager_ScanHotlists_AMFTP:      id = Msg_PrefsWin_Hotlists_IB;         break;
        case MUIV_CManager_ScanHotlists_MD2:        id = Msg_PrefsWin_Hotlists_MD2;        break;
        case MUIV_CManager_ScanHotlists_STFAX:      id = Msg_PrefsWin_Hotlists_STFax;      break;
        case MUIV_CManager_ScanHotlists_YAM:        id = Msg_PrefsWin_Hotlists_YAM;        break;
        case MUIV_CManager_ScanHotlists_SIMPLEMAIL: id = Msg_PrefsWin_Hotlists_SimpleMail; break;
        case MUIV_CManager_ScanHotlists_AMIRC:      id = Msg_PrefsWin_Hotlists_AmIRC;      break;
        case MUIV_CManager_ScanHotlists_DOPUS:      id = Msg_PrefsWin_Hotlists_DOpus;      break;
        case MUIV_CManager_ScanHotlists_DFA:        id = Msg_PrefsWin_Hotlists_DFA;        break;
        case MUIV_CManager_ScanHotlists_PINT:       id = Msg_PrefsWin_Hotlists_PINT;       break;
        case MUIV_CManager_ScanHotlists_GOODNEWS:   id = Msg_PrefsWin_Hotlists_GoodNews;   break;
        case MUIV_CManager_ScanHotlists_STRICQ:     id = Msg_PrefsWin_Hotlists_STRICQ;     break;
        case MUIV_CManager_ScanHotlists_NETSCAPE:   id = Msg_PrefsWin_Hotlists_Netscape;   break;
        default: return FALSE;
    }

    for (f = CM_GetString(id), t = buf; *f; f++)
    {
        if (*f!='_') *t++ = *f;
    }

    *t = 0;

    return TRUE;
}

/***********************************************************************/

static ULONG
mCMAppScanHotlists(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMPrefs       *prefs;
    register ULONG       flags;

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,TRUE,TAG_DONE);

    get(data->cm,MUIA_CManager_Prefs,&prefs);

    DoMethod(data->cm,MUIM_CManager_GrabLists);

    if (flags = DoMethod(data->cm,MUIM_CManager_ScanHotlists,MUIV_CManager_ScanHotlists_Check,0))
    {
        register int i;

        for (i = 0; i<NUM_HOTLISTS; i++)
        {
            if (flags & (1<<i))
            {
                REGARRAY UBYTE buf[32];

                if (getHotName(buf,i))
                {
                    if (!MUI_Request(obj,NULL,0,CM_GetString(Msg_Req_Title),CM_GetString(Msg_Req_HotChangedGads),CM_GetString(Msg_Req_HotChangedMsg),buf,prefs->Hotlists[i]))
                        flags &= ~(1<<i);
                }
            }
        }

        if (flags) DoMethod(data->cm,MUIM_CManager_ScanHotlists,MUIV_CManager_ScanHotlists_Import|MUIV_CManager_ScanHotlists_SaveDates,flags);
        else DoMethod(data->cm,MUIM_CManager_ScanHotlists,MUIV_CManager_ScanHotlists_SaveDates,0);
    }

    DoMethod(data->cm,MUIM_CManager_ReinsertLists);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,FALSE,TAG_DONE);

    return 0;
}

/***********************************************************************/

static ULONG
mCMAppMessage(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data      *data = INST_DATA(cl,obj);
    register struct CMMessage *cmsg;

    while (cmsg = (struct CMMessage *)GetMsg(data->reply))
    {
        if (cmsg->Msg.mn_Node.ln_Type!=NT_REPLYMSG)
        {
            switch(cmsg->Command)
            {
                case CMCOMMAND_Quit:
                    DoMethod(obj,MUIM_Application_PushMethod,obj,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);
                    break;

                case CMCOMMAND_GetEntry:
                    data->acceptEntry = cmsg->Data;
                    set(data->cm,MUIA_CManager_DoubleClickObject,obj);
                    break;
            }
        }

        ReplyMsg((struct Message *)cmsg);
    }

    return 0;
}

/***********************************************************************/

static ULONG
mCMAppOpen(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    REGARRAY UBYTE   file[256];
    struct CMData        *cm;

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,TRUE,TAG_DONE);

    get(data->cm,MUIA_CManager_CMData,&cm);

    strcpy(file,cm->Path);

    if (getFile(data,file,sizeof(file),NULL,FALSE))
        DoMethod(data->cm,MUIM_CManager_LoadData,NULL,file);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,FALSE,TAG_DONE);

    return 0;
}

/***********************************************************************/

static ULONG
mCMAppChanged(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMPrefs       *prefs;

    get(data->cm,MUIA_CManager_Prefs,&prefs);
    if (prefs->Options & OPT_SaveOnChanges) DoMethod(data->cm,MUIM_CManager_SaveData,NULL);

    return 0;
}

/***********************************************************************/

static ULONG
mCMAppDoubleClick(struct IClass *cl,Object *obj,struct MUIP_CManager_DoubleClick *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct CMEntry *entry = msg->entry;

    if (data->acceptEntry & CMGETENTRY_Multi)
    {
        register struct MinList *list;

        if (list = CM_AllocObject(CMALLOCOBJECT_List))
        {
            LONG pos;

            pos = MUIV_NList_NextSelected_Start;

            DoMethod(data->entryList,MUIM_NList_Select,MUIV_NList_Select_Active,MUIV_NList_Select_On,NULL);
            DoMethod(data->entryList,MUIM_NList_NextSelected,&pos);

            while (pos!=MUIV_NList_NextSelected_End)
            {
                struct CMEntry *from, *to;
                ULONG          mask, size;

                DoMethod(data->entryList,MUIM_NList_GetEntry,pos,&from);
                getMaskSize(from,&mask,&size);

                if (!(data->acceptEntry & mask))
                {
                    CM_FreeList(list);
                    return 0;
                }

                if (to = CM_AllocEntry(from->Type))
                {
                    copymem(to,from,size);
                    AddTail((struct List *)list,(struct Node *)to);
                }

                DoMethod(data->entryList,MUIM_NList_NextSelected,&pos);
            }

            sendLibMsg(data->reply,data->port,CMCOMMAND_GetEntry,list);

            data->acceptEntry = 0;

            set(data->cm,MUIA_CManager_DoubleClickObject,NULL);
        }
    }
    else
        if (entry && data->acceptEntry)
        {
            ULONG mask = 0, size;

            getMaskSize(entry,&mask,&size);

            if (data->acceptEntry & mask)
            {
                register struct CMEntry *nentry;

                if (nentry = CM_AllocEntry(entry->Type))
                    copymem(nentry,entry,size);

                sendLibMsg(data->reply,data->port,CMCOMMAND_GetEntry,nentry);

                data->acceptEntry = 0;

                set(data->cm,MUIA_CManager_DoubleClickObject,NULL);
            }
        }

    return 0;
}

/***********************************************************************/

static ULONG
mCMAppSaveAs(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    REGARRAY UBYTE       file[256];

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,TRUE,TAG_DONE);

    *file = 0;

    if (getFile(data,file,sizeof(file),NULL,TRUE))
        DoMethod(data->cm,MUIM_CManager_SaveData,file);

    SetSuperAttrs(cl,obj,MUIA_Application_Sleep,FALSE,TAG_DONE);

    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                            return mNew(cl,obj,(APTR)msg);
        case OM_GET:                            return mGet(cl,obj,(APTR)msg);
        case OM_DISPOSE:                        return mDispose(cl,obj,(APTR)msg);

        case MUIM_Application_AboutMUI:         return mAboutMUI(cl,obj,(APTR)msg);
        case MUIM_Application_OpenConfigWindow: return mOpenMUIConfigWindow(cl,obj,(APTR)msg);

        case MUIM_CMApp_About:                  return mAbout(cl,obj,(APTR)msg);
        case MUIM_CMApp_DisposeWin:             return mCMAppDisposeWin(cl,obj,(APTR)msg);
        case MUIM_CMApp_Close:                  return mClose(cl,obj,(APTR)msg);
        case MUIM_CMApp_Import:                 return mCMAppImport(cl,obj,(APTR)msg);
        case MUIM_CMApp_Export:                 return mCMAppExport(cl,obj,(APTR)msg);
        case MUIM_CMApp_ScanHotlists:           return mCMAppScanHotlists(cl,obj,(APTR)msg);
        case MUIM_CMApp_Message:                return mCMAppMessage(cl,obj,(APTR)msg);
        case MUIM_CMApp_Open:                   return mCMAppOpen(cl,obj,(APTR)msg);
        case MUIM_CMApp_Changed:                return mCMAppChanged(cl,obj,(APTR)msg);
        case MUIM_CMApp_SaveAs:                 return mCMAppSaveAs(cl,obj,(APTR)msg);

        case MUIM_CManager_DoubleClick:         return mCMAppDoubleClick(cl,obj,(APTR)msg);

        default:                                return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initCMAppClass(void)
{
    if (CMAppClass = MUI_CreateCustomClass(NULL,MUIC_Application,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        localizeMenus(appMenu);

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/
