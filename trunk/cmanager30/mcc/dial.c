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
#include <exec/execbase.h>
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/***********************************************************************/

struct data
{
    Object             *app;
    Object             *MainWnd;
    Object             *cm;

    Object             *Text;
    ULONG               Aborted;

    struct timerequest *TimeoutIO;
    struct IOExtSer    *iow;
    struct IOExtSer    *ior;
    struct MsgPort     *Port;
    struct MsgPort     *TimeoutPort;
    UBYTE              LastCmd[256];

    /* Preferences */
    UBYTE              SerDevice[256];
    UBYTE              SerUnit;
    UBYTE              SerInit[256];
    UBYTE              DialPrefix[30];
    UBYTE              DialSuffix[30];
    UWORD              RedialDelay;
    UWORD              RedialAttempts;

    ULONG              Flags;
};

enum
{
    FLG_SerShared = 1<<0,
};

#define MUIV_SerialAbort (TAGBASE+1)

/***********************************************************************/

static LONG
serialPRead(struct data *data,STRPTR buf,ULONG bufsize)
{
    if (bufsize)
    {
        data->ior->IOSer.io_Command = SDCMD_QUERY;

        DoIO((struct IORequest *)data->ior);

        if (data->ior->IOSer.io_Actual>0)
        {
            if (data->ior->IOSer.io_Actual<bufsize) bufsize = data->ior->IOSer.io_Actual;

            data->ior->IOSer.io_Command = CMD_READ;
            data->ior->IOSer.io_Data    = buf;
            data->ior->IOSer.io_Length  = bufsize;

            if (DoIO((struct IORequest *)data->ior)) return -1;

            return (LONG)data->ior->IOSer.io_Actual;
        }
    }

    return 0;
}

/***********************************************************************/
/*
 * Wait for the timeout, or data being available.
 * If timeout, return 0.
 * If data available, grab all of it (up to bufsize) and return amount.
 */

static LONG
internalRead(struct data *data,STRPTR buf,ULONG bufsize)
{
    register LONG  bytes;
    register ULONG signals, ser_sig, time_sig;
    ULONG          sigmask = 0;

    if (bytes = serialPRead(data,buf,bufsize)) return bytes;

    /* No data. Wait for something to happen. */
    data->ior->IOSer.io_Command = CMD_READ;
    data->ior->IOSer.io_Data    = buf;
    data->ior->IOSer.io_Length  = 1;

    ser_sig  = 1L << data->Port->mp_SigBit;
    time_sig = 1L << data->TimeoutPort->mp_SigBit;

    /*  Clear the serial signal */
    SetSignal(0L,ser_sig);

    SendIO((struct IORequest *)data->ior);

    signals = ser_sig | time_sig | SIGBREAKF_CTRL_C;

    /* Wait for data or timeout. */
    while (DoMethod(data->app,MUIM_Application_Input,&sigmask)!=MUIV_SerialAbort)
    {
        if (!sigmask) continue;

        sigmask = Wait(signals | sigmask);

        if (sigmask & SIGBREAKF_CTRL_C) break;

        /* Was it data? */
        if (CheckIO((struct IORequest *)data->ior))
        {
            WaitIO((struct IORequest *)data->ior);

            bytes = data->ior->IOSer.io_Actual;

            if ((bytes) && (--bufsize)) bytes += serialPRead(data,buf+1,bufsize);

            return bytes;
        }

        /* timeout? */
        if (CheckIO((struct IORequest *)data->TimeoutIO)) break;
    }

    /* Nope - a timeout */
    AbortIO((struct IORequest *)data->ior);
    WaitIO((struct IORequest *)data->ior);

    return 0;
}

/***********************************************************************/

static void
setTimeout(struct data *data,ULONG Sec)
{
    if (Sec!=0)
    {
        data->TimeoutIO->tr_node.io_Command = TR_ADDREQUEST;
        data->TimeoutIO->tr_time.tv_secs    = Sec;
        data->TimeoutIO->tr_time.tv_micro   = 0;

        SendIO((struct IORequest *)data->TimeoutIO);
    }
    else
        if (data->TimeoutIO)
        {
            AbortIO((struct IORequest *)data->TimeoutIO);
            WaitIO((struct IORequest *)data->TimeoutIO);
        }
}

/***********************************************************************/

static void
flushMem(void)
{
    register APTR mem;

    if (mem = AllocMem((ULONG)-1,0L))
    {
        flushMem();
        FreeMem(mem,(ULONG)-1);
    }
}

/***********************************************************************/

static void
serialAbort(struct data *data)
{
    if (!(CheckIO((struct IORequest *)data->iow)))
    {
        AbortIO((struct IORequest *)data->iow);
        WaitIO((struct IORequest *)data->iow);
    }

    if (!(CheckIO((struct IORequest *)data->ior)))
    {
        AbortIO((struct IORequest *)data->ior);
        WaitIO((struct IORequest *)data->ior);
    }
}

