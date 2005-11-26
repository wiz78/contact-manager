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

#include "class.h"

/***********************************************************************/

static UBYTE HTMLExportHeader[] = "<HTML>\n"
                             "<HEAD><TITLE>CManager database</TITLE></HEAD>\n"
                             "<BODY>\n"
                             "<H2><P ALIGN=CENTER>CManager database</P></H2>\n"
                             "<HR>\n";

static UBYTE HTMLExportFooter[] = "<HR>\n"
                             "<P ALIGN=CENTER><FONT SIZE=-1>"
                             "Created with <A HREF=\"http://alfie.altervista.org\">Contact Manager</A>"
                             " by <A HREF=\"mailto:alforan@tin.it\">Alfonso Ranieri</A>."
                             "</FONT></P>\n"
                             "</BODY>\n"
                             "</HTML>\n";

/***********************************************************************/

struct stream
{
    BPTR file;
    int  stop;
};

#ifndef __MORPHOS__
static void
SAVEDS ASM putStringFun(REG(d0,UBYTE c),REG(a3,struct stream *s))
{
    if (c && !s->stop)
    {
        if (c>=128) FPrintf(s->file,"&#x%04lx;",(ULONG)c);
        else
            if (c=='&') FPuts(s->file,"&amp;");
            else FPutC(s->file,(ULONG)c);
    }
}

int STDARGS
putString(BPTR file,STRPTR fmt,...)
{
    struct stream s;

    s.file = file;
    s.stop = 0;

    RawDoFmt(fmt,&fmt+1,(APTR)putStringFun,&s);

    return 0;
}
#else

int putString(BPTR file,STRPTR fmt,...) __attribute((varargs68k));

static void putStringFun(void)
{
    struct stream *s = (struct stream *)REG_A3;
    UBYTE         c  = (UBYTE)REG_D0;

    if (c && !s->stop)
    {
        if (c>=128) FPrintf(s->file,"&#x%04lx;",(ULONG)c);
        else
            if (c=='&') FPuts(s->file,"&amp;");
            else FPutC(s->file,(ULONG)c);
    }
}

static struct EmulLibEntry putStringTrap = {TRAP_LIB,0,(void *)&putStringFun};

int
putString(BPTR file,STRPTR fmt,...)
{
    struct stream s;
    va_list       va;

    va_start(va,fmt);

    s.file = file;
    s.stop = 0;

    RawDoFmt(fmt,va->overflow_arg_area,(APTR)&putStringTrap,&s);

    va_end(va);

    return 0;
}
#endif

/****************************************************************************/

static void
writeCSV(BPTR file,STRPTR string,int comma,ULONG commaFlag)
{
    register STRPTR quote;

    if (commaFlag) FPutC(file,comma);

    if (quote = strchr(string,comma)) FPuts(file,"\"");

    while (string && *string)
    {
        register STRPTR  ptr;

        if (ptr = strchr(string,'\"'))
        {
            FWrite(file,string,ptr-string+1,1);
            FPuts(file,"\"");

            ptr++;

        }
        else
            if (ptr = strchr(string,'\n'))
            {
                FWrite(file,string,ptr-string+1,1);
                FPuts(file," ");

                ptr++;
            }
            else FPuts(file,string);

        string = ptr;
    }

    if (quote) FPuts(file,"\"");
}

/***********************************************************************/

void
exportWWWasHTML(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMWWW *www;
    register BPTR         file = io->file;
    register ULONG        flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,HTMLExportHeader);

    putString(file,"<B>%s</B><UL>\n",Group->Name);

    for (www = (struct CMWWW *)Group->Entries.mlh_Head; www->Succ; www = www->Succ)
    {
        if (!www->WWW[0]) continue;

        FPuts(file,"<LI><A HREF=\"");

        if (strncmp(www->WWW,"http://",7) &&
            strncmp(www->WWW,"https://",8) &&
            strncmp(www->WWW,"file://",7) &&
            strncmp(www->WWW,"ftp://",6) &&
            strncmp(www->WWW,"gopher:",7)) FPuts(file,"http://");


        putString(file,"%s\">%s</A>\n",www->WWW,www->Name[0] ? www->Name : www->WWW);
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
        FPuts(file,"<LI>");
        exportWWWasHTML(Group,io);
    }

    FPuts(file,"</UL>\n");

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,HTMLExportFooter);
}

