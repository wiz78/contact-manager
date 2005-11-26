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

struct data
{
    Object                        *CMObj;
    struct CMData                 *CMData;

    struct MUI_NListtree_TreeNode *Active;

    struct MUI_NListtree_TreeNode *DrNode;   // Destination node
    struct MUI_NListtree_TreeNode *DrRoot;   // Source node root
    struct CMGroup                *DrGroup;  // Source node root group
    ULONG                         DrValid;   // Valid D&D
    ULONG                         DrType;    // D&D Type
    ULONG                         DrFlags;   // D&D Flags

    UBYTE                         Name[80];

#ifdef __MORPHOS__
    struct EmulLibEntry           DispTrap;
#endif
    struct Hook                   DispHook;

    struct CMPrefs                *Prefs;

    APTR                          ContextMenu;

    Object                        *ImagesObjects[8];
    APTR                          Images[8];
    UBYTE                         Names[128];

    ULONG                         Flags;
};

/* DrFlags */
enum
{
    DRFLG_Self = 1<<0,
};

/* Flags */
enum
{
    FLG_Setup          = 1<<0,
    FLG_BoldSections   = 1<<1,
    FLG_EmptyNodes     = 1<<2,
    FLG_SectionsImages = 1<<3,
    FLG_GroupsImages   = 1<<4,

    FLG_Users          = 1<<5,
    FLG_WWWs           = 1<<6,
    FLG_FTPs           = 1<<7,
    FLG_Chats          = 1<<8,
    FLG_Telnets        = 1<<9,
};

/***********************************************************************/

