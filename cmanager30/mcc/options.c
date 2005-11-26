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

/***********************************************************************/

#define DEF_PrefsFile          "CManager.prefs"
#define DEF_HotlistStampsFile  "Hotlist.stamps"

/* Locale */
#define ID_COUNTRY          MAKE_ID('L','C','O','N')
#define ID_COUNTRYCODE      MAKE_ID('L','C','I','D')
#define ID_CODESET          MAKE_ID('L','C','S','T')

/* Hotlists */
#define ID_HL_IB            MAKE_ID('H','I','B','R')
#define ID_HL_V             MAKE_ID('H','V','O','Y')
#define ID_HL_AWEB          MAKE_ID('H','A','W','B')
#define ID_HL_AMFTP         MAKE_ID('H','A','M','F')
#define ID_HL_MD2           MAKE_ID('H','M','D','2')
#define ID_HL_STFAX         MAKE_ID('H','S','T','F')
#define ID_HL_YAM           MAKE_ID('H','Y','A','M')
#define ID_HL_AMIRC         MAKE_ID('H','A','M','I')
#define ID_HL_DOPUS         MAKE_ID('H','D','O','P')
#define ID_HL_DFA           MAKE_ID('H','D','F','A')
#define ID_HL_PINT          MAKE_ID('H','P','I','N')
#define ID_HL_GOODNEWS      MAKE_ID('H','G','N','W')
#define ID_HL_STRICQ        MAKE_ID('H','I','C','Q')
#define ID_HL_NETSCAPE      MAKE_ID('H','F','I','F')

/* Options */
#define ID_DELWARNING       MAKE_ID('O','D','W','R')
#define ID_ACTION           MAKE_ID('O','D','C','K')
#define ID_GROUPSSTATUS     MAKE_ID('O','G','R','S')
#define ID_OPTIONS          MAKE_ID('O','P','T','S')

/* Dialer */
#define ID_DIALOPTIONS      MAKE_ID('D','O','P','T')
#define ID_SERDEVICE        MAKE_ID('D','D','E','V')
#define ID_SERINIT          MAKE_ID('D','I','N','T')
#define ID_DIALCOMMAND      MAKE_ID('D','C','M','D')
#define ID_DIALPREFIX       MAKE_ID('D','P','R','E')
#define ID_DIALSUFFIX       MAKE_ID('D','S','U','F')
#define ID_SERUNIT          MAKE_ID('D','U','N','I')
#define ID_REDIALDELAY      MAKE_ID('D','D','L','Y')
#define ID_REDIALATTEMPTS   MAKE_ID('D','A','T','P')
#define ID_MODEMTYPE        MAKE_ID('D','M','D','M')
#define ID_DIALPROG         MAKE_ID('D','P','R','G')

/* Actions */
#define ID_WWWPROG          MAKE_ID('A','W','P','R')
#define ID_WWWPATH          MAKE_ID('A','W','P','A')
#define ID_WWWSCRIPT        MAKE_ID('A','W','S','C')

#define ID_FTPOPTIONS       MAKE_ID('A','F','O','P')
#define ID_FTPPROG          MAKE_ID('A','F','P','R')
#define ID_FTPPATH          MAKE_ID('A','F','P','A')
#define ID_FTPSCRIPT        MAKE_ID('A','F','S','C')
#define ID_FTPLOCAL         MAKE_ID('A','F','L','O')

#define ID_MAILPROG         MAKE_ID('A','M','P','R')
#define ID_MAILPATH         MAKE_ID('A','M','P','A')
#define ID_MAILSCRIPT       MAKE_ID('A','M','S','C')

#define ID_CHATPROG         MAKE_ID('A','C','P','R')
#define ID_CHATPATH         MAKE_ID('A','C','P','A')
#define ID_CHATSCRIPT       MAKE_ID('A','C','S','C')

#define ID_FAXPROG          MAKE_ID('A','X','P','R')
#define ID_FAXPATH          MAKE_ID('A','X','P','A')
#define ID_FAXSCRIPT        MAKE_ID('A','X','S','C')

#define ID_TELNETPROG       MAKE_ID('A','T','P','R')
#define ID_TELNETPATH       MAKE_ID('A','T','P','A')
#define ID_TELNETSCRIPT     MAKE_ID('A','T','S','C')

#define ID_SAVEDPROG        MAKE_ID('A','S','P','R')

/* VisualPrefs */
#define ID_VISUALPREFS      MAKE_ID('V','P','R','F')

/* Card */
#define ID_CARDBACK         MAKE_ID('C','B','C','K')
#define ID_TITLESPEN        MAKE_ID('C','T','P','N')
#define ID_TITLESBACKPEN    MAKE_ID('C','T','B','P')
#define ID_URLPEN           MAKE_ID('C','U','P','N')
#define ID_URLOVERPEN       MAKE_ID('C','U','O','P')
#define ID_URLOVERPEN       MAKE_ID('C','U','O','P')
#define ID_MINIATUREFRAME   MAKE_ID('C','M','F','R')
#define ID_MINIATURESIZE    MAKE_ID('C','M','S','Z')
#define ID_CARDOPTIONS      MAKE_ID('C','O','P','T')
//#define ID_CARDFONT         MAKE_ID('C','F','O','N')
//#define ID_TITLESFONT       MAKE_ID('C','T','F','O')

