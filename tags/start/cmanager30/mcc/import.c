/*
**
** Contact Manager - An Amiga Centralized Bookmarks System
**
** Copyright 1999-2006 by Simone [wiz] Tellini <simone@tellini.info>
** Copyright 2003-2006 by Alfonso [alfie] Ranieri <alforan@tin.it>
** All Rights Are Reserved.
**
** Destributed Under The Terms Of The LGPL II
**
**
**/

#include "class.h"
#include <stdlib.h>
#include <ctype.h>
#include <proto/STFax.h>
#include <proto/expat.h>

/***********************************************************************/

static LONG
getChar(BPTR file)
{
    register LONG c = FGetC(file);

    return (c<0) ? 0 : c;
}

/***********************************************************************/

static void
getString(BPTR file,STRPTR str)
{
    register UWORD len = getChar(file);

    if (len) FRead(file,str,len,1);

    str[len] = '\0';
}

/***************************************************************************/

ULONG
fileNew(STRPTR name,struct DateStamp *stamp)
{
    register ULONG ret = FALSE;

    if (*name)
    {
        register struct FileInfoBlock *fib;

        if (fib = AllocDosObject(DOS_FIB,NULL))
        {
            register BPTR lock;

            if (lock = Lock(name,ACCESS_READ))
            {
                if (Examine(lock,fib))
                    ret = (ULONG)(CompareDates(stamp,&fib->fib_Date)>0);
    
                UnLock(lock);
            }

            FreeDosObject(DOS_FIB,fib);
    	}
    }

    return ret;
}

/***********************************************************************/

void
readStamps(struct CMPrefs *prefs)
{
    register struct FileInfoBlock *fib;

    memset(prefs->Stamps,0,NUM_HOTLISTS*sizeof(struct DateStamp));

    if (fib = AllocDosObject(DOS_FIB,NULL))
    {
        register ULONG i;

        for (i = 0; i<NUM_HOTLISTS; i++)
        {
            if (*prefs->Hotlists[i])
            {
                register BPTR lock;

                if (lock = Lock(prefs->Hotlists[i],ACCESS_READ))
                {
                    Examine(lock,fib);
                    copymem(&prefs->Stamps[i],&fib->fib_Date,sizeof(struct DateStamp));
                    UnLock(lock);
                }
            }
        }

        FreeDosObject(DOS_FIB,fib);
    }
}

/***********************************************************************/

void
saveDates(STRPTR fileName,struct DateStamp *stamps)
{
    register BPTR file;

    if (file = Open(fileName,MODE_NEWFILE))
    {
        Write(file,stamps,NUM_HOTLISTS*sizeof(struct DateStamp));
        Close(file);
    }
}

/***********************************************************************/

static STRPTR
fgets(BPTR file,STRPTR buf,ULONG size)
{
    register STRPTR str;

    *buf = 0;

    if (str = FGets(file,buf,size-1))
    {
        register STRPTR s;

        for (s = str; *s && *s!='\r' && *s!='\n'; s++);
        *s = 0;
    }

    return str;
}

/***********************************************************************/

static STRPTR
getCSVToken(STRPTR str,int comma,STRPTR dest,ULONG maxlen)
{
    register ULONG first = TRUE, quoted = FALSE, lastquote = FALSE, done = FALSE;

    maxlen--; // ending '\0'

    while (str && *str && !done)
    {
        register UBYTE ch, out;

        ch  = *str++;
        out = '\0';

        if (ch=='"')
        {
            if (first) quoted = TRUE;
            else if (lastquote) out = '"';
                 else if (quoted && (*str == comma)) quoted = FALSE;
                      else lastquote = TRUE;
        }
        else
            if (ch==comma)
            {
                if (quoted) out = comma;
                else done = TRUE;
            }
            else
            {
                out = ch;
            }

        if (out && maxlen)
        {
            *dest++ = out;
            maxlen--;
        }

        first = FALSE;
    }

    if (dest) *dest = '\0';

    return str;
}

/***********************************************************************/

static struct CMGroup *
checkGroup(struct CMGroup *group,struct CMGroup *entry)
{
    register struct CMGroup *mstate, *succ;

    if (!*entry->Name) return NULL;

    for (mstate = CMGROUP(group->SubGroups.mlh_Head); succ = mstate->Succ; mstate = succ)
    {
        register struct CMGroup *node;

        if (*mstate->Name && !stricmp(mstate->Name,entry->Name)) return mstate;

        if (node = checkGroup(mstate,entry)) return node;
    }

    return NULL;
}

/***********************************************************************/

static struct CMGroup *
findGroup(struct CMGroup *group,STRPTR name)
{
    register struct CMGroup *mstate, *succ;

    if (!name || !*name) return NULL;

    for (mstate = CMGROUP(group->SubGroups.mlh_Head); succ = mstate->Succ; mstate = succ)
    {
        register struct CMGroup *node;

        if (*mstate->Name && !stricmp(mstate->Name,name)) return mstate;

        if (node = findGroup(mstate,name)) return node;
    }

    return NULL;
}

/***********************************************************************/

static struct CMUser *
checkUser(struct CMGroup *group,struct CMUser *entry)
{
    register struct CMGroup *g, *gs;
    register struct CMUser  *mstate, *succ;
    REGARRAY UBYTE          this[256];

    strcpy(this,entry->Name);

    if (*entry->LastName)
    {
        strcat(this," ");
        strcat(this,entry->LastName);
    }

    if (!*this) return NULL;

    for (mstate = (struct CMUser *)group->Entries.mlh_Head; succ = mstate->Succ; mstate = succ)
    {
        REGARRAY UBYTE user[256];

        *user = '\0';

        if (*mstate->Name)
        {
            strcpy(user,mstate->Name);
            if (*mstate->LastName) strcat(user," ");
        }

        if (*mstate->LastName) strcat(user,mstate->LastName);

        if (*user && !stricmp(this,user)) return mstate;
    }

    for (g = CMGROUP(group->SubGroups.mlh_Head); gs = g->Succ; g = gs)
        if (mstate = checkUser(g,entry)) return mstate;

    return NULL;
}

/***********************************************************************/

static struct CMWWW *
checkWWW(struct CMGroup *group,struct CMWWW *entry)
{
    register struct CMGroup *g, *gs;
    register struct CMWWW   *mstate, *succ;

        for (mstate = (struct CMWWW *)group->Entries.mlh_Head; succ = mstate->Succ; mstate = succ)
        if (!stricmp(entry->WWW,mstate->WWW)) return mstate;

    for (g = CMGROUP(group->SubGroups.mlh_Head); gs = g->Succ; g = gs)
        if (mstate = checkWWW(g,entry)) return mstate;

    return NULL;
}

/***********************************************************************/

static struct CMFTP *
checkFTP(struct CMGroup *group,struct CMFTP *entry)
{
    register struct CMGroup *g, *gs;
    register struct CMFTP   *mstate, *succ;

    for (mstate = (struct CMFTP *)group->Entries.mlh_Head; succ = mstate->Succ; mstate = succ)
        if (!stricmp(entry->FTP,mstate->FTP)) return mstate;

    for (g = CMGROUP(group->SubGroups.mlh_Head); gs = g->Succ; g = gs)
        if (mstate = checkFTP(g,entry)) return mstate;

    return NULL;
}

/***********************************************************************/

static struct CMChat *
checkChat(struct CMGroup *group,struct CMChat *entry)
{
    register struct CMGroup *g, *gs;
    register struct CMChat  *mstate, *succ;

    for (mstate = (struct CMChat *)group->Entries.mlh_Head; succ = mstate->Succ; mstate = succ)
        if (!stricmp(entry->Server,mstate->Server) && !stricmp(entry->Channel,mstate->Channel))
            return mstate;

    for (g = CMGROUP(group->SubGroups.mlh_Head); gs = g->Succ; g = gs)
        if (mstate = checkChat(g,entry)) return mstate;

    return NULL;
}

/***********************************************************************/

