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
#include <dos/dostags.h>
#include <datatypes/textclass.h>
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/***********************************************************************/

enum
{
    ACT_EDIT = 1,
    ACT_COPY,
    ACT_CONFIGURE,

    ACT_EMAILHOME,
    ACT_EMAILBUSINESS,
    ACT_QUICKEMAIL,

    ACT_BROWSER,
    ACT_FTP,
    ACT_JOIN,
    ACT_TELNET,

    ACT_CALLHOME,
    ACT_CALLMOBILE,
    ACT_CALLBUSINESS,
    ACT_CALLBUSINESSMOBILE,

    ACT_FAXHOME,
    ACT_FAXBUSINESS,
};

/***********************************************************************/

struct Format
{
    STRPTR format;
    LONG   c0;
    LONG   c1;
    LONG   quick;
    BYTE   *o;
    int    cols;
    int    *width;
};

struct data
{
    /* Objects */
    Object                      *CMObj;

    /* ARexx stuff */
    struct MUI_InputHandlerNode RexxHandler;
    int                         RexxPortSig;
    struct MsgPort              RexxPort;
    ULONG                       RexxOut;

    /* Hook stuff */
#ifdef __MORPHOS__
    struct EmulLibEntry         DispTrap;
#endif
    struct Hook                 DispHook;
    UBYTE                       Buffers[LP_NUMCOLS][LEN_DATSTRING*2];

    /* Various */
    struct CMPrefs              *Prefs;
    UWORD                       Type;
    struct CMGroup              *Group;
    ULONG                       Flags;

    APTR                        TitleContextMenu;
    APTR                        ContextMenu;

    WORD                        DispColumn;
    WORD                        Column;

    struct MUI_EventHandlerNode eh;

    struct ListPrefs            ListPrefs;
};

enum
{
    FLG_AtHome     = 1<<0,
    FLG_NoMenu     = 1<<1,
    FLG_BoldTitles = 1<<2,
    FLG_NoListBar  = 1<<3,
    FLG_ManualSort = 1<<4,
    FLG_IgnoreLast = 1<<5,
};

/***********************************************************************/

static ULONG UserMenuIDs[] =
{
    Msg_Edit_MenuUser_Title,
    	Msg_Edit_MenuBrowser,
    	Msg_Edit_MenuFTP,
    	Msg_Edit_MenuEMail,
    	Msg_Edit_MenuEMailHome,
    	    Msg_Edit_MenuEMailBusiness,
    	    Msg_Edit_MenuQuickEMail,
    	0,
    	Msg_Edit_MenuCall,
    	    Msg_Edit_MenuCallHome,
    	    Msg_Edit_MenuCallMobile,
    	    Msg_Edit_MenuCallBusiness,
    	    Msg_Edit_MenuCallBusinessMobile,
    	Msg_Edit_MenuFax,
    	    Msg_Edit_MenuFaxHome,
    	    Msg_Edit_MenuFaxBusiness,
    	0,
        Msg_Edit_MenuEdit,
    	Msg_Edit_MenuCopy,
    	0,
    	Msg_Menu_Configure
};

static struct NewMenu UserMenu[] =
{
    MTITLE(Msg_Edit_MenuUser_Title),
        MITEM(ACT_BROWSER),
        MITEM(ACT_FTP),
        MITEM(Msg_Edit_MenuEMail),
            MSUBITEM(ACT_EMAILHOME),
            MSUBITEM(ACT_EMAILBUSINESS),
        MITEM(ACT_QUICKEMAIL),
        MBAR,
        MITEM(Msg_Edit_MenuCall),
            MSUBITEM(ACT_CALLHOME),
            MSUBITEM(ACT_CALLMOBILE),
            MSUBITEM(ACT_CALLBUSINESS),
            MSUBITEM(ACT_CALLBUSINESSMOBILE),
        MITEM(Msg_Edit_MenuFax),
            MSUBITEM(ACT_FAXHOME),
            MSUBITEM(ACT_FAXBUSINESS),
        MBAR,
        MITEM(ACT_EDIT),
        MITEM(ACT_COPY),
        MBAR,
        MITEM(ACT_CONFIGURE),
    MEND
};

#define USEREX(i) ((1<<16)-1-(1<<(i)))

static ULONG UserTitleMenuIDs[] =
{
    Msg_Edit_MenuUserList_Title,
    	Msg_List_Menu_DefWidthThis,
    	Msg_List_Menu_DefWidthAll,
    	Msg_List_Menu_DefOrderThis,
    	Msg_List_Menu_DefOrderAll,
    	Msg_List_Menu_DefSort,
    	0,
    	Msg_Menu_Columns,
    	    Msg_Title_Name,
    	    Msg_Title_EMail,
    	    Msg_Title_Comment,
    	    Msg_Title_Alias,
    	    Msg_Title_Birthday,
    	    Msg_Title_Anniversary,
    	    Msg_Title_Country,
    	    Msg_Title_City,
    	    Msg_Title_Phone,
    	    Msg_Title_Mobile,
    	    Msg_Title_Fax,
    	    Msg_Title_WWW,
    	    Msg_Title_Company,
    	    Msg_Title_BPhone,
    	    Msg_Title_BMobile,
    	    Msg_Title_BFax,
    	Msg_Menu_Configure
};

static struct NewMenu UserTitleMenu[] =
{
    MTITLE(Msg_Edit_MenuUserList_Title),
        MITEM(MUIV_NList_Menu_DefWidth_This),
        MITEM(MUIV_NList_Menu_DefWidth_All),
        MITEM(MUIV_NList_Menu_DefOrder_This),
        MITEM(MUIV_NList_Menu_DefOrder_All),
        MITEM(Msg_List_Menu_DefSort),
        MBAR,
        MITEM(Msg_Menu_Columns),
            MESUBITEM(Msg_Title_Name,USEREX(0)),
            MESUBITEM(Msg_Title_EMail,USEREX(1)),
            MESUBITEM(Msg_Title_Comment,USEREX(2)),
            MESUBITEM(Msg_Title_Alias,USEREX(3)),
            MESUBITEM(Msg_Title_Birthday,USEREX(4)),
            MESUBITEM(Msg_Title_Anniversary,USEREX(5)),
            MESUBITEM(Msg_Title_Country,USEREX(6)),
            MESUBITEM(Msg_Title_City,USEREX(7)),
            MESUBITEM(Msg_Title_Phone,USEREX(8)),
            MESUBITEM(Msg_Title_Mobile,USEREX(9)),
            MESUBITEM(Msg_Title_Fax,USEREX(10)),
            MESUBITEM(Msg_Title_WWW,USEREX(11)),
            MESUBITEM(Msg_Title_Company,USEREX(12)),
            MESUBITEM(Msg_Title_BPhone,USEREX(13)),
            MESUBITEM(Msg_Title_BMobile,USEREX(14)),
            MESUBITEM(Msg_Title_BFax,USEREX(15)),
        MITEM(ACT_CONFIGURE),
    MEND
};

static ULONG WWWMenuIDs[] =
{
    Msg_Edit_MenuWWW_Title,
    	Msg_Edit_MenuBrowser,
    	Msg_Edit_MenuEMail,
    	0,
        Msg_Edit_MenuEdit,
    	Msg_Edit_MenuCopy,
    	0,
    	Msg_Menu_Configure
};

static struct NewMenu WWWMenu[] =
{
    MTITLE(Msg_Edit_MenuWWW_Title),
        MITEM(ACT_BROWSER),
        MITEM(ACT_EMAILHOME),
        MBAR,
        MITEM(ACT_EDIT),
        MITEM(ACT_COPY),
        MBAR,
        MITEM(ACT_CONFIGURE),
    MEND
};

#define WWWEX(i) ((1<<7)-1-(1<<(i)))

static ULONG WWWTitleMenuIDs[] =
{
    Msg_Edit_MenuWWWList_Title,
    	Msg_List_Menu_DefWidthThis,
    	Msg_List_Menu_DefWidthAll,
    	Msg_List_Menu_DefOrderThis,
    	Msg_List_Menu_DefOrderAll,
    	Msg_List_Menu_DefSort,
    	0,
    	Msg_Menu_Columns,
    	    Msg_Title_Name,
    	    Msg_Title_Site,
    	    Msg_Title_LastVisited,
    	    Msg_Title_LastModified,
    	    Msg_Title_Comment,
    	    Msg_Title_Alias,
    	    Msg_Title_EMail,
    	Msg_Menu_Configure
};

static struct NewMenu WWWTitleMenu[] =
{
    MTITLE(Msg_Edit_MenuWWWList_Title),
        MITEM(MUIV_NList_Menu_DefWidth_This),
        MITEM(MUIV_NList_Menu_DefWidth_All),
        MITEM(MUIV_NList_Menu_DefOrder_This),
        MITEM(MUIV_NList_Menu_DefOrder_All),
        MITEM(Msg_List_Menu_DefSort),
        MBAR,
        MITEM(Msg_Menu_Columns),
            MESUBITEM(Msg_Title_Name,WWWEX(0)),
            MESUBITEM(Msg_Title_Site,WWWEX(1)),
            MESUBITEM(Msg_Title_LastVisited,WWWEX(2)),
            MESUBITEM(Msg_Title_LastModified,WWWEX(3)),
            MESUBITEM(Msg_Title_Comment,WWWEX(4)),
            MESUBITEM(Msg_Title_Alias,WWWEX(5)),
            MESUBITEM(Msg_Title_EMail,WWWEX(6)),
        MITEM(ACT_CONFIGURE),
    MEND
};

static ULONG FTPMenuIDs[] =
{
    Msg_Edit_MenuFTP_Title,
    	Msg_Edit_MenuFTP,
    	Msg_Edit_MenuBrowser,
    	Msg_Edit_MenuEMail,
    	0,
        Msg_Edit_MenuEdit,
    	Msg_Edit_MenuCopy,
    	0,
    	Msg_Menu_Configure
};

static struct NewMenu FTPMenu[] =
{
    MTITLE(Msg_Edit_MenuFTP_Title),
        MITEM(ACT_FTP),
        MITEM(ACT_BROWSER),
        MITEM(ACT_EMAILHOME),
        MBAR,
        MITEM(ACT_EDIT),
        MITEM(ACT_COPY),
        MBAR,
        MITEM(ACT_CONFIGURE),
    MEND
};

#define FTPEX(i) ((1<<7)-1-(1<<(i)))

static ULONG FTPTitleMenuIDs[] =
{
    Msg_Edit_MenuFTPList_Title,
    	Msg_List_Menu_DefWidthThis,
    	Msg_List_Menu_DefWidthAll,
    	Msg_List_Menu_DefOrderThis,
    	Msg_List_Menu_DefOrderAll,
    	Msg_List_Menu_DefSort,
    	0,
    	Msg_Menu_Columns,
    	    Msg_Title_Name,
    	    Msg_Title_Site,
    	    Msg_Title_LastVisited,
    	    Msg_Title_LastModified,
    	    Msg_Title_Comment,
    	    Msg_Title_Alias,
    	    Msg_Title_EMail,
    	Msg_Menu_Configure
};

static struct NewMenu FTPTitleMenu[] =
{
    MTITLE(Msg_Edit_MenuFTPList_Title),
        MITEM(MUIV_NList_Menu_DefWidth_This),
        MITEM(MUIV_NList_Menu_DefWidth_All),
        MITEM(MUIV_NList_Menu_DefOrder_This),
        MITEM(MUIV_NList_Menu_DefOrder_All),
        MITEM(Msg_List_Menu_DefSort),
        MBAR,
        MITEM(Msg_Menu_Columns),
            MESUBITEM(Msg_Title_Name,FTPEX(0)),
            MESUBITEM(Msg_Title_Site,FTPEX(1)),
            MESUBITEM(Msg_Title_LastVisited,FTPEX(2)),
            MESUBITEM(Msg_Title_LastModified,FTPEX(3)),
            MESUBITEM(Msg_Title_Comment,FTPEX(4)),
            MESUBITEM(Msg_Title_Alias,FTPEX(5)),
            MESUBITEM(Msg_Title_EMail,FTPEX(6)),
        MITEM(ACT_CONFIGURE),
    MEND
};

static ULONG ChatMenuIDs[] =
{
    Msg_Edit_MenuChat_Title,
    	Msg_Edit_MenuJoin,
    	Msg_Edit_MenuBrowser,
    	Msg_Edit_MenuEMail,
    	0,
        Msg_Edit_MenuEdit,
    	Msg_Edit_MenuCopy,
    	0,
    	Msg_Menu_Configure
};

static struct NewMenu ChatMenu[] =
{
    MTITLE(Msg_Edit_MenuChat_Title),
        MITEM(ACT_JOIN),
        MITEM(ACT_BROWSER),
        MITEM(ACT_EMAILHOME),
        MBAR,
        MITEM(ACT_EDIT),
        MITEM(ACT_COPY),
        MBAR,
        MITEM(ACT_CONFIGURE),
    MEND
};

#define CHATEX(i) ((1<<5)-1-(1<<(i)))

