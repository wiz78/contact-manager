/***************************************************************************
    revision             : $Id: mui.h,v 1.1.1.1 2003-01-06 16:00:32 tellini Exp $
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

#ifndef MUI_H
#define MUI_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

/// Methods
#define MUIM_MyApp_DisposeObj           (TAGBASE + 1)
#define MUIM_MyApp_Message              (TAGBASE + 2)
///
/// Structs
struct AppData {
            struct MUI_InputHandlerNode Handler;
};

struct MUIP_MyApp_DisposeObj {
            ULONG   MethodID;
            Object *Object;
};
///
/// Externs
extern struct MUI_CustomClass  *CL_MyApplication;
extern struct MUI_CustomClass  *CL_MainWindow;
extern struct MUI_CustomClass  *CL_Edit;
extern struct MUI_CustomClass  *CL_EditWWW;
extern struct MUI_CustomClass  *CL_EditFTP;

extern struct Catalog          *Catalog;

extern Object  *MakeButton( ULONG, LONG );
extern Object  *MakeButton2( STRPTR );
extern Object  *MakeSlider( ULONG, LONG, LONG );
extern Object  *MakeCheck( ULONG, LONG );
extern Object  *MakeLabel1( ULONG );
extern Object  *MakeString( ULONG, ULONG, LONG );
extern Object  *MakeSecretString( ULONG, ULONG, LONG );
extern Object  *MakeInteger( ULONG, ULONG, LONG );
extern Object  *MakeCycle( STRPTR *, ULONG );
extern Object  *MakeFile( Object **, ULONG, ULONG, LONG );
extern void     SetupLocale( void );
extern void     LocalizeMenus( struct NewMenu * );
extern void     LocalizeArray( UBYTE ** );
extern void     LocalizeSpeedBar( struct MUIS_SpeedBar_Button *button );
extern BOOL     InitClasses( void );
extern void     ExitClasses( void );

#undef set
#undef get
#undef nnset
#define set(a,b,c)      mui_set((a),(b),(APTR)(c))
#define get(a,b,c)      mui_get((a),(b),(c))
#define nnset(a,b,c)    mui_nnset((a),(b),(LONG)(c))

extern void     mui_set( APTR, ULONG, APTR );
extern void     mui_get( APTR, ULONG, APTR );
extern void     mui_nnset( APTR, ULONG, LONG );
extern LONG     xget( Object *, ULONG );
extern void     getstr( Object *, STRPTR );

extern ULONG    DoSuperNew( struct IClass *, Object *, ULONG, ... );
extern void     RemoveWindow( Object * );
///

#endif /* MUI_H */
