/*
**
** Contact Manager - An Amiga Centralized Bookmarks System
**
** Copyright 1999-2006 by Simone [wiz] Tellini https://tellini.info/
** Copyright 2003-2006 by Alfonso [alfie] Ranieri <alforan@tin.it>
** All Rights Are Reserved.
**
** Destributed Under The Terms Of The LGPL II
**
**
**/

#include "lib.h"
#include <dos.h>

/***********************************************************************/

static UBYTE CMID[] = {'C','M','D','F'};

/***********************************************************************/

#define MAXFIELDSIZE 511
#define WORKBUFSIZE  ((MAXFIELDSIZE*6)+1)

struct cmio
{
    BPTR           file;
    ULONG          version;
    struct codeset *codeset;
    UBYTE          buf1[WORKBUFSIZE];
};

#define CMIOSIZE 4096

/***********************************************************************/

static LONG
cmrand(ULONG *seed)
{
    register LONG res, a;

    a     = (LONG)*seed;    // MOVE.L  0040(A4),D0
    res   = a;              // MOVE.L  D0,D1
    res <<= 3;              // ASL.L   #3,D1
    res  -= a;              // SUB.L   D0,D1
    res <<= 3;              // ASL.L   #3,D1
    res  += a;              // ADD.L   D0,D1
    res  += res;            // ADD.L   D1,D1
    res  += a;              // ADD.L   D0,D1
    res <<= 4;              // ASL.L   #4,D1
    res  -= a;              // SUB.L   D0,D1
    res  += res;            // ADD.L   D1,D1
    res  -= a;              // SUB.L   D0,D1
    res  += 0x00000E60;     // ADDI.L  #00000e60,D1
    res  &= 0x7FFFFFFF;     // ANDI.L  #7fffffff,D1
    a     = res;            // MOVE.L  D1,D0
    a    -= 1;              // SUBQ.L  #1,D0
    *seed = (ULONG)a;       // MOVE.L  D0,0040(A4)

    return res;             // MOVE.L  D1,D0
}

/***********************************************************************/

static void
crypt(UBYTE *from,UBYTE *to,int len)
{
    register int i;
    ULONG        seed;

    seed = len;

    for (i = 0; i<len; i++) *to++ = *from++ ^ cmrand(&seed);
}

/***********************************************************************/

BPTR INLINE
open(STRPTR name,ULONG mode)
{
    if (lib_flags & BASEFLG_AsyncIO)
    {
        if (mode==MODE_OLDFILE) mode = MODE_READ;
        else if (mode==MODE_NEWFILE) mode = MODE_WRITE;
             else return NULL;

        return (BPTR)OpenAsync(name,mode,8192);
    }
    else
    {
        return Open(name,mode);
    }
}

void INLINE
close(BPTR file)
{
    if (lib_flags & BASEFLG_AsyncIO) CloseAsync((struct AsyncFile *)file);
    else Close(file);
}

LONG INLINE
fwrite(BPTR file,APTR buf,ULONG len)
{
    return (lib_flags & BASEFLG_AsyncIO) ? WriteAsync((struct AsyncFile *)file,buf,len) : Write(file,buf,len);
}

LONG INLINE
fputc(BPTR file,UBYTE c)
{
    return (lib_flags & BASEFLG_AsyncIO) ? WriteCharAsync((struct AsyncFile *)file,c) : Write(file,&c,sizeof(c));
}

LONG INLINE
fread(BPTR file,APTR buf,ULONG len)
{
    return (lib_flags & BASEFLG_AsyncIO) ? ReadAsync((struct AsyncFile *)file,buf,len) : Read(file,buf,len);
}

LONG INLINE
fgetc(BPTR file)
{
    if (lib_flags & BASEFLG_AsyncIO)
    {
        return ReadCharAsync((struct AsyncFile *)file);
    }
    else
    {
        UBYTE c;

        return Read(file,&c,sizeof(c)) ? c : -1;
    }
}

LONG INLINE
seek(BPTR file,LONG pos,LONG mode)
{
    if (lib_flags & BASEFLG_AsyncIO)
    {
        if (mode==OFFSET_BEGINNING) mode = MODE_START;
        else if (mode==OFFSET_CURRENT) mode = MODE_CURRENT;
             else if (mode==OFFSET_END) mode = MODE_END;
                  else return 0;

        return SeekAsync((struct AsyncFile *)file,pos,mode);
    }
    else
    {
        return Seek(file,pos,mode);
    }
}

/***********************************************************************/

static int
MIN(int a,int b)
{
    return (a<=b) ? a : b;
}

/***********************************************************************/

static ULONG
getString(struct cmio *cmio,STRPTR entry,ULONG entryLen)
{
    register int ml;
    UWORD        len;

    fread(cmio->file,&len,sizeof(UWORD));
    ml = MIN(len,entryLen);

    fread(cmio->file,entry,ml);
    entry[ml] = 0;

    crypt(entry,entry,ml);

    if (ml<len)
    {
        seek(cmio->file,len-ml,OFFSET_CURRENT);
    }

    return 0;
}

/***********************************************************************/

static ULONG
getStringUTF8(struct cmio *cmio,STRPTR entry,ULONG entryLen)
{
    register int ml;
    UWORD        len;

    fread(cmio->file,&len,sizeof(UWORD));
    ml = (len>sizeof(cmio->buf1)) ? sizeof(cmio->buf1)-1 : len;

    fread(cmio->file,cmio->buf1,ml);
    cmio->buf1[ml] = 0;

    if (ml<len)
    {
        seek(cmio->file,len-ml,OFFSET_CURRENT);
    }

    if (*cmio->buf1)
    {
        crypt(cmio->buf1,cmio->buf1,ml);

        if (cmio->codeset)
        {
            CodesetsUTF8ToStr(CODESETSA_Source,        (ULONG)cmio->buf1,
                              CODESETSA_Dest,          (ULONG)entry,
                              CODESETSA_DestLen,       entryLen,
                              CODESETSA_Codeset,       (ULONG)cmio->codeset,
                              CODESETSA_AllocIfNeeded, FALSE,
                              TAG_DONE);
        }
        else
        {
            stccpy(entry,cmio->buf1,entryLen);
        }
    }
    else
    {
        *entry = 0;
    }

    return 0;
}

/***********************************************************************/

static ULONG
putString(struct cmio *cmio,STRPTR entry)
{
    UWORD len;

    len = strlen(entry);

    fwrite(cmio->file,&len,sizeof(UWORD));

    crypt(entry,cmio->buf1,len);
    fwrite(cmio->file,cmio->buf1,len);

    return 0;
}

/***********************************************************************/

