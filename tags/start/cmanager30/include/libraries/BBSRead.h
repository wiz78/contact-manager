#ifndef LIBRARIES_BBSREAD_H
#define LIBRARIES_BBSREAD_H
/*
**  $VER: BBSRead.h 6.1 (07.02.98)
** 
**  BBSRead structures and definitions.
**
**  © Copyright 1995-1998 Ultima Thule Software
**  All Rights Reserved.
**
**  Authors: Eivind Nordseth and Petter Nilsen
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif
#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif
#ifndef LIBRARIES_BBSREADTAGS_H
#include <libraries/BBSReadTags.h>
#endif

#if defined(__GNUC__)
# pragma pack(2)
#endif

#define BBSREADNAME		"bbsread.library"
#define BBSREADVERSION	6L

#define ENV_BBSDATAPATH		"THOR/BBSDataPath"
#define ENV_THORPATH			"THOR/THORPath"


/***********************************************************************
*
* Global config structures and definitions
*
*/

struct ArcConfigItem
{
	struct Node ac_Node;
	UWORD	sc_pad;
	STRPTR ac_Name;			/* Name of this archiver				*/
	STRPTR ac_FindPatern;	/* Patern to identify this archiver	*/
	STRPTR ac_UnArcCmd;		/* Command to unarc an archive		*/
	STRPTR ac_ArcCmd;		/* Command to add to an archive					*/
	ULONG  ac_OpenCnt;		/* Nr of types using this archiver. Private.	*/
};

struct GlobalConfig
{
	STRPTR gc_DnloadPath;		/* Path where downloaded files resides		*/
	STRPTR gc_UploadPath;		/* Path where files to upload resides		*/
	STRPTR gc_GlobalUserName;	/* Global user name.	(from key file)	*/

	ULONG gc_Buffers;			/*	Number of buffers to use				*/
	ULONG gc_BufferSize;		/* Size of each buffer						*/

	STRPTR gc_Signature;		/* Global signature, could be a NULL pointer	*/

	ULONG gc_KeepMsg;			/* Messages to keep in each conference						*/
	ULONG gc_KeepTime;			/* Time (in seconds) to keep messages in conferences	*/

	LONGBITS gc_Flags;	

	struct MinList *gc_AvailArc;	/* List of archivers	which can be used for (un)packing.	*/

	STRPTR gc_XpkMethod;			/* What xpk method to use when packing						*/

	STRPTR gc_UserStreet;			/* User data from key file.		*/
	STRPTR gc_UserAddress;
	STRPTR gc_UserCountry;
	STRPTR gc_UserPhone;			/* User editable.								*/

	STRPTR gc_TmpDir;				/* Directory to use for temp files while packing	*/
	STRPTR gc_TagFile;				/* Global tag file. NULL-pointer, no tags				*/
	
	ULONG gc_HazeLevel1;			/* Seconds to keep messages marked with hazelevel 1	*/
	ULONG gc_HazeLevel2;			/* Seconds to keep messages marked with hazelevel 2	*/
	ULONG gc_HazeLevel3;			/* Seconds to keep messages marked with hazelevel 3	*/
	
	STRPTR gc_PGPCommand;			/* Command for pgp (Default is "(ENV:PGPPATH)pgp")					*/
	STRPTR gc_PGPSignID;    		/* Id to use when PGP signing messages. Default is '*'.			*/ 

	STRPTR gc_ReplyString;			/* Reply string to use when a message is replied _and_ moved.	*/

	STRPTR gc_StartupScript;		/* Arexx script to be run each time Thor is started.	*/
	STRPTR gc_QuitScript;			/* Arexx script to be run each time Thor is quited.	*/

	STRPTR gc_BBSEnterScript;		/* Arexx script to be run each time a bbs is entered.		*/
	STRPTR gc_BBSLeaveScript;		/* Arexx script to be run each time a bbs is left.			*/

	STRPTR gc_ConfEnterScript;		/* Arexx script to be run each time a conf is entered.	*/
	STRPTR gc_ConfLeaveScript;		/* Arexx script to be run each time a conf is left.		*/
	UBYTE  gc_QuoteChars[4];		/* String to use as quote chars in custom quote type.	*/
									/* Max length is 3. The string is NUL terminated. 		*/
};

/*  GlobalConfig.gc_Flags  */
#define GCF_IGNORE_KEEPMSG		(1<<0)	/* gc_KeepMsg is ignored. Messages won't 	*/
										/*	be counted when packing conferences		*/
#define GCF_IGNORE_KEEPTIME		(1<<1)	/* gc_KeepTime is ignored.	Time won't be	*/
										/* checked when packing conferences				*/
#define GCF_RESET_EVENT_NR		(1<<2)	/* Reset event numbers when packing events.	*/
#define GCF_NO_XPK_METHOD		(1<<3)	/* Don't use any xpk method						*/
#define GCF_FILE_SIGNATURE		(1<<4)	/* The gc_Signature field contains the path	*/
										/* and name of a file containing the signature.												*/
#define GCF_AUTO_XPK_PACK		(1<<5)	/* Automatically xpk pack all new messages 	*/
										/* when are written to the database. NB: A xpk method must have been defined.		*/
#define GCF_FORCE_PACK_REWRITE	(1<<6)	/* Force a rewrite when packing conferences	*/
#define GCF_COMMAND_SIGNATURE	(1<<7)	/* The gc_Signature field contains the path	*/
										/* and name of a command. When the command is executed should the signature 		*/
										/* contents be written to stdout. The template for the command is expected to be */	
										/* BBSNAME/K,CONFNAME/K. 																			*/
#define GCF_ALWAYS_REPLY_STR	(1<<8) 	/* Use reply string on all replies. 			*/


/***********************************************************************
*
* BBSType structures and definitions
*
*/

