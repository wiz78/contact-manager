#ifndef _PPCINLINE_STFAX_H
#define _PPCINLINE_STFAX_H

#ifndef CLIB_STFAX_PROTOS_H
#define CLIB_STFAX_PROTOS_H
#endif

#ifndef __PPCINLINE_MACROS_H
#include <ppcinline/macros.h>
#endif

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef  UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

#ifndef STFAX_BASE_NAME
#define STFAX_BASE_NAME STFaxBase
#endif

#define STF_FreePhonebook(pb) \
	LP1NR(0x1e, STF_FreePhonebook, struct MinList *, pb, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_LoadPhonebook(pb, file) \
	LP2(0x24, BOOL, STF_LoadPhonebook, struct MinList *, pb, a0, STRPTR, file, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_SavePhonebook(pb, file) \
	LP2(0x2a, BOOL, STF_SavePhonebook, struct MinList *, pb, a0, STRPTR, file, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_SaveScript(file, list) \
	LP2NR(0x30, STF_SaveScript, STRPTR, file, a0, struct MinList *, list, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_LoadScript(file, list) \
	LP2NR(0x36, STF_LoadScript, STRPTR, file, a0, struct MinList *, list, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FreeScript(list) \
	LP1NR(0x3c, STF_FreeScript, struct MinList *, list, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_AllocScriptCmd(cmd) \
	LP1(0x42, struct ScriptCommand    *, STF_AllocScriptCmd, int, cmd, d0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FreeScriptCmd(cmd) \
	LP1NR(0x48, STF_FreeScriptCmd, void *, cmd, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_GetCommandName(cmd) \
	LP1(0x4e, char *, STF_GetCommandName, int, cmd, d0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_AllocVoiceBox() \
	LP0(0x54, struct VoiceBox         *, STF_AllocVoiceBox, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FreeVoiceBox(box, free) \
	LP2NR(0x5a, STF_FreeVoiceBox, struct VoiceBox *, box, a0, BOOL, free, d0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_StripNumber(dest, src) \
	LP2NR(0x60, STF_StripNumber, UBYTE *, dest, a0, UBYTE *, src, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FindNumber(pb, number) \
	LP2(0x66, struct PBEntry  *, STF_FindNumber, struct MinList *, pb, a0, STRPTR, number, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_ReadFax(name, list) \
	LP2(0x6c, BOOL, STF_ReadFax, STRPTR, name, a0, struct MinList *, list, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FreeFax(list) \
	LP1NR(0x72, STF_FreeFax, struct MinList *, list, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_SaveFax(name, list, icon) \
	LP3(0x78, BOOL, STF_SaveFax, STRPTR, name, a0, struct MinList *, list, a1, BOOL, icon, d0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FaxOpen(file, list) \
	LP2(0x7e, APTR, STF_FaxOpen, STRPTR, file, a0, struct MinList *, list, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_BeginPage(handle, hires, width) \
	LP3(0x84, BOOL, STF_BeginPage, APTR, handle, a0, BOOL, hires, d0, ULONG, width, d1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_EndPage(handle) \
	LP1(0x8a, BOOL, STF_EndPage, APTR, handle, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FaxClose(handle) \
	LP1NR(0x90, STF_FaxClose, APTR, handle, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_EncodeLine(handle, line, width) \
	LP3NR(0x96, STF_EncodeLine, APTR, handle, a0, UBYTE *, line, a1, ULONG, width, d0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_Decode_BeginPage(fax) \
	LP1(0x9c, APTR, STF_Decode_BeginPage, struct FaxPage *, fax, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_Decode_EndPage(fax) \
	LP1NR(0xa2, STF_Decode_EndPage, APTR, fax, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_CountLines(fax) \
	LP1(0xa8, ULONG, STF_CountLines, struct FaxPage *, fax, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_DecodeLine(fax, dest) \
	LP2(0xae, LONG, STF_DecodeLine, APTR, fax, a0, UBYTE *, dest, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_CreateBitMap(width, height, depth, flags, fr) \
	LP5(0xb4, struct BitMap   *, STF_CreateBitMap, ULONG, width, d0, ULONG, height, d1, ULONG, depth, d2, ULONG, flags, d3, struct BitMap *, fr, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_DeleteBitMap(bitmap) \
	LP1NR(0xba, STF_DeleteBitMap, struct BitMap *, bitmap, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_InvertPlane(Plane, Width, Height, BytesPerRow) \
	LP4NR(0xc0, STF_InvertPlane, UBYTE *, Plane, a0, ULONG, Width, d0, ULONG, Height, d1, ULONG, BytesPerRow, d2, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_WritePageText(handle, text) \
	LP2NR(0xc6, STF_WritePageText, APTR, handle, a0, STRPTR, text, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_GetString(id) \
	LP1(0xcc, STRPTR, STF_GetString, ULONG, id, d0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_LoadIFF(file, img) \
	LP2(0xd2, UBYTE           *, STF_LoadIFF, STRPTR, file, a0, struct MyImage *, img, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_LoadImage(file) \
	LP1(0xd8, struct MyImage  *, STF_LoadImage, STRPTR, file, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FreeImg(img) \
	LP1NR(0xde, STF_FreeImg, struct MyImage *, img, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_LoadBrush(file) \
	LP1(0xe4, struct MyBrush  *, STF_LoadBrush, STRPTR, file, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FreeBrush(brush) \
	LP1NR(0xea, STF_FreeBrush, struct MyBrush *, brush, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_AllocPBItem(type) \
	LP1(0xf0, APTR, STF_AllocPBItem, ULONG, type, d0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_FreePBItem(item) \
	LP1NR(0xf6, STF_FreePBItem, APTR, item, a0, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define STF_RemovePBEntryRefs(list, entry) \
	LP2NR(0xfc, STF_RemovePBEntryRefs, struct MinList *, list, a0, struct PBEntry *, entry, a1, \
	, STFAX_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#endif /*  _PPCINLINE_STFAX_H  */