void
importVoyager(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;
    register ULONG          uls = 0;

    while (FGets(file,buf,sizeof(buf)-1))
        if (!stricmp(buf,"<UL>\n"))
        {
            uls = 1;
            break;
        }

    cg = group;

    while (uls && fgets(file,buf,sizeof(buf)))
    {
        register STRPTR ptr;

        if (!stricmp(buf,"</UL>"))
        {
            cg = CM_GetParent(group,cg);
            uls--;

            continue;
        }

        if (strncmp(buf,"<LI>",4)) continue;

        if (ptr = strstr(buf,"<UL>"))
        {
            register struct CMGroup *ng;

            *ptr = '\0';

            if (ng = CM_AllocEntry(CMEntry_Group))
            {
                register struct CMGroup *g;

                io->added++;

                ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                if (!strncmp(buf+4,"<B>",3))
                {
                    register int len = strlen(buf+7)-3;

                    if (len>sizeof(ng->Name)-1) len = sizeof(ng->Name);
                    stccpy(ng->Name,buf+7,len);
                }
                else stccpy(ng->Name,buf+4,sizeof(ng->Name));

                if (!(io->flags && MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                {
                    ADDTAIL(&cg->SubGroups,ng);
                    cg = ng;
                }
                else
                {
                    cg = g;
                    CM_FreeEntry(ng);
                }

                uls++;
            }

            continue;
        }

        if (!strncmp(buf+4,"<A HREF=\"",9))
        {
            register struct CMWWW *www;

            if (www = CM_AllocEntry(CMEntry_WWW))
            {
                register UBYTE *ptr2;

                io->added++;

                ptr = strchr(buf+13,'>')+ 1;

                if (ptr2 = strstr(ptr,"</A>")) *ptr2 = '\0';

                stccpy(www->Name,ptr,sizeof(www->Name));

                ptr[-2] = '\0';

                stccpy(www->WWW,buf+13,sizeof(www->WWW ));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkWWW(group,www))
                    ADDTAIL((struct List *)&cg->Entries,(struct Node *)www);
                else CM_FreeEntry(www);
            }
        }
    }
}

/***********************************************************************/

void
importIB(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;
    register ULONG          uls = 0;

    while (FGets(file,buf,sizeof(buf)-1))
        if (!stricmp(buf,"<UL>\n"))
        {
            uls = 1;
            break;
        }

    cg = group;

    while (uls && fgets(file,buf,sizeof(buf)))
    {
        register UBYTE *ptr, *b;

        if (!stricmp(buf,"</UL>"))
        {
            cg = CM_GetParent(group,cg);
            uls--;

            continue;
        }

        b = buf;

        if (strncmp(b,"<LI",3)) continue;

        b = strchr(b,'>')+1;

        if (ptr = strstr(buf,"<UL>"))
        {
            register struct CMGroup *ng;

            *ptr = '\0';

            if (ng = CM_AllocEntry(CMEntry_Group))
            {
                register struct CMGroup *g;
                register UBYTE          *from;

                io->added++;

                ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                from = b;

                if (!strncmp(from,"<B>",3))
                {
                    from += 3;

                    if (ptr = strstr(from,"</B>")) *ptr = '\0';
                }

                stccpy(ng->Name,from,sizeof(ng->Name));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                {
                    ADDTAIL(&cg->SubGroups,ng);
                    cg = ng;
                }
                else
                {
                    cg = g;
                    CM_FreeEntry(ng);
                }

                uls++;
            }

            continue;
        }

        if (!strncmp(b,"<A HREF=\"",9))
        {
            register struct CMWWW *www;

            b += 9;

            if (www = CM_AllocEntry(CMEntry_WWW))
            {
                register UBYTE *ptr2;

                io->added++;

                ptr = strchr(b,'>')+1;
                if (ptr2 = strstr(ptr,"</A>")) *ptr2 = '\0';
                stccpy(www->Name,ptr,sizeof(www->Name));

                if (ptr = strstr(b,"IBSHORTCUT=\""))
                {
                    ptr += 12;
                    if (ptr2 = strchr(ptr,'"'))
                    {
                        *ptr2 = 0;
                        stccpy(www->Alias,ptr,sizeof(www->Alias));
                    }
                }

                if (ptr = strchr(b,'"')) *ptr = 0;
                stccpy(www->WWW,b,sizeof(www->WWW ));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkWWW(group,www))
                    ADDTAIL(&cg->Entries,www);
                else CM_FreeEntry(www);
            }
        }
    }
}

/***********************************************************************/

void
importAWeb(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;

    FGets(file,buf,sizeof(buf)-1);

    if (stricmp(buf,"@AWeb hotlist\n")!=0) return;

    cg = group;

    while (fgets(file,buf,sizeof(buf)))
    {
        register struct CMWWW *www;

        if (!stricmp(buf,"@ENDGROUP"))
        {
            cg = CM_GetParent(group,cg);
            continue;
        }

        if (!strncmp(buf,"@GROUP",6))
        {
            register struct CMGroup *ng;
            register UBYTE          *b;

            b = strchr(buf,' ')+1;

            if (ng = CM_AllocEntry(CMEntry_Group))
            {
                register struct CMGroup *g;

                io->added++;

                ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                stccpy(ng->Name,b,sizeof(ng->Name));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                {
                    ADDTAIL(&cg->SubGroups,ng);
                    cg = ng;

                }
                else
                {
                    cg = g;
                    CM_FreeEntry(ng);
                }
            }

            continue;
        }

        if (!strncmp(buf,"@DATE",5)) continue;

        if (www = CM_AllocEntry(CMEntry_WWW))
        {
            io->added++;

            stccpy(www->WWW,buf,sizeof(www->WWW));

            fgets(file,buf,sizeof(buf));

            stccpy(www->Name,buf,sizeof(www->Name));

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkWWW(group,www))
                ADDTAIL(&cg->Entries,www);
        else CM_FreeEntry(www);
        }
    }
}

/***********************************************************************/

void
importUserCSV(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR  file = io->file;
    REGARRAY UBYTE buf[1024];

     // skip headers
    FGets(file,buf,sizeof(buf)-1);

    while (fgets(file,buf,sizeof(buf)))
    {
        register struct CMUser *entry;
        register STRPTR        ptr;

        if (entry = CM_AllocEntry(CMEntry_User))
        {
            io->added++;

            ptr = buf;

            ptr = getCSVToken(ptr,',',entry->LastName,    sizeof(entry->LastName    ));
            ptr = getCSVToken(ptr,',',entry->Name,        sizeof(entry->Name        ));
            ptr = getCSVToken(ptr,',',entry->Address,     sizeof(entry->Address     ));
            ptr = getCSVToken(ptr,',',entry->City,        sizeof(entry->City        ));
            ptr = getCSVToken(ptr,',',entry->Country,     sizeof(entry->Country     ));
            ptr = getCSVToken(ptr,',',entry->ZIP,         sizeof(entry->ZIP         ));
            ptr = getCSVToken(ptr,',',entry->Comment,     sizeof(entry->Comment     ));
            ptr = getCSVToken(ptr,',',entry->Alias,       sizeof(entry->Alias       ));
            ptr = getCSVToken(ptr,',',entry->Phone,       sizeof(entry->Phone       ));
            ptr = getCSVToken(ptr,',',entry->Fax,         sizeof(entry->Fax         ));
            ptr = getCSVToken(ptr,',',entry->Mobile,      sizeof(entry->Mobile      ));
            ptr = getCSVToken(ptr,',',entry->EMail,       sizeof(entry->EMail       ));
            ptr = getCSVToken(ptr,',',entry->WWW,         sizeof(entry->WWW         ));
            ptr = getCSVToken(ptr,',',entry->FTP,         sizeof(entry->FTP         ));
            ptr = getCSVToken(ptr,',',entry->ICQ,         sizeof(entry->ICQ         ));
            ptr = getCSVToken(ptr,',',entry->Company,     sizeof(entry->Company     ));
            ptr = getCSVToken(ptr,',',entry->BJobTitle,   sizeof(entry->BJobTitle   ));
            ptr = getCSVToken(ptr,',',entry->BDepartment, sizeof(entry->BDepartment ));
            ptr = getCSVToken(ptr,',',entry->BOffice,     sizeof(entry->BOffice     ));
            ptr = getCSVToken(ptr,',',entry->BAddress,    sizeof(entry->BAddress    ));
            ptr = getCSVToken(ptr,',',entry->BCity,       sizeof(entry->BCity       ));
            ptr = getCSVToken(ptr,',',entry->BCountry,    sizeof(entry->BCountry    ));
            ptr = getCSVToken(ptr,',',entry->BZIP,        sizeof(entry->BZIP        ));
            ptr = getCSVToken(ptr,',',entry->BPhone,      sizeof(entry->BPhone      ));
            ptr = getCSVToken(ptr,',',entry->BFax,        sizeof(entry->BFax        ));
            ptr = getCSVToken(ptr,',',entry->BMobile,     sizeof(entry->BMobile     ));
           /*ptr = */getCSVToken( ptr, ',', entry->BEMail,      sizeof( entry->BEMail      ));

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,entry))
                ADDTAIL(&group->Entries,entry);
        else CM_FreeEntry(entry);
        }
    }
}

/***********************************************************************/