struct EventConfigItem
{
	struct MinNode ec_Node;
	
	ULONG	ec_Event;		/*	Identification of this event									*/
	ULONG *ec_NeedTags;		/* Pointer to TAG_END-terminated array of tags needed		*/
							/* for this event	(NULL pointer -> no needed tags)			*/
	ULONG *ec_OptTags;		/* Pointer to TAG_END-terminated array of optional	tags	*/
							/* for this event	(NULL pointer -> no optional tags)		*/
	ULONG	ec_Reserved;	/* Reserved for future expansion									*/
};


/*  Structure holding BBS-type spesific data  */

struct TypeData
{
	STRPTR td_TypeName;			/* Name of BBS-type									*/
	STRPTR td_MsgParser;		/* Command for parsing grabs						*/
	UWORD  td_LineLength;		/* Max length of lines in messages				*/
	UWORD  td_SubjectLength;	/* Max length of subjects							*/
	UWORD  td_FileDescrLen;		/* Max length of short filedescription			*/
	UBYTE  td_CharSet;			/* Charset the grab from this BBS typr uses.	*/
								/* Se bellow for definitions						*/	
	UBYTE    td_QuoteType;		/* Prefered quote type for this bbstype. 		*/
								/* See below for definitions.						*/
	LONGBITS td_AvailScrFlags;	/* Mask of flags script uses						*/
	UWORD    td_FileNameLen;	/* Max length of filenames allowed				*/
	UWORD    td_pad2;
	STRPTR   td_EventPacker;	/* Command for packing events						*/
	STRPTR	td_EventArchiver;	/* Archiver to use when making packages		*/

	struct MinList *td_AvailEvents;	/* List of struct EventConfigItem		*/
	
	LONGBITS td_Flags;
	STRPTR   td_AcceptPattern;	/* Pattern for accepting grabs for this bbstype */
										/* NULL equals #?											*/
	STRPTR   td_InitMsgFile;	/* Command for initializing message files			*/
	STRPTR   td_ExtConfig;		/* Command for external configuration						*/
};

/* Charsets defined */
#define BRCS_ANY		0	/* Any charset, can't be used in BBSTypes.					*/
#define BRCS_ISO		1	/* ISO Latin 1 charset.												*/
#define BRCS_IBN		2	/* IBM charset used in Norway. Same as IBM CodePage 865. */
#define BRCS_SF7		3	/* 7-bit charset. Sweden and Finland.							*/
#define BRCS_NO7		4	/* 7-bit charset. Norway.											*/
#define BRCS_DE7		5	/* 7-bit charset. Denmark.				*/
#define BRCS_US7		6	/* 7-bit charset. USA.					*/
#define BRCS_UK7		7	/* 7-bit charset. United Kingdom.	*/
#define BRCS_IT7		8	/* 7-bit charset. Italia.				*/
#define BRCS_PO7		9	/* 7-bit charset. Portugal.			*/
#define BRCS_SP7		10	/* 7-bit charset. Espania.				*/
#define BRCS_GE7		11	/* 7-bit charset. Germany.				*/
#define BRCS_FR7		12	/* 7-bit charset. France.											*/
#define BRCS_IBM		13	/* IBM charset. Same as IBM CodePage 437.						*/
#define BRCS_IBMM		14	/* IBM multinational charset. Same as IBM CodePage 850. 	*/
#define BRCS_APPLE		15	/* Apple charset.														*/
#define BRCS_NO_OF		15	/* Number of charset defined.		*/


/* Charset equivalents */
#define BRCS_ISO8859.1		BRCS_ISO

#define BRCS_ISO_IR002		BRCS_US7
#define BRCS_ISO_IR004		BRCS_UK7
#define BRCS_ISO_IR010		BRCS_SF7
#define BRCS_ISO_IR011		BRCS_SF7
#define BRCS_ISO_IR015		BRCS_IT7
#define BRCS_ISO_IR016		BRCS_PO7
#define BRCS_ISO_IR017		BRCS_SP7
#define BRCS_ISO_IR021		BRCS_GE7
#define BRCS_ISO_IR060		BRCS_NO7
#define BRCS_ISO_IR061		BRCS_NO7
#define BRCS_ISO_IR069		BRCS_FR7

#define BRCS_IBM_CP437		BRCS_IBM
#define BRCS_IBM_CP850		BRCS_IBMM
#define BRCS_IBM_CP865		BRCS_IBN


/*  TypeData.td_AvailScrFlags  */
#define SF_NEWFILES		(1<<0)	/* Do newfiles scan every logon				*/
#define SF_AUTOPDNL		(1<<1)	/* Automaticaly download private files		*/
#define SF_AUTOLOGOFF	(1<<2)	/* Automaticaly logoff after all is done	*/
#define SF_USECOLORS	(1<<3)	/* Use colors on BBS								*/
#define SF_ANSIMENUES	(1<<4)	/* Use ANSI menues								*/
#define SF_BULLETINS	(1<<5)	/* Get new bulletins on every logon			*/


/*  TypeData.td_QuoteType, BBSData.bd_QuoteType, ConfData.cd_QuoteType  */
#define QT_USE_SUPER 0 /* 
    In TypeData.td_QuoteType: Same as QT_DEFAULT.
    In BBSData.bd_QuoteType:  Use quotetype defined for the BBSType.      
    In ConfData.cd_QuoteType: Use quotetype defined for the BBS.          */
#define QT_NONE      1 /* No quoting.                                     */
#define QT_DEFAULT   2 /* Default quote char. (">")                       */
#define QT_DEF_SPACE 3 /* Default quote char pluss space. ("> ")          */
#define QT_CUSTOM    4 /* Custom quote chars defined in QuoteChars field. */
#define QT_INITIALS  5 /* Use initials as quote chars.                    */