/***********************************************************************/

static ULONG
setupSerial(struct data *data,ULONG Baud,ULONG Shared)
{
    serialAbort(data);

    data->iow->IOSer.io_Command = SDCMD_SETPARAMS;
    data->iow->io_ExtFlags      = 0;
    data->iow->io_ReadLen       = 8;
    data->iow->io_WriteLen      = 8;
    data->iow->io_StopBits      = 1;
    data->iow->io_Baud          = Baud;
    data->iow->io_SerFlags      = SERF_XDISABLED | SERF_7WIRE;

    if (Shared) data->iow->io_SerFlags |= SERF_SHARED;

    DoIO((struct IORequest *)data->iow);

    data->iow->IOSer.io_Flags = IOF_QUICK;

    *data->ior = *data->iow;

    return (ULONG)((data->iow->IOSer.io_Error) ? FALSE : TRUE);
}

/***********************************************************************/

static ULONG
openSerial(struct data *data,STRPTR Device,ULONG Unit,ULONG Baud,ULONG Shared)
{
    if (!(data->Port = CreateMsgPort())) return FALSE;

    if (!(data->iow = (struct IOExtSer *)CreateIORequest(data->Port,sizeof(struct IOExtSer))))
        return FALSE;

    data->iow->IOSer.io_Flags = IOF_QUICK;

    if (!(data->ior = (struct IOExtSer *)CreateIORequest(data->Port,sizeof(struct IOExtSer))))
        return FALSE;

    data->iow->io_SerFlags = SERF_7WIRE;

    if (Shared) data->iow->io_SerFlags |= SERF_SHARED;

    if (OpenDevice(Device,Unit,(struct IORequest *)data->iow,0L))
    {
        flushMem();

        if (OpenDevice(Device,Unit,(struct IORequest *)data->iow,0L))
            return FALSE;
    }

    if (!setupSerial(data,Baud,Shared)) return FALSE;

    if (!(data->TimeoutPort = CreateMsgPort())) return FALSE;

    if (!(data->TimeoutIO = CreateIORequest(data->TimeoutPort,sizeof(struct timerequest))))
        return FALSE;

    if (OpenDevice("timer.device",UNIT_VBLANK,(struct IORequest *)data->TimeoutIO,0))
        return FALSE;

    return TRUE;
}

/***********************************************************************/

static void
closeSerial(struct data *data)
{
    if (data->TimeoutIO)
    {
        if ((data->TimeoutIO->tr_node.io_Device!=0) && ((long)data->TimeoutIO->tr_node.io_Device!=-1))
        {
            if (!(CheckIO((struct IORequest *)data->TimeoutIO)))
            {
                AbortIO((struct IORequest *)data->TimeoutIO);
                WaitIO((struct IORequest *)data->TimeoutIO);
            }

            CloseDevice((struct IORequest *)data->TimeoutIO);
        }

        DeleteIORequest((struct IORequest *)data->TimeoutIO);

        data->TimeoutIO = NULL;
    }

    if (data->iow)
    {
        register struct Device *dev;

        if ((data->iow->IOSer.io_Device!=0) && ((long)data->iow->IOSer.io_Device!=-1))
        {
            if (data->ior)
            {
                if (data->ior->IOSer.io_Message.mn_Node.ln_Type!=NT_MESSAGE)
                    if (!(CheckIO((struct IORequest *)data->ior)))
                    {
                        AbortIO((struct IORequest *)data->ior);
                        WaitIO((struct IORequest *)data->ior);
                    }
            }

            if (data->iow->IOSer.io_Message.mn_Node.ln_Type!=NT_MESSAGE)
                if (!(CheckIO((struct IORequest *)data->iow)))
                {
                    AbortIO((struct IORequest *)data->iow);
                    WaitIO((struct IORequest *)data->iow);
                }

            CloseDevice((struct IORequest *)data->iow);
        }

        DeleteIORequest((struct IORequest *)data->iow);
        data->iow = NULL;

        Forbid(); /* Remove the device from memory */

        if (dev = (struct Device *)FindName(&SysBase->DeviceList,data->SerDevice))
            RemDevice(dev);

        Permit();
    }

    if (data->ior)
    {
        DeleteIORequest((struct IORequest *)data->ior);
        data->ior = NULL;
    }

    if (data->Port)
    {
        DeleteMsgPort(data->Port);
        data->Port = NULL;
    }

    if (data->TimeoutPort)
    {
        DeleteMsgPort(data->TimeoutPort);
        data->TimeoutPort = NULL;
    }
}

/***********************************************************************/

