/***************************************************************************
    revision             : $Id: grouplist.h,v 1.1.1.1 2003-01-06 15:59:15 tellini Exp $
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

#ifndef GROUPLIST_H
#define GROUPLIST_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

/// Structs
struct GroupListData {
        struct MUIS_Listtree_TreeNode  *Node;
        ULONG                           Flags;
        struct CMData                  *CMData;
        Object                         *CMObj;
};
///
/// Tags
#define MUIA_GroupList_CMData       (TAGBASE + 1)
#define MUIA_GroupList_CMObj        (TAGBASE + 2)
///
/// Externs
extern struct MUI_CustomClass  *CL_GroupList;

extern ULONG SAVEDS ASM GroupList_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///

#endif /* GROUPLIST_H */
