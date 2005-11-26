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
    Object *App;
    Object *CMObj;

    Object *String;

    Object *Users;
    Object *WWW;
    Object *FTP;
    Object *Chat;
    Object *Telnet;

    Object *CaseSensitive;
    Object *Wildcards;
    Object *Comment;

    Object *Status;
};

/***********************************************************************/

enum
{
    SEARCH_CURRENT,
    SEARCH_TOP,
    SEARCH_BACKWARDS
};

struct EntryStore
{
    struct EntryStore *Succ;
    struct EntryStore *Pred;
    struct CMGroup    *Entry;
};

enum
{
    SFLG_User       = 1<<0,
    SFLG_WWW        = 1<<1,
    SFLG_FTP        = 1<<2,
    SFLG_Chat       = 1<<3,
    SFLG_Telnet     = 1<<4,
    SFLG_Case       = 1<<5,
    SFLG_Wildcards  = 1<<6,
    SFLG_Comment    = 1<<7,
};

/***********************************************************************/

static ULONG
mNew(struct IClass *cl,Object *obj,struct opSet *msg)
{
    struct data     temp;
    register Object *search, *Top, *Back, *Cancel;

    memset(&temp,0,sizeof(temp));

    if (obj = (Object *)DoSuperNew(cl,obj,
            MUIA_HelpNode,         "CMSEARCH",
            MUIA_Window_Title,     CM_GetString(Msg_SearchWin_Title),
            MUIA_Window_ID,        MAKE_ID('c','m','s','r'),

            WindowContents, VGroup,

                Child, VGroup,

                    Child, HGroup,
                        Child, olabel2(Msg_SearchWin_Pattern),
                        Child, temp.String = ostring(128,Msg_SearchWin_Pattern,Msg_SearchWin_PatternHelp),
                    End,

                    Child, HGroup,

                        /* Options */
                        Child, HGroup,
                            GroupFrameT(CM_GetString(Msg_SearchWin_OptionsTitle)),
                            Child, HSpace(0),
                            Child, ColGroup(2),
                                Child, temp.CaseSensitive = ocheck(Msg_SearchWin_Case,Msg_SearchWin_CaseHelp),
                                Child, ollabel1(Msg_SearchWin_Case),
                                Child, temp.Wildcards = ocheck(Msg_SearchWin_Wildcards,Msg_SearchWin_WildcardsHelp),
                                Child, ollabel1(Msg_SearchWin_Wildcards),
                                Child, temp.Comment = ocheck(Msg_SearchWin_Comment,Msg_SearchWin_CommentHelp),
                                Child, ollabel1(Msg_SearchWin_Comment),
                            End,
                            Child, HSpace(0),
                        End,

                        /* search in */
                        Child, HGroup,
                            GroupFrameT(CM_GetString(Msg_SearchWin_SearchInTitle)),
                            Child, HSpace(0),
                            Child, ColGroup(5),
                                Child, temp.Users = ocheck(Msg_SearchWin_Users,Msg_SearchWin_UsersHelp),
                                Child, ollabel1(Msg_SearchWin_Users),
                                Child, HSpace(0),
                                Child, temp.WWW = ocheck(Msg_SearchWin_WWW,Msg_SearchWin_WWWHelp),
                                Child, ollabel1(Msg_SearchWin_WWW),

                                Child, temp.FTP = ocheck(Msg_SearchWin_FTP,Msg_SearchWin_FTPHelp),
                                Child, ollabel1(Msg_SearchWin_FTP),
                                Child, HSpace(0),
                                Child, temp.Chat = ocheck(Msg_SearchWin_Chat,Msg_SearchWin_ChatHelp),
                                Child, ollabel1(Msg_SearchWin_Chat),

                                Child, temp.Telnet = ocheck(Msg_SearchWin_Telnet,Msg_SearchWin_TelnetHelp),
                                Child, ollabel1(Msg_SearchWin_Telnet),
                                Child, HSpace(0),
                                Child, HSpace(0),
                                Child, HSpace(0),
                            End,
                            Child, HSpace(0),
                        End,

                    End,
                End,

                Child, temp.Status = TextObject,
                    MUIA_Frame,         MUIV_Frame_Text,
                    MUIA_Background,    MUII_TextBack,
                    MUIA_Text_PreParse, MUIX_C,
                    MUIA_Text_Contents, CM_GetString(Msg_SearchWin_Welcome),
                End,

                /* Buttons */
                Child, HGroup,
                    Child, search = obutton(Msg_SearchWin_Search,Msg_SearchWin_SearchHelp),
                    Child, wspace(25),
                    Child, Top    = obutton(Msg_SearchWin_First,Msg_SearchWin_FirstHelp),
                    Child, wspace(25),
                    Child, Back   = obutton(Msg_SearchWin_Previous,Msg_SearchWin_PreviousHelp),
                    Child, wspace(25),
                    Child, Cancel = obutton(Msg_SearchWin_Cancel,Msg_SearchWin_CancelHelp),
                End,
            End,
            TAG_MORE,msg->ops_AttrList))
    {
        register struct data *data = INST_DATA(cl,obj);

        *data = temp;

        data->App   = (Object *)GetTagData(MUIA_CManager_AppObj,NULL,msg->ops_AttrList);
        data->CMObj = (Object *)GetTagData(MUIA_CManager_CMObj,NULL,msg->ops_AttrList);

        if (GetTagData(MUIA_SearchWindow_HideUsers,FALSE,msg->ops_AttrList))
            set(temp.Users,MUIA_Disabled,TRUE);
        else set(temp.Users,MUIA_Selected,TRUE);

        if (GetTagData(MUIA_SearchWindow_HideWWW,FALSE,msg->ops_AttrList))
            set(temp.WWW,MUIA_Disabled,TRUE);
        else set(temp.WWW,MUIA_Selected,TRUE);

        if (GetTagData(MUIA_SearchWindow_HideFTP,FALSE,msg->ops_AttrList))
            set(temp.FTP,MUIA_Disabled,TRUE);
        else set(temp.FTP,MUIA_Selected,TRUE);

        if (GetTagData(MUIA_SearchWindow_HideChat,FALSE,msg->ops_AttrList))
            set(temp.Chat,MUIA_Disabled,TRUE);
        else set(temp.Chat,MUIA_Selected,TRUE);

        if (GetTagData(MUIA_SearchWindow_HideTelnet,FALSE,msg->ops_AttrList))
            set(temp.Telnet,MUIA_Disabled,TRUE);
        else set(temp.Telnet,MUIA_Selected,TRUE);

        set(temp.String,MUIA_Textinput_ResetMarkOnCursor,TRUE);
        set(temp.Comment,MUIA_Selected,TRUE);

        DoSuperMethod(cl,obj,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,obj,3,MUIM_Set,MUIA_Window_Open,FALSE);

        DoMethod(temp.String,MUIM_Notify,MUIA_Textinput_Acknowledge,MUIV_EveryTime,obj,2,MUIM_SearchWindow_Search,SEARCH_TOP);
        DoMethod(search,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_SearchWindow_Search,SEARCH_CURRENT);
        DoMethod(Top,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_SearchWindow_Search,SEARCH_TOP);
        DoMethod(Back,MUIM_Notify,MUIA_Pressed,FALSE,obj,2,MUIM_SearchWindow_Search,SEARCH_BACKWARDS);
        DoMethod(Cancel,MUIM_Notify,MUIA_Pressed,FALSE,obj,3,MUIM_Set,MUIA_Window_Open,FALSE);
    }

    return (ULONG)obj;
}

