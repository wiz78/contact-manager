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
#include <ctype.h>

/***********************************************************************/

#define _HELP(h) ((ULONG)(h)) ? MUIA_ShortHelp   : TAG_IGNORE, ((ULONG)(h)) ? CM_GetString((ULONG)(h)) : 0
#define _KEY(k)  ((ULONG)(k)) ? MUIA_ControlChar : TAG_IGNORE, ((ULONG)(k)) ? getKeyCharID((ULONG)(k)) : 0

/***********************************************************************/

#ifndef __MORPHOS__
ULONG
DoSuperNew(struct IClass *cl,Object *obj,ULONG tag1,...)
{
    return DoSuperMethod(cl,obj,OM_NEW,&tag1,NULL);
}
#endif

/***********************************************************************/

LONG
xget(Object *obj,ULONG attr)
{
    LONG res = FALSE;

    get(obj,attr,&res);

    return res;
}

/***********************************************************************/

UBYTE
getstr(Object *obj,STRPTR str)
{
    STRPTR s;

    get(obj,MUIA_String_Contents,&s);
    if (*s)
    {
        strcpy(str,s);
        return *str;
    }

    return *str = 0;
}

/***********************************************************************/

ULONG
getKeyCharID(LONG id)
{
    register STRPTR string;
    register ULONG  res = 0;

    string = CM_GetString(id);

    for (; *string && *string!='_'; string++);
    if (*string++) res = ToLower(*string);

    return res;
}

/***********************************************************************/

ULONG
getKeyChar(STRPTR string)
{
    register ULONG res = 0;

    for (; *string && *string!='_'; string++);
    if (*string++) res = ToLower(*string);

    return res;
}

/***********************************************************************/

Object *
ostring(ULONG maxlen,ULONG key,ULONG help)
{
    return RootStringObject,
        _HELP(help),
        _KEY(key),
        MUIA_CycleChain,            TRUE,
        StringFrame,
        MUIA_Textinput_AdvanceOnCR, TRUE,
        MUIA_Textinput_MaxLen,      maxlen,
    End;
}

/***********************************************************************/

Object *
oqstring(Object *list,ULONG maxlen,ULONG key,ULONG help)
{
    return QStringObject,
        _HELP(help),
        _KEY(key),
        MUIA_CycleChain,             TRUE,
        StringFrame,
        MUIA_QString_AttachedList,   list,
        MUIA_Textinput_RemainActive, TRUE,
        MUIA_Textinput_MaxLen,       maxlen,
    End;
}

/***********************************************************************/

Object *
ofile(Object **obj,ULONG size,ULONG txt,ULONG help,ULONG drawersOnly)
{
    register Object *o, *bt;

    o = PopaObject,
        _HELP(help),
        MUIA_Popstring_String, *obj = ostring(size,txt,0),
        MUIA_Popstring_Button, bt = PopButton(drawersOnly ? MUII_PopDrawer : MUII_PopFile),
        MUIA_Popasl_Type,      ASL_FileRequest,
        ASLFR_DrawersOnly,     drawersOnly,
    End;

    if (o) set(bt,MUIA_CycleChain,TRUE);

    return o;
}
/***********************************************************************/

Object *
ocheck(ULONG key,ULONG help)
{
    register Object *obj;

    if (obj = MUI_MakeObject(MUIO_Checkmark,CM_GetString(key)))
        SetAttrs(obj,MUIA_CycleChain,TRUE,_HELP(help),TAG_DONE);

    return obj;
}

/***********************************************************************/

Object *
obutton(ULONG text,ULONG help)
{
    register Object *obj;

    if (obj = MUI_MakeObject(MUIO_Button,CM_GetString(text)))
        SetAttrs(obj,MUIA_CycleChain,TRUE,_HELP(help),TAG_DONE);

    return obj;
}

/***********************************************************************/

Object *
osstring(ULONG maxlen,ULONG key,ULONG help)
{
    return RootStringObject,
        _HELP(help),
        _KEY(key),
        MUIA_CycleChain,            TRUE,
        StringFrame,
        MUIA_Textinput_Secret,      TRUE,
        MUIA_Textinput_AdvanceOnCR, TRUE,
        MUIA_Textinput_MaxLen,      maxlen,
    End;
}

/***********************************************************************/

Object *
oqsstring(Object *list,ULONG maxlen,ULONG key,ULONG help)
{
    return QStringObject,
        _HELP(help),
        _KEY(key),
        MUIA_CycleChain,             TRUE,
        StringFrame,
        MUIA_QString_AttachedList,   list,
        MUIA_Textinput_Secret,       TRUE,
        MUIA_Textinput_RemainActive, TRUE,
        MUIA_Textinput_MaxLen,       maxlen,
    End;
}

/***********************************************************************/

Object *
ointeger(ULONG maxlen,ULONG key,ULONG help)
{
    return RootStringObject,
        _HELP(help),
        _KEY(key),
        MUIA_CycleChain,            TRUE,
        StringFrame,
        MUIA_Textinput_AdvanceOnCR, TRUE,
        MUIA_Textinput_MaxLen,      maxlen,
        MUIA_Textinput_AcceptChars, "0123456789",
        MUIA_Textinput_Format,      MUIV_Textinput_Format_Right,
    End;
}

