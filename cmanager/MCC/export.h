/***************************************************************************
    revision             : $Id: export.h,v 1.1.1.1 2003-01-06 15:59:09 tellini Exp $
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

#ifndef EXPORT_H
#define EXPORT_H


extern ULONG CManager_ExportDB( struct IClass *cl, Object *obj, struct MUIP_CManager_Export *msg );


#endif /* EXPORT_H */
