#ifndef _PROTO_GENESIS_H
#define _PROTO_GENESIS_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_GENESIS_PROTOS_H) && !defined(__GNUC__)
#include <clib/genesis_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *GenesisBase;
#endif

#ifdef __GNUC__
#include <ppcinline/genesis.h>
#elif !defined(__VBCC__)
#include <pragma/genesis_lib.h>
#endif

#endif	/*  _PROTO_GENESIS_H  */
