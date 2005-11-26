#ifndef PRAGMAS_CODESETS_PRAGMAS_H
#define PRAGMAS_CODESETS_PRAGMAS_H

/*
**  $VER: codesets_pragmas.h 2.1 (20.11.2004)
**  Includes Release 2.1
**
**  Written by Alfonso [alfie] Ranieri <alforan@tin.it>.
**
**  Released under the terms of the
**  GNU Public Licence version 2.
*/

#ifndef CLIB_CODESETS_PROTOS_H
#include <clib/codesets_protos.h>
#endif

#if defined(AZTEC_C) || defined(__MAXON__) || defined(__STORM__)
#pragma amicall(CodesetsBase,0x024,CodesetsConvertUTF32toUTF16(a0,a1,a2,a3,d0))
#pragma amicall(CodesetsBase,0x02a,CodesetsConvertUTF16toUTF32(a0,a1,a2,a3,d0))
#pragma amicall(CodesetsBase,0x030,CodesetsConvertUTF16toUTF8(a0,a1,a2,a3,d0))
#pragma amicall(CodesetsBase,0x036,CodesetsIsLegalUTF8(a0,d0))
#pragma amicall(CodesetsBase,0x03c,CodesetsIsLegalUTF8Sequence(a0,a1))
#pragma amicall(CodesetsBase,0x042,CodesetsConvertUTF8toUTF16(a0,a1,a2,a3,d0))
#pragma amicall(CodesetsBase,0x048,CodesetsConvertUTF32toUTF8(a0,a1,a2,a3,d0))
#pragma amicall(CodesetsBase,0x04e,CodesetsConvertUTF8toUTF32(a0,a1,a2,a3,d0))
#pragma amicall(CodesetsBase,0x054,CodesetsSetDefaultA(a0,a1))
#pragma amicall(CodesetsBase,0x05a,CodesetsFreeA(a0,a1))
#pragma amicall(CodesetsBase,0x060,CodesetsSupportedA(a0))
#pragma amicall(CodesetsBase,0x066,CodesetsFindA(a0,a1))
#pragma amicall(CodesetsBase,0x06c,CodesetsFindBestA(a0,d0,a1,a2))
#pragma amicall(CodesetsBase,0x072,CodesetsUTF8Len(a0))
#pragma amicall(CodesetsBase,0x078,CodesetsUTF8ToStrA(a0))
#pragma amicall(CodesetsBase,0x07e,CodesetsUTF8CreateA(a0))
#pragma amicall(CodesetsBase,0x084,CodesetsEncodeB64A(a0))
#pragma amicall(CodesetsBase,0x08a,CodesetsDecodeB64A(a0))
#pragma amicall(CodesetsBase,0x090,CodesetsStrLenA(a0,a1))
#pragma amicall(CodesetsBase,0x096,CodesetsIsValidUTF8(a0))
#pragma amicall(CodesetsBase,0x09c,CodesetsFreeVecPooledA(a0,a1,a2))
#endif
#if defined(_DCC) || defined(__SASC)
#pragma  libcall CodesetsBase CodesetsConvertUTF32toUTF16 024 0ba9805
#pragma  libcall CodesetsBase CodesetsConvertUTF16toUTF32 02a 0ba9805
#pragma  libcall CodesetsBase CodesetsConvertUTF16toUTF8 030 0ba9805
#pragma  libcall CodesetsBase CodesetsIsLegalUTF8    036 0802
#pragma  libcall CodesetsBase CodesetsIsLegalUTF8Sequence 03c 9802
#pragma  libcall CodesetsBase CodesetsConvertUTF8toUTF16 042 0ba9805
#pragma  libcall CodesetsBase CodesetsConvertUTF32toUTF8 048 0ba9805
#pragma  libcall CodesetsBase CodesetsConvertUTF8toUTF32 04e 0ba9805
#pragma  libcall CodesetsBase CodesetsSetDefaultA    054 9802
#pragma  libcall CodesetsBase CodesetsFreeA          05a 9802
#pragma  libcall CodesetsBase CodesetsSupportedA     060 801
#pragma  libcall CodesetsBase CodesetsFindA          066 9802
#pragma  libcall CodesetsBase CodesetsFindBestA      06c a90804
#pragma  libcall CodesetsBase CodesetsUTF8Len        072 801
#pragma  libcall CodesetsBase CodesetsUTF8ToStrA     078 801
#pragma  libcall CodesetsBase CodesetsUTF8CreateA    07e 801
#pragma  libcall CodesetsBase CodesetsEncodeB64A     084 801
#pragma  libcall CodesetsBase CodesetsDecodeB64A     08a 801
#pragma  libcall CodesetsBase CodesetsStrLenA        090 9802
#pragma  libcall CodesetsBase CodesetsIsValidUTF8    096 801
#pragma  libcall CodesetsBase CodesetsFreeVecPooledA 09c a9803
#endif
#ifdef __STORM__
#pragma tagcall(CodesetsBase,0x054,CodesetsSetDefault(a0,a1))
#pragma tagcall(CodesetsBase,0x05a,CodesetsFree(a0,a1))
#pragma tagcall(CodesetsBase,0x060,CodesetsSupported(a0))
#pragma tagcall(CodesetsBase,0x066,CodesetsFind(a0,a1))
#pragma tagcall(CodesetsBase,0x06c,CodesetsFindBest(a0,d0,a1,a2))
#pragma tagcall(CodesetsBase,0x078,CodesetsUTF8ToStr(a0))
#pragma tagcall(CodesetsBase,0x07e,CodesetsUTF8Create(a0))
#pragma tagcall(CodesetsBase,0x084,CodesetsEncodeB64(a0))
#pragma tagcall(CodesetsBase,0x08a,CodesetsDecodeB64(a0))
#pragma tagcall(CodesetsBase,0x090,CodesetsStrLen(a0,a1))
#pragma tagcall(CodesetsBase,0x09c,CodesetsFreeVecPooled(a0,a1,a2))
#endif
#ifdef __SASC_60
#pragma  tagcall CodesetsBase CodesetsSetDefault     054 9802
#pragma  tagcall CodesetsBase CodesetsFree           05a 9802
#pragma  tagcall CodesetsBase CodesetsSupported      060 801
#pragma  tagcall CodesetsBase CodesetsFind           066 9802
#pragma  tagcall CodesetsBase CodesetsFindBest       06c a90804
#pragma  tagcall CodesetsBase CodesetsUTF8ToStr      078 801
#pragma  tagcall CodesetsBase CodesetsUTF8Create     07e 801
#pragma  tagcall CodesetsBase CodesetsEncodeB64      084 801
#pragma  tagcall CodesetsBase CodesetsDecodeB64      08a 801
#pragma  tagcall CodesetsBase CodesetsStrLen         090 9802
#pragma  tagcall CodesetsBase CodesetsFreeVecPooled  09c a9803
#endif

#endif /* PRAGMAS_CODESETS_PRAGMAS_H */
