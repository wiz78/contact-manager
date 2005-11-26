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


#include <proto/exec.h>
#include <proto/CManager.h>
#include <stdio.h>
#include <string.h>

/***************************************************************************/

struct Library *CManagerBase;

/***************************************************************************/

int
main(int argc,char **argv)
{
    int res = 20;

    if (CManagerBase = OpenLibrary("CManager.library",2))
    {
        APTR handle;

        if (handle = CM_StartManager(NULL,NULL))
        {
            struct MinList *mlist;

            res = 5;

            if (mlist = CM_GetEntry(handle,CMGETENTRY_User | CMGETENTRY_Multi))
            {
                struct MinNode *mstate, *succ;

                for (mstate = mlist->mlh_Head; succ = mstate->mln_Succ; mstate = succ)
                {
                    printf("FirstName:%s LastName:%s\n",((struct CMUser *)mstate)->Name,((struct CMUser *)mstate)->LastName);
                    CM_FreeEntry(mstate);
                }

                res = 0;
            }
            else printf("No entry selected\n");

            CM_FreeHandle(handle,TRUE);
        }
        else printf("Can't start CManager\n");

        CloseLibrary(CManagerBase);
    }
    else printf("Can't open CManager.library\n");

    return res;
}

/***********************************************************************/
