/***************************************************************************
    revision             : $Id: main.h,v 1.1.1.1 2003-01-06 15:53:55 tellini Exp $
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

#ifndef MAIN_H
#define MAIN_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

/// Methods
#define MUIM_MainWindow_Open            (TAGBASE + 1)
#define MUIM_MainWindow_Close           (TAGBASE + 2)
#define MUIM_MainWindow_Load            (TAGBASE + 3)
#define MUIM_MainWindow_Import          (TAGBASE + 4)
#define MUIM_MainWindow_About           (TAGBASE + 5)
#define MUIM_MainWindow_Prefs           (TAGBASE + 6)
#define MUIM_MainWindow_Export          (TAGBASE + 7)
///
/// Structs
#ifndef ORDERPREFS
struct OrderPrefs {
        BOOL    OrderByName;
        BOOL    Invert;
};

#define ORDERPREFS
#endif

struct CMCountry {
        struct CMCountry   *Succ;
        struct CMCountry   *Pred;
        TEXT                Country[80];
        TEXT                Country2[80];
        TEXT                Code[8];
};

struct MainWindowData {
        Object                     *CMObj;
        struct FileRequester       *Req;
        struct Hook                 ClickHook;
};

struct MUIP_MainWindow_Load {
        ULONG               MethodID;
        ULONG               Type;
};

struct MUIP_MainWindow_Import {
        ULONG               MethodID;
        ULONG               From;
};

struct MUIP_MainWindow_Export {
        ULONG               MethodID;
        ULONG               Type;
};

struct MUIP_MainWindow_Close {
        ULONG               MethodID;
        ULONG               Save;
};
///
/// Externs
extern ULONG SAVEDS ASM MainWindow_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );

extern struct CMGroup  *GetParent( struct CMGroup *, struct CMGroup * );
extern void             RebuildList( Object *, Object *, struct MUIS_Listtree_TreeNode * );
extern void             RebuildTree( Object * );
extern void             FPutString( BPTR, STRPTR );
extern void             FGetString( BPTR, STRPTR );
extern void             SendLibMsg( ULONG, APTR );

extern struct MsgPort  *MyLibPort;
extern struct MsgPort  *LibPort;
extern UBYTE            Flags;
extern struct MinList   Countries;

extern struct OrderPrefs    Order;

extern Object          *MainWin;
///

#endif /* MAIN_H */