static ULONG
putStringUTF8(struct cmio *cmio,STRPTR entry)
{
    if (*entry && cmio->codeset)
    {
        ULONG dlen;
        UWORD len;
        struct TagItem t[] = {CODESETSA_Source,        0, //sasc bhaaa
                              CODESETSA_Codeset,       0,
                              CODESETSA_Dest,          0,
                              CODESETSA_DestLen,       0,
                              CODESETSA_DestLenPtr,    0,
                              CODESETSA_AllocIfNeeded, 0,
                              TAG_DONE};

        t[0].ti_Data = (ULONG)entry;
        t[1].ti_Data = (ULONG)cmio->codeset;
        t[2].ti_Data = (ULONG)cmio->buf1;
        t[3].ti_Data = sizeof(cmio->buf1);
        t[4].ti_Data = (ULONG)&dlen;
        t[5].ti_Data = FALSE;

        CodesetsUTF8CreateA(t);

        len = (UWORD)dlen;

        fwrite(cmio->file,&len,sizeof(UWORD));
        crypt(cmio->buf1,cmio->buf1,len);

        fwrite(cmio->file,cmio->buf1,len);
    }
    else
    {
        UWORD len = strlen(entry);

        fwrite(cmio->file,&len,sizeof(UWORD));

        if (len) crypt(entry,cmio->buf1,len);
        fwrite(cmio->file,cmio->buf1,len);
    }

    return 0;
}

/***********************************************************************/

static APTR
allocEntry(ULONG type)
{
    register struct CMEntry *entry;
    register ULONG          size;

    switch (type)
    {
        case CMEntry_Section:
        case CMEntry_Group:
            size = sizeof(struct CMGroup);
            break;

        case CMEntry_User:
            size = sizeof(struct CMUser);
            break;

        case CMEntry_WWW:
            size = sizeof(struct CMWWW);
            break;

        case CMEntry_FTP:
            size = sizeof(struct CMFTP);
            break;

        case CMEntry_Chat:
            size = sizeof(struct CMChat);
            break;

        case CMEntry_Telnet:
            size = sizeof(struct CMTelnet);
            break;

        case CMEntry_List:
            size = sizeof(struct MinList);
            break;

        default:
            size = 0;
            break;
    }

    if (size) entry = allocVecPooled(lib_pool,size);
    else entry = NULL;

    if (entry)
    {
        memset(entry,0,size);

        entry->Type = type;

        switch (type)
        {
            case CMEntry_Section:
            case CMEntry_Group:
                if (type==CMEntry_Section) entry->Flags = CMGROUPFLG_Bold;
                QUICKNEWLIST(&(CMGROUP(entry)->Entries));
                QUICKNEWLIST(&(CMGROUP(entry)->SubGroups));
                break;

            case CMEntry_User:
                QUICKNEWLIST(&(CMUSER(entry)->EMails));
                break;

            case CMEntry_List:
                QUICKNEWLIST(entry);
                break;
        }
    }

    return entry;
}

/***********************************************************************/

#ifdef __MORPHOS__
APTR CM_AllocEntry(ULONG type)
#else
APTR SAVEDS ASM CM_AllocEntry(REG(d0,ULONG type))
#endif
{
    register struct CMEntry *entry;

    ObtainSemaphore(&lib_poolSem);
    entry = allocEntry(type);
    ReleaseSemaphore(&lib_poolSem);

    return entry;
}

#ifdef __MORPHOS__
APTR LIB_AllocEntry(void)
{
    return CM_AllocEntry((ULONG)REG_D0);
}
#endif

/***********************************************************************/

static void
freeEntry(APTR entry)
{
    if (entry)
    {
        switch (CMENTRY(entry)->Type)
        {
            case CMEntry_Section:
            case CMEntry_Group:
            {
                register struct CMEntry *sub;
                register struct CMGroup *grp;

                while (sub = CMENTRY(RemTail((struct List *)&(CMGROUP(entry)->Entries))))
                    freeEntry(sub);

                while (grp = CMGROUP(RemTail((struct List *)&(CMGROUP(entry)->SubGroups))))
                    freeEntry(grp);

                break;
            }

            case CMEntry_User:
            {
                register struct CMEMail *email, *succ;

                for (email = CMEMAIL(CMUSER(entry)->EMails.mlh_Head); succ = email->Succ; email = succ)
                    freeVecPooled(lib_pool,email);

                break;
            }

            case CMEntry_WWW:  case CMEntry_FTP:
            case CMEntry_Chat: case CMEntry_Telnet:
                break;
        }

        freeVecPooled(lib_pool,entry);
    }
}

/***********************************************************************/

#ifdef __MORPHOS__
void CM_FreeEntry(APTR entry)
#else
void SAVEDS ASM CM_FreeEntry(REG(a0,APTR entry))
#endif
{
    ObtainSemaphore(&lib_poolSem);
    freeEntry(entry);
    ReleaseSemaphore(&lib_poolSem);
}