/***********************************************************************/

static ULONG
mGet( struct IClass *cl, Object *obj, struct opGet *msg)
{
    switch (msg->opg_AttrID)
    {
        case MUIA_CManager_IsCMWin: *msg->opg_Storage = TRUE; return TRUE;
        default: return DoSuperMethodA(cl,obj,(Msg)msg);
    }
}

/***********************************************************************/

static  ULONG
mSets( struct IClass *cl, Object *obj, struct opSet *msg)
{
    register struct data *data = INST_DATA(cl,obj);
    register ULONG       res;

    res = DoSuperMethodA(cl,obj,(Msg)msg);

    if (GetTagData(MUIA_Window_Open,FALSE,msg->ops_AttrList))
    {
        set(data->Status,MUIA_Text_Contents,CM_GetString(Msg_SearchWin_Welcome));
        DoMethod(data->String,MUIM_Textinput_DoMarkAll);
        SetSuperAttrs(cl,obj,MUIA_Window_ActiveObject,data->String,TAG_DONE);
    }

    return res;
}

/***********************************************************************/

static ULONG
mClose( struct IClass *cl, Object *obj, Msg msg)
{
    register struct data *data = INST_DATA(cl,obj);

    SetSuperAttrs(cl,obj,MUIA_Window_Open,FALSE,TAG_DONE);
    DoMethod(data->App,MUIM_Application_PushMethod,data->CMObj,2,MUIM_CManager_DisposeObj,obj);

    return 0;
}