#ifdef __MORPHOS__
static ULONG DispFun(void)
{
    struct Hook                          *hook = (struct Hook *)REG_A0;
    //Object                               *obj  = (Object *)REG_A2;
    struct MUIP_NListtree_DisplayMessage *msg  = (struct MUIP_NListtree_DisplayMessage *)REG_A1;
#else
static ULONG SAVEDS ASM DispFun(REG(a0,struct Hook *hook),REG(a2,Object *obj),REG(a1,struct MUIP_NListtree_DisplayMessage *msg))
{
#endif

    register struct data    *data = hook->h_Data;
    register struct CMGroup *entry = CMGROUP(msg->TreeNode->tn_User);
    register APTR           image = NULL;

    switch (entry->Type)
    {
        case CMEntry_Group:
            image = data->Images[0];
            break;

        case CMEntry_Section:
            if (entry==data->CMData->Users) image = data->Images[1];
            else if (entry==data->CMData->WWWs) image = data->Images[2];
                 else if (entry==data->CMData->FTPs) image = data->Images[3];
                      else if (entry==data->CMData->Chat) image = data->Images[4];
                           else if (entry==data->CMData->Telnet) image = data->Images[5];
            break;
    }

    if (image)
    {
        sprintf(data->Names,"\33O[%08lx] %s",(ULONG)image,entry->Name);
        *msg->Array = data->Names;
    }
    else *msg->Array = entry->Name;

    if ((data->Flags & FLG_BoldSections) && (entry->Type==CMEntry_Section)) *msg->Preparse = MUIX_B;

    return 0;
}

/***************************************************************************/

M_HOOK(treeDest,Object *obj,struct MUIP_NListtree_DestructMessage *msg)
{
    register struct CMEntry *sub;
    register struct CMGroup *entry = CMGROUP(msg->UserData);

    while (sub = CMENTRY(RemTail((struct List *)&entry->Entries))) CM_FreeEntry(sub);

    if (entry->Type!=CMEntry_Section)
    {
        Remove((struct Node *)entry);
        CM_FreeEntry(entry);
    }

    return 0;
}

/***************************************************************************/

M_HOOK(treeOpen,Object *obj,struct MUIP_NListtree_OpenMessage *msg)
{
    register struct CMGroup *entry;

    if (msg->TreeNode && (entry = CMGROUP(msg->TreeNode->tn_User)))
    {
        entry->Flags |= CMGROUPFLG_Open;
    }

    return 0;
}
/***************************************************************************/

M_HOOK(treeClose,Object *obj,struct MUIP_NListtree_CloseMessage *msg)
{
    register struct CMGroup *entry;

    if (msg->TreeNode && (entry = CMGROUP(msg->TreeNode->tn_User)))
    {
        entry->Flags &= ~CMGROUPFLG_Open;
    }

    return 0;
}

/***************************************************************************/

M_HOOK(treeCompare,Object *obj,struct MUIP_NListtree_CompareMessage *msg)
{
    return StrnCmp(lib_locale,CMGROUP(msg->TreeNode1->tn_User)->Name,CMGROUP(msg->TreeNode2->tn_User)->Name,-1,SC_COLLATE2);
}

/***************************************************************************/

static ULONG MenuIDs[] =
{
    Msg_Groups_Menu_Title,
    	Msg_Groups_Menu_OpenAll,
    	Msg_Groups_Menu_CloseAll,
    	0,
    	Msg_Groups_Menu_Sort,
    	0,
    	Msg_Menu_Configure
};

static struct NewMenu Menu[] =
{
    MTITLE(Msg_Groups_Menu_Title),
        MITEM(Msg_Groups_Menu_OpenAll),
        MITEM(Msg_Groups_Menu_CloseAll),
        MBAR,
        MITEM(Msg_Groups_Menu_Sort),
        MBAR,
        MITEM(Msg_Menu_Configure),
    MEND
};

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_ObjectID,               MAKE_ID('c','m','t','r'),
            MUIA_Frame,                  (lib_flags & BASEFLG_GroupsInputFrame) ? MUIV_Frame_InputList : MUIV_Frame_ReadList,
            MUIA_ContextMenu,            TRUE,
            MUIA_NListtree_CompareHook,  MUIV_NListtree_CompareHook_LeavesBottom,
            MUIA_NListtree_DragDropSort, TRUE,
            MUIA_NListtree_DupNodeName,  FALSE,
            MUIA_NListtree_FindNameHook, MUIV_NListtree_FindNameHook_CaseSensitive,
            TAG_MORE, msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        data->Active = MUIV_NListtree_Active_Off;

#ifdef __MORPHOS__
        data->DispTrap.Trap      = TRAP_LIB;
        data->DispTrap.Extension = 0;
        data->DispTrap.Func      = (APTR)DispFun;

        data->DispHook.h_Entry   = (HOOKFUNC)&data->DispTrap;
#else
        data->DispHook.h_Entry = (HOOKFUNC)DispFun;
#endif
        data->DispHook.h_Data  = data;

        SetSuperAttrs(cl,obj,MUIA_NListtree_DisplayHook,  &data->DispHook,
                             MUIA_NListtree_DestructHook, &treeDest_hook,
                             MUIA_NListtree_OpenHook,     &treeOpen_hook,
                             MUIA_NListtree_CloseHook,    &treeClose_hook,
                             TAG_DONE);

        data->ContextMenu = MUI_MakeObject(MUIO_MenustripNM,Menu,0);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mDispose(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->ContextMenu) MUI_DisposeObject(data->ContextMenu);

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static
void disposeGroupsImages(struct IClass *cl,Object *obj,struct data *data)
{
    if (!(data->Flags & FLG_Setup)) return;

    if (data->ImagesObjects[0])
    {
        if (data->Images[0])
        {
            DoSuperMethod(cl,obj,MUIM_NList_DeleteImage,data->Images[0]);
            data->Images[0] = NULL;
        }

        MUI_DisposeObject(data->ImagesObjects[0]);
        data->ImagesObjects[0] = NULL;
    }
}

/***********************************************************************/

static
void createGroupsImages(struct IClass *cl,Object *obj,struct data *data)
{
    REGARRAY UBYTE imagePath[256];

    if (!(data->Flags & FLG_Setup) || !(data->Flags & FLG_GroupsImages))
        return;

    strcpy(imagePath,lib_CMPath);
    AddPart(imagePath,DEF_ImagePath,sizeof(imagePath));

    if (data->Flags & FLG_GroupsImages)
    {
        AddPart(imagePath,"GROUPs",sizeof(imagePath));
        if (data->ImagesObjects[0] = PicObject,MUIA_Pic_File,imagePath,MUIA_Pic_Transparent,TRUE,MUIA_Pic_Preload,TRUE,End)
        {
            if (!(data->Images[0] = (APTR)DoSuperMethod(cl,obj,MUIM_NList_CreateImage,data->ImagesObjects[0],0L)))
            {
                MUI_DisposeObject(data->ImagesObjects[0]);
                data->ImagesObjects[0] = NULL;
            }
        }

        *PathPart(imagePath) = 0;
    }
}

/***********************************************************************/

static
void disposeSectionsImages(struct IClass *cl,Object *obj,struct data *data)
{
    register int i;

    if (!(data->Flags & FLG_Setup)) return;

    for (i = 1; i<8; i++)
    {
        if (data->ImagesObjects[i])
        {
            if (data->Images[i])
            {
                DoSuperMethod(cl,obj,MUIM_NList_DeleteImage,data->Images[i]);
                data->Images[i] = NULL;
            }

            MUI_DisposeObject(data->ImagesObjects[i]);
            data->ImagesObjects[i] = NULL;
        }
    }
}

/***********************************************************************/

static
void createSectionsImages(struct IClass *cl,Object *obj,struct data *data)
{
    REGARRAY UBYTE imagePath[256];

    if (!(data->Flags & FLG_Setup) || !(data->Flags & FLG_SectionsImages))
        return;

    strcpy(imagePath,lib_CMPath);
    AddPart(imagePath,DEF_ImagePath,sizeof(imagePath));

    if (data->Flags & FLG_SectionsImages)
    {
        /* Users */
        if (data->Flags & FLG_Users)
        {
            AddPart(imagePath,"USERs",sizeof(imagePath));
            if (data->ImagesObjects[1] = PicObject,MUIA_Pic_File,imagePath,MUIA_Pic_Transparent,TRUE,MUIA_Pic_Preload,TRUE,End)
            {
                if (!(data->Images[1] = (APTR)DoSuperMethod(cl,obj,MUIM_NList_CreateImage,data->ImagesObjects[1],0L)))
                {
                    MUI_DisposeObject(data->ImagesObjects[1]);
                    data->ImagesObjects[1] = NULL;
                }
            }

            *PathPart(imagePath) = 0;
        }

        /* WWW */
        if (data->Flags & FLG_WWWs)
        {
            AddPart(imagePath,"WWWs",sizeof(imagePath));
            if (data->ImagesObjects[2] = PicObject,MUIA_Pic_File,imagePath,MUIA_Pic_Transparent,TRUE,MUIA_Pic_Preload,TRUE,End)
            {
                if (!(data->Images[2] = (APTR)DoSuperMethod(cl,obj,MUIM_NList_CreateImage,data->ImagesObjects[2],0L)))
                {
                    MUI_DisposeObject(data->ImagesObjects[2]);
                    data->ImagesObjects[2] = NULL;
                }
            }

            *PathPart(imagePath) = 0;
        }

        /* FTP */
        if (data->Flags & FLG_FTPs)
        {
            AddPart(imagePath,"FTPs",sizeof(imagePath));
            if (data->ImagesObjects[3] = PicObject,MUIA_Pic_File,imagePath,MUIA_Pic_Transparent,TRUE,MUIA_Pic_Preload,TRUE,End)
            {
                if (!(data->Images[3] = (APTR)DoSuperMethod(cl,obj,MUIM_NList_CreateImage,data->ImagesObjects[3],0L)))
                {
                    MUI_DisposeObject(data->ImagesObjects[3]);
                    data->ImagesObjects[3] = NULL;
                }
            }

            *PathPart(imagePath) = 0;
        }

        /* Chat */
        if (data->Flags & FLG_Chats)
        {
            AddPart(imagePath,"CHATs",sizeof(imagePath));
            if (data->ImagesObjects[4] = PicObject,MUIA_Pic_File,imagePath,MUIA_Pic_Transparent,TRUE,MUIA_Pic_Preload,TRUE,End)
            {
                if (!(data->Images[4] = (APTR)DoSuperMethod(cl,obj,MUIM_NList_CreateImage,data->ImagesObjects[4],0L)))
                {
                    MUI_DisposeObject(data->ImagesObjects[4]);
                    data->ImagesObjects[4] = NULL;
                }
            }

            *PathPart(imagePath) = 0;
        }

        /* Telnet */
        if (data->Flags & FLG_Telnets)
        {
            AddPart(imagePath,"TELNETs",sizeof(imagePath));
            if (data->ImagesObjects[5] = PicObject,MUIA_Pic_File,imagePath,MUIA_Pic_Transparent,TRUE,MUIA_Pic_Preload,TRUE,End)
            {
                if (!(data->Images[5] = (APTR)DoSuperMethod(cl,obj,MUIM_NList_CreateImage,data->ImagesObjects[5],0L)))
                {
                    MUI_DisposeObject(data->ImagesObjects[5]);
                    data->ImagesObjects[5] = NULL;
                }
            }
        }
    }
}

/***********************************************************************/

static ULONG
mSetup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (!DoSuperMethodA(cl,obj,msg)) return FALSE;

    data->Flags |= FLG_Setup;

    createGroupsImages(cl,obj,data);
    createSectionsImages(cl,obj,data);

    return TRUE;
}