/***********************************************************************/

void
exportURLasHTML(struct CMData *CMData,struct MUIS_CManager_IO *io)
{
    register BPTR  file = io->file;
    //register ULONG flags = io->flags;

    FPuts(file,HTMLExportHeader);

    FPuts(file,"<CENTER><H3>Web sites</H3></CENTER><P>\n");

    io->flags |= MUIV_CManager_Export_NoHead;

    exportWWWasHTML(CMData->WWWs,io);

    FPuts(file,"<CENTER><H3>FTP sites</H3></CENTER><P>\n");

    exportFTPasHTML(CMData->FTPs,io);

    FPuts(file,HTMLExportFooter);
}

/***********************************************************************/

void
exportFTPasHTML(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMFTP *ftp;
    register BPTR         file = io->file;
    register ULONG        flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,HTMLExportHeader);

    FPrintf(file,"<B>%s</B><UL>\n",(ULONG)Group->Name);

    for (ftp = (struct CMFTP *)Group->Entries.mlh_Head; ftp->Succ; ftp = ftp->Succ)
    {
        if (!ftp->FTP[0]) continue;

        FPuts(file,"<LI><A HREF=\"");

        if (strncmp(ftp->FTP,"ftp://",6)) FPuts(file,"ftp://");

        FPrintf(file,"%s\">%s</A>\n",(ULONG)ftp->FTP,(ULONG)(ftp->Name[0] ? ftp->Name : ftp->FTP));
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
         FPuts(file,"<LI>");
         exportFTPasHTML(Group,io);
    }

    FPuts(file,"</UL>\n");

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,HTMLExportFooter);
}

/***********************************************************************/

void
exportUsersAsCSV(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMUser *entry;
    register BPTR          file = io->file;
    register ULONG         flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead))
    {
        writeCSV(file,"Last Name",   ',', FALSE);
        writeCSV(file,"First Name",  ',', TRUE );
        writeCSV(file,"Address",     ',', TRUE );
        writeCSV(file,"City",        ',', TRUE );
        writeCSV(file,"Country",     ',', TRUE );
        writeCSV(file,"ZIP",         ',', TRUE );
        writeCSV(file,"Comment",     ',', TRUE );
        writeCSV(file,"Alias",       ',', TRUE );
        writeCSV(file,"Phone",       ',', TRUE );
        writeCSV(file,"Fax",         ',', TRUE );
        writeCSV(file,"Mobile",      ',', TRUE );
        writeCSV(file,"EMail",       ',', TRUE );
        writeCSV(file,"WWW",         ',', TRUE );
        writeCSV(file,"FTP",         ',', TRUE );
        writeCSV(file,"ICQ",         ',', TRUE );
        writeCSV(file,"Company",     ',', TRUE );
        writeCSV(file,"Job Title",   ',', TRUE );
        writeCSV(file,"Department",  ',', TRUE );
        writeCSV(file,"Office",      ',', TRUE );
        writeCSV(file,"Work Address",',', TRUE );
        writeCSV(file,"Work City",   ',', TRUE );
        writeCSV(file,"Work Country",',', TRUE );
        writeCSV(file,"Work ZIP",    ',', TRUE );
        writeCSV(file,"Work Phone",  ',', TRUE );
        writeCSV(file,"Work Fax",    ',', TRUE );
        writeCSV(file,"Work Mobile", ',', TRUE );
        writeCSV(file,"Work EMail",  ',', TRUE );
        FPuts(file,"\n");
    }

    for (entry = (struct CMUser *)Group->Entries.mlh_Head; entry->Succ; entry = entry->Succ)
    {
        writeCSV(file,entry->LastName,   ',', FALSE);
        writeCSV(file,entry->Name,       ',', TRUE );
        writeCSV(file,entry->Address,    ',', TRUE );
        writeCSV(file,entry->City,       ',', TRUE );
        writeCSV(file,entry->Country,    ',', TRUE );
        writeCSV(file,entry->ZIP,        ',', TRUE );
        writeCSV(file,entry->Comment,    ',', TRUE );
        writeCSV(file,entry->Alias,      ',', TRUE );
        writeCSV(file,entry->Phone,      ',', TRUE );
        writeCSV(file,entry->Fax,        ',', TRUE );
        writeCSV(file,entry->Mobile,     ',', TRUE );
        writeCSV(file,entry->EMail,      ',', TRUE );
        writeCSV(file,entry->WWW,        ',', TRUE );
        writeCSV(file,entry->FTP,        ',', TRUE );
        writeCSV(file,entry->ICQ,        ',', TRUE );
        writeCSV(file,entry->Company,    ',', TRUE );
        writeCSV(file,entry->BJobTitle,  ',', TRUE );
        writeCSV(file,entry->BDepartment,',', TRUE );
        writeCSV(file,entry->BOffice,    ',', TRUE );
        writeCSV(file,entry->BAddress,   ',', TRUE );
        writeCSV(file,entry->BCity,      ',', TRUE );
        writeCSV(file,entry->BCountry,   ',', TRUE );
        writeCSV(file,entry->BZIP,       ',', TRUE );
        writeCSV(file,entry->BPhone,     ',', TRUE );
        writeCSV(file,entry->BFax,       ',', TRUE );
        writeCSV(file,entry->BMobile,    ',', TRUE );
        writeCSV(file,entry->BEMail,     ',', TRUE );
        FPuts(file,"\n");
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
        exportUsersAsCSV(Group,io);
    }
}

