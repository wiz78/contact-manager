//
// AmFTP Amiga FTP Client, amftp.h
//
// Copyright 1995-1996 by Mathias Mischler
//

#define ATVER

#define __USE_SYSBASE

//
// AmigaOS Includes 
//

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/locale.h>
#include <proto/icon.h>
#include <proto/graphics.h>
#include <proto/utility.h>
#include <proto/timer.h>
#include <proto/rexxsyslib.h>
#include <proto/datatypes.h>

#include <exec/memory.h>
#include <exec/interrupts.h>
#include <proto/muimaster.h>
#include <libraries/mui.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>
#include <libraries/gadtools.h>
#include <libraries/asl.h>
#include <devices/keyboard.h>
#include <dos/dostags.h>
#include <datatypes/soundclass.h>
#include <devices/timer.h>
#include <MUI/busy_mcc.h>
#include <MUI/NList_mcc.h>
#include <MUI/NListview_mcc.h>

//
// Support Includes
//

#include "rev.h"
#include <cl/lists.h>
#include "asyncio.h"

#define USE_BUILTIN_MATH

// Include NetConnect Stuff
#include "nc_lib.h"

//
// ANSI/SAS C Includes
//

#include <time.h>
#include <string.h>
#include <constructor.h>
#include <ctype.h>
#include <stdlib.h>

