
        SECTION CManager,CODE

        NOLIST

        INCLUDE "CManager.library_rev.i"
        INCLUDE "exec/libraries.i"
        INCLUDE "exec/resident.i"
        INCLUDE "exec/initializers.i"
        INCLUDE "exec/lists.i"
        INCLUDE "exec/semaphores.i"

        LIST

        XREF    _LinkerDB
        XREF    _lib_name
        XREF    ENDCODE

        XREF    _initLib
        XREF    _openLib
        XREF    _expungeLib
        XREF    _closeLib
        XREF    _CM_StartManager
        XREF    _OBSOLETE_CM_LoadDataOld
        XREF    _OBSOLETE_CM_SaveDataOld
        XREF    _OBSOLETE_CM_FreeDataOld
        XREF    _CM_GetParent
        XREF    _CM_AllocEntry
        XREF    _CM_FreeEntry
        XREF    _CM_FreeHandle
        XREF    _CM_GetEntry
        XREF    _OBSOLETE_CM_CreateBitMap
        XREF    _OBSOLETE_CM_DeleteBitMap
        XREF    _CM_AddEntry
        XREF    _CM_FreeList
        XREF    _OBSOLETE_CM_LoadCurrentUserData
        XREF    _OBSOLETE_CM_LoadData
        XREF    _OBSOLETE_CM_SaveData
        XREF    _CM_FreeData
        XREF    _CM_AllocCMData
        XREF    _CM_GetString
        XREF    _CM_GetOption
        XREF    _CM_StartCManagerA
        XREF    _CM_AllocObject
        XREF    _CM_FreeObject

        XREF    _CM_LoadDataA
        XREF    _CM_SaveDataA

        XDEF    _ID

PRI     EQU 0

start:  moveq   #-1,d0
        rts

romtag:
        dc.w    RTC_MATCHWORD
        dc.l    romtag
        dc.l    ENDCODE
        dc.b    RTF_AUTOINIT
        dc.b    VERSION
        dc.b    NT_LIBRARY
        dc.b    PRI
        dc.l    _lib_name
        dc.l    _ID
        dc.l    init

_ID:    VSTRING

        CNOP    0,4

init:   dc.l    LIB_SIZE
        dc.l    funcTable
        dc.l    dataTable
        dc.l    _initLib

V_DEF   MACRO
    dc.w    \1+(*-funcTable)
    ENDM

funcTable:
        DC.L   _openLib
        DC.L   _closeLib
        DC.L   _expungeLib
        DC.L   nil

        DC.L   _CM_StartManager
        DC.L   _OBSOLETE_CM_LoadDataOld
        DC.L   _OBSOLETE_CM_SaveDataOld
        DC.L   _OBSOLETE_CM_FreeDataOld
        DC.L   _CM_GetParent
        DC.L   _CM_AllocEntry
        DC.L   _CM_FreeEntry
        DC.L   _CM_FreeHandle
        DC.L   _CM_GetEntry
        DC.L   _OBSOLETE_CM_CreateBitMap
        DC.L   _OBSOLETE_CM_DeleteBitMap
        DC.L   _CM_AddEntry
        DC.L   _CM_FreeList
        DC.L   _OBSOLETE_CM_LoadCurrentUserData
        DC.L   _OBSOLETE_CM_LoadData
        DC.L   _OBSOLETE_CM_SaveData
        DC.L   _CM_FreeData
        DC.L   _CM_AllocCMData
        DC.L   _CM_GetString
        DC.L   _CM_GetOption
        DC.L   _CM_StartCManagerA
        DC.L   _CM_AllocObject
        DC.L   _CM_FreeObject

        DC.L   _CM_LoadDataA
        DC.L   _CM_SaveDataA

        DC.L    -1

dataTable:
        INITBYTE LN_TYPE,NT_LIBRARY
        INITLONG LN_NAME,_lib_name
        INITBYTE LIB_FLAGS,(LIBF_SUMUSED!LIBF_CHANGED)
        INITWORD LIB_VERSION,VERSION
        INITWORD LIB_REVISION,REVISION
        INITLONG LIB_IDSTRING,_ID
        dc.w     0

        CNOP    0,4

nil:    moveq   #0,d0
        rts

        END