/***********************************************************************/

void
exportUsersAsCSVFullOE(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMUser *entry;
    register BPTR          file = io->file;
    register ULONG         flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead))
    {
        writeCSV(file,"First Name",  ';', FALSE);
        writeCSV(file,"Last Name",   ';', TRUE );
        writeCSV(file,"Second Name", ';', TRUE );
        writeCSV(file,"Full Name",   ';', TRUE );
        writeCSV(file,"Alias",       ';', TRUE );
        writeCSV(file,"EMail",       ';', TRUE );
        writeCSV(file,"Address",     ';', TRUE );
        writeCSV(file,"City",        ';', TRUE );
        writeCSV(file,"ZIP",         ';', TRUE );
        writeCSV(file,"Zone",        ';', TRUE );
        writeCSV(file,"Country",     ';', TRUE );
        writeCSV(file,"Phone",       ';', TRUE );
        writeCSV(file,"Fax",         ';', TRUE );
        writeCSV(file,"Mobile",      ';', TRUE );
        writeCSV(file,"WWW",         ';', TRUE );
        writeCSV(file,"Work Address",';', TRUE );
        writeCSV(file,"Work City",   ';', TRUE );
        writeCSV(file,"Work ZIP",    ';', TRUE );
        writeCSV(file,"Work Zone",   ';', TRUE );
        writeCSV(file,"Work Country",';', TRUE );
        writeCSV(file,"Work Phone",  ';', TRUE );
        writeCSV(file,"Work Fax",    ';', TRUE );
        writeCSV(file,"Work Mobile", ';', TRUE );
        writeCSV(file,"Company",     ';', TRUE );
        writeCSV(file,"Job Title",   ';', TRUE );
        writeCSV(file,"Department",  ';', TRUE );
        writeCSV(file,"Office",      ';', TRUE );
        writeCSV(file,"Comment",     ';', TRUE );
        FPuts(file,"\n");
    }

    for (entry = (struct CMUser *)Group->Entries.mlh_Head; entry->Succ; entry = entry->Succ)
    {
        writeCSV(file,entry->Name,       ';', FALSE);
        writeCSV(file,entry->LastName,   ';', TRUE);
        writeCSV(file,"",                ';', TRUE);
        writeCSV(file,"",                ';', TRUE);
        writeCSV(file,entry->Alias,      ';', TRUE );
        writeCSV(file,entry->EMail,      ';', TRUE );
        writeCSV(file,entry->Address,    ';', TRUE );
        writeCSV(file,entry->City,       ';', TRUE );
        writeCSV(file,entry->ZIP,        ';', TRUE );
        writeCSV(file,"",                ';', TRUE);
        writeCSV(file,entry->Country,    ';', TRUE );
        writeCSV(file,entry->Phone,      ';', TRUE );
        writeCSV(file,entry->Fax,        ';', TRUE );
        writeCSV(file,entry->Mobile,     ';', TRUE );
        writeCSV(file,entry->WWW,        ';', TRUE );
        writeCSV(file,entry->BAddress,   ';', TRUE );
        writeCSV(file,entry->BCity,      ';', TRUE );
        writeCSV(file,entry->BZIP,       ';', TRUE );
        writeCSV(file,"",                ';', TRUE);
        writeCSV(file,entry->BCountry,   ';', TRUE );
        writeCSV(file,entry->BPhone,     ';', TRUE );
        writeCSV(file,entry->BFax,       ';', TRUE );
        writeCSV(file,entry->BMobile,    ';', TRUE );
        writeCSV(file,entry->Company,    ';', TRUE );
        writeCSV(file,entry->BJobTitle,  ';', TRUE );
        writeCSV(file,entry->BDepartment,';', TRUE );
        writeCSV(file,entry->BOffice,    ';', TRUE );
        writeCSV(file,entry->Comment,    ';', TRUE );
        FPuts(file,"\n");
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
        exportUsersAsCSV(Group,io);
    }
}