#define MAKE_ID(a,b,c,d)	\
	((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#define findmenu(id) (APTR)DoMethod(menu,MUIM_FindUData,id)

#define D_S(type,name) char a_##name[sizeof(type)+3]; \
                       type *name = (type *)((LONG)(a_##name+3) & ~3);

// 
// Locale / FlexCat Definitions
//

#define GS(x) GetString(MSG_##x)
#define GSI(x) GetString(x)
#include "AmFTP_Cat.h"

//
// Amiga.lib
//

#include "pools.h"

//
// Structures
//

#include "request.h"

struct filenode {
	char name[ 144 ];
	ULONG type;
//	ULONG entries;
	char date[ 32 ];
	ULONG len;
	char linkdest[ 64 ];
};

struct profile {
	char label[ 40 ];
	char host[ 128 ];
	char dummy4[ 32 ];
	char password[ 64 ];
	char defdir[ 120 ];
	char dummy[ 8 ];
	char localdir[ 120 ];
	char port[ 6 ];
	UBYTE compress;
	UBYTE saveldir;
	UBYTE is_anon;
	UBYTE retries;
	UBYTE savedir;
	UBYTE quiet_login;
	UBYTE is_adt;
	char username[ 64 ];
	UBYTE dummy3; // ACHTUNG siehe profile.c sizeof - 1
};

struct mysock {
	struct Library *Lib;	// Libbase to use
	int sock;
	int errno, *realerrno;
	int dontshutdown;
};

//
// Menu-IDs
//

enum {
	MENU_dummy = 5000,

	ID_CHECKSERIAL,

	MENU_ABOUT,
	MENU_ICONIFY,
	MENU_QUIT,
	MENU_MUIOPTS,
	MENU_FLAT,
	MENU_DIRTOOL,
	MENU_OPENBATCHWIN,
	MENU_SETTINGS,
	MENU_REGISTER,
	MENU_OPENCONNECT,
	MENU_DOWNLOAD,
	MENU_SENDRAW,
	MENU_ADTSHOWMOTD,
	MENU_SNAPSHOT,
	MENU_SAVEPROFILES,
	MENU_IMPORTPROFILES,
	MENU_SAVEBATCH,
	MENU_LOADBATCH,
	ID_HANG	// in main.c / noop.c
};

//
// ExportIDs
//

enum {
	EXID_dummy = 1
};


//
//	MUI-IDs
//

enum {
	ID_dummy = 1,
	ID_CONNECT,
	ID_ABORT_FILE,
	ID_ABORT_ALL,
	ID_CLOSEABOUT
};

// rexx.c

extern BOOL reqrexxwait;
extern struct MsgPort *rxmsgp;

//
// MUI objects
//

extern APTR app, lv_log, lv_1, lv_2, str_p1, str_p2, lv_3, lv_togo, lv_batch, win_batch;
extern APTR win_main, grp_main, grp_adt;
extern APTR lv_profiles;
extern APTR bt_cw_new, bt_cw_delete, bt_cdup_2;
extern APTR chk_cw_anon, str_cw_user, str_cw_pass, str_cw_label, str_cw_host, str_cw_dir;
extern APTR chk_cw_savedir, sl_cw_retries;
extern APTR grp_cw_edit, bt_cw_doconnect;
extern APTR tx_xf_local, g_xf;
extern APTR tx_xf_remote, tx_xf_size, tx_xf_time, tx_xf_togo, tx_xf_speed;
extern APTR bt_xf_abort;
extern APTR tx_i1, tx_i2, tx_i3, tx_lb;
extern APTR cyc_mode;
extern APTR lv_his1, lv_his2;
extern APTR bar_togo, bt_xf_abortall;
extern APTR grp_iob, busyobj;
extern APTR adtstate;
extern APTR gr_1, grp_v, grp_remote, amftp_im;

extern struct MUI_Command rexxcmds[];

extern char copyright[];
extern char *st_yes, *st_no, *st_ok, *st_cancel;
extern struct mysock *ctrlsock, *datasock;
extern int datasockport;
extern int isconnected;
extern char *basename;
extern char startup_cfgfile[];

extern struct profile *currentprofile;
extern ULONG nextexpected_count;
extern ULONG nextexpected_start;
extern ULONG nextexpected_action;

extern ULONG sendmode;
#define SM_Download 0
#define SM_Upload 1
#define SM_Batch 2
extern ULONG adtmode;
extern APTR adtsort_mode, adttime_mode;

//
// Prototypes
//

//
// main.c
//

extern long isdemo;
void __stdargs sprintf( char *, char *, ... );
struct Hook *allochook( HOOKFUNC f, APTR data );
#define hookd(f,d) allochook((HOOKFUNC)f,d)
#define hook(f) allochook((HOOKFUNC)f,NULL)
APTR myalloc( ULONG size );
void myfree( APTR );
STRPTR mystrdup( STRPTR string );
char *getpubscreen( void );
void playsound( char *file, ULONG vol );

//
// tcp.c
//

//ULONG tcp_gethostid( struct mysock *s );
struct mysock *tcp_socket( void );
void tcp_close( struct mysock *s );
int tcp_lookup( struct mysock *s, char *hostname, char *canname, ULONG *ip );
char *tcp_ntoa( struct mysock *s, ULONG ip );
int tcp_error( struct mysock *s, char *errstr );
int tcp_connect( struct mysock *s, ULONG ip, ULONG port );
void __stdargs tcp_sendfmt( struct mysock *s, char *fmt, ... );
int tcp_send( struct mysock *s, char *data, LONG len );
//int tcp_sendoob( struct mysock *s, char *data, LONG len );
int tcp_recv( struct mysock *s, char *to, int len );
int tcp_waitsel( struct mysock *s, ULONG *sigs );
int tcp_waitselwrite( struct mysock *s, ULONG *sigs );
int tcp_bindlisten( struct mysock *s, int port, int listval );
struct mysock *tcp_accept( struct mysock *s, ULONG *remoteip, ULONG *remoteport );
int tcp_lookupip( struct mysock *s, ULONG ip, char *canname );
int tcp_isread( struct mysock *s );
void tcp_stackid( char *to );
void tcp_gethostname( struct mysock *s, char *to );
ULONG tcp_getmyip( struct mysock *s );
unsigned short tcp_getmyport( struct mysock *s );

//
// mui.c
//

APTR makebutton( ULONG stringid );
int getmenucheck( ULONG menuid );
void mui_set( APTR obj, ULONG attr, LONG val );
#undef set
#define set(o,a,v) mui_set(o,a,(LONG)v)
void mui_get( APTR obj, ULONG attr, LONG *valp );
#undef get
#define get(o,a,v) mui_get(o,a,(LONG*)v)
void mui_nnset( APTR obj, ULONG attr, LONG val );
#undef nnset
#define nnset(o,a,v) mui_nnset(o,a,(LONG)v)
void mui_setstring( APTR obj, STRPTR str );
#define setstr(o,s) mui_setstring(o,s)
void mui_getstring( APTR obj, STRPTR *strp );
#define getstr(o,s) mui_getstring(o,s)
void mui_settxt( APTR obj, STRPTR txt );
#define settxt(o,s) mui_settxt(o,s)
void mui_gettxt( APTR obj, STRPTR *txtp );
#define gettxt(o,s) mui_gettxt(o,s)
STRPTR mui_getstrptr( APTR obj );
#define getstrp(o) mui_getstrptr(o)

//
// about.c
//

void showabout( void );
void removeabout( void );

//
// init.c
//

extern BOOL dontcompress;
extern BOOL inaction;
extern BOOL mynohome;
APTR button( APTR label, APTR helpid );
APTR buttonw0( APTR label, APTR helpid );
APTR buttontw0( APTR label, APTR helpid );
APTR string( char *defstring, int maxlen, ULONG exid, APTR helpid );
extern ULONG nocache;
char *expandcompress( char *filename );
extern void __saveds __asm profile_act( register __a1 LONG *act );
void setconnectstate( void );
void __saveds setlayout( void );
void initxferwin( char *localfilename, char *remotefilename, LONG filesize );
void exitxferwin( void );
void __saveds __asm beginreceivelist( register __a1 ULONG *view );
void beginsendlist( void );
void addhis( APTR lv, char *s );
void __saveds __asm batch( register __a1 long *dropmark );
void __saveds __asm batchadt( register __a1 long *dropmark );
void __saveds __asm batchdownload( void );
void __saveds __asm upload( void );
void __saveds __asm download( register __a1 ULONG *view );
void __saveds __asm readmeadt( register __a1 ULONG *click );

//
// main.c
//

void readlocal( void );
void readremote( void );
void displayfile( char *filename );
void __stdargs addlog( char *, ... );
time_t time( time_t *tp );
time_t timev( void );
clock_t clock( void );


//
// ftp.c
//

char *addtasknr( const char *filename, BOOL addz );
ULONG getdiskspacebytes( char *dev );
void startconnect( void );
void closesocks( void );
void dotcpread( void );
void startxfer( ULONG lcSendmode, char *cmd, char *expectfile, char *remotename, ULONG len );

//
// dir.c
//

void parsedir( char *filename );
void calclinfo( int which );
void calclbatch( void );

//
// profile.c
//

#include "prefs/struct.h"
extern struct amftpprefs config;
void addprofile( char *label, char *host, char *dir );
BOOL loadprofiles( char *name );
BOOL saveprofiles( BOOL force );
BOOL loadenv( void );
BOOL loadrecent( void );
void saverecent( void );
void profilesort( void );
void profileremove( void );
void newprofile( void );
void copyprofile( void );
void importprofiles( void );
BOOL loadweights( void );
BOOL saveweights( void );

extern ULONG sortmode;
extern ULONG timemode;
extern ULONG showallrecent;

struct adtentry
{
	char	name[ 24 ];
	char	dir[ 12 ];
	char	size[ 10 ];
	char	desc[ 80 ];
	long	time;					// >>3
	long	readmesize;
};

extern long newestrecenttime;
extern long myrecenttime;

//
// recent.c
//

void parserecent( char *filename );
//extern ULONG lastcall;	

//
// logo.c
//

extern ULONG amftpCMap32[];
extern struct BitMap amftpBitMap;

//
// noop.c
//

#define REG(x) register __ ## x
extern BOOL transferflag;
struct NoopData
{
	struct MsgPort     *port;
	struct timerequest *req;

	struct MUI_InputHandlerNode ihnode;

	LONG index;
};
extern __saveds __asm ULONG MyDispatcher(REG(a0) struct IClass *cl,REG(a2) Object *obj,REG(a1) Msg msg);

//
// dd.c
//

extern APTR l_batch, l_local, l_remote, l_adt;
APTR getddlistclass( void );
int InitDDList( void );
void DeleteDDList( void );

//
// cache.c
//

char *getcacheentry( char *hostname, char *directory );
void newcacheentry( char *hostname, char *directory, char *filename );
void updatecacheentry( char *hostname, char *directory, char *filename );

//
// archie.c
//

extern APTR bt_afind, win_archie;
void __saveds __asm archiehook( void );
extern char ahostname[ 256 ];

//
// adtfind.c
//

void __saveds __asm adtfindhook( void );
extern APTR bt_gofind, win_adtfind;
extern char afhostname[ 256 ];

//
// connect.c
//

extern struct Process *mainproc;
void openconnectwin( char *hostname, char *port );
void closeconnectwin( void );
void connectwinworking( void );

//
// login.c
//

extern char ext_username[ 64 ], ext_passwort[ 64 ];
BOOL openloginwin( void );

//
// compress.c
//

BOOL uncompress( char *inname );

//
// adtdir.c
//

BOOL open_win_adtdir( void );
void close_win_adtdir( void );
void getdirsfromadt( void );

//
// mime.c
//

BOOL mime_view( const char * filename );
void ExpandTemplate( char *from, char *to, char *filename, char *pubname );
