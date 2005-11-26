#ifndef LIBRARIES_BBSREADTAGS_H
#define LIBRARIES_BBSREADTASG_H
/*
**  $VER: BBSReadTags.h 6.1 (07.02.98)
** 
**  BBSRead tag definitions.
**
**  © Copyright 1994-1998 Ultima Thule Software,
**  All Rights Reserved.
**
**  Authors: Eivind Nordseth and Petter Nilsen
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

#if defined(__GNUC__)
# pragma pack(2)
#endif

/* Tag modifers */

#define BR_TagBase		TAG_USER			/* Base for normal tags		*/
#define BR_MsgTags		((1L<<16)+TAG_USER)	/* Base for message tags	*/
#define BR_EventTags	((2L<<16)+TAG_USER)	/* Base for event tags		*/
#define BR_UserTags		((3L<<16)+TAG_USER)	/* Base for user tags		*/
#define BR_FileTags		((4L<<16)+TAG_USER)	/* Base for file tags		*/
#define BR_KillTags		((5L<<16)+TAG_USER)	/* Base for kill tags		*/

#define BR_TagVarPar	(1L<<29)			/* Tagitem points to data to be set/changed	*/
#define BR_TagTagArr	BR_TagVarPar		/* Tagitem points to a tag array. Used only	*/ 
											/* by tags saved in database. 					*/
#define BR_TagString	(1L<<30)			/* Tagitem is a STRPTR								*/
#define BR_TagStringArr	((1L<<29)|(1L<<30))	/* Tagitem points to an array of STRPTR		*/


/*  Tags understood by ConfigBBS  */

#define BC_BBSList					(BR_TagBase + BR_TagVarPar + 1)
#define BC_BBSName					(BR_TagBase + BR_TagString + 2)
#define BC_GrabName					(BR_TagBase + BR_TagString + 3)
#define BC_BBSType					(BR_TagBase + BR_TagString + 4)
#define BC_UserName					(BR_TagBase + BR_TagString + 5)
#define BC_DeleteBBS				(BR_TagBase                + 6)
#define BC_ScriptFlags				(BR_TagBase                + 7)
#define BC_Signature				(BR_TagBase + BR_TagString + 8)
#define BC_Top						(BR_TagBase                + 9)
#define BC_Bottom					(BR_TagBase                + 10)	
#define BC_Up						(BR_TagBase                + 11)
#define BC_Down						(BR_TagBase                + 12)
#define BC_SortBBSList				(BR_TagBase                + 13)
#define BC_KeepMessages				(BR_TagBase                + 14)
#define BC_KeepTime					(BR_TagBase                + 15)
#define BC_SetBBSFlags				(BR_TagBase                + 16)	
#define BC_ClearBBSFlags			(BR_TagBase                + 17)	
#define BC_EMailAddr				(BR_TagBase + BR_TagString + 18)
#define BC_XpkMethod				(BR_TagBase + BR_TagString + 19)
#define BC_CharSet					(BR_TagBase                + 20)
#define BC_LineLength				(BR_TagBase                + 21)
#define BC_UserStreet				(BR_TagBase + BR_TagString + 22)
#define BC_UserAddress				(BR_TagBase + BR_TagString + 23)
#define BC_UserCountry				(BR_TagBase + BR_TagString + 24)
#define BC_UserPhone				(BR_TagBase + BR_TagString + 25)
#define BC_Alias					(BR_TagBase + BR_TagString + 26)
#define BC_DnloadPath				(BR_TagBase + BR_TagString + 27)
#define BC_TagFile					(BR_TagBase + BR_TagString + 28)
#define BC_EventArchiver			(BR_TagBase + BR_TagString + 29)
#define BC_ReplyPacket				(BR_TagBase + BR_TagString + 30)
#define BC_UploadPath				(BR_TagBase + BR_TagString + 31)
#define BC_NewBBSOrder				(BR_TagBase +              + 32)
#define BC_QuoteType				(BR_TagBase +              + 33)
#define BC_QuoteChars				(BR_TagBase + BR_TagString + 34)
#define BC_ReplyString				(BR_TagBase + BR_TagString + 35)
#define BC_BBSEnterScript			(BR_TagBase + BR_TagString + 36)
#define BC_BBSLeaveScript			(BR_TagBase + BR_TagString + 37)
#define BC_ConfEnterScript			(BR_TagBase + BR_TagString + 38)
#define BC_ConfLeaveScript			(BR_TagBase + BR_TagString + 39)


/*  Tags understood by ConfigConf()  */

