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

/***********************************************************************/

static struct MUI_CustomClass *VNPopClass = NULL, *VNListClass = NULL, *birthdayClass = NULL;

#define VNListObject   NewObject(VNListClass->mcc_Class,NULL
#define VNPopObject    NewObject(VNPopClass->mcc_Class,NULL
#define birthdayObject NewObject(birthdayClass->mcc_Class,NULL

/***********************************************************************/

struct birthdayData
{
    Object *use;
    Object *str;
    Object *cal;
};

/***********************************************************************/

M_HOOK(bdClose,Object *cal,Object *str)
{
    REGARRAY UBYTE buf[128];

    DoMethod(cal,MUIM_Calendar_FormatDate,MUIV_Calendar_FormatDate_Long,buf,sizeof(buf));
    set(str,MUIA_Text_Contents,buf);

    return 0;
}

/***********************************************************************/

static ULONG
mbirthdayNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object *use, *str, *bt, *cal;

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_Popstring_String, HGroup,
                TextFrame,
                Child, use = MUI_MakeObject(MUIO_Checkmark,0),
                Child, str = TextObject, MUIA_ShowSelState, FALSE, MUIA_InputMode, MUIV_InputMode_RelVerify, MUIA_Text_PreParse, "\33c", End,
            End,
            MUIA_Popstring_Button, bt = MUI_MakeObject(MUIO_PopButton,MUII_PopUp),
            MUIA_Popobject_Object, cal = CalendarObject,
                InputListFrame,
                MUIA_Calendar_MinVer,           15,
                MUIA_Calendar_ShowToday,        TRUE,
                MUIA_Calendar_TodayClick,       MUIV_Calendar_TodayClick_Double,
                MUIA_Calendar_SetDaysAsDefault, TRUE,
            End,
            MUIA_Popobject_ObjStrHook, &bdClose_hook,
            TAG_MORE,msg->ops_AttrList))
    {
        register struct birthdayData *data = INST_DATA(cl,obj);
        register ULONG  ccharid;

        data->use = use;
        data->str = str;
        data->cal = cal;

#ifdef __MORPHOS__
        bdClose_GATE2(NULL,cal,str);
#else
	bdClose_func(NULL,cal,str);
#endif

        DoMethod(cal,MUIM_Notify,MUIA_Calendar_DoubleClick,MUIV_EveryTime,obj,2,MUIM_Popstring_Close,TRUE);
        DoMethod(cal,MUIM_Notify,MUIA_Calendar_DoubleClick,MUIV_EveryTime,use,3,MUIM_Set,MUIA_Selected,TRUE);
        DoMethod(str,MUIM_Notify,MUIA_Pressed,TRUE,obj,1,MUIM_Popstring_Open);

        set(use,MUIA_CycleChain,TRUE);

        set(bt,MUIA_CycleChain,TRUE);
        if  (ccharid = GetTagData(MUIA_Birthday_ControlChar,0,msg->ops_AttrList))
            SetAttrs(bt,MUIA_ControlChar,getKeyCharID(ccharid),TAG_DONE);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mbirthdayGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    register struct birthdayData *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_Birthday_Use:  return get(data->use,MUIA_Selected,msg->opg_Storage);
        case MUIA_Birthday_Date: return get(data->cal,MUIA_Calendar_Greg,msg->opg_Storage);

        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mbirthdaySets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct birthdayData *data = INST_DATA(cl,obj);
    register struct TagItem      *tag;
    struct TagItem               *tags;

    for (tags = msg->ops_AttrList; tag = NextTagItem(&tags); )
    {
        register ULONG tidata = tag->ti_Data;

        switch (tag->ti_Tag)
        {
            case MUIA_Birthday_Use:
                set(data->use,MUIA_Selected,tidata);
                break;

            case MUIA_Birthday_Date:
                if (tidata==0) break;
                set(data->cal,MUIA_Calendar_Greg,tidata);
#ifdef __MORPHOS__
	        bdClose_GATE2(NULL,data->cal,data->str);
#else
		bdClose_func(NULL,data->cal,data->str);
#endif
                break;
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

M_DISP(birthdayDispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW: return mbirthdayNew(cl,obj,(APTR)msg);
        case OM_GET: return mbirthdayGet(cl,obj,(APTR)msg);
        case OM_SET: return mbirthdaySets(cl,obj,(APTR)msg);
        default:     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(birthdayDispatcher)

/***********************************************************************/

static ULONG
initBirthday(void)
{
    return (ULONG)(birthdayClass = MUI_CreateCustomClass(NULL,MUIC_Popobject,NULL,sizeof(struct birthdayData),DISP(birthdayDispatcher)));
}

/***********************************************************************/

static void
disposeBirthday(void)
{
    MUI_DeleteCustomClass(birthdayClass);
    birthdayClass = NULL;
}

/***********************************************************************/

struct VNListData
{
    Object *edit;
};

/***********************************************************************/

static ULONG
mVNListNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    return (ULONG)DoSuperNew(cl,obj,
            MUIA_ContextMenu,         NULL,
            MUIA_NList_ConstructHook, MUIV_NList_ConstructHook_String,
            MUIA_NList_DestructHook,  MUIV_NList_DestructHook_String,
            TAG_MORE,msg->ops_AttrList);
}

/***********************************************************************/

static ULONG
mVNListSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct VNListData *data = INST_DATA(cl,obj);
    register struct TagItem    *tag;

    if (tag = FindTagItem(MUIA_VNList_Edit,msg->ops_AttrList))
        data->edit = (Object *)tag->ti_Data;

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static  ULONG
mVNListUpdate(struct IClass *cl,Object *obj,Msg msg)
{
    register struct VNListData  *data = INST_DATA(cl,obj);
    REGARRAY UBYTE              buf[256];
    register STRPTR 		vn;
    struct MUIS_Edit_VisualInfo info;

    DoSuperMethod(cl,obj,MUIM_NList_Clear);
    DoMethod(data->edit,MUIM_Edit_GetVisualInfo,&info);

    vn = info.VisualName;

    if (*vn) DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,vn,MUIV_NList_Insert_Bottom);

    if (*info.Name)
    {
        if (*info.LastName)
        {
            sprintf(buf,"%s, %s",info.Name,info.LastName);
            if (strcmp(buf,vn)) DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,buf,MUIV_NList_Insert_Bottom);

            sprintf(buf,"%s %s",info.Name,info.LastName);
            if (strcmp(buf,vn)) DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,buf,MUIV_NList_Insert_Bottom);

            sprintf(buf,"%s, %s",info.LastName,info.Name);
            if (strcmp(buf,vn)) DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,buf,MUIV_NList_Insert_Bottom);

            sprintf(buf,"%s %s",info.LastName,info.Name);
            if (strcmp(buf,vn)) DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,buf,MUIV_NList_Insert_Bottom);
        }

        if (strcmp(info.Name,vn)) DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,info.Name,MUIV_NList_Insert_Bottom);
    }

    if (*info.LastName)
        if (strcmp(info.LastName,vn)) DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,info.LastName,MUIV_NList_Insert_Bottom);

    if (*info.Alias)
        if (strcmp(info.Alias,vn)) DoSuperMethod(cl,obj,MUIM_NList_InsertSingle,info.Alias,MUIV_NList_Insert_Bottom);

    return 0;
}

