/***************************************************************************
    revision             : $Id: edit.c,v 1.1.1.1 2003-01-06 15:58:35 tellini Exp $
	copyright            : (C) 1999-2003 by Simone Tellini
	email                : tellini@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/// Includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ASL_V38_NAMES_ONLY
#define INTUI_V36_NAMES_ONLY
#define CATCOMP_NUMBERS

#include <exec/types.h>                 // exec
#include <exec/memory.h>
#include <intuition/intuition.h>        // intuition
#include <dos/dos.h>                    // dos
#include <graphics/scale.h>             // graphics
#include <mui/nlistview_mcc.h>          // mui
#include <mui/nlist_mcc.h>
#include <mui/listtree_mcc.h>
#include <mui/textinput_mcc.h>
#include <datatypes/datatypes.h>        // datatypes
#include <datatypes/datatypesclass.h>
#include <datatypes/pictureclass.h>
#include <libraries/mui.h>              // libraries
#include <libraries/asl.h>
#include <clib/exec_protos.h>           // protos
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/utility_protos.h>
#include <clib/datatypes_protos.h>
#include <clib/graphics_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/intuition_pragmas.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/utility_pragmas.h>
#include <pragmas/datatypes_pragmas.h>
#include <pragmas/graphics_pragmas.h>

#include <mymacros.h>

#include "/CManager.h"
#include "/main.h"
#include "mui.h"
#include "edit.h"
#include "/prefs.h"
#include "/image.h"
#include "/locale.h"

#include "/Library/CManager_protos.h"
#include "/Library/CManager_lib_lib.h"
///
/// Prototypes
enum { URL_WWW = 0, URL_FTP };

static ULONG                Edit_New( struct IClass *, Object *, struct opSet * );
static ULONG                Edit_Dispose( struct IClass *, Object *, Msg );
static ULONG                Edit_Set( struct IClass *, Object *, struct opSet * );
static ULONG                Edit_Get( struct IClass *, Object *, struct opGet * );
static ULONG                Edit_Update( struct IClass *, Object *, Msg );
static ULONG                Edit_Phone( struct IClass *, Object *, struct MUIP_Edit_Phone * );
// static ULONG                Edit_GetImage( struct IClass *, Object *, Msg );

static ULONG                EditWWW_New( struct IClass *, Object *, struct opSet * );
static ULONG                EditWWW_Set( struct IClass *, Object *, struct opSet * );
static ULONG                EditWWW_Get( struct IClass *, Object *, struct opGet * );
static ULONG                EditWWW_Update( struct IClass *, Object *, Msg );

static ULONG                EditFTP_New( struct IClass *, Object *, struct opSet * );
static ULONG                EditFTP_Set( struct IClass *, Object *, struct opSet * );
static ULONG                EditFTP_Get( struct IClass *, Object *, struct opGet * );
static ULONG                EditFTP_Update( struct IClass *, Object *, Msg );

static ULONG                EditIRC_New( struct IClass *, Object *, struct opSet * );
static ULONG                EditIRC_Set( struct IClass *, Object *, struct opSet * );
static ULONG                EditIRC_Get( struct IClass *, Object *, struct opGet * );
static ULONG                EditIRC_Update( struct IClass *, Object *, Msg );

static ULONG                EditTelnet_New( struct IClass *, Object *, struct opSet * );
static ULONG                EditTelnet_Set( struct IClass *, Object *, struct opSet * );
static ULONG                EditTelnet_Get( struct IClass *, Object *, struct opGet * );
static ULONG                EditTelnet_Update( struct IClass *, Object *, Msg );

static ULONG                Edit_URL( struct IClass *, Object *, struct MUIP_Edit_URL * );

// static struct CMImage      *LoadImage( STRPTR, struct Screen * );

// Object *MakeImgButton( ULONG );

// extern struct Library *DataTypesBase;

#ifdef MY_DEBUG

extern void kprintf( char *, ... );

#define DBG(x)  { kprintf( "CManager.mcc - %s - ", __FUNC__ ); x; }

#else

#define DBG(x)

#endif
///
/// Data
struct MUI_CustomClass *CL_Edit;
struct MUI_CustomClass *CL_EditWWW;
struct MUI_CustomClass *CL_EditFTP;
struct MUI_CustomClass *CL_EditIRC;
struct MUI_CustomClass *CL_EditTelnet;
///


/*
/// MakeImgButton
Object *MakeImgButton( ULONG Help )
{
    Object *obj;

    obj = NewObject( CL_ImageGad->mcc_Class, NULL,
            ImageButtonFrame,
            MUIA_Background,            MUII_ButtonBack,
            MUIA_CycleChain,            1,
            MUIA_ShortHelp,             CM_GetString( Help ),
            MUIA_InputMode,             MUIV_InputMode_RelVerify,
            MUIA_FixWidth,              IMG_WIDTH,
            MUIA_FixHeight,             IMG_HEIGHT,
          End;

    return( obj );
}
///
*/
/*
/// LoadImage
static struct CMImage *LoadImage( STRPTR File, struct Screen *scr )
{
    struct CMImage *Img;

    if(!( DataTypesBase ))
        return( 0 );

    if( Img = CM_AllocEntry( CME_IMAGE )) {
        Object *dto;
        BOOL    ok = FALSE;

        if( dto = NewDTObject( File,
                               DTA_GroupID,         GID_PICTURE,
                               DTA_SourceType,      DTST_FILE,
                               PDTA_Remap,          FALSE,
                               TAG_DONE )) {
            struct BitMap          *b;
            ULONG                   w, h, colors;
            struct gpLayout         gpl;
            APTR                    creg;

            gpl.MethodID    = DTM_PROCLAYOUT;
            gpl.gpl_GInfo   = NULL;
            gpl.gpl_Initial = 1;

            DoDTMethodA( dto, NULL, NULL, (Msg) &gpl );

            GetDTAttrs( dto,
                        DTA_NominalVert,        &h,
                        DTA_NominalHoriz,       &w,
                        PDTA_BitMap,            &b,
                        PDTA_ColorRegisters,    &creg,
                        PDTA_NumColors,         &colors,
                        TAG_DONE );

            if( Img->BitMap = CM_CreateBitMap( IMG_WIDTH, IMG_HEIGHT, b->Depth, 0, NULL )) {
                struct BitScaleArgs args = { 0 };
                ULONG               creg_size;

                args.bsa_SrcWidth       = w;
                args.bsa_SrcHeight      = h;
                args.bsa_XSrcFactor     = w;
                args.bsa_YSrcFactor     = h;
                args.bsa_XDestFactor    = IMG_WIDTH;
                args.bsa_YDestFactor    = IMG_HEIGHT;
                args.bsa_SrcBitMap      = b;
                args.bsa_DestBitMap     = Img->BitMap;

                BitMapScale( &args );

                creg_size = colors * 3 * sizeof( ULONG );

                if( Img->Colors = AllocVec( creg_size, MEMF_ANY )) {

                    CopyMem( creg, Img->Colors, creg_size );

                    ok = TRUE;
                }
            }

            DisposeDTObject( dto );
        }

        if(!( ok )) {
            CM_FreeEntry( Img );
            Img = NULL;
        }
    }

    return( Img );
}
///
*/