#define CC_ConfList					(BR_TagBase + BR_TagVarPar + 101)	
#define CC_AddToBBS					(BR_TagBase + BR_TagVarPar + 102)	
#define CC_ConfName					(BR_TagBase + BR_TagString + 103)	
#define CC_SetConfFlags				(BR_TagBase                + 104)	
#define CC_ClearConfFlags			(BR_TagBase                + 105)	
#define CC_DeleteConf				(BR_TagBase                + 106)
#define CC_Signature				(BR_TagBase + BR_TagString + 107)
#define CC_KeepMessages				(BR_TagBase                + 108)
#define CC_KeepTime					(BR_TagBase                + 109)
#define CC_Top						(BR_TagBase                + 110)
#define CC_Bottom					(BR_TagBase                + 111)	
#define CC_Up						(BR_TagBase                + 112)
#define CC_Down						(BR_TagBase                + 113)
#define CC_Alias					(BR_TagBase + BR_TagString + 114)
#define CC_BBSConfNr				(BR_TagBase                + 115)
#define CC_SortConfList				(BR_TagBase                + 116)
#define CC_XpkMethod				(BR_TagBase + BR_TagString + 117)
#define CC_CharSet					(BR_TagBase                + 118)
#define CC_LineLength				(BR_TagBase                + 119)
#define CC_TagFile					(BR_TagBase + BR_TagString + 120)
#define CC_EmailAddr				(BR_TagBase + BR_TagString + 121)
#define CC_NewConfOrder				(BR_TagBase                + 122)
#define CC_QuoteType				(BR_TagBase +              + 123)
#define CC_QuoteChars				(BR_TagBase + BR_TagString + 124)
#define CC_ReplyString				(BR_TagBase + BR_TagString + 125)
#define CC_ConfEnterScript			(BR_TagBase + BR_TagString + 126)
#define CC_ConfLeaveScript			(BR_TagBase + BR_TagString + 127)
#define CC_ConfNetType				(BR_TagBase +              + 128)
#define CC_HighMark					(BR_TagBase +              + 129)


/*  Tags understood by MarkMessage()  */

#define MM_MarkMessage				(BR_TagBase                + 401)
#define MM_MarkArray				(BR_TagBase + BR_TagVarPar + 402)
#define MM_UnMarkMessage			(BR_TagBase                + 403)
#define MM_UnMarkArray				(BR_TagBase + BR_TagVarPar + 404)
#define MM_Reset					(BR_TagBase                + 405)
#define MM_MineFirst				(BR_TagBase                + 406)
#define MM_Reference				(BR_TagBase                + 407)
#define MM_ToAllFirst				(BR_TagBase                + 408)
#define MM_SortByMsgNumbers			(BR_TagBase                + 409)
#define MM_GroupSubject				(BR_TagBase                + 410)
#define MM_Reverse					(BR_TagBase                + 411)
#define MM_SuperMarking				(BR_TagBase                + 412)
#define MM_SortAlphabetical			(BR_TagBase                + 413)
#define MM_SortByAuthor				(BR_TagBase                + 414)


/*  Tags understood by ConfigType()  */

#define CT_DeleteType				(BR_TagBase                + 601)
#define CT_TypeList					(BR_TagBase + BR_TagVarPar + 602)
#define CT_TypeName					(BR_TagBase + BR_TagString + 603)
#define CT_LineLength				(BR_TagBase                + 604)
#define CT_SubjectLength			(BR_TagBase                + 605)
#define CT_FileDescrLen				(BR_TagBase                + 606)
#define CT_DeleteEvent				(BR_TagBase                + 607)
#define CT_ConfigEvent				(BR_TagBase                + 608)
#define CT_EventNeedTags			(BR_TagBase                + 609)
#define CT_EventOptTags				(BR_TagBase                + 610)
#define CT_CharSet					(BR_TagBase                + 611)
#define CT_MsgParser				(BR_TagBase + BR_TagString + 612)	
#define CT_AvailScrFlags			(BR_TagBase                + 613)
#define CT_FileNameLen				(BR_TagBase                + 614)
#define CT_EventPacker				(BR_TagBase + BR_TagString + 615)
#define CT_EventArchiver			(BR_TagBase + BR_TagString + 616)
#define CT_SetTypeFlags				(BR_TagBase                + 617)
#define CT_ClearTypeFlags			(BR_TagBase                + 618)
#define CT_AcceptPattern			(BR_TagBase + BR_TagString + 619)
#define CT_InitMsgFile				(BR_TagBase + BR_TagString + 620)
#define CT_ExtConfig				(BR_TagBase + BR_TagString + 621)
#define CT_QuoteType				(BR_TagBase                + 622)