/***********************************************************************/

static STRPTR
mystrstr(STRPTR s1,STRPTR s2,ULONG caseSensitive)
{
    register ULONG len2 = strlen(s2);
    register int   c2 = caseSensitive ? *s2 : ToLower(*s2);

    if (!len2) return s1;

    for (; *s1; s1++)
    {
        register int c1 = caseSensitive ? *s1 : ToLower(*s1);

        if (c2==c1 && !(caseSensitive ? strncmp(s1,s2,len2) : strnicmp(s1,s2,len2)))
            return s1;
    }

    return NULL;
}

/***********************************************************************/

static ULONG INLINE
checkString(STRPTR patt,STRPTR string,ULONG flags)
{
    if (flags & SFLG_Wildcards) return (ULONG)((flags & SFLG_Case) ? MatchPattern(patt,string) : MatchPatternNoCase(patt,string));
    return (ULONG)mystrstr(string,patt,flags & SFLG_Case);
}

/***********************************************************************/

static ULONG
search(ULONG Type,STRPTR patt,struct EntryStore **StartGroup,struct CMEntry **StartEntry,ULONG flags)
{
    register struct EntryStore *group;
    register struct CMEntry    *entry;

    group = *StartGroup;
    entry = *StartEntry;

    while ((Type==SEARCH_BACKWARDS) ? group->Pred : group->Succ)
    {
        if (entry)
            for ( ; (Type==SEARCH_BACKWARDS) ? entry->Prec : entry->Succ; entry = (Type==SEARCH_BACKWARDS) ? entry->Prec : entry->Succ)
            {
                register ULONG found = FALSE;

                switch (entry->Type)
                {
                    case CMEntry_User:
                        found = checkString(patt,CMUSER(entry)->Name,flags) ||
                                checkString(patt,CMUSER(entry)->LastName,flags) ||
                                checkString(patt,CMUSER(entry)->VisualName,flags) ||
                                checkString(patt,CMUSER(entry)->EMail,flags) ||
                                checkString(patt,CMUSER(entry)->Address,flags) ||
                                checkString(patt,CMUSER(entry)->City,flags) ||
                                checkString(patt,CMUSER(entry)->ZIP,flags) ||
                                checkString(patt,CMUSER(entry)->Phone,flags) ||
                                checkString(patt,CMUSER(entry)->Fax,flags) ||
                                checkString(patt,CMUSER(entry)->WWW,flags) ||
                                checkString(patt,CMUSER(entry)->FTP,flags) ||
                                ((flags & SFLG_Comment) ? checkString(patt,CMUSER(entry)->Comment,flags) : 0);
                         break;

                    case CMEntry_WWW:
                        found = checkString(patt,CMWWW(entry)->Name,flags) ||
                                checkString(patt,CMWWW(entry)->WWW,flags) ||
                                ((flags & SFLG_Comment) ? checkString(patt,CMWWW(entry)->Comment,flags) : 0);
                        break;

                    case CMEntry_FTP:
                        found = checkString(patt,CMFTP(entry)->Name,flags) ||
                                checkString(patt,CMFTP(entry)->FTP,flags) ||
                                ((flags & SFLG_Comment) ? checkString(patt,CMFTP(entry)->Comment,flags) : 0);
                        break;

                    case CMEntry_Chat:
                        found = checkString(patt,CMCHAT(entry)->Channel,flags) ||
                                checkString(patt,CMCHAT(entry)->Server,flags) ||
                                checkString(patt,CMCHAT(entry)->Maintainer,flags) ||
                                checkString(patt,CMCHAT(entry)->Nick,flags) ||
                                checkString(patt,CMCHAT(entry)->WWW,flags) ||
                                ((flags & SFLG_Comment) ? checkString(patt,CMCHAT(entry)->Comment,flags) : 0);
                        break;

                    case CMEntry_Telnet:
                        found = checkString(patt,CMTELNET(entry)->Host,flags) ||
                                ((flags & SFLG_Comment) ? checkString(patt,CMTELNET(entry)->Comment,flags) : 0);
                        break;
                }

                if (found)
                {
                    *StartGroup = group;
                    *StartEntry = entry;

                    return TRUE;
                }
            }

            group = (Type==SEARCH_BACKWARDS) ? group->Pred : group->Succ;

            if ((Type==SEARCH_BACKWARDS) ? group->Pred : group->Succ)
                if (Type==SEARCH_BACKWARDS) entry = CMENTRY(group->Entry->Entries.mlh_TailPred);
                else entry = CMENTRY(group->Entry->Entries.mlh_Head);
    }

    return FALSE;
}

