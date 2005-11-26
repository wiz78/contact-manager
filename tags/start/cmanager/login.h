/***************************************************************************
    revision             : $Id: login.h,v 1.1.1.1 2003-01-06 15:53:27 tellini Exp $
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

#ifndef LOGIN_H
#define LOGIN_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

/// Methods
#define MUIM_LoginWindow_Close          (TAGBASE + 1)
#define MUIM_LoginWindow_Result         (TAGBASE + 2)
///
/// Externs
extern struct MUI_CustomClass  *CL_LoginWindow;

extern ULONG SAVEDS ASM LoginWindow_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///
/// Structs
struct LoginWindowData {
        Object *User;
        BOOL    Ok;
};

struct MUIP_LoginWindow_Close {
        ULONG   MethodID;
        ULONG   Ok;
};
///

#endif /* LOGIN_H */
