/***************************************************************************
    revision             : $Id: dial.h,v 1.1.1.1 2003-01-06 15:58:03 tellini Exp $
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

#ifndef DIAL_H
#define DIAL_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

/// Methods
#define MUIM_DialWindow_Open            (TAGBASE + 1)
#define MUIM_DialWindow_Close           (TAGBASE + 2)
///
/// Tags
#ifdef CMANAGER_MCC
#define MUIA_DialWindow_CMObj           (TAGBASE + 1)
#endif
///
/// Externs
extern struct MUI_CustomClass  *CL_DialWindow;

extern ULONG SAVEDS ASM DialWindow_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );

extern TEXT     SerDevice[256];
extern UBYTE    SerUnit;
extern BOOL     SerShared;
extern TEXT     SerInit[256];
extern TEXT     DialCommand[30];

extern UWORD    RedialDelay, RedialAttempts;
extern BOOL     LogCalls;
///
/// Structs
struct DialWindowData {
        Object             *AppObj;
#ifdef CMANAGER_MCC
        Object             *CMObj;
#endif
        Object             *Text;
        Object             *MainWnd;
        BOOL                Aborted;
        struct timerequest *TimeoutIO;
        struct IOExtSer    *iow, *ior;
        struct MsgPort     *Port, *TimeoutPort;
        TEXT                LastCmd[256];
};

struct MUIP_DialWindow_Open {
        ULONG       MethodID;
        STRPTR      Number;
        Object     *MainWnd;
};
///


#endif /* DIAL_H */