static ULONG ChatTitleMenuIDs[] =
{
    Msg_Edit_MenuChatList_Title,
    	Msg_List_Menu_DefWidthThis,
    	Msg_List_Menu_DefWidthAll,
    	Msg_List_Menu_DefOrderThis,
    	Msg_List_Menu_DefOrderAll,
    	Msg_List_Menu_DefSort,
    	0,
    	Msg_Menu_Columns,
    	    Msg_Title_Channel,
    	    Msg_Title_Server,
    	    Msg_Title_Comment,
    	    Msg_Title_EMail,
    	    Msg_Title_WWW,
    	Msg_Menu_Configure
};

static struct NewMenu ChatTitleMenu[] =
{
    MTITLE(Msg_Edit_MenuChatList_Title),
        MITEM(MUIV_NList_Menu_DefWidth_This),
        MITEM(MUIV_NList_Menu_DefWidth_All),
        MITEM(MUIV_NList_Menu_DefOrder_This),
        MITEM(MUIV_NList_Menu_DefOrder_All),
        MITEM(Msg_List_Menu_DefSort),
        MBAR,
        MITEM(Msg_Menu_Columns),
            MESUBITEM(Msg_Title_Channel,CHATEX(0)),
            MESUBITEM(Msg_Title_Server,CHATEX(1)),
            MESUBITEM(Msg_Title_Comment,CHATEX(2)),
            MESUBITEM(Msg_Title_EMail,CHATEX(3)),
            MESUBITEM(Msg_Title_WWW,CHATEX(4)),
        MITEM(ACT_CONFIGURE),
    MEND
};

static ULONG TelnetMenuIDs[] =
{
    Msg_Edit_MenuTelnet_Title,
    	Msg_Edit_MenuTelnet,
    	Msg_Edit_MenuEMail,
    	0,
        Msg_Edit_MenuEdit,
    	Msg_Edit_MenuCopy,
    	0,
    	Msg_Menu_Configure
};

static struct NewMenu TelnetMenu[] =
{
    MTITLE(Msg_Edit_MenuTelnet_Title),
        MITEM(ACT_TELNET),
        MITEM(ACT_EMAILHOME),
        MBAR,
        MITEM(ACT_EDIT),
        MITEM(ACT_COPY),
        MBAR,
        MITEM(ACT_CONFIGURE),
    MEND
};

#define TELNETEX(i) ((1<<4)-1-(1<<(i)))

static ULONG TelnetTitleMenuIDs[] =
{
    Msg_Edit_MenuTelnetList_Title,
    	Msg_List_Menu_DefWidthThis,
    	Msg_List_Menu_DefWidthAll,
    	Msg_List_Menu_DefOrderThis,
    	Msg_List_Menu_DefOrderAll,
    	Msg_List_Menu_DefSort,
    	0,
    	Msg_Menu_Columns,
    	    Msg_Title_Host,
    	    Msg_Title_Comment,
    	    Msg_Title_Port,
    	    Msg_Title_Alias,
    	    Msg_Title_EMail,
    	Msg_Menu_Configure
};

static struct NewMenu TelnetTitleMenu[] =
{
    MTITLE(Msg_Edit_MenuTelnetList_Title),
        MITEM(MUIV_NList_Menu_DefWidth_This),
        MITEM(MUIV_NList_Menu_DefWidth_All),
        MITEM(MUIV_NList_Menu_DefOrder_This),
        MITEM(MUIV_NList_Menu_DefOrder_All),
        MITEM(Msg_List_Menu_DefSort),
        MBAR,
        MITEM(Msg_Menu_Columns),
            MESUBITEM(Msg_Title_Host,TELNETEX(0)),
            MESUBITEM(Msg_Title_Comment,TELNETEX(1)),
            MESUBITEM(Msg_Title_Port,TELNETEX(2)),
            MESUBITEM(Msg_Title_Alias,TELNETEX(3)),
            MESUBITEM(Msg_Title_EMail,TELNETEX(4)),
        MITEM(ACT_CONFIGURE),
    MEND
};

/***********************************************************************/

static ULONG
colToID(ULONG type,ULONG col)
{
    switch (type)
    {
        case CMEntry_User:
            switch (col)
            {
                case USERCOL_Name:        return Msg_Title_Name;
                case USERCOL_EMail:       return Msg_Title_EMail;
                case USERCOL_Comment:     return Msg_Title_Comment;
                case USERCOL_Alias:       return Msg_Title_Alias;
                case USERCOL_Birthday:    return Msg_Title_Birthday;
                case USERCOL_Anniversary: return Msg_Title_Anniversary;
                case USERCOL_Country:     return Msg_Title_Country;
                case USERCOL_City:        return Msg_Title_City;
                case USERCOL_Phone:       return Msg_Title_Phone;
                case USERCOL_Mobile:      return Msg_Title_Mobile;
                case USERCOL_Fax:         return Msg_Title_Fax;
                case USERCOL_WWW:         return Msg_Title_WWW;
                case USERCOL_Company:     return Msg_Title_Company;
                case USERCOL_BPhone:      return Msg_Title_BPhone;
                case USERCOL_BMobile:     return Msg_Title_BMobile;
                case USERCOL_BFax:        return Msg_Title_BFax;
            }
            break;

        case CMEntry_WWW:
            switch (col)
            {
                case WWWCOL_Name:         return Msg_Title_Name;
                case WWWCOL_Site:         return Msg_Title_Site;
                case WWWCOL_LastVisited:  return Msg_Title_LastVisited;
                case WWWCOL_LastModified: return Msg_Title_LastModified;
                case WWWCOL_Comment:      return Msg_Title_Comment;
                case WWWCOL_Alias:        return Msg_Title_Alias;
                case WWWCOL_EMail:        return Msg_Title_EMail;
            }
            break;

        case CMEntry_FTP:
            switch (col)
            {
                case FTPCOL_Name:         return Msg_Title_Name;
                case FTPCOL_Site:         return Msg_Title_Site;
                case FTPCOL_LastVisited:  return Msg_Title_LastVisited;
                case FTPCOL_LastModified: return Msg_Title_LastModified;
                case FTPCOL_Comment:      return Msg_Title_Comment;
                case FTPCOL_Alias:        return Msg_Title_Alias;
                case FTPCOL_EMail:        return Msg_Title_EMail;
            }
            break;

        case CMEntry_Chat:
            switch (col)
            {
                case CHATCOL_Channel:     return Msg_Title_Channel;
                case CHATCOL_Server:      return Msg_Title_Server;
                case CHATCOL_Comment:     return Msg_Title_Comment;
                case CHATCOL_EMail:       return Msg_Title_EMail;
                case CHATCOL_WWW:         return Msg_Title_WWW;
            }
            break;

        case CMEntry_Telnet:
            switch (col)
            {
                case TELNETCOL_Host:      return Msg_Title_Host;
                case TELNETCOL_Comment:   return Msg_Title_Comment;
                case TELNETCOL_Port:      return Msg_Title_Port;
                case TELNETCOL_EMail:     return Msg_Title_EMail;
                case TELNETCOL_Alias:     return Msg_Title_Alias;
            }
            break;
    }

    return 0;
}

static ULONG
IDToCol(ULONG type,ULONG ID)
{
    switch (type)
    {
        case CMEntry_User:
            switch (ID)
            {
                case Msg_Title_Name:        return USERCOL_Name;
                case Msg_Title_EMail:       return USERCOL_EMail;
                case Msg_Title_Comment:     return USERCOL_Comment;
                case Msg_Title_Alias:       return USERCOL_Alias;
                case Msg_Title_Birthday:    return USERCOL_Birthday;
                case Msg_Title_Anniversary: return USERCOL_Anniversary;
                case Msg_Title_Country:     return USERCOL_Country;
                case Msg_Title_City:        return USERCOL_City;
                case Msg_Title_Phone:       return USERCOL_Phone;
                case Msg_Title_Mobile:      return USERCOL_Mobile;
                case Msg_Title_Fax:         return USERCOL_Fax;
                case Msg_Title_WWW:         return USERCOL_WWW;
                case Msg_Title_Company:     return USERCOL_Company;
                case Msg_Title_BPhone:      return USERCOL_BPhone;
                case Msg_Title_BMobile:     return USERCOL_BMobile;
                case Msg_Title_BFax:        return USERCOL_BFax;
            }
            break;

        case CMEntry_WWW:
            switch (ID)
            {
                case Msg_Title_Name:         return WWWCOL_Name;
                case Msg_Title_Site:         return WWWCOL_Site;
                case Msg_Title_LastVisited:  return WWWCOL_LastVisited;
                case Msg_Title_LastModified: return WWWCOL_LastModified;
                case Msg_Title_Comment:      return WWWCOL_Comment;
                case Msg_Title_Alias:        return WWWCOL_Alias;
                case Msg_Title_EMail:        return WWWCOL_EMail;
            }
            break;

        case CMEntry_FTP:
            switch (ID)
            {
                case Msg_Title_Name:         return FTPCOL_Name;
                case Msg_Title_Site:         return FTPCOL_Site;
                case Msg_Title_LastVisited:  return FTPCOL_LastVisited;
                case Msg_Title_LastModified: return FTPCOL_LastModified;
                case Msg_Title_Comment:      return FTPCOL_Comment;
                case Msg_Title_Alias:        return FTPCOL_Alias;
                case Msg_Title_EMail:        return FTPCOL_EMail;
            }
            break;

        case CMEntry_Chat:
            switch (ID)
            {
                case Msg_Title_Channel:      return CHATCOL_Channel;
                case Msg_Title_Server:       return CHATCOL_Server;
                case Msg_Title_Comment:      return CHATCOL_Comment;
                case Msg_Title_EMail:        return CHATCOL_EMail;
                case Msg_Title_WWW:          return CHATCOL_WWW;
            }
            break;

        case CMEntry_Telnet:
            switch (ID)
            {
                case Msg_Title_Host:         return TELNETCOL_Host;
                case Msg_Title_Comment:      return TELNETCOL_Comment;
                case Msg_Title_Port:         return TELNETCOL_Port;
                case Msg_Title_EMail:        return TELNETCOL_EMail;
                case Msg_Title_Alias:        return TELNETCOL_Alias;
            }
            break;
    }

    return 0;
}

/***********************************************************************/

#define ISZERO(ds) (!(ds).ds_Days && !(ds).ds_Minute && !(ds).ds_Tick)