/***********************************************************************/

void
exportAWeb(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMWWW *www;
    register BPTR         file = io->file;
    register ULONG        flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,"@AWeb hotlist\n");
    else FPrintf(file,"@GROUP %s\n",(ULONG)Group->Name);

    for (www = (struct CMWWW *)Group->Entries.mlh_Head; www->Succ; www = www->Succ)
    {
        if (!www->WWW[0]) continue;

        if (strncmp(www->WWW,"http://",7) &&
            strncmp(www->WWW,"https://",8) &&
            strncmp(www->WWW,"file://",7) &&
            strncmp(www->WWW,"ftp://",6) &&
            strncmp(www->WWW,"gopher:",7)) FPuts(file,"http://");

        FPrintf(file,"%s\n%s\n",(ULONG)www->WWW,(ULONG)(www->Name[0] ? www->Name : www->WWW));
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
        exportAWeb(Group,io);
    }

    if (flags & MUIV_CManager_Export_NoHead) FPuts(file,"@ENDGROUP\n");
}

/***********************************************************************/

void
exportIBrowse(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMWWW *www;
    register BPTR         file = io->file;
    register ULONG        flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,"<!-- IBrowse Hotlist V2 -->\n\
<HTML>\n\
<HEAD><TITLE>IBrowse Hotlist</TITLE></HEAD>\n\
<BODY>\n\
<H2><P ALIGN=CENTER>IBrowse Hotlist</P></H2>\n\
<HR>\n\
<UL>\n");
    else FPrintf(file,"<LI><B>%s</B><UL>\n",(ULONG)Group->Name);

    for (www = (struct CMWWW *)Group->Entries.mlh_Head; www->Succ; www = www->Succ)
    {
        if (!www->WWW[0]) continue;

        if (*www->Alias) FPrintf(file,"<LI><A HREF=\"%s\" IBSHORTCUT=\"%s\">",(ULONG)www->WWW,(ULONG)www->Alias);
        else FPrintf(file,"<LI><A HREF=\"%s\">",(ULONG)www->WWW);

        if (strncmp(www->WWW,"http://",7) &&
            strncmp(www->WWW,"https://",8) &&
            strncmp(www->WWW,"file://",7) &&
            strncmp(www->WWW,"ftp://",6) &&
            strncmp(www->WWW,"gopher:",7)) FPuts(file,"http://");

        FPrintf(file,"%s</A>\n",(ULONG)(www->Name[0] ? www->Name : www->WWW));
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
        exportIBrowse(Group,io);
    }

    if (flags & MUIV_CManager_Export_NoHead) FPuts(file,"</UL>\n");
    else FPuts(file,"</UL>\n\
<HR>\n\
<P align=center><font size=-1>This hotlist has been created using <A HREF=\"http://alfie.altervista.org\">CManager</A>.</font></P></BODY>\n\
</HTML>\n");
}

/***********************************************************************/

