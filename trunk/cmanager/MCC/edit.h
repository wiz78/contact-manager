/***************************************************************************
    revision             : $Id: edit.h,v 1.1.1.1 2003-01-06 15:58:41 tellini Exp $
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

#ifndef EDIT_H
#define EDIT_H

#ifndef TAGBASE
#define TAGBASE ( TAG_USER | ( 30571UL << 16 ))
#endif

/// Methods
#define MUIM_Edit_Update                (TAGBASE + 1)
#define MUIM_Edit_URL                   (TAGBASE + 2)
#define MUIM_Edit_Phone                 (TAGBASE + 3)
#define MUIM_Edit_GetImage              (TAGBASE + 4)
///
/// Tags
#define MUIA_Edit_List      (TAGBASE + 1)
#define MUIA_Edit_Entry     (TAGBASE + 2)
///
/// Structs
struct EditData {
        Object                 *Name;
        Object                 *LastName;
        Object                 *Address;
        Object                 *City;
        Object                 *Country;
        Object                 *ZIP;
        Object                 *Alias;
        Object                 *Comment;
        Object                 *Phone;
        Object                 *Mobile;
        Object                 *Fax;
        Object                 *EMail;
        Object                 *WWW;
        Object                 *FTP;
        Object                 *ICQ;
        Object                 *Image;
        Object                 *Company;
        Object                 *BJobTitle;
        Object                 *BOffice;
        Object                 *BAddress;
        Object                 *BCity;
        Object                 *BCountry;
        Object                 *BZIP;
        Object                 *BDepartment;
        Object                 *BPhone;
        Object                 *BMobile;
        Object                 *BFax;
        Object                 *BEMail;
        Object                 *PGP;
        struct CMUser          *Current;
        struct FileRequester   *Req;
};

struct EditWWWData {
        Object         *Name;
        Object         *WWW;
        Object         *Comment;
        Object         *WebMaster;
        Object         *EMail;
        Object         *Alias;
        struct CMWWW   *Current;
};

struct EditFTPData {
        Object         *Name;
        Object         *FTP;
        Object         *Alias;
        Object         *Comment;
        Object         *Advanced;
        Object         *Port;
        Object         *Username;
        Object         *Password;
        Object         *IsADT;
        Object         *Anonymous;
        Object         *Quiet;
        Object         *Retries;
        Object         *Compression;
        Object         *List;
        Object         *UseLister;
        Object         *Lister;
        Object         *WinType;
        Object         *ShowFTPOut;
        Object         *ShowHidden;
        Object         *UseProxy;
        Object         *SendNOOP;
        struct CMFTP   *Current;
};

struct EditIRCData {
        Object         *Channel;
        Object         *Server;
        Object         *Maintainer;
        Object         *Nick;
        Object         *Comment;
        Object         *WWW;
        Object         *Password;
        struct CMChat  *Current;
};

struct EditTelnetData {
        Object             *Host;
        Object             *Port;
        Object             *SSH;
        Object             *Login;
        Object             *Password;
        Object             *Comment;
        struct CMTelnet    *Current;
};

struct MUIP_Edit_Update {
        ULONG           MethodID;
        ULONG           Refresh;
};

struct MUIP_Edit_Phone {
        ULONG           MethodID;
        Object         *Gadget;
};

struct MUIP_Edit_URL {
        ULONG           MethodID;
        Object         *URL;
        ULONG           Type;
};
///
/// Externs
extern struct MUI_CustomClass  *CL_Edit;
extern struct MUI_CustomClass  *CL_EditWWW;
extern struct MUI_CustomClass  *CL_EditFTP;
extern struct MUI_CustomClass  *CL_EditIRC;
extern struct MUI_CustomClass  *CL_EditTelnet;

extern ULONG SAVEDS ASM Edit_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
extern ULONG SAVEDS ASM EditWWW_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
extern ULONG SAVEDS ASM EditFTP_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
extern ULONG SAVEDS ASM EditIRC_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
extern ULONG SAVEDS ASM EditTelnet_Dispatcher( REG( a0 ) struct IClass *, REG( a2 ) Object *, REG( a1 ) Msg );
///

#endif /* EDIT_H */
