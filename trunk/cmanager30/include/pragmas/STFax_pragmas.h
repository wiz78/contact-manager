#ifndef PRAGMAS_STFAX_PRAGMAS_H
#define PRAGMAS_STFAX_PRAGMAS_H

#ifndef CLIB_STFAX_PROTOS_H
#include <clib/stfax_protos.h>
#endif

#if defined(AZTEC_C) || defined(__MAXON__) || defined(__STORM__)
#pragma amicall(STFaxBase,0x01e,STF_FreePhonebook(a0))
#pragma amicall(STFaxBase,0x024,STF_LoadPhonebook(a0,a1))
#pragma amicall(STFaxBase,0x02a,STF_SavePhonebook(a0,a1))
#pragma amicall(STFaxBase,0x030,STF_SaveScript(a0,a1))
#pragma amicall(STFaxBase,0x036,STF_LoadScript(a0,a1))
#pragma amicall(STFaxBase,0x03c,STF_FreeScript(a0))
#pragma amicall(STFaxBase,0x042,STF_AllocScriptCmd(d0))
#pragma amicall(STFaxBase,0x048,STF_FreeScriptCmd(a0))
#pragma amicall(STFaxBase,0x04e,STF_GetCommandName(d0))
#pragma amicall(STFaxBase,0x054,STF_AllocVoiceBox())
#pragma amicall(STFaxBase,0x05a,STF_FreeVoiceBox(a0,d0))
#pragma amicall(STFaxBase,0x060,STF_StripNumber(a0,a1))
#pragma amicall(STFaxBase,0x066,STF_FindNumber(a0,a1))
#pragma amicall(STFaxBase,0x06c,STF_ReadFax(a0,a1))
#pragma amicall(STFaxBase,0x072,STF_FreeFax(a0))
#pragma amicall(STFaxBase,0x078,STF_SaveFax(a0,a1,d0))
#pragma amicall(STFaxBase,0x07e,STF_FaxOpen(a0,a1))
#pragma amicall(STFaxBase,0x084,STF_BeginPage(a0,d0,d1))
#pragma amicall(STFaxBase,0x08a,STF_EndPage(a0))
#pragma amicall(STFaxBase,0x090,STF_FaxClose(a0))
#pragma amicall(STFaxBase,0x096,STF_EncodeLine(a0,a1,d0))
#pragma amicall(STFaxBase,0x09c,STF_Decode_BeginPage(a0))
#pragma amicall(STFaxBase,0x0a2,STF_Decode_EndPage(a0))
#pragma amicall(STFaxBase,0x0a8,STF_CountLines(a0))
#pragma amicall(STFaxBase,0x0ae,STF_DecodeLine(a0,a1))
#pragma amicall(STFaxBase,0x0b4,STF_CreateBitMap(d0,d1,d2,d3,a0))
#pragma amicall(STFaxBase,0x0ba,STF_DeleteBitMap(a0))
#pragma amicall(STFaxBase,0x0c0,STF_InvertPlane(a0,d0,d1,d2))
#pragma amicall(STFaxBase,0x0c6,STF_WritePageText(a0,a1))
#pragma amicall(STFaxBase,0x0cc,STF_GetString(d0))
#pragma amicall(STFaxBase,0x0d2,STF_LoadIFF(a0,a1))
#pragma amicall(STFaxBase,0x0d8,STF_LoadImage(a0))
#pragma amicall(STFaxBase,0x0de,STF_FreeImg(a0))
#pragma amicall(STFaxBase,0x0e4,STF_LoadBrush(a0))
#pragma amicall(STFaxBase,0x0ea,STF_FreeBrush(a0))
#pragma amicall(STFaxBase,0x0f0,STF_AllocPBItem(d0))
#pragma amicall(STFaxBase,0x0f6,STF_FreePBItem(a0))
#pragma amicall(STFaxBase,0x0fc,STF_RemovePBEntryRefs(a0,a1))
#endif
#if defined(_DCC) || defined(__SASC)
#pragma  libcall STFaxBase STF_FreePhonebook      01e 801
#pragma  libcall STFaxBase STF_LoadPhonebook      024 9802
#pragma  libcall STFaxBase STF_SavePhonebook      02a 9802
#pragma  libcall STFaxBase STF_SaveScript         030 9802
#pragma  libcall STFaxBase STF_LoadScript         036 9802
#pragma  libcall STFaxBase STF_FreeScript         03c 801
#pragma  libcall STFaxBase STF_AllocScriptCmd     042 001
#pragma  libcall STFaxBase STF_FreeScriptCmd      048 801
#pragma  libcall STFaxBase STF_GetCommandName     04e 001
#pragma  libcall STFaxBase STF_AllocVoiceBox      054 00
#pragma  libcall STFaxBase STF_FreeVoiceBox       05a 0802
#pragma  libcall STFaxBase STF_StripNumber        060 9802
#pragma  libcall STFaxBase STF_FindNumber         066 9802
#pragma  libcall STFaxBase STF_ReadFax            06c 9802
#pragma  libcall STFaxBase STF_FreeFax            072 801
#pragma  libcall STFaxBase STF_SaveFax            078 09803
#pragma  libcall STFaxBase STF_FaxOpen            07e 9802
#pragma  libcall STFaxBase STF_BeginPage          084 10803
#pragma  libcall STFaxBase STF_EndPage            08a 801
#pragma  libcall STFaxBase STF_FaxClose           090 801
#pragma  libcall STFaxBase STF_EncodeLine         096 09803
#pragma  libcall STFaxBase STF_Decode_BeginPage   09c 801
#pragma  libcall STFaxBase STF_Decode_EndPage     0a2 801
#pragma  libcall STFaxBase STF_CountLines         0a8 801
#pragma  libcall STFaxBase STF_DecodeLine         0ae 9802
#pragma  libcall STFaxBase STF_CreateBitMap       0b4 8321005
#pragma  libcall STFaxBase STF_DeleteBitMap       0ba 801
#pragma  libcall STFaxBase STF_InvertPlane        0c0 210804
#pragma  libcall STFaxBase STF_WritePageText      0c6 9802
#pragma  libcall STFaxBase STF_GetString          0cc 001
#pragma  libcall STFaxBase STF_LoadIFF            0d2 9802
#pragma  libcall STFaxBase STF_LoadImage          0d8 801
#pragma  libcall STFaxBase STF_FreeImg            0de 801
#pragma  libcall STFaxBase STF_LoadBrush          0e4 801
#pragma  libcall STFaxBase STF_FreeBrush          0ea 801
#pragma  libcall STFaxBase STF_AllocPBItem        0f0 001
#pragma  libcall STFaxBase STF_FreePBItem         0f6 801
#pragma  libcall STFaxBase STF_RemovePBEntryRefs  0fc 9802
#endif

#endif /* PRAGMAS_STFAX_PRAGMAS_H */
