#ifndef LOCALE_H
#define LOCALE_H


/****************************************************************************/


/* This file was created automatically by CatComp.
 * Do NOT edit by hand!
 */


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef CATCOMP_ARRAY
#undef CATCOMP_NUMBERS
#undef CATCOMP_STRINGS
#define CATCOMP_NUMBERS
#define CATCOMP_STRINGS
#endif

#ifdef CATCOMP_BLOCK
#undef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif


/****************************************************************************/


#ifdef CATCOMP_NUMBERS

#define MSG_APP_DESCRIPTION 0
#define GAD_ADD_GROUP 1
#define GAD_ADD_ENTRY 2
#define GAD_CLONE_ENTRY 3
#define GAD_REMOVE 4
#define GAD_REM_GROUP 5
#define WND_ADD_GROUP 6
#define GAD_GROUP_NAME 7
#define GAD_OK 8
#define GAD_CANCEL 9
#define MSG_USERS 10
#define MSG_FTP 11
#define MSG_WWW 12
#define MSG_CHAT 13
#define MSG_NAME 14
#define MSG_FIRST_NAME 15
#define MSG_LAST_NAME 16
#define MSG_ADDRESS 17
#define MSG_CITY 18
#define MSG_COUNTRY 19
#define MSG_ZIP 20
#define MSG_COMMENT 21
#define GAD_PHONE 22
#define MSG_FAX 23
#define MSG_EMAIL 24
#define GAD_WWW 25
#define GAD_FTP 26
#define GAD_TO_WWW 27
#define GAD_TO_FTP 28
#define GAD_TO_EMAIL 29
#define GAD_TO_PHONE 30
#define GAD_TO_FAX 31
#define MSG_TO_DOPUS 32
#define HELP_REMOVE 33
#define HELP_ADD_ENTRY 34
#define HELP_CLONE_ENTRY 35
#define HELP_EDIT 36
#define HELP_SORT 37
#define MENU_PROJECT 38
#define MENU_ABOUT 39
#define KEY_ABOUT 40
#define MENU_ABOUT_MUI 41
#define MENU_OPEN 42
#define KEY_OPEN 43
#define MENU_SAVE 44
#define KEY_SAVE 45
#define MENU_SAVE_AS 46
#define KEY_SAVE_AS 47
#define MENU_QUIT 48
#define KEY_QUIT 49
#define GAD_ADVANCED_OPTIONS 50
#define GAD_ANONYMOUS_LOGIN 51
#define GAD_USERNAME 52
#define GAD_PASSWORD 53
#define GAD_ADT 54
#define GAD_QUIET 55
#define GAD_RETRIES 56
#define GAD_COMPRESSION 57
#define GAD_PORT 58
#define HDR_NAME 59
#define HDR_COMMENT 60
#define HDR_SITE 61
#define HDR_EMAIL 62
#define WND_EDIT_USER 63
#define WND_EDIT_WWW 64
#define WND_EDIT_FTP 65
#define GAD_EDIT 66
#define GAD_SEARCH 67
#define GAD_FROM_TOP 68
#define GAD_BACKWARDS 69
#define MENU_OPTIONS 70
#define MENU_SEARCH 71
#define GAD_SEARCH_FOR 72
#define MENU_IMPORT 73
#define MENU_V_HOTLIST 74
#define MENU_THOR_USERS 75
#define MENU_IB_HOTLIST 76
#define MENU_AWEB_HOTLIST 77
#define MENU_AMFTP_PROFILES 78
#define MENU_DOPUS_FTP 79
#define MENU_MD_ADDRESSES 80
#define MENU_YAM_ADDRESSES 81
#define MENU_STFAX_PHONEBOOK 82
#define MENU_AMIRC_SERVERS 83
#define MENU_SETTINGS 84
#define MENU_CMANAGER 85
#define MENU_MUI 86
#define GAD_ALIAS 87
#define GAD_SORT 88
#define MSG_ABOUT 89
#define GAD_MORE 90
#define MSG_REGISTERED 91
#define GAD_WEBMASTER 92
#define MSG_NOT_SAVED 93
#define ANS_SAVE_QUIT_CANCEL 94
#define MENU_JOIN_CHANNEL 95
#define HDR_CHANNEL 96
#define HDR_SERVER 97
#define GAD_CHANNEL 98
#define GAD_SERVER 99
#define GAD_MAINTAINER 100
#define GAD_NICK 101
#define GAD_WEB_SITE 102
#define WND_EDIT_CHAT 103
#define MSG_PATHS 104
#define GAD_V 105
#define GAD_IB 106
#define GAD_MD2 107
#define GAD_YAM 108
#define GAD_STFAX 109
#define GAD_AWEB 110
#define GAD_AMFTP 111
#define GAD_AMIRC 112
#define GAD_USE 113
#define GAD_SAVE 114
#define GAD_SILENT_SCAN 115
#define HELP_SILENT_SCAN 116
#define GAD_FROM_LOCALE 117
#define HELP_COUNTRY 118
#define GAD_COUNTRY_CODE 119
#define HELP_COUNTRY_CODE 120
#define MSG_HOTLIST_CHANGED 121
#define ANS_OK_CANCEL 122
#define HELP_USER_IMAGE 123
#define GAD_MOBILE 124
#define MSG_ON_DOUBLE_CLICK 125
#define HELP_DOUBLE_CLICK 126
#define MSG_EDIT_ENTRIES 127
#define MSG_START_SCRIPT 128
#define MSG_SCRIPT_AND_CLOSE 129
#define GAD_DOPUSFTP 130
#define MSG_DOPUS_FTP 131
#define MSG_LOCAL_LISTER 132
#define MSG_DEFAULT_FTP 133
#define GAD_OPEN_URL 134
#define MSG_DIALLER 135
#define GAD_USE_DIALLER 136
#define MSG_USE_EXT_DIALER 137
#define GAD_DEVICE 138
#define GAD_UNIT 139
#define GAD_SHARED 140
#define GAD_INIT_STRING 141
#define GAD_DIAL_COMMAND 142
#define MSG_PROGRAMS 143
#define GAD_MAILER 144
#define MSG_SCRIPT 145
#define MSG_PATH 146
#define GAD_IRC 147
#define GAD_PROGRAM 148
#define MSG_LOCALE 149
#define MSG_MISC 150
#define MSG_DIALER_PLACEHOLDERS 151
#define MSG_DIALLING 152
#define ERR_NO_DEVICE 153
#define GAD_REDIAL_DELAY 154
#define GAD_REDIAL_ATTEMPTS 155
#define MSG_WAITING 156
#define ANS_RETRY_CANCEL 157
#define MSG_PHONE 158
#define MSG_MOBILE 159
#define GAD_MODEM_TYPE 160
#define ERR_MODEM_NOT_RESPONDING 161
#define GAD_ICQ 162
#define MSG_TO_CLIP 163
#define MSG_HOME 164
#define MSG_BUSINESS 165
#define MSG_NOTES 166
#define GAD_DEPARTMENT 167
#define GAD_JOB_TITLE 168
#define GAD_OFFICE 169
#define GAD_COMPANY 170
#define MSG_LIMIT 171
#define MENU_DFA_ADDRESSES 172
#define GAD_DFA 173
#define MENU_UMS_GOODNEWS 174
#define GAD_GOODNEWS 175
#define MENU_UMS_PINT 176
#define GAD_UMS 177
#define GAD_SAVE_ON_EXIT 178
#define HELP_SAVE_ON_EXIT 179
#define MENU_STRICQ_USERS 180
#define GAD_STRICQ_USERS 181
#define MSG_SCRIPT_AND_ICONIFY 182
#define GAD_PGP_ID 183
#define HDR_LAST_NAME 184
#define GAD_DONT_MOVE_COLUMNS 185
#define HELP_DONT_MOVE_COLUMNS 186
#define GAD_LOG_CALLS 187
#define MSG_BUSINESS_MOBILE 188
#define HDR_LAST_MODIFIED 189
#define HDR_LAST_VISITED 190
#define MSG_TELNET 191
#define HDR_HOST 192
#define GAD_HOST 193
#define GAD_LOGIN 194
#define WND_EDIT_TELNET 195
#define GAD_SSH 196
#define MENU_EXPORT 197
#define MENU_EXPORT_URL_HTML 198
#define MENU_EXPORT_WWW_HTML 199
#define MENU_EXPORT_FTP_HTML 200
#define MENU_EXPORT_USERS_CSV 201
#define MSG_GET_DB_DIR 202
#define ANS_SELECT_CANCEL 203
#define MSG_NO_DIR_WARNING 204
#define GAD_SAVE_ON_CHANGES 205
#define HELP_SAVE_ON_CHANGES 206
#define MGS_ADVANCED 207
#define GAD_WINDOW_TYPE 208
#define MSG_SINGLE_LIST 209
#define MSG_DUAL_LIST 210
#define GAD_USE_PROXY 211
#define GAD_SHOW_HIDDEN 212
#define GAD_SHOW_FTP_OUTPUT 213
#define GAD_SEND_NOOP 214
#define MSG_TOOLBAR 215

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define MSG_APP_DESCRIPTION_STR "Global bookmarks manager"
#define GAD_ADD_GROUP_STR "_Add"
#define GAD_ADD_ENTRY_STR "Add Entr_y"
#define GAD_CLONE_ENTRY_STR "_Clone Entry"
#define GAD_REMOVE_STR "_Remove"
#define GAD_REM_GROUP_STR "R_emove"
#define WND_ADD_GROUP_STR "Add group"
#define GAD_GROUP_NAME_STR "Group _name:"
#define GAD_OK_STR "_Ok"
#define GAD_CANCEL_STR "_Cancel"
#define MSG_USERS_STR "Users"
#define MSG_FTP_STR "FTP Sites"
#define MSG_WWW_STR "WWW Sites"
#define MSG_CHAT_STR "Chat"
#define MSG_NAME_STR "_Name"
#define MSG_FIRST_NAME_STR "First _Name"
#define MSG_LAST_NAME_STR "Last Na_me"
#define MSG_ADDRESS_STR "Addres_s"
#define MSG_CITY_STR "Cit_y"
#define MSG_COUNTRY_STR "Co_untry"
#define MSG_ZIP_STR "_ZIP"
#define MSG_COMMENT_STR "Co_mment"
#define GAD_PHONE_STR "_Phone"
#define MSG_FAX_STR "Fa_x"
#define MSG_EMAIL_STR "_E-Mail"
#define GAD_WWW_STR "_WWW"
#define GAD_FTP_STR "_FTP"
#define GAD_TO_WWW_STR "Send to Browser"
#define GAD_TO_FTP_STR "Send to FTP"
#define GAD_TO_EMAIL_STR "Send EMail"
#define GAD_TO_PHONE_STR "Dial"
#define GAD_TO_FAX_STR "Send Fax"
#define MSG_TO_DOPUS_STR "Send DOpus FTP"
#define HELP_REMOVE_STR "Remove the selected entry"
#define HELP_ADD_ENTRY_STR "Add a new entry to the list"
#define HELP_CLONE_ENTRY_STR "Duplicate the selected entries"
#define HELP_EDIT_STR "Open the Edit window for the selected entries"
#define HELP_SORT_STR "Sort alphabetically the list"
#define MENU_PROJECT_STR "Project"
#define MENU_ABOUT_STR "About..."
#define KEY_ABOUT_STR "?"
#define MENU_ABOUT_MUI_STR "About MUI..."
#define MENU_OPEN_STR "Open..."
#define KEY_OPEN_STR "O"
#define MENU_SAVE_STR "Save"
#define KEY_SAVE_STR "S"
#define MENU_SAVE_AS_STR "Save as..."
#define KEY_SAVE_AS_STR "A"
#define MENU_QUIT_STR "Quit"
#define KEY_QUIT_STR "Q"
#define GAD_ADVANCED_OPTIONS_STR "_Advanced options"
#define GAD_ANONYMOUS_LOGIN_STR "Anon_ymous login"
#define GAD_USERNAME_STR "_Username"
#define GAD_PASSWORD_STR "_Password"
#define GAD_ADT_STR "A_DT server"
#define GAD_QUIET_STR "_Quiet"
#define GAD_RETRIES_STR "_Retries"
#define GAD_COMPRESSION_STR "_Compression"
#define GAD_PORT_STR "P_ort"
#define HDR_NAME_STR "Name"
#define HDR_COMMENT_STR "Comment"
#define HDR_SITE_STR "Site"
#define HDR_EMAIL_STR "EMail"
#define WND_EDIT_USER_STR "Edit user"
#define WND_EDIT_WWW_STR "Edit WWW site data"
#define WND_EDIT_FTP_STR "Edit FTP site data"
#define GAD_EDIT_STR "E_dit"
#define GAD_SEARCH_STR "_Search"
#define GAD_FROM_TOP_STR "From _top"
#define GAD_BACKWARDS_STR "_Backwards"
#define MENU_OPTIONS_STR "Database"
#define MENU_SEARCH_STR "Search..."
#define GAD_SEARCH_FOR_STR "Search _for:"
#define MENU_IMPORT_STR "Import"
#define MENU_V_HOTLIST_STR "Voyager Hotlist..."
#define MENU_THOR_USERS_STR "THOR User list..."
#define MENU_IB_HOTLIST_STR "IBrowse Hotlist..."
#define MENU_AWEB_HOTLIST_STR "AWeb Hotlist..."
#define MENU_AMFTP_PROFILES_STR "AmFTP Profiles..."
#define MENU_DOPUS_FTP_STR "DOpus FTP sites"
#define MENU_MD_ADDRESSES_STR "Microdot 2 Addressbook..."
#define MENU_YAM_ADDRESSES_STR "YAM Addressbook..."
#define MENU_STFAX_PHONEBOOK_STR "STFax Phonebook..."
#define MENU_AMIRC_SERVERS_STR "AmIRC Servers..."
#define MENU_SETTINGS_STR "Settings"
#define MENU_CMANAGER_STR "Contact Manager..."
#define MENU_MUI_STR "MUI..."
#define GAD_ALIAS_STR "_Alias"
#define GAD_SORT_STR "_Sort"
#define MSG_ABOUT_STR "Contact Manager %ld.%ld\n©1998-1999 by Simone Tellini\n\nThanks to Oliver 'Olli' Wagner for his help.\n\nSpecial greetings to Federico 'x-Fede' Pomi,\nLuca 'ElleDi' Danelon, Gian Maria 'Giangi' Calzolari,\nthe \"This doesn't werk, you suck!\" team,\nAlex 'Leto' Pagnoni and all the #amigaita guys.\n\nBased on an idea of Chris Wiles."
#define GAD_MORE_STR "_More"
#define MSG_REGISTERED_STR "This copy of CManager is registered to\n%s (%ld)"
#define GAD_WEBMASTER_STR "Web_Master"
#define MSG_NOT_SAVED_STR "Some changes have been made!\nWhat do you want to do?"
#define ANS_SAVE_QUIT_CANCEL_STR "*_Save|_Quit|_Cancel"
#define MENU_JOIN_CHANNEL_STR "Join channel"
#define HDR_CHANNEL_STR "Channel"
#define HDR_SERVER_STR "Server"
#define GAD_CHANNEL_STR "C_hannel"
#define GAD_SERVER_STR "_Server"
#define GAD_MAINTAINER_STR "_Maintainer"
#define GAD_NICK_STR "_Nick"
#define GAD_WEB_SITE_STR "_Web Site"
#define WND_EDIT_CHAT_STR "Edit IRC Channel"
#define MSG_PATHS_STR "Hotlists"
#define GAD_V_STR "_Voyager"
#define GAD_IB_STR "_IBrowse"
#define GAD_MD2_STR "_MicroDot II"
#define GAD_YAM_STR "_YAM"
#define GAD_STFAX_STR "ST_Fax"
#define GAD_AWEB_STR "_AWeb"
#define GAD_AMFTP_STR "Am_FTP"
#define GAD_AMIRC_STR "AmI_RC"
#define GAD_USE_STR "_Use"
#define GAD_SAVE_STR "_Save"
#define GAD_SILENT_SCAN_STR "Si_lent Scan"
#define HELP_SILENT_SCAN_STR "If set, Contact Manager will re-import your\nbookmarks if they have changed without asking\nfor confirmation."
#define GAD_FROM_LOCALE_STR "_Get from Locale"
#define HELP_COUNTRY_STR "Enter here the name of the country you live in"
#define GAD_COUNTRY_CODE_STR "Country Code"
#define HELP_COUNTRY_CODE_STR "Your country telephone prefix"
#define MSG_HOTLIST_CHANGED_STR "The hotlist \"%s\" has changed.\nShall I import it again?"
#define ANS_OK_CANCEL_STR "*_Ok|_Cancel"
#define HELP_USER_IMAGE_STR "Click here to change the image"
#define GAD_MOBILE_STR "Mo_bile"
#define MSG_ON_DOUBLE_CLICK_STR "_Double-click:"
#define HELP_DOUBLE_CLICK_STR "Action to perform when double-clicking on FTP or WWW entries"
#define MSG_EDIT_ENTRIES_STR "to edit WWW/FTP entries"
#define MSG_START_SCRIPT_STR "to start ARexx script"
#define MSG_SCRIPT_AND_CLOSE_STR "to start ARexx script and quit"
#define GAD_DOPUSFTP_STR "DOp_us FTP"
#define MSG_DOPUS_FTP_STR "DOpus FTP"
#define MSG_LOCAL_LISTER_STR "_Local lister"
#define MSG_DEFAULT_FTP_STR "_Default FTP target"
#define GAD_OPEN_URL_STR "Use _OpenURL library"
#define MSG_DIALLER_STR "Dialer"
#define GAD_USE_DIALLER_STR "Use internal dialer"
#define MSG_USE_EXT_DIALER_STR "Use external dial interface"
#define GAD_DEVICE_STR "_Device"
#define GAD_UNIT_STR "Uni_t"
#define GAD_SHARED_STR "S_hared"
#define GAD_INIT_STRING_STR "_Modem init"
#define GAD_DIAL_COMMAND_STR "Dial c_ommand"
#define MSG_PROGRAMS_STR "Programs"
#define GAD_MAILER_STR "_Mailer"
#define MSG_SCRIPT_STR "Script"
#define MSG_PATH_STR "Path"
#define GAD_IRC_STR "_IRC"
#define GAD_PROGRAM_STR "_Program"
#define MSG_LOCALE_STR "Locale"
#define MSG_MISC_STR "Misc"
#define MSG_DIALER_PLACEHOLDERS_STR "(%s will be replaced with the phone number to dial)"
#define MSG_DIALLING_STR "Dialling %s...\n(attempt %ld/%ld)"
#define ERR_NO_DEVICE_STR "Cannot open unit %ld of %s!"
#define GAD_REDIAL_DELAY_STR "_Redial Delay"
#define GAD_REDIAL_ATTEMPTS_STR "R_edial Attempts"
#define MSG_WAITING_STR "Waiting... (%ld)"
#define ANS_RETRY_CANCEL_STR "*_Retry|_Cancel"
#define MSG_PHONE_STR "Phone"
#define MSG_MOBILE_STR "Mobile"
#define GAD_MODEM_TYPE_STR "Modem t_ype"
#define ERR_MODEM_NOT_RESPONDING_STR "No response from the modem.\nHave you turned it on?"
#define GAD_ICQ_STR "IC_Q"
#define MSG_TO_CLIP_STR "Copy to clipboard"
#define MSG_HOME_STR "Home"
#define MSG_BUSINESS_STR "Business"
#define MSG_NOTES_STR "Notes"
#define GAD_DEPARTMENT_STR "_Department:"
#define GAD_JOB_TITLE_STR "_Job Title:"
#define GAD_OFFICE_STR "O_ffice:"
#define GAD_COMPANY_STR "Compan_y:"
#define MSG_LIMIT_STR "Sorry, you have reached the limit for\nthe demo version.\n\nPlease consider registering."
#define MENU_DFA_ADDRESSES_STR "DFA Addresses..."
#define GAD_DFA_STR "_DFA"
#define MENU_UMS_GOODNEWS_STR "UMS - GoodNews Addresses..."
#define GAD_GOODNEWS_STR "UMS - GoodNe_ws"
#define MENU_UMS_PINT_STR "UMS - PINT Addresses..."
#define GAD_UMS_STR "U_MS - PINT"
#define GAD_SAVE_ON_EXIT_STR "Save on e_xit"
#define HELP_SAVE_ON_EXIT_STR "If set Contact Manager will automatically\nsave the database when quitting, if changes\nwere done."
#define MENU_STRICQ_USERS_STR "STRICQ Users..."
#define GAD_STRICQ_USERS_STR "STRIC_Q Users"
#define MSG_SCRIPT_AND_ICONIFY_STR "to start ARexx script and iconify"
#define GAD_PGP_ID_STR "_PGP UserID"
#define HDR_LAST_NAME_STR "Last name"
#define GAD_DONT_MOVE_COLUMNS_STR "Don't move col_umns"
#define HELP_DONT_MOVE_COLUMNS_STR "If set, the column order in the user section\nwon't change according to the sort criteria used."
#define GAD_LOG_CALLS_STR "Log to GenesisReport"
#define MSG_BUSINESS_MOBILE_STR "Business mobile"
#define HDR_LAST_MODIFIED_STR "Last modified"
#define HDR_LAST_VISITED_STR "Last visited"
#define MSG_TELNET_STR "Telnet"
#define HDR_HOST_STR "Host"
#define GAD_HOST_STR "_Host"
#define GAD_LOGIN_STR "_Login"
#define WND_EDIT_TELNET_STR "Edit Telnet Host"
#define GAD_SSH_STR "_SSH"
#define MENU_EXPORT_STR "Export"
#define MENU_EXPORT_URL_HTML_STR "URLs as HTML..."
#define MENU_EXPORT_WWW_HTML_STR "Web sites as HTML..."
#define MENU_EXPORT_FTP_HTML_STR "FTP sites as HTML..."
#define MENU_EXPORT_USERS_CSV_STR "Users as CSV..."
#define MSG_GET_DB_DIR_STR "Select the directory for CM's database..."
#define ANS_SELECT_CANCEL_STR "*_Select|_Cancel"
#define MSG_NO_DIR_WARNING_STR "Warning! CManager won't work properly if\nyou don't provide a directory where to store\nits databases!\n\nDo you want to select the directory now?"
#define GAD_SAVE_ON_CHANGES_STR "Save on c_hanges"
#define HELP_SAVE_ON_CHANGES_STR "If set, CManager will save the database after\nevery change."
#define MGS_ADVANCED_STR "Advanced"
#define GAD_WINDOW_TYPE_STR "_Window type"
#define MSG_SINGLE_LIST_STR "Single list"
#define MSG_DUAL_LIST_STR "Dual list"
#define GAD_USE_PROXY_STR "Use pro_xy"
#define GAD_SHOW_HIDDEN_STR "Show _hidden files"
#define GAD_SHOW_FTP_OUTPUT_STR "Show ftp o_utput"
#define GAD_SEND_NOOP_STR "Send _NOOPs"
#define MSG_TOOLBAR_STR "Toolbar"

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG   cca_ID;
    STRPTR cca_Str;
};

             struct CatCompArrayType CatCompArray[] =
{
    {MSG_APP_DESCRIPTION,(STRPTR)MSG_APP_DESCRIPTION_STR},
    {GAD_ADD_GROUP,(STRPTR)GAD_ADD_GROUP_STR},
    {GAD_ADD_ENTRY,(STRPTR)GAD_ADD_ENTRY_STR},
    {GAD_CLONE_ENTRY,(STRPTR)GAD_CLONE_ENTRY_STR},
    {GAD_REMOVE,(STRPTR)GAD_REMOVE_STR},
    {GAD_REM_GROUP,(STRPTR)GAD_REM_GROUP_STR},
    {WND_ADD_GROUP,(STRPTR)WND_ADD_GROUP_STR},
    {GAD_GROUP_NAME,(STRPTR)GAD_GROUP_NAME_STR},
    {GAD_OK,(STRPTR)GAD_OK_STR},
    {GAD_CANCEL,(STRPTR)GAD_CANCEL_STR},
    {MSG_USERS,(STRPTR)MSG_USERS_STR},
    {MSG_FTP,(STRPTR)MSG_FTP_STR},
    {MSG_WWW,(STRPTR)MSG_WWW_STR},
    {MSG_CHAT,(STRPTR)MSG_CHAT_STR},
    {MSG_NAME,(STRPTR)MSG_NAME_STR},
    {MSG_FIRST_NAME,(STRPTR)MSG_FIRST_NAME_STR},
    {MSG_LAST_NAME,(STRPTR)MSG_LAST_NAME_STR},
    {MSG_ADDRESS,(STRPTR)MSG_ADDRESS_STR},
    {MSG_CITY,(STRPTR)MSG_CITY_STR},
    {MSG_COUNTRY,(STRPTR)MSG_COUNTRY_STR},
    {MSG_ZIP,(STRPTR)MSG_ZIP_STR},
    {MSG_COMMENT,(STRPTR)MSG_COMMENT_STR},
    {GAD_PHONE,(STRPTR)GAD_PHONE_STR},
    {MSG_FAX,(STRPTR)MSG_FAX_STR},
    {MSG_EMAIL,(STRPTR)MSG_EMAIL_STR},
    {GAD_WWW,(STRPTR)GAD_WWW_STR},
    {GAD_FTP,(STRPTR)GAD_FTP_STR},
    {GAD_TO_WWW,(STRPTR)GAD_TO_WWW_STR},
    {GAD_TO_FTP,(STRPTR)GAD_TO_FTP_STR},
    {GAD_TO_EMAIL,(STRPTR)GAD_TO_EMAIL_STR},
    {GAD_TO_PHONE,(STRPTR)GAD_TO_PHONE_STR},
    {GAD_TO_FAX,(STRPTR)GAD_TO_FAX_STR},
    {MSG_TO_DOPUS,(STRPTR)MSG_TO_DOPUS_STR},
    {HELP_REMOVE,(STRPTR)HELP_REMOVE_STR},
    {HELP_ADD_ENTRY,(STRPTR)HELP_ADD_ENTRY_STR},
    {HELP_CLONE_ENTRY,(STRPTR)HELP_CLONE_ENTRY_STR},
    {HELP_EDIT,(STRPTR)HELP_EDIT_STR},
    {HELP_SORT,(STRPTR)HELP_SORT_STR},
    {MENU_PROJECT,(STRPTR)MENU_PROJECT_STR},
    {MENU_ABOUT,(STRPTR)MENU_ABOUT_STR},
    {KEY_ABOUT,(STRPTR)KEY_ABOUT_STR},
    {MENU_ABOUT_MUI,(STRPTR)MENU_ABOUT_MUI_STR},
    {MENU_OPEN,(STRPTR)MENU_OPEN_STR},
    {KEY_OPEN,(STRPTR)KEY_OPEN_STR},
    {MENU_SAVE,(STRPTR)MENU_SAVE_STR},
    {KEY_SAVE,(STRPTR)KEY_SAVE_STR},
    {MENU_SAVE_AS,(STRPTR)MENU_SAVE_AS_STR},
    {KEY_SAVE_AS,(STRPTR)KEY_SAVE_AS_STR},
    {MENU_QUIT,(STRPTR)MENU_QUIT_STR},
    {KEY_QUIT,(STRPTR)KEY_QUIT_STR},
    {GAD_ADVANCED_OPTIONS,(STRPTR)GAD_ADVANCED_OPTIONS_STR},
    {GAD_ANONYMOUS_LOGIN,(STRPTR)GAD_ANONYMOUS_LOGIN_STR},
    {GAD_USERNAME,(STRPTR)GAD_USERNAME_STR},
    {GAD_PASSWORD,(STRPTR)GAD_PASSWORD_STR},
    {GAD_ADT,(STRPTR)GAD_ADT_STR},
    {GAD_QUIET,(STRPTR)GAD_QUIET_STR},
    {GAD_RETRIES,(STRPTR)GAD_RETRIES_STR},
    {GAD_COMPRESSION,(STRPTR)GAD_COMPRESSION_STR},
    {GAD_PORT,(STRPTR)GAD_PORT_STR},
    {HDR_NAME,(STRPTR)HDR_NAME_STR},
    {HDR_COMMENT,(STRPTR)HDR_COMMENT_STR},
    {HDR_SITE,(STRPTR)HDR_SITE_STR},
    {HDR_EMAIL,(STRPTR)HDR_EMAIL_STR},
    {WND_EDIT_USER,(STRPTR)WND_EDIT_USER_STR},
    {WND_EDIT_WWW,(STRPTR)WND_EDIT_WWW_STR},
    {WND_EDIT_FTP,(STRPTR)WND_EDIT_FTP_STR},
    {GAD_EDIT,(STRPTR)GAD_EDIT_STR},
    {GAD_SEARCH,(STRPTR)GAD_SEARCH_STR},
    {GAD_FROM_TOP,(STRPTR)GAD_FROM_TOP_STR},
    {GAD_BACKWARDS,(STRPTR)GAD_BACKWARDS_STR},
    {MENU_OPTIONS,(STRPTR)MENU_OPTIONS_STR},
    {MENU_SEARCH,(STRPTR)MENU_SEARCH_STR},
    {GAD_SEARCH_FOR,(STRPTR)GAD_SEARCH_FOR_STR},
    {MENU_IMPORT,(STRPTR)MENU_IMPORT_STR},
    {MENU_V_HOTLIST,(STRPTR)MENU_V_HOTLIST_STR},
    {MENU_THOR_USERS,(STRPTR)MENU_THOR_USERS_STR},
    {MENU_IB_HOTLIST,(STRPTR)MENU_IB_HOTLIST_STR},
    {MENU_AWEB_HOTLIST,(STRPTR)MENU_AWEB_HOTLIST_STR},
    {MENU_AMFTP_PROFILES,(STRPTR)MENU_AMFTP_PROFILES_STR},
    {MENU_DOPUS_FTP,(STRPTR)MENU_DOPUS_FTP_STR},
    {MENU_MD_ADDRESSES,(STRPTR)MENU_MD_ADDRESSES_STR},
    {MENU_YAM_ADDRESSES,(STRPTR)MENU_YAM_ADDRESSES_STR},
    {MENU_STFAX_PHONEBOOK,(STRPTR)MENU_STFAX_PHONEBOOK_STR},
    {MENU_AMIRC_SERVERS,(STRPTR)MENU_AMIRC_SERVERS_STR},
    {MENU_SETTINGS,(STRPTR)MENU_SETTINGS_STR},
    {MENU_CMANAGER,(STRPTR)MENU_CMANAGER_STR},
    {MENU_MUI,(STRPTR)MENU_MUI_STR},
    {GAD_ALIAS,(STRPTR)GAD_ALIAS_STR},
    {GAD_SORT,(STRPTR)GAD_SORT_STR},
    {MSG_ABOUT,(STRPTR)MSG_ABOUT_STR},
    {GAD_MORE,(STRPTR)GAD_MORE_STR},
    {MSG_REGISTERED,(STRPTR)MSG_REGISTERED_STR},
    {GAD_WEBMASTER,(STRPTR)GAD_WEBMASTER_STR},
    {MSG_NOT_SAVED,(STRPTR)MSG_NOT_SAVED_STR},
    {ANS_SAVE_QUIT_CANCEL,(STRPTR)ANS_SAVE_QUIT_CANCEL_STR},
    {MENU_JOIN_CHANNEL,(STRPTR)MENU_JOIN_CHANNEL_STR},
    {HDR_CHANNEL,(STRPTR)HDR_CHANNEL_STR},
    {HDR_SERVER,(STRPTR)HDR_SERVER_STR},
    {GAD_CHANNEL,(STRPTR)GAD_CHANNEL_STR},
    {GAD_SERVER,(STRPTR)GAD_SERVER_STR},
    {GAD_MAINTAINER,(STRPTR)GAD_MAINTAINER_STR},
    {GAD_NICK,(STRPTR)GAD_NICK_STR},
    {GAD_WEB_SITE,(STRPTR)GAD_WEB_SITE_STR},
    {WND_EDIT_CHAT,(STRPTR)WND_EDIT_CHAT_STR},
    {MSG_PATHS,(STRPTR)MSG_PATHS_STR},
    {GAD_V,(STRPTR)GAD_V_STR},
    {GAD_IB,(STRPTR)GAD_IB_STR},
    {GAD_MD2,(STRPTR)GAD_MD2_STR},
    {GAD_YAM,(STRPTR)GAD_YAM_STR},
    {GAD_STFAX,(STRPTR)GAD_STFAX_STR},
    {GAD_AWEB,(STRPTR)GAD_AWEB_STR},
    {GAD_AMFTP,(STRPTR)GAD_AMFTP_STR},
    {GAD_AMIRC,(STRPTR)GAD_AMIRC_STR},
    {GAD_USE,(STRPTR)GAD_USE_STR},
    {GAD_SAVE,(STRPTR)GAD_SAVE_STR},
    {GAD_SILENT_SCAN,(STRPTR)GAD_SILENT_SCAN_STR},
    {HELP_SILENT_SCAN,(STRPTR)HELP_SILENT_SCAN_STR},
    {GAD_FROM_LOCALE,(STRPTR)GAD_FROM_LOCALE_STR},
    {HELP_COUNTRY,(STRPTR)HELP_COUNTRY_STR},
    {GAD_COUNTRY_CODE,(STRPTR)GAD_COUNTRY_CODE_STR},
    {HELP_COUNTRY_CODE,(STRPTR)HELP_COUNTRY_CODE_STR},
    {MSG_HOTLIST_CHANGED,(STRPTR)MSG_HOTLIST_CHANGED_STR},
    {ANS_OK_CANCEL,(STRPTR)ANS_OK_CANCEL_STR},
    {HELP_USER_IMAGE,(STRPTR)HELP_USER_IMAGE_STR},
    {GAD_MOBILE,(STRPTR)GAD_MOBILE_STR},
    {MSG_ON_DOUBLE_CLICK,(STRPTR)MSG_ON_DOUBLE_CLICK_STR},
    {HELP_DOUBLE_CLICK,(STRPTR)HELP_DOUBLE_CLICK_STR},
    {MSG_EDIT_ENTRIES,(STRPTR)MSG_EDIT_ENTRIES_STR},
    {MSG_START_SCRIPT,(STRPTR)MSG_START_SCRIPT_STR},
    {MSG_SCRIPT_AND_CLOSE,(STRPTR)MSG_SCRIPT_AND_CLOSE_STR},
    {GAD_DOPUSFTP,(STRPTR)GAD_DOPUSFTP_STR},
    {MSG_DOPUS_FTP,(STRPTR)MSG_DOPUS_FTP_STR},
    {MSG_LOCAL_LISTER,(STRPTR)MSG_LOCAL_LISTER_STR},
    {MSG_DEFAULT_FTP,(STRPTR)MSG_DEFAULT_FTP_STR},
    {GAD_OPEN_URL,(STRPTR)GAD_OPEN_URL_STR},
    {MSG_DIALLER,(STRPTR)MSG_DIALLER_STR},
    {GAD_USE_DIALLER,(STRPTR)GAD_USE_DIALLER_STR},
    {MSG_USE_EXT_DIALER,(STRPTR)MSG_USE_EXT_DIALER_STR},
    {GAD_DEVICE,(STRPTR)GAD_DEVICE_STR},
    {GAD_UNIT,(STRPTR)GAD_UNIT_STR},
    {GAD_SHARED,(STRPTR)GAD_SHARED_STR},
    {GAD_INIT_STRING,(STRPTR)GAD_INIT_STRING_STR},
    {GAD_DIAL_COMMAND,(STRPTR)GAD_DIAL_COMMAND_STR},
    {MSG_PROGRAMS,(STRPTR)MSG_PROGRAMS_STR},
    {GAD_MAILER,(STRPTR)GAD_MAILER_STR},
    {MSG_SCRIPT,(STRPTR)MSG_SCRIPT_STR},
    {MSG_PATH,(STRPTR)MSG_PATH_STR},
    {GAD_IRC,(STRPTR)GAD_IRC_STR},
    {GAD_PROGRAM,(STRPTR)GAD_PROGRAM_STR},
    {MSG_LOCALE,(STRPTR)MSG_LOCALE_STR},
    {MSG_MISC,(STRPTR)MSG_MISC_STR},
    {MSG_DIALER_PLACEHOLDERS,(STRPTR)MSG_DIALER_PLACEHOLDERS_STR},
    {MSG_DIALLING,(STRPTR)MSG_DIALLING_STR},
    {ERR_NO_DEVICE,(STRPTR)ERR_NO_DEVICE_STR},
    {GAD_REDIAL_DELAY,(STRPTR)GAD_REDIAL_DELAY_STR},
    {GAD_REDIAL_ATTEMPTS,(STRPTR)GAD_REDIAL_ATTEMPTS_STR},
    {MSG_WAITING,(STRPTR)MSG_WAITING_STR},
    {ANS_RETRY_CANCEL,(STRPTR)ANS_RETRY_CANCEL_STR},
    {MSG_PHONE,(STRPTR)MSG_PHONE_STR},
    {MSG_MOBILE,(STRPTR)MSG_MOBILE_STR},
    {GAD_MODEM_TYPE,(STRPTR)GAD_MODEM_TYPE_STR},
    {ERR_MODEM_NOT_RESPONDING,(STRPTR)ERR_MODEM_NOT_RESPONDING_STR},
    {GAD_ICQ,(STRPTR)GAD_ICQ_STR},
    {MSG_TO_CLIP,(STRPTR)MSG_TO_CLIP_STR},
    {MSG_HOME,(STRPTR)MSG_HOME_STR},
    {MSG_BUSINESS,(STRPTR)MSG_BUSINESS_STR},
    {MSG_NOTES,(STRPTR)MSG_NOTES_STR},
    {GAD_DEPARTMENT,(STRPTR)GAD_DEPARTMENT_STR},
    {GAD_JOB_TITLE,(STRPTR)GAD_JOB_TITLE_STR},
    {GAD_OFFICE,(STRPTR)GAD_OFFICE_STR},
    {GAD_COMPANY,(STRPTR)GAD_COMPANY_STR},
    {MSG_LIMIT,(STRPTR)MSG_LIMIT_STR},
    {MENU_DFA_ADDRESSES,(STRPTR)MENU_DFA_ADDRESSES_STR},
    {GAD_DFA,(STRPTR)GAD_DFA_STR},
    {MENU_UMS_GOODNEWS,(STRPTR)MENU_UMS_GOODNEWS_STR},
    {GAD_GOODNEWS,(STRPTR)GAD_GOODNEWS_STR},
    {MENU_UMS_PINT,(STRPTR)MENU_UMS_PINT_STR},
    {GAD_UMS,(STRPTR)GAD_UMS_STR},
    {GAD_SAVE_ON_EXIT,(STRPTR)GAD_SAVE_ON_EXIT_STR},
    {HELP_SAVE_ON_EXIT,(STRPTR)HELP_SAVE_ON_EXIT_STR},
    {MENU_STRICQ_USERS,(STRPTR)MENU_STRICQ_USERS_STR},
    {GAD_STRICQ_USERS,(STRPTR)GAD_STRICQ_USERS_STR},
    {MSG_SCRIPT_AND_ICONIFY,(STRPTR)MSG_SCRIPT_AND_ICONIFY_STR},
    {GAD_PGP_ID,(STRPTR)GAD_PGP_ID_STR},
    {HDR_LAST_NAME,(STRPTR)HDR_LAST_NAME_STR},
    {GAD_DONT_MOVE_COLUMNS,(STRPTR)GAD_DONT_MOVE_COLUMNS_STR},
    {HELP_DONT_MOVE_COLUMNS,(STRPTR)HELP_DONT_MOVE_COLUMNS_STR},
    {GAD_LOG_CALLS,(STRPTR)GAD_LOG_CALLS_STR},
    {MSG_BUSINESS_MOBILE,(STRPTR)MSG_BUSINESS_MOBILE_STR},
    {HDR_LAST_MODIFIED,(STRPTR)HDR_LAST_MODIFIED_STR},
    {HDR_LAST_VISITED,(STRPTR)HDR_LAST_VISITED_STR},
    {MSG_TELNET,(STRPTR)MSG_TELNET_STR},
    {HDR_HOST,(STRPTR)HDR_HOST_STR},
    {GAD_HOST,(STRPTR)GAD_HOST_STR},
    {GAD_LOGIN,(STRPTR)GAD_LOGIN_STR},
    {WND_EDIT_TELNET,(STRPTR)WND_EDIT_TELNET_STR},
    {GAD_SSH,(STRPTR)GAD_SSH_STR},
    {MENU_EXPORT,(STRPTR)MENU_EXPORT_STR},
    {MENU_EXPORT_URL_HTML,(STRPTR)MENU_EXPORT_URL_HTML_STR},
    {MENU_EXPORT_WWW_HTML,(STRPTR)MENU_EXPORT_WWW_HTML_STR},
    {MENU_EXPORT_FTP_HTML,(STRPTR)MENU_EXPORT_FTP_HTML_STR},
    {MENU_EXPORT_USERS_CSV,(STRPTR)MENU_EXPORT_USERS_CSV_STR},
    {MSG_GET_DB_DIR,(STRPTR)MSG_GET_DB_DIR_STR},
    {ANS_SELECT_CANCEL,(STRPTR)ANS_SELECT_CANCEL_STR},
    {MSG_NO_DIR_WARNING,(STRPTR)MSG_NO_DIR_WARNING_STR},
    {GAD_SAVE_ON_CHANGES,(STRPTR)GAD_SAVE_ON_CHANGES_STR},
    {HELP_SAVE_ON_CHANGES,(STRPTR)HELP_SAVE_ON_CHANGES_STR},
    {MGS_ADVANCED,(STRPTR)MGS_ADVANCED_STR},
    {GAD_WINDOW_TYPE,(STRPTR)GAD_WINDOW_TYPE_STR},
    {MSG_SINGLE_LIST,(STRPTR)MSG_SINGLE_LIST_STR},
    {MSG_DUAL_LIST,(STRPTR)MSG_DUAL_LIST_STR},
    {GAD_USE_PROXY,(STRPTR)GAD_USE_PROXY_STR},
    {GAD_SHOW_HIDDEN,(STRPTR)GAD_SHOW_HIDDEN_STR},
    {GAD_SHOW_FTP_OUTPUT,(STRPTR)GAD_SHOW_FTP_OUTPUT_STR},
    {GAD_SEND_NOOP,(STRPTR)GAD_SEND_NOOP_STR},
    {MSG_TOOLBAR,(STRPTR)MSG_TOOLBAR_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

             char CatCompBlock[] =
{
    "\x00\x00\x00\x00\x00\x1A"
    MSG_APP_DESCRIPTION_STR "\x00\x00"
    "\x00\x00\x00\x01\x00\x06"
    GAD_ADD_GROUP_STR "\x00\x00"
    "\x00\x00\x00\x02\x00\x0C"
    GAD_ADD_ENTRY_STR "\x00\x00"
    "\x00\x00\x00\x03\x00\x0E"
    GAD_CLONE_ENTRY_STR "\x00\x00"
    "\x00\x00\x00\x04\x00\x08"
    GAD_REMOVE_STR "\x00"
    "\x00\x00\x00\x05\x00\x08"
    GAD_REM_GROUP_STR "\x00"
    "\x00\x00\x00\x06\x00\x0A"
    WND_ADD_GROUP_STR "\x00"
    "\x00\x00\x00\x07\x00\x0E"
    GAD_GROUP_NAME_STR "\x00\x00"
    "\x00\x00\x00\x08\x00\x04"
    GAD_OK_STR "\x00"
    "\x00\x00\x00\x09\x00\x08"
    GAD_CANCEL_STR "\x00"
    "\x00\x00\x00\x0A\x00\x06"
    MSG_USERS_STR "\x00"
    "\x00\x00\x00\x0B\x00\x0A"
    MSG_FTP_STR "\x00"
    "\x00\x00\x00\x0C\x00\x0A"
    MSG_WWW_STR "\x00"
    "\x00\x00\x00\x0D\x00\x06"
    MSG_CHAT_STR "\x00\x00"
    "\x00\x00\x00\x0E\x00\x06"
    MSG_NAME_STR "\x00"
    "\x00\x00\x00\x0F\x00\x0C"
    MSG_FIRST_NAME_STR "\x00"
    "\x00\x00\x00\x10\x00\x0C"
    MSG_LAST_NAME_STR "\x00\x00"
    "\x00\x00\x00\x11\x00\x0A"
    MSG_ADDRESS_STR "\x00\x00"
    "\x00\x00\x00\x12\x00\x06"
    MSG_CITY_STR "\x00"
    "\x00\x00\x00\x13\x00\x0A"
    MSG_COUNTRY_STR "\x00\x00"
    "\x00\x00\x00\x14\x00\x06"
    MSG_ZIP_STR "\x00\x00"
    "\x00\x00\x00\x15\x00\x0A"
    MSG_COMMENT_STR "\x00\x00"
    "\x00\x00\x00\x16\x00\x08"
    GAD_PHONE_STR "\x00\x00"
    "\x00\x00\x00\x17\x00\x06"
    MSG_FAX_STR "\x00\x00"
    "\x00\x00\x00\x18\x00\x08"
    MSG_EMAIL_STR "\x00"
    "\x00\x00\x00\x19\x00\x06"
    GAD_WWW_STR "\x00\x00"
    "\x00\x00\x00\x1A\x00\x06"
    GAD_FTP_STR "\x00\x00"
    "\x00\x00\x00\x1B\x00\x10"
    GAD_TO_WWW_STR "\x00"
    "\x00\x00\x00\x1C\x00\x0C"
    GAD_TO_FTP_STR "\x00"
    "\x00\x00\x00\x1D\x00\x0C"
    GAD_TO_EMAIL_STR "\x00\x00"
    "\x00\x00\x00\x1E\x00\x06"
    GAD_TO_PHONE_STR "\x00\x00"
    "\x00\x00\x00\x1F\x00\x0A"
    GAD_TO_FAX_STR "\x00\x00"
    "\x00\x00\x00\x20\x00\x10"
    MSG_TO_DOPUS_STR "\x00\x00"
    "\x00\x00\x00\x21\x00\x1A"
    HELP_REMOVE_STR "\x00"
    "\x00\x00\x00\x22\x00\x1C"
    HELP_ADD_ENTRY_STR "\x00"
    "\x00\x00\x00\x23\x00\x20"
    HELP_CLONE_ENTRY_STR "\x00\x00"
    "\x00\x00\x00\x24\x00\x2E"
    HELP_EDIT_STR "\x00"
    "\x00\x00\x00\x25\x00\x1E"
    HELP_SORT_STR "\x00\x00"
    "\x00\x00\x00\x26\x00\x08"
    MENU_PROJECT_STR "\x00"
    "\x00\x00\x00\x27\x00\x0A"
    MENU_ABOUT_STR "\x00\x00"
    "\x00\x00\x00\x28\x00\x02"
    KEY_ABOUT_STR "\x00"
    "\x00\x00\x00\x29\x00\x0E"
    MENU_ABOUT_MUI_STR "\x00\x00"
    "\x00\x00\x00\x2A\x00\x08"
    MENU_OPEN_STR "\x00"
    "\x00\x00\x00\x2B\x00\x02"
    KEY_OPEN_STR "\x00"
    "\x00\x00\x00\x2C\x00\x06"
    MENU_SAVE_STR "\x00\x00"
    "\x00\x00\x00\x2D\x00\x02"
    KEY_SAVE_STR "\x00"
    "\x00\x00\x00\x2E\x00\x0C"
    MENU_SAVE_AS_STR "\x00\x00"
    "\x00\x00\x00\x2F\x00\x02"
    KEY_SAVE_AS_STR "\x00"
    "\x00\x00\x00\x30\x00\x06"
    MENU_QUIT_STR "\x00\x00"
    "\x00\x00\x00\x31\x00\x02"
    KEY_QUIT_STR "\x00"
    "\x00\x00\x00\x32\x00\x12"
    GAD_ADVANCED_OPTIONS_STR "\x00"
    "\x00\x00\x00\x33\x00\x12"
    GAD_ANONYMOUS_LOGIN_STR "\x00\x00"
    "\x00\x00\x00\x34\x00\x0A"
    GAD_USERNAME_STR "\x00"
    "\x00\x00\x00\x35\x00\x0A"
    GAD_PASSWORD_STR "\x00"
    "\x00\x00\x00\x36\x00\x0C"
    GAD_ADT_STR "\x00"
    "\x00\x00\x00\x37\x00\x08"
    GAD_QUIET_STR "\x00\x00"
    "\x00\x00\x00\x38\x00\x0A"
    GAD_RETRIES_STR "\x00\x00"
    "\x00\x00\x00\x39\x00\x0E"
    GAD_COMPRESSION_STR "\x00\x00"
    "\x00\x00\x00\x3A\x00\x06"
    GAD_PORT_STR "\x00"
    "\x00\x00\x00\x3B\x00\x06"
    HDR_NAME_STR "\x00\x00"
    "\x00\x00\x00\x3C\x00\x08"
    HDR_COMMENT_STR "\x00"
    "\x00\x00\x00\x3D\x00\x06"
    HDR_SITE_STR "\x00\x00"
    "\x00\x00\x00\x3E\x00\x06"
    HDR_EMAIL_STR "\x00"
    "\x00\x00\x00\x3F\x00\x0A"
    WND_EDIT_USER_STR "\x00"
    "\x00\x00\x00\x40\x00\x14"
    WND_EDIT_WWW_STR "\x00\x00"
    "\x00\x00\x00\x41\x00\x14"
    WND_EDIT_FTP_STR "\x00\x00"
    "\x00\x00\x00\x42\x00\x06"
    GAD_EDIT_STR "\x00"
    "\x00\x00\x00\x43\x00\x08"
    GAD_SEARCH_STR "\x00"
    "\x00\x00\x00\x44\x00\x0A"
    GAD_FROM_TOP_STR "\x00"
    "\x00\x00\x00\x45\x00\x0C"
    GAD_BACKWARDS_STR "\x00\x00"
    "\x00\x00\x00\x46\x00\x0A"
    MENU_OPTIONS_STR "\x00\x00"
    "\x00\x00\x00\x47\x00\x0A"
    MENU_SEARCH_STR "\x00"
    "\x00\x00\x00\x48\x00\x0E"
    GAD_SEARCH_FOR_STR "\x00\x00"
    "\x00\x00\x00\x49\x00\x08"
    MENU_IMPORT_STR "\x00\x00"
    "\x00\x00\x00\x4A\x00\x14"
    MENU_V_HOTLIST_STR "\x00\x00"
    "\x00\x00\x00\x4B\x00\x12"
    MENU_THOR_USERS_STR "\x00"
    "\x00\x00\x00\x4C\x00\x14"
    MENU_IB_HOTLIST_STR "\x00\x00"
    "\x00\x00\x00\x4D\x00\x10"
    MENU_AWEB_HOTLIST_STR "\x00"
    "\x00\x00\x00\x4E\x00\x12"
    MENU_AMFTP_PROFILES_STR "\x00"
    "\x00\x00\x00\x4F\x00\x10"
    MENU_DOPUS_FTP_STR "\x00"
    "\x00\x00\x00\x50\x00\x1A"
    MENU_MD_ADDRESSES_STR "\x00"
    "\x00\x00\x00\x51\x00\x14"
    MENU_YAM_ADDRESSES_STR "\x00\x00"
    "\x00\x00\x00\x52\x00\x14"
    MENU_STFAX_PHONEBOOK_STR "\x00\x00"
    "\x00\x00\x00\x53\x00\x12"
    MENU_AMIRC_SERVERS_STR "\x00\x00"
    "\x00\x00\x00\x54\x00\x0A"
    MENU_SETTINGS_STR "\x00\x00"
    "\x00\x00\x00\x55\x00\x14"
    MENU_CMANAGER_STR "\x00\x00"
    "\x00\x00\x00\x56\x00\x08"
    MENU_MUI_STR "\x00\x00"
    "\x00\x00\x00\x57\x00\x08"
    GAD_ALIAS_STR "\x00\x00"
    "\x00\x00\x00\x58\x00\x06"
    GAD_SORT_STR "\x00"
    "\x00\x00\x00\x59\x01\x42"
    MSG_ABOUT_STR "\x00"
    "\x00\x00\x00\x5A\x00\x06"
    GAD_MORE_STR "\x00"
    "\x00\x00\x00\x5B\x00\x30"
    MSG_REGISTERED_STR "\x00"
    "\x00\x00\x00\x5C\x00\x0C"
    GAD_WEBMASTER_STR "\x00\x00"
    "\x00\x00\x00\x5D\x00\x36"
    MSG_NOT_SAVED_STR "\x00\x00"
    "\x00\x00\x00\x5E\x00\x16"
    ANS_SAVE_QUIT_CANCEL_STR "\x00\x00"
    "\x00\x00\x00\x5F\x00\x0E"
    MENU_JOIN_CHANNEL_STR "\x00\x00"
    "\x00\x00\x00\x60\x00\x08"
    HDR_CHANNEL_STR "\x00"
    "\x00\x00\x00\x61\x00\x08"
    HDR_SERVER_STR "\x00\x00"
    "\x00\x00\x00\x62\x00\x0A"
    GAD_CHANNEL_STR "\x00\x00"
    "\x00\x00\x00\x63\x00\x08"
    GAD_SERVER_STR "\x00"
    "\x00\x00\x00\x64\x00\x0C"
    GAD_MAINTAINER_STR "\x00"
    "\x00\x00\x00\x65\x00\x06"
    GAD_NICK_STR "\x00"
    "\x00\x00\x00\x66\x00\x0A"
    GAD_WEB_SITE_STR "\x00"
    "\x00\x00\x00\x67\x00\x12"
    WND_EDIT_CHAT_STR "\x00\x00"
    "\x00\x00\x00\x68\x00\x0A"
    MSG_PATHS_STR "\x00\x00"
    "\x00\x00\x00\x69\x00\x0A"
    GAD_V_STR "\x00\x00"
    "\x00\x00\x00\x6A\x00\x0A"
    GAD_IB_STR "\x00\x00"
    "\x00\x00\x00\x6B\x00\x0E"
    GAD_MD2_STR "\x00\x00"
    "\x00\x00\x00\x6C\x00\x06"
    GAD_YAM_STR "\x00\x00"
    "\x00\x00\x00\x6D\x00\x08"
    GAD_STFAX_STR "\x00\x00"
    "\x00\x00\x00\x6E\x00\x06"
    GAD_AWEB_STR "\x00"
    "\x00\x00\x00\x6F\x00\x08"
    GAD_AMFTP_STR "\x00\x00"
    "\x00\x00\x00\x70\x00\x08"
    GAD_AMIRC_STR "\x00\x00"
    "\x00\x00\x00\x71\x00\x06"
    GAD_USE_STR "\x00\x00"
    "\x00\x00\x00\x72\x00\x06"
    GAD_SAVE_STR "\x00"
    "\x00\x00\x00\x73\x00\x0E"
    GAD_SILENT_SCAN_STR "\x00\x00"
    "\x00\x00\x00\x74\x00\x6C"
    HELP_SILENT_SCAN_STR "\x00"
    "\x00\x00\x00\x75\x00\x12"
    GAD_FROM_LOCALE_STR "\x00\x00"
    "\x00\x00\x00\x76\x00\x30"
    HELP_COUNTRY_STR "\x00\x00"
    "\x00\x00\x00\x77\x00\x0E"
    GAD_COUNTRY_CODE_STR "\x00\x00"
    "\x00\x00\x00\x78\x00\x1E"
    HELP_COUNTRY_CODE_STR "\x00"
    "\x00\x00\x00\x79\x00\x38"
    MSG_HOTLIST_CHANGED_STR "\x00\x00"
    "\x00\x00\x00\x7A\x00\x0E"
    ANS_OK_CANCEL_STR "\x00\x00"
    "\x00\x00\x00\x7B\x00\x20"
    HELP_USER_IMAGE_STR "\x00\x00"
    "\x00\x00\x00\x7C\x00\x08"
    GAD_MOBILE_STR "\x00"
    "\x00\x00\x00\x7D\x00\x10"
    MSG_ON_DOUBLE_CLICK_STR "\x00\x00"
    "\x00\x00\x00\x7E\x00\x3E"
    HELP_DOUBLE_CLICK_STR "\x00\x00"
    "\x00\x00\x00\x7F\x00\x18"
    MSG_EDIT_ENTRIES_STR "\x00"
    "\x00\x00\x00\x80\x00\x16"
    MSG_START_SCRIPT_STR "\x00"
    "\x00\x00\x00\x81\x00\x20"
    MSG_SCRIPT_AND_CLOSE_STR "\x00\x00"
    "\x00\x00\x00\x82\x00\x0C"
    GAD_DOPUSFTP_STR "\x00\x00"
    "\x00\x00\x00\x83\x00\x0A"
    MSG_DOPUS_FTP_STR "\x00"
    "\x00\x00\x00\x84\x00\x0E"
    MSG_LOCAL_LISTER_STR "\x00"
    "\x00\x00\x00\x85\x00\x14"
    MSG_DEFAULT_FTP_STR "\x00"
    "\x00\x00\x00\x86\x00\x16"
    GAD_OPEN_URL_STR "\x00\x00"
    "\x00\x00\x00\x87\x00\x08"
    MSG_DIALLER_STR "\x00\x00"
    "\x00\x00\x00\x88\x00\x14"
    GAD_USE_DIALLER_STR "\x00"
    "\x00\x00\x00\x89\x00\x1C"
    MSG_USE_EXT_DIALER_STR "\x00"
    "\x00\x00\x00\x8A\x00\x08"
    GAD_DEVICE_STR "\x00"
    "\x00\x00\x00\x8B\x00\x06"
    GAD_UNIT_STR "\x00"
    "\x00\x00\x00\x8C\x00\x08"
    GAD_SHARED_STR "\x00"
    "\x00\x00\x00\x8D\x00\x0C"
    GAD_INIT_STRING_STR "\x00"
    "\x00\x00\x00\x8E\x00\x0E"
    GAD_DIAL_COMMAND_STR "\x00"
    "\x00\x00\x00\x8F\x00\x0A"
    MSG_PROGRAMS_STR "\x00\x00"
    "\x00\x00\x00\x90\x00\x08"
    GAD_MAILER_STR "\x00"
    "\x00\x00\x00\x91\x00\x08"
    MSG_SCRIPT_STR "\x00\x00"
    "\x00\x00\x00\x92\x00\x06"
    MSG_PATH_STR "\x00\x00"
    "\x00\x00\x00\x93\x00\x06"
    GAD_IRC_STR "\x00\x00"
    "\x00\x00\x00\x94\x00\x0A"
    GAD_PROGRAM_STR "\x00\x00"
    "\x00\x00\x00\x95\x00\x08"
    MSG_LOCALE_STR "\x00\x00"
    "\x00\x00\x00\x96\x00\x06"
    MSG_MISC_STR "\x00\x00"
    "\x00\x00\x00\x97\x00\x34"
    MSG_DIALER_PLACEHOLDERS_STR "\x00"
    "\x00\x00\x00\x98\x00\x22"
    MSG_DIALLING_STR "\x00\x00"
    "\x00\x00\x00\x99\x00\x1C"
    ERR_NO_DEVICE_STR "\x00"
    "\x00\x00\x00\x9A\x00\x0E"
    GAD_REDIAL_DELAY_STR "\x00"
    "\x00\x00\x00\x9B\x00\x12"
    GAD_REDIAL_ATTEMPTS_STR "\x00\x00"
    "\x00\x00\x00\x9C\x00\x12"
    MSG_WAITING_STR "\x00\x00"
    "\x00\x00\x00\x9D\x00\x10"
    ANS_RETRY_CANCEL_STR "\x00"
    "\x00\x00\x00\x9E\x00\x06"
    MSG_PHONE_STR "\x00"
    "\x00\x00\x00\x9F\x00\x08"
    MSG_MOBILE_STR "\x00\x00"
    "\x00\x00\x00\xA0\x00\x0C"
    GAD_MODEM_TYPE_STR "\x00"
    "\x00\x00\x00\xA1\x00\x34"
    ERR_MODEM_NOT_RESPONDING_STR "\x00\x00"
    "\x00\x00\x00\xA2\x00\x06"
    GAD_ICQ_STR "\x00\x00"
    "\x00\x00\x00\xA3\x00\x12"
    MSG_TO_CLIP_STR "\x00"
    "\x00\x00\x00\xA4\x00\x06"
    MSG_HOME_STR "\x00\x00"
    "\x00\x00\x00\xA5\x00\x0A"
    MSG_BUSINESS_STR "\x00\x00"
    "\x00\x00\x00\xA6\x00\x06"
    MSG_NOTES_STR "\x00"
    "\x00\x00\x00\xA7\x00\x0E"
    GAD_DEPARTMENT_STR "\x00\x00"
    "\x00\x00\x00\xA8\x00\x0C"
    GAD_JOB_TITLE_STR "\x00"
    "\x00\x00\x00\xA9\x00\x0A"
    GAD_OFFICE_STR "\x00\x00"
    "\x00\x00\x00\xAA\x00\x0A"
    GAD_COMPANY_STR "\x00"
    "\x00\x00\x00\xAB\x00\x56"
    MSG_LIMIT_STR "\x00"
    "\x00\x00\x00\xAC\x00\x12"
    MENU_DFA_ADDRESSES_STR "\x00\x00"
    "\x00\x00\x00\xAD\x00\x06"
    GAD_DFA_STR "\x00\x00"
    "\x00\x00\x00\xAE\x00\x1C"
    MENU_UMS_GOODNEWS_STR "\x00"
    "\x00\x00\x00\xAF\x00\x10"
    GAD_GOODNEWS_STR "\x00"
    "\x00\x00\x00\xB0\x00\x18"
    MENU_UMS_PINT_STR "\x00"
    "\x00\x00\x00\xB1\x00\x0C"
    GAD_UMS_STR "\x00"
    "\x00\x00\x00\xB2\x00\x0E"
    GAD_SAVE_ON_EXIT_STR "\x00"
    "\x00\x00\x00\xB3\x00\x62"
    HELP_SAVE_ON_EXIT_STR "\x00\x00"
    "\x00\x00\x00\xB4\x00\x10"
    MENU_STRICQ_USERS_STR "\x00"
    "\x00\x00\x00\xB5\x00\x0E"
    GAD_STRICQ_USERS_STR "\x00"
    "\x00\x00\x00\xB6\x00\x22"
    MSG_SCRIPT_AND_ICONIFY_STR "\x00"
    "\x00\x00\x00\xB7\x00\x0C"
    GAD_PGP_ID_STR "\x00"
    "\x00\x00\x00\xB8\x00\x0A"
    HDR_LAST_NAME_STR "\x00"
    "\x00\x00\x00\xB9\x00\x14"
    GAD_DONT_MOVE_COLUMNS_STR "\x00"
    "\x00\x00\x00\xBA\x00\x60"
    HELP_DONT_MOVE_COLUMNS_STR "\x00\x00"
    "\x00\x00\x00\xBB\x00\x16"
    GAD_LOG_CALLS_STR "\x00\x00"
    "\x00\x00\x00\xBC\x00\x10"
    MSG_BUSINESS_MOBILE_STR "\x00"
    "\x00\x00\x00\xBD\x00\x0E"
    HDR_LAST_MODIFIED_STR "\x00"
    "\x00\x00\x00\xBE\x00\x0E"
    HDR_LAST_VISITED_STR "\x00\x00"
    "\x00\x00\x00\xBF\x00\x08"
    MSG_TELNET_STR "\x00\x00"
    "\x00\x00\x00\xC0\x00\x06"
    HDR_HOST_STR "\x00\x00"
    "\x00\x00\x00\xC1\x00\x06"
    GAD_HOST_STR "\x00"
    "\x00\x00\x00\xC2\x00\x08"
    GAD_LOGIN_STR "\x00\x00"
    "\x00\x00\x00\xC3\x00\x12"
    WND_EDIT_TELNET_STR "\x00\x00"
    "\x00\x00\x00\xC4\x00\x06"
    GAD_SSH_STR "\x00\x00"
    "\x00\x00\x00\xC5\x00\x08"
    MENU_EXPORT_STR "\x00\x00"
    "\x00\x00\x00\xC6\x00\x10"
    MENU_EXPORT_URL_HTML_STR "\x00"
    "\x00\x00\x00\xC7\x00\x16"
    MENU_EXPORT_WWW_HTML_STR "\x00\x00"
    "\x00\x00\x00\xC8\x00\x16"
    MENU_EXPORT_FTP_HTML_STR "\x00\x00"
    "\x00\x00\x00\xC9\x00\x10"
    MENU_EXPORT_USERS_CSV_STR "\x00"
    "\x00\x00\x00\xCA\x00\x2A"
    MSG_GET_DB_DIR_STR "\x00"
    "\x00\x00\x00\xCB\x00\x12"
    ANS_SELECT_CANCEL_STR "\x00\x00"
    "\x00\x00\x00\xCC\x00\x90"
    MSG_NO_DIR_WARNING_STR "\x00\x00"
    "\x00\x00\x00\xCD\x00\x12"
    GAD_SAVE_ON_CHANGES_STR "\x00\x00"
    "\x00\x00\x00\xCE\x00\x3C"
    HELP_SAVE_ON_CHANGES_STR "\x00"
    "\x00\x00\x00\xCF\x00\x0A"
    MGS_ADVANCED_STR "\x00\x00"
    "\x00\x00\x00\xD0\x00\x0E"
    GAD_WINDOW_TYPE_STR "\x00\x00"
    "\x00\x00\x00\xD1\x00\x0C"
    MSG_SINGLE_LIST_STR "\x00"
    "\x00\x00\x00\xD2\x00\x0A"
    MSG_DUAL_LIST_STR "\x00"
    "\x00\x00\x00\xD3\x00\x0C"
    GAD_USE_PROXY_STR "\x00\x00"
    "\x00\x00\x00\xD4\x00\x14"
    GAD_SHOW_HIDDEN_STR "\x00\x00"
    "\x00\x00\x00\xD5\x00\x12"
    GAD_SHOW_FTP_OUTPUT_STR "\x00\x00"
    "\x00\x00\x00\xD6\x00\x0C"
    GAD_SEND_NOOP_STR "\x00"
    "\x00\x00\x00\xD7\x00\x08"
    MSG_TOOLBAR_STR "\x00"
};

#endif /* CATCOMP_BLOCK */


/****************************************************************************/


struct LocaleInfo
{
    APTR li_LocaleBase;
    APTR li_Catalog;
};


#ifdef CATCOMP_CODE

STRPTR GetString(struct LocaleInfo *li, LONG stringNum)
{
LONG   *l;
UWORD  *w;
STRPTR  builtIn;

    l = (LONG *)CatCompBlock;

    while (*l != stringNum)
    {
        w = (UWORD *)((ULONG)l + 4);
        l = (LONG *)((ULONG)l + (ULONG)*w + 6);
    }
    builtIn = (STRPTR)((ULONG)l + 6);

#define XLocaleBase LocaleBase
#define LocaleBase li->li_LocaleBase
    
    if (LocaleBase)
        return(GetCatalogStr(li->li_Catalog,stringNum,builtIn));
#define LocaleBase XLocaleBase
#undef XLocaleBase

    return(builtIn);
}


#endif /* CATCOMP_CODE */


/****************************************************************************/


#endif /* LOCALE_H */