/***********************************************************************/

static ULONG
mCleanup(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    disposeSectionsImages(cl,obj,data);
    disposeGroupsImages(cl,obj,data);

    data->Flags &= ~FLG_Setup;

    return DoSuperMethodA(cl,obj,msg);
}

/***********************************************************************/

static ULONG
mSets(struct IClass *cl,Object *obj,struct opSet *msg)
{
    register struct data    *data = INST_DATA(cl,obj);
    struct TagItem          *tags;
    register struct TagItem *tag;

    if (freeStack()<MINSTACKSIZE)
        return 0;

    for (tags = msg->ops_AttrList; tag = NextTagItem(&tags); )
    {
        register ULONG tidata = tag->ti_Data;

        switch (tag->ti_Tag)
        {
            case MUIA_GroupList_CMData:
                data->CMData = (struct CMData *)tidata;
                break;

            case MUIA_CManager_CMObj:
                data->CMObj = (Object *)tidata;
                break;

            case MUIA_CManager_Prefs:
            {
                register ULONG redraw = FALSE;

                data->Prefs = (struct CMPrefs *)tidata;

                if (!BOOLSAME(data->Flags & FLG_BoldSections,data->Prefs->Options & OPT_BoldSections))
                {
                    if (data->Prefs->Options & OPT_BoldSections) data->Flags |= FLG_BoldSections;
                    else data->Flags &= ~FLG_BoldSections;

                    redraw = TRUE;
                }

                if (!BOOLSAME(data->Flags & FLG_GroupsImages,data->Prefs->Options & OPT_GroupsImages))
                {
                    if (data->Prefs->Options & OPT_GroupsImages)
                    {
                        data->Flags |= FLG_GroupsImages;
                        createGroupsImages(cl,obj,data);
                    }
                    else
                    {
                        disposeGroupsImages(cl,obj,data);
                        data->Flags &= ~FLG_GroupsImages;
                    }
                    redraw = TRUE;
                }

                if (!BOOLSAME(data->Flags & FLG_SectionsImages,data->Prefs->Options & OPT_SectionsImages))
                {
                    if (data->Prefs->Options & OPT_SectionsImages)
                    {
                        data->Flags |= FLG_SectionsImages;
                        createSectionsImages(cl,obj,data);
                    }
                    else
                    {
                        disposeSectionsImages(cl,obj,data);
                        data->Flags &= ~FLG_SectionsImages;
                    }
                    redraw = TRUE;
                }


                if (!BOOLSAME(data->Flags & FLG_EmptyNodes,data->Prefs->Options & OPT_EmptyNodes))
                {
                    if (data->Prefs->Options & OPT_EmptyNodes) data->Flags |= FLG_EmptyNodes;
                    else data->Flags &= ~FLG_EmptyNodes;

                    SetSuperAttrs(cl,obj,MUIA_NListtree_EmptyNodes,data->Flags & FLG_EmptyNodes,TAG_DONE);
                    redraw = FALSE;
                }

                if (redraw) DoSuperMethod(cl,obj,MUIM_NListtree_Redraw,MUIV_NListtree_Redraw_All);

                break;
            }
        }
    }

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mActive(struct IClass *cl,Object *obj,struct MUIP_NListtree_Active *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    data->Active = msg->ActiveNode;

    if (!msg->ActiveNode || !msg->ActiveNode->tn_User || ((struct CMEntry *)msg->ActiveNode->tn_User)->Type==CMEntry_Section)
        set(obj,MUIA_NList_DragType,MUIV_NList_DragType_None);
    else set(obj,MUIA_NList_DragType,MUIV_NList_DragType_Immediate);

    return 0;
}

/***********************************************************************/

static struct CMGroup *
getRootGroup(struct IClass *cl,Object *obj,struct MUI_NListtree_TreeNode *tn,struct MUI_NListtree_TreeNode *tn2)
{
    register struct MUI_NListtree_TreeNode *root;

    for (root = tn; ;)
    {
        tn = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,root,MUIV_NListtree_GetEntry_Position_Parent,0);
        if (!tn || !tn->tn_User) break;

        if (tn2 && (tn==tn2))
        {
            root = NULL;
            break;
        }

        root = tn;
    }

    return CMGROUP(root ? root->tn_User : NULL);
}

