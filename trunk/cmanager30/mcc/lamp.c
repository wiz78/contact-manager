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
#include <graphics/rpattr.h>
#define CATCOMP_NUMBERS
#include <CManager_loc.h>

/***********************************************************************/

struct data
{
    LONG  offPen;
    LONG  onPen;
    ULONG flags;
};

enum
{
    FLG_Setup    = 1<<0,
    FLG_FreePens = 1<<1,
    FLG_Off      = 1<<2,
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Font,  MUIV_Font_Normal,
            MUIA_Frame, "110000",
            TAG_MORE, msg->ops_AttrList))
    {
        struct data *data = INST_DATA(cl,obj);

        if (GetTagData(MUIA_Lamp_Off,TRUE,msg->ops_AttrList))
            data->flags |= FLG_Off;
    }

    return (ULONG)obj;
}

/**************************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data    *data = INST_DATA(cl,obj);
    struct TagItem *tag;

    if ((tag = FindTagItem(MUIA_Lamp_Off,msg->ops_AttrList)) && !BOOLSAME(tag->ti_Data,data->flags & FLG_Off))
    {
        if (tag->ti_Data) data->flags |= FLG_Off;
        else data->flags &= ~FLG_Off;

        MUI_Redraw(obj,MADF_DRAWUPDATE);
    }

    return DoSuperMethodA(cl,obj,(APTR)msg);
}

/***********************************************************************/

static ULONG
mSetup(struct IClass *cl,Object *obj,struct MUIP_Setup *msg)
{
    struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,(APTR)msg)) return FALSE;

    if (GetBitMapAttr(_screen(obj)->RastPort.BitMap,BMA_DEPTH)>=8)
    {
        //data->offPen = MUI_ObtainPen(muiRenderInfo(obj),(APTR)"r00000000,00000000,00000000",0);
        data->offPen = MPEN_TEXT;
        data->onPen  = MUI_ObtainPen(muiRenderInfo(obj),(APTR)"rb4b4b4b4,21212121,43434343",0);
        data->flags |= FLG_FreePens;
    }
    else
    {
        data->offPen = MPEN_TEXT;
        data->onPen  = MPEN_FILL;
    }

    data->flags |= FLG_Setup;

    return TRUE;
}

/***********************************************************************/

static ULONG
mCleanup(struct IClass *cl,Object *obj,struct MUIP_Setup *msg)
{
    struct data *data = INST_DATA(cl,obj);

    if (data->flags & FLG_FreePens)
    {
        //MUI_ReleasePen(muiRenderInfo(obj),data->offPen);
        MUI_ReleasePen(muiRenderInfo(obj),data->onPen);
    }

    data->flags &= ~(FLG_Setup|FLG_FreePens);

    return DoSuperMethodA(cl,obj,(APTR)msg);
}

/***********************************************************************/

static ULONG
mAskMinMax(struct IClass *cl,Object *obj,struct MUIP_AskMinMax *msg)
{
    struct RastPort   rp;
    struct TextExtent te;
    UWORD             w, h, d;

    DoSuperMethodA(cl,obj,(APTR)msg);

    CopyMem(&_screen(obj)->RastPort,&rp,sizeof(rp));

    SetFont(&rp,_font(obj));
    TextExtent(&rp,"X",1,&te);

    w = te.te_Width;
    h = te.te_Height;

    if (w>=h) d = h;
    else d = w;
    if (d<12) d = 6;
    else d /= 2;

    if (w>=h) d = w;
    else d = h;
    d = d -2;

    msg->MinMaxInfo->MinWidth  += d;
    msg->MinMaxInfo->MinHeight += d;
    msg->MinMaxInfo->DefWidth  += d;
    msg->MinMaxInfo->DefHeight += d;
    msg->MinMaxInfo->MaxWidth  += d;
    msg->MinMaxInfo->MaxHeight += d;

    return 0;
}

/***********************************************************************/

static ULONG
mDraw(struct IClass *cl,Object *obj,struct MUIP_Draw *msg)
{
    struct data *data = INST_DATA(cl,obj);
    ULONG       res;

    res = DoSuperMethodA(cl,obj,(APTR)msg);

    if (msg->flags & (MADF_DRAWOBJECT|MADF_DRAWUPDATE))
    {
        SetAPen(_rp(obj),MUIPEN((data->flags & FLG_Off) ? data->offPen : data->onPen));
        RectFill(_rp(obj),_mleft(obj),_mtop(obj),_mright(obj),_mbottom(obj));
    }

    return res;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:         return mNew(cl,obj,(APTR)msg);
        case OM_SET:         return mSets(cl,obj,(APTR)msg);

        case MUIM_Setup:     return mSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup:   return mCleanup(cl,obj,(APTR)msg);
        case MUIM_AskMinMax: return mAskMinMax(cl,obj,(APTR)msg);
        case MUIM_Draw:      return mDraw(cl,obj,(APTR)msg);

        default:             return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initLamp(void)
{
    if (lib_Lamp = MUI_CreateCustomClass(NULL,MUIC_Area,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
disposeLamp(void)
{
    if (lib_Lamp)
    {
        MUI_DeleteCustomClass(lib_Lamp);
        lib_Lamp = NULL;
    }
}

/**********************************************************************/
