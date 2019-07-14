#ifndef PRAGMAS_GENESIS_PRAGMAS_H
#define PRAGMAS_GENESIS_PRAGMAS_H

/*
**	$VER: genesis_pragmas.h
**	Includes Release
**
**	Direct ROM interface (pragma) definitions.
**
*/

#ifndef CLIB_GENESIS_PROTOS_H
#include <clib/genesis_protos.h>
#endif

#pragma libcall GenesisBase GetFileSize 1e 801
#pragma libcall GenesisBase ParseConfig 24 9802
#pragma libcall GenesisBase ParseNext 2a 801
#pragma libcall GenesisBase ParseNextLine 30 801
#pragma libcall GenesisBase ParseEnd 36 801
#pragma libcall GenesisBase ReallocCopy 3c 9802
#pragma libcall GenesisBase FreeUser 42 801
#pragma libcall GenesisBase GetUserName 48 18003
#pragma libcall GenesisBase GetUser 4e 0A9804
#pragma libcall GenesisBase GetGlobalUser 54 0
#pragma libcall GenesisBase SetGlobalUser 5a 801
#pragma libcall GenesisBase ClearUserList 60 0
#pragma libcall GenesisBase ReloadUserList 66 0
#pragma libcall GenesisBase ReadFile 6c 09803
#pragma libcall GenesisBase WriteFile 72 09803
#pragma libcall GenesisBase IsOnline 78 001

#endif /* PRAGMAS_GENESIS_PRAGMAS_H */