/***********************************************************************/

static struct MUI_NListtree_TreeNode *
getRoot(struct IClass *cl,Object *obj,struct MUI_NListtree_TreeNode *tn)
{
    register struct MUI_NListtree_TreeNode *root;

    for (root = tn; ;)
    {
        tn = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,root,MUIV_NListtree_GetEntry_Position_Parent,0);
        if (!tn || !tn->tn_User) break;
        root = tn;
    }

    return root;
}

/***********************************************************************/

static ULONG
mDragQuery(struct IClass *cl,Object *obj,struct MUIP_DragQuery *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (msg->obj==obj)
    {
        register struct CMGroup *active = data->Active->tn_User;

        if (active->Type==CMEntry_Section) return MUIV_DragQuery_Refuse;

        data->DrRoot  = getRoot(cl,obj,data->Active);
        data->DrGroup = data->DrRoot ? data->DrRoot->tn_User : NULL;
        data->DrFlags |= DRFLG_Self;

        return DoSuperMethodA(cl,obj,(Msg)msg);
    }
    else
    {
        struct CMEntry *entry = NULL;

        DoMethod(msg->obj,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);
        if (!entry) return MUIV_DragQuery_Refuse;

        if (entry->Type==CMEntry_User) data->DrGroup = data->CMData->Users;
        else if (entry->Type==CMEntry_WWW) data->DrGroup = data->CMData->WWWs;
             else if (entry->Type==CMEntry_FTP) data->DrGroup = data->CMData->FTPs;
                  else if (entry->Type==CMEntry_Chat) data->DrGroup = data->CMData->Chat;
                       else if (entry->Type==CMEntry_Telnet) data->DrGroup = data->CMData->Telnet;
                            else return MUIV_DragQuery_Refuse;

        data->DrFlags &= ~DRFLG_Self;

        return MUIV_DragQuery_Accept;
    }
}

/***********************************************************************/

static ULONG
mDragBegin(struct IClass *cl,Object *obj,Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    data->DrNode = NULL;

    return DoSuperMethodA(cl,obj,(Msg)msg);
}

/***********************************************************************/