static STRPTR
getDisplayCol(struct data *data,struct CMEntry *entry,int col,STRPTR *p)
{
    switch (entry->Type)
    {
        case CMEntry_User:
            switch (data->ListPrefs.UserCols[col])
            {
                case USERCOL_Name:
                    if (!*CMUSER(entry)->VisualName)
                    {
                        if (*CMUSER(entry)->Name)
                        {
                            if (*CMUSER(entry)->LastName) snprintf(CMUSER(entry)->VisualName,sizeof(CMUSER(entry)->VisualName),"%s %s",CMUSER(entry)->Name,CMUSER(entry)->LastName);
                            else stccpy(CMUSER(entry)->VisualName,CMUSER(entry)->Name,sizeof(CMUSER(entry)->VisualName));
                        }
                        else
                        {
                            if (*CMUSER(entry)->LastName) stccpy(CMUSER(entry)->VisualName,CMUSER(entry)->LastName,sizeof(CMUSER(entry)->VisualName));
                            else
                            {
                                if (*CMUSER(entry)->Alias) stccpy(CMUSER(entry)->VisualName,CMUSER(entry)->Alias,sizeof(CMUSER(entry)->VisualName));
                            }
                        }
                    }
                    return CMUSER(entry)->VisualName;

                case USERCOL_EMail:    return CMUSER(entry)->EMail;
                case USERCOL_Comment:  return CMUSER(entry)->Comment;
                case USERCOL_Alias:    return CMUSER(entry)->Alias;
                case USERCOL_Country:  return CMUSER(entry)->Country;
                case USERCOL_City:     return CMUSER(entry)->City;
                case USERCOL_Phone:    return CMUSER(entry)->Phone;
                case USERCOL_Mobile:   return CMUSER(entry)->Mobile;
                case USERCOL_Fax:      return CMUSER(entry)->Fax;
                case USERCOL_WWW:      return CMUSER(entry)->WWW;
                case USERCOL_Company:  return CMUSER(entry)->Company;
                case USERCOL_BPhone:   return CMUSER(entry)->BPhone;
                case USERCOL_BMobile:  return CMUSER(entry)->BMobile;
                case USERCOL_BFax:     return CMUSER(entry)->BFax;

                case USERCOL_Birthday: case USERCOL_Anniversary:
                {
                    register ULONG date, use;

                    if (data->ListPrefs.UserCols[col]==USERCOL_Birthday)
                    {
                        date = CMUSER(entry)->Birthday;
                        use  = CMUSER(entry)->Flags & CMUSERFLG_Birthday;
                    }
                    else
                    {
                        date = CMUSER(entry)->Anniversary;
                        use  = CMUSER(entry)->Flags & CMUSERFLG_Anniversary;
                    }

                    if (use)
                    {
                        formatDate(data->Buffers[col],sizeof(data->Buffers[col]),lib_locale->loc_ShortDateFormat,date);
                        if (checkBirthday(date)) *(p+col) = MUIX_B;
                        return data->Buffers[col];
                    }
                    else return "";
                }
            }

        case CMEntry_WWW:
            switch (data->ListPrefs.WWWCols[col])
            {
                case WWWCOL_Name: return CMWWW(entry)->Name;
                case WWWCOL_Site: return CMWWW(entry)->WWW;

                case WWWCOL_LastVisited:
                {
                    REGARRAY UBYTE  date[LEN_DATSTRING], time[LEN_DATSTRING];
                    struct DateTime dt;

                    if (ISZERO(CMWWW(entry)->LastVisited)) strcpy(data->Buffers[col],CM_GetString(Msg_Never));
                    else
                    {
                        dt.dat_Stamp   = CMWWW(entry)->LastVisited;
                        dt.dat_Format  = FORMAT_DEF;
                        dt.dat_Flags   = DTF_SUBST;
                        dt.dat_StrDay  = NULL;
                        dt.dat_StrDate = date;
                        dt.dat_StrTime = time;

                        DateToStr(&dt);

                        sprintf(data->Buffers[col],"%s %s",date,time);
                    }

                    return data->Buffers[col];
                }

                case WWWCOL_LastModified:
                {
                    REGARRAY UBYTE  date[LEN_DATSTRING], time[LEN_DATSTRING];
                    struct DateTime dt;

                    if (ISZERO(CMWWW(entry)->LastModified)) strcpy(data->Buffers[col],CM_GetString(Msg_Never));
                    else
                    {
                        dt.dat_Stamp   = CMWWW(entry)->LastModified;
                        dt.dat_Format  = FORMAT_DEF;
                        dt.dat_Flags   = DTF_SUBST;
                        dt.dat_StrDay  = NULL;
                        dt.dat_StrDate = date;
                        dt.dat_StrTime = time;

                        DateToStr(&dt);

                        sprintf(data->Buffers[col],"%s %s",date,time);
                    }

                    return data->Buffers[col];
                }

                case WWWCOL_Comment: return CMWWW(entry)->Comment;
                case WWWCOL_Alias:   return CMWWW(entry)->Alias;
                case WWWCOL_EMail:   return CMWWW(entry)->EMail;
            }

        case CMEntry_FTP:
            switch (data->ListPrefs.FTPCols[col])
            {
                case FTPCOL_Name: return CMFTP(entry)->Name;
                case FTPCOL_Site: return CMFTP(entry)->FTP;

                case FTPCOL_LastVisited:
                {
                    REGARRAY UBYTE  date[LEN_DATSTRING], time[LEN_DATSTRING];
                    struct DateTime dt;

                    if (ISZERO(CMFTP(entry)->LastVisited)) strcpy(data->Buffers[col],CM_GetString(Msg_Never));
                    else
                    {
                        dt.dat_Stamp   = CMFTP(entry)->LastVisited;
                        dt.dat_Format  = FORMAT_DEF;
                        dt.dat_Flags   = DTF_SUBST;
                        dt.dat_StrDay  = NULL;
                        dt.dat_StrDate = date;
                        dt.dat_StrTime = time;

                        DateToStr(&dt);

                        sprintf(data->Buffers[col],"%s %s",date,time);
                    }

                    return data->Buffers[col];
                }

                case FTPCOL_LastModified:
                {
                    REGARRAY UBYTE  date[LEN_DATSTRING], time[LEN_DATSTRING];
                    struct DateTime dt;

                    if (ISZERO(CMFTP(entry)->LastModified)) strcpy(data->Buffers[col],CM_GetString(Msg_Never));
                    else
                    {
                        dt.dat_Stamp   = CMFTP(entry)->LastModified;
                        dt.dat_Format  = FORMAT_DEF;
                        dt.dat_Flags   = DTF_SUBST;
                        dt.dat_StrDay  = NULL;
                        dt.dat_StrDate = date;
                        dt.dat_StrTime = time;

                        DateToStr(&dt);

                        sprintf(data->Buffers[col],"%s %s",date,time);
                    }

                    return data->Buffers[col];
                }

                case FTPCOL_Comment: return CMFTP(entry)->Comment;
                case FTPCOL_Alias:   return CMFTP(entry)->Alias;
                case FTPCOL_EMail:   return CMFTP(entry)->EMail;
            }

        case CMEntry_Chat:
            switch (data->ListPrefs.ChatCols[col])
            {
                case CHATCOL_Channel: return CMCHAT(entry)->Channel;
                case CHATCOL_Server:  return CMCHAT(entry)->Server;
                case CHATCOL_Comment: return CMCHAT(entry)->Comment;
                case CHATCOL_EMail:   return CMCHAT(entry)->EMail;
                case CHATCOL_WWW:     return CMCHAT(entry)->WWW;
            }

        case CMEntry_Telnet:
            switch (data->ListPrefs.TelnetCols[col])
            {
                case TELNETCOL_Host:    return CMTELNET(entry)->Host;
                case TELNETCOL_Comment: return CMTELNET(entry)->Comment;
                case TELNETCOL_EMail:   return CMTELNET(entry)->EMail;
                case TELNETCOL_Alias:   return CMTELNET(entry)->Alias;
                case TELNETCOL_Port:
                    sprintf(data->Buffers[col],"%ld",CMTELNET(entry)->Port);
                    return data->Buffers[col];
            }

        default:
            return NULL;
    }
}

/***********************************************************************/

#ifdef __MORPHOS__
static ULONG DispFun(void)
{
    struct Hook         *Hook = (struct Hook *)REG_A0;
    //Object              *obj  = (Object *)REG_A2;
    struct NList_DisplayMessage *msg = (struct NList_DisplayMessage *)REG_A1;

#else
static ULONG SAVEDS ASM DispFun(REG(a0,struct Hook *Hook),REG(a2,Object *obj),REG(a1,struct NList_DisplayMessage *msg))
{
#endif

    register struct data    *data = Hook->h_Data;
    register struct CMEntry *entry = msg->entry;
    register STRPTR         *a = (STRPTR *)msg->strings, *p = (STRPTR *)msg->preparses;

    if (entry)
    {
        switch (entry->Type)
        {
            case CMEntry_User:
                *a++ = getDisplayCol(data,entry,0,p);
                *a++ = getDisplayCol(data,entry,1,p);
                *a++ = getDisplayCol(data,entry,2,p);
                *a   = getDisplayCol(data,entry,3,p);
                break;

            case CMEntry_WWW:
                *a++ = getDisplayCol(data,entry,0,p);
                *a++ = getDisplayCol(data,entry,1,p);
                *a++ = getDisplayCol(data,entry,2,p);
                *a++ = getDisplayCol(data,entry,3,p);
                *a   = getDisplayCol(data,entry,4,p);
                break;

            case CMEntry_FTP:
                *a++ = getDisplayCol(data,entry,0,p);
                *a++ = getDisplayCol(data,entry,1,p);
                *a++ = getDisplayCol(data,entry,2,p);
                *a++ = getDisplayCol(data,entry,3,p);
                *a   = getDisplayCol(data,entry,4,p);
                break;

            case CMEntry_Chat:
                *a++ = getDisplayCol(data,entry,0,p);
                *a++ = getDisplayCol(data,entry,1,p);
                *a   = getDisplayCol(data,entry,2,p);
                break;

            case CMEntry_Telnet:
                *a++ = getDisplayCol(data,entry,0,p);
                *a   = getDisplayCol(data,entry,1,p);
                break;
        }
    }
    else
    {
        switch (data->Type)
        {
            case CMEntry_User:
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.UserCols[0]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.UserCols[1]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.UserCols[2]));
                *a   = CM_GetString(colToID(data->Type,data->ListPrefs.UserCols[3]));
                break;

            case CMEntry_WWW:
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.WWWCols[0]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.WWWCols[1]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.WWWCols[2]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.WWWCols[3]));
                *a   = CM_GetString(colToID(data->Type,data->ListPrefs.WWWCols[4]));
                break;

            case CMEntry_FTP:
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.FTPCols[0]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.FTPCols[1]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.FTPCols[2]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.FTPCols[3]));
                *a   = CM_GetString(colToID(data->Type,data->ListPrefs.FTPCols[4]));
                break;

            case CMEntry_Chat:
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.ChatCols[0]));
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.ChatCols[1]));
                *a   = CM_GetString(colToID(data->Type,data->ListPrefs.ChatCols[2]));
                break;

            case CMEntry_Telnet:
                *a++ = CM_GetString(colToID(data->Type,data->ListPrefs.TelnetCols[0]));
                *a   = CM_GetString(colToID(data->Type,data->ListPrefs.TelnetCols[1]));
                break;
        }

        if (data->Type && (data->Flags & FLG_BoldTitles))
        {
            *p++ = MUIX_B;
            *p++ = MUIX_B;
            *p++ = MUIX_B;
            *p++ = MUIX_B;
            *p   = MUIX_B;
        }
    }

    return 0;
}

/***********************************************************************/

static int
compareCol(struct data *data,struct CMEntry *e1,struct CMEntry *e2,int col)
{
    switch (e1->Type)
    {
        case CMEntry_User:
        {
            register APTR  a= NULL, b = NULL; //gcc
            register ULONG birth = 0, use1 = 0, use2 = 0; //gcc
            register int   res;

            switch (data->ListPrefs.UserCols[col])
            {
                case USERCOL_Name:        a = CMUSER(e1)->VisualName; b = CMUSER(e2)->VisualName; break;
                case USERCOL_EMail:       a = CMUSER(e1)->EMail;      b = CMUSER(e2)->EMail;      break;
                case USERCOL_Comment:     a = CMUSER(e1)->Comment;    b = CMUSER(e2)->Comment;    break;
                case USERCOL_Alias:       a = CMUSER(e1)->Alias;      b = CMUSER(e2)->Alias;      break;
                case USERCOL_Country:     a = CMUSER(e1)->Country;    b = CMUSER(e2)->Country;    break;
                case USERCOL_City:        a = CMUSER(e1)->City;       b = CMUSER(e2)->City;       break;
                case USERCOL_Phone:       a = CMUSER(e1)->Phone;      b = CMUSER(e2)->Phone;      break;
                case USERCOL_Mobile:      a = CMUSER(e1)->Mobile;     b = CMUSER(e2)->Mobile;     break;
                case USERCOL_Fax:         a = CMUSER(e1)->Fax;        b = CMUSER(e2)->Fax;        break;
                case USERCOL_WWW:         a = CMUSER(e1)->WWW;        b = CMUSER(e2)->WWW;        break;
                case USERCOL_Company:     a = CMUSER(e1)->Company;    b = CMUSER(e2)->Company;    break;
                case USERCOL_BPhone:      a = CMUSER(e1)->BPhone;     b = CMUSER(e2)->BPhone;     break;
                case USERCOL_BMobile:     a = CMUSER(e1)->BMobile;    b = CMUSER(e2)->BMobile;    break;
                case USERCOL_BFax:        a = CMUSER(e1)->BFax;       b = CMUSER(e2)->BFax;       break;
        case USERCOL_Birthday:    a = (STRPTR)CMUSER(e1)->Birthday;    b = (STRPTR)CMUSER(e2)->Birthday;    birth = 1; use1 = CMUSER(e1)->Flags & CMUSERFLG_Birthday;    use2 = CMUSER(e2)->Flags & CMUSERFLG_Birthday;    break;
        case USERCOL_Anniversary: a = (STRPTR)CMUSER(e1)->Anniversary; b = (STRPTR)CMUSER(e2)->Anniversary; birth = 1; use1 = CMUSER(e1)->Flags & CMUSERFLG_Anniversary; use2 = CMUSER(e2)->Flags & CMUSERFLG_Anniversary; break;
            }

            if (birth)
            {
                if (use1)
                    if (use2) res = (ULONG)a-(ULONG)b;
                    else res = -1;
                else res = 1;
            }
            else res = StrnCmp(lib_locale,a,b,-1,SC_COLLATE2);

            return res;
        }

        case CMEntry_WWW:
        {
            register APTR  a = NULL, b = NULL; //gcc
            register ULONG date = 0;
            register int   res;

            switch (data->ListPrefs.WWWCols[col])
            {
                case WWWCOL_Name:    a = CMWWW(e1)->Name;    b = &CMWWW(e2)->Name;   break;
                case WWWCOL_Site:    a = CMWWW(e1)->WWW;     b = &CMWWW(e2)->WWW;    break;
                case WWWCOL_Comment: a = CMWWW(e1)->Comment; b = CMWWW(e2)->Comment; break;
                case WWWCOL_Alias:   a = CMWWW(e1)->Alias;   b = CMWWW(e2)->Alias;   break;
                case WWWCOL_EMail:   a = CMWWW(e1)->EMail;   b = CMWWW(e2)->EMail;   break;
                case WWWCOL_LastVisited:  a = (STRPTR)&CMWWW(e1)->LastVisited;  b = (STRPTR)&CMWWW(e2)->LastVisited;  date = 1; break;
                case WWWCOL_LastModified: a = (STRPTR)&CMWWW(e1)->LastModified; b = (STRPTR)&CMWWW(e2)->LastModified; date = 1; break;
            }

            if (date) res = CompareDates((struct DateStamp *)a,(struct DateStamp *)b);
            else res = StrnCmp(lib_locale,a,b,-1,SC_COLLATE2);

            return res;
        }

        case CMEntry_FTP:
        {
            register STRPTR a = NULL, b = NULL; //gcc
            register ULONG  date = 0;
            register int    res;

            switch (data->ListPrefs.FTPCols[col])
            {
                case FTPCOL_Name:    a = CMFTP(e1)->Name;    b = CMFTP(e2)->Name;    break;
                case FTPCOL_Site:    a = CMFTP(e1)->FTP;     b = CMFTP(e2)->FTP;     break;
                case FTPCOL_Comment: a = CMFTP(e1)->Comment; b = CMFTP(e2)->Comment; break;
                case FTPCOL_Alias:   a = CMFTP(e1)->Alias;   b = CMFTP(e2)->Alias;   break;
                case FTPCOL_EMail:   a = CMFTP(e1)->EMail;   b = CMFTP(e2)->EMail;   break;
                case FTPCOL_LastVisited:  a = (STRPTR)&CMFTP(e1)->LastVisited,  b = (STRPTR)&CMFTP(e2)->LastVisited;  date = 1; break;
                case FTPCOL_LastModified: a = (STRPTR)&CMFTP(e1)->LastModified, b = (STRPTR)&CMFTP(e2)->LastModified; date = 1; break;
            }

            if (date) res = CompareDates((struct DateStamp *)a,(struct DateStamp *)b);
            else res = StrnCmp(lib_locale,a,b,-1,SC_COLLATE2);

            return res;
        }

        case CMEntry_Chat:
        {
            register APTR a = NULL, b = NULL; //gcc
            register int  res;

            switch (data->ListPrefs.ChatCols[col])
            {
                case CHATCOL_Channel: a = CMCHAT(e1)->Channel; b = CMCHAT(e2)->Channel; break;
                case CHATCOL_Server:  a = CMCHAT(e1)->Server;  b = CMCHAT(e2)->Server;  break;
                case CHATCOL_Comment: a = CMCHAT(e1)->Comment; b = CMCHAT(e2)->Comment; break;
                case CHATCOL_EMail:   a = CMCHAT(e1)->EMail;   b = CMCHAT(e2)->EMail;   break;
            }

            res = StrnCmp(lib_locale,a,b,-1,SC_COLLATE2);

            return res;
        }

        case CMEntry_Telnet:
        {
            register APTR  a = NULL, b = NULL; //gcc
            register ULONG port = 0;
            register int   res;

            switch (data->ListPrefs.TelnetCols[col])
            {
                case TELNETCOL_Host:    a = CMTELNET(e1)->Host;    b = CMTELNET(e2)->Host;    break;
                case TELNETCOL_Comment: a = CMTELNET(e1)->Comment; b = CMTELNET(e2)->Comment; break;
                case TELNETCOL_EMail:   a = CMTELNET(e1)->EMail;   b = CMTELNET(e2)->EMail;   break;
                case TELNETCOL_Alias:   a = CMTELNET(e1)->Alias;   b = CMTELNET(e2)->Alias;   break;
                case TELNETCOL_Port:    a = (APTR)CMTELNET(e1)->Port; b = (APTR)CMTELNET(e2)->Port, port = 1; break;
            }

            if (port) res = (LONG)a-(LONG)b;
            else res = StrnCmp(lib_locale,a,b,-1,SC_COLLATE2);

            return res;
        }

        default:
            return 0;
    }
}

