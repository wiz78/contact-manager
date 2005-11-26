#ifndef CLIB_STFAX_PROTOS_H
#define CLIB_STFAX_PROTOS_H


#ifndef LIBRARIES_STFAXPHONEBOOK_H
#include <libraries/STFaxPhoneBook.h>
#endif

// STFax.c
extern STRPTR STF_GetString(  ULONG ID );


// voice_support.c
extern void STF_SaveScript(  STRPTR ScriptName,  struct MinList *Script );
extern void STF_LoadScript(  STRPTR ScriptName,  struct MinList *Script );
extern void STF_FreeScript(  struct MinList *Script );

extern struct ScriptCommand    *STF_AllocScriptCmd(  int Command );
extern void                     STF_FreeScriptCmd(  void *Cmd );

extern char *STF_GetCommandName(  int Command );

// voice.c
extern struct VoiceBox         *STF_AllocVoiceBox( void );
extern void                     STF_FreeVoiceBox(  struct VoiceBox *box,  BOOL free );

// phonebook.c
extern void             STF_StripNumber(  UBYTE *to,  UBYTE *from );
extern struct PBEntry  *STF_FindNumber(  struct MinList *Numbers,  STRPTR Number );

extern APTR             STF_AllocPBItem(  ULONG type );
extern void             STF_FreePBItem(  APTR item );
extern void             STF_RemovePBEntryRefs(  struct MinList *Numbers,  struct PBEntry *Entry );

extern void             STF_FreePhonebook(  struct MinList *Numbers );
extern BOOL             STF_LoadPhonebook(  struct MinList *Numbers,  STRPTR Phonebook );
extern BOOL             STF_SavePhonebook(  struct MinList *Numbers,  STRPTR Phonebook );

// faxread.c
extern BOOL     STF_ReadFax(  STRPTR Filename,  struct MinList *PageList );
extern void     STF_FreeFax(  struct MinList *Pages );
extern BOOL     STF_SaveFax(  STRPTR save_file,  struct MinList *Fax,  BOOL Icon );

// tofax.c
extern APTR     STF_FaxOpen(  STRPTR,  struct MinList * );
extern BOOL     STF_BeginPage(  APTR,  BOOL,  ULONG );
extern BOOL     STF_EndPage(  APTR );
extern void     STF_WritePageText(  APTR handle,  STRPTR text );
extern void     STF_FaxClose(  APTR );
extern void     STF_EncodeLine(  APTR,  UBYTE *,  ULONG );

// fromfax.c
extern APTR     STF_Decode_BeginPage(  APTR Fax );
extern void     STF_Decode_EndPage(  APTR handle );
extern ULONG    STF_CountLines(  APTR Fax );
extern LONG     STF_DecodeLine(  APTR handle,  UBYTE *bitrow );

// support.c
extern struct BitMap   *STF_CreateBitMap(  ULONG width,  ULONG height,  ULONG depth,  ULONG flags,  struct BitMap *friend );
extern void             STF_DeleteBitMap(  struct BitMap *bm );
extern void             STF_InvertPlane(  UBYTE *Plane, ULONG Width, ULONG Height, ULONG BytesPerRow );

// mybrush.c
extern UBYTE           *STF_LoadIFF(  STRPTR File,  APTR Img );
extern struct MyImage  *STF_LoadImage(  STRPTR File );
extern void             STF_FreeImg(  struct MyImage *Img );
extern struct MyBrush  *STF_LoadBrush(  STRPTR File );
extern void             STF_FreeBrush(  struct MyBrush *Brush );


#endif /* CLIB_STFAX_PROTOS_H */
