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

void
loadCountries(void)
{
    if (!(lib_flags & BASEFLG_Countries))
    {
        register BPTR  file;
        REGARRAY UBYTE name[256];

        lib_flags |= BASEFLG_Countries;

        strcpy(name,lib_CMPath);

        AddPart(name,DEF_CountriesFile,sizeof(name));

        if (file = Open(name,MODE_OLDFILE))
        {
            REGARRAY UBYTE buf[1024];

            while (FGets(file,buf,sizeof(buf)-1))
            {
                struct CSource            cs;
                register struct CMCountry *country;
                register STRPTR           ptr;

                if (ptr = strchr(buf,'\n')) *ptr = '\0';

                cs.CS_Buffer = buf;
                cs.CS_Length = strlen(buf);
                cs.CS_CurChr = 0;

                if (country = allocArbitratePooled(sizeof(struct CMCountry)))
                {
                    AddTail((struct List * )&lib_countries,(struct Node *)country);

                    ReadItem(country->Country,sizeof(country->Country),&cs);
                    ReadItem(country->Country2,sizeof(country->Country2),&cs);
                    ReadItem(country->Code,sizeof(country->Code),&cs);
                }
                else break;
            }

            Close(file);
        }
    }
}

/***********************************************************************/

void
localizeArray(STRPTR *strings,ULONG *ids)
{
    for (;;)
    {
    	if (*ids) *strings++ = CM_GetString(*ids++);
        else
        {
            *strings = NULL;
            break;
        }
    }
}

/***********************************************************************/

void
localizeMenus(struct NewMenu *menu,ULONG *ids)
{
    while (menu->nm_Type!=NM_END)
    {
        register ULONG id = *ids++;

        if (id && menu->nm_Label!=NM_BARLABEL)
            menu->nm_Label = CM_GetString(id);

        menu++;
    }
}

/***********************************************************************/

void
localizeButtonsBar(struct MUIS_TheBar_Button *buttons,ULONG *ids)
{
    while (buttons->img!=MUIV_TheBar_End)
    {
        register ULONG t = *ids++;
        register ULONG h = *ids++;

        if (t) buttons->text = CM_GetString(t);
        if (h) buttons->help = CM_GetString(h);

        buttons++;
    }
}

/***********************************************************************/