static ULONG
mDropType(struct IClass *cl,Object *obj,struct MUIP_NListtree_DropType *msg)
{
    register struct data                *data = INST_DATA(cl,obj);
    struct MUI_NListtree_TestPos_Result result;

    DoSuperMethod(cl,obj,MUIM_NListtree_TestPos,msg->MouseX,msg->MouseY,&result);

    if (!result.tpr_TreeNode)
    {
        *msg->Type = MUIV_NListtree_DropType_None;
        *msg->Pos += (abs(msg->MouseY-_top(obj))>abs(msg->MouseY-_bottom(obj))) ? -1 : +1;
        data->DrValid = FALSE;
        return 0;
    }

    data->DrNode = result.tpr_TreeNode;
    data->DrType = *msg->Type;

    if (data->DrFlags & DRFLG_Self)
    {
        register struct CMGroup *root = getRootGroup(cl,obj,result.tpr_TreeNode,data->Active);

        if (!root || (data->DrGroup!=root) || (data->DrNode==data->Active))
        {
            *msg->Type = MUIV_NListtree_DropType_None;
            data->DrValid = FALSE;
        }
        else
        {
            if ((data->DrNode==data->DrRoot) && (*msg->Type==MUIV_NListtree_DropType_Above))
                *msg->Type = MUIV_NListtree_DropType_Onto;

            data->DrValid = TRUE;
        }
    }
    else
    {
        if ((data->Active==result.tpr_TreeNode) || (data->DrGroup!=getRootGroup(cl,obj,result.tpr_TreeNode,NULL)))
        {
            *msg->Type = MUIV_NListtree_DropType_None;
            data->DrValid = FALSE;
        }
        else
        {
            *msg->Type = MUIV_NListtree_DropType_Onto;
            data->DrValid = TRUE;
        }
    }

    return 0;
}

/***********************************************************************/

static ULONG
mDragDrop(struct IClass *cl,Object *obj,struct MUIP_DragDrop *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->DrFlags & DRFLG_Self)
    {
        if (!data->DrValid || !data->DrNode || (data->DrGroup!=getRootGroup(cl,obj,data->DrNode,NULL)))
            return 0;

        DoSuperMethod(cl,obj,MUIM_NListtree_Open,MUIV_NListtree_Open_ListNode_Root,data->DrNode,0);

        DoSuperMethodA(cl,obj,(Msg)msg);
        set(data->CMObj,MUIA_CManager_Changed,TRUE);

        return 0;
    }
    else
    {
        register struct List *toList;
        LONG                 pos;
        register ULONG       remove;

        if (!data->DrValid || !data->DrNode || (data->Active==data->DrNode))
            return 0;

        toList = (struct List *)&(CMGROUP(data->DrNode->tn_User)->Entries);

        for (remove = FALSE, pos = MUIV_NList_NextSelected_Start; ;)
        {
            struct CMEntry *entry;

            DoMethod(msg->obj,MUIM_NList_NextSelected,&pos);
            if (pos==MUIV_NList_NextSelected_End) break;

            DoMethod(msg->obj,MUIM_NList_GetEntry,pos,&entry);
            ADDTAIL(toList,entry);

            remove = TRUE;
        }

        if (remove)
        {
            DoMethod(msg->obj,MUIM_NList_Remove,MUIV_NList_Remove_Selected);
            set(data->CMObj,MUIA_CManager_Changed,TRUE);
        }

        return 0;
    }
}

/***********************************************************************/

static ULONG
mArrow(struct IClass *cl,Object *obj,struct MUIP_GroupList_Arrow *msg)
{
    register struct data                   *data = INST_DATA(cl,obj);
    register struct MUI_NListtree_TreeNode *tn;

    if (tn = data->Active)
    {
        register struct MUI_NListtree_TreeNode *tn2;

        if (tn2 = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,tn,msg->Up ? MUIV_NListtree_GetEntry_Position_Previous : MUIV_NListtree_GetEntry_Position_Next,0))
        {
            register struct MUI_NListtree_TreeNode *a = data->Active;

            if (msg->Up)
            {
                set(obj,MUIA_NListtree_Quiet,TRUE);

                DoSuperMethod(cl,obj,MUIM_NListtree_Move,
                    MUIV_NListtree_Move_OldListNode_Active,
                    tn2,
                    MUIV_NListtree_Move_NewListNode_Active,
                    tn,
                    0);

                set(obj,MUIA_NListtree_Active,a);
                set(obj,MUIA_NListtree_Quiet,FALSE);
            }
            else
            {
                DoSuperMethod(cl,obj,MUIM_NListtree_Move,
                    MUIV_NListtree_Move_OldListNode_Active,
                    tn,
                    MUIV_NListtree_Move_NewListNode_Active,
                    tn2,
                    0);
            }

            set(data->CMObj,MUIA_CManager_Changed,TRUE);
        }
    }

    return 0;
}

/***************************************************************************/

static void
InsertSubGroups(Object *Tree,struct MUI_NListtree_TreeNode *tn,struct CMGroup *Group,ULONG gstatus)
{
    register struct CMGroup *entry;

    for (entry = CMGROUP(Group->SubGroups.mlh_Head); entry->Succ; entry = entry->Succ)
    {
        register struct MUI_NListtree_TreeNode *tn2;
        register ULONG  flags;

        if (gstatus==GRSTATUS_OPENED) flags = TNF_LIST|TNF_OPEN;
        else if (gstatus==GRSTATUS_CLOSED) flags = TNF_LIST;
             else flags = (entry->Flags & CMGROUPFLG_Open) ? (TNF_LIST|TNF_OPEN) : TNF_LIST;

        tn2 = (struct MUI_NListtree_TreeNode *)
                DoMethod(Tree, MUIM_NListtree_Insert,
                          "M",entry,tn,MUIV_NListtree_Insert_PrevNode_Tail,
                          flags);

        InsertSubGroups(Tree,tn2,entry,gstatus);
    }
}

