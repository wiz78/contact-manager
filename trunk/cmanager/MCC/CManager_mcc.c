/***************************************************************************
    revision             : $Id: CManager_mcc.c,v 1.1.1.1 2003-01-06 15:57:01 tellini Exp $
	copyright            : (C) 1999-2003 by Simone Tellini
	email                : tellini@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/// Include
#include <exec/types.h>             //  exec
#include <exec/libraries.h>
#include <exec/memory.h>
#include <dos/dos.h>                //  dos
#include <intuition/sghooks.h>      //  intuition
#include <mui/textinput_mcc.h>      //  MUI
#include <mui/nlist_mcc.h>
#include <mui/nlistview_mcc.h>
#include <mui/listtree_mcc.h>
#include <libraries/mui.h>          //  libraries
#include <libraries/asl.h>
#include <libraries/genesis.h>
#include <clib/muimaster_protos.h>  //  clib
#include <clib/alib_protos.h>
#include <proto/exec.h>             //  proto
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/muimaster.h>
#include <proto/locale.h>
#include <proto/genesis.h>
#include <pragmas/genesis_pragmas.h> // pragmas

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef __STORM__
#include <extrafuncs.h>
#endif

#define CATCOMP_NUMBERS

#include "CManager_mcc.h"
#include "CManager_private.h"
#include "mui.h"
#include "grouplist.h"
#include "entrylist.h"
#include "export.h"
#include "search.h"
#include "mybrush.h"
#include "/CManager.h"
#include "/about.h"
#include "/prefs.h"
#include "/locale.h"

#include "/Library/CManager_protos.h"
#include "/Library/CManager_lib_lib.h"

#include "//SpeedBar/SpeedBar_mcc.h"
#include "//SpeedBar/SpeedBarCfg/SpeedBarCfg_mcc.h"
///
/// Defs
#define DATA_VERSION    0

#define VERSION         14
#define REVISION        3
#define VERS            "14.3"
#define DATE            "9.4.00"

struct CMCountry {
        struct CMCountry   *Succ;
        struct CMCountry   *Pred;
        TEXT                Country[80];
        TEXT                Country2[80];
        TEXT                Code[8];
};
///
/// Prototypes
BOOL ClassInitFunc( struct Library * );
void ClassExitFunc( struct Library * );

#ifdef __STORM__
extern void EXIT_4_free( void );
#endif

static void LoadBrushes( void );
static void FreeBrushes( void );

static void LoadCountries( void );
static void FreeCountries( void );

ULONG SAVEDS ASM CManager_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg );
static ULONG CManager_New( struct IClass *cl, Object *obj, struct opSet *msg );
static ULONG CManager_Dispose( struct IClass *cl, Object *obj, Msg *msg );
static ULONG CManager_Get( struct IClass *cl, Object *obj, struct opGet *msg );
static ULONG CManager_Set( struct IClass *cl, Object *obj, struct opSet *msg );
static ULONG CManager_Setup( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_Cleanup( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_CMCleanup( struct IClass *cl, Object *obj, Msg *msg );
static ULONG CManager_Import( struct IClass *cl, Object *obj, struct MUIP_Import *msg );
static ULONG CManager_Export( struct IClass *cl, Object *obj, struct MUIP_Export *msg );
static ULONG CManager_SetGroup( struct IClass *cl, Object *obj, struct MUIP_CManager_SetGroup *msg );
static ULONG CManager_GetGroup( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_Arrow( struct IClass *cl, Object *obj, struct MUIP_CManager_Arrow *msg );
static ULONG CManager_AddGroup( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_RemGroup( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_AddItem( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_HandleNotify( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_LoadData( struct IClass *cl, Object *obj, struct MUIP_CManager_LoadData *msg );
static ULONG CManager_ChangeSort( struct IClass *cl, Object *obj, struct MUIP_CManager_ChangeSort *msg );
static ULONG CManager_Sort( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_FreeData( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_SaveData( struct IClass *cl, Object *obj, struct MUIP_CManager_SaveData *msg );
static ULONG CManager_DisposeObj( struct IClass *cl, Object *obj, struct MUIP_CManager_DisposeObj *msg );
static ULONG CManager_GrabLists( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_ReinsertLists( struct IClass *cl, Object *obj, Msg msg );
static ULONG CManager_AddEntry( struct IClass *cl, Object *obj, struct MUIP_CManager_AddEntry *msg );
static ULONG CManager_LoadPrefs( struct IClass *cl, Object *obj, struct MUIP_CManager_LoadPrefs *msg );
static ULONG CManager_Login( struct IClass *cl, Object *obj, Msg msg );
extern ULONG CManager_ImportDB( struct IClass *cl, Object *obj, struct MUIP_CManager_Import *msg );


static void SAVEDS ASM User_ObjStrHook( REG( a0 ) struct Hook *hook, REG( a2 ) Object *list, REG( a1 ) Object *str );

static ULONG SAVEDS ASM Tree_DispHook( REG( a2 ) STRPTR *array, REG( a1 ) struct MUIS_Listtree_TreeNode *e );
static void  SAVEDS ASM Tree_DestHook( REG( a1 ) struct CMGroup *entry );
static ULONG SAVEDS ASM Tree_OpenHook( REG( a1 ) struct MUIS_Listtree_TreeNode *e );
static ULONG SAVEDS ASM Tree_CloseHook( REG( a1 ) struct MUIS_Listtree_TreeNode *e );

static ULONG SAVEDS ASM List_DispHook( REG( a0 ) struct Hook *, REG( a2 ) STRPTR *, REG( a1 ) struct CMUser * );
static LONG  SAVEDS ASM List_SortHook( REG( a0 ) struct Hook *, REG( a1 ) struct CMUser *, REG( a2 ) struct CMUser * );

static void InsertGroups( struct CManagerData *data );
static void InsertSubGroups( Object *Tree, struct MUIS_Listtree_TreeNode *tn, struct CMGroup *Group );
void RebuildTree( struct CManagerData *data );
static void RebuildTreeList( struct CManagerData *data, struct MinList *List, APTR Node );
void RebuildList( Object *Tree, Object *List, struct MUIS_Listtree_TreeNode *tn );
static void ClearList( Object *List );

static void StartDOSNotify( Object *obj, struct CManagerData *data );
static void StopDOSNotify( Object *obj, struct CManagerData *data );
static void ExtractDateStamp( STRPTR File, struct DateStamp *ds );

BOOL GetFile( Object *, struct CManagerData *, STRPTR, STRPTR, BOOL );
BOOL GetDir( Object *obj, STRPTR Title, STRPTR Path, BOOL SaveMode );

#ifdef MY_DEBUG

extern void kprintf( char *, ... );

#define DBG(x)  { kprintf( "CManager.mcc - %s - ", __FUNC__ ); x; }

#else

#define DBG(x)

#endif
///
/// Data
struct Library                 *MathIeeeDoubTransBase;
struct Library                 *MathIeeeDoubBasBase;
struct Library                 *LocaleBase;
struct Library                 *CManagerBase;
struct Library                 *OpenURLBase;
struct Library                 *RexxSysBase;
struct Library                 *IFFParseBase;
struct Library                 *OwnDevUnitBase;
struct Library                 *GenesisLoggerBase;
struct Library                 *GenesisBase;

static TEXT                 CMPath[256];

struct Locale              *Locale;
struct MinList              Countries;
struct SignalSemaphore      PrefsSem;

#define NUM_BRUSHES         5

static struct MyBrush      *Brushes[ NUM_BRUSHES ];

static STRPTR               BrushFiles[] = {
                                "Add.iff",  "Clone.iff",    "Edit.iff",     "Remove.iff",
                                "Sort.iff",
                            };
///


/// mcc header
#define CLASS           "CManager.mcc"
#define UserLibID       "$VER: "CLASS" "VERS" ("DATE") ©1999-2003 Simone Tellini\r\n"
#define Data            CManagerData
#define SUPERCLASS      MUIC_Group
#define _Dispatcher     CManager_Dispatcher

#define ClassInit
#define ClassExit

#include "mccheader.h"
///

/// CountNodes
static ULONG CountNodes( struct MinList *List )
{
    ULONG           cnt;
    struct MinNode *node;

    cnt = 0;

    for( node = List->mlh_Head; node->mln_Succ; node = node->mln_Succ )
        ++cnt;

    return( cnt );
}
///

/*
/// FPutString
void FPutString( BPTR file, STRPTR str )
{
    UWORD   len;

    len = strlen( str );

    FPutC( file, len );

    FWrite( file, str, len, 1 );
}
///
*/
/// FGetString
void FGetString( BPTR file, STRPTR str )
{
    UWORD   len;

    len = FGetC( file );

    FRead( file, str, len, 1 );

    str[ len ] = '\0';
}
///

#define CMLIB_VERSION   10

