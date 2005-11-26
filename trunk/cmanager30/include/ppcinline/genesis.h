#ifndef _PPCINLINE_GENESIS_H
#define _PPCINLINE_GENESIS_H

#ifndef CLIB_GENESIS_PROTOS_H
#define CLIB_GENESIS_PROTOS_H
#endif

#ifndef __PPCINLINE_MACROS_H
#include <ppcinline/macros.h>
#endif

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef GENESIS_BASE_NAME
#define GENESIS_BASE_NAME GenesisBase
#endif

#define GetFileSize(file) \
	LP1(0x1e, LONG, GetFileSize, STRPTR, file, a0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define ParseConfig(file, data) \
	LP2(0x24, BOOL, ParseConfig, STRPTR, file, a0, struct ParseConfig_Data *, data, a1, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define ParseNext(data) \
	LP1(0x2a, BOOL, ParseNext, struct ParseConfig_Data *, data, a0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define ParseNextLine(data) \
	LP1(0x30, BOOL, ParseNextLine, struct ParseConfig_Data *, data, a0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define ParseEnd(data) \
	LP1NR(0x36, ParseEnd, struct ParseConfig_Data *, data, a0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define ReallocCopy(old, src) \
	LP2(0x3c, STRPTR, ReallocCopy, STRPTR *, old, a0, STRPTR, src, a1, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define FreeUser(user) \
	LP1NR(0x42, FreeUser, struct User *, user, a0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define GetUserName(id, buffer, len) \
	LP3(0x48, BOOL, GetUserName, LONG, id, d0, char *, buffer, a0, LONG, len, d1, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define GetUser(name, password, title, flags) \
	LP4(0x4e, struct User *, GetUser, STRPTR, name, a0, STRPTR, password, a1, STRPTR, title, a2, LONG, flags, d0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define GetGlobalUser() \
	LP0(0x54, struct User *, GetGlobalUser, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define SetGlobalUser(user) \
	LP1NR(0x5a, SetGlobalUser, struct User *, user, a0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define ClearUserList() \
	LP0NR(0x60, ClearUserList, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define ReloadUserList() \
	LP0(0x66, BOOL, ReloadUserList, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define ReadFile(file, buffer, len) \
	LP3(0x6c, LONG, ReadFile, STRPTR, file, a0, STRPTR, buffer, a1, LONG, len, d0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define WriteFile(file, buffer, len) \
	LP3(0x72, BOOL, WriteFile, STRPTR, file, a0, STRPTR, buffer, a1, LONG, len, d0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define IsOnline(flags) \
	LP1(0x78, BOOL, IsOnline, LONG, flags, d0, \
	, GENESIS_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#endif /*  _PPCINLINE_GENESIS_H  */