static LONG
compare(struct data *data,struct CMEntry *e1,struct CMEntry *e2,LONG col,ULONG rev)
{
    register LONG res = compareCol(data,e1,e2,col);

    return rev ? -res : res;
}

static ULONG
mCompare(struct IClass *cl,Object *obj,struct MUIP_NList_Compare *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct CMEntry *e1 = msg->entry1, *e2 = msg->entry2;
    register ULONG          sort1 = msg->sort_type1, sort2 = msg->sort_type2;
    register LONG           col1 = sort1 & MUIV_NList_TitleMark_ColMask, col2 = sort2 & MUIV_NList_TitleMark2_ColMask, res;

    if (data->Flags & FLG_ManualSort) return 0;

    if ((sort1 & MUIV_NList_SortType_None)==MUIV_NList_SortType_None) return 0;

    res = compare(data,e1,e2,col1,sort1 & MUIV_NList_TitleMark_TypeMask);
    if (res || (col1==col2) || ((sort2 & MUIV_NList_SortType_None)==MUIV_NList_SortType_None))
        return (ULONG)res;

    return (ULONG)compare(data,e1,e2,col2,sort2 & MUIV_NList_TitleMark2_TypeMask);
}

/***************************************************************************/

static ULONG
mSelect(struct IClass *cl,Object *obj,struct MUIP_NList_Select *msg)
{
    if (msg->pos==MUIV_NList_Select_All)
    {
        register ULONG res;

        if (msg->seltype==MUIV_NList_Select_Off)
        {
            res = DoSuperMethodA(cl,obj,(Msg)msg);
            DoSuperMethod(cl,obj,MUIM_NList_Select,MUIV_NList_Select_Active,MUIV_NList_Select_Off,NULL);

            return res;
        }
        else
            if (msg->seltype==MUIV_NList_Select_Toggle)
            {
                ULONG sel;

                DoSuperMethod(cl,obj,MUIM_NList_Select,MUIV_NList_Select_Active,MUIV_NList_Select_Ask,&sel);
                res = DoSuperMethodA(cl,obj,(Msg)msg);
                DoSuperMethod(cl,obj,MUIM_NList_Select,MUIV_NList_Select_Active,!sel,NULL);

                return res;
            }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***************************************************************************/

static void
getFormat(struct data *data,struct Format *f)
{
    register ULONG noBar = data->Flags & FLG_NoListBar;
    register int   idx = 0, cols = 0; //gcc

    switch (data->Type)
    {
        case CMEntry_User:
            f->format = noBar ? ",,," : "BAR,BAR,BAR,BAR";
            idx       = IDX_User;
            cols      = COLS_User;
            break;

        case CMEntry_WWW:
            f->format = noBar ? ",,,," : "BAR,BAR,BAR,BAR,BAR";
            idx       = IDX_WWW;
            cols      = COLS_WWW;
            break;

        case CMEntry_FTP:
            f->format = noBar ? ",,," : "BAR,BAR,BAR,BAR";
            idx       = IDX_FTP;
            cols      = COLS_FTP;
            break;

        case CMEntry_Chat:
            f->format = noBar ? ",," : "BAR,BAR,BAR";
            idx       = IDX_Chat;
            cols      = COLS_Chat;
            break;

        case CMEntry_Telnet:
            f->format = noBar ? "," : "BAR,BAR";
            idx       = IDX_Telnet;
            cols      = COLS_Telnet;
            break;
    }

    f->c0    = data->ListPrefs.Sorts[idx].SortCol0;
    f->c1    = data->ListPrefs.Sorts[idx].SortCol1;
    f->o     = data->ListPrefs.Sorts[idx].Order;
    f->width = data->ListPrefs.Sorts[idx].Width;
    f->quick = idx;
    f->cols  = cols;
}

/***************************************************************************/

static void
getOrder(struct IClass *cl,Object *obj,struct data *data)
{
    BYTE         *o;
    register int i, idx, cols;

    switch (data->Type)
    {
        case CMEntry_User:   idx  = IDX_User;   cols = COLS_User;   break;
        case CMEntry_WWW:    idx  = IDX_WWW;    cols = COLS_WWW;    break;
        case CMEntry_FTP:    idx  = IDX_FTP;    cols = COLS_FTP;    break;
        case CMEntry_Chat:   idx  = IDX_Chat;   cols = COLS_Chat;   break;
        case CMEntry_Telnet: idx  = IDX_Telnet; cols = COLS_Telnet; break;
        default: return;
    }

    superget(cl,obj,MUIA_NList_TitleMark,&data->ListPrefs.Sorts[idx].SortCol0);
    superget(cl,obj,MUIA_NList_TitleMark2,&data->ListPrefs.Sorts[idx].SortCol1);
    superget(cl,obj,MUIA_NList_Columns,&o);

    for (i = 0; i<cols; i++)
    {
        data->ListPrefs.Sorts[idx].Order[i] = o[i];
        data->ListPrefs.Sorts[idx].Width[i] = (int)DoSuperMethod(cl,obj,MUIM_NList_ColWidth,i,MUIV_NList_ColWidth_Get);
    }

    data->ListPrefs.Sorts[idx].Order[i] = -1;
}

/***************************************************************************/

static void
makeMenus(struct data *data)
{
    switch (data->Type)
    {
        case CMEntry_User:
            data->ContextMenu      = MUI_MakeObject(MUIO_MenustripNM,UserMenu,0);
            data->TitleContextMenu = MUI_MakeObject(MUIO_MenustripNM,UserTitleMenu,0);
            break;

        case CMEntry_WWW:
            data->ContextMenu      = MUI_MakeObject(MUIO_MenustripNM,WWWMenu,0);
            data->TitleContextMenu = MUI_MakeObject(MUIO_MenustripNM,WWWTitleMenu,0);
            break;

        case CMEntry_FTP:
            data->ContextMenu      = MUI_MakeObject(MUIO_MenustripNM,FTPMenu,0);
            data->TitleContextMenu = MUI_MakeObject(MUIO_MenustripNM,FTPTitleMenu,0);
            break;

        case CMEntry_Chat:
            data->ContextMenu      = MUI_MakeObject(MUIO_MenustripNM,ChatMenu,0);
            data->TitleContextMenu = MUI_MakeObject(MUIO_MenustripNM,ChatTitleMenu,0);
            break;

        case CMEntry_Telnet:
            data->ContextMenu      = MUI_MakeObject(MUIO_MenustripNM,TelnetMenu,0);
            data->TitleContextMenu = MUI_MakeObject(MUIO_MenustripNM,TelnetTitleMenu,0);
            break;
    }
}

/***************************************************************************/

static void
freeMenus(struct data *data)
{
    if (data->ContextMenu)
    {
        MUI_DisposeObject(data->ContextMenu);
        data->ContextMenu = NULL;
    }

    if (data->TitleContextMenu)
    {
        MUI_DisposeObject(data->TitleContextMenu);
        data->TitleContextMenu = NULL;
    }
}

/***************************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct TagItem *attrs = msg->ops_AttrList;

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_ContextMenu,          MUIV_NList_ContextMenu_Always,
            MUIA_NList_Title,          "",
            MUIA_NList_AutoVisible,    TRUE,
            MUIA_NList_DragSortable,   FALSE,
            MUIA_NList_ShowDropMarks,  FALSE,
            MUIA_NList_DragType,       MUIV_NList_DragType_Default,
            MUIA_NList_MultiSelect,    MUIV_NList_MultiSelect_Default,
            MUIA_NList_MinColSortable, 0,
            TAG_MORE,attrs))
    {
        register struct data *data = INST_DATA(cl,obj);

        if ((data->RexxPortSig = AllocSignal(-1))>=0)
            INITPORT(&data->RexxPort,data->RexxPortSig);

#ifdef __MORPHOS__
        data->DispTrap.Trap      = TRAP_LIB;
        data->DispTrap.Extension = 0;
        data->DispTrap.Func      = (APTR)DispFun;

        data->DispHook.h_Entry   = (HOOKFUNC)&data->DispTrap;
#else
        data->DispHook.h_Entry = (HOOKFUNC)DispFun;
#endif
        data->DispHook.h_Data  = data;

        superset(cl,obj,MUIA_NList_DisplayHook2,&data->DispHook);

        data->Flags = (GetTagData(MUIA_EntryList_NoMenu,FALSE,attrs) ? FLG_NoMenu : 0) |
                      (GetTagData(MUIA_CManager_AtHome,FALSE,attrs) ? FLG_AtHome : 0);


        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_NList_TitleClick,MUIV_EveryTime,obj,4,MUIM_NList_Sort3,MUIV_TriggerValue,MUIV_NList_SortTypeAdd_2Values,MUIV_NList_Sort3_SortType_Both);
        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_NList_SortType,MUIV_EveryTime,obj,3,MUIM_Set,MUIA_NList_TitleMark,MUIV_TriggerValue);
    }

    return (ULONG)obj;
}

/***************************************************************************/

static ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    freeMenus(data);

    if (data->RexxPortSig>=0)
    {
        while (data->RexxOut)
        {
            register struct RexxMsg *RxMsg;

            WaitPort(&data->RexxPort);

            if (RxMsg = (struct RexxMsg *)GetMsg(&data->RexxPort))
            {
                DeleteArgstring(RxMsg->rm_Args[0]);
                DeleteRexxMsg(RxMsg);

                data->RexxOut--;
            }
        }

        FreeSignal(data->RexxPortSig);
    }

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_EntryList_Type:
            *msg->opg_Storage = data->Type;
            return TRUE;

        case MUIA_EntryList_ListPrefs:
            getOrder(cl,obj,data);
            *msg->opg_Storage = (ULONG)&data->ListPrefs;
            return TRUE;

        default:
            return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct TagItem *tag;
    struct TagItem          *tags;

    if (freeStack()<MINSTACKSIZE) return 0;

    for (tags = msg->ops_AttrList; tag = NextTagItem(&tags); )
    {
        register ULONG tidata = tag->ti_Data;

        switch (tag->ti_Tag)
        {
            case MUIA_EntryList_NoMenu:
                if (tidata) data->Flags |= FLG_NoMenu;
                else data->Flags &= ~FLG_NoMenu;
                if (tidata && data->ContextMenu)
                {
                    if (data->ContextMenu) MUI_DisposeObject(data->ContextMenu);
                    data->ContextMenu = NULL;
                }
                break;

            case MUIA_EntryList_Type:
            {
                struct Format f;
                register int  i;

                if (data->Type==tidata) break;

                getOrder(cl,obj,data);
                data->Type = tidata;
                getFormat(data,&f);

                SetSuperAttrs(cl,obj,MUIA_NList_Format,   f.format,
                                     MUIA_NList_SortType, f.c0,
                                     MUIA_NList_Columns,  f.o,
                                     TAG_DONE);

                for (i = 0; i<f.cols; i++) DoSuperMethod(cl,obj,MUIM_NList_ColWidth,i,f.width[i]);

                freeMenus(data);
                if (!(data->Flags & FLG_NoMenu)) makeMenus(data);

                break;
            }

            case MUIA_EntryList_Group:
                if (tidata!=MUIV_NListtree_Active_Off)
                {
                    register struct CMEntry *entry;

                    data->Group = CMGROUP(((struct MUI_NListtree_TreeNode *)tidata)->tn_User);

                    DoSuperMethod(cl,obj,MUIM_NList_Clear);

                    while (entry = CMENTRY(RemHead((struct List *)&data->Group->Entries)))
                        DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,entry,(data->Flags & FLG_ManualSort) ? MUIV_NList_Insert_Bottom : MUIV_NList_Insert_Sorted);
                }
                break;

            case MUIA_CManager_CMObj:
                data->CMObj = (Object *)tidata;
                break;

            case MUIA_CManager_Prefs:
            {
                register ULONG redraw = 0, sort = FALSE;

                data->Prefs = (struct CMPrefs *)tidata;

                if (!BOOLSAME(data->Flags & FLG_NoListBar,data->Prefs->Options & OPT_NoListBar) ||
                    memcmp(&data->ListPrefs,&data->Prefs->VisualPrefs.ListPrefs,sizeof(struct ListPrefs)))
                {
                    struct Format f;
                    register int  i;

                    copymem(&data->ListPrefs,&data->Prefs->VisualPrefs.ListPrefs,sizeof(struct ListPrefs));
                    if (data->Prefs->Options & OPT_NoListBar) data->Flags |= FLG_NoListBar;
                    else data->Flags &= ~FLG_NoListBar;

                    if (data->Type)
                    {
                        getFormat(data,&f);

                        SetAttrs(obj,MUIA_NList_Format,   f.format,
                                     MUIA_NList_SortType, f.c0,
                                     MUIA_NList_Columns,  f.o,
                                     TAG_DONE);

                        for (i = 0; i<f.cols; i++) DoSuperMethod(cl,obj,MUIM_NList_ColWidth,i,f.width[i]);

                        switch (data->Type)
                        {
                            case CMEntry_User:
                                for (i = 0; i<LP_NUMCOLS; i++)
                                    if (data->ListPrefs.UserCols[i]!=data->Prefs->VisualPrefs.ListPrefs.UserCols[i])
                                    {
                                        data->ListPrefs.UserCols[i] = data->Prefs->VisualPrefs.ListPrefs.UserCols[i];
                                        redraw = MUIV_NList_Redraw_All;
                                        sort = TRUE;
                                    }
                                break;

                            case CMEntry_WWW:
                                for (i = 0; i<LP_NUMCOLS; i++)
                                    if (data->ListPrefs.WWWCols[i]!=data->Prefs->VisualPrefs.ListPrefs.WWWCols[i])
                                    {
                                        data->ListPrefs.WWWCols[i] = data->Prefs->VisualPrefs.ListPrefs.WWWCols[i];
                                        redraw = MUIV_NList_Redraw_All;
                                        sort = TRUE;
                                    }
                                break;

                            case CMEntry_FTP:
                                for (i = 0; i<LP_NUMCOLS; i++)
                                    if (data->ListPrefs.FTPCols[i]!=data->Prefs->VisualPrefs.ListPrefs.FTPCols[i])
                                    {
                                        data->ListPrefs.FTPCols[i] = data->Prefs->VisualPrefs.ListPrefs.FTPCols[i];
                                        redraw = MUIV_NList_Redraw_All;
                                        sort = TRUE;
                                    }
                                break;

                            case CMEntry_Chat:
                                for (i = 0; i<LP_NUMCOLS; i++)
                                    if (data->ListPrefs.ChatCols[i]!=data->Prefs->VisualPrefs.ListPrefs.ChatCols[i])
                                    {
                                        data->ListPrefs.ChatCols[i] = data->Prefs->VisualPrefs.ListPrefs.ChatCols[i];
                                        redraw = MUIV_NList_Redraw_All;
                                        sort = TRUE;
                                    }
                                break;

                            case CMEntry_Telnet:
                                for (i = 0; i<LP_NUMCOLS; i++)
                                    if (data->ListPrefs.TelnetCols[i]!=data->Prefs->VisualPrefs.ListPrefs.TelnetCols[i])
                                    {
                                        data->ListPrefs.TelnetCols[i] = data->Prefs->VisualPrefs.ListPrefs.TelnetCols[i];
                                        redraw = MUIV_NList_Redraw_All;
                                        sort = TRUE;
                                    }
                                break;
                        }
                    }
                }

                if (!BOOLSAME(data->Flags & FLG_BoldTitles,data->Prefs->Options & OPT_BoldTitles))
                {
                    if (data->Prefs->Options & OPT_BoldTitles) data->Flags |= FLG_BoldTitles;
                    else data->Flags &= ~FLG_BoldTitles;

                    if (!redraw) redraw = MUIV_NList_Redraw_Title;
                }

                if (!BOOLSAME(data->Flags & FLG_ManualSort,data->Prefs->Options & OPT_ManualSort))
                {
                    if (data->Prefs->Options & OPT_ManualSort)
                    {
                        data->Flags |= FLG_ManualSort;

                        SetSuperAttrs(cl,obj,MUIA_NList_DragSortable,  TRUE,
                                             MUIA_NList_ShowDropMarks, TRUE,
                                             TAG_DONE);
                    }
                    else
                    {
                        data->Flags &= ~FLG_ManualSort;

                        SetSuperAttrs(cl,obj,MUIA_NList_DragSortable,  FALSE,
                                             MUIA_NList_ShowDropMarks, FALSE,
                                             TAG_DONE);
                    }

                    redraw = MUIV_NList_Redraw_All;
                }

                if (!BOOLSAME(data->Flags & FLG_IgnoreLast,data->Prefs->Options & OPT_IgnoreLast))
                {
                    if (data->Prefs->Options & OPT_IgnoreLast) data->Flags |= FLG_IgnoreLast;
                    else data->Flags &= ~FLG_IgnoreLast;
                }

                if (redraw) DoSuperMethod(cl,obj,MUIM_NList_Redraw,redraw);
                if (sort) DoSuperMethod(cl,obj,MUIM_NList_Sort);

                break;
            }
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mShow(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!(DoSuperMethodA(cl,obj,msg))) return FALSE;

    memset(&data->eh,0,sizeof(data->eh));
    data->eh.ehn_Class  = cl;
    data->eh.ehn_Object = obj;
    data->eh.ehn_Events = IDCMP_RAWKEY;
    DoMethod(_win(obj),MUIM_Window_AddEventHandler,&data->eh);

    return TRUE;
}

/***************************************************************************/

static ULONG
mHide(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    DoMethod(_win(obj),MUIM_Window_RemEventHandler,&data->eh);

    return DoSuperMethodA(cl,obj,msg);
}

/***************************************************************************/

static ULONG
mSetup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,(Msg)msg)) return FALSE;

    if (data->RexxPortSig>=0)
    {
        data->RexxHandler.ihn_Object         = obj;
        data->RexxHandler.ihn_stuff.ihn_sigs = 1L<<data->RexxPortSig;
        data->RexxHandler.ihn_Method         = MUIM_EntryList_HandleARexx;

        DoMethod(_app(obj),MUIM_Application_AddInputHandler,&data->RexxHandler);
    }

    return TRUE;
}