void
importUserCSVFullOE(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR  file = io->file;
    REGARRAY UBYTE buf[1024], dummy[64];

     // skip headers
    FGets(file,buf,sizeof(buf)-1);

    while (fgets(file,buf,sizeof(buf)))
    {
        register struct CMUser *entry;
        register STRPTR        ptr;

        if (entry = CM_AllocEntry(CMEntry_User))
        {
            io->added++;

            ptr = buf;

            ptr = getCSVToken(ptr,';',entry->Name,sizeof(entry->Name));
            ptr = getCSVToken(ptr,';',entry->LastName,sizeof(entry->LastName));
            ptr = getCSVToken(ptr,';',dummy,sizeof(dummy));
            ptr = getCSVToken(ptr,';',dummy,sizeof(dummy));
            ptr = getCSVToken(ptr,';',entry->Alias,sizeof(entry->Alias));
            ptr = getCSVToken(ptr,';',entry->EMail,sizeof(entry->EMail));
            ptr = getCSVToken(ptr,';',entry->Address,sizeof(entry->Address));
            ptr = getCSVToken(ptr,';',entry->City,sizeof(entry->City));
            ptr = getCSVToken(ptr,';',entry->ZIP,sizeof(entry->ZIP));
            ptr = getCSVToken(ptr,';',dummy,sizeof(dummy));
            ptr = getCSVToken(ptr,';',entry->Country,sizeof(entry->Country));
            ptr = getCSVToken(ptr,';',entry->Phone,sizeof(entry->Phone));
            ptr = getCSVToken(ptr,';',entry->Fax,sizeof(entry->Fax));
            ptr = getCSVToken(ptr,';',entry->Mobile,sizeof(entry->Mobile));
            ptr = getCSVToken(ptr,';',entry->WWW,sizeof(entry->WWW));
            ptr = getCSVToken(ptr,';',entry->BAddress,sizeof(entry->BAddress));
            ptr = getCSVToken(ptr,';',entry->BCity,sizeof(entry->BCity));
            ptr = getCSVToken(ptr,';',entry->BZIP,sizeof(entry->BZIP));
            ptr = getCSVToken(ptr,';',dummy,sizeof(dummy));
            ptr = getCSVToken(ptr,';',entry->BCountry,sizeof(entry->BCountry));
            ptr = getCSVToken(ptr,';',entry->BPhone,sizeof(entry->BPhone));
            ptr = getCSVToken(ptr,';',entry->BFax,sizeof(entry->BFax));
            ptr = getCSVToken(ptr,';',entry->BMobile,sizeof(entry->BMobile));
            ptr = getCSVToken(ptr,';',entry->Company,sizeof(entry->Company));
            ptr = getCSVToken(ptr,';',entry->BJobTitle,sizeof(entry->BJobTitle));
            ptr = getCSVToken(ptr,';',entry->BDepartment,sizeof(entry->BDepartment));
            ptr = getCSVToken(ptr,';',entry->BOffice,sizeof(entry->BOffice));
            /*ptr = */getCSVToken(ptr,';',entry->Comment,sizeof(entry->Comment));

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,entry))
                ADDTAIL(&group->Entries,entry);
        else CM_FreeEntry(entry);
        }
    }
}

/***********************************************************************/

struct profile
{
    UBYTE label[40];
    UBYTE host[128];
    UBYTE dummy4[32];
    UBYTE password[64];
    UBYTE defdir[120];
    UBYTE dummy[8];
    UBYTE localdir[120];
    UBYTE port[6];
    UBYTE compress;
    UBYTE saveldir;
    UBYTE is_anon;
    UBYTE retries;
    UBYTE savedir;
    UBYTE quiet_login;
    UBYTE is_adt;
    UBYTE username[64];
    UBYTE dummy3;
};

static LONG
myrand(ULONG *seed)
{
    register LONG ret, a;

    a     = (LONG)*seed;    // MOVE.L  0040(A4),D0
    ret   = a;              // MOVE.L  D0,D1
    ret <<= 3;              // ASL.L   #3,D1
    ret  -= a;              // SUB.L   D0,D1
    ret <<= 3;              // ASL.L   #3,D1
    ret  += a;              // ADD.L   D0,D1
    ret  += ret;            // ADD.L   D1,D1
    ret  += a;              // ADD.L   D0,D1
    ret <<= 4;              // ASL.L   #4,D1
    ret  -= a;              // SUB.L   D0,D1
    ret  += ret;            // ADD.L   D1,D1
    ret  -= a;              // SUB.L   D0,D1
    ret  += 0x00000E60;     // ADDI.L  #00000e60,D1
    ret  &= 0x7FFFFFFF;     // ANDI.L  #7fffffff,D1
    a     = ret;            // MOVE.L  D1,D0
    a    -= 1;              // SUBQ.L  #1,D0
    *seed = (ULONG)a;       // MOVE.L  D0,0040(A4)

    return ret;             // MOVE.L  D1,D0
}

void
importAmFTP(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR   file = io->file;
    struct profile  pf;
    UWORD           version;
    ULONG           install;

    Read(file,&version,2);
    Read(file,&install,4);

    if (version>2) return;

    while (Read(file,&pf,sizeof(pf)-1)==sizeof(pf)-1)
    {
        register struct CMFTP *ftp;
        register BYTE         *p;
        register ULONG        j;
        ULONG                 seed;

        seed = (ULONG)-1;

        p = (BYTE *)&pf;

        for (j = 0; j<sizeof(pf); j++) *p++ ^= myrand(&seed);

        if (!*(pf.port)) strcpy(pf.port,"21");

        if (ftp = CM_AllocEntry(CMEntry_FTP))
        {
            register ULONG len;
            register UBYTE *ptr;

            io->added++;

            strcpy(ftp->Name,pf.label);
            strcpy(ftp->FTP,pf.host);
            strcpy(ftp->Username,pf.username);
            strcpy(ftp->Password,pf.password);

            len = strlen(ftp->FTP)-1;

            if (ftp->FTP[len]=='/') ftp->FTP[len] = '\0';

            ftp->Port    = atoi(pf.port);
            ftp->Retries = pf.retries;

            ftp->Flags = CMFTPFLG_Advanced;

            if (pf.compress) ftp->Flags |= CMFTPFLG_Compress;
            if (pf.quiet_login) ftp->Flags |= CMFTPFLG_Quiet;
            if (pf.is_adt) ftp->Flags |= CMFTPFLG_ADT;
            if (pf.is_anon) ftp->Flags |= CMFTPFLG_Anonymous;

            ptr = pf.defdir;

            if (*ptr=='/') ptr++;

            AddPart(ftp->FTP,ptr,sizeof(ftp->FTP));

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkFTP(group,ftp))
                ADDTAIL(&group->Entries,ftp);
            else CM_FreeEntry(ftp);
        }
    }
}

/***********************************************************************/

void
importMD(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;

    FGets(file,buf,sizeof(buf)-1);

    if (stricmp(buf,"MicroDot - Bookmarks.mcc\n")) return;

    cg = group;

    while (fgets(file,buf,sizeof(buf)))
    {
        register struct CMUser *user;

        if (*buf==(UBYTE)'«')
        {
            cg = CM_GetParent(group,cg);
            continue;
        }

        if (*buf==(UBYTE)'»')
        {
            register struct CMGroup *ng;

            if (ng = CM_AllocEntry(CMEntry_Group))
            {
                register struct CMGroup *g;

                io->added++;

                ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                stccpy(ng->Name,buf+1,sizeof(ng->Name));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                {
                    ADDTAIL(&cg->SubGroups,ng);
                    cg = ng;
                }
                else
                {
                    cg = g;
                    CM_FreeEntry(ng);
                }
            }

            continue;
        }

        if (user =CM_AllocEntry(CMEntry_User))
        {
            register UBYTE *ptr;

            io->added++;

            ptr = strchr(buf,'|');

            stccpy(user->EMail,ptr+1,sizeof(user->EMail));

            *ptr = '\0';

            stccpy(user->Name,buf,sizeof(user->Name));

            if (ptr = strrchr(user->Name,' '))
            {
                stccpy(user->LastName,ptr+1,sizeof(user->LastName));

                *ptr = '\0';
            }

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,user))
                ADDTAIL(&cg->Entries,user);
        else CM_FreeEntry(user);
        }
    }
}

/***********************************************************************/