/***********************************************************************/

Object *
oqinteger(Object *list,ULONG maxlen,ULONG key,ULONG help)
{
    return QStringObject,
        _HELP(help),
        _KEY(key),
        StringFrame,
        MUIA_CycleChain,             TRUE,
        MUIA_QString_AttachedList,   list,
        MUIA_Textinput_RemainActive, TRUE,
        MUIA_Textinput_MaxLen,       maxlen,
        MUIA_Textinput_AcceptChars,  "0123456789",
        MUIA_Textinput_Format,       MUIV_Textinput_Format_Right,
    End;
}

/***********************************************************************/

Object *
ocycle(STRPTR *array,ULONG key,ULONG help)
{
    register Object *obj;

    if (obj = MUI_MakeObject(MUIO_Cycle,CM_GetString(key),array))
        SetAttrs(obj,MUIA_CycleChain,TRUE,_HELP(help),TAG_DONE);

    return obj;
}

/***********************************************************************/

Object *
oslider(ULONG key,ULONG help,LONG min,LONG max)
{
    register Object *obj;

    if (obj = MUI_MakeObject(MUIO_Slider,CM_GetString(key),min,max))
        SetAttrs(obj,MUIA_CycleChain,TRUE,_HELP(help),TAG_DONE);

    return obj;
}

/***********************************************************************/

Object *
olabel(ULONG id)
{
    return Label(CM_GetString(id));
}

/****************************************************************************/

Object *
ollabel(ULONG id)
{
    return LLabel(CM_GetString(id));
}

/***********************************************************************/

Object *
olabel1(ULONG id)
{
    return Label1(CM_GetString(id));
}

/***********************************************************************/

Object *
ollabel1(ULONG id)
{
    return LLabel1(CM_GetString(id));
}

/***********************************************************************/

Object *
olabel2(ULONG id)
{
    return Label2(CM_GetString(id));
}

/***********************************************************************/

Object *
ofreelabel2(ULONG text)
{
    return FreeLabel2(CM_GetString(text));
}

/***********************************************************************/

Object *
ourltext(STRPTR url,STRPTR text)
{
    return UrltextObject,
        MUIA_Urltext_Text,           text,
        MUIA_Urltext_Url,            url,
        MUIA_Urltext_SetMax,         FALSE,
        MUIA_Urltext_NoOpenURLPrefs, TRUE,
    End;
}

/***********************************************************************/

Object *
oibutton(ULONG spec,ULONG help)
{
    if (spec==IBT_Up) spec = (ULONG)"\33I[6:38]";
    else if (spec==IBT_Down) spec = (ULONG)"\33I[6:39]";
         else return NULL;

    return TextObject,
        _HELP(help),
        MUIA_CycleChain,    TRUE,
        MUIA_Font,          MUIV_Font_Button,
        MUIA_InputMode,     MUIV_InputMode_RelVerify,
        ButtonFrame,
        MUIA_Background,    MUII_ButtonBack,
        MUIA_Text_Contents, spec,
        MUIA_Text_PreParse, MUIX_C,
        MUIA_Text_SetMax,   TRUE,
    End;
}

/***********************************************************************/

Object *
osscroll(ULONG len,ULONG key,ULONG help)
{
    return RootScrollObject,
        _HELP(help),
        _KEY(key),
        StringFrame,
        MUIA_CycleChain,                       TRUE,
        MUIA_Textinput_Multiline,              TRUE,
        MUIA_Textinput_AutoExpand,             FALSE,
        MUIA_Textinput_MaxLen,                 len,
        MUIA_Textinputscroll_VertScrollerOnly, TRUE,
    End;
}

/***********************************************************************/

APTR
allocVecPooled(APTR pool,ULONG size)
{
    register ULONG *mem;

    if (mem = AllocPooled(pool,size = size+sizeof(ULONG))) *mem++ = size;

    return mem;
}

/****************************************************************************/

void
freeVecPooled(APTR pool,APTR mem)
{
    FreePooled(pool,(LONG *)mem-1,*((LONG *)mem-1));
}

/****************************************************************************/