/***********************************************************************/

static ULONG
mCleanup(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->RexxPortSig>=0) DoMethod(_app(obj),MUIM_Application_RemInputHandler,&data->RexxHandler);

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mHandleEvent(struct IClass *cl,Object *obj,struct MUIP_HandleEvent *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (msg->imsg)
    {
        Object *o;

        get(_win(obj),MUIA_Window_ActiveObject,&o);
        if (!o || o==obj)
        {
            get(_win(obj),MUIA_Window_DefaultObject,&o);
            if (o==obj)
            {
                switch (msg->imsg->Class)
                {
                    case IDCMP_RAWKEY:
                        switch (msg->imsg->Code)
                        {
                            // DEL
                            case 0x46:
                                DoMethod(data->CMObj,MUIM_CManager_RemSelected);
                                return 0;
                        }
                        break;
                }
            }
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mContextMenuChoice(struct IClass *cl,Object *obj,struct MUIP_ContextMenuChoice *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       item;

    switch (item = muiUserData(msg->item))
    {
        case ACT_EMAILHOME:          case ACT_EMAILBUSINESS: case ACT_BROWSER:
        case ACT_FTP:                case ACT_JOIN:          case ACT_TELNET:
        case ACT_CALLHOME:           case ACT_CALLMOBILE:    case ACT_CALLBUSINESS:
        case ACT_CALLBUSINESSMOBILE: case ACT_FAXHOME:       case ACT_FAXBUSINESS:
            DoMethod(obj,MUIM_EntryList_StartScript,item);
            break;

        case ACT_QUICKEMAIL:
        {
            struct CMEntry *entry;

            DoSuperMethod(cl,obj,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
            if (entry) DoMethod(data->CMObj,MUIM_CManager_MiniMailWindow,entry);
            break;
        }

        case ACT_COPY:
            DoMethod(obj,MUIM_EntryList_CopyToClip);
            break;

        case ACT_EDIT:
            DoMethod(obj,MUIM_EntryList_Edit);
            break;

        case MUIV_NList_Menu_DefWidth_This:
            DoSuperMethod(cl,obj,MUIM_NList_ColWidth,data->DispColumn,MUIV_NList_ColWidth_Default);
            break;

        case MUIV_NList_Menu_DefWidth_All:
            DoSuperMethod(cl,obj,MUIM_NList_ColWidth,MUIV_NList_ColWidth_All,MUIV_NList_ColWidth_Default);
            break;

        case MUIV_NList_Menu_DefOrder_This:
            DoSuperMethod(cl,obj,MUIM_NList_SetColumnCol,data->DispColumn,MUIV_NList_SetColumnCol_Default);
            break;

        case MUIV_NList_Menu_DefOrder_All:
            DoSuperMethod(cl,obj,MUIM_NList_SetColumnCol,MUIV_NList_SetColumnCol_Default,MUIV_NList_SetColumnCol_Default);
            break;

        case Msg_List_Menu_DefSort:
            SetSuperAttrs(cl,obj,MUIA_NList_SortType,0,TAG_DONE);
            DoSuperMethod(cl,obj,MUIM_NList_Sort);
            break;

        case ACT_CONFIGURE:
            DoMethod(data->CMObj,MUIM_CManager_PrefsWindow,MUIV_PrefsWindow_ActivePage_Programs);
            break;

        case Msg_Menu_Configure:
            DoMethod(data->CMObj,MUIM_CManager_PrefsWindow,MUIV_PrefsWindow_ActivePage_Options);
            break;

        case Msg_Title_Name:         case Msg_Title_EMail:
        case Msg_Title_Comment:      case Msg_Title_Alias:
        case Msg_Title_Birthday:     case Msg_Title_Anniversary:
        case Msg_Title_Country:      case Msg_Title_City:
        case Msg_Title_Phone:        case Msg_Title_Mobile:
        case Msg_Title_Fax:          case Msg_Title_WWW:
        case Msg_Title_Company:      case Msg_Title_BPhone:
        case Msg_Title_BMobile:      case Msg_Title_BFax:
        case Msg_Title_Site:         case Msg_Title_LastVisited:
        case Msg_Title_LastModified: case Msg_Title_Channel:
        case Msg_Title_Server:       case Msg_Title_Port:
        case Msg_Title_Host:
        {
            register ULONG v;

            v = IDToCol(data->Type,item);

            switch (data->Type)
            {
                case CMEntry_User:
                    data->ListPrefs.UserCols[data->Column] = v;
                    break;

                case CMEntry_WWW:
                    data->ListPrefs.WWWCols[data->Column] = v;
                    break;

                case CMEntry_FTP:
                    data->ListPrefs.FTPCols[data->Column] = v;
                    break;

                case CMEntry_Chat:
                    data->ListPrefs.ChatCols[data->Column] = v;
                    break;

                case CMEntry_Telnet:
                    data->ListPrefs.TelnetCols[data->Column] = v;
                    break;
            }

            DoMethod(data->CMObj,MUIM_CManager_SetFreeCol,data->Type,data->Column,v);
            DoSuperMethod(cl,obj,MUIM_NList_Redraw,MUIV_NList_Redraw_All);
            DoSuperMethod(cl,obj,MUIM_NList_Sort);

            break;
        }
    }

    return 0;
}

/***********************************************************************/

enum
{
    CFLG_WidthAll  = 1<<0,
    CFLG_WidthThis = 1<<1,
    CFLG_OrderAll  = 1<<2,
    CFLG_OrderThis = 1<<3,
    CFLG_Sort      = 1<<4,
};

static ULONG
mContextMenuBuild(struct IClass *cl,Object *obj,struct MUIP_NList_ContextMenuBuild *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register APTR        menu;
    
    if (!msg->ontop)
    {
        if (data->Prefs->Options & OPT_RightMouseActive)
        {
            struct MUI_NList_TestPos_Result tp;

            DoSuperMethod(cl,obj,MUIM_NList_TestPos,msg->mx,msg->my,&tp);
            if (tp.entry>=0) SetSuperAttrs(cl,obj,MUIA_NList_Active,tp.entry,TAG_DONE);
        }

        if (menu = data->ContextMenu)
        {
            struct CMEntry *entry;

            DoSuperMethod(cl,obj,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
            if (entry)
            {
                register LONG www = 0, ftp = 0, email = 0, copy = 0; //gcc

                switch (entry->Type)
                {
                    case CMEntry_User:
                        set((Object *)DoMethod(menu,MUIM_FindUData,Msg_Edit_MenuEMail),MUIA_Menuitem_Enabled,*CMUSER(entry)->EMail || *CMUSER(entry)->BEMail);
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_EMAILBUSINESS),MUIA_Menuitem_Enabled,*CMUSER(entry)->BEMail);
                        set((Object *)DoMethod(menu,MUIM_FindUData,Msg_Edit_MenuCall),MUIA_Menuitem_Enabled,*CMUSER(entry)->Phone || *CMUSER(entry)->Mobile || *CMUSER(entry)->BPhone || *CMUSER(entry)->BMobile);
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_CALLHOME),MUIA_Menuitem_Enabled,*CMUSER(entry)->Phone);
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_CALLMOBILE),MUIA_Menuitem_Enabled,*CMUSER(entry)->Mobile);
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_CALLBUSINESS),MUIA_Menuitem_Enabled,*CMUSER(entry)->BPhone);
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_CALLBUSINESSMOBILE),MUIA_Menuitem_Enabled,*CMUSER(entry)->BMobile);
                        set((Object *)DoMethod(menu,MUIM_FindUData,Msg_Edit_MenuFax),MUIA_Menuitem_Enabled,*CMUSER(entry)->Fax || *CMUSER(entry)->BFax);
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_FAXHOME),MUIA_Menuitem_Enabled,*CMUSER(entry)->Fax);
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_FAXBUSINESS),MUIA_Menuitem_Enabled,*CMUSER(entry)->BFax);

                        www   = *CMUSER(entry)->WWW;
                        ftp   = *CMUSER(entry)->FTP;
                        email = *CMUSER(entry)->EMail || *CMUSER(entry)->BEMail;
                        copy  = *CMUSER(entry)->EMail || *CMUSER(entry)->BEMail;
                        break;

                    case CMEntry_WWW:
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_EMAILHOME),MUIA_Menuitem_Enabled,*CMWWW(entry)->EMail);

                        www   = *CMWWW(entry)->WWW;
                        ftp   = -1;
                        email = *CMWWW(entry)->EMail;
                        copy  = *CMWWW(entry)->WWW;
                        break;

                    case CMEntry_FTP:
                        www   = *CMFTP(entry)->FTP;
                        ftp   = *CMFTP(entry)->FTP;
                        email = *CMFTP(entry)->EMail;
                        copy  = *CMFTP(entry)->FTP;
                        break;

                    case CMEntry_Chat:
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_JOIN),MUIA_Menuitem_Enabled,*CMCHAT(entry)->Server);

                        www   = *CMCHAT(entry)->WWW;
                        ftp   = -1;
                        email = *CMCHAT(entry)->EMail;
                        copy  = *CMCHAT(entry)->Channel;
                        break;

                    case CMEntry_Telnet:
                        set((Object *)DoMethod(menu,MUIM_FindUData,ACT_TELNET),MUIA_Menuitem_Enabled,*CMTELNET(entry)->Host);

                        www   = -1;
                        ftp   = -1;
                        email = *CMTELNET(entry)->EMail;
                        copy  = *CMTELNET(entry)->Host;
                        break;
                }

                if (www>=0)
                    set((Object *)DoMethod(menu,MUIM_FindUData,ACT_BROWSER),MUIA_Menuitem_Enabled,www);

                if (ftp>=0)
                    set((Object *)DoMethod(menu,MUIM_FindUData,ACT_FTP),MUIA_Menuitem_Enabled,ftp);

                if (email>=0)
                    set((Object *)DoMethod(menu,MUIM_FindUData,ACT_EMAILHOME),MUIA_Menuitem_Enabled,email);

                if (copy>=0)
                    set((Object *)DoMethod(menu,MUIM_FindUData,ACT_COPY),MUIA_Menuitem_Enabled,copy);

                return (ULONG)menu;
            }
        }
    }

    data->Column = (WORD)DoSuperMethod(cl,obj,MUIM_NList_ColumnToCol,msg->column);
    menu = data->TitleContextMenu;

    if (menu)
    {
        BYTE           *o;
        register ULONG flags = 0;
        ULONG          s;
        register ULONG this;
        register int   i;
        register WORD  column;

        column = data->DispColumn = msg->column;

        superget(cl,obj,MUIA_NList_Columns,&o);

        for (i = 0; o[i]>=0; i++)
        {
            if (o[i]!=i)
            {
                if (i==column) flags |= CFLG_OrderThis;
                flags |= CFLG_OrderAll;
            }

            if ((int)DoSuperMethod(cl,obj,MUIM_NList_ColWidth,i,MUIV_NList_ColWidth_Get)!=-1)
            {
                if (i==column) flags |= CFLG_WidthThis;
                flags |= CFLG_WidthAll;
            }
        }

        superget(cl,obj,MUIA_NList_SortType,&s);
        if (s!=0) flags |= CFLG_Sort;

        set((Object *)DoMethod(menu,MUIM_FindUData,MUIV_NList_Menu_DefWidth_This),MUIA_Menuitem_Enabled,flags & CFLG_WidthThis);
        set((Object *)DoMethod(menu,MUIM_FindUData,MUIV_NList_Menu_DefWidth_All), MUIA_Menuitem_Enabled,flags & CFLG_WidthAll);
        set((Object *)DoMethod(menu,MUIM_FindUData,MUIV_NList_Menu_DefOrder_This),MUIA_Menuitem_Enabled,flags & CFLG_OrderThis);
        set((Object *)DoMethod(menu,MUIM_FindUData,MUIV_NList_Menu_DefOrder_All), MUIA_Menuitem_Enabled,flags & CFLG_OrderAll);
        set((Object *)DoMethod(menu,MUIM_FindUData,Msg_List_Menu_DefSort),        MUIA_Menuitem_Enabled,flags & CFLG_Sort);


        switch (data->Type)
        {
            case CMEntry_User:
                this  = data->ListPrefs.UserCols[data->Column];
                set((Object *)DoMethod(menu,MUIM_FindUData,colToID(data->Type,this)),MUIA_Menuitem_Checked,TRUE);
                break;

            case CMEntry_WWW:
                this  = data->ListPrefs.WWWCols[data->Column];
                set((Object *)DoMethod(menu,MUIM_FindUData,colToID(data->Type,this)),MUIA_Menuitem_Checked,TRUE);
                break;

            case CMEntry_FTP:
                this  = data->ListPrefs.FTPCols[data->Column];
                set((Object *)DoMethod(menu,MUIM_FindUData,colToID(data->Type,this)),MUIA_Menuitem_Checked,TRUE);
                break;

            case CMEntry_Chat:
                this  = data->ListPrefs.ChatCols[data->Column];
                set((Object *)DoMethod(menu,MUIM_FindUData,colToID(data->Type,this)),MUIA_Menuitem_Checked,TRUE);
                break;

            case CMEntry_Telnet:
                this  = data->ListPrefs.TelnetCols[data->Column];
                set((Object *)DoMethod(menu,MUIM_FindUData,colToID(data->Type,this)),MUIA_Menuitem_Checked,TRUE);
                break;
        }

        return (ULONG)menu;
    }

    return (ULONG)-1;
}

