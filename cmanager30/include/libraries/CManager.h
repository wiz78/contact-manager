#ifndef LIBRARIES_CMANAGER_H
#define LIBRARIES_CMANAGER_H

#ifndef EXEC_PORTS_H
#include <exec/ports.h>
#endif

#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

/*
**  $VER: CManager.h 30 (25.10.2004)
**  Includes Release 30
**
**  CManager.h
**  Global bookmarks manager shared library
**
**  Written by Simone Tellini and Alfonso Ranieri <alforan@tin.it>.
**
**  Released under the terms of the
**  GNU Public Licence version 2
*/

#if defined(__GNUC__)
# pragma pack(2)
#endif

/***********************************************************************/
/*
** Library
*/

#define CMANAGERNAME "CManager.library"
#define CMANAGERVER  30

/***********************************************************************/
/*
** CM entry
*/

struct CMEntry
{
    struct CMEntry *Succ;
    struct CMEntry *Prec;
    UBYTE          Type;
    UBYTE          Flags;
};

/* Type */
enum
{
    CMEntry_Group   = 0,
    CMEntry_User    = 1,
    CMEntry_WWW     = 2,
    CMEntry_FTP     = 3,
    CMEntry_Chat    = 4,
    CMEntry_Telnet  = 5,

    CMEntry_List    = 253,
    CMEntry_Section = 255,
};

/* Flags -  FLAGS BIT 7 IS RESERVED !!! */
enum
{
    CMENTRYFLG_New = 1<<7,
};

/* Some macros to cast pointers */
#define CMENTRY(e)  ((struct CMEntry *)(e))
#define CMGROUP(e)  ((struct CMGroup *)(e))
#define CMUSER(e)   ((struct CMUser *)(e))
#define CMWWW(e)    ((struct CMWWW *)(e))
#define CMFTP(e)    ((struct CMFTP *)(e))
#define CMCHAT(e)   ((struct CMChat *)(e))
#define CMTELNET(e) ((struct CMTelnet *)(e))

/***********************************************************************/
/*
** CM Group
*/

struct CMGroup
{
    struct CMGroup *Succ;
    struct CMGroup *Prec;
    UBYTE          Type;
    UBYTE          Flags;

    UBYTE          Name[80];
    struct MinList Entries;
    struct MinList SubGroups;
};

/* Flags */
enum
{
    CMGROUPFLG_Bold = 1<<0,
    CMGROUPFLG_Open = 1<<1,
};

/***********************************************************************/
/*
** CM User
*/

struct CMEMail
{
    struct CMEMail *Succ;
    struct CMEMail *Prec;
    UBYTE          EMail[128];
    UBYTE          Flags;
};

#define CMEMAIL(e) ((struct CMEMail *)(e))

struct CMUser
{
    struct CMUser  *Succ;
    struct CMUser  *Prec;
    UBYTE          Type;
    UBYTE          Flags;

    UBYTE          Name[80];
    UBYTE          Address[128];
    UBYTE          City[128];
    UBYTE          Country[80];
    UBYTE          ZIP[20];
    UBYTE          Comment[512];
    UBYTE          Alias[40];
    UBYTE          Phone[40];
    UBYTE          Fax[40];
    UBYTE          EMail[128];
    UBYTE          WWW[256];
    UBYTE          FTP[256];
    UBYTE          LastName[80];
    UBYTE          Mobile[40];
    UBYTE          ICQ[20];
    UBYTE          Company[40];
    UBYTE          BAddress[128];
    UBYTE          BCity[128];
    UBYTE          BCountry[80];
    UBYTE          BZIP[20];
    UBYTE          BPhone[40];
    UBYTE          BFax[40];
    UBYTE          BEMail[128];
    UBYTE          BJobTitle[20];
    UBYTE          BDepartment[40];
    UBYTE          BOffice[40];
    ULONG          Reserved1;
    UBYTE          PGPUserID[80];
    UBYTE          BMobile[40];
    UBYTE          ImageFile[256];
    UBYTE          VisualName[64];
    UBYTE          Sex;
    ULONG          Birthday;
    ULONG          Anniversary;
    struct MinList EMails;
};

/* Flags */
enum
{
    CMUSERFLG_Transparent = 1<<0,
    CMUSERFLG_Birthday    = 1<<1,
    CMUSERFLG_Anniversary = 1<<2,
};

/* Sex */
enum
{
    CMUSERSEX_None,
    CMUSERSEX_Male,
    CMUSERSEX_Female,
};

/***********************************************************************/
/*
** CM WWW
*/

struct CMWWW
{
    struct CMWWW       *Succ;
    struct CMWWW       *Prec;
    UBYTE              Type;
    UBYTE              Flags;

    UBYTE              Name[80];
    UBYTE              WWW[256];
    UBYTE              Comment[512];
    UBYTE              WebMaster[80];
    UBYTE              EMail[128];
    struct DateStamp   LastModified;
    struct DateStamp   LastVisited;
    UBYTE              Alias[40];
};

/***********************************************************************/
/*
** CM FTP
*/

struct CMFTP
{
    struct CMFTP       *Succ;
    struct CMFTP       *Prec;
    UBYTE              Type;
    UBYTE              Flags;

    UBYTE              Name[80];
    UBYTE              FTP[256];
    UBYTE              Comment[512];
    UBYTE              Username[60];
    UBYTE              Password[60];
    ULONG              Port;
    ULONG              Retries;
    UBYTE              Local[256];
    struct DateStamp   LastModified;
    struct DateStamp   LastVisited;
    UBYTE              Alias[40];
    ULONG              ExtraFlags;
    UWORD              WindowStyle;
    UBYTE              EMail[128];
};