/*  Tags understood by PackDataFile()	*/

#define PD_EventData				(BR_TagBase + BR_TagVarPar + 801)
#define PD_Conference				(BR_TagBase + BR_TagVarPar + 802)
#define PD_UserData					(BR_TagBase + BR_TagVarPar + 803)
#define PD_KillData					(BR_TagBase + BR_TagVarPar + 804)
#define PD_FileData					(BR_TagBase + BR_TagVarPar + 805)
#define PD_SavePackedBRIEF			(BR_TagBase + BR_TagString + 806)
#define PD_AttachmentList			(BR_TagBase + BR_TagString + 807)

/*  Tags understood by ConfigGlobal()	*/

#define CG_DnloadPath				(BR_TagBase + BR_TagString + 1001)
#define CG_UploadPath				(BR_TagBase + BR_TagString + 1002)
#define CG_Buffers					(BR_TagBase                + 1004)
#define CG_BufferSize				(BR_TagBase                + 1005)
#define CG_HitRate					(BR_TagBase + BR_TagVarPar + 1007)
#define CG_ConfigArchiver			(BR_TagBase + BR_TagString + 1008)
#define CG_ArcPattern				(BR_TagBase + BR_TagString + 1009)
#define CG_UnArcCmd					(BR_TagBase + BR_TagString + 1010)
#define CG_ArcCmd					(BR_TagBase + BR_TagString + 1011)
#define CG_DeleteArchiver			(BR_TagBase + BR_TagString + 1012)
#define CG_Signature				(BR_TagBase + BR_TagString + 1013)
#define CG_KeepMessages				(BR_TagBase                + 1014)
#define CG_KeepTime					(BR_TagBase                + 1015)
#define CG_SetGlobalFlags			(BR_TagBase                + 1016)	
#define CG_ClearGlobalFlags			(BR_TagBase                + 1017)	
#define CG_XpkMethod				(BR_TagBase + BR_TagString + 1018)
#define CG_UserPhone				(BR_TagBase + BR_TagString + 1019)
#define CG_TmpDir					(BR_TagBase + BR_TagString + 1020)
#define CG_TagFile					(BR_TagBase + BR_TagString + 1021)
#define CG_BufCopyBack				(BR_TagBase                + 1022)
#define CG_BufEndCopyBack			(BR_TagBase                + 1023)
#define CG_ReadHitRate				(BR_TagBase + BR_TagVarPar + 1024)
#define CG_WriteHitRate				(BR_TagBase + BR_TagVarPar + 1025)
#define CG_ClearHitRate				(BR_TagBase                + 1026)
#define CG_HazeLevel1				(BR_TagBase                + 1027)
#define CG_HazeLevel2				(BR_TagBase                + 1028)
#define CG_HazeLevel3				(BR_TagBase                + 1029)
#define CG_PGPCommand				(BR_TagBase + BR_TagString + 1030)
#define CG_PGPSignID				(BR_TagBase + BR_TagString + 1031)
#define CG_QuoteChars				(BR_TagBase + BR_TagString + 1032)
#define CG_ReplyString				(BR_TagBase + BR_TagString + 1033)
#define CG_StartupScript			(BR_TagBase + BR_TagString + 1034)
#define CG_QuitScript				(BR_TagBase + BR_TagString + 1035)
#define CG_BBSEnterScript			(BR_TagBase + BR_TagString + 1036)
#define CG_BBSLeaveScript			(BR_TagBase + BR_TagString + 1037)
#define CG_ConfEnterScript			(BR_TagBase + BR_TagString + 1038)
#define CG_ConfLeaveScript			(BR_TagBase + BR_TagString + 1039)
#define CG_BufFreeAllocated			(BR_TagBase                + 1040)


/*  Tags understood by UpdateDataStruct()	*/

#define UD_ConfList					(BR_TagBase + BR_TagVarPar + 1101)
#define UD_ConfItem					(BR_TagBase + BR_TagVarPar + 1102)
#define UD_BBSList					(BR_TagBase + BR_TagVarPar + 1103)
#define UD_BBSItem					(BR_TagBase + BR_TagVarPar + 1104)
#define UD_FAreaList				(BR_TagBase + BR_TagVarPar + 1105)
#define UD_FAreaItem				(BR_TagBase + BR_TagVarPar + 1106)
#define UD_RemoveDeleted			(BR_TagBase + BR_TagVarPar + 1107)


