/***************************************************************************
    revision             : $Id: about.h,v 1.1.1.1 2003-01-06 15:50:47 tellini Exp $
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

#ifndef ABOUT_H
#define ABOUT_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

/// Methods
#define MUIM_AboutWindow_Close          (TAGBASE + 1)
///
/// Tags
#define MUIA_AboutWindow_More           (TAGBASE + 1)
#define MUIA_AboutWindow_Limit          (TAGBASE + 2)
#define MUIA_AboutWindow_CMObj          (TAGBASE + 3)
///
/// Externs
extern struct MUI_CustomClass  *CL_AboutWindow;

extern ULONG SAVEDS ASM AboutWindow_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///
/// Structs
#ifdef CMANAGER_MCC
struct AboutData {
        Object *CMObj;
};
#endif

struct MUIP_AboutWindow_Close {
        ULONG   MethodID;
        ULONG   More;
};
///

#endif /* ABOUT_H */