static ULONG
serialWrite(struct data *data,STRPTR Buffer,ULONG BufLen)
{
    if (WaitIO((struct IORequest *)data->iow))
    {
        data->iow->IOSer.io_Flags = IOF_QUICK;
        return(FALSE);
    }

    data->iow->IOSer.io_Command = CMD_WRITE;
    data->iow->IOSer.io_Data    = Buffer;
    data->iow->IOSer.io_Length  = BufLen;

    DoIO((struct IORequest *)data->iow);

    return (ULONG)(data->iow->IOSer.io_Error ? FALSE : TRUE);
}

/***********************************************************************/

static ULONG
serialPrintf(struct data *data,STRPTR Format,...)
{
    REGARRAY UBYTE buf[1024];

    vsprintf(buf,Format,&Format+1);
    strcpy(data->LastCmd,buf);

    return serialWrite(data,buf,strlen(buf));
}

/***********************************************************************/

static ULONG
serialReadln(struct data *data,STRPTR Dest,ULONG Time)
{
    register ULONG  ret = FALSE, Something = FALSE;
    UBYTE           Line[128];
    register STRPTR Buffer;

    Line[0] = '\0';

    setTimeout(data, Time);

    Buffer = Line;

    for(;;)
    {
        LONG n;

        if ((n = internalRead(data,Buffer,1))<0) break;
        else
            if (n==0)
            {   // Timeout
                Line[0] = '\0';
                break;
            }

        if ((*Buffer=='\r') || (*Buffer=='\n'))
        {
            if (Something)
            {   // Skip initial <CR><LF>

                *Buffer = '\0';

                if (strncmp(Line,data->LastCmd,strlen(Line))!=0)
                {   // not an echo?
                    ret = TRUE;
                    break;
                }
                else
                {
                    Buffer    = Line;
                    Something = FALSE;
                }
            }
        }
        else
        {
            Something = TRUE;

            if (++Buffer>&Line[sizeof(Line)-1]) Buffer = Line;
        }
    }

    setTimeout(data,0);
    strcpy(Dest,Line);

    return ret;
}

/***********************************************************************/