/* Tags understood by Archive()	*/

#define AR_AddFile					(BR_TagBase + BR_TagString + 1301)
#define AR_SourceDir				(BR_TagBase + BR_TagString + 1302)


/* Tags understood by MakeEventPackage()	*/

#define EP_PublicScreen				(BR_TagBase + BR_TagString + 1401)


/* Tags understood by ParseGrab()	*/

#define PG_PublicScreen				(BR_TagBase + BR_TagString + 1501)
#define PG_RequestWindow			(BR_TagBase + BR_TagVarPar + 1502)


/*  Tags understood by UnArchive()	*/

#define UA_RetrieveFile				(BR_TagBase + BR_TagString + 1701)
#define UA_DestDir					(BR_TagBase + BR_TagString + 1702)
#define UA_ArchiverUsed				(BR_TagBase + BR_TagVarPar + 1703)


/*  Tags understood by SearchConf()	*/

#define SC_FindString				(BR_TagBase + BR_TagString + 1801)
#define SC_FromMessage				(BR_TagBase                + 1802)
#define SC_ToMessage				(BR_TagBase                + 1803)
#define SC_FromUser					(BR_TagBase + BR_TagString + 1804)
#define SC_ToUser					(BR_TagBase + BR_TagString + 1805)
#define SC_SearchSubject			(BR_TagBase                + 1806)
#define SC_SearchMessage			(BR_TagBase                + 1807)
#define SC_MessageArray				(BR_TagBase + BR_TagVarPar + 1808)
#define SC_ToAll					(BR_TagBase                + 1809)
#define SC_KeptMessages				(BR_TagBase                + 1810)
#define SC_NewerThan				(BR_TagBase                + 1811)
#define SC_OlderThan				(BR_TagBase                + 1812)
#define SC_SearchComment			(BR_TagBase                + 1813)
#define SC_SearchMsgID				(BR_TagBase                + 1814)


/* Message fields */
#define MSG_FromName				0	/* Saved with the message header */
#define MSG_FromAddr				1
#define MSG_ToName					2
#define MSG_ToAddr					3
#define MSG_MsgID					4
#define MSG_OrginalNr				5
#define MSG_RefID					6
#define MSG_RefNr					7
#define MSG_CreationDate			8
#define MSG_CreationDateTxt			9
#define MSG_Subject					10
#define MSG_MsgSize					24
#define MSG_SpecialID				25
#define MSG_ErrorString				26

#define MSG_ReplyConf				11	/* Not saved with the message header */
#define MSG_ReplyName				12
#define MSG_ReplyAddr				13
#define MSG_Comment					14
#define MSG_Text					15
#define MSG_BinaryPart				16
#define MSG_BinaryPartDesc			17
#define MSG_BinaryPartComment		18
#define MSG_TextPart				19
#define MSG_TextPartComment			20
#define MSG_MsgPart					21
#define MSG_TextPartUniqueID		22
#define MSG_TextPartFilename		23


/*  Message Tags  */
#define BRMSG_FromName				(BR_MsgTags + BR_TagString    + MSG_FromName)
#define BRMSG_FromAddr				(BR_MsgTags + BR_TagString    + MSG_FromAddr)
#define BRMSG_ToName				(BR_MsgTags + BR_TagString    + MSG_ToName)
#define BRMSG_ToAddr				(BR_MsgTags + BR_TagString    + MSG_ToAddr)
#define BRMSG_MsgID					(BR_MsgTags + BR_TagString    + MSG_MsgID)
#define BRMSG_OrginalNr				(BR_MsgTags                   + MSG_OrginalNr)
#define BRMSG_RefID					(BR_MsgTags + BR_TagString    + MSG_RefID)
#define BRMSG_RefNr					(BR_MsgTags                   + MSG_RefNr)
#define BRMSG_MsgSize				(BR_MsgTags                   + MSG_MsgSize)
#define BRMSG_SpecialID				(BR_MsgTags                   + MSG_SpecialID)
#define BRMSG_ErrorString			(BR_MsgTags                   + MSG_ErrorString)
#define BRMSG_CreationDate			(BR_MsgTags                   + MSG_CreationDate)
#define BRMSG_CreationDateTxt		(BR_MsgTags + BR_TagString    + MSG_CreationDateTxt)
#define BRMSG_Subject				(BR_MsgTags + BR_TagString    + MSG_Subject)
#define BRMSG_ReplyConf				(BR_MsgTags + BR_TagString    + MSG_ReplyConf)
#define BRMSG_ReplyName				(BR_MsgTags + BR_TagString    + MSG_ReplyName)
#define BRMSG_ReplyAddr				(BR_MsgTags + BR_TagString    + MSG_ReplyAddr)
#define BRMSG_Comment				(BR_MsgTags + BR_TagStringArr + MSG_Comment)
#define BRMSG_Text					(BR_MsgTags + BR_TagStringArr + MSG_Text)
#define BRMSG_BinaryPart			(BR_MsgTags + BR_TagString    + MSG_BinaryPart)
#define BRMSG_BinaryPartDesc		(BR_MsgTags + BR_TagString    + MSG_BinaryPartDesc)
#define BRMSG_BinaryPartComment		(BR_MsgTags + BR_TagStringArr + MSG_BinaryPartComment)
#define BRMSG_TextPart				(BR_MsgTags + BR_TagStringArr + MSG_TextPart)
#define BRMSG_TextPartComment		(BR_MsgTags + BR_TagStringArr + MSG_TextPartComment)
#define BRMSG_MsgPart				(BR_MsgTags + BR_TagTagArr    + MSG_MsgPart)
#define BRMSG_TextPartUniqueID		(BR_MsgTags                   + MSG_TextPartUniqueID)
#define BRMSG_TextPartFilename		(BR_MsgTags + BR_TagString    + MSG_TextPartFilename)