/***************************************************************************/

static void
RebuildTreeList(struct IClass *cl,Object *obj,struct CMData *CMData,struct MinList *List,APTR Node)
{
    register struct MUI_NListtree_TreeNode *tn;
    register ULONG                         i = 0;

    NEWLIST(List);

    while (tn = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,Node,i++,MUIV_NListtree_GetEntry_Flag_SameLevel))
    {
        struct CMGroup                *group;
        struct MUI_NListtree_TreeNode *tn2;
        ULONG                         i2 = 0;

        group = CMGROUP(tn->tn_User);

        if ((group!=CMData->Users) && (group!=CMData->FTPs) && (group!=CMData->WWWs) && (group!=CMData->Chat) && (group!=CMData->Telnet))
            ADDTAIL(List,group);

        NEWLIST(&group->SubGroups);

        while (tn2 = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,tn,i2++,MUIV_NListtree_GetEntry_Flag_SameLevel))
        {
            struct CMGroup *group2;

            group2 = CMGROUP(tn2->tn_User);

            ADDTAIL(&group->SubGroups,group2);
            RebuildTreeList(cl,obj,CMData,&group2->SubGroups,tn2);
        }
    }
}

/***************************************************************************/

static ULONG
mInsertGroups(struct IClass *cl,Object *obj,struct MUIP_GroupList_InsertGroups *msg)
{
    register struct data          *data = INST_DATA(cl,obj);
    struct MUI_NListtree_TreeNode *tn, *first = NULL;
    register ULONG                gstatus = data->Prefs->GroupsStatus, flags;

    set(obj,MUIA_NListtree_Quiet,TRUE);

    data->Flags &= ~(FLG_Users|FLG_WWWs|FLG_FTPs|FLG_Chats|FLG_Telnets);

    /* Users */
    if (!(msg->Flags & MUIV_GroupList_HideUsers))
    {
        if (gstatus==GRSTATUS_OPENED) flags = TNF_LIST|TNF_OPEN;
        else if (gstatus==GRSTATUS_CLOSED) flags = TNF_LIST;
             else flags = (msg->CMData->Users->Flags & CMGROUPFLG_Open) ? (TNF_LIST|TNF_OPEN) : TNF_LIST;

        first = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_Insert,
                          "USERS",msg->CMData->Users,NULL,MUIV_NListtree_Insert_PrevNode_Tail,
                          flags);

        InsertSubGroups(obj,first,msg->CMData->Users,gstatus);

        data->Flags |= FLG_Users;
    }

    /* WWW */
    if (!(msg->Flags & MUIV_GroupList_HideWWW))
    {
        if (gstatus==GRSTATUS_OPENED) flags = TNF_LIST|TNF_OPEN;
        else if (gstatus==GRSTATUS_CLOSED) flags = TNF_LIST;
             else flags = (msg->CMData->WWWs->Flags & CMGROUPFLG_Open) ? (TNF_LIST|TNF_OPEN) : TNF_LIST;

        tn = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_Insert,
                          "WWWS",msg->CMData->WWWs,NULL,MUIV_NListtree_Insert_PrevNode_Tail,
                          flags);


        InsertSubGroups(obj,tn,msg->CMData->WWWs,gstatus);

        if (!first) first = tn;

        data->Flags |= FLG_WWWs;
    }

    /* FTP */
    if (!(msg->Flags & MUIV_GroupList_HideFTP))
    {
        if (gstatus==GRSTATUS_OPENED) flags = TNF_LIST|TNF_OPEN;
        else if (gstatus==GRSTATUS_CLOSED) flags = TNF_LIST;
             else flags = (msg->CMData->FTPs->Flags & CMGROUPFLG_Open) ? (TNF_LIST|TNF_OPEN) : TNF_LIST;

        tn = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_Insert,
                          "FTPS",msg->CMData->FTPs,NULL,MUIV_NListtree_Insert_PrevNode_Tail,
                          flags);

        InsertSubGroups(obj,tn,msg->CMData->FTPs,gstatus);

        if (!first) first = tn;

        data->Flags |= FLG_FTPs;
    }

    /* Chat */
    if (!(msg->Flags & MUIV_GroupList_HideChat))
    {
        if (gstatus==GRSTATUS_OPENED) flags = TNF_LIST|TNF_OPEN;
        else if (gstatus==GRSTATUS_CLOSED) flags = TNF_LIST;
             else flags = (msg->CMData->Chat->Flags & CMGROUPFLG_Open) ? (TNF_LIST|TNF_OPEN) : TNF_LIST;

        tn = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_Insert,
                          "CHATS",msg->CMData->Chat,NULL,MUIV_NListtree_Insert_PrevNode_Tail,
                          flags);

        InsertSubGroups(obj,tn,msg->CMData->Chat,gstatus);

        if (!first) first = tn;

        data->Flags |= FLG_Chats;
    }

    /* Telnet */
    if (!(msg->Flags & MUIV_GroupList_HideTelnet))
    {
        if (gstatus==GRSTATUS_OPENED) flags = TNF_LIST|TNF_OPEN;
        else if (gstatus==GRSTATUS_CLOSED) flags = TNF_LIST;
             else flags = (msg->CMData->Telnet->Flags & CMGROUPFLG_Open) ? (TNF_LIST|TNF_OPEN) : TNF_LIST;

        tn = (struct MUI_NListtree_TreeNode *)DoSuperMethod(cl,obj,MUIM_NListtree_Insert,
                          "TELNETS",msg->CMData->Telnet,NULL,MUIV_NListtree_Insert_PrevNode_Tail,
                          flags);

        InsertSubGroups(obj,tn,msg->CMData->Telnet,gstatus);

        if (!first) first = tn;

        data->Flags |= FLG_Telnets;
    }

    if (first)
    {
        //createGroupsImages(cl,obj,data);
        //createSectionsImages(cl,obj,data);
    }

    set(obj,MUIA_NListtree_Quiet,FALSE);

    if (first) set(obj,MUIA_NListtree_Active,first);

    return 0;
}

