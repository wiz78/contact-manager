/***************************************************************************
    revision             : $Id: CManagerMsg.h,v 1.1.1.1 2003-01-06 15:55:48 tellini Exp $
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

#ifndef CMANAGERMSG_H
#define CMANAGERMSG_H


struct CMMessage {
        struct Message  Msg;
        ULONG           Command;
        ULONG           Data;
};

#define CMC_QUIT        0
#define CMC_GET_ENTRY   1
#define CMC_PORT        2
#define CMC_AREXX       3


#endif /* CMANAGERMSG_H */