/***********************************************************************/

M_DISP(VNListDispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:             return mVNListNew(cl,obj,(APTR)msg);
        case OM_SET:             return mVNListSets(cl,obj,(APTR)msg);
        case MUIM_VNList_Update: return mVNListUpdate(cl,obj,(APTR)msg);
        default:                 return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(VNListDispatcher)

/***********************************************************************/

static ULONG
initVNList(void)
{
    return (ULONG)(VNListClass = MUI_CreateCustomClass(NULL,MUIC_NList,NULL,sizeof(struct VNListData),DISP(VNListDispatcher)));
}

/***********************************************************************/

static void
disposeVNList(void)
{
    MUI_DeleteCustomClass(VNListClass);
    VNListClass = NULL;
}

/***********************************************************************/

struct VNPopData
{
    Object      *VNList;
};

/***********************************************************************/

M_HOOK(VNWindow,Object *pop,Object *win)
{
    set(win,MUIA_Window_DefaultObject,pop);

    return 0;
}

/***********************************************************************/

M_HOOK(VNOpen,Object *list,Object *str)
{
    register STRPTR s;
    STRPTR          x;
    register int    i;

    DoMethod(list,MUIM_VNList_Update);
    s = (STRPTR)xget(str,MUIA_Textinput_Contents);

    for (i = 0; ;i++)
    {
        DoMethod(list,MUIM_NList_GetEntry,i,&x);
        if (!x)
        {
            set(list,MUIA_NList_Active,MUIV_NList_Active_Off);
            break;
        }
        else
            if (!stricmp(x,s))
            {
                set(list,MUIA_NList_Active,i);
                break;
            }
    }

    return TRUE;
}

/***********************************************************************/

M_HOOK(VNClose,Object *list,Object *str)
{
    STRPTR e;

    DoMethod(list,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&e);
    set(str,MUIA_Textinput_Contents,e);

    return 0;
}

/***********************************************************************/

static  ULONG
mVNPopNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object *bt, *lv, *VNList;

    if (obj = (Object *)DoSuperNew(cl,obj,

            MUIA_Popstring_String, ostring(64,Msg_Edit_UserVisualName,0),
            MUIA_Popstring_Button, bt = MUI_MakeObject(MUIO_PopButton,MUII_PopUp),

            MUIA_Popobject_Object, NListviewObject,
                MUIA_NListview_NList, lv = VNList = VNListObject, End,
            End,

            MUIA_Popobject_StrObjHook, &VNOpen_hook,
            MUIA_Popobject_ObjStrHook, &VNClose_hook,

            TAG_MORE,msg->ops_AttrList))
    {
        register struct VNPopData *data = INST_DATA(cl,obj);

        data->VNList = VNList;

        set(bt,MUIA_CycleChain,TRUE);

        DoMethod(lv,MUIM_Notify,MUIA_NList_DoubleClick,MUIV_EveryTime,obj,2,MUIM_Popstring_Close,TRUE);

        SetSuperAttrs(cl,obj,MUIA_Popobject_WindowHook,&VNWindow_hook,TAG_DONE);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mVNPopSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct VNPopData *data = INST_DATA(cl,obj);
    register struct TagItem   *tag;

    if (tag = FindTagItem(MUIA_VNList_Edit,msg->ops_AttrList))
        set(data->VNList,MUIA_VNList_Edit,tag->ti_Data);

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

M_DISP(VNPopDispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW: return mVNPopNew(cl,obj,(APTR)msg);
        case OM_SET: return mVNPopSets(cl,obj,(APTR)msg);
        default:     return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(VNPopDispatcher)

/***********************************************************************/

static ULONG
initVNPop(void)
{
    if ((!VNListClass && !initVNList()) ||
        !(VNPopClass = MUI_CreateCustomClass(NULL,MUIC_Popobject,NULL,sizeof(struct VNPopData),DISP(VNPopDispatcher))))
    {
        if (VNListClass) disposeVNList();
        return FALSE;
    }

    return TRUE;
}

/***********************************************************************/

static void
disposeVNPop(void)
{
    MUI_DeleteCustomClass(VNPopClass);
    VNPopClass = NULL;
    disposeVNList();
}

/***********************************************************************/

struct data
{

    struct CMUser *Current;

    Object        *CMObj;
    Object        *Name;
    Object        *VisualName;
    Object        *LastName;
    Object        *Address;
    Object        *City;
    Object        *Country;
    Object        *ZIP;
    Object        *Alias;
    Object        *Comment;
    Object        *Phone;
    Object        *Mobile;
    Object        *Fax;
    Object        *EMails;
    Object        *EMail;
    Object        *EMailAdd;
    Object        *EMailRem;
    Object        *EMailUp;
    Object        *EMailDown;
    Object        *Sex;
    Object        *Birthday;
    Object        *Anniversary;
    Object        *WWW;
    Object        *FTP;
    Object        *ICQ;
    Object        *Company;
    Object        *BJobTitle;
    Object        *BOffice;
    Object        *BAddress;
    Object        *BCity;
    Object        *BCountry;
    Object        *BZIP;
    Object        *BDepartment;
    Object        *BPhone;
    Object        *BMobile;
    Object        *BFax;
    Object        *BEMail;
    Object        *PGP;
    Object        *ImagePic;
    Object        *ImageFile;
    Object        *ImageTrans;

    ULONG         Flags;
};

enum
{
    EDITFLG_NTChangeVisualName = 1<<0,
};

/***********************************************************************/

M_HOOK(emCon,APTR pool,struct CMEMail *email)
{
    register struct CMEMail *entry;

    if (entry = CM_AllocObject(CMALLOCOBJECT_EMail))
    {
        if (email==(APTR)(-1)) *entry->EMail = 0;
        else copymem(entry,email,sizeof(struct CMEMail));
    }

    return (LONG)entry;
}

M_HOOK(emDes,APTR pool,APTR entry)
{
    CM_FreeObject(entry);

    return 0;
}

M_HOOK(emDisp,STRPTR *array,struct CMEMail *entry)
{
    *array = entry->EMail;

    return 0;
}

/***********************************************************************/

static ULONG regLabelsIDs[] =
{
    Msg_Edit_UserNameTitle,
    Msg_Edit_UserHomeTitle,
    Msg_Edit_UserBusiTitle,
    Msg_Edit_UserNoteTitle,
    Msg_Edit_UserImageTitle,
    0
};
static STRPTR regLabels[IDSSIZE(regLabelsIDs)];

static ULONG sexLabelsIDs[] =
{
    Msg_Edit_UserSex_None,
    Msg_Edit_UserSex_Male,
    Msg_Edit_UserSex_Female,
    0
};
static STRPTR sexLabels[IDSSIZE(sexLabelsIDs)];

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register Object *goUrl, *getMap;
    struct data     temp;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode, "CMUSERS",

            Child, RegisterGroup(regLabels),
                MUIA_CycleChain, TRUE,

                /* Name */
                Child, RootScrollgroupObject,
                    MUIA_Scrollgroup_FreeHoriz, FALSE,
                    MUIA_Scrollgroup_Contents, VirtgroupObject,
                        MUIA_Frame, MUIV_Frame_Virtual,
                        Child, ColGroup(2),

                            Child, olabel2(Msg_Edit_UserFirst),
                            Child, temp.Name = ostring(80,Msg_Edit_UserFirst,0),
                            Child, olabel2(Msg_Edit_UserLast),
                            Child, temp.LastName = ostring(80,Msg_Edit_UserLast,0),

                            Child, olabel2(Msg_Edit_UserAlias),
                            Child, HGroup,
                                Child, temp.Alias = ostring(40,Msg_Edit_UserAlias,0),
                                Child, olabel2(Msg_Edit_UserSex),
                                Child, temp.Sex = ocycle(sexLabels,Msg_Edit_UserSex,0),
                            End,

                            Child, olabel2(Msg_Edit_UserVisualName),
                            Child, temp.VisualName = VNPopObject, End,

                            Child, olabel2(Msg_Edit_UserBirthday),
                            Child, temp.Birthday = birthdayObject, MUIA_Birthday_ControlChar, Msg_Edit_UserBirthday, End,

                            Child, olabel2(Msg_Edit_UserAnniversary),
                            Child, temp.Anniversary = birthdayObject, MUIA_Birthday_ControlChar, Msg_Edit_UserAnniversary, End,

                            Child, olabel2(Msg_Edit_UserPGP),
                            Child, temp.PGP = ostring(80,Msg_Edit_UserPGP,0),

                            Child, VGroup,
                                Child, HGroup,
                                    Child, HSpace(0),
                                    Child, olabel2(Msg_Edit_UserEMail),
                                End,
                                Child, VSpace(0),
                            End,

                            Child, VGroup,
                                Child, temp.EMail = oqstring(NULL,128,Msg_Edit_UserEMail,0),
                                Child, HGroup,
                                    Child, NListviewObject,
                                        MUIA_CycleChain, TRUE,
                                        MUIA_NListview_NList, temp.EMails = NListObject,
                                            MUIA_ContextMenu,         NULL,
                                            MUIA_NList_MultiSelect,   MUIV_NList_MultiSelect_Default,
                                            MUIA_List_DragSortable,   TRUE,
                                            MUIA_NList_ConstructHook, &emCon_hook,
                                            MUIA_NList_DestructHook,  &emDes_hook,
                                            MUIA_NList_DisplayHook,   &emDisp_hook,
                                        End,
                                    End,
                                    Child, VGroup,
                                        MUIA_Weight, 0,
                                        Child, temp.EMailAdd  = obutton(Msg_Edit_UserEMailAdd,0),
                                        Child, temp.EMailRem  = obutton(Msg_Edit_UserEMailRem,0),
                                        Child, temp.EMailUp   = obutton(Msg_Edit_UserEMailUp,0),
                                        Child, temp.EMailDown = obutton(Msg_Edit_UserEMailDown,0),
                                        Child, VSpace(0),
                                    End,
                                End,
                            End,
                        End,
                    End,
                End,

                /* Home */
                Child, RootScrollgroupObject,
                    MUIA_Scrollgroup_FreeHoriz, FALSE,
                    MUIA_Scrollgroup_Contents, VirtgroupObject,
                        MUIA_Frame, MUIV_Frame_Virtual,
                        Child, ColGroup(2),

                            Child, ofreelabel2(Msg_Edit_UserAddress),
                            Child, temp.Address = osscroll(128,Msg_Edit_UserAddress,0),

                            Child, olabel2(Msg_Edit_UserCity),
                            Child, HGroup,
                                Child, temp.City = ostring(128,Msg_Edit_UserCity,0),
                                Child, olabel2(Msg_Edit_UserZIP),
                                Child, temp.ZIP = ostring(20,Msg_Edit_UserZIP,0),
                            End,

                            Child, olabel2(Msg_Edit_UserCountry),
                            Child, HGroup,
                                MUIA_Group_Spacing, 1,
                                Child, temp.Country = ostring(60,Msg_Edit_UserCountry,0),
                                Child, getMap = TextObject,
                                    MUIA_CycleChain,    TRUE,
                                    MUIA_Font,          MUIV_Font_Button,
                                    MUIA_InputMode,     MUIV_InputMode_RelVerify,
                                    ButtonFrame,
                                    MUIA_Background,    MUII_ButtonBack,
                                    MUIA_Text_Contents, CM_GetString(Msg_Edit_UserGetMap),
                                    MUIA_ControlChar,   getKeyCharID(Msg_Edit_UserGetMap),
                                    MUIA_Text_HiCharIdx,(ULONG)'_',
                                    MUIA_Text_PreParse, MUIX_C,
                                    MUIA_Text_SetMax,   TRUE,
                                End,
                            End,

                            Child, olabel2(Msg_Edit_UserPhone),
                            Child, HGroup,
                                Child, temp.Phone = ostring(40,Msg_Edit_UserPhone,0),
                                Child, olabel2(Msg_Edit_UserMobile),
                                Child, temp.Mobile = ostring(40,Msg_Edit_UserMobile,0),
                            End,

                            Child, olabel2(Msg_Edit_UserFax),
                            Child, HGroup,
                                Child, temp.Fax = ostring(40,Msg_Edit_UserFax,0),
                                Child, olabel2(Msg_Edit_UserICQ),
                                Child, temp.ICQ = ostring(20,Msg_Edit_UserICQ,0),
                            End,

                            Child, olabel2(Msg_Edit_UserWWW),
                            Child, HGroup,
                                MUIA_Group_Spacing, 1,
                                Child, temp.WWW = ostring(256,Msg_Edit_UserWWW,0),
                                Child, goUrl = MUI_MakeObject(MUIO_PopButton,MUII_PopUp),
                            End,
                            Child, olabel2(Msg_Edit_UserFTP),
                            Child, temp.FTP = ostring(256,Msg_Edit_UserFTP,0),
                        End,
                    End,
                End,

                /* Business */
                Child, RootScrollgroupObject,
                    MUIA_Scrollgroup_FreeHoriz, FALSE,
                    MUIA_Scrollgroup_Contents, VirtgroupObject,
                        MUIA_Frame, MUIV_Frame_Virtual,
                        Child, ColGroup(2),

                            Child, olabel2(Msg_Edit_UserCompany),
                            Child, temp.Company = ostring(40,Msg_Edit_UserCompany,0),

                            Child, olabel2(Msg_Edit_UserJob),
                            Child, temp.BJobTitle = ostring(20,Msg_Edit_UserJob,0),

                            Child, olabel2(Msg_Edit_UserDept),
                            Child, temp.BDepartment = ostring(40,Msg_Edit_UserDept,0),

                            Child, olabel2(Msg_Edit_UserOffice),
                            Child, temp.BOffice = ostring(40,Msg_Edit_UserOffice,0),

                            Child, olabel2(Msg_Edit_UserOffEMail),
                            Child, temp.BEMail = ostring(128,Msg_Edit_UserOffEMail,0),

                            Child, ofreelabel2(Msg_Edit_UserOffAddress),
                            Child, temp.BAddress = osscroll(128,Msg_Edit_UserOffAddress,0),

                            Child, olabel2(Msg_Edit_UserOffCity),
                            Child, HGroup,
                                Child, temp.BCity = ostring(128,Msg_Edit_UserOffCity,0),
                                Child, olabel2(Msg_Edit_UserOffZIP),
                                Child, temp.BZIP = ostring(20,Msg_Edit_UserOffZIP,0),
                            End,

                            Child, olabel2(Msg_Edit_UserOffCountry),
                            Child, HGroup,
                                Child, temp.BCountry = ostring(60,Msg_Edit_UserOffCountry,0),
                                Child, olabel2(Msg_Edit_UserOffMobile),
                                Child, temp.BMobile = ostring(40,Msg_Edit_UserOffMobile,0),
                            End,

                            Child, olabel2(Msg_Edit_UserOffPhone),
                            Child, HGroup,
                                Child, temp.BPhone = ostring(40,Msg_Edit_UserOffPhone,0),
                                Child, olabel2(Msg_Edit_UserOffFax),
                                Child, temp.BFax = ostring(40,Msg_Edit_UserOffFax,0),
                            End,
                        End,
                    End,
                End,

                /* Notes */
                Child, temp.Comment = osscroll(512,0,0),

                /* Image */
                Child, VGroup,
                    Child, RootScrollgroupObject,
                        MUIA_Scrollgroup_Contents, VirtgroupObject,
                            MUIA_Frame, MUIV_Frame_Virtual,
                            Child, temp.ImagePic = PicObject, End,
                        End,
                    End,
                    Child, HGroup,
                        Child, olabel2(Msg_Edit_UserImage),
                        Child, ofile(&temp.ImageFile,256,Msg_Edit_UserImage,0,FALSE),
                        Child, olabel1(Msg_Edit_UserImageTransparent),
                        Child, temp.ImageTrans = ocheck(Msg_Edit_UserImageTransparent,0),
                    End,
                End,

            End,
            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->CMObj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList);

        set(temp.ZIP,MUIA_Weight,60);
        set(temp.Alias,MUIA_Weight,60);
        set(temp.PGP,MUIA_Weight,60);
        set(temp.Sex,MUIA_Weight,0);
        set(temp.VisualName,MUIA_VNList_Edit,obj);
        set(temp.EMail,MUIA_QString_AttachedList,temp.EMails);

        DoMethod(temp.WWW,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,3,MUIM_Edit_URL,temp.WWW,URL_WWW);
        DoMethod(temp.FTP,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,3,MUIM_Edit_URL,temp.FTP,URL_FTP);
        DoMethod(temp.Phone,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,2,MUIM_Edit_Phone,temp.Phone);
        DoMethod(temp.Fax,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,2,MUIM_Edit_Phone,temp.Fax);
        DoMethod(temp.ImageFile,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,temp.ImagePic,3,MUIM_Set,MUIA_Pic_File,MUIV_TriggerValue);
        DoMethod(temp.ImageTrans,MUIM_Notify,MUIA_Selected,MUIV_EveryTime,temp.ImagePic,3,MUIM_Set,MUIA_Pic_Transparent,MUIV_TriggerValue);

        DoMethod(temp.Name,MUIM_Notify,MUIA_String_Contents,MUIV_EveryTime,obj,3,MUIM_Edit_ChangeVisualName,MUIV_Edit_ChangeVisualName_Name,MUIV_TriggerValue);
        DoMethod(temp.LastName,MUIM_Notify,MUIA_String_Contents,MUIV_EveryTime,obj,3,MUIM_Edit_ChangeVisualName,MUIV_Edit_ChangeVisualName_LastName,MUIV_TriggerValue);

        DoMethod(temp.EMailAdd,MUIM_Notify,MUIA_Pressed,FALSE,temp.EMails,3,MUIM_NList_InsertSingle,(APTR)(-1),MUIV_NList_Insert_Bottom);
        DoMethod(temp.EMailAdd,MUIM_Notify,MUIA_Pressed,FALSE,temp.EMails,3,MUIM_Set,MUIA_NList_Active,MUIV_NList_Active_Bottom);
        DoMethod(temp.EMailAdd,MUIM_Notify,MUIA_Pressed,FALSE,MUIV_Notify_Window,3,MUIM_Set,MUIA_Window_ActiveObject,temp.EMail);
        DoMethod(temp.EMailRem,MUIM_Notify,MUIA_Pressed,FALSE,temp.EMails,3,MUIM_NList_Remove,MUIV_NList_Remove_Selected);
        DoMethod(temp.EMailUp,MUIM_Notify,MUIA_Pressed,FALSE,temp.EMails,3,MUIM_NList_Move,MUIV_NList_Move_Active,MUIV_NList_Move_Previous);
        DoMethod(temp.EMailDown,MUIM_Notify,MUIA_Pressed,FALSE,temp.EMails,3,MUIM_NList_Move,MUIV_NList_Move_Active,MUIV_NList_Move_Next);
        DoMethod(temp.EMails,MUIM_Notify,MUIA_NList_Active,MUIV_EveryTime,obj,1,MUIM_Edit_ChangeEMail);
        DoMethod(temp.EMail,MUIM_Notify,MUIA_Textinput_Contents,MUIV_EveryTime,obj,1,MUIM_Edit_EMailChange);

        if (OpenURLBase)
        {
            DoMethod(goUrl,MUIM_Notify,MUIA_Pressed,TRUE,obj,2,MUIM_Edit_GoURL,data->WWW);
            DoMethod(getMap,MUIM_Notify,MUIA_Pressed,TRUE,obj,1,MUIM_Edit_GetMap);
        }
        else
        {
            set(goUrl,MUIA_ShowMe,0);
            set(getMap,MUIA_ShowMe,0);
        }
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register struct TagItem      *tag;

    if (tag = FindTagItem(MUIA_Edit_Entry,msg->ops_AttrList))
    {
        struct CMEMail *email, *succ;
        ULONG          p;

        data->Current = CMUSER(tag->ti_Data);

        /* Name */
        set(data->Name,MUIA_Textinput_Contents,data->Current->Name);
        set(data->LastName,MUIA_Textinput_Contents,data->Current->LastName);
        set(data->Alias,MUIA_Textinput_Contents,data->Current->Alias);
        set(data->Sex,MUIA_Cycle_Active,data->Current->Sex);
        set(data->Birthday,MUIA_Birthday_Use,data->Current->Flags & CMUSERFLG_Birthday);
        set(data->Birthday,MUIA_Birthday_Date,data->Current->Birthday);
        set(data->Anniversary,MUIA_Birthday_Use,data->Current->Flags & CMUSERFLG_Anniversary);
        set(data->Anniversary,MUIA_Birthday_Date,data->Current->Anniversary);
        set(data->PGP,MUIA_Textinput_Contents,data->Current->PGPUserID);

        if (*data->Current->VisualName) set(data->VisualName,MUIA_Textinput_Contents,data->Current->VisualName);
        else
            if (*data->Current->Name)
                if (*data->Current->LastName)
                {
                    REGARRAY UBYTE buf[256];

                    sprintf(buf,"%s %s",data->Current->Name,data->Current->LastName);
                    set(data->VisualName,MUIA_Textinput_Contents,buf);
                }
                else set(data->VisualName,MUIA_Textinput_Contents,data->Current->Name);
            else
                if (*data->Current->LastName) set(data->VisualName,MUIA_Textinput_Contents,data->Current->LastName);
                else if (*data->Current->Alias) set(data->VisualName,MUIA_Textinput_Contents,data->Current->Alias);


        DoMethod(data->EMails,MUIM_NList_Clear);
        DoMethod(data->EMails,MUIM_NList_InsertSingle,(APTR)(-1),MUIV_NList_Insert_Top);
        get(data->EMails,MUIA_NList_InsertPosition,&p);
        DoMethod(data->EMails,MUIM_NList_GetEntry,p,&email);
        strcpy(email->EMail,data->Current->EMail);
        DoMethod(data->EMails,MUIM_NList_Redraw,p);

        for (email = CMEMAIL(data->Current->EMails.mlh_Head); succ = email->Succ; email = succ)
            DoMethod(data->EMails,MUIM_NList_InsertSingle,email,MUIV_NList_Insert_Bottom);

        set(data->EMails,MUIA_NList_Active,MUIV_NList_Active_Top);

        /* Home */
        set(data->Address,MUIA_Textinput_Contents,data->Current->Address);
        set(data->City,MUIA_Textinput_Contents,data->Current->City);
        set(data->Country,MUIA_Textinput_Contents,data->Current->Country);
        set(data->ZIP,MUIA_Textinput_Contents,data->Current->ZIP);
        set(data->Phone,MUIA_Textinput_Contents,data->Current->Phone);
        set(data->Mobile,MUIA_Textinput_Contents,data->Current->Mobile);
        set(data->Fax,MUIA_Textinput_Contents,data->Current->Fax);
        set(data->WWW,MUIA_Textinput_Contents,data->Current->WWW);
        set(data->FTP,MUIA_Textinput_Contents,data->Current->FTP);
        set(data->ICQ,MUIA_Textinput_Contents,data->Current->ICQ);

        /* Office */
        set(data->Company,MUIA_Textinput_Contents,data->Current->Company);
        set(data->BJobTitle,MUIA_Textinput_Contents,data->Current->BJobTitle);
        set(data->BOffice,MUIA_Textinput_Contents,data->Current->BOffice);
        set(data->BAddress,MUIA_Textinput_Contents,data->Current->BAddress);
        set(data->BCity,MUIA_Textinput_Contents,data->Current->BCity);
        set(data->BCountry,MUIA_Textinput_Contents,data->Current->BCountry);
        set(data->BZIP,MUIA_Textinput_Contents,data->Current->BZIP);
        set(data->BDepartment,MUIA_Textinput_Contents,data->Current->BDepartment);
        set(data->BPhone,MUIA_Textinput_Contents,data->Current->BPhone);
        set(data->BMobile,MUIA_Textinput_Contents,data->Current->BMobile);
        set(data->BFax,MUIA_Textinput_Contents,data->Current->BFax);
        set(data->BEMail,MUIA_Textinput_Contents,data->Current->BEMail);

        /* Note */
        set(data->Comment,MUIA_Textinput_Contents,data->Current->Comment);

        /* Image */
        set(data->ImageFile,MUIA_Textinput_Contents,data->Current->ImageFile);
        set(data->ImageTrans,MUIA_Selected,data->Current->Flags & CMUSERFLG_Transparent);
        set(data->ImagePic,MUIA_Pic_File,data->Current->ImageFile);
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mGet(struct IClass *cl,Object *obj,struct opGet *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch (msg->opg_AttrID)
    {
        case MUIA_Edit_Entry:                *msg->opg_Storage = (ULONG)data->Current; return TRUE;
        case MUIA_CManager_ActiveEditObject: *msg->opg_Storage = (ULONG)data->Name; return TRUE;

        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static ULONG
mUpdate(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register struct CMUser       *entry;
    register struct CMEMail      *email, *succ;
    STRPTR                       v;
    register int                 i;

    entry = data->Current;

    /* Name */
    getstr(data->Name,entry->Name);
    getstr(data->LastName,entry->LastName);
    getstr(data->Address,entry->Address);
    getstr(data->Alias,entry->Alias);
    getstr(data->PGP,entry->PGPUserID);

    entry->Sex = xget(data->Sex,MUIA_Cycle_Active);

    entry->Birthday = xget(data->Birthday,MUIA_Birthday_Date);
    if (xget(data->Birthday,MUIA_Birthday_Use)) entry->Flags |= CMUSERFLG_Birthday;
    else entry->Flags &= ~CMUSERFLG_Birthday;

    entry->Anniversary = xget(data->Anniversary,MUIA_Birthday_Date);
    if (xget(data->Anniversary,MUIA_Birthday_Use)) entry->Flags |= CMUSERFLG_Anniversary;
    else entry->Flags &= ~CMUSERFLG_Anniversary;

    *data->Current->VisualName = 0;
    get(data->VisualName,MUIA_Textinput_Contents,&v);
    if (*v) strcpy(data->Current->VisualName,v);
    else
        if (*data->Current->Name)
            if (*data->Current->LastName) sprintf(data->Current->VisualName,"%s %s",data->Current->Name,data->Current->LastName);
            else strcpy(data->Current->VisualName,data->Current->Name);
        else
            if (*data->Current->LastName) strcpy(data->Current->VisualName,data->Current->LastName);
            else if (*data->Current->Alias) strcpy(data->Current->VisualName,data->Current->Alias);

    set(data->EMails,MUIA_NList_DestructHook,NULL);

    for (email = CMEMAIL(data->Current->EMails.mlh_Head); succ = email->Succ; email = succ)
        CM_FreeObject(email);

    QUICKNEWLIST(&data->Current->EMails);

    *data->Current->EMail = 0;
    for (i = 0; ;i++)
    {
        struct CMEMail *entry;

        DoMethod(data->EMails,MUIM_NList_GetEntry,i,&entry);
        if (!entry) break;

        if (i==0) strcpy(data->Current->EMail,entry->EMail);
        else AddTail((struct List *)&data->Current->EMails,(struct Node *)entry);
    }

    /* Home */
    getstr(data->City,entry->City);
    getstr(data->Country,entry->Country);
    getstr(data->ZIP,entry->ZIP);
    getstr(data->Phone,entry->Phone);
    getstr(data->Mobile,entry->Mobile);
    getstr(data->Fax,entry->Fax);
    getstr(data->WWW,entry->WWW);
    getstr(data->FTP,entry->FTP);
    getstr(data->ICQ,entry->ICQ);

    /* Office */
    getstr(data->Company,entry->Company);
    getstr(data->BJobTitle,entry->BJobTitle);
    getstr(data->BOffice,entry->BOffice);
    getstr(data->BAddress,entry->BAddress);
    getstr(data->BCity,entry->BCity);
    getstr(data->BCountry,entry->BCountry);
    getstr(data->BZIP,entry->BZIP);
    getstr(data->BDepartment,entry->BDepartment);
    getstr(data->BPhone,entry->BPhone);
    getstr(data->BMobile,entry->BMobile);
    getstr(data->BFax,entry->BFax);
    getstr(data->BEMail,entry->BEMail);

    /* Note */
    getstr(data->Comment,entry->Comment);

    /* Image */
    getstr(data->ImageFile,entry->ImageFile);
    if (xget(data->ImageTrans,MUIA_Selected)) entry->Flags |= CMUSERFLG_Transparent;
    else entry->Flags &= ~CMUSERFLG_Transparent;

    return 0;
}

/***********************************************************************/

static ULONG
mPhone(struct IClass *cl,Object *obj,struct MUIP_Edit_Phone *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    REGARRAY UBYTE       country[60];

    if (getstr(data->Country,country))
    {
        struct CMPrefs *prefs;

        get(data->CMObj,MUIA_CManager_Prefs,&prefs);

        if (stricmp(country,prefs->Country))
        {
            register struct CMCountry *mstate, *succ;

            for (mstate = (struct CMCountry *)lib_countries.mlh_Head; succ = mstate->Succ; mstate = succ)
                if (!stricmp(country,mstate->Country) || !stricmp(country,mstate->Country2))
                {
                    REGARRAY UBYTE buf1[64];

                    getstr(msg->Gadget,buf1);

                    if (*buf1)
                    {
                        REGARRAY UBYTE prefix[12];

                        sprintf(prefix,"+%s",mstate->Code);

                        if (strncmp(buf1,prefix,strlen(prefix)))
                        {
                            REGARRAY UBYTE buf2[60];

                            sprintf(buf2,"%s %s",prefix,buf1);
                            set(msg->Gadget, MUIA_Textinput_Contents,buf2);
                        }
                    }

                    break;
                }
        }
    }

    return 0;
}

/***********************************************************************/

static ULONG
mChangeEMail(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    LONG                 a;

    get(data->EMails,MUIA_NList_Active,&a);

    set(data->EMailUp,MUIA_Disabled,a<=0);
    set(data->EMailRem,MUIA_Disabled,a<0);
    set(data->EMail,MUIA_Disabled,a<0);

    if (a<0)
    {
        set(data->EMailDown,MUIA_Disabled,TRUE);
        SetAttrs(data->EMail,MUIA_Disabled,TRUE,MUIA_Textinput_Contents,NULL,TAG_DONE);
    }
    else
    {
        struct CMEMail *entry;
        LONG           e;

        DoMethod(data->EMails,MUIM_NList_GetEntry,a,&entry);

        SetAttrs(data->EMail,MUIA_Disabled,FALSE,MUIA_Textinput_Contents,entry->EMail,TAG_DONE);

        get(data->EMails,MUIA_NList_Entries,&e);
        set(data->EMailDown,MUIA_Disabled,a==e-1);
    }

    return 0;
}

/***********************************************************************/

static ULONG
mEMailChange(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);
    struct CMEMail       *entry;

    DoMethod(data->EMails,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
    if (entry)
    {
        getstr(data->EMail,entry->EMail);
        DoMethod(data->EMails,MUIM_NList_Redraw,MUIV_NList_Redraw_Active);
    }

    return 0;
}

/***********************************************************************/

static ULONG
mEditURL(struct IClass *cl,Object *obj,struct MUIP_Edit_URL *msg)
{
    return editURLFun(msg->URL,msg->Type);
}

/***********************************************************************/

static ULONG
mEditGetVisualInfo(struct IClass *cl,Object *obj,struct MUIP_Edit_GetVisualInfo *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    get(data->VisualName,MUIA_Textinput_Contents,&msg->info->VisualName);
    get(data->Name,MUIA_Textinput_Contents,&msg->info->Name);
    get(data->LastName,MUIA_Textinput_Contents,&msg->info->LastName);
    get(data->Alias,MUIA_Textinput_Contents,&msg->info->Alias);

    return 0;
}

/***********************************************************************/

static ULONG
mEditChangeVisualName(struct IClass *cl,Object *obj,struct MUIP_Edit_ChangeVisualName *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch(msg->what)
    {
        case MUIV_Edit_ChangeVisualName_Name:
        {
            STRPTR l;

            get(data->LastName,MUIA_Textinput_Contents,&l);
            if (*msg->val)
            {
                REGARRAY UBYTE buf[256];

                sprintf(buf,"%s %s",msg->val,l);
                set(data->VisualName,MUIA_Textinput_Contents,buf);
            }
            else
            {
                if (*l) set(data->VisualName,MUIA_Textinput_Contents,l);
            }
            break;
        }

        case MUIV_Edit_ChangeVisualName_LastName:
        {
            STRPTR n;

            get(data->Name,MUIA_Textinput_Contents,&n);
            if (*msg->val)
            {
                REGARRAY UBYTE buf[256];

                sprintf(buf,"%s %s",n,msg->val);
                set(data->VisualName,MUIA_Textinput_Contents,buf);
            }
            else
            {
                if (*n) set(data->VisualName,MUIA_Textinput_Contents,n);
            }

            break;
        }
    }

    return 0;
}

/***********************************************************************/

static ULONG
mEditGoURL(struct IClass *cl,Object *obj,struct MUIP_Edit_GoURL *msg)
{
    return editGoURLFun((STRPTR)xget(msg->URL,MUIA_Textinput_Contents));
}

/***********************************************************************/

static ULONG
mEditGetMap(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    return getMap((STRPTR)xget(data->Address,MUIA_Textinput_Contents),
                  (STRPTR)xget(data->Country,MUIA_Textinput_Contents),
                  (STRPTR)xget(data->City,MUIA_Textinput_Contents),
                  (STRPTR)xget(data->ZIP,MUIA_Textinput_Contents));
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:                     return mNew(cl,obj,(APTR)msg);
        case OM_SET:                     return mSets(cl,obj,(APTR)msg);
        case OM_GET:                     return mGet(cl,obj,(APTR)msg);

        case MUIM_Edit_Update:           return mUpdate(cl,obj,(APTR)msg);
        case MUIM_Edit_URL:              return mEditURL(cl,obj,(APTR)msg);
        case MUIM_Edit_Phone:            return mPhone(cl,obj,(APTR)msg);
        case MUIM_Edit_GetVisualInfo:    return mEditGetVisualInfo(cl,obj,(APTR)msg);
        case MUIM_Edit_ChangeVisualName: return mEditChangeVisualName(cl,obj,(APTR)msg);
        case MUIM_Edit_ChangeEMail:      return mChangeEMail(cl,obj,(APTR)msg);
        case MUIM_Edit_EMailChange:      return mEMailChange(cl,obj,(APTR)msg);
        case MUIM_Edit_GoURL:            return mEditGoURL(cl,obj,(APTR)msg);
        case MUIM_Edit_GetMap:           return mEditGetMap(cl,obj,(APTR)msg);

        default:                         return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initEditUser(void)
{
    if ((!VNPopClass && !initVNPop()) ||
        (!birthdayClass && !initBirthday()) ||
       !(lib_EditUser = MUI_CreateCustomClass(NULL,MUIC_Group,NULL,sizeof(struct data),DISP(dispatcher))))
    {
        if (VNPopClass)
        {
            if (birthdayClass) disposeBirthday();
            disposeVNPop();
        }

        return FALSE;
    }

    localizeArray(regLabels,regLabelsIDs);
    localizeArray(sexLabels,sexLabelsIDs);

    loadCountries();

    return TRUE;
}

/***********************************************************************/

void
disposeEditUser(void)
{
    disposeVNPop();
    disposeBirthday();
    MUI_DeleteCustomClass(lib_EditUser);
    lib_EditUser = NULL;
}

/***********************************************************************/