static void
serialFlush(struct data *data)
{
    if (!(CheckIO((struct IORequest *)data->ior)))
    {
        AbortIO((struct IORequest *)data->ior);
        WaitIO((struct IORequest *)data->ior);
    }

    data->ior->IOSer.io_Command = CMD_CLEAR;

    DoIO((struct IORequest *)data->ior);
}

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data     temp;
    register Object *Abort;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode,        "CMDIAL",
            MUIA_BWin_Background, MUII_RequesterBack,
            MUIA_Window_Title,    CM_GetString(Msg_Dial_WinTitle),
            MUIA_Window_ID,       MAKE_ID('c','m','d','i'),
            MUIA_Window_NoMenus,  TRUE,

            WindowContents, VGroup,
                MUIA_Background, MUII_RequesterBack,

                Child, temp.Text = TextObject,
                    TextFrame,
                    InnerSpacing(8,8),
                    MUIA_Background,    MUII_TextBack,
                    MUIA_Text_PreParse, MUIX_C,
                End,

                Child, HGroup,
                    Child, HSpace(0),
                    Child, Abort = obutton(Msg_Dial_Cancel,0),
                    Child, HSpace(0),
                End,
            End,

            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl, obj);
        struct CMPrefs       *prefs;

        *data = temp;

        data->app = (Object *)GetTagData(MUIA_CManager_AppObj,NULL,msg->ops_AttrList);
        data->cm  = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList);

        get(data->cm,MUIA_CManager_Prefs,&prefs);
        strcpy(data->SerDevice,prefs->SerDevice);
        strcpy(data->SerInit,prefs->SerInit);
        strcpy(data->DialPrefix,prefs->DialPrefix);
        strcpy(data->DialSuffix,prefs->DialSuffix);
        data->SerUnit        = prefs->SerUnit;
        data->RedialDelay    = prefs->RedialDelay;
        data->RedialAttempts = prefs->RedialAttempts;

        if (prefs->DialOptions & DIALOPT_SerShared) data->Flags |= FLG_SerShared;

        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,1,MUIM_CManager_WinClose);
        DoMethod(Abort,MUIM_Notify,MUIA_Pressed,FALSE,obj,1,MUIM_CManager_WinClose);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    switch (msg->opg_AttrID)
    {
        case MUIA_CManager_IsCMWin: *msg->opg_Storage = TRUE; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mDialWindowOpen(struct IClass *cl,Object *obj,struct MUIP_DialWindow_Open *msg)
{
    register struct data *data = INST_DATA(cl, obj);
    REGARRAY UBYTE       buffer[1024];
    register STRPTR      suffix;
    register ULONG       go;

    data->app = _app(obj);
    data->MainWnd = msg->MainWnd;

    set(data->MainWnd,MUIA_Window_Sleep,TRUE);
    sprintf(buffer,CM_GetString(Msg_Dial_Dialling),msg->Number,1,data->RedialAttempts);
    set(data->Text,MUIA_Text_Contents,buffer);
    SetSuperAttrs(cl,obj,MUIA_Window_Open,TRUE,TAG_DONE);
    data->Aborted = FALSE;

    suffix = *data->DialSuffix ? data->DialSuffix : (STRPTR)"\r";

    do
    {
        go = FALSE;

        if (openSerial(data,data->SerDevice,data->SerUnit,9600,data->Flags & FLG_SerShared))
        {
            ULONG  loop;
            UWORD attempt = 0;

            serialPrintf(data,"\r");
            Delay(35);
            serialFlush(data);

            serialPrintf(data,"ATZ\r");

            if (!(serialReadln(data,buffer,10)))
            {
                if (go = MUI_Request(_app(obj),data->MainWnd,0,CM_GetString(Msg_Req_Title),
                                      CM_GetString(Msg_Dial_RetryCancelGads),
                                      CM_GetString(Msg_Dial_ModemNotResponding)))
                    closeSerial(data);

            }
            else
            {
                register STRPTR ptr;

                Delay(15);

                if (ptr = strchr(data->SerInit,'\\'))
                    if (ptr[1]=='r') *ptr = '\0';

                serialPrintf(data,"%s\r",data->SerInit);
                serialReadln(data,buffer,10);

                Delay(15);

                do
                {
                    loop = FALSE;

                    ++attempt;

                    sprintf(buffer,CM_GetString(Msg_Dial_Dialling),msg->Number,attempt,data->RedialAttempts);

                    set(data->Text,MUIA_Text_Contents,buffer);

                    serialFlush(data);
                    serialPrintf(data,"%s%s%s",data->DialPrefix,msg->Number,suffix);

                    do
                    {
                        serialReadln(data,buffer,65000);
                        if (data->Aborted) break;

                        if ((strncmp(buffer,"NO ANSWER",9)==0) ||
                            (strncmp(buffer,"8",1)==0) ||
                            (strncmp(buffer,"BUSY",4)==0) ||
                            (strncmp(buffer,"7",1)==0)) loop = TRUE;

                    }
                    while (strncmp(buffer,"RINGING",7)==0);

                    if (loop && (attempt<=data->RedialAttempts))
                    {
                        register int i;

                        for (i = data->RedialDelay; i>0; i--)
                        {
                            ULONG sigmask = 0;

                            sprintf(buffer,CM_GetString(Msg_Dial_Waiting),i);
                            set(data->Text,MUIA_Text_Contents,buffer);

                            Delay(TICKS_PER_SECOND);

                            if (DoMethod(data->app,MUIM_Application_Input,&sigmask)==MUIV_SerialAbort)
                            {
                                loop = FALSE;
                                break;
                            }
                        }
                    }
                    else loop = FALSE;
                }
                while (loop);

                serialAbort(data);
                serialPrintf(data,"\r");
                serialFlush(data);
            }
        }
        else
        {
            go = MUI_Request(_app(obj),data->MainWnd,0,CM_GetString(Msg_Req_Title),
                              CM_GetString(Msg_Dial_RetryCancelGads),
                              CM_GetString(Msg_Dial_NoDevice),
                              data->SerUnit, data->SerDevice);
        }

    }
    while (go);

    closeSerial(data);


    /* Setting super here generates:
     * CXERR: 460351, Line: 683, Qpos: 983
     */
    SetAttrs(obj,MUIA_Window_Open,FALSE,TAG_DONE);
    DoMethod(data->app,MUIM_Application_PushMethod,data->cm,2,MUIM_CManager_DisposeObj,obj);
    set(data->MainWnd,MUIA_Window_Sleep,FALSE);

    return 0;
}

/***********************************************************************/

static ULONG
mClose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl, obj);

    data->Aborted = TRUE;

    DoMethod(_app(obj),MUIM_Application_ReturnID,MUIV_SerialAbort);

    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                 return mNew(cl,obj,(APTR)msg);
        case OM_GET:                 return mGet(cl,obj,(APTR)msg);

        case MUIM_DialWindow_Open:   return mDialWindowOpen(cl,obj,(APTR)msg);
        case MUIM_CManager_WinClose: return mClose(cl,obj,(APTR)msg);

        default:                     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initDialWindow(void)
{
    return (ULONG)(lib_DialWindow = MUI_CreateCustomClass(NULL,NULL,lib_MatherWin,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeDialWindow(void)
{
    MUI_DeleteCustomClass(lib_DialWindow);
    lib_DialWindow = NULL;
}

/***********************************************************************/
