/***************************************************************************
    revision             : $Id: rexx.h,v 1.1.1.1 2003-01-06 15:54:48 tellini Exp $
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

#ifndef REXX_H
#define REXX_H


extern ULONG SendRexxMsg( STRPTR, STRPTR, STRPTR, STRPTR );

extern LONG SAVEDS ASM GetEntryRexxed( REG( a0 ) struct Hook *, REG( a1 ) ULONG *, REG( a2 ) Object * );

#endif /* REXX_H */