/***********************************************************************/

static void
closeEditWindow(Object *AppObj,APTR Entry)
{
    struct List     *list;
    Object          *anchor;
    register Object *wnd;

    get(AppObj,MUIA_Application_WindowList,&list);

    anchor = (Object *)list->lh_Head;

    while (wnd = NextObject(&anchor))
        if ((APTR)xget(wnd,MUIA_EditWindow_Entry)==Entry)
        {
            DoMethod(wnd,MUIM_CManager_WinClose,FALSE);
            return;
        }
}

/***********************************************************************/

static ULONG
mRemove(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMEntry       *entry;
    LONG                 pos;
    register ULONG        changed = FALSE;

    set(obj,MUIA_NList_Quiet,MUIV_NList_Quiet_Visual);

    for (pos = MUIV_NList_NextSelected_Start; ; )
    {
        DoSuperMethod(cl,obj,MUIM_NList_NextSelected,&pos);
        if (pos==MUIV_NList_NextSelected_End) break;

        DoSuperMethod(cl,obj,MUIM_NList_GetEntry,pos,&entry);
        entry->Flags &= ~CMENTRYFLG_New;
        closeEditWindow(_app(obj),entry);
        CM_FreeEntry(entry);

        changed = TRUE;
    }

    if (changed)
    {
        DoSuperMethod(cl,obj,MUIM_NList_Remove,MUIV_NList_Remove_Selected);
        DoSuperMethod(cl,obj,MUIM_NList_Select,MUIV_NList_Select_All,MUIV_NList_Select_Off,NULL);

        DoMethod(data->CMObj,MUIM_CManager_ChangeQuick);
        set(data->CMObj,MUIA_CManager_Changed,TRUE);
    }

    set(obj,MUIA_NList_Quiet,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mClone(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMEntry       *from;
    LONG                 pos;
    struct MinList       toadd;
    register ULONG        changed = FALSE;

    pos = MUIV_NList_NextSelected_Start;

    NEWLIST(&toadd);

    DoSuperMethod(cl,obj,MUIM_NList_NextSelected,&pos);
    while (pos!=MUIV_NList_NextSelected_End)
    {
        register struct CMEntry *to;
        register ULONG          size = 0; //gcc

        DoSuperMethod(cl,obj,MUIM_NList_GetEntry,pos,&from);

        switch (from->Type)
        {
            case CMEntry_User:
                size = sizeof(struct CMUser);
                break;

            case CMEntry_WWW:
                size = sizeof(struct CMWWW);
                break;

            case CMEntry_FTP:
                size = sizeof(struct CMFTP);
                break;

            case CMEntry_Chat:
                size = sizeof(struct CMChat);
                break;

            case CMEntry_Telnet:
                size = sizeof(struct CMTelnet);
                break;
        }

        if (to = CM_AllocEntry(from->Type))
        {
            copymem(to,from,size);
            AddTail((struct List *)&toadd,(struct Node *)to);
        }

        DoSuperMethod(cl,obj,MUIM_NList_NextSelected,&pos);

        changed = TRUE;
    }

    set(obj,MUIA_NList_Quiet,MUIV_NList_Quiet_Visual);

    while (from = CMENTRY(RemHead((struct List *)&toadd)))
        DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,from,(data->Flags & FLG_ManualSort) ? MUIV_NList_Insert_Bottom : MUIV_NList_Insert_Sorted);

    SetAttrs(obj,MUIA_NList_Active, xget(obj,MUIA_NList_InsertPosition),
                 MUIA_NList_Quiet, FALSE,
                 TAG_DONE);

    if (changed) set(data->CMObj,MUIA_CManager_Changed,TRUE);

    return 0;
}

/***********************************************************************/

static ULONG
mDoubleClick(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMEntry       *entry;

    DoSuperMethod(cl,obj,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
    if (entry)
    {
        switch (data->Prefs->Action)
        {
            case ACTION_EDIT:
                DoMethod(_app(obj),MUIM_Application_PushMethod,obj,1,MUIM_EntryList_Edit);
                break;

            case ACTION_SCRIPT:
            case ACTION_SCRIPT_CLOSE:
            case ACTION_SCRIPT_ICONIFY:
                if ((entry->Type==CMEntry_WWW) || (entry->Type==CMEntry_FTP) || (entry->Type==CMEntry_User))
                {
                    if (entry->Type==CMEntry_User)
                    	DoMethod(_app(obj),MUIM_Application_PushMethod,obj,2,MUIM_EntryList_StartScript,*CMUSER(entry)->EMail ? ACT_EMAILHOME : ACT_EMAILBUSINESS);
		    else DoMethod(_app(obj),MUIM_Application_PushMethod,obj,2,MUIM_EntryList_StartScript,(entry->Type==CMEntry_WWW) ? ACT_BROWSER : ACT_FTP);

                    if ((data->Prefs->Action!=ACTION_SCRIPT) && (data->Flags & FLG_AtHome))
                    {
                        switch (data->Prefs->Action)
                        {
                            case ACTION_SCRIPT_CLOSE:
                                DoMethod(_app(obj),MUIM_Application_PushMethod,_app(obj),2,MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
                                break;

                            case ACTION_SCRIPT_ICONIFY:
                                set(_app(obj),MUIA_Application_Iconified,TRUE);
                                break;
                        }
                    }
                }
                else DoMethod(_app(obj),MUIM_Application_PushMethod,obj,1,MUIM_EntryList_Edit);
                break;
        }
    }

    return 0;
}

/***********************************************************************/

static ULONG
emailFun(struct data *data,STRPTR email,STRPTR Command,ULONG useOURL)
{
    if (!*email) return TRUE;

    if (useOURL)
    {
        sprintf(Command,"mailto:%s",email);
        (void)URL_OpenA(Command,NULL);

        return TRUE;
    }

    strcat(Command,email);
    strcat(Command,"\" \"");
    strcat(Command,data->Prefs->MailPath);

    return FALSE;
}

static ULONG
urlFun(struct data *data,Object *obj,ULONG WWW,STRPTR url,STRPTR Command,ULONG useOURL,struct CMEntry *entry)
{
    if (!*url) return TRUE;

    if (entry)
    {
        if (entry->Type==CMEntry_WWW) DateStamp(&CMWWW(entry)->LastVisited);
        else DateStamp(&CMFTP(entry)->LastVisited);

        DoMethod(obj,MUIM_NList_RedrawEntry,entry);
        DoMethod(obj,MUIM_NList_Sort);

        if (!(data->Flags & FLG_IgnoreLast))
            set(data->CMObj,MUIA_CManager_Changed,TRUE);
    }

    if (WWW && useOURL)
    {
        URL_OpenA(url,NULL);

        return TRUE;
    }

    strcat(Command,url);
    strcat(Command,"\" \"");
    strcat(Command,WWW ? data->Prefs->WWWPath : data->Prefs->FTPPath);

    return FALSE;
}

static ULONG
mStartScript(struct IClass *cl,Object *obj,struct MUIP_EntryList_StartScript *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMEntry       *entry;
    register ULONG       useOURL = (data->Prefs->Options & OPT_UseOpenURL) && OpenURLBase;

    DoSuperMethod(cl,obj,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
    if (entry)
    {
        UBYTE          Command[1024], *MyPort;
        struct RexxMsg *rxmsg;
        ULONG          script;
        STRPTR         scriptFile = NULL; //gcc

        *Command = 0;
        stccpy(Command,(STRPTR)xget(data->CMObj,MUIA_CManager_Path),sizeof(Command));

        switch (script = msg->Script)
        {
            case ACT_CALLHOME: case ACT_CALLMOBILE:
            case ACT_CALLBUSINESS: case ACT_CALLBUSINESSMOBILE:
                script = ACT_CALLHOME;
                scriptFile = data->Prefs->DialProg;
                break;

            case ACT_EMAILHOME: case ACT_EMAILBUSINESS:
                script = ACT_EMAILHOME;
                scriptFile = data->Prefs->MailScript;
                break;

            case ACT_FAXHOME: case ACT_FAXBUSINESS:
                script = ACT_FAXHOME;
                scriptFile = data->Prefs->FaxScript;
                break;

            case ACT_BROWSER:
                scriptFile = data->Prefs->WWWScript;
                break;

            case ACT_FTP:
                scriptFile = data->Prefs->FTPScript;
                break;

            case ACT_JOIN:
                scriptFile = data->Prefs->ChatScript;
                break;

            case ACT_TELNET:
                scriptFile = data->Prefs->TelnetScript;
                break;

            default:
                break;
        }

        if (script!=ACT_CALLHOME)
        {
            register BPTR lock;

            if (!*scriptFile) return 0;
            AddPart(Command,"Rexx",sizeof(Command));
            AddPart(Command,scriptFile,sizeof(Command));

            lock = Lock(Command,ACCESS_READ);
            if (!lock)
            {
                lock = Lock(scriptFile,ACCESS_READ);
                if (lock) strcpy(Command,scriptFile);
            }

            UnLock(lock);

            strcat(Command," \"");
        }

        switch (entry->Type)
        {
            case CMEntry_User:
                switch (script)
                {
                    case ACT_BROWSER:
                        if (urlFun(data,obj,TRUE,CMUSER(entry)->WWW,Command,useOURL,NULL)) return 0;
                        break;

                    case ACT_FTP:
                        if (!*CMUSER(entry)->FTP) return 0;
                        strcat(Command,CMUSER(entry)->FTP);
                        strcat(Command,"\" \"");
                        strcat(Command,data->Prefs->FTPPath);
                        break;

                    case ACT_EMAILHOME:
                        if (emailFun(data,(msg->Script==ACT_EMAILHOME) ? CMUSER(entry)->EMail : CMUSER(entry)->BEMail,Command,useOURL)) return 0;
                        break;

                    case ACT_CALLHOME:
                    {
                        STRPTR phone = NULL;

                        switch (msg->Script)
                        {
                            case ACT_CALLHOME:
                                phone = CMUSER(entry)->Phone;
                                break;

                            case ACT_CALLMOBILE:
                                phone = CMUSER(entry)->Mobile;
                                break;

                            case ACT_CALLBUSINESS:
                                phone = CMUSER(entry)->BPhone;
                                break;

                            case ACT_CALLBUSINESSMOBILE:
                                phone = CMUSER(entry)->BMobile;
                                break;
                        }

                        if (!*phone) return 0;

                        if (data->Prefs->DialOptions & DIALOPT_ExternalDialer)
                        {
                            if (!*data->Prefs->DialProg) return 0;
                            sprintf(Command,data->Prefs->DialProg,phone);

                            SystemTags(Command,SYS_Asynch, TRUE,
                                               SYS_Input,  Open("NIL:", MODE_NEWFILE),
                                               SYS_Output, Open("NIL:", MODE_NEWFILE),
                                               TAG_END);

                            return 0;
                        }
                        else
                        {
                            Object *wnd;

                            DoMethod(data->CMObj,MUIM_CManager_Cleanup,FALSE);

                            ObtainSemaphore(&lib_sem);

                            if (lib_DialWindow || initDialWindow())
                            {
                                ReleaseSemaphore(&lib_sem);

                                if (wnd = DialWindowObject,
                                        MUIA_CManager_AppObj,  _app(obj),
                                        MUIA_CManager_CMObj,   data->CMObj,
                                        MUIA_Window_RefWindow, _win(obj),
                                        End)
                                {
                                    DoMethod(_app(obj),OM_ADDMEMBER,wnd);
                                    DoMethod(wnd,MUIM_DialWindow_Open,phone,_win(obj));
                                }
                            }
                            else ReleaseSemaphore(&lib_sem);

                            return 0;
                        }
                    }
                    break;

                    case ACT_FAXHOME:
                    {
                        register STRPTR fax;

                        fax = (msg->Script==ACT_FAXHOME) ? CMUSER(entry)->Fax : CMUSER(entry)->BFax;
                        if (!*fax) return 0;
                        strcat(Command,fax);
                        strcat(Command,"\" \"");
                        strcat(Command,data->Prefs->FaxPath);
                        break;
                    }
                }
                break;

            case CMEntry_WWW:
                switch (script)
                {
                    case ACT_BROWSER:
                        if (urlFun(data,obj,TRUE,CMWWW(entry)->WWW,Command,useOURL,(struct CMEntry *)entry)) return 0;
                        break;

                    case ACT_EMAILHOME:
                        if (emailFun(data,CMWWW(entry)->EMail,Command,useOURL)) return 0;
                        break;
                }
                break;

            case CMEntry_FTP:
                switch (script)
                {
                    case ACT_FTP:
                        if (urlFun(data,obj,FALSE,CMFTP(entry)->FTP,Command,useOURL,(struct CMEntry *)entry)) return 0;
                        break;

                    case ACT_BROWSER:
                        if (urlFun(data,obj,TRUE,CMFTP(entry)->FTP,Command,useOURL,(struct CMEntry *)entry)) return 0;
                        break;

                    case ACT_EMAILHOME:
                        if (emailFun(data,CMFTP(entry)->EMail,Command,useOURL)) return 0;
                        break;
                }
                break;

            case CMEntry_Chat:
                switch (script)
                {
                    case ACT_JOIN:
                        if (!*CMCHAT(entry)->Server) return 0;
                        strcat(Command,CMCHAT(entry)->Server);
                        strcat(Command,"\" \"");
                        strcat(Command,CMCHAT(entry)->Channel);
                        strcat(Command,"\" \"");
                        strcat(Command,data->Prefs->ChatPath);
                        break;

                    case ACT_BROWSER:
                        if (urlFun(data,obj,TRUE,CMCHAT(entry)->WWW,Command,useOURL,(struct CMEntry *)entry)) return 0;
                        break;

                    case ACT_EMAILHOME:
                        if (emailFun(data,CMCHAT(entry)->EMail,Command,useOURL)) return 0;
                        break;
                }
                break;

            case CMEntry_Telnet:
                switch (script)
                {
                    case ACT_TELNET:
                        if (!*CMTELNET(entry)->Host) return 0;
                        strcat(Command,CMTELNET(entry)->Host);
                        strcat(Command,"\" \"");
                        strcat(Command,data->Prefs->TelnetPath);
                        break;

                    case ACT_EMAILHOME:
                        if (emailFun(data,CMTELNET(entry)->EMail,Command,useOURL)) return 0;
                        break;
                }
                break;
        }

        strcat(Command,"\"");
        get(_app(obj),MUIA_Application_Base,&MyPort);

        if ((data->RexxPortSig>=0) && (rxmsg = CreateRexxMsg(&data->RexxPort,"REXX",MyPort)))
        {
            if (rxmsg->rm_Args[0] = CreateArgstring(Command,strlen(Command)+1))
            {
                register struct MsgPort *dest;

                rxmsg->rm_Action = RXCOMM;

                Forbid();
                if (dest = FindPort("REXX")) PutMsg(dest,(struct Message *)rxmsg);
                Permit();

                if (dest) data->RexxOut++;
                else
                {
                    DeleteArgstring(rxmsg->rm_Args[0]);
                    DeleteRexxMsg(rxmsg);
                }
            }
        }
    }

    return 0;
}

/***********************************************************************/

static ULONG
showEditWindow(Object *AppObj,APTR Entry)
{
    struct List *list;
    Object      *anchor, *wnd;

    get(AppObj,MUIA_Application_WindowList,&list);
    anchor = (Object *)list->lh_Head;

    while (wnd = NextObject(&anchor))
        if ((APTR)xget(wnd,MUIA_EditWindow_Entry)==Entry)
        {
            DoMethod(wnd,MUIM_Window_ToFront);
            set(wnd,MUIA_Window_Activate,TRUE);

            return TRUE;
        }

    return FALSE;
}

/***********************************************************************/

static ULONG
mEdit(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    LONG                 pos;

    sleepApp(obj,TRUE);

    for (pos = MUIV_NList_NextSelected_Start; ;)
    {
        struct CMEntry *entry;

        DoSuperMethod(cl,obj,MUIM_NList_NextSelected,&pos);
        if (pos==MUIV_NList_NextSelected_End) break;

        DoSuperMethod(cl,obj,MUIM_NList_GetEntry,pos,&entry);
        if (!entry) continue; /* !?! */

        if (!showEditWindow(_app(obj),entry))
        {
            register Object *Body = NULL;
            register ULONG  Title = 0, ID = 0; //gcc

            switch (entry->Type)
            {
                case CMEntry_User:
                    ObtainSemaphore(&lib_sem);
                    if ((lib_Popasl || initPopasl()) &&
                        (lib_RootScroll || initRootScroll()) &&
                        (lib_EditUser || initEditUser()))
                    {
                        ReleaseSemaphore(&lib_sem);
                        if (Body = EditUserObject, MUIA_CManager_CMObj, data->CMObj, End)
                        {
                            Title = Msg_Edit_UserWinTitle;
                            ID    = MAKE_ID('c','m','u','s');
                        }
                    }
                    else ReleaseSemaphore(&lib_sem);
                    break;

                case CMEntry_WWW:
                    ObtainSemaphore(&lib_sem);
                    if ((lib_EditWWW || initEditWWW()) &&
                        (lib_RootScroll || initRootScroll()))
                    {
                        ReleaseSemaphore(&lib_sem);
                        if (Body = EditWWWObject, MUIA_CManager_CMObj, data->CMObj, End)
                        {
                            Title = Msg_Edit_WWWWinTitle;
                            ID    = MAKE_ID('c','m','w','w');
                        }
                    }
                    else ReleaseSemaphore(&lib_sem);
                    break;

                case CMEntry_FTP:
                    ObtainSemaphore(&lib_sem);
                    if ((lib_Popasl || initPopasl()) &&
                        (lib_RootScroll || initRootScroll()) &&
                        (lib_EditFTP || initEditFTP()))
                    {
                        ReleaseSemaphore(&lib_sem);
                        if (Body = EditFTPObject, MUIA_CManager_CMObj, data->CMObj, End)
                        {
                            Title = Msg_Edit_FTPWinTitle;
                            ID    = MAKE_ID('c','m','f','t');
                        }
                    }
                    else ReleaseSemaphore(&lib_sem);
                    break;

                case CMEntry_Chat:
                    ObtainSemaphore(&lib_sem);
                    if ((lib_EditIRC || initEditIRC()) &&
                        (lib_RootScroll || initRootScroll()))
                    {
                        ReleaseSemaphore(&lib_sem);
                        if (Body = EditIRCObject, MUIA_CManager_CMObj, data->CMObj, End)
                        {
                            Title = Msg_Edit_ChatWinTitle;
                            ID    = MAKE_ID('c','m','c','h');
                        }
                    }
                    else ReleaseSemaphore(&lib_sem);
                    break;

                case CMEntry_Telnet:
                    ObtainSemaphore(&lib_sem);
                    if ((lib_EditTelnet || initEditTelnet()) &&
                        (lib_RootScroll || initRootScroll()))
                    {
                        ReleaseSemaphore(&lib_sem);
                        if (Body = EditTelnetObject, MUIA_CManager_CMObj, data->CMObj, End)
                        {
                            Title = Msg_Edit_TelnetWinTitle;
                            ID    = MAKE_ID('c','m','t','n');
                        }
                    }
                    else ReleaseSemaphore(&lib_sem);
                    break;
            }

            if (Body)
            {
                ObtainSemaphore(&lib_sem);

                if (lib_EditWindow || initEditWindow())
                {
                    register Object *wnd;

                    ReleaseSemaphore(&lib_sem);

                    if (wnd = EditWindowObject,
                            MUIA_CManager_CMObj,    data->CMObj,
                            MUIA_EditWindow_Title,  Title,
                            MUIA_EditWindow_ID,     ID,
                            MUIA_EditWindow_Object, Body,
                            MUIA_EditWindow_List,   obj,
                            MUIA_Window_RefWindow,  _win(obj),
                        End)
                    {
                        register Object *o;

                        DoMethod(_app(obj),OM_ADDMEMBER,wnd);
                        DoMethod(wnd,MUIM_EditWindow_Open,entry);

                        if (o = (Object *)xget(Body,MUIA_CManager_ActiveEditObject))
                            set(wnd,MUIA_Window_ActiveObject,o);
                    }
                }
                else ReleaseSemaphore(&lib_sem);
            }
        }
    }

    sleepApp(obj,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mHandleARexx(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    register struct RexxMsg *RxMsg;

    while (RxMsg = (struct RexxMsg *)GetMsg(&data->RexxPort))
    {
        DeleteArgstring(RxMsg->rm_Args[0]);
        DeleteRexxMsg(RxMsg);

        data->RexxOut--;
    }

    return 0;
}

/***********************************************************************/

static ULONG
mCopyToClip(struct IClass *cl,Object *obj,Msg msg)
{
    struct CMEntry         *entry;
    register ULONG         res = FALSE;

    DoSuperMethod(cl,obj,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
    if (entry)
    {
        REGARRAY UBYTE buf[1024];

        switch (entry->Type)
        {
            case CMEntry_User:
                strcpy(buf,*CMUSER(entry)->EMail ? CMUSER(entry)->EMail : CMUSER(entry)->BEMail);
                break;

            case CMEntry_WWW:
                strcpy(buf,CMWWW(entry)->WWW);
                break;

            case CMEntry_FTP:
                strcpy(buf,CMFTP(entry)->FTP);
                break;

            case CMEntry_Chat:
                strcpy(buf,CMCHAT(entry)->Channel);
                break;

            case CMEntry_Telnet:
                strcpy(buf,CMTELNET(entry)->Host);
                break;
        }

        if (*buf)
        {
            register struct IFFHandle *iff;

            if (iff = AllocIFF())
            {
                if (iff->iff_Stream = (ULONG)OpenClipboard(0))
                {
                    InitIFFasClip(iff);

                    if (!(OpenIFF(iff,IFFF_WRITE)))
                    {
                        register ULONG len = strlen(buf);

                        res = !PushChunk(iff,ID_FTXT,ID_FORM,IFFSIZE_UNKNOWN) &&
                              !PushChunk(iff,ID_FTXT,ID_CHRS,len) &&
                              (WriteChunkBytes(iff,buf,len)==len) &&
                              !PopChunk(iff);

                        CloseIFF(iff);
                    }

                    CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);
                }

                FreeIFF(iff);
            }
        }
    }

    if (!res) DisplayBeep(0);

    return 0;
}

/***********************************************************************/

static ULONG
mLocate(struct IClass *cl,Object *obj,struct MUIP_EntryList_Locate *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register STRPTR      c = msg->contents;

    if (*c)
    {
        struct CMEntry  *e;
        register ULONG  res = 1;
        LONG            col;
        register int    i, len;

        superget(cl,obj,MUIA_NList_TitleMark,&col);

        for (len = strlen(c), i = 0; ; i++)
        {
            DoSuperMethod(cl,obj,MUIM_NList_GetEntry,i,&e);
            if (!e) break;

            switch (e->Type)
            {
                case CMEntry_User:
                {
                    register APTR a;

                    switch (data->ListPrefs.UserCols[col])
                    {
                        case USERCOL_Name:        a = CMUSER(e)->VisualName; break;
                        case USERCOL_EMail:       a = CMUSER(e)->EMail;      break;
                        case USERCOL_Comment:     a = CMUSER(e)->Comment;    break;
                        case USERCOL_Alias:       a = CMUSER(e)->Alias;      break;
                        case USERCOL_Country:     a = CMUSER(e)->Country;    break;
                        case USERCOL_City:        a = CMUSER(e)->City;       break;
                        case USERCOL_Phone:       a = CMUSER(e)->Phone;      break;
                        case USERCOL_Mobile:      a = CMUSER(e)->Mobile;     break;
                        case USERCOL_Fax:         a = CMUSER(e)->Fax;        break;
                        case USERCOL_WWW:         a = CMUSER(e)->WWW;        break;
                        case USERCOL_Company:     a = CMUSER(e)->Company;    break;
                        case USERCOL_BPhone:      a = CMUSER(e)->BPhone;     break;
                        case USERCOL_BMobile:     a = CMUSER(e)->BMobile;    break;
                        case USERCOL_BFax:        a = CMUSER(e)->BFax;       break;
                        default:                  a = CMUSER(e)->VisualName; break;
                    }

                    if (a) res = StrnCmp(lib_locale,a,c,len,SC_COLLATE1);

                    break;
                }

                case CMEntry_WWW:
                {
                    register APTR a;

                    switch (data->ListPrefs.WWWCols[col])
                    {
                        case WWWCOL_Name:    a = CMWWW(e)->Name;    break;
                        case WWWCOL_Site:    a = CMWWW(e)->WWW;     break;
                        case WWWCOL_Comment: a = CMWWW(e)->Comment; break;
                        case WWWCOL_Alias:   a = CMWWW(e)->Alias;   break;
                        case WWWCOL_EMail:   a = CMWWW(e)->EMail;   break;
                        default:             a = CMWWW(e)->Name;    break;
                    }

                    if (a) res = StrnCmp(lib_locale,a,c,len,SC_COLLATE1);

                    break;
                }

                case CMEntry_FTP:
                {
                    register APTR a;

                    switch (data->ListPrefs.FTPCols[col])
                    {
                        case FTPCOL_Name:    a = CMFTP(e)->Name;    break;
                        case FTPCOL_Site:    a = CMFTP(e)->FTP;     break;
                        case FTPCOL_Comment: a = CMFTP(e)->Comment; break;
                        case FTPCOL_Alias:   a = CMFTP(e)->Alias;   break;
                        case FTPCOL_EMail:   a = CMFTP(e)->EMail;   break;
                        default:             a = CMFTP(e)->Name;    break;
                    }

                    if (a) res = StrnCmp(lib_locale,a,c,len,SC_COLLATE1);

                    break;
                }

                case CMEntry_Chat:
                {
                    register APTR a = NULL;

                    switch (data->ListPrefs.ChatCols[col])
                    {
                        case CHATCOL_Channel: a = CMCHAT(e)->Channel; break;
                        case CHATCOL_Server:  a = CMCHAT(e)->Server;  break;
                        case CHATCOL_Comment: a = CMCHAT(e)->Comment; break;
                        case CHATCOL_EMail:   a = CMCHAT(e)->EMail;   break;
                    }

                    if (a) res = StrnCmp(lib_locale,a,c,len,SC_COLLATE1);

                    break;
                }

                case CMEntry_Telnet:
                {
                    register APTR a = NULL;

                    switch (data->ListPrefs.TelnetCols[col])
                    {
                        case TELNETCOL_Host:    a = CMTELNET(e)->Host;    break;
                        case TELNETCOL_Comment: a = CMTELNET(e)->Comment; break;
                        case TELNETCOL_Alias:   a = CMTELNET(e)->Alias;   break;
                    }

                    if (a) res = StrnCmp(lib_locale,a,c,len,SC_COLLATE1);

                    break;
                }
            }

            if (!res)
            {
                set(obj,MUIA_NList_Active,i);
                break;
            }
        }
    }

    return 0;
}

/***************************************************************************/

static ULONG
mSort(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       ManualSort = data->Prefs->Options & OPT_ManualSort;

    if (ManualSort) data->Flags &= ~FLG_ManualSort;
    DoSuperMethod(cl,obj,MUIM_NList_Sort);
    if (ManualSort) data->Flags |= FLG_ManualSort;

    return 0;
}

/***************************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:                      return mNew(cl,obj,(APTR)msg);
        case OM_SET:                      return mSets(cl,obj,(APTR)msg);
        case OM_GET:                      return mGet(cl,obj,(APTR)msg);
        case OM_DISPOSE:                  return mDispose(cl,obj,(APTR)msg);

        case MUIM_Setup:                  return mSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup:                return mCleanup(cl,obj,(APTR)msg);
        case MUIM_Show:                   return mShow(cl,obj,(APTR)msg);
        case MUIM_Hide:                   return mHide(cl,obj,(APTR)msg);
        case MUIM_HandleEvent:            return mHandleEvent(cl,obj,(APTR)msg);
        case MUIM_ContextMenuChoice:      return mContextMenuChoice(cl,obj,(APTR)msg);

        case MUIM_NList_ContextMenuBuild: return mContextMenuBuild(cl,obj,(APTR)msg);
        case MUIM_NList_Compare:          return mCompare(cl,obj,(APTR)msg);
        case MUIM_NList_Select:           return mSelect(cl,obj,(APTR)msg);

        case MUIM_EntryList_Remove:       return mRemove(cl,obj,(APTR)msg);
        case MUIM_EntryList_Clone:        return mClone(cl,obj,(APTR)msg);
        case MUIM_EntryList_DoubleClick:  return mDoubleClick(cl,obj,(APTR)msg);
        case MUIM_EntryList_Edit:         return mEdit(cl,obj,(APTR)msg);
        case MUIM_EntryList_CopyToClip:   return mCopyToClip(cl,obj,(APTR)msg);
        case MUIM_EntryList_StartScript:  return mStartScript(cl,obj,(APTR)msg);
        case MUIM_EntryList_HandleARexx:  return mHandleARexx(cl,obj,(APTR)msg);
        case MUIM_EntryList_Locate:       return mLocate(cl,obj,(APTR)msg);
        case MUIM_EntryList_Sort:         return mSort(cl,obj,(APTR)msg);

        default:                          return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initEntryList(void)
{
    if (lib_EntryList = MUI_CreateCustomClass(NULL,MUIC_NList,NULL,sizeof(struct data),DISP(dispatcher)))
    {
    	localizeMenus(UserMenu,UserMenuIDs);
        localizeMenus(UserTitleMenu,UserTitleMenuIDs);

        localizeMenus(WWWMenu,WWWMenuIDs);
        localizeMenus(WWWTitleMenu,WWWTitleMenuIDs);

        localizeMenus(FTPMenu,FTPMenuIDs);
        localizeMenus(FTPTitleMenu,FTPTitleMenuIDs);

        localizeMenus(ChatMenu,ChatMenuIDs);
        localizeMenus(ChatTitleMenu,ChatTitleMenuIDs);

        localizeMenus(TelnetMenu,TelnetMenuIDs);
        localizeMenus(TelnetTitleMenu,TelnetTitleMenuIDs);

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
disposeEntryList(void)
{
    MUI_DeleteCustomClass(lib_EntryList);
    lib_EntryList = NULL;
}

/***********************************************************************/
