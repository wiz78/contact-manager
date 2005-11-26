#ifndef	CLIB_BBSREAD_PROTOS_H
#define	CLIB_BBSREAD_PROTOS_H
/*
** Protos for bbsread.library
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

void FreeBRObject( void * );
struct List * GetBBSList( void );
struct BBSListItem * ConfigBBS( struct BBSListItem *, struct TagItem * );
struct BBSListItem * ConfigBBSTags( struct BBSListItem *, ULONG, ... );
struct List * GetConfList( struct BBSListItem * );
struct ConfListItem * ConfigConf( struct ConfListItem *, struct TagItem * );
struct ConfListItem * ConfigConfTags( struct ConfListItem *, ULONG, ... );
BOOL CharsetConvert( UBYTE, UBYTE, STRPTR, STRPTR, ULONG );
BOOL MarkMessage( struct ConfListItem *, struct TagItem * );
BOOL MarkMessageTags( struct ConfListItem *, ULONG, ... );
ULONG * GetMarkedMsg( struct ConfListItem *, ULONG *, ULONG, ULONG );
struct List * GetTypeList(void);
struct TypeListItem * ConfigType( struct TypeListItem *, struct TagItem * );
struct TypeListItem * ConfigTypeTags( struct TypeListItem *, ULONG, ... );
BOOL UpdateEvent( struct EventData *, struct TagItem * );
BOOL UpdateEventTags( struct EventData *, ULONG, ... );
struct TagItem * PackDataFile( struct TagItem * );
struct TagItem * PackDataFileTags( ULONG, ... );
struct GlobalConfig *GetGlobalConfig(void);
BOOL ConfigGlobal( struct GlobalConfig *, struct TagItem * );
BOOL ConfigGlobalTags( struct GlobalConfig *, ULONG, ... );
struct TypeListItem * TypeFromBBS( struct BBSListItem * );
LONG UnArchive( STRPTR, struct TagItem * );
LONG UnArchiveTags( STRPTR, ULONG, ...);
struct MinList * ScanForGrabs( void );
LONG ParseGrab( struct Node *, struct TagItem * );
LONG ParseGrabTags( struct Node *, ULONG, ... );
void StartOfAdding( struct BBSListItem * );
void EndOfAdding( struct BBSListItem * );
struct TagItem * UpdateDataStruct( struct TagItem * );
struct TagItem * UpdateDataStructTags( ULONG, ... );
STRPTR GetSignature(struct GlobalConfig *, struct BBSListItem *, struct ConfListItem *);
LONG Archive( struct ArcConfigItem *, STRPTR, struct TagItem * );
LONG ArchiveTags( struct ArcConfigItem *, STRPTR, ULONG, ...);
LONG MakeEventPackage( struct BBSListItem *, struct TagItem * );
LONG MakeEventPackageTags( struct BBSListItem *, ULONG, ... );
ULONG FindOrginalNr( struct ConfListItem *, ULONG );
ULONG DeleteDuplicates(struct BBSListItem *);
struct SearchResult * SearchBRMessage( struct ConfListItem *, struct TagItem * );
struct SearchResult * SearchBRMessageTags( struct ConfListItem *, ULONG, ... );
ULONG WriteBRMessage( struct ConfListItem *, struct TagItem * );
ULONG WriteBRMessageTags( struct ConfListItem *, ULONG, ... );
APTR ReadBRMessage( struct ConfListItem *, ULONG, struct TagItem * );
APTR ReadBRMessageTags( struct ConfListItem *, ULONG, ULONG, ... );
UBYTE ConfCharset(struct BBSListItem *, struct ConfListItem *);
UWORD ConfLineLength(struct ConfListItem *);
ULONG WriteBREvent( struct BBSListItem *, ULONG, struct TagItem * );
ULONG WriteBREventTags( struct BBSListItem *, ULONG, ULONG, ... );
APTR ReadBREvent( struct BBSListItem *, ULONG, struct Tagitem * );
APTR ReadBREventTags( struct BBSListItem *, ULONG, ULONG, ... );
BOOL UpdateBRMessage( struct ConfListItem *, ULONG, struct TagItem * );
BOOL UpdateBRMessageTags( struct ConfListItem *, ULONG, ULONG, ... );
BOOL UpdateBREvent( struct BBSListItem *, ULONG, struct TagItem * );
BOOL UpdateBREventTags( struct BBSListItem *, ULONG, ULONG, ... );
ULONG WriteBRUser( struct BBSListItem *, struct TagItem * );
ULONG WriteBRUserTags( struct BBSListItem *, ULONG, ... );
APTR ReadBRUser( struct BBSListItem *, ULONG, struct Tagitem * );
APTR ReadBRUserTags( struct BBSListItem *, ULONG, ULONG, ... );
struct SUserResult * SearchBRUser( struct BBSListItem *, struct TagItem * );
struct SUserResult * SearchBRUserTags( struct BBSListItem *, ULONG, ... );
ULONG WriteBRKill( struct BBSListItem *, struct TagItem * );
ULONG WriteBRKillTags( struct BBSListItem *, ULONG, ... );
APTR ReadBRKill( struct BBSListItem *, ULONG, struct Tagitem * );
APTR ReadBRKillTags( struct BBSListItem *, ULONG, ULONG, ... );
struct List * GetFAreaList( struct BBSListItem * );
struct FAreaListItem * ConfigFArea( struct FAreaListItem *, struct TagItem *);
struct FAreaListItem * ConfigFAreaTags( struct FAreaListItem *, ULONG, ... );
ULONG WriteBRFile( struct FAreaListItem *, struct TagItem * );
ULONG WriteBRFileTags( struct FAreaListItem *, ULONG, ... );
APTR ReadBRFile( struct FAreaListItem *, ULONG, struct Tagitem * );
APTR ReadBRFileTags( struct FAreaListItem *, ULONG, ULONG, ... );
struct SFileResult * SearchBRFile( struct TagItem * );
struct SFileResult * SearchBRFileTags( ULONG, ... );
struct UserData * BBSUserData( struct GlobalConfig *, struct BBSListItem * );
STRPTR GetTagFile( struct GlobalConfig *, struct BBSListItem *, struct ConfListItem * );
struct ArcConfigItem * BBSEventArchiver( struct BBSListItem * );
APTR BufBROpen( STRPTR, LONG );
BOOL BufBRClose( APTR );
LONG BufBRRead( APTR, APTR, LONG );
LONG BufBRWrite( APTR, APTR, LONG );
LONG BufBRSeek(APTR, LONG, LONG);
BOOL WriteBRIEFMsg( APTR, struct ConfListItem *, ULONG );
BOOL FindDupBRMsg( struct BBSListItem *, struct TagItem * );
BOOL FindDupBRMsgTags( struct BBSListItem *, ULONG, ... );
BOOL WritePassiveConfList( struct BBSListItem *, struct List * );
struct List * ReadPassiveConfList( struct BBSListItem * );
STRPTR UniqueMsgFile( struct BBSListItem *, STRPTR *, struct TagItem * );
STRPTR UniqueMsgFileTags( struct BBSListItem *, STRPTR *, ULONG, ... );
LONG ExternalBBSConfig( struct BBSListItem *, struct TagItem * );
LONG ExternalBBSConfigTags(struct BBSListItem *, ULONG, ... );
BOOL AppendPassiveConfList( struct BBSListItem *, struct List * );
ULONG PGPBREvents( struct BBSListItem *, struct TagItem * );
ULONG PGPBREventsTags(struct BBSListItem *, ULONG, ... );
BOOL GetConfigValue( struct TagItem * );
BOOL GetConfigValueTags( ULONG, ... );
ULONG SortMessageArray(struct ConfListItem *conf, ULONG *usebuf, ULONG method);

#endif  /* CLIB_BBSREAD_PROTOS_H */