struct saveHots
{
    LONG  index;
    ULONG id;
};

static ULONG ids[] =
{
    ID_COUNTRY,
    ID_COUNTRYCODE,
    ID_CODESET,

    ID_HL_IB,
    ID_HL_NETSCAPE,
    ID_HL_V,
    ID_HL_AWEB,
    ID_HL_AMFTP,
    ID_HL_MD2,
    ID_HL_STFAX,
    ID_HL_YAM,
    ID_HL_AMIRC,
    ID_HL_DOPUS,
    ID_HL_DFA,
    ID_HL_PINT,
    ID_HL_GOODNEWS,
    ID_HL_STRICQ,

    ID_DELWARNING,
    ID_ACTION,
    ID_OPTIONS,

    ID_DIALOPTIONS,
    ID_SERDEVICE,
    ID_SERINIT,
    ID_DIALCOMMAND,
    ID_DIALPREFIX,
    ID_DIALSUFFIX,
    ID_SERUNIT,
    ID_REDIALDELAY,
    ID_REDIALATTEMPTS,
    ID_MODEMTYPE,
    ID_DIALPROG,

    ID_WWWPROG,
    ID_WWWPATH,
    ID_WWWSCRIPT,

    ID_FTPOPTIONS,
    ID_FTPPROG,
    ID_FTPPATH,
    ID_FTPSCRIPT,
    ID_FTPLOCAL,

    ID_MAILPROG,
    ID_MAILPATH,
    ID_MAILSCRIPT,

    ID_CHATPROG,
    ID_CHATPATH,
    ID_CHATSCRIPT,

    ID_FAXPROG,
    ID_FAXPATH,
    ID_FAXSCRIPT,

    ID_TELNETPROG,
    ID_TELNETPATH,
    ID_TELNETSCRIPT,

    ID_SAVEDPROG,

    ID_VISUALPREFS,
    ID_GROUPSSTATUS,

    ID_CARDBACK,
    ID_TITLESPEN,
    ID_TITLESBACKPEN,
    ID_URLPEN,
    ID_URLOVERPEN,
    ID_MINIATUREFRAME,
    ID_MINIATURESIZE,
    ID_CARDOPTIONS,
    //ID_CARDFONT,
    //ID_TITLESFONT,

    0
};

static struct saveHots hotIds[] =
{
    HL_IB,          ID_HL_IB,
    HL_NETSCAPE,    ID_HL_NETSCAPE,
    HL_V,           ID_HL_V,
    HL_AWEB,        ID_HL_AWEB,
    HL_AMFTP,       ID_HL_AMFTP,
    HL_MD2,         ID_HL_MD2,
    HL_STFAX,       ID_HL_STFAX,
    HL_YAM,         ID_HL_YAM,
    HL_AMIRC,       ID_HL_AMIRC,
    HL_DOPUS,       ID_HL_DOPUS,
    HL_DFA,         ID_HL_DFA,
    HL_PINT,        ID_HL_PINT,
    HL_GOODNEWS,    ID_HL_GOODNEWS,
    HL_STRICQ,      ID_HL_STRICQ,
    -1,
};

/***********************************************************************/

