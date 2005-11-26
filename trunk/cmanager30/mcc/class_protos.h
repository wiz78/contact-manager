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

#ifndef _CLASS_PROTOS_H
#define _CLASS_PROTOS_H

/* init.c */
#ifdef __MORPHOS__
ULONG query ( void );
#else
ULONG SAVEDS ASM query ( REG (d0,LONG which )) ;
#endif
void freeBase ( void );
ULONG initBase ( void );

/* loc.c */
void loadCountries ( void );
void localizeArray ( UBYTE **array , ULONG *ids );
void localizeMenus ( struct NewMenu *menu , ULONG *ids );
void localizeButtonsBar ( struct MUIS_TheBar_Button *button , ULONG *ids );

/* utils.c */
#ifdef __MORPHOS__
void sprintf ( STRPTR buf , STRPTR fmt , ...);
int snprintf ( STRPTR buf , int size , STRPTR fmt , ...) __attribute((varargs68k));
Object * opopframe(ULONG key,ULONG title,ULONG help);
#else
ULONG DoSuperNew ( struct IClass *cl , Object *obj , ULONG tag1 , ...);
void STDARGS sprintf ( STRPTR to , STRPTR fmt , ...);
int STDARGS snprintf ( STRPTR buf , int size , STRPTR fmt , ...);
#endif
void vsprintf ( STRPTR to , STRPTR fmt , APTR args );
LONG xget ( Object *obj , ULONG attr );
UBYTE getstr ( Object *obj , STRPTR str );
ULONG getKeyCharID ( LONG id );
ULONG getKeyChar ( STRPTR string );
Object *ostring ( ULONG maxlen , ULONG key , ULONG help );
Object *oqstring ( Object *list , ULONG maxlen , ULONG key , ULONG help );
Object *ofile ( Object **obj , ULONG size , ULONG txt , ULONG help , ULONG drawersOnly );
Object *ocheck ( ULONG key , ULONG help );
Object *obutton ( ULONG text , ULONG help );
Object *osstring ( ULONG maxlen , ULONG key , ULONG help );
Object *oqsstring ( Object *list , ULONG maxlen , ULONG key , ULONG help );
Object *ointeger ( ULONG maxlen , ULONG key , ULONG help );
Object *oqinteger ( Object *list , ULONG maxlen , ULONG key , ULONG help );
Object *ocycle ( STRPTR *array , ULONG key , ULONG help );
Object *oslider ( ULONG key , ULONG help , LONG min , LONG max );
Object *olabel ( ULONG id );
Object *ollabel ( ULONG id );
Object *olabel1 ( ULONG id );
Object *ollabel1 ( ULONG id );
Object *olabel2 ( ULONG id );
Object *ofreelabel2 ( ULONG text );
Object *ourltext ( STRPTR url , STRPTR text );
Object *oibutton ( ULONG spec , ULONG help );
Object *osscroll ( ULONG len , ULONG key , ULONG help );
APTR allocVecPooled ( APTR pool , ULONG size );
void freeVecPooled ( APTR pool , APTR mem );
APTR allocArbitratePooled ( ULONG size );
APTR freeArbitratePooled ( APTR mem , ULONG size );
APTR allocArbitrateVecPooled ( ULONG size );
APTR freeArbitrateVecPooled ( APTR mem );
APTR reallocArbitrateVecPooled ( APTR mem , ULONG size );
ULONG getDir ( Object *obj , STRPTR Title , STRPTR Path , ULONG SaveMode );
ULONG openWindow ( Object *app , Object *win );
void sleepApp ( Object *obj , ULONG sleep );
STRPTR expandPlaceHolders ( STRPTR template , struct placeHolder *ph , STRPTR buf , ULONG bufSize , int num );
ULONG editURLFun ( Object *URL , ULONG type );
ULONG editGoURLFun ( STRPTR url );
ULONG dateToGreg ( ULONG day , ULONG month , ULONG year );
ULONG gregToDate ( ULONG greg , ULONG *day , ULONG *month , ULONG *year );
ULONG dateWeekday ( ULONG day , ULONG month , ULONG year );
ULONG gregWeekday ( ULONG greg );
STRPTR formatDate ( STRPTR buf , int size , STRPTR fmt , ULONG greg );
ULONG getToday ( void );
ULONG checkBirthday ( ULONG bgreg );
ULONG createUserDir ( STRPTR user );
ULONG etranslate ( STRPTR buffer , STRPTR to , int len );
ULONG getMap ( STRPTR address , STRPTR country , STRPTR city , STRPTR zip );
Object *oflabel ( ULONG id );
Object *ofont ( Object **obj , ULONG size , ULONG txt , ULONG help );
Object *opoppen ( ULONG key , ULONG title , ULONG help );
Object *opopimage ( ULONG type , ULONG key , ULONG title , ULONG help );
ULONG freeStack ( void );

