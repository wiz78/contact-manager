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
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/****************************************************************************/

struct data
{
    UBYTE buf[64];
};

/****************************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    return (ULONG)DoSuperNew(cl,obj,
            MUIA_CycleChain,   TRUE,
            MUIA_ShortHelp,    CM_GetString(Msg_PrefsWin_Settings_DelWarningHelp),
            MUIA_ControlChar,  getKeyCharID(Msg_PrefsWin_Settings_DelWarning),
            MUIA_Slider_Horiz, TRUE,
            MUIA_Slider_Min,   0,
            MUIA_Slider_Max,   DEF_DelLimit+1,
            TAG_MORE, msg->ops_AttrList);
}

/***********************************************************************/

static ULONG
mStringify(struct IClass *cl,Object *obj,struct MUIP_Numeric_Stringify *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       v = msg->value, id;

    if (v==0) id = Msg_DelWarning_Always;
    else if (v==1) id = Msg_DelWarning_Entry;
         else if (v<=DEF_DelLimit) id = Msg_DelWarning_Entries;
              else id = Msg_DelWarning_Never;

    snprintf(data->buf,sizeof(data->buf),CM_GetString(id),v);

    return (ULONG)data->buf;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                 return mNew(cl,obj,(APTR)msg);

        case MUIM_Numeric_Stringify: return mStringify(cl,obj,(APTR)msg);

        default:                     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initDelWarning(void)
{
    return (ULONG)(lib_DelWarning = MUI_CreateCustomClass(NULL,MUIC_Slider,NULL,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeDelWarning(void)
{
    MUI_DeleteCustomClass(lib_DelWarning);
    lib_DelWarning = NULL;
}

/***********************************************************************/