void
setDefaultVisualPrefs(struct VisualPrefs *VisualPrefs)
{
    register int i;

    VisualPrefs->ListPrefs.TreeW = DEF_TreeW;
    VisualPrefs->ListPrefs.ListW = DEF_ListW;

    VisualPrefs->ListPrefs.ListCW = DEF_ListCW;
    VisualPrefs->ListPrefs.CardCW = DEF_CardCW;

    VisualPrefs->ListPrefs.Sorts[IDX_User].SortCol0 = VisualPrefs->ListPrefs.Sorts[IDX_WWW].SortCol0  =
        VisualPrefs->ListPrefs.Sorts[IDX_FTP].SortCol0 = VisualPrefs->ListPrefs.Sorts[IDX_Chat].SortCol0 =
        VisualPrefs->ListPrefs.Sorts[IDX_Telnet].SortCol0 = 0;

    VisualPrefs->ListPrefs.Sorts[IDX_User].SortCol1 = VisualPrefs->ListPrefs.Sorts[IDX_WWW].SortCol1 =
        VisualPrefs->ListPrefs.Sorts[IDX_FTP].SortCol1 = VisualPrefs->ListPrefs.Sorts[IDX_Chat].SortCol1 = 1;

    for (i = 0; i<LP_NUMCOLS; i++)
    {
        VisualPrefs->ListPrefs.Sorts[IDX_User].Order[i]   = VisualPrefs->ListPrefs.Sorts[IDX_WWW].Order[i]  =
        VisualPrefs->ListPrefs.Sorts[IDX_FTP].Order[i]    = VisualPrefs->ListPrefs.Sorts[IDX_Chat].Order[i] =
        VisualPrefs->ListPrefs.Sorts[IDX_Telnet].Order[i] = i;

        VisualPrefs->ListPrefs.Sorts[IDX_User].Width[i]   = VisualPrefs->ListPrefs.Sorts[IDX_WWW].Width[i]  =
        VisualPrefs->ListPrefs.Sorts[IDX_FTP].Width[i]    = VisualPrefs->ListPrefs.Sorts[IDX_Chat].Width[i] =
        VisualPrefs->ListPrefs.Sorts[IDX_Telnet].Width[i] = -1;

        VisualPrefs->ListPrefs.UserCols[i]   = VisualPrefs->ListPrefs.WWWCols[i] =
        VisualPrefs->ListPrefs.FTPCols[i]    = VisualPrefs->ListPrefs.ChatCols[i] =
        VisualPrefs->ListPrefs.TelnetCols[i] = 0;
    }

    VisualPrefs->ListPrefs.Sorts[IDX_User].Order[COLS_User] =
        VisualPrefs->ListPrefs.Sorts[IDX_WWW].Order[COLS_WWW] =
        VisualPrefs->ListPrefs.Sorts[IDX_FTP].Order[COLS_FTP] =
        VisualPrefs->ListPrefs.Sorts[IDX_Chat].Order[COLS_Chat] =
        VisualPrefs->ListPrefs.Sorts[IDX_Telnet].Order[COLS_Telnet] = -1;

    VisualPrefs->ListPrefs.UserCols[0] = USERCOL_Name;
    VisualPrefs->ListPrefs.UserCols[1] = USERCOL_EMail;
    VisualPrefs->ListPrefs.UserCols[2] = USERCOL_Comment;
    VisualPrefs->ListPrefs.UserCols[3] = USERCOL_Alias;

    VisualPrefs->ListPrefs.WWWCols[0] = WWWCOL_Name;
    VisualPrefs->ListPrefs.WWWCols[1] = WWWCOL_Site;
    VisualPrefs->ListPrefs.WWWCols[2] = WWWCOL_LastVisited;
    VisualPrefs->ListPrefs.WWWCols[3] = WWWCOL_LastModified;
    VisualPrefs->ListPrefs.WWWCols[4] = WWWCOL_Comment;

    VisualPrefs->ListPrefs.FTPCols[0] = FTPCOL_Name;
    VisualPrefs->ListPrefs.FTPCols[1] = FTPCOL_Site;
    VisualPrefs->ListPrefs.FTPCols[2] = FTPCOL_LastVisited;
    VisualPrefs->ListPrefs.FTPCols[3] = FTPCOL_LastModified;
    VisualPrefs->ListPrefs.FTPCols[4] = FTPCOL_Comment;

    VisualPrefs->ListPrefs.ChatCols[0] = CHATCOL_Channel;
    VisualPrefs->ListPrefs.ChatCols[1] = CHATCOL_Server;
    VisualPrefs->ListPrefs.ChatCols[2] = CHATCOL_Comment;

    VisualPrefs->ListPrefs.TelnetCols[0] = TELNETCOL_Host;
    VisualPrefs->ListPrefs.TelnetCols[1] = TELNETCOL_Comment;

    VisualPrefs->ButtonsBarPrefs.BarPosition = SBPos_Top;
    VisualPrefs->ButtonsBarPrefs.Flags       = SBFLG_Frame|SBFLG_DragBar|SBFLG_BarSpacer;

    VisualPrefs->Flags = VPFLG_NoQuick|VPFLG_NoCard;
}

/***************************************************************************/

void
setDefaultPrefs(struct CMPrefs *prefs)
{
    memset(prefs,0,sizeof(*prefs));

    prefs->Options = (OpenURLBase ? OPT_UseOpenURL : 0) | OPT_EditNew | OPT_BoldSections | OPT_SectionsImages | OPT_GroupsImages;

    strcpy(prefs->WWWScript,"SendAWeb.rexx");
    strcpy(prefs->FTPScript,"SendAmFTP.rexx");
    strcpy(prefs->MailScript,"SendYAM.rexx");
    strcpy(prefs->ChatScript,"SendAmIRC.rexx");
    strcpy(prefs->FaxScript,"SendSTFax.rexx");
    strcpy(prefs->TelnetScript,"SendAmTelnet.rexx");

    strcpy(prefs->SerDevice,"serial.device");
    strcpy(prefs->SerInit,"AT&F");
    strcpy(prefs->DialPrefix,"ATDT");
    strcpy(prefs->DialSuffix,"\\r");
    strcpy(prefs->DialProg,"Rx SendPhone.rexx \"%s\"");
    prefs->RedialDelay    = 30;
    prefs->RedialAttempts = 1;

    prefs->Codeset = CodesetsFindA(NULL,NULL);
    strcpy(prefs->CodesetName,prefs->Codeset->name);

    setDefaultVisualPrefs(&prefs->VisualPrefs);

    strcpy((STRPTR)&prefs->CardBack,"2:rFFFFFFFF,FFFFFFFF,FFFFFFFF");
    strcpy((STRPTR)&prefs->TitlesPen,"rFFFFFFFF,FFFFFFFF,FFFFFFFF");
    strcpy((STRPTR)&prefs->TitlesBackPen,"r0a0a0a0a,24242424,6a6a6a6a");
    strcpy((STRPTR)&prefs->URLPen,"r0a0a0a0a,24242424,6a6a6a6a");
    strcpy((STRPTR)&prefs->URLOverPen,"r80808080,00000000,80808080");
    strcpy((STRPTR)&prefs->MiniatureFrame,"301111");
    prefs->MiniatureSize = 128;
}