/***************************************************************************/

static ULONG
mRebuildTree(struct IClass *cl,Object *obj,struct MUIP_GroupList_RebuildTree *msg)
{
    register ULONG next = 0;

    if (!(msg->Flags & MUIV_RebuildTree_HideUsers))
        RebuildTreeList(cl,obj,msg->CMData,&msg->CMData->Users->SubGroups,(APTR)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,MUIV_NListtree_GetEntry_ListNode_Root,next++,MUIV_NListtree_GetEntry_Flag_SameLevel));

    if (!(msg->Flags & MUIV_RebuildTree_HideWWW))
        RebuildTreeList(cl,obj,msg->CMData,&msg->CMData->WWWs->SubGroups,(APTR)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,MUIV_NListtree_GetEntry_ListNode_Root,next++,MUIV_NListtree_GetEntry_Flag_SameLevel));

    if (!(msg->Flags & MUIV_RebuildTree_HideFTP))
        RebuildTreeList(cl,obj,msg->CMData,&msg->CMData->FTPs->SubGroups,(APTR)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,MUIV_NListtree_GetEntry_ListNode_Root,next++,MUIV_NListtree_GetEntry_Flag_SameLevel));

    if (!(msg->Flags & MUIV_RebuildTree_HideChat))
        RebuildTreeList(cl,obj,msg->CMData,&msg->CMData->Chat->SubGroups,(APTR)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,MUIV_NListtree_GetEntry_ListNode_Root,next++,MUIV_NListtree_GetEntry_Flag_SameLevel));

    if (!(msg->Flags & MUIV_RebuildTree_HideTelnet))
        RebuildTreeList(cl,obj,msg->CMData,&msg->CMData->Telnet->SubGroups,(APTR)DoSuperMethod(cl,obj,MUIM_NListtree_GetEntry,MUIV_NListtree_GetEntry_ListNode_Root,next,MUIV_NListtree_GetEntry_Flag_SameLevel));

    return 0;
}

/***************************************************************************/

static ULONG
mRebuildList(struct IClass *cl,Object *obj,struct MUIP_GroupList_RebuildList *msg)
{
    if (msg->tn && (msg->tn!=MUIV_NListtree_Active_Off))
    {
        register struct CMGroup *entry;
        register ULONG          i;

        entry = CMGROUP(msg->tn->tn_User);
        if (!entry) return 0;

        NEWLIST(&entry->Entries);

        for (i = 0; ; i++)
        {
            struct CMEntry *e;

            DoMethod(msg->List,MUIM_NList_GetEntry,i,&e);
            if (!e) break;

            ADDTAIL(&entry->Entries,e);
        }
    }

    return 0;
}

/***************************************************************************/

static ULONG
mClear(struct IClass *cl,Object *obj,Msg msg)
{
    //register struct data *data = INST_DATA(cl,obj);

    SetAttrs(obj,MUIA_NListtree_DestructHook, NULL,
                 MUIA_NListtree_Active,       MUIV_NListtree_Active_Off,
                 MUIA_NoNotify,              TRUE,
                 TAG_DONE);

    DoSuperMethod(cl,obj,MUIM_NListtree_Remove,MUIV_NListtree_Remove_ListNode_Root,
                 MUIV_NListtree_Remove_TreeNode_All,0);


    SetSuperAttrs(cl,obj,MUIA_NListtree_DestructHook,&treeDest_hook,TAG_DONE);

    return 0;
}

/***************************************************************************/

static ULONG
mGetRoot(struct IClass *cl,Object *obj,struct MUIP_GroupList_GetRoot *msg)
{
    return (ULONG)getRoot(cl,obj,msg->tn);
}

