/***************************************************************************
    revision             : $Id: import.h,v 1.1.1.1 2003-01-06 15:52:58 tellini Exp $
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

#ifndef IMPORT_H
#define IMPORT_H


/// Methods
#define MUIM_ListWindow_Open            (TAGBASE + 1)
#define MUIM_ListWindow_Close           (TAGBASE + 2)
#define MUIM_ListWindow_Result          (TAGBASE + 3)
///
/// Structs
struct ListWindowData {
        Object         *List;
        struct Node    *Node;
};

struct MUIP_ListWindow_Open {
        ULONG           MethodID;
        struct List    *List;
};

struct MUIP_ListWindow_Close {
        ULONG           MethodID;
        ULONG           Ok;
};
///
/// Externs
extern struct MUI_CustomClass  *CL_ListWindow;

extern void ImportVoyager ( BPTR, struct CMGroup *, BOOL );
extern void ImportIB ( BPTR, struct CMGroup *, BOOL );
extern void ImportAWeb( BPTR, struct CMGroup *, BOOL );
extern void ImportAmFTP( BPTR, struct CMGroup *, BOOL );
extern BOOL ImportTHOR( Object *, struct CMGroup * );
extern void ImportMD( BPTR, struct CMGroup *, BOOL );
extern void ImportYAM( BPTR, struct CMGroup *, BOOL );
extern void ImportSTRICQ( BPTR, struct CMGroup *, BOOL );
extern void ImportPINT( BPTR, struct CMGroup *, BOOL );
extern void ImportGoodNews( BPTR, struct CMGroup *, BOOL );
extern void ImportDFA( BPTR, struct CMGroup *, BOOL );
extern void ImportSTFax( BPTR, struct CMGroup *, BOOL );
extern void ImportAmIRC( BPTR, struct CMGroup *, BOOL );
extern void ImportDOpusFTP( BPTR, struct CMGroup *, BOOL );

extern ULONG SAVEDS ASM ListWindow_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///

#endif /* IMPORT_H */