/*  Tags understood by WriteBRMessage()  */

#define WBRMSG_MarkMessage			(BR_TagBase + 2000)
#define WBRMSG_Private				(BR_TagBase + 2001)
#define WBRMSG_Read					(BR_TagBase + 2002)
#define WBRMSG_Urgent				(BR_TagBase + 2003)
#define WBRMSG_Important			(BR_TagBase + 2004)
#define WBRMSG_Confidential			(BR_TagBase + 2005)
#define WBRMSG_ToFromUserStatus		(BR_TagBase + 2006)
#define WBRMSG_Queued				(BR_TagBase + 2007)
#define WBRMSG_EmptyBody			(BR_TagBase + 2008)


/*  Tags understood by ReadBRMessage()	*/

#define RBRMSG_MsgTagsPtr			(BR_TagBase + BR_TagVarPar + 2100)
#define RBRMSG_MsgDate				(BR_TagBase + BR_TagVarPar + 2101)
#define RBRMSG_Reference			(BR_TagBase + BR_TagVarPar + 2102)
#define RBRMSG_FirstRef				(BR_TagBase + BR_TagVarPar + 2103)
#define RBRMSG_LastRef				(BR_TagBase + BR_TagVarPar + 2104)
#define RBRMSG_PrevRef				(BR_TagBase + BR_TagVarPar + 2105)
#define RBRMSG_NextRef				(BR_TagBase + BR_TagVarPar + 2106)
#define RBRMSG_Flags				(BR_TagBase + BR_TagVarPar + 2107)
#define RBRMSG_GetHeader			(BR_TagBase                + 2108)
#define RBRMSG_GetText				(BR_TagBase                + 2109)


/* Event fields */
#define EV_ToName					0
#define EV_ToAddr					1
#define EV_Subject					2
#define EV_Conference				3
#define EV_RefNr					4
#define EV_RefOrginalNr				5
#define EV_RefId					6
#define EV_MsgFile					7
#define EV_Private					8
#define EV_LocalFile				9
#define EV_Directory				10
#define EV_FileName					11
#define EV_DownloadNotify			12
#define EV_FileDescr				13
#define EV_DetailedFileDescr		14
#define EV_FromMessageNr			15
#define EV_ToMessageNr				16
#define EV_CommandString			17
#define EV_Boolean					18
#define EV_Date						19
#define EV_PGPSignID				20
#define EV_PGPEncryptID				21
#define EV_RefConference			22
#define EV_Urgent					23
#define EV_Important				24
#define EV_Confidential				25
#define EV_ReturnReceipt			26
#define EV_Encode8bit				27


