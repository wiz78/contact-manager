#ifndef _PPCINLINE_CMANAGER_H
#define _PPCINLINE_CMANAGER_H

#ifndef __PPCINLINE_MACROS_H
#include <ppcinline/macros.h>
#endif /* !__PPCINLINE_MACROS_H */

#ifndef __PPCINLINE_MACROS_H
#include <ppcinline/macros.h>
#endif

#ifndef CMANAGER_BASE_NAME
#define CMANAGER_BASE_NAME CManagerBase
#endif

#define CM_StartManager(file, pubscreen) \
	LP2(0x1e, APTR, CM_StartManager, STRPTR, file, a0, STRPTR, pubscreen, a1, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_GetParent(list, current) \
	LP2(0x36, struct CMGroup  *, CM_GetParent, struct CMGroup *, list, a0, struct CMGroup *, current, a1, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_AllocEntry(type) \
	LP1(0x3c, APTR, CM_AllocEntry, ULONG, type, d0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_FreeEntry(entry) \
	LP1NR(0x42, CM_FreeEntry, APTR, entry, a0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_FreeHandle(handle, close) \
	LP2NR(0x48, CM_FreeHandle, APTR, handle, a0, BOOL, close, d0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_GetEntry(handle, flags) \
	LP2(0x4e, APTR, CM_GetEntry, APTR, handle, a0, ULONG, flags, d0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_AddEntry(entry) \
	LP1(0x60, BOOL, CM_AddEntry, APTR, entry, a0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_FreeList(list) \
	LP1NR(0x66, CM_FreeList, struct MinList *, list, a0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_FreeData(data) \
	LP1NR(0x7e, CM_FreeData, struct CMData *, data, a0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_AllocCMData() \
	LP0(0x84, struct CMData *, CM_AllocCMData, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_GetString(id) \
	LP1(0x8a, STRPTR, CM_GetString, ULONG, id, d0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_GetOption(id) \
	LP1(0x90, ULONG, CM_GetOption, ULONG, id, d0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_StartCManagerA(attrs) \
	LP1(0x96, APTR, CM_StartCManagerA, struct TagItem *, attrs, a0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_AllocObject(type) \
	LP1(0x9c, APTR, CM_AllocObject, ULONG, type, d0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_FreeObject(type) \
	LP1NR(0xa2, CM_FreeObject, APTR, type, a0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_LoadDataA(attrs) \
	LP1(0xa8, ULONG, CM_LoadDataA, struct TagItem *, attrs, a0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#define CM_SaveDataA(attrs) \
	LP1NR(0xae, CM_SaveDataA, struct TagItem *, attrs, a0, \
	, CMANAGER_BASE_NAME, IF_CACHEFLUSHALL, NULL, 0, IF_CACHEFLUSHALL, NULL, 0)

#ifdef USE_INLINE_STDARG

#include <stdarg.h>

#define CM_StartCManager(...) \
	({ULONG _tags[] = {__VA_ARGS__}; CM_StartCManagerA((struct TagItem *) _tags);})

#define CM_LoadData(...) \
	({ULONG _tags[] = {__VA_ARGS__}; CM_LoadDataA((struct TagItem *) _tags);})

#define CM_SaveData(...) \
	({ULONG _tags[] = {__VA_ARGS__}; CM_SaveDataA((struct TagItem *) _tags);})

#endif

#endif /*  _PPCINLINE_CMANAGER_H  */