/***************************************************************************/

static  ULONG
simpleSave(struct IFFHandle *iffh,ULONG id,APTR ptr,ULONG size)
{
    return (ULONG)(!PushChunk(iffh,ID_PREF,id,size) &&
                   (WriteChunkBytes(iffh,ptr,size)==size) &&
                   !PopChunk(iffh));
}

/***************************************************************************/

static  ULONG
stringSave(struct IFFHandle *iffh,ULONG id,STRPTR string)
{
    register ULONG l;

    return (ULONG)(!PushChunk(iffh,ID_PREF,id,IFFSIZE_UNKNOWN) &&
                   (WriteChunkBytes(iffh,string,l = strlen(string))==l) &&
                   !PopChunk(iffh));
}

/***************************************************************************/

static ULONG
saveLocale(struct IFFHandle *iffh,struct CMPrefs *prefs)
{
    return (ULONG)((*prefs->Country     ? stringSave(iffh,ID_COUNTRY,prefs->Country) : TRUE) &&
                   (*prefs->CountryCode ? stringSave(iffh,ID_COUNTRYCODE,prefs->CountryCode) : TRUE) &&
                   (*prefs->CodesetName ? stringSave(iffh,ID_CODESET,prefs->CodesetName) : TRUE));
}

static ULONG
saveOptions(struct IFFHandle *iffh,struct CMPrefs *prefs)
{
    return (ULONG)(simpleSave(iffh,ID_DELWARNING,&prefs->DelWarning,sizeof(prefs->DelWarning)) &&
                   simpleSave(iffh,ID_ACTION,&prefs->Action,sizeof(prefs->Action)) &&
                   simpleSave(iffh,ID_OPTIONS,&prefs->Options,sizeof(prefs->Options)) &&
                   simpleSave(iffh,ID_GROUPSSTATUS,&prefs->GroupsStatus,sizeof(prefs->GroupsStatus)));
}

static ULONG
saveHots(struct IFFHandle *iffh,struct CMPrefs *prefs)
{
    register struct saveHots *sh;

    for (sh = hotIds; sh->index>=0; sh++)
        if (*prefs->Hotlists[sh->index] && !stringSave(iffh,sh->id,prefs->Hotlists[sh->index]))
            return FALSE;

    return TRUE;
}

static ULONG
saveDialer(struct IFFHandle *iffh,struct CMPrefs *prefs)
{
    return (ULONG)(simpleSave(iffh,ID_DIALOPTIONS,&prefs->DialOptions,sizeof(prefs->DialOptions))  &&
                   simpleSave(iffh,ID_SERUNIT,&prefs->SerUnit,sizeof(prefs->SerUnit))  &&
                   simpleSave(iffh,ID_REDIALDELAY,&prefs->RedialDelay,sizeof(prefs->RedialDelay))  &&
                   simpleSave(iffh,ID_REDIALATTEMPTS,&prefs->RedialAttempts,sizeof(prefs->RedialAttempts))  &&
                   simpleSave(iffh,ID_MODEMTYPE,&prefs->ModemType,sizeof(prefs->ModemType)) &&
                   (*prefs->DialProg    ? stringSave(iffh,ID_DIALPROG,prefs->DialProg) : TRUE) &&
                   (*prefs->SerDevice   ? stringSave(iffh,ID_SERDEVICE,prefs->SerDevice) : TRUE) &&
                   (*prefs->SerInit     ? stringSave(iffh,ID_SERINIT,prefs->SerInit) : TRUE) &&
                   (*prefs->DialPrefix  ? stringSave(iffh,ID_DIALPREFIX,prefs->DialPrefix) : TRUE));
}

