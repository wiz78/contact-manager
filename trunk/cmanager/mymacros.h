/***************************************************************************
    revision             : $Id: mymacros.h,v 1.1.1.1 2003-01-06 15:54:10 tellini Exp $
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

#ifndef MYMACROS_H
#define MYMACROS_H

/*                      Support stuff                   */
#ifndef REG
#ifdef _DCC
#define REG(x) __ ## x
#else
#define REG(x) register __ ## x
#endif
#endif

#ifndef ASM
#if defined __MAXON__ || defined __STORM__ || defined _DCC
#define ASM
#else
#define ASM __asm
#endif
#endif

#ifndef SAVEDS
#ifdef __MAXON__
#define SAVEDS
#endif
#if defined __STORM__ || defined __SASC
#define SAVEDS __saveds
#endif
#if defined _GCC || defined _DCC
#define SAVEDS __geta4
#endif
#endif


/*                      Miscellaneous                   */
#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif


/*                      MUI related                     */
#define METHOD(name)        ULONG name( struct IClass *cl, Object *obj, struct opSet *msg )
#define METHOD2(name,data)  ULONG name( struct IClass *cl, Object *obj, struct data *msg )

#define DISPATCHER(name)    ULONG SAVEDS ASM name( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg ) { switch( msg->MethodID ) {
#define ENDDISP             return( DoSuperMethodA( cl, obj, msg )); }

#define CALLM(name)         case MUIM_##name: return( name( cl, obj, (APTR)msg ))
#define CALLM2(method,name) case method: return( name( cl, obj, (APTR)msg ))

#endif