void
importYAM(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;

    FGets(file,buf,sizeof(buf)-1);
    if (stricmp(buf,"YAB4 - YAM Addressbook\n")) return;

    cg = group;

    while (fgets(file,buf,sizeof(buf)))
    {
        register struct CMUser *user;
        register UBYTE         *ptr;

        if (!stricmp(buf,"@ENDGROUP"))
        {
            cg = CM_GetParent(group,cg);
            continue;
        }

        if (!strncmp(buf,"@GROUP",6))
        {
            register struct CMGroup *ng;

            if (ng = CM_AllocEntry(CMEntry_Group))
            {
                register struct CMGroup *g;

                io->added++;

                ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                stccpy(ng->Name,buf+7,sizeof(ng->Name));
                fgets(file,buf,sizeof(buf));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                {
                    ADDTAIL(&cg->SubGroups,ng);
                    cg = ng;
                }
                else
                {
                    cg = g;
                    CM_FreeEntry(ng);
                }
            }

            continue;
        }

        if (!strncmp(buf,"@USER ",6))
        {
            if (user = CM_AllocEntry(CMEntry_User))
            {
                io->added++;

                /* Alias */
                stccpy(user->Alias,buf+6,sizeof(user->Alias));

                /* EMail */
                fgets(file,buf,sizeof(buf));
                stccpy(user->EMail,buf,sizeof(user->EMail));
                if (ptr = strchr(user->EMail,' ')) *ptr = '\0';

                /* Name, LasName */
                fgets(file,buf,sizeof(buf));
                if (ptr = strrchr(buf,' '))
                {
                    stccpy(user->LastName,ptr+1,sizeof(user->LastName));
                    *ptr = '\0';
                }
                stccpy(user->Name,buf,sizeof(user->Name));

                if (*user->Alias) stccpy(user->VisualName,user->Alias,sizeof(user->VisualName));
                //else stccpy(user->VisualName,user->Alias,sizeof(user->VisualName));

                /* Comment */
                fgets(file,buf,sizeof(buf));
                stccpy(user->Comment,buf,sizeof(user->Comment));

                /* Phone */
                fgets(file,buf,sizeof(buf));
                stccpy(user->Phone,buf,sizeof(user->Phone));
                if (ptr = strchr(user->Phone,' ')) *ptr = '\0';

                /* Address */
                fgets(file,buf,sizeof(buf));
                stccpy(user->Address,buf,sizeof(user->Address));

                /* City */
                fgets(file,buf,sizeof(buf));
                if (ptr = strrchr(buf,'/'))
                {
                    stccpy(user->ZIP,ptr+1,sizeof(user->ZIP));
                    *ptr = '\0';
                }
                stccpy(user->City,buf,sizeof(user->City));

                /* Country */
                fgets(file,buf,sizeof(buf));
                stccpy(user->Country,buf,sizeof(user->Country));

                /* PGP */
                fgets(file,buf,sizeof(buf));
                stccpy(user->PGPUserID,buf,sizeof(user->PGPUserID));

                /* Birthday */
                FGets(file,buf,sizeof(buf)-1);

                /* Image */
                fgets(file,buf,sizeof(buf));
                stccpy(user->ImageFile,buf,sizeof(user->ImageFile));

                /* Homepage */
                fgets(file,buf,sizeof(buf));
                stccpy(user->WWW,buf,sizeof(user->WWW));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,user))
                    ADDTAIL(&cg->Entries,user);
        else CM_FreeEntry(user);
            }

            /* Reach @ENDUSER */
            while (FGets(file,buf,sizeof(buf)-1))
                if (!stricmp(buf,"@ENDUSER\n")) break;
        }
    }
}

/***********************************************************************/

void
importDFA(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR  file = io->file;
    REGARRAY UBYTE buf[1024];

    FGets(file,buf,sizeof(buf)-1);
    if (strncmp(buf,"DFAddress",9)) return;

    while (fgets(file,buf,sizeof(buf)))
    {
        register struct CMUser *user;

        if (user = CM_AllocEntry(CMEntry_User))
        {
            io->added++;

            /*
                Last
                First
                Street
                ZIP
                City
                Country
                Birthday
                Phone
                Email1
                Email2
                Email3
                Comment
                ?
                Salutation
                Fax
                ?
                c/o
                State
                ?
                ?
            */
            stccpy(user->LastName,buf,sizeof(user->LastName));
            fgets(file,user->Name,sizeof(user->Name));
            fgets(file,user->Address,sizeof(user->Address));
            fgets(file,user->ZIP,sizeof(user->ZIP));
            fgets(file,user->City,sizeof(user->City));
            FGets(file,buf,sizeof(buf)-1);

            fgets(file,buf,sizeof(buf));
            snprintf(user->Comment,sizeof(user->Comment),"Birthday: %s\n",buf);
            fgets(file,user->Phone,sizeof(user->Phone));

            fgets(file,user->EMail,sizeof(user->EMail));
            fgets(file,buf,sizeof(buf));
            if (*buf)
            {
                register struct CMEMail *email;

                if (email = CM_AllocObject(CMALLOCOBJECT_EMail))
                {
                    stccpy(email->EMail,buf,sizeof(email->EMail));
                    ADDTAIL(&user->EMails,email);
                }
            }
            fgets(file,buf,sizeof(buf));
            if (*buf)
            {
                register struct CMEMail *email;

                if (email = CM_AllocObject(CMALLOCOBJECT_EMail))
                {
                    stccpy(email->EMail,buf,sizeof(email->EMail));
                    ADDTAIL(&user->EMails,email);
                }
            }

            FGets(file,buf,sizeof(buf)-1);
            strcat(user->Comment,buf);
            FGets(file,buf,sizeof(buf)-1);
            FGets(file,buf,sizeof(buf)-1);
            fgets(file,user->Fax,sizeof(user->Fax));
            FGets(file,buf,sizeof(buf)-1);
            FGets(file,buf,sizeof(buf)-1);
            fgets(file,user->Country,sizeof(user->Country));
            FGets(file,buf,sizeof(buf)-1);

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,user))
                ADDTAIL(&group->Entries,user);
        else CM_FreeEntry(user);
        }
    }
}

/***********************************************************************/

void
importPINT(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;
    register ULONG          inlist = FALSE;

    FGets(file,buf,sizeof(buf)-1);    // $VER:
    FGets(file,buf,sizeof(buf)-1);    // empty line

    cg = group;

    while (fgets(file,buf,sizeof(buf)))
    {
        register struct CMUser *user;
        register UBYTE         *ptr;

        if (inlist && !*buf)
        {
            cg = CM_GetParent(group,cg);
            inlist = FALSE;

            continue;
        }

        if (!stricmp(buf,"ENDGROUP"))
        {
            cg = CM_GetParent(group,cg);
            FGets(file,buf,sizeof(buf)-1);    // empty line

            continue;
        }

        if (!stricmp(buf,"LIST")) inlist = TRUE;

        if (!stricmp(buf,"LIST") || !stricmp(buf,"GROUP"))
        {
            register struct CMGroup *ng;

            if (ng = CM_AllocEntry(CMEntry_Group))
            {
                register struct CMGroup *g;

                io->added++;

                ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                fgets(file,ng->Name,sizeof(ng->Name));

                FGets(file,buf,sizeof(buf)-1);    // comment

                if (!inlist) FGets(file,buf,sizeof(buf)-1);    // empty line

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                {
                    ADDTAIL(&cg->SubGroups,ng);
                    cg = ng;
                }
                else
                {
                    cg = g;
                    CM_FreeEntry(ng);
                }
            }

            continue;
        }

        if (user = CM_AllocEntry(CMEntry_User))
        {
            io->added++;

            if (ptr = strrchr(user->Name,' '))
            {
                stccpy(user->LastName,ptr+1,sizeof(user->LastName));
                *ptr = '\0';
            }
            stccpy(user->Name,buf,sizeof(user->Name));

            if (!inlist) fgets(file,user->Comment,sizeof(user->Comment));

            fgets(file,user->EMail,sizeof(user->EMail));

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,user))
                ADDTAIL(&cg->Entries,user);
        else CM_FreeEntry(user);
        }

        if (!inlist)
            while (FGets(file,buf,sizeof(buf)-1))
                if (*buf=='\n') break;
    }
}

/***********************************************************************/

void
importGoodNews(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    register struct CMGroup *cg;
    REGARRAY UBYTE          buf[1024];
    register ULONG          inlist = FALSE;

    cg = group;

    while (fgets(file,buf,sizeof(buf)))
    {
        register struct CMUser *user;
        register UBYTE         *ptr;

        if (!*buf)
        {
            if (inlist)
            {
                cg = CM_GetParent(group,cg);
                inlist = FALSE;
            }

            continue;
        }

        if (!stricmp(buf,"GROUP"))
        {
            register struct CMGroup *ng;

            inlist = TRUE;

            if (ng = CM_AllocEntry(CMEntry_Group))
            {
                register struct CMGroup *g;

                io->added++;

                ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                fgets(file,ng->Name,sizeof(ng->Name));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                {
                    ADDTAIL(&cg->SubGroups,ng);
                    cg = ng;
                }
                else
                {
                    cg = g;
                    CM_FreeEntry(ng);
                }
            }

            continue;
        }

        if (user = CM_AllocEntry(CMEntry_User))
        {
            io->added++;

            if (ptr = strrchr(user->Name,' '))
            {
                stccpy(user->LastName,ptr+1,sizeof(user->LastName));
                *ptr = '\0';
            }
            stccpy(user->Name,buf,sizeof(user->Name));

            if (!inlist) fgets(file,user->Comment,sizeof(user->Comment));

            fgets(file,user->EMail,sizeof(user->EMail));

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser( group, user ))
                ADDTAIL(&cg->Entries,user);
        else CM_FreeEntry(user);
        }

        if (!inlist)
            while (FGets(file,buf,sizeof(buf)-1))
                if (*buf=='\n') break;
    }
}