/* TypeData.td_Flags  */
#define TDF_NO_NAME_CHECK		(1<<0)	/* Don't check BBSData->bd_UserName/			*/
										/* ConfData->cd_Alias when adding messages	*/
#define TDF_NO_ADDR_CHECK		(1<<1)	/* Don't check BBSData->bd_UserAddr when 		*/
										/* adding messages 									*/
#define TDF_SELECT_ADD_USER		(1<<2)	/* BBSType supports selectable user adding.	*/
#define TDF_ADD_USERS			(1<<3)	/* The parser for this eventtype adds users 	*/
										/* to the user database by default. Used by ConfigBBS() when creatig new BBS'es.	*/
#define TDF_ABS_LINELENGTH		(1<<4)	/* The line length in messages from this bbs */
										/* type is never longer than defined for the conf/bbs/bbsttype.						*/
#define TDF_MULTIPLE_CONFS		(1<<5)	/* BBSType supports "conf1,conf2,...,confn"	*/
										/* in BREV_Conference tag. 						*/
#define TDF_MULTIPLE_TO			(1<<6)	/* BBSType supports "to1,to2,...,ton" in 		*/
										/* BREV_ToName and/or BREV_ToAddr. If both BREV_ToName and BREV_ToAddr tags are	*/
										/* used must name x in BREV_ToName correspond to address x in BREV_ToAddr.			*/
#define TDF_REFLOW_QUOTING		(1<<7)	/* Reflow quoting should be on as default 	*/
										/* when answering messages. 						*/
#define TDF_GLOBAL_REPLIES		(1<<8)	/* BRMSG_MsgID or BRMSG_OrginalNr is globaly */
										/* unique and can be used when repling messages to another conference than the 	*/
										/* orginal.	See BREV_RefConference tag.														*/
#define TDF_NAME_REPLIES		(1<<9)	/* It is possible to reply messages to 		*/
										/* another user than the author of the orginal message.									*/
#define TDF_REPLY_SUBJ_PREFIX	(1<<10)	/* Use "Re: " as subject prefix on replies.	*/
#define TDF_EVENTS_HANDELED		(1<<11)	/* The library shouldn't ask if packed 		*/
										/* events should be deleted when parsing. This is handled by the event 				*/
										/* packer/parser.																						*/
#define TDF_SUPPORTS_ENCODE_8BIT_MAIL	(1<<12)	/* Supports encoding of 8-bit 		*/
												/* messages only in mail conferences. 															*/
#define TDF_SUPPORTS_ENCODE_8BIT_MSGS	(1<<13)	/* Supports encoding of 8-bit 		*/
												/* messages in public conferences. 																*/
#define TDF_SUPPORTS_MULTIPLE_ATTACHMENTS	(1<<14) /* Supports multiple attachments */


/*  Structure used in List returned by GetTypeList  */

struct TypeListItem
{
	struct Node tl_Node;			/* Caller is free to use nl_Name		*/
	WORD        tl_pad;
	APTR        tl_Link;			/* Private!!!!!!!!									*/
	APTR        tl_UserData;	/* For use by the caller; no restrictions! 	*/

	struct TypeData *tl_Data;	/* Pointer to the Type data						*/
};


/***********************************************************************
*
* BBS structures and definitions
*
*/

/*  Structure holding user modifiable BBS data  */

struct BBSData
{
	STRPTR bd_Name;			/* Name of BBS																*/
	STRPTR bd_GrabName;		/* Name of grabfile (without extension) or with wildcards	*/
	STRPTR bd_BBSType;		/* Type of BBS, used when splitting grabs and scripting		*/
	STRPTR bd_UserName;		/* The name the user is registered as							*/
	STRPTR bd_Signature;	/* Signature to use on this BBS, could be a NULL-pointer	*/

	UWORD  bd_LineLength;	/* Max linelength in messages. Overrides TypeData if >0				*/
	UBYTE  bd_CharSet;		/* Charset for the grab from this BBS uses. Overrides					*/
							/* TypeData if != BRCS_ANY														*/
	UBYTE  bd_QuoteType;	/* Prefered quite type for this bbs. See above for definitions.	*/

	LONGBITS bd_ScrFlags;	/* Flags for script (for definitions se TypeData)	*/
	LONGBITS bd_Flags;		/* Flags for this BBS										*/

	ULONG  bd_KeepMsg;		/* Messages to keep in each conference						*/
	ULONG  bd_KeepTime;		/* Time (in seconds) to keep messages in conferences	*/

	STRPTR bd_EMailAddr;	/* The address the user has on the bbs						*/
	STRPTR bd_XpkMethod;	/* What xpk method to use when packing this bbs			*/

	STRPTR bd_UserStreet;	/* User data to override GlobalConfig		*/
	STRPTR bd_UserAddress;  /* NULL equals to value in GlobalConfig	*/
	STRPTR bd_UserCountry;
	STRPTR bd_UserPhone;
	STRPTR bd_Alias;		/* Alias used on this BBS. NULL-pointer, no alias	*/
							/* This alias will only be used in conferences 		*/
							/* marked with CDF_ALIAS. 											*/
	STRPTR bd_DnloadPath;	/* Dnloadpath for this BBS. Overrides global settings		*/
	STRPTR bd_TagFile;		/* Tagfile to use on this BBS. NULL-pointer, use global	*/
	STRPTR bd_EventArchiver;/* Archiver to use when packing the events from this BBS	*/
							/* Overrides the setting in the BBSType if non NULL.		*/
	STRPTR bd_ReplyPacket;	/* File name of eventual reply packet. Is expected to be */
							/* relative to the defined upload dir for this bbs.		*/
	STRPTR bd_UploadPath;	/* Upload path for this BBS. Overrides global settings 	*/
	STRPTR bd_ReplyString;	/* Reply string to use when a message is replied _and_ 	*/
							/* moved. NULL means: Use global reply string. 				*/
	STRPTR bd_BBSEnterScript;	/* Arexx script to be run each time this bbs is 		*/
								/* entered. Overrides global setting.						*/
	STRPTR bd_BBSLeaveScript;	/* Arexx script to be run each time this bbs is left.	*/
								/* Overrides global setting.									*/
	STRPTR bd_ConfEnterScript;	/* Arexx script to be run each time a conference on 	*/
								/* this bbs is entered. Overrides global setting.		*/
	STRPTR bd_ConfLeaveScript;	/* Arexx script to be run each time a conference on 	*/
								/* this bbs is left. Overrides global setting.			*/
	UBYTE  bd_QuoteChars[4];	/* String to use as quote chars in custom quote type. */
								/* Max length is 3. The string is NUL terminated.		*/
};