void
exportYAM(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMUser *user;
    register BPTR          file = io->file;
    register ULONG         flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,"YAB4 - YAM Addressbook\n");
    else FPrintf(file,"@GROUP %s\n%s\n",(ULONG)Group->Name,(ULONG)Group->Name);

    for (user = (struct CMUser *)Group->Entries.mlh_Head; user->Succ; user = user->Succ)
    {
        ULONG d, m, y;

        FPrintf(file,"@USER %s\n%s\n%s %s\n",
(ULONG)(user->Alias[0] ? user->Alias : user->Name),
(ULONG)user->EMail,
(ULONG)user->Name,
(ULONG)user->LastName);

//kprintf("[%s]\n",user->Alias[0] ? user->Alias : user->Name);
//kprintf("[%s]\n",user->EMail);
//kprintf("[%s %s]\n",user->Name,user->LastName);

        if (user->Comment[0])
        {
            register UBYTE *c, t;

            for (c = user->Comment; *c && *c!='\n'; c++);

            t = *c;
            *c = 0;
            FPrintf(file,"%s\n",(ULONG)user->Comment);
            *c = t;
        }
        else FPuts(file,"\n");

        FPrintf(file,"%s\n",(ULONG)user->Phone);

        if (user->Address[0])
        {
            register UBYTE *c, t;

            for (c = user->Address; *c && *c!='\n'; c++);

            t = *c;
            *c = 0;
            FPrintf(file,"%s\n",(ULONG)user->Address);
            *c = t;
        }
        else FPuts(file,"\n");

        if (user->City[0] || user->ZIP[0])
            FPrintf(file,"%s/%s\n",(ULONG)user->City,(ULONG)user->ZIP);
        else FPuts(file,"\n");

        if (user->Flags & CMUSERFLG_Birthday) gregToDate(user->Birthday,&d,&m,&y);
        else d = m = y = 0;

        FPrintf(file,"%s\n%s\n%02ld%02ld%04ld\n%s\n%s\n0\n@ENDUSER\n",
(ULONG)user->Country,
(ULONG)user->PGPUserID,
d, m, y,
(ULONG)user->ImageFile,
(ULONG)user->WWW);
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
        exportYAM(Group,io);
    }

    if (flags & MUIV_CManager_Export_NoHead) FPuts(file,"@ENDGROUP\n");
}

/***********************************************************************/

static STRPTR
uft8toucs(STRPTR chr,ULONG *code)
{
    UBYTE c = *chr++;
    ULONG ucs = 0;
    int   i, bytes;

    if (!(c & 0x80))
    {
	*code = c;
	
        return chr;
    }
    else
    {
	if (!(c & 0x20))
	{
	    bytes = 2;
	    ucs = c & 0x1f;
	}
	else
            if (!(c & 0x10))
	    {
		bytes = 3;
		ucs = c & 0xf;
	    }
	    else
            	if (!(c & 0x08))
		{
		    bytes = 4;
		    ucs = c & 0x7;
		}
		else
                    if (!(c & 0x04))
		    {
			bytes = 5;
			ucs = c & 0x3;
		    }
		    else
		    {
			bytes = 6;
			ucs = c & 0x1;
		    }

	for (i = 1; i<bytes; i++)
	    ucs = (ucs<<6) | ((*chr++)&0x3f);
    }
    
    *code = ucs;
    
    return chr;
}

static void
putEl(struct codeset *codeset,BPTR file,STRPTR name,STRPTR value,ULONG nl)
{
    STRPTR utf8;
    ULONG dlen;

    if (utf8 = CodesetsUTF8Create(CODESETSA_Source,        (ULONG)value,
                           	  CODESETSA_Codeset,       (ULONG)codeset,
                           	  CODESETSA_Pool,          (ULONG)lib_pool,
                           	  CODESETSA_PoolSem,       (ULONG)&lib_sem,
	                          CODESETSA_DestLenPtr,    (ULONG)&dlen,
                                  TAG_DONE))
    {
        STRPTR src;
    	UBYTE  c;

        src = utf8;

    	FPrintf(file,"<%s>",(ULONG)name);

        while (src && (c = *src))
        {
	    if (c>=128)
	    {
	        ULONG code;

                src = uft8toucs(src,&code);
                FPrintf(file,"&#x%04lX;",code);
	        continue;
	    }
	    else
                if (c=='&') FPuts(file,"&amp;");
	        else FPutC(file,c);

	    src++;
        }

        FPrintf(file,"</%s>",(ULONG)name);
    	if (nl) FPuts(file,"\n");

        CodesetsFreeVecPooled(lib_pool,utf8,CODESETSA_PoolSem,(ULONG)&lib_poolSem,TAG_DONE);
    }
}

