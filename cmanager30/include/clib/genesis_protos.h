#ifndef GENESIS_LIB_PROTOS_H
#define GENESIS_LIB_PROTOS_H

#ifndef LIBRARIES_GENESIS_H
#include <libraries/genesis.h>
#endif

LONG   GetFileSize(STRPTR file);
BOOL   ParseConfig(STRPTR file, struct ParseConfig_Data *pc_data);
BOOL   ParseNext(struct ParseConfig_Data *pc_data);
BOOL   ParseNextLine(struct ParseConfig_Data *pc_data);
void   ParseEnd(struct ParseConfig_Data *pc_data);
void   FreeUser(struct User *user);
STRPTR ReallocCopy(STRPTR *old, STRPTR src);
LONG   ReadFile(STRPTR file, STRPTR buffer, LONG len);
BOOL   WriteFile(STRPTR file, STRPTR buffer, LONG len);

BOOL   GetUserName(LONG user_number, char *buffer, LONG len);
struct User *GetUser(STRPTR name, STRPTR password, STRPTR title, LONG flags);
struct User *GetGlobalUser(void);
void   SetGlobalUser(struct User *user);
void   ClearUserList(void);
BOOL   ReloadUserList(void);
BOOL   IsOnline(LONG flags);

#endif