/// ClassInitFunc
BOOL ClassInitFunc( struct Library *base )
{
    struct Process *proc;
    APTR            wnd;
    BOOL            ret = FALSE;

    DBG( kprintf( "start\n" ));

    NewList(( struct List * )&Countries );

    proc = (struct Process *)FindTask( NULL );

    wnd = proc->pr_WindowPtr;

    proc->pr_WindowPtr = (APTR)-1;

    if(
#ifdef __STORM
       ( MathIeeeDoubTransBase = OpenLibrary( "mathieeedoubtrans.library",  0 )) &&
       ( MathIeeeDoubBasBase   = OpenLibrary( "mathieeedoubbas.library",    0 )) &&
#endif
       ( LocaleBase            = OpenLibrary( "locale.library",            36 )) &&
       ( RexxSysBase           = OpenLibrary( "rexxsyslib.library",        36 )) &&
       ( IFFParseBase          = OpenLibrary( "iffparse.library",          36 )) &&
       ( CManagerBase          = OpenLibrary( "CManager.library",          CMLIB_VERSION )) &&
       InitClasses() ) {

        OpenURLBase       = OpenLibrary( "openurl.library",                   0 );
        OwnDevUnitBase    = OpenLibrary( "OwnDevUnit.library",                0 );
        GenesisLoggerBase = OpenLibrary( "AmiTCP:libs/genesislogger.library", 0 );
        GenesisBase       = OpenLibrary( GENESISNAME,                         2 );

        DBG( kprintf( "GetVar()" ));

        if( GetVar( "Vapor/CManager_path", CMPath, sizeof( CMPath ), 0 ) > 0 ) {
            STRPTR  ptr;

            DBG( kprintf( "got var\n" ));

            if( ptr = PathPart( CMPath ))
                *ptr = '\0';

        } else {
            BOOL    loop;

            DBG( kprintf( "no ENV: var\n" ));

            do {

                loop = FALSE;

                if( GetDir( NULL, CM_GetString( MSG_GET_DB_DIR ), CMPath, TRUE )) {
                    TEXT    cm[256];
                    BPTR    f;

                    strcpy( cm, CMPath );
                    AddPart( cm, "CManager", sizeof( cm ));

                    UnLock( CreateDir( "ENV:Vapor"    ));
                    UnLock( CreateDir( "ENVARC:Vapor" ));

                    if( f = Open( "ENVARC:Vapor/CManager_path", MODE_NEWFILE )) {
                        Write( f, cm, strlen( cm ));
                        Close( f );
                    }

                    if( f = Open( "ENV:Vapor/CManager_path", MODE_NEWFILE )) {
                        Write( f, cm, strlen( cm ));
                        Close( f );
                    }

                } else if( MUI_Request( NULL, NULL, 0, NULL,
                                        CM_GetString( ANS_SELECT_CANCEL ),
                                        CM_GetString( MSG_NO_DIR_WARNING )))
                    loop = TRUE;

            } while( loop );
        }

        memset( &PrefsSem, 0, sizeof( PrefsSem ));

        InitSemaphore( &PrefsSem );

        DBG( kprintf( "Loading brushes\n" ));

        LoadBrushes();

        DBG( kprintf( "Loading the countries\n" ));

        LoadCountries();

        DBG( kprintf( "setup locale\n" ));

        Locale = OpenLocale( NULL );

        ret = TRUE;

    } else if( !CManagerBase ) {

        MUI_Request( NULL, NULL, 0, NULL,
                     "Ok",
                     "Couldn't open CManager.library v%ld",
                     CMLIB_VERSION );
    }

    proc->pr_WindowPtr = wnd;

    if( !ret )
        ClassExitFunc( base );

    if( MUIMasterBase->lib_Version >= 20 )
        ThisClass->mcc_Class->cl_ID = CLASS;

    DBG( kprintf( "end (%ld)\n", ret ));

    return( ret );
}
///
/// ClassExitFunc
void ClassExitFunc( struct Library *base )
{
    DBG( kprintf( "start\n" ));

    FreeCountries();

    DBG( kprintf( "about to call exitclasses\n" ));

    ExitClasses();

    DBG( kprintf( "about to call FreeBrushes\n" ));

    FreeBrushes();

    DBG( kprintf( "close locale\n" ));

    if( Locale ) {
        CloseLocale( Locale );
        Locale = NULL;
    }

    DBG( kprintf( "close catalog\n" ));

    if( Catalog ) {
        CloseCatalog( Catalog );
        Catalog = NULL;
    }

    DBG( kprintf( "close libs\n" ));

    if( GenesisBase ) {
        CloseLibrary( GenesisBase );
        GenesisBase = NULL;
    }

    if( GenesisLoggerBase ) {
        CloseLibrary( GenesisLoggerBase );
        GenesisLoggerBase = NULL;
    }

    if( OwnDevUnitBase ) {
        CloseLibrary( OwnDevUnitBase );
        OwnDevUnitBase = NULL;
    }

    if( OpenURLBase ) {
        CloseLibrary( OpenURLBase );
        OpenURLBase = NULL;
    }

    if( CManagerBase ) {
        CloseLibrary( CManagerBase );
        CManagerBase = NULL;
    }

    if( IFFParseBase ) {
        CloseLibrary( IFFParseBase );
        IFFParseBase = NULL;
    }

    if( RexxSysBase ) {
        CloseLibrary( RexxSysBase );
        RexxSysBase = NULL;
    }

    if( LocaleBase ) {
        CloseLibrary( LocaleBase );
        LocaleBase = NULL;
    }

#ifdef __STORM__
    EXIT_4_free();

    if( MathIeeeDoubBasBase ) {
        CloseLibrary( MathIeeeDoubBasBase );
        MathIeeeDoubBasBase = NULL;
    }

    if( MathIeeeDoubTransBase ) {
        CloseLibrary( MathIeeeDoubTransBase );
        MathIeeeDoubTransBase = NULL;
    }
#endif

    DBG( kprintf( "end\n" ));
}
///

/// LoadBrushes
static void LoadBrushes( void )
{
    BPTR    lock;
    TEXT    path[256];

    strcpy( path, CMPath );
    AddPart( path, "Images", sizeof( path ));

    if( lock = Lock( path, ACCESS_READ )) {
        BPTR    old;
        ULONG   i;

        old = CurrentDir( lock );

        for( i = 0; i < NUM_BRUSHES; i++ )
            Brushes[ i ] = LoadBrush( BrushFiles[ i ]);

        CurrentDir( old );

        UnLock( lock );
    }
}
///
/// FreeBrushes
static void FreeBrushes( void )
{
    ULONG   i;

    for( i = 0; i < NUM_BRUSHES; i++ )
        FreeBrush( Brushes[ i ]);
}
///

/// LoadCountries
static void LoadCountries( void )
{
    BPTR    file;
    TEXT    name[256];

    strcpy( name, CMPath );

    AddPart( name, "CountryCodes.txt", sizeof( name ));

    if( file = Open( name, MODE_OLDFILE )) {
        TEXT    buf[1024];

        while( FGets( file, buf, sizeof( buf ) - 1 )) {
            struct CSource      cs;
            struct CMCountry   *country;
            STRPTR              ptr;

            if( ptr = strchr( buf, '\n' ))
                *ptr = '\0';

            cs.CS_Buffer = buf;
            cs.CS_Length = strlen( buf );
            cs.CS_CurChr = 0;

            if( country = AllocMem( sizeof( struct CMCountry ), MEMF_CLEAR )) {

                AddTail(( struct List * )&Countries, ( struct Node * )country );

                ReadItem( country->Country,  sizeof( country->Country  ), &cs );
                ReadItem( country->Country2, sizeof( country->Country2 ), &cs );
                ReadItem( country->Code,     sizeof( country->Code     ), &cs );

            } else
                break;
        }

        Close( file );
    }
}
///
/// FreeCountries
static void FreeCountries( void )
{
    struct CMCountry   *country;

    while( country = (struct CMCountry *) RemTail(( struct List * )&Countries ))
        FreeMem( country, sizeof( struct CMCountry ));
}
///