/***********************************************************************/

void
importSTRICQ(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR   file = io->file;
    register struct Process *this;
    register APTR           win;
    register BPTR           dir;

    /* If the user supply a wrong file, not in STRICQ format, strange things may happen */
    this = (struct Process *)FindTask(NULL);
    win = this->pr_WindowPtr;
    this->pr_WindowPtr = (APTR)-1;

    if (dir = ParentOfFH(file))
    {
        register BPTR  olddir;
        REGARRAY UBYTE uin[40];

        olddir = CurrentDir(dir);

        while (fgets(file,uin,sizeof(uin)))
        {
            register BPTR  userfile;
            REGARRAY UBYTE name[256];

            sprintf(name,"Users/%s",uin);

            if (userfile = Open(name,MODE_OLDFILE))
            {
                register struct CMUser *user;

                if (user = CM_AllocEntry(CMEntry_User))
                {
                    REGARRAY UBYTE buf[256];

                    io->added++;

                    stccpy(user->ICQ,uin,sizeof(user->ICQ));

                    while (fgets(userfile,buf,sizeof(buf)))
                    {
                        if (!strncmp(buf,"NICK",4)) stccpy(user->Alias,buf+5,sizeof(user->Alias));
                        else if (!strncmp(buf,"FIRST",5))stccpy(user->Name,buf+6,sizeof(user->Name));
                             else if (!strncmp(buf,"LAST",4)) stccpy(user->LastName,buf+5,sizeof(user->LastName));
                                  else if (!strncmp(buf,"EMAIL",5)) stccpy(user->EMail,buf+6,sizeof(user->EMail));
                                       else if (!strncmp(buf,"CITY",4)) stccpy(user->City,buf+5,sizeof(user->City));
                                            else if (!strncmp(buf,"STATE",5)) stccpy(user->Country,buf+6,sizeof(user->Country));
                                                 else if (!strncmp(buf,"PHONE",5)) stccpy(user->Phone,buf+6,sizeof(user->Phone));
                                                      else if (!strncmp(buf,"HOMEPAGE",8)) stccpy(user->WWW,buf+9,sizeof(user->WWW));
                                                           else if (!strncmp(buf,"COMMENT",7))
                                                                {
                                                                    Flush(userfile);
                                                                    Read(userfile,user->Comment,sizeof(user->Comment) - 1);
                                                                    user->Comment[strlen(user->Comment)] = '\0';
                                                                }
                    }

                    if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,user))
                        ADDTAIL(&group->Entries,user);
            else CM_FreeEntry(user);
                }

                Close(userfile);
            }
        }

        CurrentDir(olddir);
        UnLock(dir);
    }

    this->pr_WindowPtr = win;
}

/***********************************************************************/

static void
importOldSTFax(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR   file = io->file;
    register UBYTE  f;

    while (!(f = FGetC(file)))
    {
        register struct CMUser *user;

        if (user = CM_AllocEntry(CMEntry_User))
        {
            REGARRAY UBYTE  buf[256];
            register STRPTR ptr;
            register UBYTE  flags;

            io->added++;

            flags = FGetC(file);

            getString(file,user->Fax);

            if (ptr = strrchr(user->Name,' '))
            {
                stccpy(user->LastName,ptr+1,sizeof(user->LastName));
                *ptr = '\0';
            }
            getString(file,user->Name);

            getString(file,user->Comment);

            getString(file,buf);
            getString(file,buf);
            getString(file,buf);

            if (flags & PB_EXTENDED)
            {
                getString(file,user->Phone);

                if (FGetC(file))
                {
                    getString(file,buf);
                    FGetC(file);  // more data?
                }
            }

            if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,user))
                ADDTAIL(&group->Entries,user);
            else CM_FreeEntry(user);
        }
    }
}

void
importSTFax(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR   file = io->file;
    register struct Library *STFaxBase;

    if (STFaxBase = OpenLibrary("STFax.library",1))
    {
        REGARRAY UBYTE name[256];
        struct MinList numbers;

        NameFromFH(file,name,sizeof(name));

        NEWLIST(&numbers);

        if (STF_LoadPhonebook(&numbers,name))
        {
            register struct PBEntry *e, *es;

            for (e = (struct PBEntry *)numbers.mlh_Head; es = (struct PBEntry *)e->Node.ln_Succ; e = es)
            {
                register struct CMUser *user;

                if (e->Node.ln_Type!=PBT_ENTRY) continue;

                if (user = CM_AllocEntry(CMEntry_User))
                {
                    register STRPTR ptr;

                    io->added++;

                    if (ptr = strrchr(user->Name,' '))
                    {
                        stccpy(user->LastName,ptr+1,sizeof(user->LastName));
                        *ptr = '\0';
                    }
                    stccpy(user->Name,e->Name,sizeof(user->Name));

                    stccpy(user->Fax,e->Number,sizeof(user->Fax));
                    stccpy(user->Phone,e->Phone,sizeof(user->Phone));
                    stccpy(user->Comment,e->Note,sizeof(user->Comment));
                    stccpy(user->Address,e->Address,sizeof(user->Address));
                    stccpy(user->City,e->City,sizeof(user->City));
                    stccpy(user->Country,e->Country,sizeof(user->Country));
                    stccpy(user->ZIP,e->ZIP,sizeof(user->ZIP));
                    stccpy(user->Mobile,e->Mobile,sizeof(user->Mobile ));
                    stccpy(user->Company,e->Company,sizeof(user->Company));
                    stccpy(user->EMail,e->EMail,sizeof(user->EMail));

                    if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkUser(group,user))
                        ADDTAIL(&group->Entries,user);
                    else CM_FreeEntry(user);
                }
            }

            STF_FreePhonebook(&numbers);
        }

        CloseLibrary(STFaxBase);
    }
    else importOldSTFax(group,io);
}

/***********************************************************************/

void
importAmIRC(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;

    cg = group;

    while (fgets(file,buf,sizeof(buf)))
    {
        register UBYTE *ptr;

        if (!strncmp(buf,"SERVER",6))
        {
            register struct CMChat *entry;

            if (!strncmp(buf+7,"<<",2))
            {
                cg = CM_GetParent(group,cg);

                continue;
            }

            if (!strncmp(buf+7,">>",2))
            {
                register struct CMGroup *ng;

                if (ng = CM_AllocEntry(CMEntry_Group))
                {
                    register struct CMGroup *g;

                    io->added++;

                    ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                    stccpy(ng->Name,buf+9,sizeof(ng->Name));

                    if (ptr = strrchr(ng->Name,',')) *ptr = '\0';

                    if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                    {
                        ADDTAIL(&cg->SubGroups,ng);
                        cg = ng;
                    }
                    else
                    {
                        cg = g;
                        CM_FreeEntry(ng);
                    }
                }

                continue;
            }

            if (entry = CM_AllocEntry(CMEntry_Chat))
            {
                register STRPTR ptr, ptr2;

                io->added++;

                ptr  = buf+7;
                ptr2 = strchr(ptr,',');

                *ptr2 = '\0';

                stccpy(entry->Server,ptr,sizeof(entry->Server));

                ptr  = strchr(ptr2+1,',')+1;
                ptr2 = strchr(ptr,',');

                *ptr2 = '\0';

                stccpy(entry->Channel,ptr,sizeof(entry->Channel));

                ptr = strchr(ptr2+1,',')+1;

                stccpy(entry->Comment,ptr,sizeof(entry->Comment));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkChat(group,entry))
                    ADDTAIL(&cg->Entries,entry);
        else CM_FreeEntry(entry);
            }
        }
    }
}

/***********************************************************************/

#define DOPUSTEMPLATE "ANONYMOUS=ANON/S,USERACCOUNT=ACCT/S,ALIAS=NAME/K,HOST,ADDRESS=ADDR/K,PORT/N,PATH=DIR,USER/K,PASSWORD=PASS/K"

struct site
{
    ULONG  anon;
    ULONG  acct;
    STRPTR name;
    STRPTR host;
    STRPTR addr;
    ULONG  *port;
    STRPTR dir;
    STRPTR user;
    STRPTR pass;
};