/* options.c */
void setDefaultVisualPrefs ( struct VisualPrefs *VisualPrefs );
void setDefaultPrefs ( struct CMPrefs *prefs );
ULONG savePrefs ( STRPTR user , struct CMPrefs *prefs );
ULONG loadPrefs ( STRPTR user , struct CMPrefs *prefs );

/* card.c */
ULONG initCard ( void );
void disposeCard ( void );

/* prefs.c */
ULONG initPrefsWindow ( void );
void disposePrefsWindow ( void );

/* search.c */
ULONG initSearchWindow ( void );
void disposeSearchWindow ( void );

/* login.c */
ULONG initLoginWindow ( void );
void disposeLoginWindow ( void );

/* entrylist.c */
ULONG initEntryList ( void );
void disposeEntryList ( void );

/* grouplist.c */
ULONG initGroupList ( void );
void disposeGroupList ( void );

/* dial.c */
ULONG initDialWindow ( void );
void disposeDialWindow ( void );

/* pic.c */
ULONG initPic ( void );
void disposePic ( void );

/* delwarning.c */
ULONG initDelWarning ( void );
void disposeDelWarning ( void );

/* rootscrollgroup.c */
ULONG initRootScrollgroup ( void );
void disposeRootScrollgroup ( void );

/* rootstring.c */
ULONG initRootString ( void );
void disposeRootString ( void );

/* rootscroll.c */
ULONG initRootScroll ( void );
void disposeRootScroll ( void );

/* rootbar.c */
ULONG initRootBar ( void );
void disposeRootBar ( void );

/* qstring.c */
ULONG initQString ( void );
void disposeQString ( void );

/* popupcodesets.c */
ULONG initPopupCodesets ( void );
void disposePopupCodesets ( void );

/* popasl.c */
ULONG initPopasl ( void );
void disposePopasl ( void );

/* editWin.c */
ULONG initEditWindow ( void );
void disposeEditWindow ( void );

/* editUser.c */
ULONG initEditUser ( void );
void disposeEditUser ( void );

/* editWWW.c */
ULONG initEditWWW ( void );
void disposeEditWWW ( void );

/* editFTP.c */
ULONG initEditFTP ( void );
void disposeEditFTP ( void );

/* editIRC.c */
ULONG initEditIRC ( void );
void disposeEditIRC ( void );

/* minimail.c */
ULONG initMiniMailWindow ( void );
void disposeMiniMailWindow ( void );

/* editTelnet.c */
ULONG initEditTelnet ( void );
void disposeEditTelnet ( void );

/* import.c */
ULONG fileNew ( STRPTR name , struct DateStamp *stamp );
void readStamps ( struct CMPrefs *prefs );
void saveDates ( STRPTR fileName , struct DateStamp *stamps );
void importVoyager ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importIB ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importAWeb ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importUserCSV ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importUserCSVFullOE ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importAmFTP ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importMD ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importYAM ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importSimpleMail ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importDFA ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importPINT ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importGoodNews ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importSTRICQ ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importSTFax ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importAmIRC ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importDOpusFTP ( struct CMGroup *group , struct MUIS_CManager_IO *io );
void importNetscape ( struct CMGroup *group , struct MUIS_CManager_IO *io );

/* export.c */
#ifdef __MORPHOS__
int PutString ( BPTR file , STRPTR fmt , ...) __attribute((varargs68k));
#else
int STDARGS PutString ( BPTR file , STRPTR fmt , ...);
#endif
void exportWWWasHTML ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportURLasHTML ( struct CMData *CMData , struct MUIS_CManager_IO *io );
void exportFTPasHTML ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportUsersAsCSV ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportUsersAsCSVFullOE ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportAWeb ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportIBrowse ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportYAM ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportSimpleMail ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportIE ( struct CMGroup *Group , struct MUIS_CManager_IO *io );
void exportNetscape ( struct CMGroup *Group , struct MUIS_CManager_IO *io );

/* matherwin.c */
ULONG initMatherWin ( void );
void disposeMatherWin ( void );

/* mcc.c */
ULONG initMCC ( void );
void disposeMCC ( void );

/* lamp.c */
ULONG initLamp ( void );
void disposeLamp ( void );

/* mcc.c */
ULONG initPopph ( void );
void disposePopph ( void );

#endif /* _CLASS_PROTOS_H */
