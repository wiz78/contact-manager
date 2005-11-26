#ifndef _LIB_PROTOS_H
#define _LIB_PROTOS_H

/* init.c */
void freeBase ( void );
ULONG initBase ( void );

/* loc.c */
void initStrings ( void );
#ifdef __MORPHOS__
STRPTR CM_GetString ( ULONG id );
STRPTR LIB_GetString ( void );
#else
STRPTR SAVEDS ASM CM_GetString ( REG(d0,ULONG id ));
#endif

/* utils.c */
#ifdef __MORPHOS__
void sprintf ( STRPTR buf , STRPTR fmt , ...);
#else
void STDARGS sprintf ( STRPTR to , STRPTR fmt , ...);
#endif
APTR allocVecPooled ( APTR pool , ULONG size );
void freeVecPooled ( APTR pool , APTR mem );
APTR allocArbitratePooled ( ULONG size );
APTR freeArbitratePooled ( APTR mem , ULONG size );
APTR allocArbitrateVecPooled ( ULONG size );
APTR freeArbitrateVecPooled ( APTR mem );
ULONG userCodeset ( STRPTR user , STRPTR cbuf , ULONG clen );
int countNodes ( struct MinList *List );

/* CManager.c */
#ifdef __MORPHOS__
APTR CM_AllocEntry ( ULONG type );
APTR LIB_AllocEntry ( void );
void CM_FreeEntry ( APTR entry );
void LIB_FreeEntry ( void );
void CM_FreeObject ( APTR entry );
void LIB_FreeObject ( void );
void CM_FreeList ( struct MinList *list );
void LIB_FreeList ( void );
void CM_FreeData ( struct CMData *data );
void LIB_FreeData ( void );
struct CMData *CM_AllocCMData ( void );
struct CMData *LIB_AllocCMData ( void );
ULONG checkStack ( void );
void CM_FreeHandle ( APTR handle , ULONG close );
void LIB_FreeHandle ( void );
APTR CM_StartCManagerA ( struct TagItem *attrs );
APTR LIB_StartCManagerA ( struct TagItem *attrs );
APTR CM_StartManager ( STRPTR file , STRPTR pubScreen );
APTR LIB_StartManager ( void );
APTR CM_GetEntry ( APTR handle , ULONG flags );
APTR LIB_GetEntry ( void );
struct CMGroup *CM_GetParent ( struct CMGroup *list , struct CMGroup *current );
struct CMGroup *LIB_GetParent ( void );
APTR CM_AllocObject ( ULONG type );
APTR LIB_AllocObject ( void );
ULONG CM_GetOption ( ULONG option );
ULONG LIB_GetOption ( void );
ULONG CM_LoadDataA ( struct TagItem *attrs );
ULONG LIB_LoadDataA ( void );
ULONG CM_SaveDataA ( struct TagItem *attrs );
ULONG LIB_SaveDataA ( void );
ULONG CM_AddEntry ( APTR entry );
ULONG LIB_AddEntry ( void );
#else
APTR SAVEDS ASM CM_AllocEntry ( REG (d0,ULONG type) );
void SAVEDS ASM CM_FreeEntry ( REG (a0,APTR entry) );
void SAVEDS ASM CM_FreeObject ( REG (a0,APTR entry) );
void SAVEDS ASM CM_FreeList ( REG (a0,struct MinList *list) );
void SAVEDS ASM CM_FreeData ( REG (a0,struct CMData *data) );
struct CMData *SAVEDS ASM CM_AllocCMData ( void );
void SAVEDS ASM CM_FreeHandle ( REG (a0,APTR handle) , REG (d0,ULONG close ));
APTR SAVEDS ASM CM_StartCManagerA ( REG (a0 ,struct TagItem *attrs) );
APTR SAVEDS ASM CM_StartManager ( REG (a0,STRPTR file) , REG (a1,STRPTR pubScreen) );
APTR SAVEDS ASM CM_GetEntry ( REG (a0 ,APTR handle) , REG (d0,ULONG flags) );
struct CMGroup *SAVEDS ASM CM_GetParent ( REG (a0,struct CMGroup *list) , REG (a1,struct CMGroup *current) );
APTR SAVEDS ASM CM_AllocObject ( REG (d0 ,ULONG type) );
ULONG SAVEDS ASM CM_GetOption ( REG (d0,ULONG option) );
ULONG SAVEDS ASM CM_LoadDataA ( REG (a0,struct TagItem *attrs) );
ULONG SAVEDS ASM CM_SaveDataA ( REG (a0,struct TagItem *attrs) );
ULONG SAVEDS ASM CM_AddEntry ( REG (a0,APTR entry) );
#endif

/* obsolete.c */
#ifdef __MORPHOS__
ULONG OBSOLETE_CM_LoadData(void);
ULONG OBSOLETE_CM_LoadDataOld(void);
void OBSOLETE_CM_SaveData(void);
void OBSOLETE_CM_SaveDataOld(void);
void OBSOLETE_CM_FreeDataOld(void);
struct CMData * OBSOLETE_CM_LoadCurrentUserData(void);
void OBSOLETE_CM_DeleteBitMap(void);
struct BitMap * OBSOLETE_CM_CreateBitMap(void);
#endif

#endif