/// Edit_Dispatcher
ULONG SAVEDS ASM Edit_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( Edit_New( cl, obj, (APTR)msg ));

        case OM_DISPOSE:
            return( Edit_Dispose( cl, obj, (APTR)msg ));

        case OM_SET:
            return( Edit_Set( cl, obj, (APTR)msg ));

        case OM_GET:
            return( Edit_Get( cl, obj, (APTR)msg ));

        case MUIM_Edit_Update:
            return( Edit_Update( cl, obj, (APTR)msg ));

        case MUIM_Edit_URL:
            return( Edit_URL( cl, obj, (APTR)msg ));

        case MUIM_Edit_Phone:
            return( Edit_Phone( cl, obj, (APTR)msg ));

//        case MUIM_Edit_GetImage:
//            return( Edit_GetImage( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// Edit_New
static ULONG Edit_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditData temp = { 0 };
    static STRPTR   Labels[] = { (STRPTR)MSG_HOME, (STRPTR)MSG_BUSINESS, (STRPTR)MSG_NOTES, NULL };
    static BOOL     FirstTime = TRUE;

    if( FirstTime ) {

        LocalizeArray( Labels );

        FirstTime = FALSE;
    }

    if( obj = ( Object * )DoSuperNew( cl, obj,

                        Child, RegisterGroup( Labels ),

                            /*                  Home                */

                            Child, ColGroup( 2 ),

/*                                Child, VGroup,
                                    Child, MakeLabel1( MSG_FIRST_NAME ),
                                    Child, MakeLabel1( MSG_LAST_NAME ),
                                    Child, VSpace ( 0 ),
                                End,
                                Child, HGroup,
                                    Child, VGroup,
                                        Child, temp.Name = MakeString( 80, MSG_FIRST_NAME, -1 ),
                                        Child, temp.LastName = MakeString( 80, MSG_LAST_NAME, -1 ),
                                        Child, VSpace( 0 ),
                                    End,
                                    Child, temp.Image = MakeImgButton( HELP_USER_IMAGE ),
                                End,
*/
                                Child, MakeLabel1( MSG_FIRST_NAME ),
                                Child, HGroup,
                                    Child, temp.Name = MakeString( 80, MSG_FIRST_NAME, -1 ),
                                    Child, MakeLabel1( MSG_LAST_NAME ),
                                    Child, temp.LastName = MakeString( 80, MSG_LAST_NAME, -1 ),
                                End,

                                Child, MakeLabel1( MSG_ADDRESS ),
                                Child, temp.Address = TextinputscrollObject,
                                        StringFrame,
                                        MUIA_CycleChain,            TRUE,
                                        MUIA_Textinput_Multiline,   TRUE,
                                        MUIA_Textinput_AutoExpand,  TRUE,
                                        MUIA_Textinput_MinVersion,  12,
                                        MUIA_Textinput_MaxLen,      128,
                                End,

                                Child, MakeLabel1( MSG_CITY ),
                                Child, HGroup,
                                    Child, temp.City = MakeString( 128, MSG_CITY, -1 ),

                                    Child, MakeLabel1( MSG_ZIP ),
                                    Child, temp.ZIP = MakeString( 20, MSG_ZIP, -1 ),
                                End,

                                Child, MakeLabel1( MSG_COUNTRY ),
                                Child, temp.Country = MakeString( 60, MSG_COUNTRY, -1 ),

                                Child, MakeLabel1( GAD_ALIAS  ),
                                Child, HGroup,
                                    Child, temp.Alias = MakeString( 40, GAD_ALIAS, -1 ),

                                    Child, MakeLabel1( GAD_PGP_ID ),
                                    Child, temp.PGP = MakeString( 80, GAD_PGP_ID, -1 ),
                                End,

                                Child, MakeLabel1( GAD_PHONE ),
                                Child, HGroup,
                                    Child, temp.Phone = MakeString( 40, GAD_PHONE, -1 ),

                                    Child, MakeLabel1( GAD_MOBILE ),
                                    Child, temp.Mobile = MakeString( 40, GAD_MOBILE, -1 ),
                                End,

                                Child, MakeLabel1( MSG_FAX ),
                                Child, HGroup,
                                    Child, temp.Fax = MakeString( 40, MSG_FAX, -1 ),

                                    Child, MakeLabel1( GAD_ICQ ),
                                    Child, temp.ICQ = MakeString( 20, GAD_ICQ, -1 ),
                                End,

                                Child, MakeLabel1( MSG_EMAIL ),
                                Child, temp.EMail = MakeString( 128, MSG_EMAIL, -1 ),

                                Child, MakeLabel1( GAD_WWW ),
                                Child, temp.WWW = MakeString( 256, GAD_WWW, -1 ),

                                Child, MakeLabel1( GAD_FTP ),
                                Child, temp.FTP = MakeString( 256, GAD_FTP, -1 ),
                            End,

                            /*                  Business                */
                            Child, ColGroup( 2 ),
                                Child, MakeLabel1( GAD_COMPANY ),
                                Child, temp.Company = MakeString( 40, GAD_COMPANY, -1 ),

                                Child, MakeLabel1( GAD_JOB_TITLE ),
                                Child, temp.BJobTitle = MakeString( 20, GAD_JOB_TITLE, -1 ),

                                Child, MakeLabel1( GAD_DEPARTMENT ),
                                Child, temp.BDepartment = MakeString( 40, GAD_DEPARTMENT, -1 ),

                                Child, MakeLabel1( GAD_OFFICE ),
                                Child, temp.BOffice = MakeString( 40, GAD_OFFICE, -1 ),

                                Child, MakeLabel1( MSG_ADDRESS ),
                                Child, temp.BAddress = TextinputscrollObject,
                                        StringFrame,
                                        MUIA_CycleChain,            TRUE,
                                        MUIA_Textinput_Multiline,   TRUE,
                                        MUIA_Textinput_AutoExpand,  TRUE,
                                        MUIA_Textinput_MinVersion,  12,
                                        MUIA_Textinput_MaxLen,      128,
                                End,

                                Child, MakeLabel1( MSG_CITY ),
                                Child, HGroup,
                                    Child, temp.BCity = MakeString( 128, MSG_CITY, -1 ),

                                    Child, MakeLabel1( MSG_ZIP ),
                                    Child, temp.BZIP = MakeString( 20, MSG_ZIP, -1 ),
                                End,

                                Child, MakeLabel1( MSG_COUNTRY ),
                                Child, HGroup,
                                    Child, temp.BCountry = MakeString( 60, MSG_COUNTRY, -1 ),

                                    Child, MakeLabel1( GAD_MOBILE ),
                                    Child, temp.BMobile = MakeString( 40, GAD_MOBILE, -1 ),
                                End,


                                Child, MakeLabel1( GAD_PHONE ),
                                Child, HGroup,
                                    Child, temp.BPhone = MakeString( 40, GAD_PHONE, -1 ),

                                    Child, MakeLabel1( MSG_FAX ),
                                    Child, temp.BFax = MakeString( 40, MSG_FAX, -1 ),
                                End,

                                Child, MakeLabel1( MSG_EMAIL ),
                                Child, temp.BEMail = MakeString( 128, MSG_EMAIL, -1 ),
                            End,

                            /*                  Notes                   */
                            Child, VGroup,
                                Child, temp.Comment = TextinputscrollObject,
                                        StringFrame,
                                        MUIA_CycleChain,            TRUE,
                                        MUIA_Textinput_Multiline,   TRUE,
                                        MUIA_Textinput_AutoExpand,  TRUE,
                                        MUIA_Textinput_MinVersion,  12,
                                        MUIA_Textinput_MaxLen,      512,
                                End,
                            End,
                        End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct EditData    *data = INST_DATA( cl, obj );

        *data = temp;

        set( temp.ZIP, MUIA_Weight, 60 );

        data->Req = MUI_AllocAslRequest( ASL_FileRequest, NULL );

        /*  Gadgets */
//        DoMethod( temp.Image, MUIM_Notify, MUIA_Pressed, FALSE, obj, 1, MUIM_Edit_GetImage );

        DoMethod( temp.WWW, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, obj, 3, MUIM_Edit_URL, temp.WWW, URL_WWW );
        DoMethod( temp.FTP, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, obj, 3, MUIM_Edit_URL, temp.FTP, URL_FTP );

        DoMethod( temp.Phone, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, obj, 2, MUIM_Edit_Phone, temp.Phone );
        DoMethod( temp.Fax,   MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, obj, 2, MUIM_Edit_Phone, temp.Fax   );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// Edit_Dispose
static ULONG Edit_Dispose( struct IClass *cl, Object *obj, Msg msg )
{
    struct EditData    *data = INST_DATA( cl, obj );

    DBG( kprintf( "enter\n" ));

    if( data->Req )
        MUI_FreeAslRequest( data->Req );

    DBG( kprintf( "exit\n" ));

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// Edit_Set
static ULONG Edit_Set( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditData    *data = INST_DATA( cl, obj );
    struct TagItem     *tag;

    if( tag = FindTagItem( MUIA_Edit_Entry, msg->ops_AttrList )) {

        data->Current = (struct CMUser *) tag->ti_Data;

        set( data->Name,        MUIA_String_Contents, data->Current->Name        );
        set( data->LastName,    MUIA_String_Contents, data->Current->LastName    );
        set( data->Address,     MUIA_String_Contents, data->Current->Address     );
        set( data->City,        MUIA_String_Contents, data->Current->City        );
        set( data->Country,     MUIA_String_Contents, data->Current->Country     );
        set( data->ZIP,         MUIA_String_Contents, data->Current->ZIP         );
        set( data->Alias,       MUIA_String_Contents, data->Current->Alias       );
        set( data->Comment,     MUIA_String_Contents, data->Current->Comment     );
        set( data->Phone,       MUIA_String_Contents, data->Current->Phone       );
        set( data->Mobile,      MUIA_String_Contents, data->Current->Mobile      );
        set( data->Fax,         MUIA_String_Contents, data->Current->Fax         );
        set( data->EMail,       MUIA_String_Contents, data->Current->EMail       );
        set( data->WWW,         MUIA_String_Contents, data->Current->WWW         );
        set( data->FTP,         MUIA_String_Contents, data->Current->FTP         );
        set( data->ICQ,         MUIA_String_Contents, data->Current->ICQ         );
        set( data->Company,     MUIA_String_Contents, data->Current->Company     );
        set( data->BJobTitle,   MUIA_String_Contents, data->Current->BJobTitle   );
        set( data->BOffice,     MUIA_String_Contents, data->Current->BOffice     );
        set( data->BAddress,    MUIA_String_Contents, data->Current->BAddress    );
        set( data->BCity,       MUIA_String_Contents, data->Current->BCity       );
        set( data->BCountry,    MUIA_String_Contents, data->Current->BCountry    );
        set( data->BZIP,        MUIA_String_Contents, data->Current->BZIP        );
        set( data->BDepartment, MUIA_String_Contents, data->Current->BDepartment );
        set( data->BPhone,      MUIA_String_Contents, data->Current->BPhone      );
        set( data->BMobile,     MUIA_String_Contents, data->Current->BMobile     );
        set( data->BFax,        MUIA_String_Contents, data->Current->BFax        );
        set( data->BEMail,      MUIA_String_Contents, data->Current->BEMail      );
        set( data->PGP,         MUIA_String_Contents, data->Current->PGPUserID   );

//        set( data->Image, MUIA_ImageGad_Image, data->Current->Image );
    }

    return( DoSuperMethodA( cl, obj, (Msg)msg ));
}
///
/// Edit_Get
static ULONG Edit_Get( struct IClass *cl, Object *obj, struct opGet *msg )
{
    struct EditData    *data = INST_DATA( cl, obj );

    switch ( msg->opg_AttrID ) {

        case MUIA_Edit_Entry:
            *msg->opg_Storage = (ULONG) data->Current;
            return( TRUE );
    }

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// Edit_Update
static ULONG Edit_Update( struct IClass *cl, Object *obj, Msg msg )
{
    struct EditData    *data = INST_DATA( cl, obj );
    struct CMUser      *entry;

    DBG( kprintf( "enter\n" ));

    entry = data->Current;

    getstr( data->Name,         entry->Name        );
    getstr( data->LastName,     entry->LastName    );
    getstr( data->Address,      entry->Address     );
    getstr( data->City,         entry->City        );
    getstr( data->Country,      entry->Country     );
    getstr( data->ZIP,          entry->ZIP         );
    getstr( data->Alias,        entry->Alias       );
    getstr( data->Comment,      entry->Comment     );
    getstr( data->Phone,        entry->Phone       );
    getstr( data->Mobile,       entry->Mobile      );
    getstr( data->Fax,          entry->Fax         );
    getstr( data->EMail,        entry->EMail       );
    getstr( data->WWW,          entry->WWW         );
    getstr( data->FTP,          entry->FTP         );
    getstr( data->ICQ,          entry->ICQ         );
    getstr( data->Company,      entry->Company     );
    getstr( data->BJobTitle,    entry->BJobTitle   );
    getstr( data->BOffice,      entry->BOffice     );
    getstr( data->BAddress,     entry->BAddress    );
    getstr( data->BCity,        entry->BCity       );
    getstr( data->BCountry,     entry->BCountry    );
    getstr( data->BZIP,         entry->BZIP        );
    getstr( data->BDepartment,  entry->BDepartment );
    getstr( data->BPhone,       entry->BPhone      );
    getstr( data->BMobile,      entry->BMobile     );
    getstr( data->BFax,         entry->BFax        );
    getstr( data->BEMail,       entry->BEMail      );
    getstr( data->PGP,          entry->PGPUserID   );

    DBG( kprintf( "exit\n" ));

    return( 0 );
}
///
/// Edit_Phone
static ULONG Edit_Phone( struct IClass *cl, Object *obj, struct MUIP_Edit_Phone *msg )
{
    struct EditData    *data = INST_DATA( cl, obj );
    TEXT                country[60];

    getstr( data->Country, country );

    if( country[0] ) {

        if( stricmp( country, Country ) != 0 ) {
            struct CMCountry   *c;

            for( c = (struct CMCountry *)Countries.mlh_Head; c->Succ; c = c->Succ )
                if(( stricmp( country, c->Country ) == 0 ) || ( stricmp( country, c->Country2 ) == 0 )) {
                    TEXT    buf1[0];

                    getstr( msg->Gadget, buf1 );

                    if( buf1[0] ) {
                        TEXT    prefix[12];

                        sprintf( prefix, "+%s", c->Code );

                        if( strncmp( buf1, prefix, strlen( prefix ))) {
                            TEXT    buf2[60];

                            sprintf( buf2, "%s %s", prefix, buf1 );

                            set( msg->Gadget, MUIA_String_Contents, buf2 );
                        }
                    }
                    break;
                }
        }
    }

    return( 0 );
}
///
/*
/// Edit_GetImage
static ULONG Edit_GetImage( struct IClass *cl, Object *obj, Msg msg )
{
    struct EditData    *data = INST_DATA( cl, obj );

    if( data->Req ) {
        Object *wnd;

        wnd = _win( obj );

        set( wnd, MUIA_Window_Sleep, TRUE );

        if( MUI_AslRequestTags( data->Req,
                                ASLFR_DoPatterns,   TRUE,
                                TAG_DONE )) {
            TEXT            File[256];
            struct Screen  *scr;

            get( wnd, MUIA_Window_Screen, &scr );

            strcpy( File, data->Req->fr_Drawer );
            AddPart( File, data->Req->fr_File, sizeof( File ));

            if( data->Current->Image )
                CM_FreeEntry( data->Current->Image );

            data->Current->Image = LoadImage( File, scr );

            set( data->Image, MUIA_ImageGad_Image, data->Current->Image );
        }

        set( wnd, MUIA_Window_Sleep, FALSE );
    }

    return( 0 );
}
///
*/

/// EditWWW_Dispatcher
ULONG SAVEDS ASM EditWWW_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( EditWWW_New( cl, obj, (APTR)msg ));

        case OM_SET:
            return( EditWWW_Set( cl, obj, (APTR)msg ));

        case OM_GET:
            return( EditWWW_Get( cl, obj, (APTR)msg ));

        case MUIM_Edit_Update:
            return( EditWWW_Update( cl, obj, (APTR)msg ));

        case MUIM_Edit_URL:
            return( Edit_URL( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// EditWWW_New
static ULONG EditWWW_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditWWWData  temp = { 0 };

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            Child, ColGroup( 2 ),

                                Child, MakeLabel1( MSG_NAME ),
                                Child, temp.Name = MakeString( 60, MSG_NAME, -1 ),

                                Child, MakeLabel1( GAD_WWW ),
                                Child, temp.WWW = MakeString( 256, GAD_WWW, -1 ),

                                Child, MakeLabel1( GAD_ALIAS  ),
                                Child, temp.Alias = MakeString( 40, GAD_ALIAS, -1 ),

                                Child, MakeLabel1( GAD_WEBMASTER ),
                                Child, temp.WebMaster = MakeString( 80, GAD_WEBMASTER, -1 ),

                                Child, MakeLabel1( MSG_EMAIL ),
                                Child, temp.EMail = MakeString( 128, MSG_EMAIL, -1 ),

                                Child, MakeLabel1( MSG_COMMENT  ),
                                Child, temp.Comment = TextinputscrollObject,
                                        StringFrame,
                                        MUIA_CycleChain,            TRUE,
                                        MUIA_Textinput_Multiline,   TRUE,
                                        MUIA_Textinput_AutoExpand,  TRUE,
                                        MUIA_Textinput_MinVersion,  12,
                                        MUIA_Textinput_MaxLen,      512,
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct EditWWWData *data = INST_DATA( cl, obj );

        *data = temp;

        /*  Gadgets */

        DoMethod( temp.WWW, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, obj, 3, MUIM_Edit_URL, temp.WWW, URL_WWW );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// EditWWW_Set
static ULONG EditWWW_Set( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditWWWData *data = INST_DATA( cl, obj );
    struct TagItem     *tag;

    if( tag = FindTagItem( MUIA_Edit_Entry, msg->ops_AttrList )) {

        data->Current = (struct CMWWW *) tag->ti_Data;

        set( data->Name,      MUIA_String_Contents, data->Current->Name      );
        set( data->Comment,   MUIA_String_Contents, data->Current->Comment   );
        set( data->WWW,       MUIA_String_Contents, data->Current->WWW       );
        set( data->Alias,     MUIA_String_Contents, data->Current->Alias     );
        set( data->WebMaster, MUIA_String_Contents, data->Current->WebMaster );
        set( data->EMail,     MUIA_String_Contents, data->Current->EMail     );
    }

    return( DoSuperMethodA( cl, obj, (Msg)msg ));
}
///
/// EditWWW_Get
static ULONG EditWWW_Get( struct IClass *cl, Object *obj, struct opGet *msg )
{
    struct EditWWWData *data = INST_DATA( cl, obj );

    switch ( msg->opg_AttrID ) {

        case MUIA_Edit_Entry:
            *msg->opg_Storage = (ULONG) data->Current;
            return( TRUE );
    }

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// EditWWW_Update
static ULONG EditWWW_Update( struct IClass *cl, Object *obj, Msg msg )
{
    struct EditWWWData *data = INST_DATA( cl, obj );
    struct CMWWW       *entry;

    entry = data->Current;

    getstr( data->Name,      entry->Name      );
    getstr( data->Comment,   entry->Comment   );
    getstr( data->WWW,       entry->WWW       );
    getstr( data->WebMaster, entry->WebMaster );
    getstr( data->EMail,     entry->EMail     );
    getstr( data->Alias,     entry->Alias     );

    return( 0 );
}
///

/// EditFTP_Dispatcher
ULONG SAVEDS ASM EditFTP_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( EditFTP_New( cl, obj, (APTR)msg ));

        case OM_SET:
            return( EditFTP_Set( cl, obj, (APTR)msg ));

        case OM_GET:
            return( EditFTP_Get( cl, obj, (APTR)msg ));

        case MUIM_Edit_Update:
            return( EditFTP_Update( cl, obj, (APTR)msg ));

        case MUIM_Edit_URL:
            return( Edit_URL( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// EditFTP_New
static ULONG EditFTP_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditFTPData  temp = { 0 };
    Object             *Advanced;
    static STRPTR       Labels[] = { (STRPTR)MENU_SETTINGS, (STRPTR)MGS_ADVANCED, NULL };
    static STRPTR       WinTypes[] = { (STRPTR)MSG_DUAL_LIST, (STRPTR)MSG_SINGLE_LIST, NULL };
    static BOOL         FirstTime = TRUE;

    if( FirstTime ) {

        LocalizeArray( Labels   );
        LocalizeArray( WinTypes );

        FirstTime = FALSE;
    }

    if( obj = ( Object * )DoSuperNew( cl, obj,

                        Child, RegisterGroup( Labels ),

                            /*              Settings                */
                            Child, ColGroup( 2 ),

                                Child, MakeLabel1( MSG_NAME ),
                                Child, temp.Name = MakeString( 80, MSG_NAME, -1 ),

                                Child, MakeLabel1( GAD_FTP ),
                                Child, temp.FTP = MakeString( 256, GAD_FTP, -1 ),

                                Child, MakeLabel1( GAD_ALIAS  ),
                                Child, temp.Alias = MakeString( 40, GAD_ALIAS, -1 ),

                                Child, MakeLabel1( MSG_COMMENT  ),
                                Child, temp.Comment = TextinputscrollObject,
                                        StringFrame,
                                        MUIA_CycleChain,            TRUE,
                                        MUIA_Textinput_Multiline,   TRUE,
                                        MUIA_Textinput_AutoExpand,  TRUE,
                                        MUIA_Textinput_MinVersion,  12,
                                        MUIA_Textinput_MaxLen,      512,
                                End,

                                Child, MakeLabel1( MSG_LOCAL_LISTER ),
                                Child, HGroup, MUIA_Group_Spacing, 0,
                                    Child, temp.UseLister = MakeCheck( MSG_LOCAL_LISTER, -1 ),
                                    Child, MakeFile( &temp.Lister, 256, MSG_LOCAL_LISTER, -1 ),
                                End,
                            End,

                            /*              Advanced                */
                            Child, VGroup,

                                Child, HGroup,
                                    Child, MakeLabel1( GAD_ADVANCED_OPTIONS ),
                                    Child, temp.Advanced = MakeCheck( GAD_ADVANCED_OPTIONS, -1 ),
                                    Child, HSpace( 0 ),
                                End,

                                Child, Advanced = ColGroup( 2 ),

                                    MUIA_Disabled,  TRUE,

                                    Child, MakeLabel1( GAD_PORT ),
                                    Child, HGroup,
                                        Child, temp.Port = MakeInteger( 12, GAD_PORT, -1 ),
                                        Child, HSpace( 0 ),
                                        Child, MakeLabel1( GAD_ADT ),
                                        Child, temp.IsADT = MakeCheck( GAD_ADT, -1 ),
                                        Child, HSpace( 0 ),
                                        Child, MakeLabel1( GAD_ANONYMOUS_LOGIN ),
                                        Child, temp.Anonymous = MakeCheck( GAD_ANONYMOUS_LOGIN, -1 ),
                                    End,

                                    Child, MakeLabel1( GAD_USERNAME ),
                                    Child, temp.Username = MakeString( 60, GAD_USERNAME, -1 ),

                                    Child, MakeLabel1( GAD_PASSWORD ),
                                    Child, temp.Password = MakeSecretString( 60, GAD_PASSWORD, -1 ),

                                    Child, MakeLabel1( GAD_QUIET ),
                                    Child, HGroup,
                                        Child, temp.Quiet = MakeCheck( GAD_QUIET, -1 ),
                                        Child, HSpace( 0 ),
                                        Child, MakeLabel1( GAD_COMPRESSION ),
                                        Child, temp.Compression = MakeCheck( GAD_COMPRESSION, -1 ),
                                    End,

                                    Child, MakeLabel1( GAD_RETRIES ),
                                    Child, temp.Retries = MakeSlider( GAD_RETRIES, 0, 50 ),

                                    Child, MakeLabel1( GAD_WINDOW_TYPE ),
                                    Child, temp.WinType = MakeCycle( WinTypes, GAD_WINDOW_TYPE ),

                                    Child, MakeLabel1( GAD_SHOW_FTP_OUTPUT ),
                                    Child, HGroup,
                                        Child, temp.ShowFTPOut = MakeCheck( GAD_SHOW_FTP_OUTPUT, -1 ),
                                        Child, HSpace( 0 ),
                                        Child, MakeLabel1( GAD_SHOW_HIDDEN ),
                                        Child, temp.ShowHidden = MakeCheck( GAD_SHOW_HIDDEN, -1 ),
                                    End,

                                    Child, MakeLabel1( GAD_SEND_NOOP ),
                                    Child, HGroup,
                                        Child, temp.SendNOOP = MakeCheck( GAD_SEND_NOOP, -1 ),
                                        Child, HSpace( 0 ),
                                        Child, MakeLabel1( GAD_USE_PROXY ),
                                        Child, temp.UseProxy = MakeCheck( GAD_USE_PROXY, -1 ),
                                    End,
                                End,
                            End,
                        End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct EditFTPData *data = INST_DATA( cl, obj );

        *data = temp;

        set( temp.Username,  MUIA_Disabled, TRUE );
        set( temp.Password,  MUIA_Disabled, TRUE );
        set( temp.Anonymous, MUIA_Selected, TRUE );
        set( temp.Lister,    MUIA_Disabled, TRUE );

        /*  Gadgets */
        DoMethod( temp.Advanced,  MUIM_Notify, MUIA_Selected, MUIV_EveryTime, Advanced,      3, MUIM_Set, MUIA_Disabled, MUIV_NotTriggerValue );
        DoMethod( temp.Anonymous, MUIM_Notify, MUIA_Selected, MUIV_EveryTime, temp.Username, 3, MUIM_Set, MUIA_Disabled, MUIV_TriggerValue    );
        DoMethod( temp.Anonymous, MUIM_Notify, MUIA_Selected, MUIV_EveryTime, temp.Password, 3, MUIM_Set, MUIA_Disabled, MUIV_TriggerValue    );

        DoMethod( temp.FTP, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, obj, 3, MUIM_Edit_URL, temp.FTP, URL_FTP );

        DoMethod( temp.UseLister, MUIM_Notify, MUIA_Selected, MUIV_EveryTime, temp.Lister, 3, MUIM_Set, MUIA_Disabled, MUIV_NotTriggerValue );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// EditFTP_Set
static ULONG EditFTP_Set( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditFTPData *data = INST_DATA( cl, obj );
    struct TagItem     *tag;

    if( tag = FindTagItem( MUIA_Edit_Entry, msg->ops_AttrList )) {

        data->Current = (struct CMFTP *) tag->ti_Data;

        set( data->Name,     MUIA_String_Contents, data->Current->Name        );
        set( data->Comment,  MUIA_String_Contents, data->Current->Comment     );
        set( data->FTP,      MUIA_String_Contents, data->Current->FTP         );
        set( data->Username, MUIA_String_Contents, data->Current->Username    );
        set( data->Password, MUIA_String_Contents, data->Current->Password    );
        set( data->Port,     MUIA_String_Integer,  data->Current->Port        );
        set( data->Retries,  MUIA_Numeric_Value,   data->Current->Retries     );
        set( data->Lister,   MUIA_String_Contents, data->Current->Local       );
        set( data->Alias,    MUIA_String_Contents, data->Current->Alias       );
        set( data->WinType,  MUIA_Cycle_Active,    data->Current->WindowStyle );

        if( data->Current->Flags & FTPF_ADVANCED )
            set( data->Advanced, MUIA_Selected, TRUE );

        if( data->Current->Flags & FTPF_QUIET )
            set( data->Quiet, MUIA_Selected, TRUE );

        if(!( data->Current->Flags & FTPF_ANONYMOUS ))
            set( data->Anonymous, MUIA_Selected, FALSE );

        if( data->Current->Flags & FTPF_COMPRESS )
            set( data->Compression, MUIA_Selected, TRUE );

        if( data->Current->Flags & FTPF_LOCAL )
            set( data->UseLister, MUIA_Selected, TRUE );

        if( data->Current->Flags & FTPF_PROXY )
            set( data->UseProxy, MUIA_Selected, TRUE );

        if( data->Current->Flags & FTPF_SHOW_HIDDEN )
            set( data->ShowHidden, MUIA_Selected, TRUE );

        if( data->Current->Flags & FTPF_ADT )
            set( data->IsADT, MUIA_Selected, TRUE );

        if( data->Current->ExtraFlags & FTPEF_SHOW_FTP_OUTPUT )
            set( data->ShowFTPOut, MUIA_Selected, TRUE );

        if( data->Current->ExtraFlags & FTPEF_NOOPS )
            set( data->SendNOOP, MUIA_Selected, TRUE );
    }

    return( DoSuperMethodA( cl, obj, (Msg)msg ));
}
///
/// EditFTP_Get
static ULONG EditFTP_Get( struct IClass *cl, Object *obj, struct opGet *msg )
{
    struct EditFTPData *data = INST_DATA( cl, obj );

    switch ( msg->opg_AttrID ) {

        case MUIA_Edit_Entry:
            *msg->opg_Storage = (ULONG) data->Current;
            return( TRUE );
    }

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// EditFTP_Update
static ULONG EditFTP_Update( struct IClass *cl, Object *obj, Msg msg )
{
    struct EditFTPData *data = INST_DATA( cl, obj );
    struct CMFTP       *entry;

    entry = data->Current;

    getstr( data->Name,     entry->Name     );
    getstr( data->Comment,  entry->Comment  );
    getstr( data->FTP,      entry->FTP      );
    getstr( data->Username, entry->Username );
    getstr( data->Password, entry->Password );
    getstr( data->Lister,   entry->Local    );
    getstr( data->Alias,    entry->Alias    );

    entry->Port        = xget( data->Port,    MUIA_String_Integer );
    entry->Retries     = xget( data->Retries, MUIA_Numeric_Value  );
    entry->WindowStyle = xget( data->WinType, MUIA_Cycle_Active   );

    entry->Flags &= ~( FTPF_ADVANCED | FTPF_QUIET | FTPF_ANONYMOUS | FTPF_COMPRESS |
                       FTPF_ADT | FTPF_LOCAL | FTPF_PROXY | FTPF_SHOW_HIDDEN );

    entry->ExtraFlags &= ~( FTPEF_SHOW_FTP_OUTPUT | FTPEF_NOOPS );

    if( xget( data->Advanced, MUIA_Selected ))
        entry->Flags |= FTPF_ADVANCED;

    if( xget( data->Quiet, MUIA_Selected ))
        entry->Flags |= FTPF_QUIET;

    if( xget( data->Anonymous, MUIA_Selected ))
        entry->Flags |= FTPF_ANONYMOUS;

    if( xget( data->Compression, MUIA_Selected ))
        entry->Flags |= FTPF_COMPRESS;

    if( xget( data->IsADT, MUIA_Selected ))
        entry->Flags |= FTPF_ADT;

    if( xget( data->UseLister, MUIA_Selected ))
        entry->Flags |= FTPF_LOCAL;

    if( xget( data->UseProxy, MUIA_Selected ))
        entry->Flags |= FTPF_PROXY;

    if( xget( data->ShowHidden, MUIA_Selected ))
        entry->Flags |= FTPF_SHOW_HIDDEN;

    if( xget( data->ShowFTPOut, MUIA_Selected ))
        entry->ExtraFlags |= FTPEF_SHOW_FTP_OUTPUT;

    if( xget( data->SendNOOP, MUIA_Selected ))
        entry->ExtraFlags |= FTPEF_NOOPS;

    return( 0 );
}
///

/// EditIRC_Dispatcher
ULONG SAVEDS ASM EditIRC_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( EditIRC_New( cl, obj, (APTR)msg ));

        case OM_SET:
            return( EditIRC_Set( cl, obj, (APTR)msg ));

        case OM_GET:
            return( EditIRC_Get( cl, obj, (APTR)msg ));

        case MUIM_Edit_Update:
            return( EditIRC_Update( cl, obj, (APTR)msg ));

        case MUIM_Edit_URL:
            return( Edit_URL( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// EditIRC_New
static ULONG EditIRC_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditIRCData  temp = { 0 };

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            Child, ColGroup( 2 ),

                                Child, MakeLabel1( GAD_CHANNEL ),
                                Child, HGroup,
                                    Child, temp.Channel = MakeString( 128, GAD_CHANNEL, -1 ),

                                    Child, MakeLabel1( GAD_SERVER ),
                                    Child, temp.Server = MakeString( 128, GAD_SERVER, -1 ),
                                End,

                                Child, MakeLabel1( GAD_PASSWORD ),
                                Child, temp.Password = MakeSecretString( 60, GAD_PASSWORD, -1 ),

                                Child, MakeLabel1( GAD_MAINTAINER ),
                                Child, HGroup,
                                    Child, temp.Maintainer = MakeString( 80, GAD_MAINTAINER, -1 ),

                                    Child, MakeLabel1( GAD_NICK ),
                                    Child, temp.Nick = MakeString( 20, GAD_NICK, -1 ),
                                End,

                                Child, MakeLabel1( GAD_WEB_SITE ),
                                Child, temp.WWW = MakeString( 256, GAD_WEB_SITE, -1 ),

                                Child, MakeLabel1( MSG_COMMENT  ),
                                Child, temp.Comment = TextinputscrollObject,
                                        StringFrame,
                                        MUIA_CycleChain,            TRUE,
                                        MUIA_Textinput_Multiline,   TRUE,
                                        MUIA_Textinput_AutoExpand,  TRUE,
                                        MUIA_Textinput_MinVersion,  12,
                                        MUIA_Textinput_MaxLen,      512,
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct EditIRCData *data = INST_DATA( cl, obj );

        *data = temp;

        /*  Gadgets */
        DoMethod( temp.WWW, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, obj, 3, MUIM_Edit_URL, temp.WWW, URL_WWW );

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// EditIRC_Set
static ULONG EditIRC_Set( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditIRCData *data = INST_DATA( cl, obj );
    struct TagItem     *tag;

    if( tag = FindTagItem( MUIA_Edit_Entry, msg->ops_AttrList )) {

        data->Current = (struct CMChat *) tag->ti_Data;

        set( data->Channel,     MUIA_String_Contents, data->Current->Channel    );
        set( data->Server,      MUIA_String_Contents, data->Current->Server     );
        set( data->Maintainer,  MUIA_String_Contents, data->Current->Maintainer );
        set( data->Nick,        MUIA_String_Contents, data->Current->Nick       );
        set( data->Comment,     MUIA_String_Contents, data->Current->Comment    );
        set( data->WWW,         MUIA_String_Contents, data->Current->WWW        );
        set( data->Password,    MUIA_String_Contents, data->Current->Password   );
    }

    return( DoSuperMethodA( cl, obj, (Msg)msg ));
}
///
/// EditIRC_Get
static ULONG EditIRC_Get( struct IClass *cl, Object *obj, struct opGet *msg )
{
    struct EditIRCData    *data = INST_DATA( cl, obj );

    switch ( msg->opg_AttrID ) {

        case MUIA_Edit_Entry:
            *msg->opg_Storage = (ULONG) data->Current;
            return( TRUE );
    }

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// EditIRC_Update
static ULONG EditIRC_Update( struct IClass *cl, Object *obj, Msg msg )
{
    struct EditIRCData *data = INST_DATA( cl, obj );
    struct CMChat      *entry;

    entry = data->Current;

    getstr( data->Channel,    entry->Channel    );
    getstr( data->Server,     entry->Server     );
    getstr( data->Maintainer, entry->Maintainer );
    getstr( data->Nick,       entry->Nick       );
    getstr( data->Comment,    entry->Comment    );
    getstr( data->WWW,        entry->WWW        );
    getstr( data->Password,   entry->Password   );

    return( 0 );
}
///

/// EditTelnet_Dispatcher
ULONG SAVEDS ASM EditTelnet_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( EditTelnet_New( cl, obj, (APTR)msg ));

        case OM_SET:
            return( EditTelnet_Set( cl, obj, (APTR)msg ));

        case OM_GET:
            return( EditTelnet_Get( cl, obj, (APTR)msg ));

        case MUIM_Edit_Update:
            return( EditTelnet_Update( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// EditTelnet_New
static ULONG EditTelnet_New( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditTelnetData  temp = { 0 };

    if( obj = ( Object * )DoSuperNew( cl, obj,

                            Child, ColGroup( 2 ),

                                Child, MakeLabel1( GAD_HOST ),
                                Child, HGroup,
                                    Child, temp.Host = MakeString( 128, GAD_HOST, -1 ),

                                    Child, MakeLabel1( GAD_PORT ),
                                    Child, temp.Port = MakeInteger( 12, GAD_PORT, -1 ),

                                    Child, MakeLabel1( GAD_SSH ),
                                    Child, temp.SSH = MakeCheck( GAD_SSH, -1 ),
                                End,

                                Child, MakeLabel1( GAD_LOGIN ),
                                Child, HGroup,
                                    Child, temp.Login = MakeString( 128, GAD_LOGIN, -1 ),

                                    Child, MakeLabel1( GAD_PASSWORD ),
                                    Child, temp.Password = MakeSecretString( 60, GAD_PASSWORD, -1 ),
                                End,

                                Child, MakeLabel1( MSG_COMMENT  ),
                                Child, temp.Comment = TextinputscrollObject,
                                        StringFrame,
                                        MUIA_CycleChain,            TRUE,
                                        MUIA_Textinput_Multiline,   TRUE,
                                        MUIA_Textinput_AutoExpand,  TRUE,
                                        MUIA_Textinput_MinVersion,  12,
                                        MUIA_Textinput_MaxLen,      512,
                                End,
                            End,

                        TAG_MORE, msg->ops_AttrList )) {

        struct EditTelnetData  *data = INST_DATA( cl, obj );

        set( temp.Port, MUIA_Weight, 30 );

        *data = temp;

        /*  Gadgets */

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// EditTelnet_Set
static ULONG EditTelnet_Set( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct EditTelnetData  *data = INST_DATA( cl, obj );
    struct TagItem         *tag;

    if( tag = FindTagItem( MUIA_Edit_Entry, msg->ops_AttrList )) {

        data->Current = (struct CMTelnet *) tag->ti_Data;

        set( data->Host,     MUIA_String_Contents, data->Current->Host     );
        set( data->Login,    MUIA_String_Contents, data->Current->Login    );
        set( data->Password, MUIA_String_Contents, data->Current->Password );
        set( data->Comment,  MUIA_String_Contents, data->Current->Comment  );
        set( data->Port,     MUIA_String_Integer,  data->Current->Port     );

        if( data->Current->Flags & TNETF_SSH )
            set( data->SSH, MUIA_Selected, TRUE );
    }

    return( DoSuperMethodA( cl, obj, (Msg)msg ));
}
///
/// EditTelnet_Get
static ULONG EditTelnet_Get( struct IClass *cl, Object *obj, struct opGet *msg )
{
    struct EditTelnetData  *data = INST_DATA( cl, obj );

    switch ( msg->opg_AttrID ) {

        case MUIA_Edit_Entry:
            *msg->opg_Storage = (ULONG) data->Current;
            return( TRUE );
    }

    return( DoSuperMethodA( cl,obj, (Msg) msg ));
}
///
/// EditTelnet_Update
static ULONG EditTelnet_Update( struct IClass *cl, Object *obj, Msg msg )
{
    struct EditTelnetData  *data = INST_DATA( cl, obj );
    struct CMTelnet        *entry;

    entry = data->Current;

    getstr( data->Host,     entry->Host     );
    getstr( data->Login,    entry->Login    );
    getstr( data->Password, entry->Password );
    getstr( data->Comment,  entry->Comment  );

    entry->Port = xget( data->Port, MUIA_String_Integer );

    entry->Flags &= ~TNETF_SSH;

    if( xget( data->SSH, MUIA_Selected ))
        entry->Flags |= TNETF_SSH;

    return( 0 );
}
///

/// Edit_URL
static ULONG Edit_URL( struct IClass *cl, Object *obj, struct MUIP_Edit_URL *msg )
{
    TEXT    buf1[256], buf2[256];
    BOOL    change = FALSE;

    getstr( msg->URL, buf1 );

    if( buf1[0] == '\0' )
        return( 0 );

    switch( msg->Type ) {

        case URL_WWW:
            if( strncmp( buf1, "http://", 7 ) && strncmp( buf1, "https://", 8 )) {
                sprintf( buf2, "http://%s", buf1 );
                change = TRUE;
            }
            break;

        case URL_FTP:
            if( strncmp( buf1, "ftp://", 6 )) {
                sprintf( buf2, "ftp://%s", buf1 );
                change = TRUE;
            }
            break;
    }

    if( change ) {

        strcpy( buf1, buf2 );

        set( msg->URL, MUIA_String_Contents, buf1 );
    }

    return( 0 );
}
///