/*  Event Tags  */
#define BREV_ToName					(BR_EventTags + BR_TagString + EV_ToName)
#define BREV_ToAddr					(BR_EventTags + BR_TagString + EV_ToAddr)
#define BREV_Subject				(BR_EventTags + BR_TagString + EV_Subject)
#define BREV_Conference				(BR_EventTags + BR_TagString + EV_Conference)
#define BREV_RefNr					(BR_EventTags                + EV_RefNr)
#define BREV_RefOrginalNr			(BR_EventTags                + EV_RefOrginalNr)
#define BREV_RefId					(BR_EventTags + BR_TagString + EV_RefId)
#define BREV_MsgFile				(BR_EventTags + BR_TagString + EV_MsgFile)
#define BREV_Private				(BR_EventTags                + EV_Private)
#define BREV_LocalFile				(BR_EventTags + BR_TagString + EV_LocalFile)
#define BREV_Directory				(BR_EventTags + BR_TagString + EV_Directory)
#define BREV_FileName				(BR_EventTags + BR_TagString + EV_FileName)
#define BREV_DownloadNotify			(BR_EventTags                + EV_DownloadNotify)
#define BREV_FileDescr				(BR_EventTags + BR_TagString + EV_FileDescr)
#define BREV_DetailedFileDescr		(BR_EventTags + BR_TagString + EV_DetailedFileDescr)
#define BREV_FromMessageNr			(BR_EventTags                + EV_FromMessageNr)
#define BREV_ToMessageNr			(BR_EventTags                + EV_ToMessageNr)
#define BREV_CommandString			(BR_EventTags + BR_TagString + EV_CommandString)
#define BREV_Boolean				(BR_EventTags                + EV_Boolean)
#define BREV_Date					(BR_EventTags                + EV_Date)
#define BREV_PGPSignID				(BR_EventTags + BR_TagString + EV_PGPSignID)
#define BREV_PGPEncryptID			(BR_EventTags + BR_TagString + EV_PGPEncryptID)
#define BREV_RefConference			(BR_EventTags + BR_TagString + EV_RefConference)
#define BREV_Urgent					(BR_EventTags                + EV_Urgent)
#define BREV_Important				(BR_EventTags                + EV_Important)
#define BREV_Confidential			(BR_EventTags                + EV_Confidential)
#define BREV_ReturnReceipt			(BR_EventTags                + EV_ReturnReceipt)
#define BREV_Encode8bit				(BR_EventTags                + EV_Encode8bit)


/*  Tags understood by WriteBREvent()  */

#define WBREV_UpdateEventNr 		(BR_TagBase + 2200)


/*  Tags understood by ReadBREvent()  */

#define RBREV_EventTagsPtr			(BR_TagBase + BR_TagVarPar + 2300)
#define RBREV_EventType				(BR_TagBase + BR_TagVarPar + 2301)
#define RBREV_EventDate				(BR_TagBase + BR_TagVarPar + 2302)
#define RBREV_Flags					(BR_TagBase + BR_TagVarPar + 2303)


/*  Tags understood by UpdateBRMessage()  */

#define UBRM_SetDelete				(BR_TagBase + 2400)
#define UBRM_ClearDelete			(BR_TagBase + 2401)
#define UBRM_SetKeep				(BR_TagBase + 2402)
#define UBRM_ClearKeep				(BR_TagBase + 2403)
#define UBRM_SetReplied				(BR_TagBase + 2404)
#define UBRM_ClearReplied			(BR_TagBase + 2405)
#define UBRM_SetUrgent				(BR_TagBase + 2406)
#define UBRM_ClearUrgent			(BR_TagBase + 2407)
#define UBRM_SetImportant			(BR_TagBase + 2408)
#define UBRM_ClearImportant			(BR_TagBase + 2409)
#define UBRM_SetHazeLevel			(BR_TagBase + 2410)
#define UBRM_SetConfidential		(BR_TagBase + 2411)
#define UBRM_ClearConfidential		(BR_TagBase + 2412)
#define UBRM_SetMarked				(BR_TagBase + 2413)
#define UBRM_ClearMarked			(BR_TagBase + 2414)
#define UBRM_SetSuperUnread			(BR_TagBase + 2415)
#define UBRM_ClearSuperUnread		(BR_TagBase + 2416)
#define UBRM_SetQueued				(BR_TagBase + 2417)
#define UBRM_ClearQueued			(BR_TagBase + 2418)
#define UBRM_UpdateHeaders			(BR_TagBase + 2419)
#define UBRM_UpdateText				(BR_TagBase + 2420)


/*  Tags understood by UpdateBREvent()  */

#define UBRE_SetDeleted				(BR_TagBase + 2500)
#define UBRE_ClearDeleted			(BR_TagBase + 2501)
#define UBRE_SetPacked				(BR_TagBase + 2502)
#define UBRE_ClearPacked			(BR_TagBase + 2503)
#define UBRE_SetError				(BR_TagBase + 2504)
#define UBRE_ClearError				(BR_TagBase + 2505)
#define UBRE_SetDone				(BR_TagBase + 2506)
#define UBRE_ClearDone				(BR_TagBase + 2507)
#define UBRE_SetFreeze				(BR_TagBase + 2508)
#define UBRE_ClearFreeze			(BR_TagBase + 2509)
#define UBRE_Activate				(BR_TagBase + 2510)


