#ifndef LIBRARIES_GENESIS_H
#define LIBRARIES_GENESIS_H

#include <exec/libraries.h>

#if defined(__GNUC__)
# pragma pack(2)
#endif

#define GENESISNAME "AmiTCP:libs/genesis.library"

#define GUF_TextObject  1   /* use a TextObject for loginname instead of StringObject *
                             * => user can't change it                                */

#define IOC_AskUser 1
#define IOC_Force   2

struct User
{
   UBYTE  *us_name;    /* Username */
   UBYTE  *us_passwd;  /* Encrypted password */
   LONG   us_uid;      /* User ID */
   LONG   us_gid;      /* Group ID */
   UBYTE  *us_gecos;   /* Real name etc */
   UBYTE  *us_dir;     /* Home directory */
   UBYTE  *us_shell;   /* Shell */
   ULONG  us_flags;
   ULONG  us_max_time;
};

struct ParseConfig_Data
{
   STRPTR pc_buffer;    /* buffer holding the file (internal use only) */
   LONG   pc_size;      /* holding the size of the buffer (internal use only) */
   STRPTR pc_current;   /* pointer to the current position (internal use only) */

   STRPTR pc_argument;  /* pointer to the argument name */
   STRPTR pc_contents;  /* pointer to the argument's contents */
};

#if defined(__GNUC__)
# pragma pack()
#endif

#endif