/*  BBSData.bd_Flags  */
#define BDF_GLOBAL_KEEPMSG		(1<<0)	/* Use global KeepMsg for this BBS			*/
#define BDF_GLOBAL_KEEPTIME		(1<<1)	/* Use global KeepTime for this BBS			*/
#define BDF_IGNORE_KEEPMSG		(1<<2)	/* bd_KeepMsg is ignored. Messages won't 	*/
										/* be counted when packing conferences		*/
#define BDF_IGNORE_KEEPTIME		(1<<3)	/* bd_KeepTime is ignored.	Time won't be	*/
										/* checked when packing conferences			*/
#define BDF_NO_XPK_METHOD		(1<<4)	/* Don't use any xpk method on this bbs	*/

/*	This flag is used to show that the events for this bbs has changed since last		*
 *	call to MakeEventPackage(). It is maintained by the library, and should normally	*
 *	not be set or cleard by other. A successfull call to MakeEventPackage() will		*
 *	clear the flag.																						*/
#define BDF_EVENTS_CHANGED		(1<<5)	
#define BDF_NO_SIGNATURE		(1<<6)	/* Don't use signatures on this bbs				*/
#define BDF_MARK_OWN_MSGS		(1<<7)	/* Also mark messages from user when adding 	*/
										/* messages	with the WBRMSG_MarkMessage tag	*/
#define BDF_NO_TAGS				(1<<8)	/* Don't use tags on this bbs						*/
#define BDF_ADD_USERS			(1<<9)	/* The parser for this bbs should add users 	*/
										/* to the database if it supports it. 			*/
#define BDF_FILE_SIGNATURE		(1<<10)	/* The bd_Signature field contains the path	*/
										/* and name of a file containing the signature.												*/
#define BDF_AUTO_XPK_PACK		(1<<11)	/* Automatically xpk pack new messages when	*/
										/* they are written to this bbs. NB: A xpk method must have been defined.			*/
#define BDF_NO_AUTO_XPK_PACK	(1<<12)	/* No automatically xpk packing on this bbs.	*/
										/* Overrides global flags for auto xpk packing.												*/
#define BDF_COMMAND_SIGNATURE	(1<<13)	/* The bd_Signature field contains the path	*/
										/* and name of a command. When the command is executed should the signature 		*/
										/* contents be written to stdout. The template for the command is expected to be */	
										/* BBSNAME/K,CONFNAME/K. 																			*/
#define BDF_REFLOW_QUOTING		(1<<14)	/* Reflow quoting should be on as default 	*/
										/* when answering messages. This flag should only be considered when 				*/
										/* bd_QuoteType is != QT_USE_SUPER.																*/
#define BDF_ALWAYS_REPLY_STR	(1<<15)	/* Use reply string on all replies. This 		*/
										/* flag should only be considered when bd_ReplyString is non NULL.					*/
#define BDF_NO_REPLY_STR		(1<<16)	/* Don't use reply strings on this bbs. 		*/
										/* Overrides the setting of bd_ReplyString.	*/
#define BDF_GLOBAL_REPLY_STR	(1<<17)	/* Use global reply string settings.						*/
										/* Overrides all other bbs reply string settings.														*/
#define BDF_NO_BBS_ENTER_SCR	(1<<18)	/* Don't use a bbs enter script on this bbs.				*/
#define BDF_NO_BBS_LEAVE_SCR	(1<<19)	/* Don't use a bbs leave script on this bbs. 			*/
#define BDF_NO_CONF_ENTER_SCR	(1<<20)	/* Don't use a conference enter script on this bbs.	*/
#define BDF_NO_CONF_LEAVE_SCR	(1<<21)	/* Don't use a conference leave script on this bbs.	*/
#define BDF_ENCODE_8BIT_MAIL	(1<<22) /* Encode 8 bits messages in mail conferences. Only 	*/
										/* appliable when the TDF_SUPPORTS_ENCODE_8BIT_MAIL flag is set for the bbstype.				*/
#define BDF_ENCODE_8BIT_MSGS	(1<<23) /* Encode 8 bits messages in public conferences. Only */
										/* appliable when the TDF_SUPPORTS_ENCODE_8BIT_MSGS flag is set for the bbstype.				*/


/*  Structure holding BBS data maintained by the library */

struct BBSInternal
{
	ULONG bi_FirstEvent;	/* First event for this BBS								*/
	ULONG bi_LastEvent;	/* Last event for this BBS									*/
	ULONG bi_NumEvents;	/* Number of active events (not deleted or done)	*/

	ULONG bi_FirstUser;	/* First user in userfile					*/
	ULONG bi_LastUser;	/* Last user in userfile					*/
	ULONG bi_NumUsers;	/* Number of active users in userfile	*/

	ULONG bi_FirstKill;	/* First kill in killfile					*/
	ULONG bi_LastKill;	/* Number of last kill in killfile 		*/
	ULONG bi_NumKills;	/* Number of active kills in killfile	*/