void exportSimpleMailGroups(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register BPTR file = io->file;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
    	FPuts(file,"<newgroup>\n");
        putEl(io->codeset,file,"name",Group->Name,FALSE);
    	FPuts(file,"</newgroup>\n");

        exportSimpleMailGroups(Group,io);
    }
}

void
exportSimpleMail(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMUser *user;
    register BPTR          file = io->file;
    register ULONG         flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead))
    {
    	FPuts(file,"<newaddressbook>\n");
	exportSimpleMailGroups(Group,io);
    }

    for (user = (struct CMUser *)Group->Entries.mlh_Head; user->Succ; user = user->Succ)
    {
	register struct CMEMail *email, *succ;

    	FPuts(file,"<newcontact>\n");

        if (*user->Alias) putEl(io->codeset,file,"alias",user->Alias,FALSE);

        if (*user->Name) putEl(io->codeset,file,"name",user->Name,FALSE);

        if (*user->EMail) putEl(io->codeset,file,"email",user->EMail,FALSE);

	for (email = (struct CMEMail *)user->EMails.mlh_Head; succ = email->Succ; email = succ)
            putEl(io->codeset,file,"email",email->EMail,FALSE);

    	if (flags & MUIV_CManager_Export_NoHead)
            putEl(io->codeset,file,"group",Group->Name,FALSE);

        if (user->Sex==CMUSERSEX_Female) putEl(io->codeset,file,"sex","female",FALSE);
        else if (user->Sex==CMUSERSEX_Male) putEl(io->codeset,file,"sex","male",FALSE);

	if (user->Flags & CMUSERFLG_Birthday)
	{
            ULONG d, m, y;
	    
            gregToDate(user->Birthday,&d,&m,&y);
            FPrintf(file,"<birthday>%ld/%ld/%ld</birthday>",m,d,y);
        }

        if (*user->ImageFile) putEl(io->codeset,file,"portrait",user->ImageFile,FALSE);

        if (*user->WWW) putEl(io->codeset,file,"homepage",user->WWW,FALSE);

        if (*user->PGPUserID) putEl(io->codeset,file,"pgpid",user->PGPUserID,FALSE);

    	FPuts(file,"<private>\n");

        if (*user->Address) putEl(io->codeset,file,"street",user->Address,FALSE);

        if (*user->City) putEl(io->codeset,file,"city",user->City,FALSE);

        if (*user->ZIP) putEl(io->codeset,file,"zip",user->ZIP,FALSE);

        if (*user->Country) putEl(io->codeset,file,"country",user->Country,FALSE);

        if (*user->Phone) putEl(io->codeset,file,"phone",user->Phone,FALSE);

        if (*user->Mobile) putEl(io->codeset,file,"mobil",user->Mobile,FALSE);

        if (*user->Fax) putEl(io->codeset,file,"fax",user->Fax,FALSE);

    	FPuts(file,"</private>\n");

    	FPuts(file,"<work>\n");

        if (*user->BAddress) putEl(io->codeset,file,"street",user->BAddress,FALSE);

        if (*user->BCity) putEl(io->codeset,file,"city",user->BCity,FALSE);

        if (*user->BZIP) putEl(io->codeset,file,"zip",user->BZIP,FALSE);

        if (*user->BCountry) putEl(io->codeset,file,"country",user->BCountry,FALSE);

        if (*user->BPhone) putEl(io->codeset,file,"phone",user->BPhone,FALSE);

        if (*user->BMobile) putEl(io->codeset,file,"mobil",user->BMobile,FALSE);

        if (*user->BFax) putEl(io->codeset,file,"fax",user->BFax,FALSE);

    	FPuts(file,"</work>\n");

    	FPuts(file,"</newcontact>\n");
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
        exportSimpleMail(Group,io);

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,"</newaddressbook>\n");
}

/***********************************************************************/