/*  User fields  */
#define USR_Name					0
#define USR_Address					1
#define USR_Alias					2
#define USR_Comment					3
#define USR_Encode8BitMsg			4
#define USR_PGPkeyID				5


/*  User tags  */
#define BRUSR_Name					(BR_UserTags + BR_TagString    + USR_Name)
#define BRUSR_Address				(BR_UserTags + BR_TagString    + USR_Address)
#define BRUSR_Alias					(BR_UserTags + BR_TagString    + USR_Alias)
#define BRUSR_Comment				(BR_UserTags + BR_TagStringArr + USR_Comment)
#define BRUSR_Encode8BitMsg			(BR_UserTags                   + USR_Encode8BitMsg)
#define BRUSR_PGPkeyID				(BR_UserTags + BR_TagString    + USR_PGPkeyID)


/*  Tags understood by WriteBRUser()  */

#define WBRUSR_UpdateUserNr 		(BR_TagBase + 2600)
#define WBRUSR_DeleteUser			(BR_TagBase + 2601)
#define WBRUSR_OnlyIfNotExist		(BR_TagBase + 2602)


/*  Tags understood by ReadBREvent()  */

#define RBRUSR_UserTagsPtr			(BR_TagBase + BR_TagVarPar + 2700)
#define RBRUSR_UserDate				(BR_TagBase + BR_TagVarPar + 2701)
#define RBRUSR_Flags				(BR_TagBase + BR_TagVarPar + 2702)


/* Tags understood by SearchBRUser()	*/

#define SBRU_SearchStr				(BR_TagBase + BR_TagString + 2800)
#define SBRU_SearchName				(BR_TagBase                + 2801)
#define SBRU_SearchAddr				(BR_TagBase                + 2802)
#define SBRU_SearchAlias			(BR_TagBase                + 2803)
#define SBRU_SuggestUsers			(BR_TagBase + BR_TagVarPar + 2804)
#define SBRU_SearchComment			(BR_TagBase                + 2805)


/*  Kill Tags */

#define BRKILL_Conference			(BR_KillTags + BR_TagString + 0)
#define BRKILL_Private				(BR_KillTags +              + 1)
#define BRKILL_Description			(BR_KillTags + BR_TagString + 2)


/*  Tags understood by WriteBRKill()  */

#define WBRK_UpdateKillNr 			(BR_TagBase + 3000)
#define WBRK_DeleteKill				(BR_TagBase + 3001)
#define WBRK_MarkKeep				(BR_TagBase + 3002)
#define WBRK_MarkUrgent				(BR_TagBase + 3003)
#define WBRK_MarkImportant			(BR_TagBase + 3004)
#define WBRK_MarkHazeLevel			(BR_TagBase + 3005)
#define WBRK_MarkDeleted			(BR_TagBase + 3006)

/*  Tags understood by ReadBRKill()  */

#define RBRK_KillTagsPtr			(BR_TagBase + BR_TagVarPar + 3100)
#define RBRK_KillDate				(BR_TagBase + BR_TagVarPar + 3101)
#define RBRK_LastKill				(BR_TagBase + BR_TagVarPar + 3102)
#define RBRK_Flags					(BR_TagBase + BR_TagVarPar + 3103)


/* Tags understood by ConfigFArea()	*/

#define CFA_DeleteFArea				(BR_TagBase                + 3200)
#define CFA_FAreaList				(BR_TagBase + BR_TagVarPar + 3201)
#define CFA_AddToBBS				(BR_TagBase + BR_TagVarPar + 3202)
#define CFA_Name					(BR_TagBase + BR_TagString + 3203)
#define CFA_Top						(BR_TagBase                + 3204)
#define CFA_Bottom					(BR_TagBase                + 3205)
#define CFA_Up						(BR_TagBase                + 3206)
#define CFA_Down					(BR_TagBase                + 3207)
#define CFA_SortFAreaList			(BR_TagBase                + 3208)


/*  File fields  */
#define FILE_Name					0
#define FILE_Date					1
#define FILE_Size					2
#define FILE_Description			3
#define FILE_Downloads				4


