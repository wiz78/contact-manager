#ifndef _PROTO_GENESIS_H
#define _PROTO_GENESIS_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef CLIB_GENESIS_PROTOS_H
#include <clib/genesis_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *GenesisBase;
#endif

#ifdef __GNUC__
#include <ppcinline/genesis.h>
#elif !defined(__VBCC__)
#include <pragmas/genesis_pragmas.h>
#endif

#endif	/*  _PROTO_GENESIS_H  */
