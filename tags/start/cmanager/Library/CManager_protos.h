/***************************************************************************
    revision             : $Id: CManager_protos.h,v 1.1.1.1 2003-01-06 15:55:50 tellini Exp $
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

#ifndef CMANAGER_PROTOS_H
#define CMANAGER_PROTOS_H

struct CMGroup  *CM_GetParent( struct CMGroup *, struct CMGroup * );
BOOL             CM_LoadData( STRPTR, struct CMData *, STRPTR );
void             CM_SaveData( STRPTR, struct CMData *, STRPTR );
void             CM_FreeData( struct CMData * );
APTR             CM_StartManager( STRPTR, STRPTR );
void             CM_FreeHandle( APTR, BOOL );
APTR             CM_AllocEntry( ULONG );
void             CM_FreeEntry( APTR );
APTR             CM_GetEntry( APTR, ULONG );
struct BitMap   *CM_CreateBitMap( ULONG, ULONG, ULONG, ULONG, struct BitMap * );
void             CM_DeleteBitMap( struct BitMap * );
BOOL             CM_AddEntry( APTR );
void             CM_FreeList( struct MinList * );
struct CMData   *CM_LoadCurrentUserData( BOOL );
BOOL             CM_LoadDataOld( STRPTR, struct CMDataOld *, STRPTR );
void             CM_SaveDataOld( STRPTR, struct CMDataOld *, STRPTR );
void             CM_FreeDataOld( struct CMDataOld * );
struct CMData   *CM_AllocCMData( void );
STRPTR           CM_GetString( ULONG ID );


#endif /* CMANAGER_PROTOS_H */