APTR
allocArbitratePooled(ULONG size)
{
    register APTR mem;

    ObtainSemaphore(&lib_poolSem);
    mem = AllocPooled(lib_pool,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

APTR
freeArbitratePooled(APTR mem,ULONG size)
{
    ObtainSemaphore(&lib_poolSem);
    FreePooled(lib_pool,mem,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

APTR
allocArbitrateVecPooled(ULONG size)
{
    register APTR mem;

    ObtainSemaphore(&lib_poolSem);
    mem = allocVecPooled(lib_pool,size);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

APTR
freeArbitrateVecPooled(APTR mem)
{
    ObtainSemaphore(&lib_poolSem);
    freeVecPooled(lib_pool,mem);
    ReleaseSemaphore(&lib_poolSem);

    return mem;
}

/***********************************************************************/

APTR
reallocArbitrateVecPooled(APTR mem,ULONG size)
{
    register APTR  new;
    register ULONG sold = 0; //gcc

    if (size==0) return NULL;

    if (mem)
    {
        sold = *((ULONG *)mem-1);

        if (sold-sizeof(ULONG)>=size) return mem;
    }

    if (new = allocArbitrateVecPooled(size))
    {
        if (mem) copymem(new,mem,sold);
    }

    if (mem) freeArbitrateVecPooled(mem);

    return new;
}

/***********************************************************************/
ULONG
getDir(Object *obj,STRPTR Title,STRPTR Path,ULONG SaveMode)
{
    register struct FileRequester *Req;
    register ULONG                ok = FALSE;

    if (Req = MUI_AllocAslRequest(ASL_FileRequest,NULL))
    {
        APTR scr = NULL, Win = NULL;

        if (obj)
        {
            get(obj,MUIA_WindowObject,&Win);
            if (Win) get(Win,MUIA_Window_Screen,&scr);
        }

        if (MUI_AslRequestTags(Req,
                ASLFR_DrawersOnly,                          TRUE,
                ASLFR_DoSaveMode,                           SaveMode,
                Title[0] ? ASLFR_TitleText : TAG_IGNORE,    Title,
                Path[0] ? ASLFR_InitialDrawer : TAG_IGNORE, Path,
                scr ? ASLFR_Screen : TAG_IGNORE,            scr,
                TAG_DONE))
        {
            strcpy(Path,Req->fr_Drawer);

            ok = TRUE;
        }

        MUI_FreeAslRequest(Req);
    }

    return ok;
}

/***************************************************************************/

ULONG
openWindow(Object *app,Object *win)
{
    ULONG v;

    if (win)
    {
        set(win,MUIA_Window_Open,TRUE);
        get(win,MUIA_Window_Open,&v);
        if (!v) get(app,MUIA_Application_Iconified,&v);
    }
    else v = FALSE;

    if (!v) DisplayBeep(0);

    return v;
}

/***********************************************************************/

void
sleepApp(Object *obj,ULONG sleep)
{
    Object *app;

    get(obj,MUIA_ApplicationObject,&app);
    if (app) set(app,MUIA_Application_Sleep,sleep);
}

/***************************************************************************/

struct stream
{
    UBYTE   *buf;
    int     size;
    int     counter;
    int     stop;
};

#ifndef __MORPHOS__
UWORD fmtfunc[] = { 0x16c0, 0x4e75 };

void STDARGS
sprintf(STRPTR to,STRPTR fmt,...)
{
    RawDoFmt(fmt,&fmt+1,(APTR)fmtfunc,to);
}

void
vsprintf(STRPTR to,STRPTR fmt,APTR args)
{
    RawDoFmt(fmt,args,(APTR)fmtfunc,to);
}

static void ASM
snprintfStuff(REG(d0,UBYTE c),REG(a3,struct stream *s))
{
    if (!s->stop)
    {
        if (++s->counter>=s->size)
        {
            *(s->buf) = 0;
            s->stop   = 1;
        }
        else *(s->buf++) = c;
    }
}

int STDARGS
snprintf(STRPTR buf,int size,STRPTR fmt,...)
{
    struct stream s;

    s.buf     = buf;
    s.size    = size;
    s.counter = 0;
    s.stop    = 0;

    RawDoFmt(fmt,&fmt+1,(APTR)snprintfStuff,&s);

    return s.counter-1;
}
#else

void
sprintf(STRPTR buf,STRPTR fmt,...)
{
    va_list va;

    va_start(va,fmt);
    VNewRawDoFmt(fmt,0,buf,va);
    va_end(va);
}

static void
snprintfStuff(void)
{
    register struct stream *s = (struct stream *)REG_A3;
    register UBYTE         c  = (UBYTE)REG_D0;

    if (!s->stop)
    {
        if (++s->counter>=s->size)
        {
            *(s->buf) = 0;
            s->stop   = 1;
        }
        else *(s->buf++) = c;
    }
}

static struct EmulLibEntry snprintfStuffTrap = {TRAP_LIB,0,(void *)&snprintfStuff};

int
snprintf(STRPTR buf,int size,STRPTR fmt,...)
{
    struct stream s;
    va_list       va;

    va_start(va,fmt);

    s.buf     = buf;
    s.size    = size;
    s.counter = 0;
    s.stop    = 0;

    RawDoFmt(fmt,va->overflow_arg_area,(APTR)&snprintfStuffTrap,&s);

    va_end(va);

    return s.counter-1;
}

static void
vsprintfStuff(void)
{
    register STRPTR s = (STRPTR)REG_A3;
    register UBYTE  c = (UBYTE)REG_D0;

    *s++ = c;
}

static struct EmulLibEntry vsprintfStuffTrap = {TRAP_LIB,0,(void *)&vsprintfStuff};

void
vsprintf(STRPTR to,STRPTR fmt,APTR args)
{
    RawDoFmt(fmt,args,(APTR)&vsprintfStuffTrap,to);
}

#endif

/****************************************************************************/

STRPTR
expandPlaceHolders(STRPTR template,
                   struct placeHolder *ph,
                   STRPTR buf,
                   ULONG bufSize,
                   int num)
{
    register STRPTR p, result;
    register int    i, length = 0;

    /* calculate length of result string */
    for (p = template; *p; p++)
    {
        for (i = 0; i<num; i++)
            if ((*p=='%') && (*(p+1)==ph[i].flag)) length += strlen(ph[i].value);

        length++;
    }

    /* allocate result string */
    if (!buf || (length+1>bufSize))
    {
        if (!(result = allocArbitrateVecPooled(length+1))) return NULL;
    }
    else result = buf;

    /* perform substitution */
    for (p = result; *template; template++)
    {
        for (i = 0; i<num; i++)
            if ((*template=='%') && (*(template+1)== ph[i].flag)) break;

        if (i<num)
        {
            strcpy(p,ph[i].value);
            p += strlen(ph[i].value);
            template++;
            continue;
        }

        *p++ = *template;
    }

    *p = '\0';

    return result;
}

/**************************************************************************/

ULONG
editURLFun(Object *URL,ULONG type)
{
    REGARRAY UBYTE buf1[256], buf2[256];
    register ULONG change = FALSE;

    getstr(URL,buf1);

    if (buf1[0] == '\0') return 0;

    switch (type)
    {
        case URL_WWW:
            if (strncmp(buf1,"http://",7) && strncmp(buf1,"https://",8))
            {
                sprintf(buf2,"http://%s",buf1);
                change = TRUE;
            }
            break;

        case URL_FTP:
            if (strncmp(buf1,"ftp://",6))
            {
                sprintf(buf2,"ftp://%s",buf1);
                change = TRUE;
            }
            break;
    }

    if (change)
    {
        strcpy(buf1,buf2);
        set(URL,MUIA_Textinput_Contents,buf1);
    }

    return 0;
}

/***********************************************************************/
ULONG
editGoURLFun(STRPTR url)
{
    if (OpenURLBase)
    {
        URL_OpenA(url,NULL);

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

ULONG dateToGreg(ULONG day,ULONG month,ULONG year)
{
    register ULONG a, y, m;

    a = (14-month)/12;
    y = year+4800-a;
    m = month + 12*a - 3;

    return day + (153*m+2)/5 + y*365 + y/4 - y/100 + y/400 - 32045;
}

/***********************************************************************/

ULONG
gregToDate(ULONG greg,ULONG *day,ULONG *month,ULONG *year)
{
    register ULONG a, b, c, d, e, m;

    a = greg+32044;
    b = (4*a+3)/146097;
    c = a-(b*146097)/4;

    d = (4*c+3)/1461;
    e = c - (1461*d)/4;
    m = (5*e+2)/153;

    *day   = e - (153*m+2)/5 + 1;
    *month = m + 3 - 12*(m/10);
    *year  = b*100 + d - 4800 + m/10;

    return 0;
}

/***********************************************************************/

ULONG
dateWeekday(ULONG day,ULONG month,ULONG year)
{
    register ULONG a, y, m;

    a = (14 - month) / 12;
    y = year - a;
    m = month + 12*a - 2;

    return ((day + y + y/4 - y/100 + y/400 + (31*m)/12)) % 7;
}

ULONG
gregWeekday(ULONG greg)
{
    ULONG day, month, year;

    gregToDate(greg,&day,&month,&year);

    return dateWeekday(day,month,year);
}

/***********************************************************************/

static void
putChar(STRPTR buf,UBYTE c,int *size)
{
    if (*size>0)
    {
        *buf = c;
        *size = *size-1;
    }
}

static STRPTR
mystrcat(STRPTR a,STRPTR b,int *size)
{
    while (*b) putChar(a++,*b++,size);

    return a;
}

STRPTR
formatDate(STRPTR buf,int size,STRPTR fmt,ULONG greg)
{
    register UBYTE *b, *f;
    register ULONG sp, wd;
    ULONG          d, m, y;
    int            s;

    gregToDate(greg,&d,&m,&y);
    wd = gregWeekday(greg);

    if (size<=0) return buf;
    s = size-1;
    sp = 0;

    for (b = buf, f = fmt; *f && s; f++)
    {
        if (sp)
        {
            switch (*f)
            {
                case 'a':
                {
                    STRPTR c;

                    c = GetLocaleStr(lib_locale,ABDAY_1+wd);
                    b = mystrcat(b,c,&s);
                    break;
                }

                case 'A':
                {
                    STRPTR c;

                    c = GetLocaleStr(lib_locale,DAY_1+wd);
                    b = mystrcat(b,c,&s);
                    break;
                }

                case 'b': case 'h':
                {
                    STRPTR c;

                    c = GetLocaleStr(lib_locale,ABMON_1+m-1);
                    b = mystrcat(b,c,&s);
                    break;
                }

                case 'B':
                {
                    STRPTR c;

                    c = GetLocaleStr(lib_locale,MON_1+m-1);
                    b = mystrcat(b,c,&s);
                    break;
                }

                case 'd':
                    if (d<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }
                    break;

                case 'e':
                    if (d<10)
                    {
                        putChar(b++,' ',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }
                    break;

                case 'm':
                    if (m<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }
                    break;

                case 'M':
                    if (m<10)
                    {
                        putChar(b++,' ',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }
                    break;

                case 'U':
                    putChar(b++,48+wd,&s);
                    break;

                case 'y': case 'Y':
                {
                    UBYTE yb[16];

                    sprintf(yb,"%ld",y);
                    b = mystrcat(b,yb,&s);
                    break;
                }

                case 'g':
                {
                    UBYTE gb[16];

                    sprintf(gb,"%ld",greg);
                    b = mystrcat(b,gb,&s);
                    break;
                }

                case 'I':
                {
                    UBYTE yb[16];

                    sprintf(yb,"%ld",y);
                    b = mystrcat(b,yb,&s);

                    if (m<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }

                    if (d<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }

                    break;
                }

                case 'x':
                {
                    UBYTE yb[16];

                    if (m<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }

                    putChar(b++,'/',&s);

                    if (d<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }

                    putChar(b++,'/',&s);

                    sprintf(yb,"%ld",y);
                    b = mystrcat(b,yb,&s);

                    break;
                }

                case 'E':
                {
                    UBYTE yb[16];

                    if (d<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+d,&s);
                    }
                    else
                    {
                        putChar(b++,48+d/10,&s);
                        putChar(b++,48+d-d/10*10,&s);
                    }

                    putChar(b++,'/',&s);

                    if (m<10)
                    {
                        putChar(b++,'0',&s);
                        putChar(b++,48+m,&s);
                    }
                    else
                    {
                        putChar(b++,48+m/10,&s);
                        putChar(b++,48+m-m/10*10,&s);
                    }

                    putChar(b++,'/',&s);

                    sprintf(yb,"%ld",y);
                    b = mystrcat(b,yb,&s);

                    break;
                }

                case '%':
                    putChar(b++,'%',&s);
                    break;

                case 'n':
                    putChar(b++,'\n',&s);
                    break;

                case 't':
                    putChar(b++,'\t',&s);
                    break;
            }

            sp = 0;

            continue;
        }

        if (*f=='%') sp = 1;
        else putChar(b++,*f,&s);
    }

    *(buf+size-s-1) = 0;

    return buf+size-s-1;
}

/***********************************************************************/

#define TODAYOFS 2443510

ULONG
getToday(void)
{
    struct DateStamp ds;

    DateStamp(&ds);
    return (ULONG)(TODAYOFS+ds.ds_Days);
}

/***********************************************************************/

ULONG
checkBirthday(ULONG bgreg)
{
    ULONG bd, bm, by, td, tm, ty;

    gregToDate(bgreg,&bd,&bm,&by);
    gregToDate(getToday(),&td,&tm,&ty);

    return (ULONG)(bd==td && bm==tm);
}

/***********************************************************************/

ULONG
createUserDir(STRPTR user)
{
    REGARRAY UBYTE buf[256];
    register BPTR  lock = NULL;

    strcpy(buf,lib_CMPath);

    if (AddPart(buf,user,sizeof(buf)))
    {
        lock = Lock(buf,SHARED_LOCK);
        if (!lock) lock = CreateDir(buf);
        if (lock) UnLock(lock);
    }

    return (ULONG)lock;
}

/***************************************************************************/

struct chardes
{
    STRPTR name;
    int    len;
    ULONG  ch;
};

static struct chardes chars[] =
{
  "AElig",      sizeof("AElig"),    198,
  "Aacute",     sizeof("Aacute"),   193,
  "Acirc",      sizeof("Acirc"),    194,
  "Agrave",     sizeof("Agrave"),   192,
  "Aring",      sizeof("Aring"),    197,
  "Atilde",     sizeof("Atilde"),   195,
  "Auml",       sizeof("Auml"),     196,
  "Ccedil",     sizeof("Ccedil"),   199,
  "Dagger",     sizeof("Dagger"),   8225,
  "ETH",        sizeof("ETH"),      208,
  "Eacute",     sizeof("Eacute"),   201,
  "Ecirc",      sizeof("Ecirc"),    202,
  "Egrave",     sizeof("Egrave"),   200,
  "Euml",       sizeof("Euml"),     203,
  "Iacute",     sizeof("Iacute"),   205,
  "Icirc",      sizeof("Icirc"),    206,
  "Igrave",     sizeof("Igrave"),   204,
  "Iuml",       sizeof("Iuml"),     207,
  "Ntilde",     sizeof("Ntilde"),   209,
  "OElig",      sizeof("OElig"),    338,
  "Oacute",     sizeof("Oacute"),   211,
  "Ocirc",      sizeof("Ocirc"),    212,
  "Ograve",     sizeof("Ograve"),   210,
  "Oslash",     sizeof("Oslash"),   216,
  "Otilde",     sizeof("Otilde"),   213,
  "Ouml",       sizeof("Ouml"),     214,
  "Prime",      sizeof("Prime"),    8243,
  "Scaron",     sizeof("Scaron"),   352,
  "THORN",      sizeof("THORN"),    222,
  "Uacute",     sizeof("Uacute"),   218,
  "Ucirc",      sizeof("Ucirc"),    219,
  "Ugrave",     sizeof("Ugrave"),   217,
  "Uuml",       sizeof("Uuml"),     220,
  "Yacute",     sizeof("Yacute"),   221,
  "Yuml",       sizeof("Yuml"),     376,
  "aacute",     sizeof("aacute"),   225,
  "acirc",      sizeof("acirc"),    226,
  "acute",      sizeof("acute"),    180,
  "aelig",      sizeof("aelig"),    230,
  "agrave",     sizeof("agrave"),   224,
  "amp",        sizeof("amp"),      38,
  "aring",      sizeof("aring"),    229,
  "atilde",     sizeof("atilde"),   227,
  "auml",       sizeof("auml"),     228,
  "bdquo",      sizeof("bdquo"),    8222,
  "brvbar",     sizeof("brvbar"),   166,
  "bull",       sizeof("bull"),     8226,
  "ccedil",     sizeof("ccedil"),   231,
  "cedil",      sizeof("cedil"),    184,
  "cent",       sizeof("cent"),     162,
  "circ",       sizeof("circ"),     710,
  "copy",       sizeof("copy"),     169,
  "curren",     sizeof("curren"),   164,
  "dagger",     sizeof("dagger"),   8224,
  "deg",        sizeof("deg"),      176,
  "divide",     sizeof("divide"),   247,
  "eacute",     sizeof("eacute"),   233,
  "ecirc",      sizeof("ecirc"),    234,
  "egrave",     sizeof("egrave"),   232,
  "empty",      sizeof("empty"),    8709,
  "emsp",       sizeof("emsp"),     8195,
  "ensp",       sizeof("ensp"),     8194,
  "eth",        sizeof("eth"),      240,
  "euml",       sizeof("euml"),     235,
  "fnof",       sizeof("fnof"),     402,
  "frac12",     sizeof("frac12"),   189,
  "frac14",     sizeof("frac14"),   188,
  "frac34",     sizeof("frac34"),   190,
  "frasl",      sizeof("frasl"),    8260,
  "ge",         sizeof("ge"),       8805,
  "gt",         sizeof("gt"),       62,
  "hellip",     sizeof("hellip"),   8230,
  "iacute",     sizeof("iacute"),   237,
  "icirc",      sizeof("icirc"),    238,
  "iexcl",      sizeof("iexcl"),    161,
  "igrave",     sizeof("igrave"),   236,
  "iquest",     sizeof("iquest"),   191,
  "iuml",       sizeof("iuml"),     239,
  "lang",       sizeof("lang"),     9001,
  "laquo",      sizeof("laquo"),    171,
  "ldquo",      sizeof("ldquo"),    8220,
  "le",         sizeof("le"),       8804,
  "lowast",     sizeof("lowast"),   8727,
  "loz",        sizeof("loz"),      9674,
  "lsaquo",     sizeof("lsaquo"),   8249,
  "lsquo",      sizeof("lsquo"),    8216,
  "lt",         sizeof("lt"),       60,
  "macr",       sizeof("macr"),     175,
  "mdash",      sizeof("mdash"),    8212,
  "micro",      sizeof("micro"),    181,
  "middot",     sizeof("middot"),   183,
  "minus",      sizeof("minus"),    8722,
  "nbsp",       sizeof("nbsp"),     160,
  "ndash",      sizeof("ndash"),    8211,
  "not",        sizeof("not"),      172,
  "ntilde",     sizeof("ntilde"),   241,
  "oacute",     sizeof("oacute"),   243,
  "ocirc",      sizeof("ocirc"),    244,
  "oelig",      sizeof("oelig"),    339,
  "ograve",     sizeof("ograve"),   242,
  "oline",      sizeof("oline"),    8254,
  "ordf",       sizeof("ordf"),     170,
  "ordm",       sizeof("ordm"),     186,
  "oslash",     sizeof("oslash"),   248,
  "otilde",     sizeof("otilde"),   245,
  "ouml",       sizeof("ouml"),     246,
  "para",       sizeof("para"),     182,
  "permil",     sizeof("permil"),   8240,
  "plusmn",     sizeof("plusmn"),   177,
  "pound",      sizeof("pound"),    163,
  "prime",      sizeof("prime"),    8242,
  "quot",       sizeof("quot"),     34,
  "rang",       sizeof("rang"),     9002,
  "raquo",      sizeof("raquo"),    187,
  "reg",        sizeof("reg"),      174,
  "rdquo",      sizeof("rdquo"),    8221,
  "rsaquo",     sizeof("rsaquo"),   8250,
  "rsquo",      sizeof("rsquo"),    8217,
  "sbquo",      sizeof("sbquo"),    8218,
  "scaron",     sizeof("scaron"),   353,
  "sdot",       sizeof("sdot"),     8901,
  "sect",       sizeof("sect"),     167,
  "shy",        sizeof("shy"),      173,
  "sim",        sizeof("sim"),      8764,
  "sup1",       sizeof("sup1"),     185,
  "sup2",       sizeof("sup2"),     178,
  "sup3",       sizeof("sup3"),     179,
  "szlig",      sizeof("szlig"),    223,
  "thinsp",     sizeof("thinsp"),   8201,
  "thorn",      sizeof("thorn"),    254,
  "tilde",      sizeof("tilde"),    732,
  "times",      sizeof("times"),    215,
  "trade",      sizeof("trade"),    8482,
  "uacute",     sizeof("uacute"),   250,
  "ucirc",      sizeof("ucirc"),    251,
  "ugrave",     sizeof("ugrave"),   249,
  "uml",        sizeof("uml"),      168,
  "uuml",       sizeof("uuml"),     252,
  "yacute",     sizeof("yacute"),   253,
  "yen",        sizeof("yen"),      165,
  "yuml",       sizeof("yuml"),     255,
  "zwj",        sizeof("zwj"),      8205,
  "zwnj",       sizeof("zwnj"),     8204,
};

#define NRCHARS (sizeof(chars)/sizeof(struct chardes))

static struct chardes *
findChar(UBYTE *name)
{
    register int a = 0, b = NRCHARS-1, m, c;

    while (a<=b)
    {
        m = (a+b)/2;
        c = strncmp(chars[m].name,name,chars[m].len-1);

        if (c == 0) return &chars[m];

        if (c<0) a = m+1;
        else b = m-1;
    }

    return NULL;
}

ULONG
etranslate(STRPTR buffer,STRPTR to,int len)
{
    REGARRAY UBYTE ebuf[12];
    register UBYTE *p = buffer, *q;
    register ULONG res = FALSE, n;

    len--;

    while (*p)
    {
        //n = *p;

        if (*p=='&')
        {
            q = p;
            if (!*(++q)) break;

            if (*q=='#')
            {
                n = 0;

                if (isdigit(q[1]))
                {
                    while (*(++q) && isdigit(*q)) n = 10*n+(*q-'0');
                }
                else
                    if (toupper(q[1])=='X')
                    {
                        q++;

                        while (*(++q) && isxdigit(*q))
                        {
                            n = 16*n+((*q<='9') ? (*q-'0') : (toupper(*q)-'A'+10));
                        }
                    }

                /* Blindly replace the entity by the character value.
                 * Validity check and translation is done below.
                 * First remember the exact source in case we don't know the character.
                */

                if (*q)
                {
                    if (*q!=';') q--;

                    if (n)
                    {
                        register int i;

                        for (i = 0; i<11; )
                        {
                            ebuf[i++] = n & 0xff;
                            n >>= 8;
                            if (n==0) break;
                        }

                        while (i--)
                        {
                            if (--len==0) goto end;
                            *to++ = ebuf[i];
                        }
                    }
                }
                else q--;
            }
            else
            {
                UBYTE           name[8];
                struct chardes  *cd;
                short           i;

                for (i=0; *q && i<7 && (isalnum(*q) || *q=='.' || *q=='-'); q++, i++) name[i] = *q;
                name[i] = '\0';

                if (cd = findChar(name))
                {
                    q = p+cd->len;
                    if (*q)
                    {
                        if (*q!=';') q--;
                    }
                    else q--;

                    n = cd->ch;
                    if (n>255)
                    {
                        if (--len==0) goto end;
                        *to++ = (n & 0xff)>>8;
                    }

                    if (--len==0) goto end;
                    *to++ = n & 0xff;
                }
            }

            p = q+1;
        }
        else
        {
            if (--len==0) goto end;
            *to++ = *p++;
        }

        res = TRUE;
    }

end:
    *to = 0;
    return res;
}

/***********************************************************************/

static UBYTE rfc1738_unsafe_chars[] =
{
    0x3C,        /* < */
    0x3E,        /* > */
    0x22,        /* " */
    0x23,        /* # */
    0x25,        /* % */
    0x7B,        /* { */
    0x7D,        /* } */
    0x7C,        /* | */
    0x5C,        /* \ */
    0x5E,        /* ^ */
    0x7E,        /* ~ */
    0x5B,        /* [ */
    0x5D,        /* ] */
    0x60,        /* ` */
    0x27,        /* ' */
    0x20         /* space */
};

static ULONG
rfc1738_escape(STRPTR from,STRPTR to,ULONG toLen)
{
    register STRPTR p, q;
    register int    i, do_escape;

    if (toLen<strlen(from)*3+1) return FALSE;

    for (p = from, q = to; *p!='\0'; p++, q++)
    {
        do_escape = 0;

        /* RFC 1738 defines these chars as unsafe */
        for (i = 0; i<sizeof(rfc1738_unsafe_chars); i++)
        {
            if (*p==rfc1738_unsafe_chars[i])
            {
                do_escape = 1;
                break;
            }
        }

        /* RFC 1738 says any control chars (0x00-0x1F) are encoded */
        if ((unsigned char)*p<=(unsigned char)0x1F)
        {
            do_escape = 1;
        }

        /* RFC 1738 says 0x7f is encoded */
        if (*p==0x7F)
        {
            do_escape = 1;
        }

        /* RFC 1738 says any non-US-ASCII are encoded */
        if ((*p>=0x80) && (*p<=0xFF))
        {
            do_escape = 1;
        }

        /* Do the triplet encoding, or just copy the char */
        /* note: we do not need snprintf here as q is appropriately
         * allocated - KA */

        if (do_escape == 1)
        {
            (void)sprintf(q,"%%%02lx",(ULONG)*p);
            q += sizeof(UBYTE)*2;
        }
        else
        {
            *q = *p;
        }
    }

    *q = '\0';

    return TRUE;
}

/***********************************************************************/

ULONG
getMap(STRPTR address,STRPTR country,STRPTR city,STRPTR zip)
{
    if (OpenURLBase)
    {
        REGARRAY UBYTE addr[256], buf[256], url[512];
        register UBYTE *c;

        stccpy(addr,address,sizeof(addr));
        for (c = addr; *c; c++)
        {
            if (*c=='\n' || *c=='\r')
            {
                *c = 0;
                break;
            }
        }

/*
http://www.mapquest.com/maps/map.adp?
country=Italia
&address=via%20Ponte%20Nuovo%2C%2013%20%20
&city=Castel%20di%20Sangro%20(AQ)
&state=&zipcode=67031 */

        snprintf(buf,sizeof(buf),"http://www.mapquest.com/maps/map.adp?country=%s&address=%s&city=%s&state=&zipcode=%s",
            country,
            addr,
            city,
            zip);

        if (rfc1738_escape(buf,url,sizeof(url)))
        {
            URL_OpenA(url,NULL);

            return TRUE;
        }
    }

    return FALSE;
}

/***********************************************************************/

Object *
oflabel(ULONG id)
{
    return FreeLabel(CM_GetString(id));
}

/***********************************************************************/

Object *
ofont(Object **obj,ULONG size,ULONG txt,ULONG help)
{
    register Object *o, *bt;

    o = PopaObject,
        _HELP(help),
        MUIA_Popstring_String, *obj = ostring(size,txt,0),
        MUIA_Popstring_Button, bt = PopButton(MUII_PopUp),
        MUIA_Popasl_Type,      ASL_FontRequest,
    End;

    if (o) set(bt,MUIA_CycleChain,TRUE);

    return o;
}
/***********************************************************************/

Object *
opoppen(ULONG key,ULONG title,ULONG help)
{
    return PoppenObject,
        //MUIA_FixWidthTxt,  "nn",
        MUIA_FixHeightTxt, "\n",
        MUIA_Window_Title, CM_GetString(title),
        MUIA_Draggable,    TRUE,
        MUIA_CycleChain,   1,
        //MUIA_FixHeightTxt, "\n",
        _HELP(help),
        _KEY(key),
    End;
}

/***********************************************************************/

#ifdef __MORPHOS__

#ifndef PopframeObject
#define PopframeObject MUI_NewObject(MUIC_Popframe
#endif

Object *
opopframe(ULONG key,ULONG title,ULONG help)
{
    return PopframeObject,
        //MUIA_FixWidthTxt,  "nn",
        MUIA_FixHeightTxt, "\n",
        MUIA_Window_Title, CM_GetString(title),
        MUIA_Draggable,    TRUE,
        MUIA_CycleChain,   1,
        //MUIA_FixHeightTxt, "\n",
        _HELP(help),
        _KEY(key),
    End;
}
#endif

/***********************************************************************/

#ifndef PopimageObject
#define PopimageObject MUI_NewObject(MUIC_Popimage
#endif /* PopimageObject */

Object *
opopimage(ULONG type,ULONG key,ULONG title,ULONG help)
{
    return PopimageObject,
        //MUIA_FixWidthTxt,      "nn",
        MUIA_FixHeightTxt,     "\n",
        MUIA_Imageadjust_Type, type,
        MUIA_Window_Title,     CM_GetString(title),
        MUIA_Draggable,        TRUE,
        MUIA_CycleChain,       1,
        //MUIA_FixHeightTxt,     "\n",
        _HELP(help),
        _KEY(key),
    End;
}

/***********************************************************************/

ULONG
freeStack(void)
{
    register struct Task *me = FindTask(NULL);
    register ULONG free;

#ifdef __MORPHOS__
    {
        ULONG used, size;

    	NewGetTaskAttrs(me,&size,sizeof(size),TASKINFOTYPE_STACKSIZE,TAG_DONE);
    	NewGetTaskAttrs(me,&used,sizeof(used),TASKINFOTYPE_USEDSTACKSIZE,TAG_DONE);
	free = size-used;
    }
#else
    free = getreg(REG_A7)-(ULONG)me->tc_SPLower;
#endif

    return free;
}

/***********************************************************************/

