#ifndef CLIB_CMANAGER_PROTOS_H
#define CLIB_CMANAGER_PROTOS_H

/*
**  $VER: CManager_protos.h 33.3 (2.4.2005)
**  Includes Release 33.3
**
**  C prototypes. For use with 32 bit integers only.
**
**  (C) Simone Tellini <wiz@vapor.com> and
**      Alfonso Ranieri <alforan@tin.it>.
*/

#ifndef LIBRARIES_CMANAGER_H
#include <libraries/CManager.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Database */
ULONG CM_LoadDataA ( struct TagItem *attrs );
ULONG CM_LoadData ( Tag , ... );
void CM_SaveDataA ( struct TagItem *attrs );
void CM_SaveData ( Tag , ... );
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
APTR CM_StartCManager( Tag , ... );

/* Various */
STRPTR CM_GetString( ULONG ID );
ULONG CM_GetOption(ULONG option);
APTR CM_AllocObject( ULONG type);
void CM_FreeObject( APTR object );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLIB_CMANAGER_PROTOS_H */
