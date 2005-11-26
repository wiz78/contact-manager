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

#include "cm.h"
#include "CManager_rev.h"

/***********************************************************************/

#define ARG_FILE       "FILE"
#define ARG_PUBSCREEN  "PUBSCREEN"
#define ARG_PORT       "PORT"
#define ARG_NOLOGIN    "NOLOGIN"
#define ARG_USER       "USER"
#define ARG_PASSWORD   "PASSWORD"
#define ARG_HIDEUSER   "HIDEUSER"
#define ARG_HIDEWWW    "HIDEWWW"
#define ARG_HIDEFTP    "HIDEFTP"
#define ARG_HIDECHAT   "HIDECHAT"
#define ARG_HIDETELNET "HIDETELNET"

#define TEMPLATE      ARG_FILE","ARG_PUBSCREEN"/K,"ARG_PORT"/K/N,"ARG_NOLOGIN"/S,"ARG_USER"/K,"ARG_PASSWORD"/K,"ARG_HIDEUSER"/S,"ARG_HIDEWWW"/S,"ARG_HIDEFTP"/S,"ARG_HIDECHAT"/S,"ARG_HIDETELNET"/S"

#define GETNUM(a)     (*((LONG *)a))

/***********************************************************************/

LONG
parseArgs(struct parseArgs *pa)
{
    register LONG res = RETURN_FAIL;

    if (_WBenchMsg)
    {
        register struct DiskObject *icon;
        register struct WBArg      *args;
        register BPTR              oldDir;

        args = _WBenchMsg->sm_ArgList;
        oldDir = CurrentDir(args[0].wa_Lock);

        if (icon = GetDiskObject(args[0].wa_Name))
        {
            register STRPTR value;

            if (value = FindToolType(icon->do_ToolTypes,ARG_FILE))
            {
                strncpy(pa->file,value,sizeof(pa->file));
                pa->flags |= PAFLG_File;
            }

            if (value = FindToolType(icon->do_ToolTypes,ARG_PUBSCREEN))
            {
                strncpy(pa->pubscreen,value,sizeof(pa->pubscreen));
                pa->flags |= PAFLG_Pubscreen;
            }

            if (FindToolType(icon->do_ToolTypes,ARG_NOLOGIN))
                pa->flags |= PAFLG_NoLogin;

            if (value = FindToolType(icon->do_ToolTypes,ARG_USER))
            {
                strncpy(pa->user,value,sizeof(pa->user));
                pa->flags |= PAFLG_User;
            }

            if (value = FindToolType(icon->do_ToolTypes,ARG_PASSWORD))
            {
                strncpy(pa->password,value,sizeof(pa->password));
                pa->flags |= PAFLG_Password;
            }

            if (FindToolType(icon->do_ToolTypes,ARG_HIDEUSER))
                pa->flags |= PAFLG_HideUser;

            if (FindToolType(icon->do_ToolTypes,ARG_HIDEWWW))
                pa->flags |= PAFLG_HideWWW;

            if (FindToolType(icon->do_ToolTypes,ARG_HIDEFTP))
                pa->flags |= PAFLG_HideFTP;

            if (FindToolType(icon->do_ToolTypes,ARG_HIDECHAT))
                pa->flags |= PAFLG_HideChat;

            if (FindToolType(icon->do_ToolTypes,ARG_HIDETELNET))
                pa->flags |= PAFLG_HideTelnet;

            FreeDiskObject(icon);
        }

        CurrentDir(oldDir);

        res = RETURN_OK;
    }
    else
    {
        register struct RDArgs *ra;
        APTR                   arg[16] = {0};

        if (ra = ReadArgs(TEMPLATE,(LONG *)arg,NULL))
        {
            if (arg[0])
            {
                strncpy(pa->file,(STRPTR)arg[0],sizeof(pa->file));
                pa->flags |= PAFLG_File;
            }

            if (arg[1])
            {
                strncpy(pa->pubscreen,(STRPTR)arg[1],sizeof(pa->pubscreen));
                pa->flags |= PAFLG_Pubscreen;
            }

            if (arg[2])
            {
                pa->port = (struct MsgPort *)GETNUM(arg[2]);
                pa->flags |= PAFLG_Port;
            }

            if (arg[3])
                pa->flags |= PAFLG_NoLogin;

            if (arg[4])
            {
                strncpy(pa->user,(STRPTR)arg[4],sizeof(pa->user));
                pa->flags |= PAFLG_User;
            }

            if (arg[5])
            {
                strncpy(pa->password,(STRPTR)arg[5],sizeof(pa->password));
                pa->flags |= PAFLG_Password;
            }

            if (arg[6])
                pa->flags |= PAFLG_HideUser;

            if (arg[7])
                pa->flags |= PAFLG_HideWWW;

            if (arg[8])
                pa->flags |= PAFLG_HideFTP;

            if (arg[9])
                pa->flags |= PAFLG_HideChat;

            if (arg[10])
                pa->flags |= PAFLG_HideTelnet;

            FreeArgs(ra);

            res = RETURN_OK;
        }
        else PrintFault(IoErr(),PRG);
    }

    return res;
}

/****************************************************************************/