	ULONG bi_SumMarked;	/* Number of marked messages on this system					*/
						/* This field should be regarded as a BOOL until you 		*/
						/* have obtained a conference list for the bbs.				*/
						/* This field will only be updated during UpdateStruct()	*/
	ULONG bi_SumM2User;	/* Number of marked messages on this system to the user		*/
						/* This field should be regarded as a BOOL until you 		*/
						/* have obtained a conference list for the bbs.				*/
						/* This field will only be updated during UpdateStruct()	*/
	ULONG bi_SumQueued;	/* Number of queued messages on this system. 				*/
						/* This field should be regarded as a BOOL until you 		*/
						/* have obtained a conference list for the bbs.				*/
						/* This field will only be updated during UpdateStruct()	*/
};


/*  Structure used in List returned by GetBBSList  */

struct BBSListItem
{
	struct Node bl_Node;	/* Caller is free to use nl_Name		*/
	WORD bl_pad;
	APTR bl_Link;			/* Private!!!!!!!!									*/
	APTR bl_UserData;		/* For use by the caller; no restrictions! 	*/

	struct BBSData     *bl_Data;		/* Pointer to the BBS data					*/
	struct BBSInternal *bl_Internal;	/* Pointer to public internal data		*/

	STRPTR bl_BBSPath;	/* Path to the data directory for this BBS	*/
								/* Used for textfiles. Ends with a '/'			*/
	ULONG  bl_ChangeCnt;	/* Private!!!!!!!!									*/
};


/***********************************************************************
*
* Conference structures and definitions
*
*/

/*  Structure holding general conference data  */

struct ConfData
{
	STRPTR	cd_Name;			/* Name of Conference				*/
	ULONG	cd_KeepMsg;			/* Max number of messages to keep			*/
	ULONG	cd_KeepTime;		/* Oldest msg to keep (time in seconds)					*/
	STRPTR	cd_Signature;		/* Signature for conference, could be a NULL-pointer	*/
	STRPTR	cd_Alias;			/* Alias used in conference, NULL pointer, no alias		*/
	LONG	cd_BBSConfNr;		/* The internal number a conference has on the BBS			*/
	UWORD	cd_LineLength;		/* Max linelength in messages. Overrides BBSData if > 0	*/
	UBYTE	cd_CharSet;			/* Charset for the grab from this conf. Overrides			*/
								/* BBSData if != BRCS_ANY											*/
	UBYTE	cd_QuoteType;		/* Prefered quite type for this conf. See above for 		*/
								/* definitions.														*/
	LONGBITS cd_Flags;	

	STRPTR	cd_XpkMethod;		/* What xpk method to use when packing this conference			*/
	STRPTR	cd_TagFile;			/* Tagfile to use in this conf. NULL-pointer, use bbs				*/
	STRPTR	cd_EMailAddr;		/* The Email address the user has in this conference. For use	*/
								/* on bbses where the user is member of more than one net.		*/
	STRPTR	cd_ReplyString;		/* Reply string to use when a message is replied _and_			*/
								/* moved. NULL means: Use bbs reply string. 						*/
	STRPTR	cd_ConfEnterScript;	/* Arexx script to be run each time this conference 	*/
								/* is entered. Overrides bbs setting.						*/
	STRPTR	cd_ConfLeaveScript;	/* Arexx script to be run each time this conference 	*/
								/* is left. Overrides bbs setting.							*/
	UBYTE	cd_QuoteChars[4];	/* String to use as quote chars in custom quote type. */
								/* Max length is 3. The string is NUL terminated.		*/
	UBYTE	cd_ConfNetType;		/* States which type of net the conference is on. To 	*/
								/* be used on bbses of bbstypes which support serveral nettypes. Should only 	*/
								/* be set by parser/packer for the bbstype. Se below for defines.					*/
	UBYTE	cd_reserved[3];
	ULONG	cd_ExtraFlags;		/* Extra flags */
	ULONG	cd_HighMark;		/* Number on the remote system/server of the last message/header */
								/* downloaded in this conference */
};


/*  ConfData.cd_Flags  */

#define CDF_MEMBER_OF			(1<<0)	/* Member of this conference	*/
#define CDF_MAIL				(1<<1)	/* Mail conference				*/
#define CDF_READ_ONLY			(1<<2)	/* Read only conference			*/
#define CDF_COMPULSORY			(1<<3)	/* Compulsory conference		*/
#define CDF_FILE_INFO			(1<<4)	/* File info conference			*/
#define CDF_USER_INFO			(1<<5)	/* User info conference			*/
#define CDF_PRIVATE_ALLOWED		(1<<6)	/* Private messages allowed	*/
#define CDF_ENTER_ONLY_TO_ALL	(1<<7)	/* Enter only allowed to ALL	*/
#define CDF_ALIAS				(1<<8)	/* Alias conference				*/
#define CDF_BBS_KEEPMSG			(1<<9)	/* Use BBS KeepMsg for this Conf		*/
#define CDF_BBS_KEEPTIME		(1<<10)	/* Use BBS KeepTime for this Conf	*/
#define CDF_IGNORE_KEEPMSG		(1<<11)	/* cd_KeepMsg is ignored. Messages won't 	*/
										/*	be counted when packing conference		*/
#define CDF_IGNORE_KEEPTIME		(1<<12)	/* cd_KeepTime is ignored.	Time won't be	*/
										/* checked when packing conference			*/