void
exportIE(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMWWW *www;
    register BPTR         file = io->file;
    register ULONG        flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,"\
<!DOCTYPE NETSCAPE-Bookmark-file-1>\n\
<!-- This is an automatically generated file.\n\
It will be read and overwritten.\n\
Do Not Edit! -->\n\
<TITLE>Bookmarks</TITLE>\n\
<H1>Bookmarks</H1>\n\
<DL><p>\n");
    else FPrintf(file,"<DT><H3 FOLDED ADD_DATE=\"0\">%s</H3>\n",(ULONG)Group->Name);

    for (www = (struct CMWWW *)Group->Entries.mlh_Head; www->Succ; www = www->Succ)
    {
        FPrintf(file,"<DT><A HREF=\"%s\" ADD_DATE=\"0\" LAST_VISIT=\"0\" LAST_MODIFIED=\"0\">",(ULONG)www->WWW);

        if (strncmp(www->WWW,"http://",7) &&
            strncmp(www->WWW,"https://",8) &&
            strncmp(www->WWW,"file://",7) &&
            strncmp(www->WWW,"ftp://",6) &&
            strncmp(www->WWW,"gopher:",7)) FPuts(file,"http://");

        FPrintf(file,"%s</A>\n",(ULONG)(www->Name[0] ? www->Name : www->WWW));
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
        exportNetscape(Group,io);
    }

    /*if (flags & MUIV_CManager_Export_NoHead) FPuts(file,"</DL><p>\n");
    else */FPuts(file,"</DL><p>\n");
}

/***********************************************************************/

void
exportNetscape(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMWWW *www;
    register BPTR         file = io->file;
    register ULONG        flags = io->flags;


    if (!(flags & MUIV_CManager_Export_NoHead)) FPuts(file,"\
<!DOCTYPE NETSCAPE-Bookmark-file-1>\n\
<!-- This is an automatically generated file.\n\
It will be read and overwritten.\n\
Do Not Edit! -->\n\
<TITLE>Bookmarks</TITLE>\n\
<H1>Bookmarks</H1>\n\
<DL><p>\n");
    else FPrintf(file,"<DT><H3 FOLDED ADD_DATE=\"0\">%s</H3>\n",(ULONG)Group->Name);

    for (www = (struct CMWWW *)Group->Entries.mlh_Head; www->Succ; www = www->Succ)
    {
        FPrintf(file,"<DT><A HREF=\"%s\" ADD_DATE=\"0\" LAST_VISIT=\"0\" LAST_MODIFIED=\"0\">",(ULONG)www->WWW);

        if (strncmp(www->WWW,"http://",7) &&
            strncmp(www->WWW,"https://",8) &&
            strncmp(www->WWW,"file://",7) &&
            strncmp(www->WWW,"ftp://",6) &&
            strncmp(www->WWW,"gopher:",7)) FPuts(file,"http://");

        FPrintf(file,"%s</A>\n",(ULONG)(www->Name[0] ? www->Name : www->WWW));
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
        exportNetscape(Group,io);
    }

    /*if (flags & MUIV_CManager_Export_NoHead) FPuts(file,"</DL><p>\n");
    else */FPuts(file,"</DL><p>\n");
}

/***********************************************************************/

void
exportATC(struct CMGroup *Group,struct MUIS_CManager_IO *io)
{
    register struct CMFTP *ftp;
    register BPTR         file = io->file;
    register ULONG        flags = io->flags;

    if (!(flags & MUIV_CManager_Export_NoHead))
    	FPuts(file,"VERSION 4\nSERVER >>CManager 1\n");

    for (ftp = (struct CMFTP *)Group->Entries.mlh_Head; ftp->Succ; ftp = ftp->Succ)
    {
        if (!ftp->FTP[0]) continue;

	FPrintf(file,"SERVER %s\t%s\t%s\t%ld\t%s\t",(ULONG)ftp->FTP,(ULONG)ftp->Username,(ULONG)ftp->Password,ftp->Port,(ULONG)ftp->Name);
	FPuts(file,"0\t5\t393\tt:\t0\t\t0\t0\t0\t0\t0\t0\t0\t5\t60\t865779183\t1\t0\t0\t0\t1\t0\t0\t1\t0\t0\t0\t1\t0\t0\t1\t27\t1\t456052263\t0\t0\n");
    }

    io->flags |= MUIV_CManager_Export_NoHead;

    for (Group = CMGROUP(Group->SubGroups.mlh_Head); Group->Succ; Group = Group->Succ)
    {
	FPrintf(file,"SERVER >> %s 1\n",(ULONG)Group->Name);
        exportATC(Group,io);
	FPuts(file,"SERVER <<\n");
    }

    if (!(flags & MUIV_CManager_Export_NoHead))
    	FPuts(file,"SERVER <<\n");
}

/***********************************************************************/

