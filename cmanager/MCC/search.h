/***************************************************************************
    revision             : $Id: search.h,v 1.1.1.1 2003-01-06 16:00:56 tellini Exp $
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

#ifndef SEARCH_H
#define SEARCH_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

#ifdef String
#undef String
#endif

/// Methods
#define MUIM_SearchWindow_Open          (TAGBASE + 1)
#define MUIM_SearchWindow_Close         (TAGBASE + 2)
#define MUIM_SearchWindow_Search        (TAGBASE + 3)
///
/// Structs
struct SearchWindowData {
        Object                 *String;
        Object                 *Users;
        Object                 *WWW;
        Object                 *FTP;
        Object                 *Chat;
        Object                 *CMObj;
};

struct MUIP_SearchWindow_Open {
        ULONG                   MethodID;
        Object                 *CMObj;
};

struct MUIP_SearchWindow_Search {
        ULONG       MethodID;
        ULONG       Type;
};
///
/// Externs
extern struct MUI_CustomClass  *CL_SearchWindow;

extern ULONG CManager_Search( struct IClass *cl, Object *obj, Msg msg );

extern ULONG SAVEDS ASM SearchWindow_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///

#endif /* SEARCH_H */