#define CDF_NO_XPK_METHOD		(1<<13)	/* Don't use any xpk method on this conf	*/
#define CDF_NO_SIGNATURE		(1<<14)	/* Don't use any signature in this conf	*/
#define CDF_NOT_ON_BBS			(1<<15)	/* This conference is not on the BBS		*/
#define CDF_NO_TAGS				(1<<16)	/* Don't use message tags in this conf		*/
#define CDF_REPLY_ONLY_TO_ALL 	(1<<17)	/* Replies should be adresses to all		*/
										/* If both CDF_REPLY_ONLY_TO_ALL and CDF_ENTER_ONLY_TO_ALL flags is set are 	*/
										/* all messages in the conference addressed to all.									*/
#define CDF_NO_LINEWRAP_DISPLAY (1<<18) /* Don't line wrap messages in this 		*/
										/* conference when they are displayed. Should be used for conferences with 	*/
										/* messages with ansi-codes. Typical conferences are conferences which are 	*/
										/* used to display bulletins from the bbs.												*/
#define CDF_FILE_SIGNATURE		(1<<19)	/* The cd_Signature field contains the 	*/
										/* path and name of a file containing the signature.									*/
#define CDF_AUTO_XPK_PACK		(1<<20)	/* Automatically xpk pack new messages 	*/
										/* when they are written to this conference. NB: A xpk method must have been	*/
										/* defined.			*/
#define CDF_NO_AUTO_XPK_PACK	(1<<21)	/* No automatically xpk packing in this 	*/
										/* conference. Overrides bbs og global flags for auto xpk packing. 				*/
#define CDF_MARK_OWN_MSGS		(1<<22)	/* Also mark messages from user when 		*/
										/* adding messages with the WBRMSG_MarkMessage tag in this conference.			*/
#define CDF_COMMAND_SIGNATURE	(1<<23)	/* The cd_Signature field contains the path	*/
										/* and name of a command. When the command is executed should the signature 		*/
										/* contents be written to stdout. The template for the command is expected to be */	
										/* BBSNAME/K,CONFNAME/K. 																			*/
#define CDF_REFLOW_QUOTING 		(1<<24)	/* Reflow quoting should be on as default 	*/
										/* when answering messages. This flag should only be considered when 				*/
										/* cd_QuoteType is != QT_USE_SUPER.																*/
#define CDF_ALWAYS_REPLY_STR	(1<<25)	/* Use reply string on all replies. This 		*/
										/* flag should only be considered when cd_ReplyString is non NULL.   				*/
#define CDF_NO_REPLY_STR		(1<<26)	/* Don't use reply strings in this conf. 		*/
										/* Overrides the setting of cd_ReplyString.	*/
#define CDF_BBS_REPLY_STR		(1<<27)	/* Use bbs reply string settings. 				*/
										/* Overrides all other conference reply string settings.									*/
#define CDF_NO_CONF_ENTER_SCR	(1<<28)	/* Don't use a conference enter script in this conference.	*/
#define CDF_NO_CONF_LEAVE_SCR	(1<<29)	/* Don't use a conference leave script on this conference.	*/
#define CDF_FORCE_PACK_REWRITE	(1<<30)	/* Force a rewrite next time this conference */
										/* is packed. If the packing succeed, the flag will be cleared.						*/
#define CDF_ATTACH_FORBIDDEN	(1<<31) /* User may not attach files in this conference.    */

/*  ConfData.cd_ExtraFlags  */

#define CDXF_ONLYHEADERS		(1<<0)	/* This conference can be used for only storing headers	*/
										/* and to request bodies on the user's request.			*/

/*  ConfData.cd_ConfNetType  */ 

#define CDNT_NONET			0	/* This conference is a local conference.	This is the	*/
								/* default values for new conferences. 					*/
#define CDNT_FIDONET		1	/* This conference is a Fido net conference.				*/
#define CDNT_USENET			2	/* This conference is a usenet conference.				*/
#define CDNT_MAILFOLDER		3	/* This conference is a virtual mail folder				*/
#define CDNT_INFO			4	/* This conference is an information conference			*/


/*  Structure holding conference data maintained by the library */

struct ConfInternal
{
	ULONG ci_FirstMsg;		/* First message in conference					*/
	ULONG ci_LastMsg;		/* Last message in conference						*/
	ULONG ci_NumMessages;	/*Number of active messages in conference	*/
	ULONG ci_MsgMarked;		/* Number of messages marked						*/
	ULONG ci_Marked2User;	/* Number of marked messages to user			*/
	ULONG ci_NumQueued;		/* Number of queued messages with only headers		*/
};


/*  Structure used in List returned by GetConfList  */

struct ConfListItem
{
	struct Node cl_Node;	/* Caller is free to use nl_Name		*/
	WORD cl_pad;
	APTR cl_Link;			/* Private!!!!!!!!									*/
	APTR cl_UserData;		/* For use by the caller; no restrictions!	*/
	
	struct ConfData     *cl_Data;		/* Pointer to the conference data	*/
	struct ConfInternal *cl_Internal;	/* Pointer to public internal data	*/

	ULONG cl_ChangeCnt;	/* Private!!!!!!!!	*/
};


/***********************************************************************
*
* File area structures and definitions
*
*/

/*  Structure holding general file area data  */

struct FAreaData
{
	STRPTR ad_Name;		/* Name of file area								*/
};


/*  Structure holding file area data maintained by the library */

struct FAreaInternal
{
	ULONG ai_FirstFile;	/* Number of the first file in this area	*/
	ULONG ai_NumFiles;	/* Number of files in this area 				*/
};


/*  Structure used in List returned by GetFAreaList  */

struct FAreaListItem
{
	struct Node al_Node;	/* Caller is free to use nl_Name					*/
	WORD al_pad;
	APTR al_Link;			/* Private!!!!!!!!									*/
	APTR al_UserData;		/* For use by the caller; no restrictions!	*/
	
	struct FAreaData     *al_Data;		/* Pointer to the file area data		*/
	struct FAreaInternal *al_Internal;	/* Pointer to public internal data	*/