/* Flags */
enum
{
    CMFTPFLG_Advanced  = 1<<0,
    CMFTPFLG_Quiet     = 1<<1,
    CMFTPFLG_Compress  = 1<<2,
    CMFTPFLG_ADT       = 1<<3,
    CMFTPFLG_Anonymous = 1<<4,
    CMFTPFLG_Local     = 1<<5,
    CMFTPFLG_Proxy     = 1<<6,
};

/* ExtraFlags */
enum
{
    CMFTPEFLG_ShowFTPOutput = 1<<0,
    CMFTPEFLG_Noops         = 1<<1,
    CMFTPEFLG_ShowHidden    = 1<<2,
};

/* WindowStyle */
enum
{
    CMFTPWSTYLE_Dual,
    CMFTPWSTYLE_Single,
};

/***********************************************************************/
/*
** CM Chat
*/

struct CMChat
{
    struct CMChat *Succ;
    struct CMChat *Prec;
    UBYTE         Type;
    UBYTE         Flags;

    UBYTE         Channel[128];
    UBYTE         Server[128];
    UBYTE         Maintainer[80];
    UBYTE         Nick[20];
    UBYTE         WWW[256];
    UBYTE         Comment[512];
    UBYTE         Password[60];
    UBYTE         EMail[128];
};

/***********************************************************************/
/*
** CM Telnet
*/

struct CMTelnet
{
    struct CMTelnet *Succ;
    struct CMTelnet *Prec;
    UBYTE           Type;
    UBYTE           Flags;

    UBYTE           Host[80];
    ULONG           Port;
    UBYTE           Login[80];
    UBYTE           Password[80];
    UBYTE           Comment[256];
    UBYTE           EMail[128];
    UBYTE           Alias[40];
};

/* Flags */
enum
{
    CMTELNETFLG_SSH = 1<<0,
};


/***********************************************************************/
/*
** Struct used by CM_LoadDataA(), CM_SaveDataA(), CM_FreeData()
*/

struct CMData
{
    struct CMGroup *Users;
    struct CMGroup *WWWs;
    struct CMGroup *FTPs;
    struct CMGroup *Chat;
    UBYTE          CurrentUser[60];
    UBYTE          Path[256];
    ULONG          FreeMe;             /* if TRUE, CM_FreeData() frees the structure as well */
    UWORD          Version;            /* version of this structure, you MUST fill it */
    ULONG          SizeOf;
    struct CMGroup *Telnet;
    ULONG          Flags;
};

enum
{
    CMDFLG_UTF8  = 1<<0,
    CMDFLG_Pass  = 1<<1,
};

/* Version */
#define CMD_CURRENT_VERSION 18
#define CMD_UTF8_VERSION    17
#define CMD_MINVER          9

/***********************************************************************/
/*
** Flags for CM_GetEntry()
*/

enum
{
    CMGETENTRY_User   = 1<<0,
    CMGETENTRY_WWW    = 1<<1,
    CMGETENTRY_FTP    = 1<<2,
    CMGETENTRY_Chat   = 1<<3,
    CMGETENTRY_Multi  = 1<<4,
    CMGETENTRY_Telnet = 1<<5,
};

/***********************************************************************/
/*
** Values for CM_GetOption()
*/

enum
{
    CMGETOPTION_AvoidGenesis,
    CMGETOPTION_Version,
};

/***********************************************************************/
/*
** Interface to CManager application
*/

struct CMMessage
{
    struct Message Msg;
    ULONG          Command;
    ULONG          Data;
};

enum
{
    CMCOMMAND_Quit,
    CMCOMMAND_GetEntry,
    CMCOMMAND_Port,
    CMCOMMAND_ARexx,
    CMCOMMAND_Fail,
};

/***********************************************************************/
/*
** CM_StartCManagerA() tags
**
** These are Simone Tellini's tags:
** Don't use them in other projects!
*/

#define LTAGBASE  ((int)0xF76B0010)
#define CMLTAG(n) (LTAGBASE+(int)(n))

#define CMSTARTA_File            CMLTAG(200)
#define CMSTARTA_PubScreen       CMLTAG(201)
#define CMSTARTA_HideUsers       CMLTAG(202)
#define CMSTARTA_HideWWW         CMLTAG(203)
#define CMSTARTA_HideFTP         CMLTAG(204)
#define CMSTARTA_HideChat        CMLTAG(205)
#define CMSTARTA_HideTelnet      CMLTAG(206)

#define CMA_LoadData_CMData      CMLTAG(220)
#define CMA_LoadData_File        CMLTAG(221)
#define CMA_LoadData_User        CMLTAG(222)
#define CMA_LoadData_Current     CMLTAG(223)
#define CMA_LoadData_Ask         CMLTAG(224)
#define CMA_LoadData_Codeset     CMLTAG(225)
#define CMA_LoadData_Pass        CMLTAG(226)

#define CMA_SaveData_CMData      CMLTAG(230)
#define CMA_SaveData_File        CMLTAG(231)
#define CMA_SaveData_User        CMLTAG(232)
#define CMA_SaveData_Codeset     CMLTAG(233)
#define CMA_SaveData_Pass        CMLTAG(234)

/***********************************************************************/
/*
** CM_AllocObject() types
*/

enum
{
    CMALLOCOBJECT_EMail,
    CMALLOCOBJECT_List,
};

/***********************************************************************/

#if defined(__GNUC__)
# pragma pack()
#endif

#endif /* LIBRARIES_CMANAGER_H */
