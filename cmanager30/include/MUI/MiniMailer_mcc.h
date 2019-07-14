#ifndef MINIMAILER_H
#define MINIMAILER_H

/*
**  $VER: MiniMailer.h 16.0 (3.11.2004)
**  Includes Release 16.0
**
**  (C) Copyright 2004 Alfonso [alfie] Ranieri <alforan@tin.it>
**      All rights reserved
*/

#if defined(__GNUC__)
# pragma pack(2)
#endif

#ifndef LIBRARIES_MUI_H
#include <libraries/mui.h>
#endif

/***********************************************************************/

#define MUIC_MiniMailer "MiniMailer.mcc"
#define MiniMailerObject MUI_NewObject(MUIC_MiniMailer

#define MMBASE_TAG(n) ((int)(0xFEC901C4+(n)))

/***********************************************************************/
/*
** Methos
**/

#define MUIM_MiniMailer_Send           MMBASE_TAG(0) /* v15 Msg                                    */
#define MUIM_MiniMailer_Stop           MMBASE_TAG(1) /* v15 Msg                                    */
#define MUIM_MiniMailer_LoadPrefs      MMBASE_TAG(2) /* v15 struct MUIP_MiniMailer_LoadPrefs *     */
#define MUIM_MiniMailer_SavePrefs      MMBASE_TAG(3) /* v15 struct MUIP_MiniMailer_SavePrefs *     */
#define MUIM_MiniMailer_Status         MMBASE_TAG(4) /* v15 struct MUIP_MiniMailer_Status *        */
#define MUIM_MiniMailer_Clear          MMBASE_TAG(5) /* v15 Msg                                    */
#define MUIM_MiniMailer_Translate      MMBASE_TAG(6) /* v16 struct MUIP_MiniMailer_Translate       */
#define MUIM_MiniMailer_FreeTranslated MMBASE_TAG(7) /* v16 struct MUIP_MiniMailer_FreeTranslate   */

/***********************************************************************/
/*
** Methos structures
**/

struct MUIP_MiniMailer_LoadPrefs
{
    ULONG  MethodID;
    STRPTR file;
};

struct MUIP_MiniMailer_SavePrefs
{
    ULONG  MethodID;
    STRPTR file;
};

struct MUIP_MiniMailer_Status
{
    ULONG MethodID;
    ULONG status;
    ULONG err;
};

enum
{
    MUIV_MiniMailer_Resolving,
    MUIV_MiniMailer_Connecting,
    MUIV_MiniMailer_Sending,
    MUIV_MiniMailer_Done,
};

struct MUIP_MiniMailer_Translate
{
    ULONG  MethodID;
    STRPTR text;
    STRPTR codesetName;
    ULONG  codesetNameLen;
    ULONG  flags;
};

struct MUIP_MiniMailer_FreeTranslate
{
    ULONG  MethodID;
    STRPTR text;
};

/***********************************************************************/
/*
** Attributes
*/

#define MUIA_MiniMailer_MinVer             MMBASE_TAG(0)  /* v15 [I...] ULONG            */
#define MUIA_MiniMailer_Host               MMBASE_TAG(1)  /* v15 [ISG.] STRPTR           */
#define MUIA_MiniMailer_HostPort           MMBASE_TAG(2)  /* v15 [ISG.] STRPTR           */
#define MUIA_MiniMailer_EMail              MMBASE_TAG(3)  /* v15 [ISG.] STRPTR           */
#define MUIA_MiniMailer_RealName           MMBASE_TAG(4)  /* v15 [ISG.] STRPTR           */
#define MUIA_MiniMailer_To                 MMBASE_TAG(5)  /* v15 [ISG.] STRPTR           */
#define MUIA_MiniMailer_Subject            MMBASE_TAG(6)  /* v15 [ISG.] STRPTR           */
#define MUIA_MiniMailer_Text               MMBASE_TAG(7)  /* v15 [ISG.] STRPTR           */
#define MUIA_MiniMailer_ShowPrefs          MMBASE_TAG(8)  /* v15 [ISGN] BOOL             */
#define MUIA_MiniMailer_TranslateHook      MMBASE_TAG(9)  /* v16 [ISG.] struct Hook *    */
#define MUIA_MiniMailer_FreeTranslatedHook MMBASE_TAG(10) /* v16 [ISG.] struct Hook *    */
#define MUIA_MiniMailer_TranslateObj       MMBASE_TAG(11) /* v17 [IS..] Object *         */

/***********************************************************************/
/*
** Values
*/


/***********************************************************************/
/*
** Structures
*/

struct MUIS_MiniMailer_Translate
{
    STRPTR text;
    STRPTR codesetName;
    ULONG  codesetNameLen;
    ULONG  flags;
};

/***********************************************************************/

#if defined(__GNUC__)
# pragma pack()
#endif

#endif /* MINIMAILER_H */
