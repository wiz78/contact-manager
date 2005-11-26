#ifndef _PROTO_STFAX_H
#define _PROTO_STFAX_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef CLIB_STFAX_PROTOS_H
#include <clib/STFax_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *STFaxBase;
#endif

#ifdef __GNUC__
#include <ppcinline/STFax.h>
#elif !defined(__VBCC__)
#include <pragmas/STFax_pragmas.h>
#endif

#endif	/*  _PROTO_STFAX_H  */
