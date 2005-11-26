#ifndef CLIB_CMANAGER_PROTOS_H
#define CLIB_CMANAGER_PROTOS_H

/*
**  $VER: CManager_protos.h 25.2 (7.7.2003)
**  Includes Release 25.2
**
**  C prototypes. For use with 32 bit integers only.
**
**  Written by Simone Tellini <wiz@vapor.com> and
**             Alfonso Ranieri <alforan@tin.it>.
**
**  Released under the terms of the
**  GNU Public Licence version 2
*/

#ifndef LIBRARIES_CMANAGER_H
#include <libraries/CManager.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Database */
ULONG CM_LoadDataA ( struct TagItem *attrs );
#if !defined(USE_INLINE_STDARG)
ULONG CM_LoadData ( Tag , ... );
#endif
void CM_SaveDataA ( struct TagItem *attrs );
#if !defined(USE_INLINE_STDARG)
void CM_SaveData ( Tag , ... );
#endif
struct CMData *CM_AllocCMData( void );
struct CMGroup  *CM_GetParent( struct CMGroup *, struct CMGroup * );
void CM_FreeData( struct CMData * );
APTR CM_AllocEntry( ULONG );
void CM_FreeEntry( APTR );
APTR CM_GetEntry( APTR, ULONG );
ULONG CM_AddEntry( APTR );
void CM_FreeList( struct MinList * );

/* Application */
APTR CM_StartManager( STRPTR, STRPTR );
void CM_FreeHandle( APTR, ULONG );
APTR CM_StartCManagerA( struct TagItem *);
#if !defined(USE_INLINE_STDARG)
APTR CM_StartCManager( Tag , ... );
#endif

/* Various */
STRPTR CM_GetString( ULONG ID );
ULONG CM_GetOption(ULONG option);
APTR CM_AllocObject( ULONG type);
void CM_FreeObject( APTR object );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CLIB_CMANAGER_PROTOS_H */