#ifdef __MORPHOS__
void LIB_FreeEntry(void)
{
    CM_FreeEntry((APTR)REG_A0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
void CM_FreeObject(APTR entry)
#else
void SAVEDS ASM CM_FreeObject(REG(a0,APTR entry))
#endif
{
    freeArbitrateVecPooled(entry);
}

#ifdef __MORPHOS__
void LIB_FreeObject(void)
{
    CM_FreeObject((APTR)REG_A0);
}
#endif

/***********************************************************************/

static void
freeList(struct MinList *list)
{
    if (list)
    {
        register APTR entry;

        while (entry = RemTail((struct List *)list)) freeEntry(entry);
        freeVecPooled(lib_pool,list);
    }
}

/***********************************************************************/

#ifdef __MORPHOS__
void CM_FreeList(struct MinList *list)
#else
void SAVEDS ASM CM_FreeList(REG(a0,struct MinList *list))
#endif
{
    if (list)
    {
        ObtainSemaphore(&lib_poolSem);
        freeList(list);
        ReleaseSemaphore(&lib_poolSem);
    }
}

#ifdef __MORPHOS__
void LIB_FreeList(void)
{
    CM_FreeList((struct MinList *)REG_A0);
}
#endif

/***********************************************************************/

static void
freeSection(struct CMGroup *section)
{
    if (section)
    {
        register struct CMGroup *entry;

        while (entry = CMGROUP(RemTail((struct List *)&section->SubGroups)))
            freeEntry(entry);
    }
}

/***********************************************************************/

static void
freeData(struct CMData *data)
{
    if (data)
    {
        freeSection(data->Users);
        freeSection(data->WWWs );
        freeSection(data->FTPs );
        freeSection(data->Chat );
        if (data->Version>=1) freeSection(data->Telnet);

        if (data->FreeMe)
        {
            freeEntry(data->Users);
            freeEntry(data->WWWs );
            freeEntry(data->FTPs );
            freeEntry(data->Chat );
            if (data->Version>=1) freeEntry(data->Telnet);

            freeVecPooled(lib_pool,data);
        }
    }
}

/***********************************************************************/

#ifdef __MORPHOS__
void CM_FreeData(struct CMData *data)
#else
void SAVEDS ASM CM_FreeData(REG(a0,struct CMData *data))
#endif
{
    freeData(data);
}

#ifdef __MORPHOS__
void LIB_FreeData(void)
{
    CM_FreeData((struct CMData *)REG_A0);
}
#endif

/***********************************************************************/

static struct CMData *
allocCMData(void)
{
    register struct CMData *cm;

    if (cm = allocVecPooled(lib_pool,sizeof(struct CMData)))
    {
        register struct Process *proc;
        register APTR           wnd;

	memset(cm,0,sizeof(*cm));

        cm->FreeMe  = TRUE;
        cm->Version = CMD_CURRENT_VERSION;
        cm->SizeOf  = sizeof(*cm);

        proc = (struct Process *)FindTask(NULL);
        wnd  = proc->pr_WindowPtr;

        proc->pr_WindowPtr = (APTR)-1;

        if (GenesisBase)
        {
            struct User *ud;

            if (ud = GetGlobalUser())
            {
                strcpy(cm->CurrentUser,ud->us_name);

                FreeUser(ud);
            }
        }

        proc->pr_WindowPtr = wnd;

        cm->Users  = CM_AllocEntry(CMEntry_Section);
        cm->WWWs   = CM_AllocEntry(CMEntry_Section);
        cm->FTPs   = CM_AllocEntry(CMEntry_Section);
        cm->Chat   = CM_AllocEntry(CMEntry_Section);
        cm->Telnet = CM_AllocEntry(CMEntry_Section);

        if (cm->Users && cm->WWWs && cm->FTPs && cm->Chat && cm->Telnet)
        {
            stccpy(cm->Users->Name, CM_GetString(Msg_Users ),sizeof(cm->Users->Name ));
            stccpy(cm->WWWs->Name,  CM_GetString(Msg_WWW   ),sizeof(cm->WWWs->Name  ));
            stccpy(cm->FTPs->Name,  CM_GetString(Msg_FTP   ),sizeof(cm->FTPs->Name  ));
            stccpy(cm->Chat->Name,  CM_GetString(Msg_Chat  ),sizeof(cm->Chat->Name  ));
            stccpy(cm->Telnet->Name,CM_GetString(Msg_Telnet),sizeof(cm->Telnet->Name));

        }
        else
        {
            freeData(cm);
            cm = NULL;
        }
    }

    return cm;
}

/***********************************************************************/

#ifdef __MORPHOS__
struct CMData * CM_AllocCMData(void)
#else
struct CMData * SAVEDS ASM CM_AllocCMData(void)
#endif
{
    register struct CMData *cm;

    ObtainSemaphore(&lib_poolSem);
    cm = allocCMData();
    ReleaseSemaphore(&lib_poolSem);

    return cm;
}

#ifdef __MORPHOS__
struct CMData *LIB_AllocCMData(void)
{
    return CM_AllocCMData();
}
#endif

/***********************************************************************/

#define MINSTACKSIZE 512

ULONG
checkStack(void)
{
#ifdef __MORPHOS__
    return TRUE;
#else
    register struct Process *pr = (struct Process *)FindTask(NULL);
    register ULONG          a7, avail;

    a7 = getreg(REG_A7);
    avail = a7-(ULONG)pr->pr_Task.tc_SPLower;

    return (ULONG)(avail>MINSTACKSIZE);
#endif
}

/***********************************************************************/

static ULONG
loadUsers(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR  file = cmio->file;
    register ULONG i, res;
    ULONG          n;

    if (!group) return 0;
    if (!checkStack()) return 3;
    res = 0;

    NEWLIST(&group->Entries);
    NEWLIST(&group->SubGroups);

    group->Flags = fgetc(file);
    fgetc(file);

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMUser *entry;

        if (entry = allocEntry(CMEntry_User))
        {
            entry->Flags = fgetc(file);
            fgetc(file);

            getStringUTF8(cmio,entry->Name,    sizeof(entry->Name     ));
            getStringUTF8(cmio,entry->LastName,sizeof(entry->LastName ));
            getStringUTF8(cmio,entry->Address, sizeof(entry->Address  ));
            getStringUTF8(cmio,entry->City,    sizeof(entry->City     ));
            getStringUTF8(cmio,entry->Country, sizeof(entry->Country  ));
            getStringUTF8(cmio,entry->ZIP,     sizeof(entry->ZIP      ));
            getStringUTF8(cmio,entry->Comment, sizeof(entry->Comment  ));
            getStringUTF8(cmio,entry->EMail,   sizeof(entry->EMail    ));
            getStringUTF8(cmio,entry->WWW,     sizeof(entry->WWW      ));
            getStringUTF8(cmio,entry->FTP,     sizeof(entry->FTP      ));
            getStringUTF8(cmio,entry->Phone,   sizeof(entry->Phone    ));
            getStringUTF8(cmio,entry->Fax,     sizeof(entry->Fax      ));
            getStringUTF8(cmio,entry->Alias,   sizeof(entry->Alias    ));

            if (cmio->version>=1)
            {
                getStringUTF8(cmio,entry->Mobile,sizeof(entry->Mobile));

                if (cmio->version>=4)
                {
                    getStringUTF8(cmio,entry->ICQ,sizeof(entry->ICQ));

                    if (cmio->version>=5)
                    {
                        getStringUTF8(cmio,entry->Company,    sizeof(entry->Company     ));
                        getStringUTF8(cmio,entry->BJobTitle,  sizeof(entry->BJobTitle   ));
                        getStringUTF8(cmio,entry->BOffice,    sizeof(entry->BOffice     ));
                        getStringUTF8(cmio,entry->BAddress,   sizeof(entry->BAddress    ));
                        getStringUTF8(cmio,entry->BCity,      sizeof(entry->BCity       ));
                        getStringUTF8(cmio,entry->BCountry,   sizeof(entry->BCountry    ));
                        getStringUTF8(cmio,entry->BZIP,       sizeof(entry->BZIP        ));
                        getStringUTF8(cmio,entry->BDepartment,sizeof(entry->BDepartment ));
                        getStringUTF8(cmio,entry->BPhone,     sizeof(entry->BPhone      ));
                        getStringUTF8(cmio,entry->BFax,       sizeof(entry->BFax        ));
                        getStringUTF8(cmio,entry->BEMail,     sizeof(entry->BEMail      ));

                        if (cmio->version>=6)
                        {
                            getStringUTF8(cmio,entry->PGPUserID,sizeof(entry->PGPUserID));

                            if (cmio->version>=7)
                            {
                                getStringUTF8(cmio,entry->BMobile,sizeof(entry->BMobile));

                                if (cmio->version>=12)
                                {
                                    getString(cmio,entry->ImageFile,sizeof(entry->ImageFile));

                                    if (cmio->version>=14)
                                    {
                                        getStringUTF8(cmio,entry->VisualName,sizeof(entry->VisualName));

                                        if (cmio->version>=15)
                                        {
                                            fread(file,&entry->Sex,sizeof(entry->Sex));
                                            fread(file,&entry->Birthday,sizeof(entry->Birthday));
                                            fread(file,&entry->Anniversary,sizeof(entry->Anniversary));

                                            if (cmio->version>=16)
                                            {
                                                register ULONG j;
                                                ULONG          emails;

                                                NEWLIST(&entry->EMails);

                                                fread(file,&emails,sizeof(emails));

                                                for (j = 0; j<emails; j++)
                                                {
                                                    register struct CMEMail *email;

                                                    if (email = allocVecPooled(lib_pool,sizeof(struct CMEMail)))
                                                    {
                                                        memset(email,0,sizeof(*email));

                                                        getStringUTF8(cmio,email->EMail,sizeof(email->EMail));
                                                        fread(file,&email->Flags,sizeof(email->Flags));

                                                        AddTail((struct List *)&entry->EMails,(struct Node *)email);
                                                    }
                                                    else
                                                    {
                                                        struct CMEMail dummy;

                                                        getString(cmio,dummy.EMail,sizeof(dummy.EMail));
                                                        fread(file,&dummy.Flags,sizeof(dummy.Flags));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            AddTail((struct List *)&group->Entries,(struct Node *)entry);
        }
    }

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMGroup *ngroup;

        if (ngroup = allocEntry(CMEntry_Group))
        {
            register ULONG res;

            AddTail((struct List *)&group->SubGroups,(struct Node *)ngroup);

            getStringUTF8(cmio,ngroup->Name,sizeof(ngroup->Name));
            if (res = loadUsers(cmio,ngroup)) break;
        }
    }

    return res;
}

/***********************************************************************/

static ULONG
loadWWWs(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR  file = cmio->file;
    register ULONG i, res;
    ULONG          n;

    if (!group) return 0;
    if (!checkStack()) return 3;
    res = 0;

    NEWLIST(&group->Entries);
    NEWLIST(&group->SubGroups);

    group->Flags = fgetc(file);
    fgetc(file);

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMWWW *entry;

        if (entry = allocEntry(CMEntry_WWW))
        {
            entry->Flags = fgetc(file);
            fgetc(file);

            getStringUTF8(cmio,entry->Name,sizeof(entry->Name));
            getStringUTF8(cmio,entry->Comment,sizeof(entry->Comment));
            getStringUTF8(cmio,entry->WWW,sizeof(entry->WWW));
            getStringUTF8(cmio,entry->WebMaster,sizeof(entry->WebMaster));
            getStringUTF8(cmio,entry->EMail,sizeof(entry->EMail));

            if (cmio->version>=8)
            {
                fread(file,&entry->LastModified,sizeof(entry->LastModified));
                fread(file,&entry->LastVisited, sizeof(entry->LastVisited ));

                if (cmio->version>=10) getStringUTF8(cmio,entry->Alias,sizeof(entry->Alias));
            }

            AddTail((struct List *)&group->Entries,(struct Node *)entry);
        }
    }

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMGroup *ngroup;

        if (ngroup = allocEntry(CMEntry_Group))
        {
            AddTail((struct List *)&group->SubGroups,(struct Node *)ngroup);
            getStringUTF8(cmio,ngroup->Name,sizeof(ngroup->Name));
            if (res = loadWWWs(cmio,ngroup)) break;
        }
    }

    return res;
}

/***********************************************************************/

static ULONG
loadFTPs(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR  file = cmio->file;
    register ULONG i, res;
    ULONG          n;

    if (!group) return 0;
    if (!checkStack()) return 3;
    res = 0;

    NEWLIST(&group->Entries);
    NEWLIST(&group->SubGroups);

    group->Flags = fgetc(file);
    fgetc(file);

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMFTP *entry;

        if (entry = allocEntry(CMEntry_FTP))
        {
            entry->Flags = fgetc(file);
            fgetc(file);

            fread(file,&entry->Port,sizeof(entry->Port));
            fread(file,&entry->Retries,sizeof(entry->Retries));

            getStringUTF8(cmio,entry->Name,     sizeof(entry->Name));
            getStringUTF8(cmio,entry->Comment,  sizeof(entry->Comment));
            getStringUTF8(cmio,entry->FTP,      sizeof(entry->FTP));
            getStringUTF8(cmio,entry->Username, sizeof(entry->Username));
            getStringUTF8(cmio,entry->Password, sizeof(entry->Password));

            if (cmio->version>=2)
            {
                getString(cmio,entry->Local,sizeof(entry->Local));

                if (cmio->version>=8)
                {
                    fread(file,&entry->LastModified,sizeof(entry->LastModified));
                    fread(file,&entry->LastVisited, sizeof(entry->LastVisited ));

                    if (cmio->version>=10)
                    {
                        getStringUTF8(cmio,entry->Alias,sizeof(entry->Alias));

                        if (cmio->version>=11)
                        {
                            fread(file,&entry->ExtraFlags,sizeof(entry->ExtraFlags));
                            fread(file,&entry->WindowStyle,sizeof(entry->WindowStyle));

                            if (cmio->version>=13)
                            {
                                getStringUTF8(cmio,entry->EMail,sizeof(entry->EMail));
                            }
                        }
                    }
                }
            }

            AddTail((struct List *)&group->Entries,(struct Node *)entry);
        }
    }

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMGroup *ngroup;

        if (ngroup = allocEntry(CMEntry_Group))
        {
            AddTail((struct List *)&group->SubGroups,(struct Node *)ngroup);
            getStringUTF8(cmio,ngroup->Name,sizeof(ngroup->Name));
            if (res = loadFTPs(cmio,ngroup)) break;
        }
    }

    return res;
}

/***********************************************************************/

static ULONG
loadChat(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR  file = cmio->file;
    register ULONG i, res;
    ULONG          n;

    if (!group) return 0;
    if (!checkStack()) return 3;
    res = 0;

    NEWLIST(&group->Entries);
    NEWLIST(&group->SubGroups);

    group->Flags = fgetc(file);
    fgetc(file);

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMChat *entry;

        if (entry = allocEntry(CMEntry_Chat))
        {
            entry->Flags = fgetc(file);
            fgetc(file);

            getStringUTF8(cmio,entry->Channel,sizeof(entry->Channel));
            getStringUTF8(cmio,entry->Server,sizeof(entry->Server));
            getStringUTF8(cmio,entry->Maintainer,sizeof(entry->Maintainer));
            getStringUTF8(cmio,entry->Nick,sizeof(entry->Nick));
            getStringUTF8(cmio,entry->WWW,sizeof(entry->WWW));
            getStringUTF8(cmio,entry->Comment,sizeof(entry->Comment));

            if (cmio->version>=3)
            {
                getStringUTF8(cmio,entry->Password,sizeof(entry->Password));

                if (cmio->version>=13)
                {
                    getStringUTF8(cmio,entry->EMail,sizeof(entry->EMail));
                }
            }

            AddTail((struct List *)&group->Entries,(struct Node *)entry);
        }
    }

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMGroup *ngroup;

        if (ngroup = allocEntry(CMEntry_Group))
        {
            AddTail((struct List *)&group->SubGroups,(struct Node *)ngroup);
            getStringUTF8(cmio,ngroup->Name,sizeof(ngroup->Name));
            if (res = loadChat(cmio,ngroup)) break;
        }
    }

    return res;
}

/***********************************************************************/

static ULONG
loadTelnet(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR  file = cmio->file;
    register ULONG i, res;
    ULONG          n;

    if (!group) return 0;
    if (!checkStack()) return 3;
    res = 0;

    NEWLIST(&group->Entries);
    NEWLIST(&group->SubGroups);

    group->Flags = fgetc(file);
    fgetc(file);

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMTelnet *entry;

        if (entry = allocEntry(CMEntry_Telnet))
        {
            entry->Flags = fgetc(file);
            fgetc(file);

            getStringUTF8(cmio,entry->Host,sizeof(entry->Host));
            getStringUTF8(cmio,entry->Login,sizeof(entry->Login));
            getStringUTF8(cmio,entry->Password,sizeof(entry->Password));
            getStringUTF8(cmio,entry->Comment,sizeof(entry->Comment));

            fread(file,&entry->Port,sizeof(ULONG));

            if (cmio->version>=13)
            {
                getStringUTF8(cmio,entry->EMail,sizeof(entry->EMail));

                if (cmio->version>=18)
                {
                    getStringUTF8(cmio,entry->Alias,sizeof(entry->Alias));
                }
            }

            AddTail((struct List *)&group->Entries,(struct Node *)entry);
        }
    }

    fread(file,&n,sizeof(ULONG));

    for (i = 0; i<n; i++)
    {
        register struct CMGroup *ngroup;

        if (ngroup = allocEntry(CMEntry_Group))
        {
            AddTail((struct List *)&group->SubGroups,(struct Node *)ngroup);
            getStringUTF8(cmio,ngroup->Name,sizeof(ngroup->Name));
            if (res = loadTelnet(cmio,ngroup)) break;
        }
    }

    return res;
}

/***********************************************************************/

static ULONG
saveUsers(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR          file = cmio->file;
    register struct CMUser *entry;
    register APTR          succ1, succ2;
    ULONG                  n;
    register ULONG         res;

    if (!checkStack()) return 3;
    res = 0;

    fputc(file,group->Flags);
    fputc(file,0);

    n = countNodes(&group->Entries);
    fwrite(file,&n,sizeof(ULONG));

    for (entry = (struct CMUser *)group->Entries.mlh_Head; succ1 = entry->Succ; entry = succ1)
    {
        register struct CMEMail *email;

        fputc(file,entry->Flags);
        fputc(file,0);

        putStringUTF8(cmio,entry->Name);
        putStringUTF8(cmio,entry->LastName);
        putStringUTF8(cmio,entry->Address);
        putStringUTF8(cmio,entry->City);
        putStringUTF8(cmio,entry->Country);
        putStringUTF8(cmio,entry->ZIP);
        putStringUTF8(cmio,entry->Comment);
        putStringUTF8(cmio,entry->EMail);
        putStringUTF8(cmio,entry->WWW);
        putStringUTF8(cmio,entry->FTP);
        putStringUTF8(cmio,entry->Phone);
        putStringUTF8(cmio,entry->Fax);
        putStringUTF8(cmio,entry->Alias);
        putStringUTF8(cmio,entry->Mobile);
        putStringUTF8(cmio,entry->ICQ);
        putStringUTF8(cmio,entry->Company);
        putStringUTF8(cmio,entry->BJobTitle);
        putStringUTF8(cmio,entry->BOffice);
        putStringUTF8(cmio,entry->BAddress);
        putStringUTF8(cmio,entry->BCity);
        putStringUTF8(cmio,entry->BCountry);
        putStringUTF8(cmio,entry->BZIP);
        putStringUTF8(cmio,entry->BDepartment);
        putStringUTF8(cmio,entry->BPhone);
        putStringUTF8(cmio,entry->BFax);
        putStringUTF8(cmio,entry->BEMail);
        putStringUTF8(cmio,entry->PGPUserID);
        putStringUTF8(cmio,entry->BMobile);
        putString(cmio,entry->ImageFile);
        putStringUTF8(cmio,entry->VisualName);
        fwrite(file,&entry->Sex,sizeof(entry->Sex));
        fwrite(file,&entry->Birthday,sizeof(entry->Birthday));
        fwrite(file,&entry->Anniversary,sizeof(entry->Anniversary));

        n = countNodes(&entry->EMails);
        fwrite(file,&n,sizeof(ULONG));

        for (email = CMEMAIL(entry->EMails.mlh_Head); succ2 = email->Succ; email = succ2)
        {
            putStringUTF8(cmio,email->EMail);
            fwrite(file,&email->Flags,sizeof(email->Flags));
        }
    }

    n = countNodes(&group->SubGroups);
    fwrite(file,&n,sizeof(ULONG));

    for (group = CMGROUP(group->SubGroups.mlh_Head); succ1 = group->Succ; group = succ1)
    {
        putStringUTF8(cmio,group->Name);
        if (res = saveUsers(cmio,group)) break;
    }

    return res;
}

/***********************************************************************/

static ULONG
saveWWWs(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR          file = cmio->file;
    register struct CMWWW  *entry;
    register APTR          succ;
    ULONG                  n;
    register ULONG         res;

    if (!checkStack()) return 3;
    res = 0;

    fputc(file,group->Flags);
    fputc(file,0);

    n = countNodes(&group->Entries);

    fwrite(file,&n,sizeof(ULONG));

    for (entry = (struct CMWWW *)group->Entries.mlh_Head; succ = entry->Succ; entry = succ)
    {
        fputc(file,entry->Flags);
        fputc(file,0);

        putStringUTF8(cmio,entry->Name);
        putStringUTF8(cmio,entry->Comment);
        putStringUTF8(cmio,entry->WWW);
        putStringUTF8(cmio,entry->WebMaster);
        putStringUTF8(cmio,entry->EMail);

        fwrite(file,&entry->LastModified,sizeof(entry->LastModified));
        fwrite(file,&entry->LastVisited,sizeof(entry->LastVisited ));

        putStringUTF8(cmio,entry->Alias);
    }

    n = countNodes(&group->SubGroups);

    fwrite(file,&n,sizeof(ULONG));

    for (group = CMGROUP(group->SubGroups.mlh_Head); succ = group->Succ; group = succ)
    {
        putStringUTF8(cmio,group->Name);
        if (res = saveWWWs(cmio,group)) break;
    }

    return res;
}

/***********************************************************************/

static ULONG
saveFTPs(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR          file = cmio->file;
    register struct CMFTP  *entry;
    register APTR          succ;
    ULONG                  n;
    register ULONG         res;

    if (!checkStack()) return 3;
    res = 0;

    fputc(file,group->Flags);
    fputc(file,0);

    n = countNodes(&group->Entries);

    fwrite(file,&n,sizeof(ULONG));

    for (entry = (struct CMFTP *)group->Entries.mlh_Head; succ = entry->Succ; entry = succ)
    {
        fputc(file,entry->Flags);
        fputc(file,0);

        fwrite(file,&entry->Port,   sizeof(entry->Port   ));
        fwrite(file,&entry->Retries,sizeof(entry->Retries));

        putStringUTF8(cmio,entry->Name);
        putStringUTF8(cmio,entry->Comment);
        putStringUTF8(cmio,entry->FTP);
        putStringUTF8(cmio,entry->Username);
        putStringUTF8(cmio,entry->Password);
        putStringUTF8(cmio,entry->Local);

        fwrite(file,&entry->LastModified,sizeof(entry->LastModified));
        fwrite(file,&entry->LastVisited, sizeof(entry->LastVisited ));

        putStringUTF8(cmio,entry->Alias);

        fwrite(file,&entry->ExtraFlags, sizeof(entry->ExtraFlags ));
        fwrite(file,&entry->WindowStyle,sizeof(entry->WindowStyle));
        putStringUTF8(cmio,entry->EMail);
    }

    n = countNodes(&group->SubGroups);

    fwrite(file,&n,sizeof(ULONG));

    for (group = CMGROUP(group->SubGroups.mlh_Head); succ = group->Succ; group = succ)
    {
        putStringUTF8(cmio,group->Name);
        if (res = saveFTPs(cmio,group)) break;
    }

    return res;
}

/***********************************************************************/

static ULONG
saveChat(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR          file = cmio->file;
    register struct CMChat *entry;
    register APTR          succ;
    ULONG                  n;
    register ULONG         res;

    if (!checkStack()) return 3;
    res = 0;

    fputc(file,group->Flags);
    fputc(file,0);

    n = countNodes(&group->Entries);

    fwrite(file,&n,sizeof(ULONG));

    for (entry = (struct CMChat *)group->Entries.mlh_Head; succ = entry->Succ; entry = succ)
    {
        fputc(file,entry->Flags);
        fputc(file,0);

        putStringUTF8(cmio,entry->Channel);
        putStringUTF8(cmio,entry->Server);
        putStringUTF8(cmio,entry->Maintainer);
        putStringUTF8(cmio,entry->Nick);
        putStringUTF8(cmio,entry->WWW);
        putStringUTF8(cmio,entry->Comment);
        putStringUTF8(cmio,entry->Password);
        putStringUTF8(cmio,entry->EMail);
    }

    n = countNodes(&group->SubGroups);

    fwrite(file,&n,sizeof(ULONG));

    for (group = CMGROUP(group->SubGroups.mlh_Head); succ = group->Succ; group = succ)
    {
        putStringUTF8(cmio,group->Name);
        if (res = saveChat(cmio,group)) break;
    }

    return res;
}

/***********************************************************************/

static ULONG
saveTelnet(struct cmio *cmio,struct CMGroup *group)
{
    register BPTR          file = cmio->file;
    register struct CMTelnet *entry;
    register APTR            succ;
    ULONG                    n;
    register ULONG         res;

    if (!checkStack()) return 3;
    res = 0;

    fputc(file,group ? group->Flags : 0);
    fputc(file,0);

    n = group ? countNodes(&group->Entries) : 0;

    fwrite(file,&n,sizeof(ULONG));

    if (group)
        for (entry = (struct CMTelnet *)group->Entries.mlh_Head; succ = entry->Succ; entry = succ)
        {

            fputc(file,entry->Flags);
            fputc(file,0);

            putStringUTF8(cmio,entry->Host);
            putStringUTF8(cmio,entry->Login);
            putStringUTF8(cmio,entry->Password);
            putStringUTF8(cmio,entry->Comment);

            fwrite(file,&entry->Port,sizeof(ULONG));
            putStringUTF8(cmio,entry->EMail);
            putStringUTF8(cmio,entry->Alias);
        }

    n = group ? countNodes(&group->SubGroups) : 0;

    fwrite(file,&n,sizeof(ULONG));

    if (group)
    {
        for (group = CMGROUP(group->SubGroups.mlh_Head); succ = group->Succ; group = succ)
        {
            putStringUTF8(cmio,group->Name);
            if (res = saveTelnet(cmio,group)) break;
        }
    }

    return res;
}

/***********************************************************************/

static ULONG
sendMsg(struct CMHandle *cm,ULONG command,ULONG data)
{
    register ULONG res = FALSE;

    Forbid();

    if (FindPort(cm->CMARexx))
    {
        if (command==CMCOMMAND_Quit)
        {
            Signal(cm->CMPort->mp_SigTask,SIGBREAKF_CTRL_C);
        }
        else
        {
            struct CMMessage msg;

            memset(&msg,0,sizeof(msg));
            INITMESSAGE(&msg,cm->MyPort,sizeof(msg));
            msg.Command = command;
            msg.Data    = data;

            PutMsg(cm->CMPort,(struct Message *)&msg);

            Permit();

            WaitPort(cm->MyPort);
            GetMsg(cm->MyPort);
        }

        res = TRUE;

    }
    else Permit();

    return res;
}

/***********************************************************************/

#ifdef __MORPHOS__
void CM_FreeHandle(APTR handle,ULONG close)
#else
void SAVEDS ASM CM_FreeHandle(REG(a0,APTR handle),REG(d0,ULONG close))
#endif
{
    register struct CMHandle *cm;

    if (cm = handle)
    {
        if (cm->MyPort)
        {
            if (close) sendMsg(cm,CMCOMMAND_Quit,0);
            DeleteMsgPort(cm->MyPort);
        }

        freeArbitrateVecPooled(cm);
    }
}

#ifdef __MORPHOS__
void LIB_FreeHandle(void)
{
    CM_FreeHandle((APTR)REG_A0,(ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
APTR CM_StartCManagerA(struct TagItem *attrs)
#else
APTR SAVEDS ASM CM_StartCManagerA(REG(a0,struct TagItem *attrs))
#endif
{
    register struct CMHandle *cm;

    if (cm = allocArbitrateVecPooled(sizeof(struct CMHandle)))
    {
        REGARRAY UBYTE  path[256] = "", command[512];
        register STRPTR file, pubScreen;
        register ULONG  ok = FALSE;
        register BPTR   lock;

	memset(cm,0,sizeof(*cm));

        GetVar("Vapor/CManager_Path",path,sizeof(path),0);
        strcpy(command,path);
        if (!(lock = Lock(command,ACCESS_READ)))
        {
            if (lock = Lock("CManager",ACCESS_READ)) strcpy(command,"CManager");
        }

        if (lock)
        {
            UnLock(lock);

            if (file = (STRPTR)GetTagData(CMSTARTA_File,NULL,attrs))
            {
                strcat(command," \"");
                strcat(command,file);
                strcat(command,"\"");
            }

            if (pubScreen = (STRPTR)GetTagData(CMSTARTA_PubScreen,NULL,attrs))
            {
                strcat(command," PUBSCREEN \"");
                strcat(command,pubScreen);
                strcat(command,"\"");
            }

            if (GetTagData(CMSTARTA_HideUsers,FALSE,attrs))  strcat(command," HIDEUSER");
            if (GetTagData(CMSTARTA_HideWWW,FALSE,attrs))    strcat(command," HIDEWWW");
            if (GetTagData(CMSTARTA_HideFTP,FALSE,attrs))    strcat(command," HIDEFTP");
            if (GetTagData(CMSTARTA_HideChat,FALSE,attrs))   strcat(command," HIDECHAT");
            if (GetTagData(CMSTARTA_HideTelnet,FALSE,attrs)) strcat(command," HIDETELNET");

            if (cm->MyPort = CreateMsgPort())
            {
                sprintf(path," PORT %ld",(ULONG)cm->MyPort);
                strcat(command,path);

                if (SystemTags(command,SYS_Asynch, TRUE,
                                       SYS_Input,  Open("NIL:",MODE_NEWFILE),
                                       SYS_Output, Open("NIL:",MODE_NEWFILE),
                                       TAG_END)==0)
                {
                    register ULONG fail = FALSE;

                    for (;;)
                    {
                        register struct CMMessage *msg;

                        WaitPort(cm->MyPort);

                        while (msg = (struct CMMessage *)GetMsg(cm->MyPort))
                        {
                            switch (msg->Command)
                            {
                                case CMCOMMAND_Fail:
                                    fail = TRUE;
                                    break;

                                case CMCOMMAND_Port:
                                    cm->CMPort = (struct MsgPort *)msg->Data;
                                    break;

                                case CMCOMMAND_ARexx:
                                    cm->CMARexx = (STRPTR)msg->Data;
                                    break;

                            }

                            ReplyMsg((struct Message *)msg);
                        }

                        if (fail) break;

                        if (cm->CMPort && cm->CMARexx)
                        {
                            ok = TRUE;
                            break;
                        }
                    }
                }
            }
        }

        if (ok==FALSE)
        {
            CM_FreeHandle(cm,FALSE);
            cm = NULL;
        }
    }

    return cm;
}

#ifdef __MORPHOS__
APTR LIB_StartCManagerA(struct TagItem *attrs)
{
    return CM_StartCManagerA((struct TagItem *)REG_A0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
APTR CM_StartManager(STRPTR file,STRPTR pubScreen)
#else
APTR SAVEDS ASM CM_StartManager(REG(a0,STRPTR file),REG(a1,STRPTR pubScreen))
#endif
{
    REGARRAY struct TagItem attrs[] = {CMSTARTA_File,0,CMSTARTA_PubScreen,0,TAG_DONE};

    attrs[0].ti_Data = (ULONG)file;
    attrs[1].ti_Data = (ULONG)pubScreen;

    return CM_StartCManagerA(attrs);
}

#ifdef __MORPHOS__
APTR LIB_StartManager(void)
{
    return CM_StartManager((STRPTR)REG_A0,(STRPTR)REG_A1);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
APTR CM_GetEntry(APTR handle,ULONG flags)
#else
APTR SAVEDS ASM CM_GetEntry(REG(a0,APTR handle),REG(d0,ULONG flags))
#endif
{
    register struct CMHandle *cm;
    register APTR            entry = NULL;

    if (cm = handle)
    {
        if (sendMsg(cm,CMCOMMAND_GetEntry,flags))
        {
            register ULONG run = TRUE;

            do
            {
                register struct CMMessage *msg;

                WaitPort(cm->MyPort);

                while(msg = (struct CMMessage *)GetMsg(cm->MyPort))
                {
                    if (msg->Command==CMCOMMAND_GetEntry)
                    {
                        entry = (APTR) msg->Data;
                        run   = FALSE;
                    }

                    ReplyMsg((struct Message *)msg);
                }

            }
            while(run);
        }
    }

    return entry;
}

#ifdef __MORPHOS__
APTR LIB_GetEntry(void)
{
    return CM_GetEntry((APTR)REG_A0,(ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
struct CMGroup * CM_GetParent(struct CMGroup *list,struct CMGroup *current)
#else
struct CMGroup * SAVEDS ASM CM_GetParent(REG(a0,struct CMGroup *list),REG(a1,struct CMGroup *current))
#endif
{
    register struct CMGroup *group;
    register APTR           succ;

    if (current==list) return list;

    for (group = CMGROUP(list->SubGroups.mlh_Head); succ = group->Succ; group = succ)
    {
        register struct CMGroup *temp;

        if (group==current) return list;
        else if (temp = CM_GetParent(group,current)) return temp;
    }

    return NULL;
}

#ifdef __MORPHOS__
struct CMGroup * LIB_GetParent(void)
{
    return CM_GetParent((struct CMGroup *)REG_A0,(struct CMGroup *)REG_A1);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
APTR CM_AllocObject(ULONG type)
#else
APTR SAVEDS ASM CM_AllocObject(REG(d0,ULONG type))
#endif
{
    register APTR  o;
    register ULONG size;

    switch (type)
    {
        case CMALLOCOBJECT_EMail:
            size = sizeof(struct CMEMail);
            break;

        case CMALLOCOBJECT_List:
            size = sizeof(struct MinList);
            break;

        default:
            size = 0;
            break;
    }

    if (size)
    {
        if (o = allocArbitrateVecPooled(size))
        {
            memset(o,0,size);

            switch (type)
            {
                case CMALLOCOBJECT_List:
                    QUICKNEWLIST(o);
                    break;
            }
        }
    }
    else o = NULL;

    return o;
}

#ifdef __MORPHOS__
APTR LIB_AllocObject(void)
{
    return CM_AllocObject((ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CM_GetOption(ULONG option)
#else
ULONG SAVEDS ASM CM_GetOption(REG(d0,ULONG option))
#endif
{
    register ULONG res;

    switch (option)
    {
        case CMGETOPTION_AvoidGenesis:
            res = (lib_flags & BASEFLG_AvoidGenesis) ? TRUE : FALSE;
            break;

        case CMGETOPTION_Version:
            res = (ULONG)lib_fullName;
            break;

        default:
            res = 0;
            break;
    }

    return res;
}

#ifdef __MORPHOS__
ULONG LIB_GetOption(void)
{
    return CM_GetOption((ULONG)REG_D0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CM_LoadDataA(struct TagItem *attrs)
#else
ULONG SAVEDS ASM CM_LoadDataA(REG(a0,struct TagItem *attrs))
#endif
{
    register struct CMData *data;
    register STRPTR        fileName, user;
    register ULONG         res, cuser;

    ObtainSemaphore(&lib_poolSem);

    if (cuser = GetTagData(CMA_LoadData_Current,FALSE,attrs))
    {
        REGARRAY UBYTE file[256];

        if (!(data = CM_AllocCMData()))
        {
            ReleaseSemaphore(&lib_poolSem);
            return NULL;
        }

        data->CurrentUser[0] = 0;
        data->Path[0]        = 0;
        strcpy(file,"Default.data");

        if (GenesisBase)
        {
            register struct User *ud;

            if ((!(ud = GetGlobalUser())) && GetTagData(CMA_LoadData_Ask,FALSE,attrs)) ud = GetUser(NULL,NULL,NULL,0);
            else GetUserName(0,data->CurrentUser,sizeof(data->CurrentUser));

            if (ud)
            {
                strcpy(data->CurrentUser,ud->us_name);
                SetGlobalUser(ud);
                FreeUser(ud);
            }

            if (data->CurrentUser[0]) sprintf(file,"%s/%s.data",data->CurrentUser,data->CurrentUser);
        }

        strcpy(data->Path,lib_CMPath);
        AddPart(data->Path,file,sizeof(data->Path));

        fileName = data->Path;
        user = data->CurrentUser[0] ? data->CurrentUser : NULL;
    }
    else
    {
        data     = (struct CMData *)GetTagData(CMA_LoadData_CMData,NULL,attrs);
        user     = (STRPTR)GetTagData(CMA_LoadData_User,NULL,attrs);
        fileName = (STRPTR)GetTagData(CMA_LoadData_File,NULL,attrs);
    }

    res = FALSE;

    if (data && fileName)
    {
        register struct cmio *cmio;

        if (cmio = AllocPooled(lib_pool,CMIOSIZE))
        {
            memset(cmio,0,sizeof(*cmio));

            if (cmio->file = open(fileName,MODE_OLDFILE))
            {
                UBYTE ID[4];

                fread(cmio->file,ID,sizeof(ID));

                if (memcmp(CMID,ID,sizeof(ID))==0)
                {
                    REGARRAY UBYTE fileUser[41];

                    fread(cmio->file,&cmio->version,sizeof(ULONG));

                    if (cmio->version>=CMD_MINVER)
                    {
                        if (cmio->version<CMD_UTF8_VERSION) cmio->codeset = NULL;
                        else cmio->codeset  = (struct codeset *)GetTagData(CMA_LoadData_Codeset,NULL,attrs);

                        getString(cmio,fileUser,sizeof(fileUser));

                        if ((user==NULL) || (fileUser[0]=='\0') || (stricmp(fileUser,user)==0))
                        {
                            if (cmio->version>=CMD_UTF8_VERSION && !cmio->codeset)
                            {
                                *cmio->buf1 = 0;

                                userCodeset(fileUser,cmio->buf1,sizeof(cmio->buf1));
                                cmio->codeset = CodesetsFindA(cmio->buf1,NULL);
                            }

                            loadUsers(cmio,data->Users);
                            loadWWWs(cmio,data->WWWs);
                            loadFTPs(cmio,data->FTPs);
                            loadChat(cmio,data->Chat);
                            loadTelnet(cmio,data->Telnet);
                        }

                        res = TRUE;
                    }
                }

                close(cmio->file);
            }

            FreePooled(lib_pool,cmio,CMIOSIZE);
        }
    }

    ReleaseSemaphore(&lib_poolSem);

    return cuser ? (ULONG)data : res;
}

#ifdef __MORPHOS__
ULONG LIB_LoadDataA(void)
{
    return CM_LoadDataA((struct TagItem *)REG_A0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CM_SaveDataA(struct TagItem *attrs)
#else
ULONG SAVEDS ASM CM_SaveDataA(REG(a0,struct TagItem *attrs))
#endif
{
    register struct cmio    *cmio;
    register struct CMData  *data;
    register STRPTR         fileName;
    register ULONG          res = FALSE;
    ULONG                   n;

    data     = (struct CMData *)GetTagData(CMA_SaveData_CMData,NULL,attrs);
    fileName = (STRPTR)GetTagData(CMA_SaveData_File,NULL,attrs);

    if (!data || !fileName) return FALSE;

    n = data->Version;
    if (n<CMD_MINVER) return FALSE;

    ObtainSemaphore(&lib_poolSem);

    if (cmio = AllocPooled(lib_pool,CMIOSIZE))
    {
	memset(cmio,0,sizeof(*cmio));

        if (cmio->file = open(fileName,MODE_NEWFILE))
        {
            register STRPTR user;

            user = (STRPTR)GetTagData(CMA_SaveData_User,NULL,attrs);
            cmio->codeset = (struct codeset *)GetTagData(CMA_SaveData_Codeset,NULL,attrs);

            fwrite(cmio->file,CMID,sizeof(CMID));
            fwrite(cmio->file,&n,sizeof(ULONG));

            putString(cmio,user ? user : (STRPTR)"");

            if (!cmio->codeset)
            {
                *cmio->buf1 = 0;

                userCodeset(user,cmio->buf1,sizeof(cmio->buf1));
                cmio->codeset = CodesetsFindA(cmio->buf1,NULL);
            }

            saveUsers(cmio,data->Users);
            saveWWWs(cmio,data->WWWs);
            saveFTPs(cmio,data->FTPs);
            saveChat(cmio,data->Chat);
            saveTelnet(cmio,data->Telnet);

            close(cmio->file);

            res = TRUE;
        }

        FreePooled(lib_pool,cmio,CMIOSIZE);
    }

    ReleaseSemaphore(&lib_poolSem);

    return res;
}

#ifdef __MORPHOS__
ULONG LIB_SaveDataA(void)
{
    return CM_SaveDataA((struct TagItem *)REG_A0);
}
#endif

/***********************************************************************/

#ifdef __MORPHOS__
ULONG CM_AddEntry(APTR entry)
#else
ULONG SAVEDS ASM CM_AddEntry(REG(a0,APTR entry))
#endif
{
    register ULONG          res = FALSE;
    register struct CMData  *cm;
    REGARRAY struct TagItem attrs[] = {CMA_LoadData_Current, TRUE,
                                       CMA_LoadData_Ask,     TRUE,
                                       TAG_DONE,             0,
                                       TAG_DONE,             0,
                                       TAG_DONE};

    if (cm = (struct CMData *)CM_LoadDataA(attrs))
    {
        register UBYTE          type;
        register struct CMEntry *nentry;

        type = CMENTRY(entry)->Type;

        if (nentry = CM_AllocEntry(type))
        {
            register struct CMGroup *list = NULL; //gcc
            register ULONG          size = 0; //gcc

            switch (type)
            {
                case CMEntry_User:
                    list = cm->Users;
                    size = sizeof(struct CMUser);
                    break;

                case CMEntry_WWW:
                    list = cm->WWWs;
                    size = sizeof(struct CMWWW);
                    break;

                case CMEntry_FTP:
                    list = cm->FTPs;
                    size = sizeof(struct CMFTP);
                    break;

                case CMEntry_Chat:
                    list = cm->Chat;
                    size = sizeof(struct CMChat);
                    break;

                case CMEntry_Telnet:
                    list = cm->Telnet;
                    size = sizeof(struct CMTelnet);
                    break;
            }

            copymem(nentry,entry,size);

            AddTail((struct List *)&list->Entries,(struct Node *)nentry);

            attrs[0].ti_Tag  = CMA_SaveData_CMData;
            attrs[0].ti_Data = (ULONG)cm;
            attrs[1].ti_Tag  = CMA_SaveData_File;
            attrs[1].ti_Data = (ULONG)cm->Path;
            attrs[2].ti_Tag  = CMA_SaveData_User;
            attrs[2].ti_Data = (ULONG)cm->CurrentUser;

            CM_SaveDataA(attrs);

            res = TRUE;
        }

        CM_FreeData(cm);
    }

    return res;
}

#ifdef __MORPHOS__
ULONG LIB_AddEntry(void)
{
    return CM_AddEntry((APTR)REG_A0);
}
#endif

/***********************************************************************/