	ULONG al_ChangeCnt;	/* Private!!!!!!!!					*/
};


/***********************************************************************
*
* Passive Conference list structures and definitions
*
*/

struct PassConfListItem
{
	struct Node pl_Node;		/* Caller is free to use nl_Name				*/
	WORD pl_pad;

	STRPTR   pl_Name;			/* Must always be set.									*/
	STRPTR   pl_Description;/* NULL equals no available description			*/
	LONG     pl_BBSConfNr;	/* The internal number the conf has on the bbs	*/
	LONGBITS pl_Flags;		/* Same as CondData.cd_Data.							*/
};

/***********************************************************************
*
* Attachment list structures and definitions
*
*/

struct AttachmentItem
{
	struct Node ai_Node;		/* Caller is free to use nl_Name				*/
	WORD ai_pad;

	STRPTR	ai_Name;			/* Must always be set and contains the filename with full path	*/
	STRPTR	ai_Description;		/* NULL equals no available file comment/description			*/
	ULONG	ai_Filesize;		/* Filesize of the file	*/
	LONGBITS ai_Flags;			/* Flags. None defined yet.							*/
	ULONG	ai_reserved[2];		/* Reserved for future use */
};

/***********************************************************************
*
* Message, event, user, kill, file flags and definitions.
*
*/

/*  Message flags (Returned by GBRMSG_Flags tag)  */

#define MDF_READ			(1<<0)	/* Message is read		*/
#define MDF_REPLIED			(1<<1)	/* Message is replied	*/
#define MDF_PRIVATE			(1<<2)	/* Message is private			*/
#define MDF_TO_USER			(1<<3)	/* Message is to the user		*/
#define MDF_FROM_USER		(1<<4)	/* Message is from the user	*/
#define MDF_DELETED			(1<<5)	/* Message is deleted 								*/
#define MDF_UNRECOVERABLE	(1<<6)	/* Message is can not be undeleted				*/
#define MDF_KEEP			(1<<7)	/* Keep message. Message will not be deleted	*/
									/* during packing of conference 					*/
#define MDF_TO_ALL			(1<<8)	/* Message is to all (has no reciever)			*/
#define MDF_XPK_TEXT		(1<<9)	/* Message text is Xpk'ed (Private flag)		*/
#define MDF_MARKED			(1<<10)	/* Message is marked	(Private flag)				*/
#define MDF_URGENT			(1<<11)	/* Message is urgent									*/
#define MDF_IMPORTANT		(1<<12)	/* Message is important								*/
#define MDF_SUPERMARKED		(1<<13) /* Message will not be unmarked as long as this flag is set.	*/

#define MDF_BINARY_PARTS	(1<<14)	/* Message contains 1 or more BRMSG_BinaryPart tags	*/
#define MDF_TEXT_PARTS		(1<<15)	/* Message contains 1 or more BRMSG_TextPart tags		*/
									/* These two flags does also concern possible message parts.									*/

#define MDF_MESSAGE_PARTS	(1<<16)	/* Message contains 1 or more BRMSG_MsgPart tags		*/
#define MDF_CONFIDENTIAL	(1<<17)	/* Message is confidential										*/
#define MDF_REFERENCED		(1<<18)	/* Message is referenced by some other component in the system,
									and can not be purged before the flag is cleared. Normally, this
									flag is only set with the EVE_FORWARDMSG and will be removed when
									the event is purged. (Private flag) */

#define MDF_HAZE_BIT0		(1<<24)	/* Use MsgHazeLevel macro to extract haze 	*/
#define MDF_HAZE_BIT1		(1<<25)  /* level from message flags.						*/
#define MsgHazeLevel(msgflags) ((msgflags & (MDF_HAZE_BIT0 | MDF_HAZE_BIT1)) >> 24)

#define MDF_QUEUED			(1<<26)	/* Message is queued for future collection of the main body and header	*/
#define MDF_EMPTY_BODY		(1<<27)	/* Message consists of only a header and no full body */


/*  Event flags (Returned by RBREV_Flags tag)  */
#define EDF_DELETED			(1<<0)	/* Event is deleted	*/
#define EDF_PACKED			(1<<1)	/* Event is packed	*/
#define EDF_DONE			(1<<2)	/* Event is done								*/
#define EDF_ERROR			(1<<3)	/* Error performing this event			*/
#define EDF_UNRECOVERABLE	(1<<4)	/* Event can not be undeleted													*/
#define EDF_FREEZE			(1<<5)	/* Event is frozen. Will not be done as long as this flag is set.	*/


/*  Event types supported   */

#define EVE_ENTERMSG		0L		/* Enter message		*/
#define EVE_REPLYMSG		1L		/* Reply message		*/
#define EVE_JOINCONF		2L		/* Join conference	*/
#define EVE_RESIGNCONF		3L		/* Resign conference	*/
#define EVE_DOWNLOAD		4L		/* Download file		*/
#define EVE_UPLOAD			5L		/* Upload file			*/
#define EVE_CONFLIST		6L		/* Get conflist									*/
#define EVE_CONFIGUREBBS	7L		/* Set correct configuration on the bbs.	*/
#define EVE_SENDUSERINFO	8L		/* Send userinfo									*/
#define EVE_FORWARDMSG		9L		/* Forward a specific message				*/
#define EVE_GETNEWFILES		10L		/* Get new files listing				*/
#define EVE_DOCOMMAND		11L		/* Command the script should do		*/
#define EVE_AUTOLOGOFF		12L		/* Autologoff on next session			*/
#define EVE_RETRACTMSG		13L		/* Retract message from remote system			*/
#define EVE_RETRACTFILE		14L		/* Retract file from remote system			*/
#define EVE_REQUESTMSG		15L		/* Request message from remote system			*/
#define EVE_MAXEVENT		16L