/***********************************************************************/

static struct MUI_NListtree_TreeNode *
findGroup(Object *Tree, struct CMGroup *Group, APTR Node)
{
    register struct MUI_NListtree_TreeNode *tn;
    register ULONG                         i = 0;

    while (tn = (struct MUI_NListtree_TreeNode *)
           DoMethod(Tree,MUIM_NListtree_GetEntry,Node,i++,MUIV_NListtree_GetEntry_Flag_SameLevel))
    {
        register struct MUI_NListtree_TreeNode *tn2;
        register ULONG                         i2 = 0;

        if (CMGROUP(tn->tn_User)==Group) return tn;

        while (tn2 = (struct MUI_NListtree_TreeNode *)DoMethod(Tree,MUIM_NListtree_GetEntry,tn,i2++,MUIV_NListtree_GetEntry_Flag_SameLevel))
        {
            register struct MUI_NListtree_TreeNode *ret;
            register struct CMGroup                *group2;

            group2 = CMGROUP(tn2->tn_User);

            if (group2 == Group)return tn2;

            if (ret = findGroup(Tree,Group,tn2)) return ret;
        }
    }

    return NULL;
}

/***********************************************************************/

static ULONG
findItem(Object *List,APTR Entry)
{
    register ULONG i;

    for (i = 0;; i++)
    {
        struct CMGroup *node;

        DoMethod(List,MUIM_NList_GetEntry,i,&node);
        if (node == Entry) break;
    }

    return i;
}

/***********************************************************************/

static void
addToList(Object *Tree,struct MinList *List,APTR Node,ULONG Flags,struct CMData *cm)
{
    register struct MUI_NListtree_TreeNode *tn;
    register ULONG                         i = 0;

    while (tn = (struct MUI_NListtree_TreeNode *)
           DoMethod(Tree,MUIM_NListtree_GetEntry,Node,i++,MUIV_NListtree_GetEntry_Flag_SameLevel))
    {
        register struct MUI_NListtree_TreeNode *tn2;
        register struct EntryStore             *entry;
        register ULONG                         i2 = 0;

        if (((APTR)tn->tn_User == cm->Users) || ((APTR)tn->tn_User == cm->WWWs ) || ((APTR)tn->tn_User == cm->FTPs ) || ((APTR)tn->tn_User == cm->Chat))
            if (!(((Flags & SFLG_User) && ((APTR)tn->tn_User == cm->Users)) ||
                 ((Flags & SFLG_WWW ) && ((APTR)tn->tn_User == cm->WWWs )) ||
                 ((Flags & SFLG_FTP ) && ((APTR)tn->tn_User == cm->FTPs )) ||
                 ((Flags & SFLG_Chat) && ((APTR)tn->tn_User == cm->Chat ))) ||
                 ((Flags & SFLG_Telnet) && ((APTR)tn->tn_User == cm->Telnet ))) continue;

        if (!(entry = allocArbitratePooled(sizeof(struct EntryStore)))) break;

        entry->Entry = CMGROUP(tn->tn_User);

        AddTail((struct List *)List,(struct Node *)entry);

        while (tn2 = (struct MUI_NListtree_TreeNode *)DoMethod(Tree,MUIM_NListtree_GetEntry,tn,i2++,MUIV_NListtree_GetEntry_Flag_SameLevel))
        {
            register struct EntryStore *entry2;

            if (entry2 = allocArbitratePooled(sizeof(struct EntryStore)))
            {
                entry2->Entry = CMGROUP(tn2->tn_User);
                AddTail((struct List *)List,(struct Node *)entry2);
                addToList(Tree,List,tn2,Flags,cm);
            }
        }
    }
}

