/*
**
** Contact Manager - An Amiga Centralized Bookmarks System
**
** Copyright 1999-2006 by Simone [wiz] Tellini https://tellini.info/
** Copyright 2003-2006 by Alfonso [alfie] Ranieri <alforan@tin.it>
** All Rights Are Reserved.
**
** Destributed Under The Terms Of The LGPL II
**
**
**/

#ifndef _CM_PROTOS_H
#define _CM_PROTOS_H

/* cmapp.c */
ULONG initCMAppClass ( void );

/* cmwin.c */
ULONG initCMWinClass ( void );

/* about.c */
ULONG initCMAboutClass ( void );

/* parseArgs.c */
LONG parseArgs ( struct parseArgs *pa );

/* utils.c */
#ifdef __MORPHOS__
void msprintf ( STRPTR buf , STRPTR fmt , ...) __attribute((varargs68k));
#else
void STDARGS msprintf ( STRPTR buf , STRPTR fmt , ...);
ULONG STDARGS DoSuperNew ( struct IClass *cl , Object *obj , ULONG tag1 , ...);
#endif
ULONG openWindow ( Object *app , Object *win );
void localizeMenus ( struct NewMenu *menu );

#endif /* _CM_PROTOS_H */
