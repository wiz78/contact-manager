/***************************************************************************
    revision             : $Id: image.h,v 1.1.1.1 2003-01-06 15:52:30 tellini Exp $
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

#ifndef IMAGE_H
#define IMAGE_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

#define IMG_WIDTH   75
#define IMG_HEIGHT  75

/// Tags
#define MUIA_ImageGad_Image             (TAGBASE + 1)
///
/// Externs
extern struct MUI_CustomClass  *CL_ImageGad;

extern ULONG SAVEDS ASM ImageGad_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///
/// Structs
struct ImageGadData {
        struct BitMap  *BitMap;
        ULONG          *Pens;
        ULONG           UsedPens;
        struct CMImage *Image;
        BOOL            AllocPens;
};
///

#endif /* IMAGE_H */