/***********************************************************************/

static ULONG
mSearch(struct IClass *cl,Object *obj,struct MUIP_SearchWindow_Search *msg)
{
    register struct data                   *data = INST_DATA(cl,obj);
    register struct MUI_NListtree_TreeNode *tn;
    struct CMEntry                         *entry = NULL;
    REGARRAY UBYTE                         String[128], patt[sizeof(String)*2+2];
    register ULONG                         flags;
    struct EntryStore                      *start;
    struct MinList                         groups;
    Object                                 *Tree, *List;
    ULONG                                  v;

    flags = 0;
    if (xget(data->Users,MUIA_Selected))  flags |= SFLG_User;
    if (xget(data->WWW,MUIA_Selected))    flags |= SFLG_WWW;
    if (xget(data->FTP,MUIA_Selected))    flags |= SFLG_FTP;
    if (xget(data->Chat,MUIA_Selected))   flags |= SFLG_Chat;
    if (xget(data->Telnet,MUIA_Selected)) flags |= SFLG_Telnet;
    if (!flags) return 0;

    DoMethod(data->CMObj,MUIM_CManager_GrabLists);

    get(data->CMObj,MUIA_CManager_TreeObject,&Tree);
    get(data->CMObj,MUIA_CManager_ListObject,&List);

    NEWLIST(&groups);

    addToList(Tree,&groups,NULL,flags,(struct CMData *)xget(data->CMObj,MUIA_CManager_CMData));

    if ((msg->Type==SEARCH_CURRENT) || (msg->Type==SEARCH_BACKWARDS))
    {
        register ULONG getcurrent = FALSE;

        start = (msg->Type==SEARCH_CURRENT) ?
                (struct EntryStore *)groups.mlh_Head : (struct EntryStore *)groups.mlh_TailPred;

        if (tn = (struct MUI_NListtree_TreeNode *)xget(Tree,MUIA_NListtree_Active))
        {
            if (msg->Type==SEARCH_CURRENT)
            {
                register struct EntryStore *tmp;

                for (tmp = start; tmp->Succ; tmp = tmp->Succ)
                    if (tmp->Entry==CMGROUP(tn->tn_User))
                    {
                        start      = tmp;
                        getcurrent = TRUE;

                        break;
                    }
            }
            else
            {
                register struct EntryStore *tmp;

                for (tmp = start; tmp->Pred; tmp = tmp->Pred)
                    if (tmp->Entry == CMGROUP(tn->tn_User))
                    {
                        start      = tmp;
                        getcurrent = TRUE;

                        break;
                    }
            }
        }

        if (getcurrent)
        {
            DoMethod(List,MUIM_NList_GetEntry,MUIV_NList_GetEntry_Active,&entry);

            if (msg->Type==SEARCH_BACKWARDS)
            {
                if (entry) entry = entry->Prec;

                if (!entry || !entry->Prec)
                    if (start!=(struct EntryStore *)groups.mlh_Head)
                    {
                        start = start->Pred;
                        entry = CMENTRY(start->Entry->Entries.mlh_TailPred);
                    }
            }
            else
            {
                if (entry) entry = entry->Succ;

                if (!entry || !entry->Succ)
                    if (start->Succ->Succ)
                    {
                        start = start->Succ;
                        entry = CMENTRY(start->Entry->Entries.mlh_Head);
                    }
            }
        }
        else
        {
            if (msg->Type==SEARCH_BACKWARDS) entry = CMENTRY(start->Entry->Entries.mlh_TailPred);
            else entry = CMENTRY(start->Entry->Entries.mlh_Head);
        }
    }

    if ((msg->Type==SEARCH_TOP) || (start==NULL))
    {
        start = (struct EntryStore *)groups.mlh_Head;

        if (msg->Type==SEARCH_BACKWARDS) entry = CMENTRY(start->Entry->Entries.mlh_TailPred);
        else entry = CMENTRY(start->Entry->Entries.mlh_Head);
    }

    getstr(data->String,String);
    if (!String[0]) return 0;

    set(data->Status,MUIA_Text_Contents,CM_GetString(Msg_SearchWin_Searching));

    if (xget(data->CaseSensitive,MUIA_Selected)) flags |= SFLG_Case;
    if (xget(data->Wildcards,MUIA_Selected))
    {
        flags |= SFLG_Wildcards;

        if (flags & SFLG_Case) ParsePattern(String,patt,sizeof(patt));
        else ParsePatternNoCase(String,patt,sizeof(patt));
    }
    else strcpy(patt,String);
    if (xget(data->Comment,MUIA_Selected)) flags |= SFLG_Comment;

    if (search(msg->Type,patt,&start,&entry,flags))
    {
        register struct MUI_NListtree_TreeNode *tn, *tn2;
        register LONG                          active;

        set(List,MUIA_NList_Quiet,TRUE);

        tn2 = tn = findGroup(Tree,start->Entry,NULL);
        while (tn2 = (struct MUI_NListtree_TreeNode *)DoMethod(Tree,MUIM_NListtree_GetEntry,tn2,MUIV_NListtree_GetEntry_Position_Parent,0))
            DoMethod(Tree,MUIM_NListtree_Open,tn2,tn2,0);

        set(Tree,MUIA_NListtree_Active,tn);
        set(List,MUIA_NList_Active,active = findItem(List,entry));
        set(List,MUIA_NList_Quiet,FALSE);
        DoMethod(List,MUIM_NList_Jump,active);

        switch (entry->Type)
        {
            case CMEntry_User:
                v = Msg_Users;
                break;

            case CMEntry_WWW:
                v = Msg_WWW;
                break;

            case CMEntry_FTP:
                v = Msg_FTP;
                break;

            case CMEntry_Chat:
                v = Msg_Chat;
                break;

            case CMEntry_Telnet:
                v = Msg_Telnet;
                break;

            default:
                v = 0;
                break;
        }

        sprintf(patt,CM_GetString(Msg_SearchWin_Found),CM_GetString(v));
        set(data->Status,MUIA_Text_Contents,patt);
    }
    else set(data->Status,MUIA_Text_Contents,CM_GetString(Msg_SearchWin_NotFound));

    while (start = (struct EntryStore *)RemTail((struct List *)&groups))
        freeArbitratePooled(start,sizeof(struct EntryStore));

    return 0;
}

/***********************************************************************/

M_DISP(dispatcher)
{
    M_DISPSTART

    switch(msg->MethodID)
    {
        case OM_NEW:                   return mNew(cl,obj,(APTR)msg);
        case OM_GET:                   return mGet(cl,obj,(APTR)msg);
        case OM_SET:                   return mSets(cl,obj,(APTR)msg);

        case MUIM_CManager_WinClose:   return mClose(cl,obj,(APTR)msg);
        case MUIM_SearchWindow_Search: return mSearch(cl,obj,(APTR)msg);

        default:                       return DoSuperMethodA(cl,obj,msg);
    }
}

M_DISPEND(dispatcher)

/***********************************************************************/

ULONG
initSearchWindow(void)
{
    return (ULONG)(lib_SearchWindow = MUI_CreateCustomClass(NULL,NULL,lib_MatherWin,sizeof(struct data),DISP(dispatcher)));
}

/***********************************************************************/

void
disposeSearchWindow(void)
{
    MUI_DeleteCustomClass(lib_SearchWindow);
    lib_SearchWindow = NULL;
}

/***********************************************************************/