/***************************************************************************/

static ULONG
mContextMenuChoice(struct IClass *cl,Object *obj,struct MUIP_ContextMenuChoice *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    switch (muiUserData(msg->item))
    {
        case Msg_Groups_Menu_OpenAll:
        {
            register struct MUI_NListtree_TreeNode *a = data->Active;

            set(obj,MUIA_NListtree_Quiet,TRUE);
            DoSuperMethod(cl,obj,MUIM_NListtree_Open,MUIV_NListtree_Open_ListNode_Root,MUIV_NListtree_Open_TreeNode_All,0);
            set(obj,MUIA_NListtree_Active,a);
            set(obj,MUIA_NListtree_Quiet,FALSE);
            break;
        }

        case Msg_Groups_Menu_CloseAll:
        {
            register struct MUI_NListtree_TreeNode *a = getRoot(cl,obj,data->Active);

            set(obj,MUIA_NListtree_Active,a);
            set(obj,MUIA_NListtree_Quiet,TRUE);
            DoSuperMethod(cl,obj,MUIM_NListtree_Close,MUIV_NListtree_Close_ListNode_Root,MUIV_NListtree_Close_TreeNode_All,0);
            set(obj,MUIA_NListtree_Quiet,FALSE);
            break;
        }

        case Msg_Groups_Menu_Sort:
            set(obj,MUIA_NListtree_Quiet,TRUE);
            set(obj,MUIA_NListtree_CompareHook,&treeCompare_hook);

            DoSuperMethod(cl,obj,MUIM_NListtree_Sort,data->Active,0);

            set(obj,MUIA_NListtree_CompareHook,MUIV_NListtree_CompareHook_LeavesBottom);
            set(obj,MUIA_NListtree_Quiet,FALSE);
            break;

        case Msg_Menu_Configure:
            DoMethod(data->CMObj,MUIM_CManager_PrefsWindow,MUIV_PrefsWindow_ActivePage_Options);
            break;
    }

    return 0;
}

/***********************************************************************/

static ULONG
mContextMenuBuild(struct IClass *cl,Object *obj,struct MUIP_ContextMenuBuild *msg)
{
    register struct data *data = INST_DATA(cl,obj);

    if (data->Prefs->Options & OPT_RightMouseActive)
    {
        struct MUI_NListtree_TestPos_Result tp;

        DoSuperMethod(cl,obj,MUIM_NListtree_TestPos,msg->mx,msg->my,&tp);
        if (tp.tpr_TreeNode) SetSuperAttrs(cl,obj,MUIA_NListtree_Active,tp.tpr_TreeNode,TAG_DONE);
    }

    return (ULONG)data->ContextMenu;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch (msg->MethodID)
    {
        case OM_NEW:                      return mNew(cl,obj,(APTR)msg);
        case OM_DISPOSE:                  return mDispose(cl,obj,(APTR)msg);
        case OM_SET:                      return mSets(cl,obj,(APTR)msg);

        case MUIM_Setup:                  return mSetup(cl,obj,(APTR)msg);
        case MUIM_Cleanup:                return mCleanup(cl,obj,(APTR)msg);

        case MUIM_DragQuery:              return mDragQuery(cl,obj,(APTR)msg);
        case MUIM_DragBegin:              return mDragBegin(cl,obj,(APTR)msg);
        case MUIM_DragDrop:               return mDragDrop(cl,obj,(APTR)msg);
        case MUIM_ContextMenuBuild:       return mContextMenuBuild(cl,obj,(APTR)msg);
        case MUIM_ContextMenuChoice:      return mContextMenuChoice(cl,obj,(APTR)msg);

        case MUIM_NListtree_DropType:     return mDropType(cl,obj,(APTR)msg);
        case MUIM_NListtree_Active:       return mActive(cl,obj,(APTR)msg);

        case MUIM_GroupList_Arrow:        return mArrow(cl,obj,(APTR)msg);
        case MUIM_GroupList_InsertGroups: return mInsertGroups(cl,obj,(APTR)msg);
        case MUIM_GroupList_RebuildTree:  return mRebuildTree(cl,obj,(APTR)msg);
        case MUIM_GroupList_RebuildList:  return mRebuildList(cl,obj,(APTR)msg);
        case MUIM_GroupList_Clear:        return mClear(cl,obj,(APTR)msg);
        case MUIM_GroupList_GetRoot:      return mGetRoot(cl,obj,(APTR)msg);

        default:                          return DoSuperMethodA(cl,obj,msg);
    }
}
M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initGroupList(void)
{
    if (lib_GroupList = MUI_CreateCustomClass(NULL,MUIC_NListtree,NULL,sizeof(struct data),DISP(dispatcher)))
    {
        localizeMenus(Menu,MenuIDs);

        return TRUE;
    }

    return FALSE;
}

/***********************************************************************/

void
disposeGroupList(void)
{
    MUI_DeleteCustomClass(lib_GroupList);
    lib_GroupList = NULL;
}

/***********************************************************************/
