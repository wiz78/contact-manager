/***************************************************************************
    revision             : $Id: entrylist.h,v 1.1.1.1 2003-01-06 15:51:38 tellini Exp $
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

#ifndef ENTRYLIST_H
#define ENTRYLIST_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

/// Methods
#define MUIM_EntryList_Remove           (TAGBASE + 1)
#define MUIM_EntryList_Clone            (TAGBASE + 2)
#define MUIM_EntryList_StartScript      (TAGBASE + 3)
#define MUIM_EntryList_Edit             (TAGBASE + 4)
#define MUIM_EntryList_DoubleClick      (TAGBASE + 5)
#define MUIM_EntryList_HandleARexx      (TAGBASE + 6)
#define MUIM_EntryList_CopyToClip       (TAGBASE + 7)

#define MUIM_EditWindow_Open            (TAGBASE + 1)
#define MUIM_EditWindow_Close           (TAGBASE + 2)
///
/// Tags
#define MUIA_EntryList_Group        (TAGBASE + 1)
#define MUIA_EntryList_Type         (TAGBASE + 2)

#define MUIA_EditWindow_Title       (TAGBASE + 10)
#define MUIA_EditWindow_Object      (TAGBASE + 11)
#define MUIA_EditWindow_List        (TAGBASE + 12)
#define MUIA_EditWindow_ID          (TAGBASE + 13)
#define MUIA_EditWindow_Entry       (TAGBASE + 14)
#define MUIA_EditWindow_Virtual     (TAGBASE + 15)
///
/// Structs
struct EntryListData {
        Object                         *ContextMenu;
        UWORD                           Type;
        struct CMGroup                 *Group;
        struct MUI_InputHandlerNode     RexxHandler;
        struct MsgPort                 *RexxPort;
        ULONG                           RexxOut;
};

struct EditWindowData {
        Object     *Body;
        Object     *List;
};

struct MUIP_EntryList_StartScript {
        ULONG       MethodID;
        ULONG       Script;
};

struct MUIP_EditWindow_Open {
        ULONG           MethodID;
        struct CMUser  *Entry;
};

struct MUIP_EditWindow_Close {
        ULONG       MethodID;
        ULONG       Ok;
};
///
/// Externs
extern struct MUI_CustomClass  *CL_EntryList;
extern struct MUI_CustomClass  *CL_EditWindow;
extern ULONG                    AcceptEntry;

extern ULONG SAVEDS ASM EntryList_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
extern ULONG SAVEDS ASM EditWindow_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///

#endif /* ENTRYLIST_H */