void
importDOpusFTP(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR   file = io->file;
    register struct RDArgs *rda;

    if (rda = AllocDosObject(DOS_RDARGS,NULL))
    {
        REGARRAY UBYTE buf[1024];

        while (fgets(file,buf,sizeof(buf)-1))
        {
            register struct RDArgs *ra;
            struct site       site = {0};

            if (!*buf || (*buf=='#')) continue;

            strcat(buf,"\12");

            rda->RDA_Source.CS_Buffer = buf;
            rda->RDA_Source.CS_Length = strlen(buf);
            rda->RDA_Source.CS_CurChr = 0;
            rda->RDA_DAList           = NULL;
            rda->RDA_Buffer           = NULL;
            rda->RDA_BufSiz           = 0L;
            rda->RDA_Flags           |= RDAF_NOPROMPT;

            if (ra = ReadArgs(DOPUSTEMPLATE,(LONG *)&site,rda))
            {
                register struct CMFTP *ftp;
                register ULONG        len;

                if (ftp = CM_AllocEntry(CMEntry_FTP))
                {
                    io->added++;

                    if (site.name) stccpy(ftp->Name,site.name,sizeof(ftp->Name));
                    if (site.host) stccpy(ftp->FTP,site.host,sizeof(ftp->FTP)-1);
                    if (site.user) stccpy(ftp->Username,site.user,sizeof(ftp->Username));
                    if (site.pass) stccpy(ftp->Password,site.pass,sizeof(ftp->Password));

                    len = strlen(ftp->FTP)-1;
                    if (ftp->FTP[len]=='/') ftp->FTP[len] = '\0';

                    ftp->Port = site.port ? *site.port : 21;

                    if (site.dir)
                    {
                        register STRPTR dir = site.dir;

                        if (*dir=='/') dir++;

                        AddPart(ftp->FTP,dir,sizeof(ftp->FTP));
                    }

                    ftp->Flags = CMFTPFLG_Advanced;

                    if (site.anon) ftp->Flags |= CMFTPFLG_Anonymous;

                    if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkFTP(group,ftp))
                        ADDTAIL(&group->Entries,ftp);
            else CM_FreeEntry(ftp);
                }

                FreeArgs(ra);
            }
        }

            FreeDosObject(DOS_RDARGS,rda);
    }
}

/***********************************************************************/

void
importNetscape(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;
    register ULONG          uls = 0;

    while (FGets(file,buf,sizeof(buf)-1))
    {
        if (!strncmp(buf,"<DL><p>",7))
        {
            uls = 1;
            break;
        }

        if (!(io->flags & MUIV_CManager_Import_Skip))
        {
            if (!strncmp(buf,"<META HTTP-EQUIV=",17))
            {
                register UBYTE *ptr;

                if (ptr = strstr(buf,"charset=UTF-8"))
                    io->flags |= MUIV_CManager_Import_UTF8;
            }
        }
    }

    io->flags |= MUIV_CManager_Import_Skip;

    cg = group;

    while (uls && fgets(file,buf,sizeof(buf)))
    {
        register UBYTE *ptr, *b;

        b = buf;

        for (; isspace(*b); b++);

        if (!strncmp(b,"</DL><p>",8))
        {
            cg  = CM_GetParent(group,cg);
            uls--;

            continue;
        }

        if (ptr = strstr(b,"<DT><H3"))
        {
            register struct CMGroup *ng;

            if (ng = CM_AllocEntry(CMEntry_Group))
            {
                register struct CMGroup *g;

                io->added++;

                ng->Flags = (io->flags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;

                if (ptr = strstr(ptr+7,">"))
                {
                    *ptr = '\0';
                    b = ptr+1;
                }

                if (ptr = strstr(b,"</H3>")) *ptr = '\0';

                if (io->flags & MUIV_CManager_Import_UTF8)
                {
                    etranslate(b,io->buf,sizeof(io->buf));

                    CodesetsUTF8ToStr(CODESETSA_Source,  (ULONG)io->buf,
                                      CODESETSA_Dest,    (ULONG)ng->Name,
                                      CODESETSA_DestLen, sizeof(ng->Name),
                                      CODESETSA_Codeset, (ULONG)io->codeset,
                                      CODESETSA_AllocIfNeeded, FALSE,
                                      TAG_DONE);
                }
                else stccpy(ng->Name,b,sizeof(ng->Name));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                {
                    ADDTAIL(&cg->SubGroups,ng);
                    cg = ng;
                }
                else
                {
                    cg = g;
                    CM_FreeEntry(ng);
                }

                uls++;
            }

            continue;
        }

        if (!strncmp(b,"<DT><A HREF=\"",13))
        {
            register struct CMWWW *www;

            b += 13;

            if (www = CM_AllocEntry(CMEntry_WWW))
            {
                register UBYTE *ptr2;

                io->added++;

                ptr = strchr(b,'>');

                if (!ptr)
                {
                    ptr = strstr(b,"ICON=");

                    if (!ptr)
                    {
                        CM_FreeEntry(www);
                        continue;
                    }

                    ptr = b;
                    if (ptr2 = strchr(b,'"')) *ptr2 = '\0';
                }
                else
                {
                    ptr++;
                    if (ptr2 = strstr(ptr,"</A>")) *ptr2 = '\0';
                }

                if (io->flags & MUIV_CManager_Import_UTF8)
                {
                    etranslate(ptr,io->buf,sizeof(io->buf));
                    CodesetsUTF8ToStr(CODESETSA_Source,  (ULONG)io->buf,
                                      CODESETSA_Dest,    (ULONG)www->Name,
                                      CODESETSA_DestLen, sizeof(www->Name),
                                      CODESETSA_Codeset, (ULONG)io->codeset,
                                      CODESETSA_AllocIfNeeded, FALSE,
                                      TAG_DONE);
                }
                else stccpy(www->Name,ptr,sizeof(www->Name));

                if (ptr2 = strchr(b,'"')) *ptr2 = '\0';

                if (io->flags & MUIV_CManager_Import_UTF8)
                    CodesetsUTF8ToStr(CODESETSA_Source,  (ULONG)b,
                                      CODESETSA_Dest,    (ULONG)www->WWW,
                                      CODESETSA_DestLen, sizeof(www->WWW),
                                      CODESETSA_Codeset, (ULONG)io->codeset,
                                      CODESETSA_AllocIfNeeded, FALSE,
                                      TAG_DONE);
                else stccpy(www->WWW,b,sizeof(www->WWW ));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkWWW(group,www))
                    ADDTAIL(&cg->Entries,www);
        else CM_FreeEntry(www);
            }
        }
    }
}

/***********************************************************************/

struct smData
{
    struct Library 	*ExpatBase;
    struct codeset	*codeset;
    XML_Parser		parser;
    ULONG		ioflags;

    int 		newaddressbook_tag;
    int 		newcontact_tag;
    int 		newgroup_tag;
    int 		private_tag;
    int 		work_tag;
    STRPTR 		data_buf;

    int 		is_classic_addressbook;
    int 		contact_tag;
    int 		group_tag;
    STRPTR		classic_group_name;

    struct CMGroup 	*rootGroup;
    struct CMGroup 	*group;
    struct CMGroup 	*cgroup;
    struct CMUser	*user;

    UBYTE		description[512];
    UBYTE		comment[512];
};

struct tag
{
    STRPTR name;
    ULONG  id;
    ULONG  flags;
};

enum
{
    TFLG_Start = 1<<0,
    TFLG_End   = 1<<1,
};

enum endTags
{
    newaddressbook_tag,
    addressbook_tag,
    newcontact_tag,
    contact_tag,
    newgroup_tag,
    group_tag,
    private_tag,
    work_tag,
    alias_tag,
    name_tag,
    description_tag,
    email_tag,
    pgpid_tag,
    homepage_tag,
    portrait_tag,
    note_tag,
    sex_tag,
    birthday_tag,
    title_tag,
    organization_tag,
    street_tag,
    city_tag,
    zip_tag,
    state_tag,
    country_tag,
    mobil_tag,
    fax_tag,
    phone_tag,
};

struct tag xmlTags[] =
{
    "newaddressbook",	newaddressbook_tag,	TFLG_Start|TFLG_End,
    "newcontact", 	newcontact_tag,		TFLG_Start|TFLG_End,
    "newgroup", 	newgroup_tag,		TFLG_Start|TFLG_End,
    "private", 		private_tag,		TFLG_Start|TFLG_End,
    "work", 		work_tag,		TFLG_Start|TFLG_End,
    "addressbook",	addressbook_tag,	TFLG_Start|TFLG_End,
    //"contact",	  contact_tag,		  TFLG_Start|TFLG_End,
    "group",		group_tag,		TFLG_Start|TFLG_End,