/// CManager_Dispatcher
ULONG SAVEDS ASM CManager_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( CManager_New( cl, obj, (APTR)msg ));

        case OM_DISPOSE:
            return( CManager_Dispose( cl, obj, (APTR)msg ));

        case OM_GET:
            return( CManager_Get( cl, obj, (APTR)msg ));

        case OM_SET:
            return( CManager_Set( cl, obj, (APTR)msg ));

        case MUIM_Setup:
            return( CManager_Setup( cl, obj, (APTR)msg ));

        case MUIM_Cleanup:
            return( CManager_Cleanup( cl, obj, (APTR)msg ));

        case MUIM_Import:
            return( CManager_Import( cl, obj, (APTR)msg ));

        case MUIM_Export:
            return( CManager_Export( cl, obj, (APTR)msg ));

        case MUIM_CManager_Cleanup:
            return( CManager_CMCleanup( cl, obj, (APTR)msg ));

        case MUIM_CManager_SetGroup:
            return( CManager_SetGroup( cl, obj, (APTR)msg ));

        case MUIM_CManager_GetGroup:
            return( CManager_GetGroup( cl, obj, (APTR)msg ));

        case MUIM_CManager_AddGroup:
            return( CManager_AddGroup( cl, obj, (APTR)msg ));

        case MUIM_CManager_RemGroup:
            return( CManager_RemGroup( cl, obj, (APTR)msg ));

        case MUIM_CManager_AddItem:
            return( CManager_AddItem( cl, obj, (APTR)msg ));

        case MUIM_CManager_Arrow:
            return( CManager_Arrow( cl, obj, (APTR)msg ));

        case MUIM_CManager_LoadData:
            return( CManager_LoadData( cl, obj, (APTR)msg ));

        case MUIM_CManager_SaveData:
            return( CManager_SaveData( cl, obj, (APTR)msg ));

        case MUIM_CManager_FreeData:
            return( CManager_FreeData( cl, obj, (APTR)msg ));

        case MUIM_CManager_HandleNotify:
            return( CManager_HandleNotify( cl, obj, (APTR)msg ));

        case MUIM_CManager_ChangeSort:
            return( CManager_ChangeSort( cl, obj, (APTR)msg ));

        case MUIM_CManager_Sort:
            return( CManager_Sort( cl, obj, (APTR)msg ));

        case MUIM_CManager_DisposeObj:
            return( CManager_DisposeObj( cl, obj, (APTR)msg ));

        case MUIM_CManager_GrabLists:
            return( CManager_GrabLists( cl, obj, (APTR)msg ));

        case MUIM_CManager_ReinsertLists:
            return( CManager_ReinsertLists( cl, obj, (APTR)msg ));

        case MUIM_CManager_AddEntry:
            return( CManager_AddEntry( cl, obj, (APTR)msg ));

        case MUIM_CManager_LoadPrefs:
            return( CManager_LoadPrefs( cl, obj, (APTR)msg ));

        case MUIM_CManager_Import:
            return( CManager_ImportDB( cl, obj, (APTR)msg ));

        case MUIM_CManager_Export:
            return( CManager_ExportDB( cl, obj, (APTR)msg ));

        case MUIM_CManager_Login:
            return( CManager_Login( cl, obj, (APTR)msg ));

        case MUIM_CManager_Search:
            return( CManager_Search( cl, obj, (APTR)msg ));

        case MUIM_CManager_EditSelected: {
            struct CManagerData    *data = INST_DATA( cl, obj );

            return( DoMethod( data->List, MUIM_EntryList_Edit ));
        }

        case MUIM_CManager_OnDoubleClick: {
            struct CManagerData    *data = INST_DATA( cl, obj );

            if( data->DoubleClickHook ) {
                APTR    entry;

                DoMethod( data->List, MUIM_NList_GetEntry, MUIV_NList_GetEntry_Active, &entry );

                DBG( kprintf( "calling hook (h_Entry: %08lx)\n", data->DoubleClickHook->h_Entry ));

                CallHookPkt( data->DoubleClickHook, obj, entry );

                return( 0 );

            } else
                return( DoMethod( data->List, MUIM_EntryList_DoubleClick ));
        }
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// CManager_New
static ULONG CManager_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct CManagerData                 temp = { 0 };
    Object                             *Tree, *AddGroup, *RemGroup;
    Object                             *list, *Pop, *Users;
    enum { B_ADD = 0, B_CLONE, B_EDIT, B_REMOVE, B_SORT };
    static struct MUIS_SpeedBar_Button  buttons[] = {
                                            { 0, (STRPTR)GAD_ADD_ENTRY,   (STRPTR)HELP_ADD_ENTRY,   0, NULL },
                                            { 1, (STRPTR)GAD_CLONE_ENTRY, (STRPTR)HELP_CLONE_ENTRY, 0, NULL },
                                            { 2, (STRPTR)GAD_EDIT,        (STRPTR)HELP_EDIT,        0, NULL },
                                            { 3, (STRPTR)GAD_REMOVE,      (STRPTR)HELP_REMOVE,      0, NULL },
                                            { 4, (STRPTR)GAD_SORT,        (STRPTR)HELP_SORT,        0, NULL },
                                            { MUIV_SpeedBar_End },
                                        };
    static BOOL                         FirstTime = TRUE;

    temp.Prefs.TreeW = 60;
    temp.Prefs.ListW = 160;

    if( FirstTime ) {
        FirstTime = FALSE;
        LocalizeSpeedBar( buttons );
    }

    if( obj = ( Object * )DoSuperNew( cl, obj,

                                Child, HGroup,
                                    Child, MakeLabel1( GAD_USERNAME ),
                                    Child, Pop = PopobjectObject,
                                            MUIA_Popstring_String,      temp.PopUser = MakeString( 40, GAD_USERNAME, -1 ),
                                            MUIA_Popstring_Button,      PopButton( MUII_PopUp ),
                                            MUIA_Popobject_Object,      Users = ListviewObject,
                                                    MUIA_Listview_List, ListObject,
                                                        InputListFrame,
                                                        MUIA_List_ConstructHook,    MUIV_List_ConstructHook_String,
                                                        MUIA_List_DestructHook,     MUIV_List_DestructHook_String,
                                                    End,
                                            End,
                                    End,
                                    Child, HSpace( 0 ),
                                End,

                                Child, HGroup,

                                    Child, temp.TreeGroup = VGroup,

                                        MUIA_HorizWeight,   temp.Prefs.TreeW,
                                        MUIA_Group_Spacing, 0,

                                        Child, Tree = ListviewObject,
                                            MUIA_CycleChain,        TRUE,
                                            MUIA_Listview_DragType, MUIV_Listview_DragType_Immediate,
                                            MUIA_Listview_List,     temp.Tree = NewObject( CL_GroupList->mcc_Class, NULL,
                                                InputListFrame,
                                                MUIA_Listtree_SortHook,         MUIV_Listtree_SortHook_LeavesBottom,
                                                MUIA_Listtree_DragDropSort,     TRUE,
                                            End,
                                        End,

                                        Child, HGroup, MUIA_Group_Spacing, 0,

                                            Child, VGroup, MUIA_Group_Spacing, 0,
                                                Child, temp.GroupName = MakeString( 80, (ULONG)-1, -1 ),
                                                Child, HGroup,
                                                    MUIA_Group_SameSize, TRUE,
                                                    MUIA_Group_Spacing,  0,
                                                    Child, AddGroup = MakeButton( GAD_ADD_GROUP, -1 ),
                                                    Child, RemGroup = MakeButton( GAD_REM_GROUP, -1 ),
                                                End,
                                            End,

                                            Child, VGroup, MUIA_Group_Spacing, 0,
                                                Child, temp.GroupUp = ImageObject,
                                                    ImageButtonFrame,
                                                    MUIA_Background,    MUII_ButtonBack,
                                                    MUIA_CycleChain,    1,
                                                    MUIA_InputMode,     MUIV_InputMode_RelVerify,
                                                    MUIA_Image_Spec,    MUII_ArrowUp,
                                                End,
                                                Child, temp.GroupDown = ImageObject,
                                                    ImageButtonFrame,
                                                    MUIA_Background,    MUII_ButtonBack,
                                                    MUIA_CycleChain,    1,
                                                    MUIA_InputMode,     MUIV_InputMode_RelVerify,
                                                    MUIA_Image_Spec,    MUII_ArrowDown,
                                                End,
                                                Child, VSpace( 0 ),
                                            End,
                                        End,
                                    End,

                                    Child, BalanceObject, End,

                                    Child, temp.ListGroup = VGroup,

                                        MUIA_HorizWeight,   temp.Prefs.ListW,

                                        Child, temp.List = NewObject( CL_EntryList->mcc_Class, NULL,
                                            MUIA_CycleChain,        TRUE,
                                            MUIA_ObjectID,          MAKE_ID( 'C', 'M', 'l', 's' ),
                                            MUIA_NListview_NList,   list = NListObject,
                                                InputListFrame,
                                                MUIA_ContextMenu,           NULL,
                                                MUIA_NList_Title,           TRUE,
                                                MUIA_NList_AutoVisible,     TRUE,
                                                MUIA_NList_DragSortable,    TRUE,
                                                MUIA_NList_Format,          "BAR,BAR,BAR NOTB,NOTB",
                                                MUIA_NList_DragType,        MUIV_NList_DragType_Default,
                                                MUIA_NList_MultiSelect,     MUIV_NList_MultiSelect_Default,
                                            End,
                                        End,

                                        Child, HGroup,
                                            Child, HSpace( 0 ),

                                            Child, temp.SpeedBar = SpeedBarObject,
                                                MUIA_Group_Horiz,               TRUE,
                                                MUIA_SpeedBar_Images,           Brushes,
                                                MUIA_SpeedBar_ViewMode,         ToolbarCfg.ViewMode,
                                                MUIA_SpeedBar_Borderless,       ( ToolbarCfg.Flags & MUIV_SpeedBarCfg_Borderless   ) ? TRUE : FALSE,
                                                MUIA_SpeedBar_RaisingFrame,     ( ToolbarCfg.Flags & MUIV_SpeedBarCfg_Raising      ) ? TRUE : FALSE,
                                                MUIA_SpeedBar_SmallImages,      ( ToolbarCfg.Flags & MUIV_SpeedBarCfg_SmallButtons ) ? TRUE : FALSE,
                                                MUIA_SpeedBar_Sunny,            ( ToolbarCfg.Flags & MUIV_SpeedBarCfg_Sunny        ) ? TRUE : FALSE,
                                                MUIA_SpeedBar_Buttons,          buttons,
                                                MUIA_SpeedBar_SpacerIndex,      -1,
                                                MUIA_SpeedBar_Spread,           FALSE,
                                                MUIA_SpeedBar_StripUnderscore,  TRUE,
                                            End,

                                            Child, HSpace( 0 ),
                                        End,
                                    End,
                                End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct CManagerData    *data = INST_DATA( cl, obj );
        struct TagItem         *tag;

        *data = temp;

        if( GenesisBase ) {
            ULONG   i;

            for( i = 0;; i++ ) {
                TEXT    name[256];

                if(!( GetUserName( i, name, sizeof( name ))))
                    break;

                DoMethod( Users, MUIM_List_InsertSingle, name, MUIV_List_Insert_Bottom );
            }

            data->UserHook.h_Entry = (HOOKFUNC)User_ObjStrHook;
            data->UserHook.h_Data  = obj;

            set( Pop, MUIA_Popobject_ObjStrHook, &data->UserHook );

        } else
            set( Pop, MUIA_Disabled, TRUE );

        data->DispHook.h_Entry = (HOOKFUNC)List_DispHook;
        data->DispHook.h_Data  = temp.List;
        data->SortHook.h_Entry = (HOOKFUNC)List_SortHook;
        data->SortHook.h_Data  = temp.List;

        data->TreeDispHook.h_Entry  = (HOOKFUNC)Tree_DispHook;
        data->TreeDestHook.h_Entry  = (HOOKFUNC)Tree_DestHook;
        data->TreeOpenHook.h_Entry  = (HOOKFUNC)Tree_OpenHook;
        data->TreeCloseHook.h_Entry = (HOOKFUNC)Tree_CloseHook;

        data->NotifyReq.nr_stuff.nr_Signal.nr_Task      = FindTask( NULL );
        data->NotifyReq.nr_stuff.nr_Signal.nr_SignalNum = AllocSignal( -1 );

        data->Req = MUI_AllocAslRequest( ASL_FileRequest, NULL );

        data->Flags      = SAVED;
        data->LastActive = MUIV_Listtree_Active_Off;

        SetAttrs( list,
                  MUIA_NList_CompareHook, &data->SortHook,
                  MUIA_NList_DisplayHook, &data->DispHook,
                  TAG_DONE );

        SetAttrs( data->Tree,
                  MUIA_Listtree_DisplayHook,      &data->TreeDispHook,
                  MUIA_Listtree_DestructHook,     &data->TreeDestHook,
                  MUIA_Listtree_OpenHook,         &data->TreeOpenHook,
                  MUIA_Listtree_CloseHook,        &data->TreeCloseHook,
                  TAG_DONE );

        set( data->Tree, MUIA_GroupList_CMObj, obj );
        set( data->List, MUIA_EntryList_CMObj, obj );

        set( temp.GroupName, MUIA_String_AttachedList, Tree );

        if( tag = FindTagItem( MUIA_CManager_NoMenu, msg->ops_AttrList ))
            set( data->List, MUIA_EntryList_NoMenu, tag->ti_Data );

        if( GetTagData( MUIA_CManager_HideUsers, FALSE, msg->ops_AttrList ))
            data->Flags |= HIDE_USERS;

        if( GetTagData( MUIA_CManager_HideWWW, FALSE, msg->ops_AttrList ))
            data->Flags |= HIDE_WWW;

        if( GetTagData( MUIA_CManager_HideFTP, FALSE, msg->ops_AttrList ))
            data->Flags |= HIDE_FTP;

        if( GetTagData( MUIA_CManager_HideChat, FALSE, msg->ops_AttrList ))
            data->Flags |= HIDE_CHAT;

        if( GetTagData( MUIA_CManager_HideTelnet, FALSE, msg->ops_AttrList ))
            data->Flags |= HIDE_TELNET;

        data->DoubleClickHook = (struct Hook *) GetTagData( MUIA_CManager_AppDoubleClick, NULL, msg->ops_AttrList );


        /*  Gadgets */
        DoMethod( Users, MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, Pop, 2, MUIM_Popstring_Close, TRUE );
        DoMethod( temp.PopUser, MUIM_Notify, MUIA_String_Acknowledge, TRUE, obj, 1, MUIM_CManager_Login );

        DoMethod( Tree, MUIM_Notify, MUIA_Listtree_Active, MUIV_EveryTime, obj, 2, MUIM_CManager_SetGroup, MUIV_TriggerValue );

        DoMethod( buttons[ B_ADD    ].Object, MUIM_Notify, MUIA_Pressed, FALSE, obj,       1, MUIM_CManager_AddItem );
        DoMethod( buttons[ B_CLONE  ].Object, MUIM_Notify, MUIA_Pressed, FALSE, temp.List, 1, MUIM_EntryList_Clone  );
        DoMethod( buttons[ B_EDIT   ].Object, MUIM_Notify, MUIA_Pressed, FALSE, temp.List, 1, MUIM_EntryList_Edit   );
        DoMethod( buttons[ B_REMOVE ].Object, MUIM_Notify, MUIA_Pressed, FALSE, temp.List, 1, MUIM_EntryList_Remove );
        DoMethod( buttons[ B_SORT   ].Object, MUIM_Notify, MUIA_Pressed, FALSE, obj,       1, MUIM_CManager_Sort    );

        DoMethod( temp.List, MUIM_Notify, MUIA_NList_DoubleClick, TRUE, obj, 1, MUIM_CManager_OnDoubleClick );
        DoMethod( temp.List, MUIM_Notify, MUIA_NList_TitleClick, MUIV_EveryTime, obj, 2, MUIM_CManager_ChangeSort, MUIV_TriggerValue );

        DoMethod( temp.GroupName, MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime, obj, 1, MUIM_CManager_GetGroup );

        DoMethod( temp.GroupUp,   MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_CManager_Arrow, 1 );
        DoMethod( temp.GroupDown, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_CManager_Arrow, 0 );

        DoMethod( AddGroup, MUIM_Notify, MUIA_Pressed, FALSE, obj, 1, MUIM_CManager_AddGroup );
        DoMethod( RemGroup, MUIM_Notify, MUIA_Pressed, FALSE, obj, 1, MUIM_CManager_RemGroup );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// CManager_Dispose
static ULONG CManager_Dispose( struct IClass *cl, Object *obj, Msg *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "start\n" ));

    data->Flags |= SAVED; // we don't care any longer for this

    DoMethod( obj, MUIM_CManager_FreeData );

    if( data->NotifyReq.nr_stuff.nr_Signal.nr_Task &&
        ( data->NotifyReq.nr_stuff.nr_Signal.nr_SignalNum != -1 ))
        FreeSignal( data->NotifyReq.nr_stuff.nr_Signal.nr_SignalNum );

    if( data->Req )
        MUI_FreeAslRequest( data->Req );

    DBG( kprintf( "end\n" ));

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// CManager_Setup
static ULONG CManager_Setup( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "start\n" ));

    if(!( DoSuperMethodA( cl, obj, msg ))) {
        DBG( kprintf( "dosupermethod failed\n" ));
        StopDOSNotify( obj, data );
        return( FALSE );
    }

    DBG( kprintf( "dosupermethod done\n" ));

    data->Flags |= SETTING_UP | SETUP_DONE;

    if( data->CMData && !( data->Flags & NOTIFIED )) {

        DBG( kprintf( "calling HandleNotify\n" ));

        DoMethod( obj, MUIM_CManager_HandleNotify );

        DBG( kprintf( "HandleNotify() returned\n" ));

        if( data->CMData && !( data->Flags & NOTIFIED )) {
            // HandleNotify could have changed the condition

            StartDOSNotify( obj, data );
        }
    }

    data->Flags &= ~SETTING_UP;

    DBG( kprintf( "end\n" ));

    return( TRUE );
}
///
/// CManager_Cleanup
static ULONG CManager_Cleanup( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "start\n" ));

    StopDOSNotify( obj, data );

    data->Flags &= ~SETUP_DONE;

    DBG( kprintf( "end\n" ));

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// CManager_Get
static ULONG CManager_Get( struct IClass *cl, Object *obj, struct opGet *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    switch ( msg->opg_AttrID ) {

        case MUIA_Version:
            *msg->opg_Storage = VERSION;
            return( TRUE );

        case MUIA_Revision:
            *msg->opg_Storage = REVISION;
            return( TRUE );

        case MUIA_CManager_ListObject:
            *msg->opg_Storage = (ULONG)data->List;
            return( TRUE );

        case MUIA_CManager_TreeObject:
            *msg->opg_Storage = (ULONG)data->Tree;
            return( TRUE );

        case MUIA_CManager_Changed:
            *msg->opg_Storage = ( data->Flags & SAVED ) ? FALSE : TRUE;
            return( TRUE );

        case MUIA_CManager_Path:
            *msg->opg_Storage = (ULONG)CMPath;
            return( TRUE );

        case MUIA_CManager_CMData:
            *msg->opg_Storage = (ULONG)data->CMData;
            DBG( kprintf( "Getting CMData: %08lx\n", data->CMData ));
            return( TRUE );
    }

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// CManager_Set
static ULONG CManager_Set( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    struct TagItem         *tags, *tag;

    for( tags = msg->ops_AttrList; tag = NextTagItem( &tags ); ) {

        switch( tag->ti_Tag ) {

            case MUIA_CManager_Changed:
                if( tag->ti_Data ) {

                    data->Flags &= ~SAVED;

                    DBG( kprintf( "MUIA_CManager_Changed\n" ));
                }
                break;

            case MUIA_CManager_NoMenu:
                set( data->List, MUIA_EntryList_NoMenu, tag->ti_Data );
                break;

            case MUIA_CManager_AppDoubleClick:
                data->Flags &= ~APP_DBLCLK;

                data->DoubleClickHook = (struct Hook *) tag->ti_Data;
                break;
        }
    }

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///
/// CManager_CMCleanup
static ULONG CManager_CMCleanup( struct IClass *cl, Object *obj, Msg *msg )
{
    DBG( kprintf( "start\n" ));

    CloseAllEditWindows(( Object * )xget( obj, MUIA_ApplicationObject ));

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_Import
static ULONG CManager_Import( struct IClass *cl, Object *obj, struct MUIP_Import *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    ULONG                   id;

    DBG( kprintf( "start\n" ));

    if( id = muiNotifyData( obj )->mnd_ObjectID ) {
        struct MCCPrefs    *prefs;

        if( prefs = (struct MCCPrefs *) DoMethod( msg->dataspace, MUIM_Dataspace_Find, id )) {

            data->Prefs = *prefs;

            set( data->TreeGroup, MUIA_HorizWeight, prefs->TreeW );
            set( data->ListGroup, MUIA_HorizWeight, prefs->ListW );
        }
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_Export
static ULONG CManager_Export( struct IClass *cl, Object *obj, struct MUIP_Export *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    ULONG                   id;

    DBG( kprintf( "start\n" ));

    if( id = muiNotifyData( obj )->mnd_ObjectID ) {

        data->Prefs.TreeW = xget( data->TreeGroup, MUIA_HorizWeight );
        data->Prefs.ListW = xget( data->ListGroup, MUIA_HorizWeight );

        DoMethod( msg->dataspace, MUIM_Dataspace_Add, &data->Prefs, sizeof( data->Prefs ), id );
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_SetGroup
static ULONG CManager_SetGroup( struct IClass *cl, Object *obj, struct MUIP_CManager_SetGroup *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    struct CMGroup         *entry;

    DBG( kprintf( "start\n" ));

    RebuildList( data->Tree, data->List, data->LastActive );

    data->LastActive = msg->Node;

    if( msg->Node && ( msg->Node != MUIV_Listtree_Active_Off )) {
        struct MUIS_Listtree_TreeNode  *tn, *tn2;
        ULONG                           type;

        tn = msg->Node;

        while( tn2 = (struct MUIS_Listtree_TreeNode *)
                        DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                                  tn,
                                  MUIV_Listtree_GetEntry_Position_Parent, 0 )) {
            tn = tn2;
        }

        if(( APTR )tn->tn_User == data->CMData->Users )
            type = CME_USER;

        else if(( APTR )tn->tn_User == data->CMData->WWWs )
            type = CME_WWW;

        else if(( APTR )tn->tn_User == data->CMData->FTPs )
            type = CME_FTP;

        else if(( APTR )tn->tn_User == data->CMData->Chat )
            type = CME_CHAT;

        else if(( APTR )tn->tn_User == data->CMData->Telnet )
            type = CME_TELNET;

        set( data->List, MUIA_EntryList_Type, type );

        switch( type ) {

            case CME_USER:
                set( data->List, MUIA_NList_Format, "BAR,BAR,BAR NOTB,NOTB" );
                break;

            case CME_FTP:
            case CME_WWW:
                set( data->List, MUIA_NList_Format, "BAR NOTB,BAR NOTB,BAR NOTB,BAR NOTB,NOTB" );
                break;

            case CME_TELNET:
                set( data->List, MUIA_NList_Format, "BAR NOTB,NOTB" );
                break;

            default:
                set( data->List, MUIA_NList_Format, "BAR NOTB,BAR NOTB,NOTB" );
                break;
        }

        entry = (struct CMGroup *) msg->Node->tn_User;

        if( entry->Type == CME_SECTION ) {

            set( data->GroupName, MUIA_Disabled, TRUE );
            set( data->GroupUp,   MUIA_Disabled, TRUE );
            set( data->GroupDown, MUIA_Disabled, TRUE );

        } else {

            set( data->GroupName,   MUIA_Disabled,            FALSE           );
            set( data->GroupUp,     MUIA_Disabled,            FALSE           );
            set( data->GroupDown,   MUIA_Disabled,            FALSE           );
            nnset( data->GroupName, MUIA_String_Contents,     entry->Name     );
            set((Object *) xget( obj, MUIA_WindowObject ), MUIA_Window_ActiveObject, data->GroupName );
        }

        set( data->List, MUIA_EntryList_Group, msg->Node );
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_GetGroup
static ULONG CManager_GetGroup( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData            *data = INST_DATA( cl, obj );
    struct MUIS_Listtree_TreeNode  *tn;

    DBG( kprintf( "start\n" ));

    if( tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                          MUIV_Listtree_GetEntry_ListNode_Active,
                          MUIV_Listtree_GetEntry_Position_Active, 0 )) {
        struct CMGroup *entry;

        entry = (struct CMGroup *) tn->tn_User;

        set( data->Tree, MUIA_Listtree_Quiet, TRUE );

        getstr( data->GroupName, entry->Name );

        set( data->Tree, MUIA_Listtree_Quiet, FALSE );

        set( obj, MUIA_CManager_Changed, TRUE );
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_Arrow
static ULONG CManager_Arrow( struct IClass *cl, Object *obj, struct MUIP_CManager_Arrow *msg )
{
    struct CManagerData            *data = INST_DATA( cl, obj );
    struct MUIS_Listtree_TreeNode  *tn;

    DBG( kprintf( "start\n" ));

    if( tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                          MUIV_Listtree_GetEntry_ListNode_Active,
                          MUIV_Listtree_GetEntry_Position_Active, 0 )) {
        struct MUIS_Listtree_TreeNode  *tn2;

        if( tn2 = (struct MUIS_Listtree_TreeNode *)
                    DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                              tn,
                              msg->Direction ? MUIV_Listtree_GetEntry_Position_Previous :
                              MUIV_Listtree_GetEntry_Position_Next, 0 )) {

            if( msg->Direction ) {
                struct MUIS_Listtree_TreeNode  *tn3;

                tn3 = tn2;
                tn2 = tn;
                tn  = tn3;
            }

            set( data->Tree, MUIA_Listtree_Quiet, TRUE );

            DoMethod( data->Tree, MUIM_Listtree_Move, tn, tn, tn2, tn2, 0 );

            if(!( msg->Direction ))
                set( data->Tree, MUIA_Listtree_Active, tn );

            set( data->Tree, MUIA_Listtree_Quiet, FALSE );

            set( obj, MUIA_CManager_Changed, TRUE );
        }
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_AddGroup
static ULONG CManager_AddGroup( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    struct CMGroup         *entry;

    DBG( kprintf( "start\n" ));

    if( entry = CM_AllocEntry( CME_GROUP )) {
        struct MUIS_Listtree_TreeNode  *tn;
        struct CMGroup                 *parent;

        entry->Flags = GRPF_OPEN;

        tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                          MUIV_Listtree_GetEntry_ListNode_Active,
                          MUIV_Listtree_GetEntry_Position_Active, 0 );

        parent = (struct CMGroup *) tn->tn_User;

        AddTail(( struct List * )&parent->SubGroups, ( struct Node * )entry );

        tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_Insert,
                          "M", entry, tn, MUIV_Listtree_Insert_PrevNode_Tail,
                          TNF_LIST | MUIV_Listtree_Insert_Flags_Active );

        DoMethod( data->Tree, MUIM_Listtree_Open, tn, tn, 0 );

        set( obj, MUIA_CManager_Changed, TRUE );
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_RemGroup
static ULONG CManager_RemGroup( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData            *data = INST_DATA( cl, obj );
    struct MUIS_Listtree_TreeNode  *tn;

    DBG( kprintf( "start\n" ));

    if( tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                          MUIV_Listtree_GetEntry_ListNode_Active,
                          MUIV_Listtree_GetEntry_Position_Active, 0 )) {

        if((( struct CMUser * )tn->tn_User )->Type != CME_SECTION ) {

            set( data->List, MUIA_NList_Quiet, TRUE );

            DoMethod( data->List, MUIM_NList_Clear );

            set( data->List, MUIA_NList_Quiet, FALSE );

            DoMethod( data->Tree, MUIM_Listtree_Remove, NULL, MUIV_Listtree_Remove_TreeNode_Active, 0 );

            set( obj, MUIA_CManager_Changed, TRUE );
        }
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_AddItem
static ULONG CManager_AddItem( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData            *data = INST_DATA( cl, obj );
    struct MUIS_Listtree_TreeNode  *tn, *tn2;

    DBG( kprintf( "start\n" ));

    if( tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                          MUIV_Listtree_GetEntry_ListNode_Active,
                          MUIV_Listtree_GetEntry_Position_Active, 0 )) {
        struct CMGroup *group;
        struct CMUser  *entry;
        UBYTE           type;

        group = (struct CMGroup *) tn->tn_User;

        if(!( tn->tn_Flags & TNF_OPEN ))
            DoMethod( data->Tree, MUIM_Listtree_Open, tn, tn, 0 );

        // find root
        while( tn2 = (struct MUIS_Listtree_TreeNode *)
                        DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                                  tn,
                                  MUIV_Listtree_GetEntry_Position_Parent, 0 )) {
            tn = tn2;
        }

        if(( APTR )tn->tn_User == data->CMData->Users )
            type = CME_USER;

        else if(( APTR )tn->tn_User == data->CMData->WWWs )
            type = CME_WWW;

        else if(( APTR )tn->tn_User == data->CMData->FTPs )
            type = CME_FTP;

        else if(( APTR )tn->tn_User == data->CMData->Chat )
            type = CME_CHAT;

        else if(( APTR )tn->tn_User == data->CMData->Telnet )
            type = CME_TELNET;

        if( entry = CM_AllocEntry( type )) {

            DoMethod( data->List, MUIM_NList_InsertSingle, entry, MUIV_NList_Insert_Bottom );

            set( data->List, MUIA_NList_Active, MUIV_NList_Active_Bottom );

            switch( type ) {

                case CME_FTP:
                    ((struct CMFTP *)entry )->Port = 21;
                    break;

                case CME_TELNET:
                    ((struct CMTelnet *)entry )->Port = 23;
                    break;
            }

            entry->Flags |= ENTRY_NEW;

            DoMethod( _app( obj ), MUIM_Application_PushMethod, data->List, 1, MUIM_EntryList_Edit );
        }
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_ChangeSort
static ULONG CManager_ChangeSort( struct IClass *cl, Object *obj, struct MUIP_CManager_ChangeSort *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "start\n" ));

    if(( xget( data->List, MUIA_EntryList_Type ) == CME_USER ) && ( msg->Column < 2 )) {
        struct EntryListData   *ldata = INST_DATA( CL_EntryList->mcc_Class, data->List );
        BOOL                    neworder;

        neworder = msg->Column == 0;

        if( ldata->Order.OrderByName == neworder )
            ldata->Order.Invert = !ldata->Order.Invert;
        else
            ldata->Order.OrderByName = neworder;

        set( data->List, MUIA_NList_Format, "BAR,BAR,BAR NOTB,NOTB" );

        DoMethod( obj, MUIM_CManager_Sort );
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_Sort
static ULONG CManager_Sort( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "start\n" ));

    set( data->List, MUIA_NList_Quiet, TRUE );

    DoMethod( data->List, MUIM_NList_Sort );

    data->Flags &= ~SAVED;

    set( data->List, MUIA_NList_Quiet, FALSE );

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_HandleNotify
static ULONG CManager_HandleNotify( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    struct DateStamp        ds;

    DBG( kprintf( "start\n" ));

    ExtractDateStamp( data->CMData->Path, &ds );

    if( CompareDates( &ds, &data->FileDS ) != 0 ) {
        DBG( kprintf( "file changed\n" ));
        DoMethod( obj, MUIM_CManager_LoadData, data->User, data->Path );
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_LoadData
static ULONG CManager_LoadData( struct IClass *cl, Object *obj, struct MUIP_CManager_LoadData *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    BOOL                    ok;

    DBG( kprintf( "start\n" ));

    if( msg->Path && msg->Path[0] ) {
        BPTR    lock;

        if( lock = Lock( msg->Path, ACCESS_READ ))
            UnLock( lock );
        else
            return( 0 );
    }

    DoMethod( obj, MUIM_CManager_FreeData );

    data->User[0] = '\0';
    data->Path[0] = '\0';

    if( msg->User )
        stccpy( data->User, msg->User, sizeof( data->User ));

    if( msg->Path )
        stccpy( data->Path, msg->Path, sizeof( data->Path ));

    DBG( kprintf( "User: %08lx  Path: %08lx\n", msg->User, msg->Path ));

    if( !data->User[0] && !data->Path[0] )
        if( data->CMData = CM_LoadCurrentUserData( FALSE )) {

            DBG( kprintf( "CM_LoadCurrentUserData() ok\n" ));

            strcpy( data->User, data->CMData->CurrentUser );

            ok = TRUE;

        } else
            return( FALSE );

    if( !data->CMData )
        if( data->CMData = CM_AllocCMData() ) {
            TEXT    file[256];

            if( data->User[0] )
                stccpy( data->CMData->CurrentUser, data->User, sizeof( data->CMData->CurrentUser ));

            if( data->Path[0] )
                stccpy( data->CMData->Path, data->Path, sizeof( data->CMData->Path ));
            else {

                stccpy( data->CMData->Path, CMPath, sizeof( data->CMData->Path ));

                sprintf( file, "%s/%s.data", data->User, data->User );

                AddPart( data->CMData->Path, file, sizeof( data->CMData->Path ));
            }

            ok = CM_LoadData( data->CMData->Path, data->CMData, data->CMData->CurrentUser );

            DBG( kprintf( "CM_LoadData return code: %ld\n", ok ));
        }

    if( data->CMData ) {

        DBG( kprintf( "CMData:  %08lx\n"
                      "Users:   %08lx\n"
                      "WWWs:    %08lx\n"
                      "FTPs:    %08lx\n"
                      "Telnet:  %08lx\n",
                      data->CMData, data->CMData->Users, data->CMData->WWWs,
                      data->CMData->FTPs, data->CMData->Telnet ));

        if( data->User[0] )
            set( data->PopUser, MUIA_String_Contents, data->User );

        DoMethod( obj, MUIM_CManager_LoadPrefs, NULL );

        DBG( kprintf( "after LoadPrefs()\n" ));

        data->Flags |= SAVED;

        set( obj, MUIA_CManager_Changed, FALSE );

        set( data->Tree, MUIA_GroupList_CMData, data->CMData );

        DBG( kprintf( "InsertGroups()\n" ));

        InsertGroups( data );

        StartDOSNotify( obj, data );
    }

    DBG( kprintf( "done\n" ));

    return( ok );
}
///
/// CManager_SaveData
static ULONG CManager_SaveData( struct IClass *cl, Object *obj, struct MUIP_CManager_SaveData *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    TEXT                    backup[256];

    DBG( kprintf( "start\n" ));
    DBG( kprintf( "SaveData()\n" ));

    if(( data->Flags & SAVED ) && !msg->Ask )
        return( 0 );

    StopDOSNotify( obj, data );

    if( !data->CMData ) {

        if( data->CMData = CM_AllocCMData() ) {

            stccpy( data->CMData->CurrentUser, data->User, sizeof( data->CMData->CurrentUser ));
            stccpy( data->CMData->Path, CMPath, sizeof( data->CMData->Path ));

            AddPart( data->CMData->Path, "Default.data", sizeof( data->CMData->Path ));

            set( data->Tree, MUIA_GroupList_CMData, data->CMData );

        } else
            return( FALSE );
    }

    DoMethod( obj, MUIM_CManager_GrabLists );

    if( msg->Ask ) {

        DBG( kprintf( "Asking for the file...\n" ));

        if(!( GetFile( obj, data, data->CMData->Path, NULL, TRUE )))
            return( FALSE );

        DBG( kprintf( "file name ok\n" ));

    } else if( msg->Path )
        stccpy( data->CMData->Path, msg->Path, sizeof( data->CMData->Path ));

    sprintf( backup, "%s.bak", data->CMData->Path );

    DeleteFile( backup );
    Rename( data->CMData->Path, backup );

    CM_SaveData( data->CMData->Path, data->CMData, data->User );

    StartDOSNotify( obj, data );

    data->Flags |= SAVED;

    set( obj, MUIA_CManager_Changed, FALSE );

    DBG( kprintf( "end\n" ));

    return( TRUE );
}
///
/// CManager_FreeData
static ULONG CManager_FreeData( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "start\n" ));
    DBG( kprintf( "Flags = %ld\n", data->Flags ));

    StopDOSNotify( obj, data );

    if(!( data->Flags & SAVED ))
        DoMethod( obj, MUIM_CManager_SaveData, TRUE, NULL );

    if( data->CMData ) {

        DoMethod( obj, MUIM_CManager_GrabLists );

        CM_FreeData( data->CMData );

        data->CMData = NULL;
    }

    DBG( kprintf( "clear lists\n" ));

    set( data->List, MUIA_NList_Quiet, TRUE );

    set( data->Tree, MUIA_Listtree_DestructHook, NULL );

    ClearList( data->Tree );

    set( data->Tree, MUIA_Listtree_DestructHook, &data->TreeDestHook );

    DoMethod( data->List, MUIM_NList_Clear );

    set( data->List, MUIA_NList_Quiet,    FALSE );
    set( data->Tree, MUIA_Listtree_Quiet, TRUE  );

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_DisposeObj
static ULONG CManager_DisposeObj( struct IClass *cl, Object *obj, struct MUIP_CManager_DisposeObj *msg )
{
    DBG( kprintf( "start\n" ));

    MUI_DisposeObject( msg->Object );

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_GrabLists
static ULONG CManager_GrabLists( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData            *data = INST_DATA( cl, obj );
    struct MUIS_Listtree_TreeNode  *tn;

    DBG( kprintf( "start\n" ));

    tn = (struct MUIS_Listtree_TreeNode *)
            DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                      MUIV_Listtree_GetEntry_ListNode_Active,
                      MUIV_Listtree_GetEntry_Position_Active, 0 );

    RebuildList( data->Tree, data->List, tn );

    RebuildTree( data );

    data->Flags |= GRABBED_LISTS;

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_ReinsertLists
static ULONG CManager_ReinsertLists( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "start\n" ));

    if( data->Flags & GRABBED_LISTS ) {

        set( data->List, MUIA_NList_Quiet, TRUE );
        set( data->Tree, MUIA_Listtree_DestructHook, NULL );

        ClearList( data->Tree );

        data->LastActive = MUIV_Listtree_Active_Off;

        DoMethod( data->List, MUIM_NList_Clear );

        InsertGroups( data );

        set( data->List, MUIA_NList_Quiet,    FALSE );
        set( data->Tree, MUIA_Listtree_Quiet, FALSE );

        set( data->Tree, MUIA_Listtree_DestructHook, &data->TreeDestHook );

        data->Flags &= ~GRABBED_LISTS;
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_AddEntry
static ULONG CManager_AddEntry( struct IClass *cl, Object *obj, struct MUIP_CManager_AddEntry *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "start\n" ));

    if(( msg->Flags & MUIV_CManager_AddEntry_CurrentGroup ) && ( msg->Entry->Type == xget( data->List, MUIA_EntryList_Type ))) {

        DoMethod( data->List, MUIM_NList_InsertSingle, msg->Entry, MUIV_NList_Insert_Top );

    } else {
        struct MinList                 *list;
        struct MUIS_Listtree_TreeNode  *tn;

        switch( msg->Entry->Type ) {

            case CME_USER:
                list = &data->CMData->Users->Entries;
                break;

            case CME_WWW:
                list = &data->CMData->WWWs->Entries;
                break;

            case CME_FTP:
                list = &data->CMData->FTPs->Entries;
                break;

            case CME_CHAT:
                list = &data->CMData->Chat->Entries;
                break;

            case CME_TELNET:
                list = &data->CMData->Telnet->Entries;
                break;
        }

        // shall we add to the current group?
        if(( tn = (struct MUIS_Listtree_TreeNode *)
                    DoMethod( data->Tree, MUIM_Listtree_GetEntry,
                              MUIV_Listtree_GetEntry_ListNode_Active,
                              MUIV_Listtree_GetEntry_Position_Active, 0 )) &&
            (( struct MinList * )tn->tn_User == list )) {

            DoMethod( data->List, MUIM_NList_InsertSingle, msg->Entry, MUIV_NList_Insert_Bottom );

            set( data->List, MUIA_NList_Active, MUIV_NList_Active_Bottom );

        } else // not in the visible list
            AddHead(( struct List * )list, ( struct Node * )msg->Entry );
    }

    set( obj, MUIA_CManager_Changed, TRUE );

    DoMethod( obj, MUIM_CManager_SaveData, FALSE, NULL );

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_LoadPrefs
static ULONG CManager_LoadPrefs( struct IClass *cl, Object *obj, struct MUIP_CManager_LoadPrefs *msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    TEXT                    PrefsFile[256], file[256];

    DBG( kprintf( "start\n" ));

    ObtainSemaphore( &PrefsSem );

    strcpy( PrefsFile, CMPath );

    if( data->User[0] )
        sprintf( file, "%s/%s.prefs", data->User, data->User );
    else
        strcpy( file, "CManager.prefs" );

    AddPart( PrefsFile, file, sizeof( PrefsFile ));

    LoadPrefs( msg->File ? msg->File : PrefsFile );

    ReleaseSemaphore( &PrefsSem );

    DBG( kprintf( "changing SpeedBar's attr\n" ));

    if(!( data->Flags & SETTING_UP ))
        SetAttrs( data->SpeedBar,
                  MUIA_SpeedBar_ViewMode,     ToolbarCfg.ViewMode,
                  MUIA_SpeedBar_Borderless,   ( ToolbarCfg.Flags & MUIV_SpeedBarCfg_Borderless   ) ? TRUE : FALSE,
                  MUIA_SpeedBar_RaisingFrame, ( ToolbarCfg.Flags & MUIV_SpeedBarCfg_Raising      ) ? TRUE : FALSE,
                  MUIA_SpeedBar_SmallImages,  ( ToolbarCfg.Flags & MUIV_SpeedBarCfg_SmallButtons ) ? TRUE : FALSE,
                  MUIA_SpeedBar_Sunny,        ( ToolbarCfg.Flags & MUIV_SpeedBarCfg_Sunny        ) ? TRUE : FALSE,
                  TAG_DONE );

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///
/// CManager_Login
static ULONG CManager_Login( struct IClass *cl, Object *obj, Msg msg )
{
    struct CManagerData    *data = INST_DATA( cl, obj );
    STRPTR                  user;

    DBG( kprintf( "start\n" ));

    get( data->PopUser, MUIA_String_Contents, &user );

    if( GenesisBase && strcmp( user, data->User )) {
        struct User    *ud;

        if(!( ud = GetUser( user, NULL, NULL, 0 )))
            return( 0 );

        FreeUser( ud );

        DoMethod( obj, MUIM_CManager_LoadData, user, NULL );
    }

    DBG( kprintf( "end\n" ));

    return( 0 );
}
///


/// User_ObjStrHook
static void SAVEDS ASM User_ObjStrHook( REG( a0 ) struct Hook *hook, REG( a2 ) Object *list, REG( a1 ) Object *str )
{
    STRPTR  entry;

    DoMethod( list, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &entry );

    set( str, MUIA_String_Contents, entry );

    DoMethod(( Object * )hook->h_Data, MUIM_CManager_Login );
}
///

/// Tree_DispHook
static ULONG SAVEDS ASM Tree_DispHook( REG( a2 ) STRPTR *array, REG( a1 ) struct MUIS_Listtree_TreeNode *e )
{
    struct CMGroup *entry;
    static TEXT     buffer[80];
    STRPTR          format;

    entry = (struct CMGroup *) e->tn_User;

    if( entry->Flags & GRPF_BOLD )
        format = "\033b%.77s";
    else
        format = "%.79s";

    sprintf( buffer, format, entry->Name );

    *array = buffer;

    return( 0 );
}
///
/// Tree_DestHook
static void SAVEDS ASM Tree_DestHook( REG( a1 ) struct CMGroup *entry )
{
    struct CMUser  *sub;

    while( sub = (struct CMUser *) RemTail(( struct List * )&entry->Entries ))
        CM_FreeEntry( sub );

    if( entry->Type != CME_SECTION ) {

        Remove(( struct Node * )entry );

        CM_FreeEntry( entry );
    }
}
///
/// Tree_OpenHook
static ULONG SAVEDS ASM Tree_OpenHook( REG( a1 ) struct MUIS_Listtree_TreeNode *e )
{
    struct CMGroup *entry;

    entry = (struct CMGroup *) e->tn_User;

    if( entry )
        entry->Flags |= GRPF_OPEN;

    return( 0 );
}
///
/// Tree_CloseHook
static ULONG SAVEDS ASM Tree_CloseHook( REG( a1 ) struct MUIS_Listtree_TreeNode *e )
{
    struct CMGroup *entry;

    entry = (struct CMGroup *) e->tn_User;

    if( entry )
        entry->Flags &= ~GRPF_OPEN;

    return( 0 );
}
///

/// List_DispHook
static ULONG SAVEDS ASM List_DispHook( REG( a0 ) struct Hook *Hook, REG( a2 ) STRPTR *array, REG( a1 ) struct CMUser *entry )
{
    if( entry == NULL ) {
        struct EntryListData   *data = INST_DATA( CL_EntryList->mcc_Class, (Object *)Hook->h_Data );

        switch( data->Type ) {

            case CME_USER: {
                ULONG           image = data->Order.Invert ? MUII_TapeUp : MUII_TapeDown;
                static TEXT     name[100], lastname[100];

                sprintf( name,     data->Order.OrderByName  ? "%s \33I[6:%ld]" : "%s", CM_GetString( HDR_NAME      ), image );
                sprintf( lastname, !data->Order.OrderByName ? "%s \33I[6:%ld]" : "%s", CM_GetString( HDR_LAST_NAME ), image );

                *array++ = name;
                *array++ = lastname;
                *array++ = CM_GetString( HDR_EMAIL );
            }   break;

            case CME_WWW:
            case CME_FTP:
                *array++ = CM_GetString( HDR_NAME          );
                *array++ = CM_GetString( HDR_SITE          );
                *array++ = CM_GetString( HDR_LAST_VISITED  );
                *array++ = CM_GetString( HDR_LAST_MODIFIED );
                break;

            case CME_CHAT:
                *array++ = CM_GetString( HDR_CHANNEL );
                *array++ = CM_GetString( HDR_SERVER  );
                break;

            case CME_TELNET:
                *array++ = CM_GetString( HDR_HOST );
                break;
        }

        *array = CM_GetString( HDR_COMMENT );

    } else { 
        static TEXT     LastVisited[ LEN_DATSTRING * 2 ], LastModified[ LEN_DATSTRING * 2 ];
        TEXT            date[ LEN_DATSTRING ], time[ LEN_DATSTRING ];
        struct DateTime dt;

        switch( entry->Type ) {

            case CME_USER:
                *array++ = entry->Name;
                *array++ = entry->LastName;
                *array++ = entry->EMail[0] ? entry->EMail : entry->BEMail;
                *array   = entry->Comment;
                break;

            case CME_WWW:
                dt.dat_Stamp   = (( struct CMWWW *)entry )->LastVisited;
                dt.dat_Format  = FORMAT_CDN;
                dt.dat_Flags   = 0;
                dt.dat_StrDay  = NULL;
                dt.dat_StrDate = date;
                dt.dat_StrTime = time;

                DateToStr( &dt );

                sprintf( LastVisited, "%s %s", date, time );

                dt.dat_Stamp   = (( struct CMWWW *)entry )->LastModified;
                dt.dat_Format  = FORMAT_CDN;
                dt.dat_Flags   = 0;
                dt.dat_StrDay  = NULL;
                dt.dat_StrDate = date;
                dt.dat_StrTime = time;

                DateToStr( &dt );

                sprintf( LastModified, "%s %s", date, time );

                *array++ = ((struct CMWWW *)entry )->Name;
                *array++ = ((struct CMWWW *)entry )->WWW;
                *array++ = LastVisited;
                *array++ = LastModified;
                *array   = ((struct CMWWW *)entry )->Comment;
                break;

            case CME_FTP:
                dt.dat_Stamp   = (( struct CMFTP *)entry )->LastVisited;
                dt.dat_Format  = FORMAT_CDN;
                dt.dat_Flags   = 0;
                dt.dat_StrDay  = NULL;
                dt.dat_StrDate = date;
                dt.dat_StrTime = time;

                DateToStr( &dt );

                sprintf( LastVisited, "%s %s", date, time );

                dt.dat_Stamp   = (( struct CMFTP *)entry )->LastModified;
                dt.dat_Format  = FORMAT_CDN;
                dt.dat_Flags   = 0;
                dt.dat_StrDay  = NULL;
                dt.dat_StrDate = date;
                dt.dat_StrTime = time;

                DateToStr( &dt );

                sprintf( LastModified, "%s %s", date, time );

                *array++ = ((struct CMFTP *)entry )->Name;
                *array++ = ((struct CMFTP *)entry )->FTP;
                *array++ = LastVisited;
                *array++ = LastModified;
                *array   = ((struct CMFTP *)entry )->Comment;
                break;

            case CME_CHAT:
                *array++ = ((struct CMChat *)entry )->Channel;
                *array++ = ((struct CMChat *)entry )->Server;
                *array   = ((struct CMChat *)entry )->Comment;
                break;

            case CME_TELNET:
                *array++ = ((struct CMTelnet *)entry )->Host;
                *array   = ((struct CMTelnet *)entry )->Comment;
                break;
        }
    }

    return( 0 );
}
///
/// List_SortHook
static LONG SAVEDS ASM List_SortHook( REG( a0 ) struct Hook *Hook, REG( a1 ) struct CMUser *A, REG( a2 ) struct CMUser *B )
{
    struct EntryListData   *data = INST_DATA( CL_EntryList->mcc_Class, (Object *)Hook->h_Data );
    LONG                    ret = 0;

    if( A && B ) switch( A->Type ) {

        case CME_USER:
            ret = StrnCmp( Locale, data->Order.OrderByName ? A->Name : A->LastName, data->Order.OrderByName ? B->Name : B->LastName, -1, SC_COLLATE2 );

            if( data->Order.Invert )
                ret = -ret;
            break;

        case CME_WWW:
            ret = StrnCmp( Locale, (( struct CMWWW * )A )->Name, (( struct CMWWW * )B )->Name, -1, SC_COLLATE2 );
            break;

        case CME_FTP:
            ret = StrnCmp( Locale, (( struct CMFTP * )A )->Name, (( struct CMFTP * )B )->Name, -1, SC_COLLATE2 );
            break;

        case CME_CHAT:
            ret = StrnCmp( Locale, (( struct CMChat * )A )->Channel, (( struct CMChat * )B )->Channel, -1, SC_COLLATE2 );
            break;

        case CME_TELNET:
            ret = StrnCmp( Locale, (( struct CMTelnet * )A )->Host, (( struct CMTelnet * )B )->Host, -1, SC_COLLATE2 );
            break;
    }

    return( ret );
}
///

/// InsertGroups
static void InsertGroups( struct CManagerData *data )
{
    struct MUIS_Listtree_TreeNode  *tn, *first = NULL;

    DBG( if( !data->CMData ) { kprintf( "AAAaaaaaaaaaaaaaAAAAAAAAaaaaaaaaarghhhhh!\n" ); return; } );

    set( data->Tree, MUIA_Listtree_Quiet, TRUE );

    if(!( data->Flags & HIDE_USERS )) {

        /*  Users   */
        first = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_Insert,
                          "M", data->CMData->Users, NULL, MUIV_Listtree_Insert_PrevNode_Tail,
                          ( data->CMData->Users->Flags & GRPF_OPEN ) ? ( TNF_LIST | TNF_OPEN ) : TNF_LIST );

        InsertSubGroups( data->Tree, first, data->CMData->Users );
    }

    if(!( data->Flags & HIDE_WWW )) {

        /*  WWW     */
        tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_Insert,
                          "M", data->CMData->WWWs, NULL, MUIV_Listtree_Insert_PrevNode_Tail,
                          ( data->CMData->WWWs->Flags & GRPF_OPEN ) ? ( TNF_LIST | TNF_OPEN ) : TNF_LIST );

        InsertSubGroups( data->Tree, tn, data->CMData->WWWs );

        if( !first )
            first = tn;
    }

    if(!( data->Flags & HIDE_FTP )) {

        /*  FTP     */
        tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_Insert,
                          "M", data->CMData->FTPs, NULL, MUIV_Listtree_Insert_PrevNode_Tail,
                          ( data->CMData->FTPs->Flags & GRPF_OPEN ) ? ( TNF_LIST | TNF_OPEN ) : TNF_LIST );

        InsertSubGroups( data->Tree, tn, data->CMData->FTPs );

        if( !first )
            first = tn;
    }

    if(!( data->Flags & HIDE_CHAT )) {

        /*  Chat    */
        tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_Insert,
                          "M", data->CMData->Chat, NULL, MUIV_Listtree_Insert_PrevNode_Tail,
                          ( data->CMData->Chat->Flags & GRPF_OPEN ) ? ( TNF_LIST | TNF_OPEN ) : TNF_LIST );

        InsertSubGroups( data->Tree, tn, data->CMData->Chat );

        if( !first )
            first = tn;
    }

    if(!( data->Flags & HIDE_TELNET )) {

        /*  Telnet  */
        tn = (struct MUIS_Listtree_TreeNode *)
                DoMethod( data->Tree, MUIM_Listtree_Insert,
                          "M", data->CMData->Telnet, NULL, MUIV_Listtree_Insert_PrevNode_Tail,
                          ( data->CMData->Telnet->Flags & GRPF_OPEN ) ? ( TNF_LIST | TNF_OPEN ) : TNF_LIST );

        InsertSubGroups( data->Tree, tn, data->CMData->Telnet );

        if( !first )
            first = tn;
    }

    set( data->Tree, MUIA_Listtree_Quiet, FALSE );

    if( first )
        set( data->Tree, MUIA_Listtree_Active, first );
}
///
/// InsertSubGroups
static void InsertSubGroups( Object *Tree, struct MUIS_Listtree_TreeNode *tn, struct CMGroup *Group )
{
    struct CMGroup *entry;

    for( entry = (struct CMGroup *) Group->SubGroups.mlh_Head; entry->Succ; entry = entry->Succ ) {
        struct MUIS_Listtree_TreeNode  *tn2;

        tn2 = (struct MUIS_Listtree_TreeNode *)
                DoMethod( Tree, MUIM_Listtree_Insert,
                          "M", entry, tn, MUIV_Listtree_Insert_PrevNode_Tail,
                          ( entry->Flags & GRPF_OPEN ) ? ( TNF_LIST | TNF_OPEN ) : TNF_LIST );

        InsertSubGroups( Tree, tn2, entry );
    }
}
///
/// RebuildTree
void RebuildTree( struct CManagerData *data )
{
    DBG( if( !data->CMData ) { kprintf( "AAAaaaaaaaaaaaaaAAAAAAAAaaaaaaaaarghhhhh!\n" ); return; } );

    if(!( data->Flags & HIDE_USERS ))
        RebuildTreeList( data, &data->CMData->Users->SubGroups, (APTR)DoMethod( data->Tree, MUIM_Listtree_GetEntry, MUIV_Listtree_GetEntry_ListNode_Root, 0, MUIV_Listtree_GetEntry_Flags_SameLevel ));

    if(!( data->Flags & HIDE_WWW ))
        RebuildTreeList( data, &data->CMData->WWWs->SubGroups,  (APTR)DoMethod( data->Tree, MUIM_Listtree_GetEntry, MUIV_Listtree_GetEntry_ListNode_Root, 1, MUIV_Listtree_GetEntry_Flags_SameLevel ));

    if(!( data->Flags & HIDE_FTP ))
        RebuildTreeList( data, &data->CMData->FTPs->SubGroups,  (APTR)DoMethod( data->Tree, MUIM_Listtree_GetEntry, MUIV_Listtree_GetEntry_ListNode_Root, 2, MUIV_Listtree_GetEntry_Flags_SameLevel ));

    if(!( data->Flags & HIDE_CHAT ))
        RebuildTreeList( data, &data->CMData->Chat->SubGroups,  (APTR)DoMethod( data->Tree, MUIM_Listtree_GetEntry, MUIV_Listtree_GetEntry_ListNode_Root, 3, MUIV_Listtree_GetEntry_Flags_SameLevel ));

    if(!( data->Flags & HIDE_TELNET ))
        RebuildTreeList( data, &data->CMData->Telnet->SubGroups,  (APTR)DoMethod( data->Tree, MUIM_Listtree_GetEntry, MUIV_Listtree_GetEntry_ListNode_Root, 4, MUIV_Listtree_GetEntry_Flags_SameLevel ));
}
///
/// RebuildTreeList
static void RebuildTreeList( struct CManagerData *data, struct MinList *List, APTR Node )
{
    struct MUIS_Listtree_TreeNode  *tn;
    ULONG                           i = 0;

    DBG( if( !List ) { kprintf( "AAAaaaaaaaaaaaaaAAAAAAAAaaaaaaaaarghhhhh!\n" ); return; } );

    NewList(( struct List * )List );

    while( tn = (struct MUIS_Listtree_TreeNode *)
           DoMethod( data->Tree, MUIM_Listtree_GetEntry, Node, i++, MUIV_Listtree_GetEntry_Flags_SameLevel )) {

        struct CMGroup                 *group;
        struct MUIS_Listtree_TreeNode  *tn2;
        ULONG                           i2 = 0;

        group = (struct CMGroup *) tn->tn_User;

        if(( group != data->CMData->Users ) && ( group != data->CMData->FTPs ) && ( group != data->CMData->WWWs ) && ( group != data->CMData->Chat ) && ( group != data->CMData->Telnet ))
            AddTail(( struct List * )List, ( struct Node * )group );

        NewList(( struct List * )&group->SubGroups );

        while( tn2 = (struct MUIS_Listtree_TreeNode *)DoMethod( data->Tree, MUIM_Listtree_GetEntry, tn, i2++, MUIV_Listtree_GetEntry_Flags_SameLevel )) {
            struct CMGroup     *group2;

            group2 = (struct CMGroup *) tn2->tn_User;

            AddTail(( struct List * )&group->SubGroups, ( struct Node * )group2 );

            RebuildTreeList( data, &group2->SubGroups, tn2 );
        }
    }
}
///
/// RebuildList
void RebuildList( Object *Tree, Object *List, struct MUIS_Listtree_TreeNode *tn )
{

    if( tn && ( tn != MUIV_Listtree_Active_Off )) {
        struct CMGroup *entry;
        ULONG               i;

        entry = (struct CMGroup *) tn->tn_User;

        if( !entry ) // paranoia
            return;

        for( i = 0;; i++ ) {
            struct CMUser  *user;

            DoMethod( List, MUIM_NList_GetEntry, i, &user );

            if( !user )
                break;

            if( i == 0 )
                NewList(( struct List * )&entry->Entries );

            AddTail(( struct List * )&entry->Entries, ( struct Node * )user );
        }
    }
}
///

/// ClearList
static void ClearList( Object *List )
{
    set( List, MUIA_Listtree_Quiet, TRUE );

    nnset( List, MUIA_Listtree_Active, MUIV_Listtree_Active_Off );

    DoMethod( List, MUIM_Listtree_Remove,
              MUIV_Listtree_Remove_ListNode_Root,
              MUIV_Listtree_Remove_TreeNode_All, 0 );
}
///

/// StartDOSNotify
static void StartDOSNotify( Object *obj, struct CManagerData *data )
{
    if( data->CMData && data->CMData->Path[0] &&
        !( data->Flags & NOTIFIED ) && ( data->Flags & SETUP_DONE )) {

        DBG( kprintf( "extract date stamp\n" ));

        ExtractDateStamp( data->CMData->Path, &data->FileDS );

        DBG( kprintf( "adding the input handler\n" ));

        data->Handler.ihn_Object  = obj;
        data->Handler.ihn_Signals = 1L << data->NotifyReq.nr_stuff.nr_Signal.nr_SignalNum;
        data->Handler.ihn_Flags   = 0;
        data->Handler.ihn_Method  = MUIM_CManager_HandleNotify;

        DoMethod(( Object * )xget( obj, MUIA_ApplicationObject ), MUIM_Application_AddInputHandler, &data->Handler );

        data->NotifyReq.nr_Name  = data->CMData->Path;
        data->NotifyReq.nr_Flags = NRF_SEND_SIGNAL | NRF_NOTIFY_INITIAL;

        DBG( kprintf( "starting notify\n" ));

        StartNotify( &data->NotifyReq );

        data->Flags |= NOTIFIED;
    }
}
///
/// StopDOSNotify
static void StopDOSNotify( Object *obj, struct CManagerData *data )
{
    if( data->Flags & NOTIFIED ) {

        DBG( kprintf( "stop notify\n" ));

        EndNotify( &data->NotifyReq );

        DBG( kprintf( "removing the input handler\n" ));

        DoMethod(( Object * )xget( obj, MUIA_ApplicationObject ), MUIM_Application_RemInputHandler, &data->Handler );

        data->Flags &= ~NOTIFIED;
    }
}
///
/// ExtractDateStamp
static void ExtractDateStamp( STRPTR File, struct DateStamp *ds )
{
    struct FileInfoBlock   *fib;

    memset( ds, 0, sizeof( struct DateStamp ));

    if( fib = AllocDosObject( DOS_FIB, NULL )) {
        BPTR    lock;

        if( lock = Lock( File, ACCESS_READ )) {

            Examine( lock, fib );

            CopyMem( &fib->fib_Date, ds, sizeof( struct DateStamp ));

            UnLock( lock );
        }

        FreeDosObject( DOS_FIB, fib );
    }
}
///

/// GetFile
BOOL GetFile( Object *obj, struct CManagerData *data, STRPTR File, STRPTR Pattern, BOOL SaveMode )
{
    BOOL    ok = FALSE;

    if( data->Req ) {
        TEXT    dir[256];
        APTR    scr = NULL, Win;

        DBG( kprintf( "Req ok\n" ));

        strcpy( dir, File );

        *( PathPart( dir )) = '\0';

        get( obj, MUIA_WindowObject,  &Win );

        if( Win )
            get( Win, MUIA_Window_Screen, &scr );

        if( MUI_AslRequestTags( data->Req,
                                ASLFR_DoPatterns,                                TRUE,
                                ASLFR_DoSaveMode,                                SaveMode,
                                Pattern ? ASLFR_InitialPattern : TAG_IGNORE,     Pattern,
                                File[0] ? ASLFR_InitialFile : TAG_IGNORE,        FilePart( File ),
                                File[0] ? ASLFR_InitialDrawer : TAG_IGNORE,      dir,
                                scr ? ASLFR_Screen : TAG_IGNORE,                 scr,
                                TAG_DONE )) {

            strcpy( File, data->Req->fr_Drawer );
            AddPart( File, data->Req->fr_File, 256 );

            ok = TRUE;
        }
    }

    return( ok );
}
///
/// GetDir
BOOL GetDir( Object *obj, STRPTR Title, STRPTR Path, BOOL SaveMode )
{
    BOOL                    ok = FALSE;
    struct FileRequester   *Req;

    if( Req = MUI_AllocAslRequest( ASL_FileRequest, NULL )) {
        APTR    scr = NULL, Win = NULL;

        DBG( kprintf( "Req ok\n" ));

        if( obj ) {

            get( obj, MUIA_WindowObject,  &Win );

            if( Win )
                get( Win, MUIA_Window_Screen, &scr );
        }

        if( MUI_AslRequestTags( Req,
                                ASLFR_DrawersOnly,                              TRUE,
                                ASLFR_DoSaveMode,                               SaveMode,
                                Title[0] ? ASLFR_TitleText : TAG_IGNORE,        Title,
                                Path[0] ? ASLFR_InitialDrawer : TAG_IGNORE,     Path,
                                scr ? ASLFR_Screen : TAG_IGNORE,                scr,
                                TAG_DONE )) {

            strcpy( Path, Req->fr_Drawer );

            ok = TRUE;
        }

        MUI_FreeAslRequest( Req );
    }

    return( ok );
}
///