/*  file tags  */
#define BRFILE_Name					(BR_FileTags + BR_TagString    + FILE_Name)
#define BRFILE_Date					(BR_FileTags                   + FILE_Date)
#define BRFILE_Size					(BR_FileTags                   + FILE_Size)
#define BRFILE_Description			(BR_FileTags + BR_TagStringArr + FILE_Description)
#define BRFILE_Downloads			(BR_FileTags                   + FILE_Downloads)


/*  Tags understood by WriteBRFile()  */

#define WBRF_UpdateFileNr 			(BR_TagBase + 3300)
#define WBRF_DeleteFile				(BR_TagBase + 3301)


/*  Tags understood by ReadBRFile()  */

#define RBRF_FileTagsPtr			(BR_TagBase + BR_TagVarPar + 3400)
#define RBRF_FileDate				(BR_TagBase + BR_TagVarPar + 3401)
#define RBRF_Flags					(BR_TagBase + BR_TagVarPar + 3402)
#define RBRF_NextFile				(BR_TagBase + BR_TagVarPar + 3403)


/*  Tags understood by SearchBRFile()  */

#define SBRF_SearchFAreaList		(BR_TagBase + BR_TagVarPar + 3500)
#define SBRF_SearchFArea			(BR_TagBase + BR_TagVarPar + 3501)
#define SBRF_SearchStr				(BR_TagBase + BR_TagString + 3502)
#define SBRF_SearchName				(BR_TagBase                + 3503)
#define SBRF_SearchAll				(BR_TagBase                + 3504)
#define SBRF_NewerThan				(BR_TagBase                + 3505)


/*  Tags understood by several functions  */

#define BR_ProgressHook				(BR_TagBase + BR_TagVarPar + 3600)
#define BR_ProgressUpdates			(BR_TagBase                + 3601)
#define BRCFG_Use					(BR_TagBase                + 3602)
#define BRCFG_LastSaved				(BR_TagBase                + 3603)
#define BR_ProgressReturn			(BR_TagBase + BR_TagVarPar + 3604)


/* Tags understood by FindDupBRMsg()  */

#define FDBRM_DeleteDupInConf		(BR_TagBase                + 3700)
#define FDBRM_UnMarkCrossPosts		(BR_TagBase                + 3701)


/* Tags understood by UniqueMsgFile()	*/

#define UMF_Extension				(BR_TagBase + BR_TagString + 3800)
#define UMF_UseTag					(BR_TagBase                + 3801)
#define UMF_Event					(BR_TagBase                + 3802)


/* Tags understood by ExternalBBSConfig()	*/

#define EBC_Conference				(BR_TagBase + BR_TagVarPar + 3900)
#define EBC_PublicScreen			(BR_TagBase + BR_TagString + 3901)


/* Tags understood by PGPBREvents()	*/

#define PGP_PublicScreen			(BR_TagBase + BR_TagString + 4000)


/* Tags understood by GetConfigValue() */

#define GCV_GlobalConfig			(BR_TagBase + BR_TagVarPar + 4100)
#define GCV_TypeListItem			(BR_TagBase + BR_TagVarPar + 4101)
#define GCV_BBSListItem				(BR_TagBase + BR_TagVarPar + 4102)
#define GCV_ConfListItem			(BR_TagBase + BR_TagVarPar + 4103)
#define GCV_EventType				(BR_TagBase                + 4104)
#define GCV_ConfQuoteType			(BR_TagBase + BR_TagVarPar + 4105)
#define GCV_ConfQuoteChars			(BR_TagBase + BR_TagVarPar + 4106)		
#define GCV_ConfQuoteReflow			(BR_TagBase + BR_TagVarPar + 4107)
#define GCV_BBSQuoteType			(BR_TagBase + BR_TagVarPar + 4108)
#define GCV_BBSQuoteChars			(BR_TagBase + BR_TagVarPar + 4109)
#define GCV_BBSQuoteReflow			(BR_TagBase + BR_TagVarPar + 4110)
#define GCV_ConfReplyString			(BR_TagBase + BR_TagVarPar + 4111)
#define GCV_BBSReplyString			(BR_TagBase + BR_TagVarPar + 4112)
#define GCV_ConfEnterScript			(BR_TagBase + BR_TagVarPar + 4113)
#define GCV_ConfLeaveScript			(BR_TagBase + BR_TagVarPar + 4114)
#define GCV_BBSEnterScript			(BR_TagBase + BR_TagVarPar + 4115)
#define GCV_BBSLeaveScript			(BR_TagBase + BR_TagVarPar + 4116)

#if defined(__GNUC__)
# pragma pack()
#endif

#endif /* LIBRARIES_BBSREADTAGS_H */