    "alias", 		alias_tag,		TFLG_End,
    "name", 		name_tag,		TFLG_End,
    "description", 	description_tag,	TFLG_End,
    "email", 		email_tag,		TFLG_End,
    "pgpid", 		pgpid_tag,		TFLG_End,
    "homepage", 	homepage_tag,		TFLG_End,
    "portrait", 	portrait_tag,		TFLG_End,
    "note", 		note_tag,		TFLG_End,
    "sex", 		sex_tag,		TFLG_End,
    "birthday", 	birthday_tag,		TFLG_End,
    "title", 		title_tag,		TFLG_End,
    "organization", 	organization_tag,	TFLG_End,
    "street", 		street_tag,		TFLG_End,
    "city", 		city_tag,		TFLG_End,
    "zip", 		zip_tag,		TFLG_End,
    "state", 		state_tag,		TFLG_End,
    "country", 		country_tag,		TFLG_End,
    "mobil", 		mobil_tag,		TFLG_End,
    "fax", 		fax_tag,		TFLG_End,
    "phone", 		phone_tag,		TFLG_End,
    
    NULL
};

static ULONG
findTag(struct tag *tags,STRPTR name,ULONG flags)
{
    for (; tags->name; tags++)
	if ((tags->flags & flags) && !stricmp(tags->name,name)) return tags->id;

    return 0;
}

static void
#ifndef __MORPHOS__
SAVEDS STDARGS
#endif
smStart(void *data,const XML_Char *el,const XML_Char **atts)
{
    register struct smData  *smd = data;
    register struct Library *ExpatBase = smd->ExpatBase;
    register ULONG	    id;

    id = findTag(xmlTags,(STRPTR)el,TFLG_Start);
    if (id==0) return;

    switch (id)
    {
    	case addressbook_tag:
            XML_StopParser(smd->parser,0);
    	    break;

        case newaddressbook_tag:
            smd->newaddressbook_tag = 1;
            break;

        case newgroup_tag:
            if (smd->group = CM_AllocEntry(CMEntry_Group))
            {
            	smd->group->Flags = (smd->ioflags & MUIV_CManager_Import_ClosedGroup) ? 0 : CMGROUPFLG_Open;
                smd->newgroup_tag = 1;
	    }
            else XML_StopParser(smd->parser,0);
    	    break;

        case newcontact_tag:
    	    if (smd->user = CM_AllocEntry(CMEntry_User)) smd->newcontact_tag = 1;
	    else XML_StopParser(smd->parser,0);
            break;

        case private_tag:
	    smd->private_tag = 1;
            break;

        case work_tag:
	    smd->work_tag = 1;
            break;

        default:
            break;
    }
}

static void
#ifndef __MORPHOS__
SAVEDS STDARGS
#endif
smEnd(void *data,const XML_Char *el)
{
    register struct smData  *smd = data;
    STRPTR 		    data_buf = smd->data_buf;
    register ULONG  	    id;

    id = findTag(xmlTags,(STRPTR)el,TFLG_End);
    if (id==0)
    {
        if (data_buf) freeArbitrateVecPooled(data_buf);
    	smd->data_buf = NULL;
    	
        return;
    }

    if (data_buf)
    {
        register STRPTR str = data_buf;
        register int    len = strlen(data_buf);
	
        while (len && isspace((UBYTE)data_buf[len-1]))
	{
	    if (isspace((UBYTE)data_buf[len-1]))
		len--;
	}

	data_buf = CodesetsUTF8ToStr(CODESETSA_Source,  (ULONG)data_buf,
        			     CODESETSA_Codeset, (ULONG)smd->codeset,
                              	     CODESETSA_Pool,	(ULONG)lib_pool,
                              	     CODESETSA_PoolSem,	(ULONG)&lib_poolSem,
                              	     TAG_DONE);

        freeArbitrateVecPooled(str);
    	smd->data_buf = NULL;
    }

    switch (id)
    {
        case newaddressbook_tag:
            smd->newaddressbook_tag = 0;
            break;

        case newgroup_tag:
	    if (smd->newgroup_tag)
	    {
                ADDTAIL(&smd->rootGroup->SubGroups,smd->group);
	    	smd->newgroup_tag = 0;
	    }
    	    break;

        case newcontact_tag:
	    if (smd->newcontact_tag)
            {
		if (*smd->description)
                {
                    if (*smd->comment)
                    {
                    	snprintf(smd->user->Comment,sizeof(smd->user->Comment),"%s\n%s",smd->description,smd->comment);
                        *smd->comment = 0;
                    }
		    else stccpy(smd->user->Comment,smd->description,sizeof(smd->user->Comment));

                    *smd->description = 0;
		}
            	else
                {
                    if (*smd->comment)
		    {
                    	stccpy(smd->user->Comment,smd->comment,sizeof(smd->user->Comment));
                    	*smd->comment = 0;
		    }
                }

                if (smd->cgroup)
                {
		    ADDTAIL(&smd->cgroup->Entries,smd->user);
		    smd->cgroup = NULL;
                }
            	else ADDTAIL(&smd->rootGroup->Entries,smd->user);

	        smd->newcontact_tag = 0;
	    }
            break;

        case group_tag:
            smd->cgroup = findGroup(smd->rootGroup,data_buf);
            break;

        case private_tag:
            smd->private_tag = 0;
            break;

        case work_tag:
            smd->work_tag = 0;
            break;

        case alias_tag:
	    if (smd->newcontact_tag) stccpy(smd->user->Alias,data_buf,sizeof(smd->user->Alias));
    	    break;

        case name_tag:
	    if (smd->newgroup_tag) stccpy(smd->group->Name,data_buf,sizeof(smd->group->Name));
	    else if (smd->newcontact_tag) stccpy(smd->user->Name,data_buf,sizeof(smd->user->Name));
    	    break;

        case description_tag:
	    if (smd->newcontact_tag) stccpy(smd->description,data_buf,sizeof(smd->description));
    	    break;

        case email_tag:
            if (*smd->user->EMail)
            {
	        register struct CMEMail *email;
	
            	if (email = CM_AllocObject(CMALLOCOBJECT_EMail))
                {
	            stccpy(email->EMail,data_buf,sizeof(email->EMail));
		    AddTail((struct List *)&smd->user->EMails,(struct Node *)email);
                }
            }
            else stccpy(smd->user->EMail,data_buf,sizeof(smd->user->EMail));
            break;

        case pgpid_tag:
	    if (smd->newcontact_tag) stccpy(smd->user->PGPUserID,data_buf,sizeof(smd->user->PGPUserID));
            break;

        case homepage_tag:
	    if (smd->newcontact_tag) stccpy(smd->user->WWW,data_buf,sizeof(smd->user->WWW));
            break;

        case portrait_tag:
	    if (smd->newcontact_tag) stccpy(smd->user->ImageFile,data_buf,sizeof(smd->user->ImageFile));
            break;

        case note_tag:
	    if (smd->newcontact_tag) stccpy(smd->comment,data_buf,sizeof(smd->comment));
            break;

        case sex_tag:
	    if (smd->newcontact_tag)
            {
		if (!stricmp(data_buf,"female")) smd->user->Sex = CMUSERSEX_Female;
		if (!stricmp(data_buf,"male")) smd->user->Sex = CMUSERSEX_Male;
            }
    	    break;

        case birthday_tag:
        {
	    STRPTR date = data_buf;
	    ULONG d, m, y;

	    m = atoi(date);
            if (m>=1 && m<=12)
	    {
	        if (date = strchr(date,'/')) date++;
	        if (date)
                {
                    d = atoi(date);
                    if (d>=1 && d<=31)
                    {
		        if (date = strchr(date,'/')) date++;
		    	if (date)
                        {
                            y = atoi(date);

                            smd->user->Birthday = dateToGreg(d,m,y);
			    smd->user->Flags |= CMUSERFLG_Birthday;
                        }
                    }
                }
    	    }

    	    break;
        }

	case title_tag:
	    if (smd->newcontact_tag) stccpy(smd->user->BJobTitle,data_buf,sizeof(smd->user->BJobTitle));
            break;

        case organization_tag:
	    if (smd->newcontact_tag) stccpy(smd->user->Company,data_buf,sizeof(smd->user->Company));
            break;

        case street_tag:
	    if (smd->newcontact_tag)
		if (smd->private_tag) stccpy(smd->user->Address,data_buf,sizeof(smd->user->Address));
		else if (smd->work_tag) stccpy(smd->user->BAddress,data_buf,sizeof(smd->user->BAddress));
            break;

        case city_tag:
	    if (smd->newcontact_tag)
		if (smd->private_tag) stccpy(smd->user->City,data_buf,sizeof(smd->user->City));
		else if (smd->work_tag) stccpy(smd->user->BCity,data_buf,sizeof(smd->user->BCity));
            break;

        case zip_tag:
	    if (smd->newcontact_tag)
		if (smd->private_tag) stccpy(smd->user->ZIP,data_buf,sizeof(smd->user->ZIP));
		else if (smd->work_tag) stccpy(smd->user->BZIP,data_buf,sizeof(smd->user->BZIP));
            break;

        case state_tag:
	    break;

        case country_tag:
	    if (smd->newcontact_tag)
		if (smd->private_tag) stccpy(smd->user->Country,data_buf,sizeof(smd->user->Country));
		else if (smd->work_tag) stccpy(smd->user->Country,data_buf,sizeof(smd->user->BCountry));
            break;

        case mobil_tag:
	    if (smd->newcontact_tag)
		if (smd->private_tag) stccpy(smd->user->Mobile,data_buf,sizeof(smd->user->Mobile));
		else if (smd->work_tag) stccpy(smd->user->BMobile,data_buf,sizeof(smd->user->BMobile));
            break;

        case fax_tag:
	    if (smd->newcontact_tag)
		if (smd->private_tag) stccpy(smd->user->Fax,data_buf,sizeof(smd->user->Fax));
		else if (smd->work_tag) stccpy(smd->user->BFax,data_buf,sizeof(smd->user->BFax));
            break;

        case phone_tag:
	    if (smd->newcontact_tag)
		if (smd->private_tag) stccpy(smd->user->Phone,data_buf,sizeof(smd->user->Phone));
		else if (smd->work_tag) stccpy(smd->user->BPhone,data_buf,sizeof(smd->user->BPhone));
	    break;

    	default:
            break;
    }

    if (data_buf) CodesetsFreeVecPooled(lib_pool,data_buf,CODESETSA_PoolSem,(ULONG)&lib_poolSem,TAG_DONE);
}