/*  User flags (Returned by RBRUSR_Flags tag)  */
#define UDF_DELETED			(1<<0)	/* User is deleted				*/
#define UDF_UNRECOVERABLE	(1<<1)	/* User can not be undeleted	*/


/*  Kill flags (Returned by RBRK_Flags tag)  */
#define KDF_DELETED			(1<<0)	/* Kill is deleted	*/
#define KDF_UNRECOVERABLE	(1<<1)	/* Kill can not be undeleted  */
#define KDF_MARK_KEEP		(1<<2)	/* Mark message with MDF_KEEP if matched	*/
#define KDF_MARK_URGENT		(1<<3)	/* Mark message with MDF_URGENT if matched  */
#define KDF_MARK_IMPORTANT	(1<<4)	/* Mark message with MDF_IMPORTANT if matched */
#define KDF_MARK_DELETED	(1<<5)	/* Delete message if matched */

#define KDF_MARK_HAZE_BIT0	(1<<24)	/* Use KillHazeLevel macro to extract haze	*/
#define KDF_MARK_HAZE_BIT1	(1<<25)  /* level from kill flags.							*/

#define KillHazeLevel(killflags) ((killflags & (KDF_MARK_HAZE_BIT0 | KDF_MARK_HAZE_BIT1)) >> 24)


/*  File flags (Returned by RBRF_Flags tag)  */
#define FDF_DELETED			(1<<0)	/* File is deleted			*/
#define FDF_UNRECOVERABLE	(1<<1)	/* File is unrecoverable	*/



/***********************************************************************
*
*  Search result structures and definitions.
*
*/

struct SearchResult	/* Structure returned from SearchBRMessage()  */
{
	struct SearchResult *sr_NextResult;	/* Link to next SearchResult	*/
	ULONG sr_MsgsFound;		/* Number of messagenumbers in the buffer	*/
	ULONG *sr_Messages;		/* Buffer holding the messagenumbers		*/
									/* NULL-terminated.								*/
};


struct UserSuggestion	/* Structure returned from SearchBRUser()	*/
{
	struct Node us_Node;		/* us_Node.ln_Name contains the name of the user	*/
	ULONG us_UserNr;			/* Number of user found										*/
};


struct FoundFile
{
	ULONG ff_FileNr;						/* Number of file found								*/
	struct FAreaListItem *ff_FArea;	/* File area file belongs to. This is a list	*/
												/* item from the SBRF_SearchFAreaList or the	*/
												/* SBRF_SearchFArea. 								*/
};

struct SFileResult	/* Structure returned by SearchBRFile()	*/
{
	struct SFileResult *fr_NextResult;	/* Link to next SFileResult					*/
	ULONG fr_FilesFound;						/* Number of files in *this* SFileResult	*/
	struct FoundFile *fr_Files;			/* Buffer holding FoundFile structures		*/
};


struct FoundUser
{
	ULONG fu_UserNr;		/* Number of user found											*/
	ULONG fu_FoundInTag;	/* In which tag the search string was found first in	*/
};

struct SUserResult	/* Structure returned by SearchBRUser()	*/
{
	struct SUserResult *ur_NextResult;	/* Link to next SUserResult					*/
	ULONG ur_UsersFound;						/* Number of users in *this* SUserResult	*/
	struct FoundUser *ur_Users;			/* Buffer holding FoundUser structures		*/
};



/***********************************************************************
*
*  Structure returned with data about user.
*
*/

struct UserData		/* Structure returned by BBSUserData()	*/
{
	STRPTR ud_Name;
	STRPTR ud_Street;
	STRPTR ud_Address;
	STRPTR ud_Country;
 	STRPTR ud_Phone;		/* May be NULL	*/
};


/***********************************************************************
*
*  Structures passed to callback hooks.
*
*/
#define DEF_PROGRESS_UPDATES	100	/* Default progress callbacks on each pass	*/

/* 
	Prototype for the progress callback hook function should be: 

	ULONG __saveds __asm DoBRProgress(register __a0 struct Hook *hook, 
		register __a2 struct BRProgress *progress, register __a1 APTR message);

	No messages defined yet. The messages parameter will always be NULL;
*/

struct BRProgress		/* Structure passed as object to the different progress callback hooks 	*/
{
	ULONG brp_Pass;		/* Some operations are done in more than one pass		*/
	ULONG brp_First;	/* First object in this pass.									*/
	ULONG brp_Last;		/* Last object in this pass.									*/
						/* brp_First may be greater than brp_Last!				*/
	ULONG brp_Current;	/* Current object.												*/
	ULONG brp_Actions;	/* How many objects so far which has been altered.		*/
	ULONG brp_Skip;		/* How manu objects done between each call to the hook. (read only)	*/
	ULONG brp_LastPass; /* Is set to the value of brp_Pass had the last call to the hook.	*/
	                    /* Will be 0 in the first call to the hook													*/
};

/***********************************************************************
*
*  Methods used by GetMsgsSorted().
*
*/
#define BRGMS_REFERENCE			1	/* Sort the range by references 				*/
#define BRGMS_MINEFIRST			2	/* Sort messages to you first 					*/
#define BRGMS_ALLFIRST			3	/* Sort messages to all first 					*/
#define BRGMS_MSGNUMBER			4	/* Sort the range by messagenumbers 			*/
#define BRGMS_SUBJECT			5	/* Sort the range by subject 					*/
#define BRGMS_SUBJECT_ALPHA		6	/* Sort the range by subject, alphabetically 	*/
#define BRGMS_AUTHOR			7	/* Sort the range by author 					*/
#define BRGMS_AUTHOR_ALPHA		8	/* Sort the range by author, alphabetically		*/

#if defined(__GNUC__)
# pragma pack()
#endif

#endif /* LIBRARIES_BBSREAD_H */