static ULONG
saveProgs(struct IFFHandle *iffh,struct CMPrefs *prefs)
{
                   /* Mail */
    return (ULONG)(simpleSave(iffh,ID_MAILPROG,&prefs->MailProg,sizeof(prefs->MailProg)) &&
                   (*prefs->MailScript ? stringSave(iffh,ID_MAILSCRIPT,prefs->MailScript) : TRUE) &&
                   (*prefs->MailPath ? stringSave(iffh,ID_MAILPATH,prefs->MailPath) : TRUE) &&

                   /* WWW */
                   simpleSave(iffh,ID_WWWPROG,&prefs->WWWProg,sizeof(prefs->WWWProg)) &&
                   (*prefs->WWWScript ? stringSave(iffh,ID_WWWSCRIPT,prefs->WWWScript) : TRUE) &&
                   (*prefs->WWWPath ? stringSave(iffh,ID_WWWPATH,prefs->WWWPath) : TRUE) &&

                   /* FTP */
                   simpleSave(iffh,ID_FTPPROG,&prefs->FTPProg,sizeof(prefs->FTPProg)) &&
                   (*prefs->FTPScript ? stringSave(iffh,ID_FTPSCRIPT,prefs->FTPScript) : TRUE) &&
                   (*prefs->FTPPath ? stringSave(iffh,ID_FTPPATH,prefs->FTPPath) : TRUE) &&
                   simpleSave(iffh,ID_FTPOPTIONS,&prefs->FTPOptions,sizeof(prefs->FTPOptions)) &&
                   (*prefs->Local ? stringSave(iffh,ID_FTPLOCAL,prefs->Local) : TRUE) &&

                   /* CHAT */
                   simpleSave(iffh,ID_CHATPROG,&prefs->ChatProg,sizeof(prefs->ChatProg)) &&
                   (*prefs->ChatScript ? stringSave(iffh,ID_CHATSCRIPT,prefs->ChatScript) : TRUE) &&
                   (*prefs->ChatPath ? stringSave(iffh,ID_CHATPATH,prefs->ChatPath) : TRUE) &&

                   /* Telnet */
                   simpleSave(iffh,ID_TELNETPROG,&prefs->TelnetProg,sizeof(prefs->TelnetProg)) &&
                   (*prefs->TelnetScript ? stringSave(iffh,ID_TELNETSCRIPT,prefs->TelnetScript) : TRUE) &&
                   (*prefs->TelnetPath ? stringSave(iffh,ID_TELNETPATH,prefs->TelnetPath) : TRUE) &&

                   /* Fax */
                   simpleSave(iffh,ID_FAXPROG,&prefs->FaxProg,sizeof(prefs->FaxProg)) &&
                   (*prefs->FaxScript ? stringSave(iffh,ID_FAXSCRIPT,prefs->FaxScript) : TRUE) &&
                   (*prefs->FaxPath ? stringSave(iffh,ID_FAXPATH,prefs->FaxPath) : TRUE) &&

                   /* Saved */
                   (*prefs->SavedProg ? stringSave(iffh,ID_SAVEDPROG,prefs->SavedProg) : TRUE));
}

static ULONG
saveVisuals(struct IFFHandle *iffh,struct CMPrefs *prefs)
{
    return simpleSave(iffh,ID_VISUALPREFS,&prefs->VisualPrefs,sizeof(prefs->VisualPrefs));
}

static ULONG
saveCard(struct IFFHandle *iffh,struct CMPrefs *prefs)
{
    return (ULONG)(simpleSave(iffh,ID_CARDBACK,&prefs->CardBack,sizeof(prefs->CardBack)) &&
                   simpleSave(iffh,ID_TITLESPEN,&prefs->TitlesPen,sizeof(prefs->TitlesPen)) &&
                   simpleSave(iffh,ID_TITLESBACKPEN,&prefs->TitlesBackPen,sizeof(prefs->TitlesBackPen)) &&
                   simpleSave(iffh,ID_URLPEN,&prefs->URLPen,sizeof(prefs->URLPen)) &&
                   simpleSave(iffh,ID_URLOVERPEN,&prefs->URLOverPen,sizeof(prefs->URLOverPen)) &&
                   simpleSave(iffh,ID_MINIATUREFRAME,&prefs->MiniatureFrame,sizeof(prefs->MiniatureFrame)) &&
                   simpleSave(iffh,ID_MINIATURESIZE,&prefs->MiniatureSize,sizeof(prefs->MiniatureSize)) &&
                   simpleSave(iffh,ID_CARDOPTIONS,&prefs->CardOptions,sizeof(prefs->CardOptions)));
                   //(*prefs->CardFont ? stringSave(iffh,ID_CARDFONT,prefs->CardFont) : TRUE) &&
                   //(*prefs->TitlesFont ? stringSave(iffh,ID_TITLESFONT,prefs->TitlesFont) : TRUE));
}

ULONG
savePrefs(STRPTR user,struct CMPrefs *prefs)
{
    REGARRAY UBYTE            fileName[256], buf[256];
    register struct IFFHandle *iffh;
    register ULONG            res = FALSE;

    strcpy(fileName,lib_CMPath);
    if (*user) sprintf(buf,"%s/%s.prefs",user,user);
    else strcpy(buf,DEF_PrefsFile);
    AddPart(fileName,buf,sizeof(fileName));

    if (iffh = AllocIFF())
    {
        register BPTR file;

        file = Open(fileName,MODE_NEWFILE);
        if (!file && *user)
        {
            createUserDir(user);
            file = Open(fileName,MODE_NEWFILE);
        }

        if (iffh->iff_Stream = file)
        {
            InitIFFasDOS(iffh);

            if (!OpenIFF(iffh,IFFF_WRITE))
            {
                struct PrefHeader prhd;

                if (PushChunk(iffh,ID_PREF,ID_FORM,IFFSIZE_UNKNOWN)) goto fail;
                if (PushChunk(iffh,ID_PREF,ID_PRHD,sizeof(struct PrefHeader))) goto fail;

                prhd.ph_Version = CMANAGER_PREFS_VERSION;
                prhd.ph_Type    = 0;
                prhd.ph_Flags   = 0;

                if (WriteChunkBytes(iffh,&prhd,sizeof(struct PrefHeader))!=sizeof(struct PrefHeader))
                    goto fail;

                if (PopChunk(iffh)) goto fail;

                if (!saveLocale(iffh,prefs) ||
                    !saveOptions(iffh,prefs) ||
                    !saveHots(iffh,prefs) ||
                    !saveDialer(iffh,prefs) ||
                    !saveProgs(iffh,prefs) ||
                    !saveVisuals(iffh,prefs)||
                    !saveCard(iffh,prefs)) goto fail;

                res = TRUE;

                fail: CloseIFF(iffh);
            }

            Close(file);
        }

        FreeIFF(iffh);
    }

    if (!res) DeleteFile(fileName);

    return res;
}

