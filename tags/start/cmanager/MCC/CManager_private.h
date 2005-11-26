/***************************************************************************
    revision             : $Id: CManager_private.h,v 1.1.1.1 2003-01-06 15:57:24 tellini Exp $
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

#ifndef CMANAGER_PRIVATE_H
#define CMANAGER_PRIVATE_H

// obsolete stuff
#define MUIA_CManager_Magic     0xF76B0009

struct MCCPrefs {
        UWORD       TreeW;
        UWORD       ListW;
};

struct CManagerData {
        Object                         *PopUser;
        Object                         *Tree;
        Object                         *List;
        Object                         *GroupName;
        Object                         *TreeGroup;
        Object                         *ListGroup;
        Object                         *GroupUp;
        Object                         *GroupDown;
        Object                         *SpeedBar;
        struct FileRequester           *Req;
        struct MUIS_Listtree_TreeNode  *LastActive;
        struct DateStamp                FileDS;
        struct CMData                  *CMData;
        struct NotifyRequest            NotifyReq;
        struct MUI_InputHandlerNode     Handler;
        TEXT                            User[100];
        TEXT                            Path[256];
        struct MCCPrefs                 Prefs;
        UWORD                           Flags;
        struct Hook                     TreeDispHook;
        struct Hook                     TreeDestHook;
        struct Hook                     TreeOpenHook;
        struct Hook                     TreeCloseHook;
        struct Hook                     UserHook;
        struct Hook                     SortHook, DispHook;
        struct Hook                    *DoubleClickHook;
};

#undef SAVED

#define SAVED           (1 << 0)
#define HIDE_USERS      (1 << 1)
#define HIDE_WWW        (1 << 2)
#define HIDE_FTP        (1 << 3)
#define HIDE_CHAT       (1 << 4)
#define APP_DBLCLK      (1 << 5)
#define NOTIFIED        (1 << 6)
#define REGGED          (1 << 7)
#define HIDE_TELNET     (1 << 8)
#define GRABBED_LISTS   (1 << 9)
#define SETUP_DONE      (1 << 10)
#define SETTING_UP      (1 << 11)


/*  Export flags    */
#define MUIV_CManager_Export_NoHead         (1 << 31)


#endif /* CMANAGER_PRIVATE_H */