static void
#ifndef __MORPHOS__
SAVEDS STDARGS
#endif
smData(void *data,const XML_Char *s,int len)
{
    register struct smData  *smd = data;
    //register struct Library *ExpatBase = smd->ExpatBase;

    if (smd->newcontact_tag || smd->newgroup_tag)
    {
	int old_len = 0;
	
        if (smd->data_buf)
	    old_len = strlen(smd->data_buf);

	if ((smd->data_buf = (STRPTR)reallocArbitrateVecPooled(smd->data_buf,old_len+len+1)))
	{
	    STRPTR src = (STRPTR)s, dest = (STRPTR)smd->data_buf + old_len;

            dest[len] = 0;

	    if (!old_len)
	    {
		while ((isspace(*src) && len))
		{
		    src++;
		    len--;
		}
	    }

	    while (src && len)
	    {
		*dest++ = *src++;
		len--;
	    }

	    *dest = 0;
	}
    }
}

void
importSimpleMail(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register struct Library *ExpatBase;

    if (ExpatBase = OpenLibrary("expat.library",4))
    {
    	register XML_Parser parser;

        if (parser = XML_ParserCreate(NULL))
    	{
            register struct CMGroup *g;

	    if (g = CM_AllocEntry(CMEntry_Group))
    	    {
	        struct smData  smd;
                REGARRAY UBYTE buf[1024];
                register BPTR  file = io->file;

            	strcpy(g->Name,"SimpleMail");
                ADDTAIL(&group->SubGroups,g);

                memset(&smd,0,sizeof(smd));
	        smd.ExpatBase = ExpatBase;
	        smd.codeset   = io->codeset;
                smd.parser    = parser;
	        smd.ioflags   = io->flags;
                smd.rootGroup = g;
                smd.group     = g;

                XML_SetElementHandler(parser,smStart,smEnd);
	        XML_SetCharacterDataHandler(parser,smData);
                XML_SetUserData(parser,&smd);

                for (;;)
                {
                    register int len = Read(file,buf,sizeof(buf));

                    if (len<=0) break;

                    if (XML_Parse(parser,buf,len,0)==XML_STATUS_ERROR)
                        break;
                }

                XML_Parse(parser,"",0,1);

	        if (smd.data_buf) freeArbitrateVecPooled(smd.data_buf);
	    }
	    
            XML_ParserFree(parser);
	}
    	
        CloseLibrary(ExpatBase);
    }
}

/***********************************************************************/

static STRPTR
nextToken(STRPTR *str,UBYTE sep,LONG *val)
{
    if (str && *str)
    {
	STRPTR ptr1, ptr2;

	ptr1 = *str;

    	ptr2 = strchr(ptr1,sep);
        if (ptr2)
        {
	    *ptr2++ = 0;
            *str = ptr2;
	}
    	else *str = NULL;

        if (val) StrToLong(ptr1,val);

        return ptr1;
    }

    return NULL;
}

/***********************************************************************/

void
importATC(struct CMGroup *group,struct MUIS_CManager_IO *io)
{
    register BPTR           file = io->file;
    REGARRAY UBYTE          buf[1024];
    register struct CMGroup *cg;

    fgets(file,buf,sizeof(buf));
    if (stricmp(buf,"VERSION 4")!=0) return;

    if (!(cg = CM_AllocEntry(CMEntry_Group))) return;
    strcpy(cg->Name,"ATC");
    ADDTAIL(&group->SubGroups,cg);

    while (fgets(file,buf,sizeof(buf)))
    {
        STRPTR ptr;

        if (!strncmp(buf,"SERVER",6))
        {
            register struct CMFTP *entry;

            if (!strncmp(buf+7,"<<",2))
            {
                cg = CM_GetParent(group,cg);

                continue;
            }

            if (!strncmp(buf+7,">>",2))
            {
                register struct CMGroup *ng;

                if (ng = CM_AllocEntry(CMEntry_Group))
                {
                    register struct CMGroup *g;

                    io->added++;

                    if (ptr = strchr(buf+9,'\t'))
                    {
                    	*ptr++ = '\0';
			if (atoi(ptr)) ng->Flags = CMGROUPFLG_Open;
		    }

                    stccpy(ng->Name,buf+9,sizeof(ng->Name));

                    if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateGroups) || !(g = checkGroup(group,ng)))
                    {
                        ADDTAIL(&cg->SubGroups,ng);
                        cg = ng;
                    }
                    else
                    {
                        cg = g;
                        CM_FreeEntry(ng);
                    }
                }

                continue;
            }

            if (entry = CM_AllocEntry(CMEntry_FTP))
            {
                UBYTE FTP[256], remote[256], *ptr, *ptr2;
		LONG  v, port;

                io->added++;

                *FTP    = 0;
                *remote = 0;
                port    = 21;

		entry->Flags = CMFTPFLG_Advanced;

                ptr = buf+7;
		if (!(ptr2 = nextToken(&ptr,'\t',NULL))) goto end;
                stccpy(FTP,ptr2,sizeof(FTP));
		    
                if (!(ptr2 = nextToken(&ptr,'\t',NULL))) goto end;
                if (*ptr2) stccpy(entry->Username,ptr2,sizeof(entry->Username));

                if (!(ptr2 = nextToken(&ptr,'\t',NULL))) goto end;
	        if (*ptr2) stccpy(entry->Password,ptr2,sizeof(entry->Password));

                if (!nextToken(&ptr,'\t',&v)) goto end;
                entry->Port = port = v;

                if (!(ptr2 = nextToken(&ptr,'\t',NULL))) goto end;
    	        stccpy(entry->Name,ptr2,sizeof(entry->Name));

                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (v==1) entry->Flags |= CMFTPFLG_Anonymous;
                else if (v==2) entry->Flags |= CMFTPFLG_ADT;

                if (!nextToken(&ptr,'\t',&v)) goto end;
                entry->Retries = v;

                if (!nextToken(&ptr,'\t',&v)) goto end;

                if (!(ptr2 = nextToken(&ptr,'\t',NULL))) goto end;
	        if (*ptr2) stccpy(entry->Local,ptr2,sizeof(entry->Local));

                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (v==1) entry->Flags |= CMFTPFLG_Local;

                if (!(ptr2 = nextToken(&ptr,'\t',NULL))) goto end;
	        if (*ptr2) stccpy(remote,ptr2,sizeof(remote));

                /*if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                if (!nextToken(&ptr,'\t',&v)) goto end;
                entry->WindowStyle = v ? CMFTPWSTYLE_Single : CMFTPWSTYLE_Dual;*/

	    end:
		if (*remote) snprintf(entry->FTP,sizeof(entry->FTP),"%s:%ld/%s",FTP,port,remote);
                else if (port!=21) snprintf(entry->FTP,sizeof(entry->FTP),"%s:%ld",FTP,port);
		     else stccpy(entry->FTP,FTP,sizeof(entry->FTP));

                if (!(io->flags & MUIV_CManager_Import_AvoidDuplicateEntries) || !checkFTP(group,entry))
                    ADDTAIL(&cg->Entries,entry);
        	else CM_FreeEntry(entry);
            }
        }
    }
}

/***********************************************************************/