/**************************************************************************/

ULONG
loadPrefs(STRPTR user,struct CMPrefs *prefs)
{
    REGARRAY UBYTE            fileName[256], buf[256];
    register struct IFFHandle *iffh;
    register BPTR             file;
    register ULONG            res = FALSE;

    strcpy(fileName,lib_CMPath);
    if (*user) sprintf(buf,"%s/%s.prefs",user,user);
    else strcpy(buf,DEF_PrefsFile);
    AddPart(fileName,buf,sizeof(fileName));

    setDefaultPrefs(prefs);

    if (iffh = AllocIFF())
    {
        if (iffh->iff_Stream = Open(fileName,MODE_OLDFILE))
        {
            InitIFFasDOS(iffh);

            if (!OpenIFF(iffh,IFFF_READ))
            {
                struct PrefHeader           prhd;
                register struct ContextNode *cn;
                register struct saveHots    *sh;
                register ULONG              *id, l;

                if (StopChunk(iffh,ID_PREF,ID_PRHD)) goto fail;

                for(id = ids; *id; id++) if (StopChunk(iffh,ID_PREF,*id)) goto fail;

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

                    /* Locale */
                    if ((cn->cn_ID==ID_COUNTRY) && ((l = cn->cn_Size)<sizeof(prefs->Country)))
                    {
                        if (ReadChunkBytes(iffh,prefs->Country,l)!=l) goto fail;
                        prefs->Country[l] = 0;
                        continue;
                    }

                    if ((cn->cn_ID==ID_COUNTRYCODE) && ((l = cn->cn_Size)<sizeof(prefs->CountryCode)))
                    {
                        if (ReadChunkBytes(iffh,prefs->CountryCode,l)!=l) goto fail;
                        prefs->CountryCode[cn->cn_Size+1] = 0;
                        continue;
                    }

                    if ((cn->cn_ID==ID_CODESET) && ((l = cn->cn_Size)<sizeof(prefs->CodesetName)))
                    {
                        if (ReadChunkBytes(iffh,prefs->CodesetName,l)!=l) goto fail;
                        prefs->CodesetName[l] = 0;
                        prefs->Codeset = CodesetsFindA(prefs->CodesetName,NULL);
                        strcpy(prefs->CodesetName,prefs->Codeset->name);
                        continue;
                    }

                    /* Options */
                    if ((cn->cn_ID==ID_DELWARNING) && (cn->cn_Size==sizeof(prefs->DelWarning)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->DelWarning,sizeof(prefs->DelWarning))!=sizeof(prefs->DelWarning)) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_ACTION) && (cn->cn_Size==sizeof(prefs->Action)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->Action,sizeof(prefs->Action))!=sizeof(prefs->Action)) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_OPTIONS) && (cn->cn_Size==sizeof(prefs->Options)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->Options,sizeof(prefs->Options))!=sizeof(prefs->Options)) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_GROUPSSTATUS) && (cn->cn_Size==sizeof(prefs->GroupsStatus)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->GroupsStatus,sizeof(prefs->Options))!=sizeof(prefs->GroupsStatus)) goto fail;
                        continue;
                    }

                    /* Mail */
                    if ((cn->cn_ID==ID_MAILPROG) && (cn->cn_Size==sizeof(prefs->MailProg)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->MailProg,sizeof(prefs->MailProg))!=sizeof(prefs->MailProg)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_MAILSCRIPT) && ((l = cn->cn_Size)<sizeof(prefs->MailScript)))
                    {
                        if (ReadChunkBytes(iffh,prefs->MailScript,l)!=l) goto fail;
                        prefs->MailScript[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_MAILPATH) && ((l = cn->cn_Size)<sizeof(prefs->MailPath)))
                    {
                        if (ReadChunkBytes(iffh,prefs->MailPath,l)!=l) goto fail;
                        prefs->MailPath[l] = 0;
                        continue;
                    }

                    /* WWW */
                    if ((cn->cn_ID==ID_WWWPROG) && (cn->cn_Size==sizeof(prefs->WWWProg)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->WWWProg,sizeof(prefs->WWWProg))!=sizeof(prefs->WWWProg)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_WWWSCRIPT) && ((l = cn->cn_Size)<sizeof(prefs->WWWScript)))
                    {
                        if (ReadChunkBytes(iffh,prefs->WWWScript,l)!=l)                             goto fail;
                        prefs->WWWScript[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_WWWPATH) && ((l = cn->cn_Size)<sizeof(prefs->WWWPath)))
                    {
                        if (ReadChunkBytes(iffh,prefs->WWWPath,l)!=l) goto fail;
                        prefs->WWWPath[l] = 0;
                        continue;
                    }

                    /* FTP */
                    if ((cn->cn_ID==ID_FTPPROG) && (cn->cn_Size==sizeof(prefs->FTPProg)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->FTPProg,sizeof(prefs->FTPProg))!=sizeof(prefs->FTPProg)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_FTPSCRIPT) && ((l = cn->cn_Size)<sizeof(prefs->FTPScript)))
                    {
                        if (ReadChunkBytes(iffh,prefs->FTPScript,l)!=l) goto fail;
                        prefs->FTPScript[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_FTPPATH) && ((l = cn->cn_Size)<sizeof(prefs->FTPPath)))
                    {
                        if (ReadChunkBytes(iffh,prefs->FTPPath,l)!=l) goto fail;
                        prefs->FTPPath[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_FTPOPTIONS) && (cn->cn_Size==sizeof(prefs->FTPOptions)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->FTPOptions,sizeof(prefs->FTPOptions))!=sizeof(prefs->FTPOptions)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_FTPLOCAL) && ((l = cn->cn_Size)<sizeof(prefs->Local)))
                    {
                        if (ReadChunkBytes(iffh,prefs->Local,l)!=l)goto fail;
                        prefs->Local[l] = 0;
                        continue;
                    }

                    /* CHAT */
                    if ((cn->cn_ID==ID_CHATPROG) && (cn->cn_Size==sizeof(prefs->ChatProg)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->ChatProg,sizeof(prefs->ChatProg))!=sizeof(prefs->ChatProg)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_CHATSCRIPT) && ((l = cn->cn_Size)<sizeof(prefs->ChatScript)))
                    {
                        if (ReadChunkBytes(iffh,prefs->ChatScript,l)!=l) goto fail;
                        prefs->ChatScript[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_CHATPATH) && ((l = cn->cn_Size)<sizeof(prefs->ChatPath)))
                    {
                        if (ReadChunkBytes(iffh,prefs->ChatPath,l)!=l) goto fail;
                        prefs->ChatPath[l] = 0;
                        continue;
                    }

                    /* Telnet */
                    if ((cn->cn_ID==ID_TELNETPROG) && (cn->cn_Size==sizeof(prefs->TelnetProg)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->TelnetProg,sizeof(prefs->TelnetProg))!=sizeof(prefs->TelnetProg)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_TELNETSCRIPT) && ((l = cn->cn_Size)<sizeof(prefs->TelnetScript)))
                    {
                        if (ReadChunkBytes(iffh,prefs->TelnetScript,l)!=l) goto fail;
                        prefs->TelnetScript[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_TELNETPATH) && ((l = cn->cn_Size)<sizeof(prefs->TelnetPath)))
                    {
                        if (ReadChunkBytes(iffh,prefs->TelnetPath,l)!=l) goto fail;
                        prefs->TelnetPath[l] = 0;
                        continue;
                    }

                    /* Fax */
                    if ((cn->cn_ID==ID_FAXPROG) && (cn->cn_Size==sizeof(prefs->FaxProg)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->FaxProg,sizeof(prefs->FaxProg))!=sizeof(prefs->FaxProg)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_FAXSCRIPT) && ((l = cn->cn_Size)<sizeof(prefs->FaxScript)))
                    {
                        if (ReadChunkBytes(iffh,prefs->FaxScript,l)!=l) goto fail;
                        prefs->FaxScript[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_FAXPATH) && ((l = cn->cn_Size)<sizeof(prefs->FaxPath)))
                    {
                        if (ReadChunkBytes(iffh,prefs->FaxPath,l)!=l) goto fail;
                        prefs->FaxPath[l] = 0;
                        continue;
                    }

                    /* Saved */
                    if ((cn->cn_ID==ID_SAVEDPROG) && ((l = cn->cn_Size)<sizeof(prefs->SavedProg)))
                    {
                        if (ReadChunkBytes(iffh,prefs->SavedProg,l)!=l) goto fail;
                        prefs->SavedProg[l] = 0;
                        continue;
                    }

                    /* Dialer */
                    if ((cn->cn_ID==ID_DIALOPTIONS) && (cn->cn_Size==sizeof(prefs->DialOptions)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->DialOptions,sizeof(prefs->DialOptions))!=sizeof(prefs->DialOptions)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_DIALPROG) && ((l = cn->cn_Size)<sizeof(prefs->DialProg)))
                    {
                        if (ReadChunkBytes(iffh,prefs->DialProg,l)!=l) goto fail;
                        prefs->DialProg[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_SERDEVICE) && ((l = cn->cn_Size)<sizeof(prefs->SerDevice)))
                    {
                        if (ReadChunkBytes(iffh,prefs->SerDevice,l)!=l) goto fail;
                        prefs->SerDevice[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_SERUNIT) && (cn->cn_Size==sizeof(prefs->SerUnit)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->SerUnit,sizeof(prefs->SerUnit))!=sizeof(prefs->SerUnit)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_SERINIT) && ((l = cn->cn_Size)<sizeof(prefs->SerInit)))
                    {
                        if (ReadChunkBytes(iffh,prefs->SerInit,l)!=l) goto fail;
                        prefs->SerInit[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_REDIALDELAY) && (cn->cn_Size==sizeof(prefs->RedialDelay)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->RedialDelay,sizeof(prefs->RedialDelay))!=sizeof(prefs->RedialDelay)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_REDIALATTEMPTS) && (cn->cn_Size==sizeof(prefs->RedialAttempts)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->RedialAttempts,sizeof(prefs->RedialAttempts))!=sizeof(prefs->RedialAttempts)) goto fail;
                        continue;
                    }
                    if ((cn->cn_ID==ID_DIALCOMMAND) && ((l = cn->cn_Size)<sizeof(prefs->DialPrefix)))
                    {
                        if (ReadChunkBytes(iffh,prefs->DialPrefix,l)!=l) goto fail;
                        prefs->DialPrefix[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_DIALPREFIX) && ((l = cn->cn_Size)<sizeof(prefs->DialPrefix)))
                    {
                        if (ReadChunkBytes(iffh,prefs->DialPrefix,l)!=l) goto fail;
                        prefs->DialPrefix[l] = 0;
                        continue;
                    }
                    if ((cn->cn_ID==ID_MODEMTYPE) && (cn->cn_Size==sizeof(prefs->ModemType)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->ModemType,sizeof(prefs->ModemType))!=sizeof(prefs->ModemType)) goto fail;
                        continue;
                    }

                    for (sh = hotIds; sh->index>=0; sh++)
                    {
                        if ((cn->cn_ID==sh->id) && ((l = cn->cn_Size)<sizeof(prefs->Hotlists[sh->index])))
                        {
                            if (ReadChunkBytes(iffh,prefs->Hotlists[sh->index],l)!=l) goto fail;
                            prefs->Hotlists[sh->index][l] = 0;
                            break;
                        }
                    }
                    if (sh->index>=0) continue;

                    /* VisualPrefs */
                    if ((prhd.ph_Version==CMANAGER_PREFS_VERSION) && (cn->cn_ID==ID_VISUALPREFS) && (cn->cn_Size==sizeof(prefs->VisualPrefs)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->VisualPrefs,sizeof(prefs->VisualPrefs))!=sizeof(prefs->VisualPrefs)) goto fail;
                        continue;
                    }

                    /* Card */
                    if ((cn->cn_ID==ID_CARDBACK) && ((l = cn->cn_Size)<=sizeof(prefs->CardBack)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->CardBack,l)!=l) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_TITLESPEN) && ((l = cn->cn_Size)<=sizeof(prefs->TitlesPen)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->TitlesPen,l)!=l) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_TITLESBACKPEN) && ((l = cn->cn_Size)<=sizeof(prefs->TitlesBackPen)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->TitlesBackPen,l)!=l) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_URLPEN) && ((l = cn->cn_Size)<=sizeof(prefs->URLPen)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->URLPen,l)!=l) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_URLOVERPEN) && ((l = cn->cn_Size)<=sizeof(prefs->URLOverPen)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->URLOverPen,l)!=l) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_MINIATUREFRAME) && ((l = cn->cn_Size)<=sizeof(prefs->MiniatureFrame)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->MiniatureFrame,l)!=l) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_MINIATURESIZE) && (cn->cn_Size==sizeof(prefs->MiniatureSize)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->MiniatureSize,sizeof(prefs->MiniatureSize))!=sizeof(prefs->MiniatureSize)) goto fail;
                        continue;
                    }

                    if ((cn->cn_ID==ID_CARDOPTIONS) && (cn->cn_Size==sizeof(prefs->CardOptions)))
                    {
                        if (ReadChunkBytes(iffh,&prefs->CardOptions,sizeof(prefs->CardOptions))!=sizeof(prefs->CardOptions)) goto fail;
                        continue;
                    }

                    /*if ((cn->cn_ID==ID_CARDFONT) && ((l = cn->cn_Size)<sizeof(prefs->CardFont)))
                    {
                        if (ReadChunkBytes(iffh,prefs->CardFont,l)!=l) goto fail;
                        prefs->CardFont[l] = 0;
                        continue;
                    }

                    if ((cn->cn_ID==ID_TITLESFONT) && ((l = cn->cn_Size)<sizeof(prefs->TitlesFont)))
                    {
                        if (ReadChunkBytes(iffh,prefs->TitlesFont,l)!=l) goto fail;
                        prefs->TitlesFont[l] = 0;
                        continue;
                    }*/
                }

                res = TRUE;

                fail: CloseIFF(iffh);
            }

            Close(iffh->iff_Stream);
        }

        FreeIFF(iffh);
    }

    strcpy(fileName,lib_CMPath);
    if (*user)
    {
        AddPart(fileName,user,sizeof(fileName));
        AddPart(fileName,DEF_HotlistStampsFile,sizeof(fileName));
    }
    else AddPart(fileName,DEF_HotlistStampsFile,sizeof(fileName));

    if (file = Open(fileName,MODE_OLDFILE))
    {
        Read(file,&prefs->Stamps,sizeof(prefs->Stamps));
        Close(file);
    }

    return res;
}

/**************************************************************************/
