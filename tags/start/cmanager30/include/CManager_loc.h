#ifndef CMANAGER_LOC_H
#define CMANAGER_LOC_H


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

#define Msg_GetDBDir 1
#define Msg_NOMUINoDirWarningMsg 2
#define Msg_NOMUINoDirWarningGads 3
#define MSG_ErrReqTitle 100
#define MSG_NOMUIErrReqGadget 101
#define Msg_Error_NoMem 102
#define Msg_Error_NoApp 103
#define Msg_Error_NoWin 104
#define Msg_Error_CantLogin 105
#define Msg_App_Copyright 200
#define Msg_App_Description 201
#define Msg_Menu_Project 202
#define Msg_Menu_About 203
#define Msg_Menu_AboutMUI 204
#define Msg_Menu_Hide 205
#define Msg_Menu_Quit 206
#define Msg_Menu_SaveAndQuit 207
#define Msg_Menu_ForceQuit 208
#define Msg_Menu_Database 209
#define Msg_Menu_User 210
#define Msg_Menu_Open 211
#define Msg_Menu_Save 212
#define Msg_Menu_SaveAs 213
#define Msg_Menu_LastSaved 214
#define Msg_Menu_Add 215
#define Msg_Menu_Clone 216
#define Msg_Menu_Edit 217
#define Msg_Menu_Remove 218
#define Msg_Menu_SelectAll 219
#define Msg_Menu_SelectNone 220
#define Msg_Menu_SelectToggle 221
#define Msg_Menu_Search 222
#define Msg_Menu_ScanHotlists 223
#define Msg_Menu_Import 224
#define Msg_Menu_ImportYAM 225
#define Msg_Menu_ImportMD2 226
#define Msg_Menu_ImportSTRICQ 227
#define Msg_Menu_ImportDFA 228
#define Msg_Menu_ImportSTFax 229
#define Msg_Menu_ImportPINT 230
#define Msg_Menu_ImportGoodNews 231
#define Msg_Menu_ImportCSV 232
#define Msg_Menu_ImportCSVFullOE 233
#define Msg_Menu_ImportThor 234
#define Msg_Menu_ImportAWeb 235
#define Msg_Menu_ImportV 236
#define Msg_Menu_ImportIB 237
#define Msg_Menu_ImportAmFTP 238
#define Msg_Menu_ImportDOpus 239
#define Msg_Menu_ImportAmIRC 240
#define Msg_Menu_ImportNetscape 241
#define Msg_Menu_Export 242
#define Msg_Menu_ExportCSV 243
#define Msg_Menu_ExportCSVFullOE 244
#define Msg_Menu_ExportURLHtml 245
#define Msg_Menu_ExportWWWHtml 246
#define Msg_Menu_ExportFTPHtml 247
#define Msg_Menu_ExportYAM 248
#define Msg_Menu_ExportAWeb 249
#define Msg_Menu_ExportIBrowse 250
#define Msg_Menu_ExportNetscape 251
#define Msg_Menu_Settings 252
#define Msg_Menu_EditPrefs 253
#define Msg_Menu_MUI 254
#define Msg_Menu_SavePrefs 255
#define Msg_Menu_LastSavedPrefs 256
#define Msg_Menu_NoBar 257
#define Msg_Menu_NoGroups 258
#define Msg_Menu_NoQuick 259
#define Msg_Menu_NoGroupsCtrl 260
#define Msg_Menu_NoCard 261
#define Msg_Menu_ImportSimpleMail 262
#define Msg_Menu_ExportSimpleMail 263
#define Msg_Menu_ImportATC 264
#define Msg_Menu_ExportATC 265
#define Msg_Req_Title 300
#define Msg_Req_HotChangedMsg 301
#define Msg_Req_HotChangedGads 302
#define Msg_Req_NotSavedMsg 303
#define Msg_Req_NotSavedGads 304
#define Msg_Req_ChangedMsg 305
#define Msg_Req_ChangedGads 306
#define Msg_Req_Asl 307
#define Msg_Req_AslGads 308
#define Msg_Req_DelWarningOne 309
#define Msg_Req_DelWarningMany 310
#define Msg_Req_DelWarningGads 311
#define Msg_Req_GroupDelWarningOne 312
#define Msg_Req_GroupDelWarningMany 313
#define Msg_Req_GroupDelWarningGads 314
#define Msg_Win_Title 400
#define Msg_Gad_Add 401
#define Msg_Gad_AddHelp 402
#define Msg_Gad_Clone 403
#define Msg_Gad_CloneHelp 404
#define Msg_Gad_Edit 405
#define Msg_Gad_EditHelp 406
#define Msg_Gad_Remove 407
#define Msg_Gad_RemoveHelp 408
#define Msg_Gad_Search 409
#define Msg_Gad_SearchHelp 410
#define Msg_Gad_Save 411
#define Msg_Gad_SaveHelp 412
#define Msg_Gad_Prefs 413
#define Msg_Gad_PrefsHelp 414
#define Msg_Gad_Login 415
#define Msg_Gad_LoginHelp 416
#define Msg_Gad_GroupName 417
#define Msg_Gad_GroupNameHelp 418
#define Msg_Gad_AddGroup 419
#define Msg_Gad_AddGroupHelp 420
#define Msg_Gad_RemoveGroup 421
#define Msg_Gad_RemoveGroupHelp 422
#define Msg_Gad_GroupUpHelp 423
#define Msg_Gad_GroupDownHelp 424
#define Msg_Groups_Menu_Title 425
#define Msg_Groups_Menu_OpenThis 426
#define Msg_Groups_Menu_OpenAll 427
#define Msg_Groups_Menu_CloseThis 428
#define Msg_Groups_Menu_CloseAll 429
#define Msg_Groups_Menu_Sort 430
#define Msg_Gad_Locate 431
#define Msg_Gad_LocateHelp 432
#define Msg_Entries 433
#define Msg_Gad_Sort 434
#define Msg_Gad_SortHelp 435
#define Msg_Users 500
#define Msg_WWW 501
#define Msg_FTP 502
#define Msg_Chat 503
#define Msg_Telnet 504
#define Msg_Title_Name 505
#define Msg_Title_LastName 506
#define Msg_Title_EMail 507
#define Msg_Title_Comment 508
#define Msg_Title_Alias 509
#define Msg_Title_Birthday 510
#define Msg_Title_Anniversary 511
#define Msg_Title_Country 512
#define Msg_Title_City 513
#define Msg_Title_Phone 514
#define Msg_Title_Mobile 515
#define Msg_Title_Fax 516
#define Msg_Title_WWW 517
#define Msg_Title_Company 518
#define Msg_Title_BPhone 519
#define Msg_Title_BMobile 520
#define Msg_Title_BFax 521
#define Msg_Title_Site 522
#define Msg_Title_LastVisited 523
#define Msg_Title_LastModified 524
#define Msg_Title_Channel 525
#define Msg_Title_Server 526
#define Msg_Title_Host 527
#define Msg_Title_Port 528
#define Msg_Never 529
#define Msg_Quick_UserFirst 530
#define Msg_Quick_UserLast 531
#define Msg_Quick_UserEMail 532
#define Msg_Quick_WWWName 533
#define Msg_Quick_WWWHost 534
#define Msg_Quick_FTPName 535
#define Msg_Quick_FTPHost 536
#define Msg_Quick_ChatChannel 537
#define Msg_Quick_ChatServer 538
#define Msg_Quick_TelnetHost 539
#define Msg_Quick_TelnetPort 540
#define Msg_Quick_TelnetLogin 541
#define Msg_Quick_TelnetPass 542
#define Msg_SearchWin_Title 600
#define Msg_SearchWin_Pattern 601
#define Msg_SearchWin_PatternHelp 602
#define Msg_SearchWin_SearchInTitle 603
#define Msg_SearchWin_Users 604
#define Msg_SearchWin_UsersHelp 605
#define Msg_SearchWin_WWW 606
#define Msg_SearchWin_WWWHelp 607
#define Msg_SearchWin_FTP 608
#define Msg_SearchWin_FTPHelp 609
#define Msg_SearchWin_Chat 610
#define Msg_SearchWin_ChatHelp 611
#define Msg_SearchWin_Telnet 612
#define Msg_SearchWin_TelnetHelp 613
#define Msg_SearchWin_OptionsTitle 614
#define Msg_SearchWin_Comment 615
#define Msg_SearchWin_CommentHelp 616
#define Msg_SearchWin_Case 617
#define Msg_SearchWin_CaseHelp 618
#define Msg_SearchWin_Wildcards 619
#define Msg_SearchWin_WildcardsHelp 620
#define Msg_SearchWin_Search 621
#define Msg_SearchWin_SearchHelp 622
#define Msg_SearchWin_First 623
#define Msg_SearchWin_FirstHelp 624
#define Msg_SearchWin_Previous 625
#define Msg_SearchWin_PreviousHelp 626
#define Msg_SearchWin_Cancel 627
#define Msg_SearchWin_CancelHelp 628
#define Msg_SearchWin_Searching 629
#define Msg_SearchWin_Welcome 630
#define Msg_SearchWin_Found 631
#define Msg_SearchWin_NotFound 632
#define Msg_UserWin_Title 700
#define Msg_UserWin_User 701
#define Msg_UserWin_UserHelp 702
#define Msg_PrefsWin_Title 800
#define Msg_PrefsWin_TitleSettings 801
#define Msg_PrefsWin_TitleScripts 802
#define Msg_PrefsWin_TitleHotlists 803
#define Msg_PrefsWin_TitleDialer 804
#define Msg_PrefsWin_Save 805
#define Msg_PrefsWin_SaveHelp 806
#define Msg_PrefsWin_Use 807
#define Msg_PrefsWin_UseHelp 808
#define Msg_PrefsWin_Apply 809
#define Msg_PrefsWin_ApplyHelp 810
#define Msg_PrefsWin_Cancel 811
#define Msg_PrefsWin_CancelHelp 812
#define Msg_PrefsWin_Settings_Locale 813
#define Msg_PrefsWin_Settings_Country 814
#define Msg_PrefsWin_Settings_CountryHelp 815
#define Msg_PrefsWin_Settings_CountryCode 816
#define Msg_PrefsWin_Settings_CountryCodeHelp 817
#define Msg_PrefsWin_Settings_GetFromLocale 818
#define Msg_PrefsWin_Settings_GetFromLocaleHelp 819
#define Msg_PrefsWin_Settings_Options 820
#define Msg_PrefsWin_Settings_DelWarning 821
#define Msg_PrefsWin_Settings_DelWarningHelp 822
#define Msg_PrefsWin_Settings_DoubleClick 823
#define Msg_PrefsWin_Settings_DoubleClickHelp 824
#define Msg_PrefsWin_Settings_DCEdit 825
#define Msg_PrefsWin_Settings_DCScript 826
#define Msg_PrefsWin_Settings_DCScriptQuit 827
#define Msg_PrefsWin_Settings_DCScriptHide 828
#define Msg_PrefsWin_Settings_SilentScan 829
#define Msg_PrefsWin_Settings_SilentScanHelp 830
#define Msg_PrefsWin_Settings_UseOpenURL 831
#define Msg_PrefsWin_Settings_UseOpenURLHelp 832
#define Msg_PrefsWin_Settings_BoldSections 833
#define Msg_PrefsWin_Settings_BoldSectionsHelp 834
#define Msg_PrefsWin_Settings_BoldTitles 835
#define Msg_PrefsWin_Settings_BoldTitlesHelp 836
#define Msg_PrefsWin_Settings_AutoReload 837
#define Msg_PrefsWin_Settings_AutoReloadHelp 838
#define Msg_PrefsWin_Settings_SaveOnExit 839
#define Msg_PrefsWin_Settings_SaveOnExitHelp 840
#define Msg_PrefsWin_Settings_SilentExit 841
#define Msg_PrefsWin_Settings_SilentExitHelp 842
#define Msg_PrefsWin_Settings_SaveOnChanges 843
#define Msg_PrefsWin_Settings_SaveOnChangesHelp 844
#define Msg_PrefsWin_Settings_NoListBar 845
#define Msg_PrefsWin_Settings_NoListBarHelp 846
#define Msg_PrefsWin_Settings_NtCheckChanges 847
#define Msg_PrefsWin_Settings_NtCheckChangesHelp 848
#define Msg_PrefsWin_Settings_RightMouseActive 849
#define Msg_PrefsWin_Settings_RightMouseActiveHelp 850
#define Msg_PrefsWin_Settings_EditNew 851
#define Msg_PrefsWin_Settings_EditNewHelp 852
#define Msg_PrefsWin_Settings_ButtonsBar 853
#define Msg_PrefsWin_Settings_BarPosition 854
#define Msg_PrefsWin_Settings_BarPositionHelp 855
#define Msg_PrefsWin_Settings_BarPositionTop 856
#define Msg_PrefsWin_Settings_BarPositionBottom 857
#define Msg_PrefsWin_Settings_BarPositionLeft 858
#define Msg_PrefsWin_Settings_BarPositionRight 859
#define Msg_PrefsWin_Settings_BarPositionInnertTop 860
#define Msg_PrefsWin_Settings_BarPositionInnerBottom 861
#define Msg_PrefsWin_Settings_ViewMode 862
#define Msg_PrefsWin_Settings_ViewModeHelp 863
#define Msg_PrefsWin_Settings_TextGfx 864
#define Msg_PrefsWin_Settings_Gfx 865
#define Msg_PrefsWin_Settings_Text 866
#define Msg_PrefsWin_Settings_Borderless 867
#define Msg_PrefsWin_Settings_BorderlessHelp 868
#define Msg_PrefsWin_Settings_Sunny 869
#define Msg_PrefsWin_Settings_SunnyHelp 870
#define Msg_PrefsWin_Settings_Raised 871
#define Msg_PrefsWin_Settings_RaisedHelp 872
#define Msg_PrefsWin_Settings_Scaled 873
#define Msg_PrefsWin_Settings_ScaledHelp 874
#define Msg_PrefsWin_Settings_Underscore 875
#define Msg_PrefsWin_Settings_UnderscoreHelp 876
#define Msg_PrefsWin_Settings_ButtonsPosition 877
#define Msg_PrefsWin_Settings_ButtonsPositionHelp 878
#define Msg_PrefsWin_Settings_ButtonsPosition_Left 879
#define Msg_PrefsWin_Settings_ButtonsPosition_Center 880
#define Msg_PrefsWin_Settings_ButtonsPosition_Right 881
#define Msg_PrefsWin_Settings_LabelPosition 882
#define Msg_PrefsWin_Settings_LabelPositionHelp 883
#define Msg_PrefsWin_Settings_LabelPosition_Bottom 884
#define Msg_PrefsWin_Settings_LabelPosition_Top 885
#define Msg_PrefsWin_Settings_LabelPosition_Right 886
#define Msg_PrefsWin_Settings_LabelPosition_Left 887
#define Msg_PrefsWin_Settings_BarSpacer 888
#define Msg_PrefsWin_Settings_BarSpacerHelp 889
#define Msg_PrefsWin_Settings_Frame 890
#define Msg_PrefsWin_Settings_FrameHelp 891
#define Msg_PrefsWin_Settings_DragBar 892
#define Msg_PrefsWin_Settings_DragBarHelp 893
#define Msg_PrefsWin_Programs_UserScript 894
#define Msg_PrefsWin_Programs_MailerTitle 895
#define Msg_PrefsWin_Programs_Mailer 896
#define Msg_PrefsWin_Programs_MailerHelp 897
#define Msg_PrefsWin_Programs_MailerScript 898
#define Msg_PrefsWin_Programs_MailerScriptHelp 899
#define Msg_PrefsWin_Programs_MailerPath 900
#define Msg_PrefsWin_Programs_MailerPathHelp 901
#define Msg_PrefsWin_Programs_WWWTitle 902
#define Msg_PrefsWin_Programs_WWW 903
#define Msg_PrefsWin_Programs_WWWHelp 904
#define Msg_PrefsWin_Programs_WWWScript 905
#define Msg_PrefsWin_Programs_WWWScriptHelp 906
#define Msg_PrefsWin_Programs_WWWPath 907
#define Msg_PrefsWin_Programs_WWWPathHelp 908
#define Msg_PrefsWin_Programs_FTPTitle 909
#define Msg_PrefsWin_Programs_FTP 910
#define Msg_PrefsWin_Programs_FTPHelp 911
#define Msg_PrefsWin_Programs_FTPScript 912
#define Msg_PrefsWin_Programs_FTPScriptHelp 913
#define Msg_PrefsWin_Programs_FTPPath 914
#define Msg_PrefsWin_Programs_FTPPathHelp 915
#define Msg_PrefsWin_Programs_FTPLocal 916
#define Msg_PrefsWin_Programs_FTPLocalHelp 917
#define Msg_PrefsWin_Programs_FTPLocalUse 918
#define Msg_PrefsWin_Programs_FTPLocalUseHelp 919
#define Msg_PrefsWin_Programs_ChatTitle 920
#define Msg_PrefsWin_Programs_Chat 921
#define Msg_PrefsWin_Programs_ChatHelp 922
#define Msg_PrefsWin_Programs_ChatScript 923
#define Msg_PrefsWin_Programs_ChatScriptHelp 924
#define Msg_PrefsWin_Programs_ChatPath 925
#define Msg_PrefsWin_Programs_ChatPathHelp 926
#define Msg_PrefsWin_Programs_TelnetTitle 927
#define Msg_PrefsWin_Programs_Telnet 928
#define Msg_PrefsWin_Programs_TelnetHelp 929
#define Msg_PrefsWin_Programs_TelnetScript 930
#define Msg_PrefsWin_Programs_TelnetScriptHelp 931
#define Msg_PrefsWin_Programs_TelnetPath 932
#define Msg_PrefsWin_Programs_TelnetPathHelp 933
#define Msg_PrefsWin_Programs_FaxTitle 934
#define Msg_PrefsWin_Programs_Fax 935
#define Msg_PrefsWin_Programs_FaxHelp 936
#define Msg_PrefsWin_Programs_FaxScript 937
#define Msg_PrefsWin_Programs_FaxScriptHelp 938
#define Msg_PrefsWin_Programs_FaxPath 939
#define Msg_PrefsWin_Programs_FaxPathHelp 940
#define Msg_PrefsWin_Programs_SavedProgTitle 941
#define Msg_PrefsWin_Programs_SavedProg 942
#define Msg_PrefsWin_Programs_SavedProgHelp 943
#define Msg_PrefsWin_Programs_SavedProgUse 944
#define Msg_PrefsWin_Programs_SavedProgUseHelp 945
#define Msg_PrefsWin_Programs_SavedProgPopData 946
#define Msg_PrefsWin_Dialer_Dialer 947
#define Msg_PrefsWin_Dialer_DialerHelp 948
#define Msg_PrefsWin_Dialer_DExternal 949
#define Msg_PrefsWin_Dialer_DInternal 950
#define Msg_PrefsWin_Dialer_Device 951
#define Msg_PrefsWin_Dialer_DeviceHelp 952
#define Msg_PrefsWin_Dialer_Unit 953
#define Msg_PrefsWin_Dialer_UnitHelp 954
#define Msg_PrefsWin_Dialer_Shared 955
#define Msg_PrefsWin_Dialer_SharedHelp 956
#define Msg_PrefsWin_Dialer_ModemType 957
#define Msg_PrefsWin_Dialer_ModemTypeHelp 958
#define Msg_PrefsWin_Dialer_ModemInit 959
#define Msg_PrefsWin_Dialer_ModemInitHelp 960
#define Msg_PrefsWin_Dialer_DialPrefix 961
#define Msg_PrefsWin_Dialer_DialPrefixHelp 962
#define Msg_PrefsWin_Dialer_DialSuffix 963
#define Msg_PrefsWin_Dialer_DialSuffixHelp 964
#define Msg_PrefsWin_Dialer_RedialDelay 965
#define Msg_PrefsWin_Dialer_RedialDelayHelp 966
#define Msg_PrefsWin_Dialer_RedialAttempts 967
#define Msg_PrefsWin_Dialer_RedialAttemptsHelp 968
#define Msg_PrefsWin_Dialer_Log 969
#define Msg_PrefsWin_Dialer_LogHelp 970
#define Msg_PrefsWin_Dialer_Program 971
#define Msg_PrefsWin_Dialer_ProgramHelp 972
#define Msg_PrefsWin_Dialer_ProgramPopNumber 973
#define Msg_PrefsWin_Hotlists_V 974
#define Msg_PrefsWin_Hotlists_VHelp 975
#define Msg_PrefsWin_Hotlists_IB 976
#define Msg_PrefsWin_Hotlists_IBHelp 977
#define Msg_PrefsWin_Hotlists_AWeb 978
#define Msg_PrefsWin_Hotlists_AWebHelp 979
#define Msg_PrefsWin_Hotlists_MD2 980
#define Msg_PrefsWin_Hotlists_MD2Help 981
#define Msg_PrefsWin_Hotlists_YAM 982
#define Msg_PrefsWin_Hotlists_YAMHelp 983
#define Msg_PrefsWin_Hotlists_STRICQ 984
#define Msg_PrefsWin_Hotlists_STRICQHelp 985
#define Msg_PrefsWin_Hotlists_GoodNews 986
#define Msg_PrefsWin_Hotlists_GoodNewsHelp 987
#define Msg_PrefsWin_Hotlists_PINT 988
#define Msg_PrefsWin_Hotlists_PINTHelp 989
#define Msg_PrefsWin_Hotlists_DFA 990
#define Msg_PrefsWin_Hotlists_DFAHelp 991
#define Msg_PrefsWin_Hotlists_STFax 992
#define Msg_PrefsWin_Hotlists_STFaxHelp 993
#define Msg_PrefsWin_Hotlists_Thor 994
#define Msg_PrefsWin_Hotlists_ThorHelp 995
#define Msg_PrefsWin_Hotlists_AmFTP 996
#define Msg_PrefsWin_Hotlists_AmFTPHelp 997
#define Msg_PrefsWin_Hotlists_DOpus 998
#define Msg_PrefsWin_Hotlists_DOpusHelp 999
#define Msg_PrefsWin_Hotlists_AmIRC 1000
#define Msg_PrefsWin_Hotlists_AmIRCHelp 1001
#define Msg_PrefsWin_HotsFill 1002
#define Msg_PrefsWin_HotsFillHelp 1003
#define Msg_PrefsWin_HotsClear 1004
#define Msg_PrefsWin_HotsClearHelp 1005
#define Msg_PrefsWin_Settings_EmptyNodes 1006
#define Msg_PrefsWin_Settings_EmptyNodesHelp 1007
#define Msg_PrefsWin_Settings_GroupsStatus 1008
#define Msg_PrefsWin_Settings_GroupsStatusHelp 1009
#define Msg_PrefsWin_Settings_GroupsStatusRemember 1010
#define Msg_PrefsWin_Settings_GroupsStatusOpened 1011
#define Msg_PrefsWin_Settings_GroupsStatusClosed 1012
#define Msg_PrefsWin_Settings_SectionsImages 1013
#define Msg_PrefsWin_Settings_SectionsImagesHelp 1014
#define Msg_PrefsWin_Settings_GroupsImages 1015
#define Msg_PrefsWin_Settings_GroupsImagesHelp 1016
#define Msg_PrefsWin_Settings_Codesets 1017
#define Msg_PrefsWin_Settings_CodesetsHelp 1018
#define Msg_PrefsWin_Hotlists_Netscape 1019
#define Msg_PrefsWin_Hotlists_NetscapeHelp 1020
#define Msg_PrefsWin_Settings_ManualSort 1021
#define Msg_PrefsWin_Settings_ManualSortHelp 1022
#define Msg_PrefsWin_TitleCard 1023
#define Msg_PrefsWin_CardBack 1024
#define Msg_PrefsWin_CardBackHelp 1025
#define Msg_PrefsWin_CardBackTitle 1026
#define Msg_PrefsWin_TitlesPen 1027
#define Msg_PrefsWin_TitlesPenHelp 1028
#define Msg_PrefsWin_TitlesPenTitle 1029
#define Msg_PrefsWin_TitlesBackPen 1030
#define Msg_PrefsWin_TitlesBackPenHelp 1031
#define Msg_PrefsWin_TitlesBackPenTitle 1032
#define Msg_PrefsWin_URLPen 1033
#define Msg_PrefsWin_URLPenHelp 1034
#define Msg_PrefsWin_URLPenTitle 1035
#define Msg_PrefsWin_URLOverPen 1036
#define Msg_PrefsWin_URLOverPenHelp 1037
#define Msg_PrefsWin_URLOverPenTitle 1038
#define Msg_PrefsWin_CardFont 1039
#define Msg_PrefsWin_CardFontHelp 1040
#define Msg_PrefsWin_TitlesFont 1041
#define Msg_PrefsWin_TitlesFontHelp 1042
#define Msg_PrefsWin_ShowMiniature 1043
#define Msg_PrefsWin_ShowMiniatureHelp 1044
#define Msg_PrefsWin_MiniatureSize 1045
#define Msg_PrefsWin_MiniatureSizeHelp 1046
#define Msg_PrefsWin_Settings_ClosedGroups 1047
#define Msg_PrefsWin_Settings_ClosedGroupsHelp 1048
#define Msg_PrefsWin_MiniatureFrame 1049
#define Msg_PrefsWin_MiniatureFrameHelp 1050
#define Msg_PrefsWin_MiniatureFrameTitle 1051
#define Msg_PrefsWin_Settings_IgnoreLast 1052
#define Msg_PrefsWin_Settings_IgnoreLastHelp 1053
#define Msg_PrefsWin_Hotlists_SimpleMail 1054
#define Msg_PrefsWin_Hotlists_SimpleMailHelp 1055
#define Msg_List_Menu_Title 1200
#define Msg_List_Menu_DefWidthThis 1201
#define Msg_List_Menu_DefWidthAll 1202
#define Msg_List_Menu_DefOrderThis 1203
#define Msg_List_Menu_DefOrderAll 1204
#define Msg_List_Menu_DefSort 1205
#define Msg_Text_Menu_Title 1206
#define Msg_String_Menu_Title 1207
#define Msg_String_Menu_DeleteAll 1208
#define Msg_String_Menu_Cut 1209
#define Msg_String_Menu_Copy 1210
#define Msg_String_Menu_Paste 1211
#define Msg_String_Menu_External 1212
#define Msg_String_Menu_Insert 1213
#define Msg_String_Menu_WordWrap 1214
#define Msg_Bar_Menu_Title 1215
#define Msg_Bar_Menu_ViewMode_TextGfx 1216
#define Msg_Bar_Menu_ViewMode_Gfx 1217
#define Msg_Bar_Menu_ViewMode_Text 1218
#define Msg_Bar_Menu_Borderless 1219
#define Msg_Bar_Menu_Sunny 1220
#define Msg_Bar_Menu_Raised 1221
#define Msg_Menu_Configure 1222
#define Msg_Menu_Columns 1223
#define Msg_Card_Menu_Hide 1224
#define Msg_Card_Menu_Title 1225
#define Msg_Edit_MenuEdit 1400
#define Msg_Edit_MenuCopy 1401
#define Msg_Edit_MenuBrowser 1402
#define Msg_Edit_MenuFTP 1403
#define Msg_Edit_MenuJoin 1404
#define Msg_Edit_MenuTelnet 1405
#define Msg_Edit_MenuEMail 1406
#define Msg_Edit_MenuEMailHome 1407
#define Msg_Edit_MenuEMailBusiness 1408
#define Msg_Edit_MenuCall 1409
#define Msg_Edit_MenuCallHome 1410
#define Msg_Edit_MenuCallMobile 1411
#define Msg_Edit_MenuCallBusiness 1412
#define Msg_Edit_MenuCallBusinessMobile 1413
#define Msg_Edit_MenuFax 1414
#define Msg_Edit_MenuFaxHome 1415
#define Msg_Edit_MenuFaxBusiness 1416
#define Msg_Edit_MenuUser_Title 1417
#define Msg_Edit_MenuUserList_Title 1418
#define Msg_Edit_MenuWWW_Title 1419
#define Msg_Edit_MenuWWWList_Title 1420
#define Msg_Edit_MenuFTP_Title 1421
#define Msg_Edit_MenuFTPList_Title 1422
#define Msg_Edit_MenuChat_Title 1423
#define Msg_Edit_MenuChatList_Title 1424
#define Msg_Edit_MenuTelnet_Title 1425
#define Msg_Edit_MenuTelnetList_Title 1426
#define Msg_Edit_OK 1600
#define Msg_Edit_Cancel 1601
#define Msg_Edit_MenuQuickEMail 1602
#define Msg_Edit_MenuTitle 1603
#define Msg_Edit_UserWinTitle 1800
#define Msg_Edit_UserMenuTitle 1801
#define Msg_Edit_UserNameTitle 1802
#define Msg_Edit_UserHomeTitle 1803
#define Msg_Edit_UserBusiTitle 1804
#define Msg_Edit_UserNoteTitle 1805
#define Msg_Edit_UserImageTitle 1806
#define Msg_Edit_UserFirst 1807
#define Msg_Edit_UserLast 1808
#define Msg_Edit_UserVisualName 1809
#define Msg_Edit_UserEMail 1810
#define Msg_Edit_UserEMailAdd 1811
#define Msg_Edit_UserEMailRem 1812
#define Msg_Edit_UserEMailUp 1813
#define Msg_Edit_UserEMailDown 1814
#define Msg_Edit_UserAddress 1815
#define Msg_Edit_UserCity 1816
#define Msg_Edit_UserZIP 1817
#define Msg_Edit_UserCountry 1818
#define Msg_Edit_UserAlias 1819
#define Msg_Edit_UserPGP 1820
#define Msg_Edit_UserPhone 1821
#define Msg_Edit_UserMobile 1822
#define Msg_Edit_UserFax 1823
#define Msg_Edit_UserICQ 1824
#define Msg_Edit_UserWWW 1825
#define Msg_Edit_UserFTP 1826
#define Msg_Edit_UserCompany 1827
#define Msg_Edit_UserJob 1828
#define Msg_Edit_UserDept 1829
#define Msg_Edit_UserOffice 1830
#define Msg_Edit_UserOffEMail 1831
#define Msg_Edit_UserOffAddress 1832
#define Msg_Edit_UserOffCity 1833
#define Msg_Edit_UserOffZIP 1834
#define Msg_Edit_UserOffCountry 1835
#define Msg_Edit_UserOffPhone 1836
#define Msg_Edit_UserOffMobile 1837
#define Msg_Edit_UserOffFax 1838
#define Msg_Edit_UserImage 1839
#define Msg_Edit_UserImageTransparent 1840
#define Msg_Edit_UserSex 1841
#define Msg_Edit_UserSex_None 1842
#define Msg_Edit_UserSex_Male 1843
#define Msg_Edit_UserSex_Female 1844
#define Msg_Edit_UserBirthday 1845
#define Msg_Edit_UserAnniversary 1846
#define Msg_Edit_UserGetMap 1847
#define Msg_Edit_WWWWinTitle 1900
#define Msg_Edit_WWWMenuTitle 1901
#define Msg_Edit_WWWName 1902
#define Msg_Edit_WWWHost 1903
#define Msg_Edit_WWWAlias 1904
#define Msg_Edit_WWWMaster 1905
#define Msg_Edit_WWWEMail 1906
#define Msg_Edit_WWWComment 1907
#define Msg_Edit_FTPWinTitle 2000
#define Msg_Edit_FTPMenuTitle 2001
#define Msg_Edit_FTPSettingsTitle 2002
#define Msg_Edit_FTPAdvancedTitle 2003
#define Msg_Edit_FTPName 2004
#define Msg_Edit_FTPHost 2005
#define Msg_Edit_FTPAlias 2006
#define Msg_Edit_FTPEMail 2007
#define Msg_Edit_FTPLocal 2008
#define Msg_Edit_FTPLocalUse 2009
#define Msg_Edit_FTPComment 2010
#define Msg_Edit_FTPAdvanced 2011
#define Msg_Edit_FTPUser 2012
#define Msg_Edit_FTPPass 2013
#define Msg_Edit_FTPPort 2014
#define Msg_Edit_FTPRetries 2015
#define Msg_Edit_FTPWinType 2016
#define Msg_Edit_FTPWinTypeDual 2017
#define Msg_Edit_FTPWinTypeSingle 2018
#define Msg_Edit_FTPADT 2019
#define Msg_Edit_FTPAnon 2020
#define Msg_Edit_FTPQuiet 2021
#define Msg_Edit_FTPCompression 2022
#define Msg_Edit_FTPOutput 2023
#define Msg_Edit_FTPHidden 2024
#define Msg_Edit_FTPNOOPs 2025
#define Msg_Edit_FTPUseProxy 2026
#define Msg_Edit_ChatWinTitle 2100
#define Msg_Edit_ChatMenuTitle 2101
#define Msg_Edit_ChatChannel 2102
#define Msg_Edit_ChatPass 2103
#define Msg_Edit_ChatServer 2104
#define Msg_Edit_ChatNick 2105
#define Msg_Edit_ChatMantainer 2106
#define Msg_Edit_ChatWWW 2107
#define Msg_Edit_ChatEMail 2108
#define Msg_Edit_ChatComment 2109
#define Msg_Edit_TelnetWinTitle 2200
#define Msg_Edit_TelnetMenuTitle 2201
#define Msg_Edit_TelnetHost 2202
#define Msg_Edit_TelnetPort 2203
#define Msg_Edit_TelnetSSH 2204
#define Msg_Edit_TelnetLogin 2205
#define Msg_Edit_TelnetPass 2206
#define Msg_Edit_TelnetEMail 2207
#define Msg_Edit_TelnetComment 2208
#define Msg_Edit_TelnetAlias 2209
#define Msg_Dial_WinTitle 2400
#define Msg_Dial_Cancel 2401
#define Msg_Dial_Dialling 2402
#define Msg_Dial_RetryCancelGads 2403
#define Msg_Dial_ModemNotResponding 2404
#define Msg_Dial_Waiting 2405
#define Msg_Dial_NoDevice 2406
#define Msg_MiniMailWin_Title 2500
#define Msg_About_WinTitle 2600
#define Msg_About_Descr 2601
#define Msg_About_Info 2602
#define Msg_About_Version 2603
#define Msg_About_Authors 2604
#define Msg_About_Support 2605
#define Msg_About_Database 2606
#define Msg_About_ThirdParts 2607
#define Msg_About_Translation 2608
#define Msg_About_Translator 2609
#define Msg_About_TranslatorURL 2610
#define Msg_About_OfCourse 2611
#define Msg_About_NListtree 2612
#define Msg_About_NList 2613
#define Msg_About_Popplaceholder 2614
#define Msg_About_Textinput 2615
#define Msg_About_PopupMenu 2616
#define Msg_About_GroupsImages 2617
#define Msg_About_Lamp 2618
#define Msg_DelWarning_Never 2800
#define Msg_DelWarning_Always 2801
#define Msg_DelWarning_Entry 2802
#define Msg_DelWarning_Entries 2803
#define Msg_Card_Title 3000
#define Msg_Card_Contact 3001
#define Msg_Card_Phone 3002
#define Msg_Card_Home 3003
#define Msg_Card_Work 3004
#define Msg_Card_Note 3005
#define Msg_Card_GetMap 3006
#define Msg_Card_Name 3007
#define Msg_Card_Alias 3008
#define Msg_Card_EMail 3009
#define Msg_Card_Phone_Home 3010
#define Msg_Card_Phone_Fax 3011
#define Msg_Card_Phone_Mobile 3012
#define Msg_Card_Phone_Work 3013
#define Msg_Card_Phone_WorkFax 3014
#define Msg_Card_Phone_WorkMobile 3015
#define Msg_Card_Channel 3016
#define Msg_Card_Server 3017
#define Msg_Card_Host 3018

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define Msg_GetDBDir_STR "Select the directory for CM's database"
#define Msg_NOMUINoDirWarningMsg_STR "Warning! Contact Manager won't work properly if\nyou don't provide a directory where to store\nits databases!\n\nDo you want to select the directory now?"
#define Msg_NOMUINoDirWarningGads_STR "Yes|No"
#define MSG_ErrReqTitle_STR "CManager error"
#define MSG_NOMUIErrReqGadget_STR "OK"
#define Msg_Error_NoMem_STR "Not enogh memory available"
#define Msg_Error_NoApp_STR "Contact Manager severe error:\n- not enogh memory available\n- some resource not available\n\nContact Manager needs:\n- AmigaOS 3.1+\n- MUI 19+\n- CManager.library 28+\n- codesets.library 2+\n- vapor_toolkit.library 15+\n- Calendar.mcc 15+\n- CManager.mcc 28+\n- Lamp.mcc 11.2+\n- NList.mcc 20+\n- NListtree.mcc 18+\n- NListview.mcc 19+\n- Popplaceholder.mcc 15+\n- Textinput.mcc 29+\n- TheBarVirt.mcc 11+\n- TheButton.mcc 11+\n- Urltext.mcc 18+\n\nContact Manager uses:\n- genesis.library 2+\n- openurl.library 4+\n- popupmenu.library 10+\n- BWin.mcc 20+\n- MiniMailer.mcc"
#define Msg_Error_NoWin_STR "Can't open window!"
#define Msg_Error_CantLogin_STR "Genesis user login attempt failed."
#define Msg_App_Copyright_STR "Copyright 1998-2000 by Simone Tellini\nCopyright 2000-2005 by Alfonso Ranieri"
#define Msg_App_Description_STR "Global bookmarks manager"
#define Msg_Menu_Project_STR "Project"
#define Msg_Menu_About_STR "?\0About..."
#define Msg_Menu_AboutMUI_STR "!\0About MUI..."
#define Msg_Menu_Hide_STR "H\0Hide"
#define Msg_Menu_Quit_STR "Q\0Quit"
#define Msg_Menu_SaveAndQuit_STR "W\0Save and quit"
#define Msg_Menu_ForceQuit_STR "Z\0Force quit"
#define Msg_Menu_Database_STR "Database"
#define Msg_Menu_User_STR "U\0Change user..."
#define Msg_Menu_Open_STR "O\0Open..."
#define Msg_Menu_Save_STR "S\0Save"
#define Msg_Menu_SaveAs_STR "A\0Save as..."
#define Msg_Menu_LastSaved_STR "L\0Last saved"
#define Msg_Menu_Add_STR "N\0Add"
#define Msg_Menu_Clone_STR "T\0Clone"
#define Msg_Menu_Edit_STR "B\0Edit"
#define Msg_Menu_Remove_STR "R\0Remove"
#define Msg_Menu_SelectAll_STR "+\0Select all"
#define Msg_Menu_SelectNone_STR "-\0Select none"
#define Msg_Menu_SelectToggle_STR "\\\0Select toggle"
#define Msg_Menu_Search_STR "F\0Search..."
#define Msg_Menu_ScanHotlists_STR "I\0Scan hotlists"
#define Msg_Menu_Import_STR "Import"
#define Msg_Menu_ImportYAM_STR "YAM addressbook..."
#define Msg_Menu_ImportMD2_STR "Microdot II addressbook..."
#define Msg_Menu_ImportSTRICQ_STR "STRICQ users..."
#define Msg_Menu_ImportDFA_STR "DFA addresses.."
#define Msg_Menu_ImportSTFax_STR "STFax phonebook..."
#define Msg_Menu_ImportPINT_STR "UMS - PINT addresses..."
#define Msg_Menu_ImportGoodNews_STR "UMS - GoodNews addresses..."
#define Msg_Menu_ImportCSV_STR "Users as CSV..."
#define Msg_Menu_ImportCSVFullOE_STR "Users as OE CSV..."
#define Msg_Menu_ImportThor_STR "Thor addressbook..."
#define Msg_Menu_ImportAWeb_STR "AWeb hotlist..."
#define Msg_Menu_ImportV_STR "Voyager hotlist..."
#define Msg_Menu_ImportIB_STR "IBrowse hotlist..."
#define Msg_Menu_ImportAmFTP_STR "AmFTP profiles..."
#define Msg_Menu_ImportDOpus_STR "DOpus FTP sites..."
#define Msg_Menu_ImportAmIRC_STR "AmIRC servers..."
#define Msg_Menu_ImportNetscape_STR "Firefox hotlist..."
#define Msg_Menu_Export_STR "Export"
#define Msg_Menu_ExportCSV_STR "Users as CSV..."
#define Msg_Menu_ExportCSVFullOE_STR "Users as OE CSV..."
#define Msg_Menu_ExportURLHtml_STR "URLs as HTML..."
#define Msg_Menu_ExportWWWHtml_STR "WWW sites as HTML..."
#define Msg_Menu_ExportFTPHtml_STR "FTP sites as HTML..."
#define Msg_Menu_ExportYAM_STR "Users as YAM addressbook..."
#define Msg_Menu_ExportAWeb_STR "WWW sites as AWeb hotlist..."
#define Msg_Menu_ExportIBrowse_STR "WWW sites as IBrowse hotlist..."
#define Msg_Menu_ExportNetscape_STR "WWW sites as Firefox hotlist..."
#define Msg_Menu_Settings_STR "Settings"
#define Msg_Menu_EditPrefs_STR "G\0Edit..."
#define Msg_Menu_MUI_STR "M\0MUI..."
#define Msg_Menu_SavePrefs_STR "J\0Save"
#define Msg_Menu_LastSavedPrefs_STR "D\0Last saved"
#define Msg_Menu_NoBar_STR "1\0Hide buttons"
#define Msg_Menu_NoGroups_STR "Hide groups"
#define Msg_Menu_NoQuick_STR "2\0Hide quick group"
#define Msg_Menu_NoGroupsCtrl_STR "Hide groups ctrl"
#define Msg_Menu_NoCard_STR "3\0Hide card view"
#define Msg_Menu_ImportSimpleMail_STR "SimpleMail addressbook..."
#define Msg_Menu_ExportSimpleMail_STR "Users as SimpleMail addressbook..."
#define Msg_Menu_ImportATC_STR "ATC FTP sites..."
#define Msg_Menu_ExportATC_STR "FTP sites as ATC profiles..."
#define Msg_Req_Title_STR "Contact Manager"
#define Msg_Req_HotChangedMsg_STR "The \33b%s\33n hotlist has changed.\nDo you want to import it again?"
#define Msg_Req_HotChangedGads_STR "*_Yes|_No"
#define Msg_Req_NotSavedMsg_STR "Save changes to the database?"
#define Msg_Req_NotSavedGads_STR "*_Yes|_No|_Cancel"
#define Msg_Req_ChangedMsg_STR "Another application modified the database.\nReload it ?"
#define Msg_Req_ChangedGads_STR "*_Yes|_No"
#define Msg_Req_Asl_STR "There are still some asl requesters opened.\nPlease close them."
#define Msg_Req_AslGads_STR "*_OK"
#define Msg_Req_DelWarningOne_STR "Do you really want to delete\nthe selected entry?"
#define Msg_Req_DelWarningMany_STR "Do you really want to delete\nthe %ld selected entries?"
#define Msg_Req_DelWarningGads_STR "*_Yes|_No"
#define Msg_Req_GroupDelWarningOne_STR "Do you really want to delete\nthe selected group?"
#define Msg_Req_GroupDelWarningMany_STR "Do you really want to delete\nthe %ld selected groups?"
#define Msg_Req_GroupDelWarningGads_STR "*_Yes|_No"
#define Msg_Win_Title_STR "Contact Manager (%s)"
#define Msg_Gad_Add_STR "_Add"
#define Msg_Gad_AddHelp_STR "Add a new entry."
#define Msg_Gad_Clone_STR "_Clone"
#define Msg_Gad_CloneHelp_STR "Duplicate the selected entries."
#define Msg_Gad_Edit_STR "_Edit"
#define Msg_Gad_EditHelp_STR "Edit the selected entries."
#define Msg_Gad_Remove_STR "_Remove"
#define Msg_Gad_RemoveHelp_STR "Remove the selected entry."
#define Msg_Gad_Search_STR "Searc_h"
#define Msg_Gad_SearchHelp_STR "Open the search window."
#define Msg_Gad_Save_STR "_Save"
#define Msg_Gad_SaveHelp_STR "Save the database."
#define Msg_Gad_Prefs_STR "_Prefs"
#define Msg_Gad_PrefsHelp_STR "Open the preferences window."
#define Msg_Gad_Login_STR "_User"
#define Msg_Gad_LoginHelp_STR "Open the login window."
#define Msg_Gad_GroupName_STR "Name"
#define Msg_Gad_GroupNameHelp_STR "Adjust the name of the active group."
#define Msg_Gad_AddGroup_STR "A_dd"
#define Msg_Gad_AddGroupHelp_STR "Add a group."
#define Msg_Gad_RemoveGroup_STR "Re_m"
#define Msg_Gad_RemoveGroupHelp_STR "Remove the selected group."
#define Msg_Gad_GroupUpHelp_STR "Move the active group up."
#define Msg_Gad_GroupDownHelp_STR "Move the active group down."
#define Msg_Groups_Menu_Title_STR "Groups"
#define Msg_Groups_Menu_OpenThis_STR "Open this"
#define Msg_Groups_Menu_OpenAll_STR "Open all"
#define Msg_Groups_Menu_CloseThis_STR "Close this"
#define Msg_Groups_Menu_CloseAll_STR "Close all"
#define Msg_Groups_Menu_Sort_STR "Sort"
#define Msg_Gad_Locate_STR "_Locate"
#define Msg_Gad_LocateHelp_STR "Locate an entry in the current group."
#define Msg_Entries_STR "Entries:%ld"
#define Msg_Gad_Sort_STR "Sort"
#define Msg_Gad_SortHelp_STR "Sort entries."
#define Msg_Users_STR "Users"
#define Msg_WWW_STR "WWW sites"
#define Msg_FTP_STR "FTP sites"
#define Msg_Chat_STR "Chat"
#define Msg_Telnet_STR "Telnet"
#define Msg_Title_Name_STR "Name"
#define Msg_Title_LastName_STR "Last name"
#define Msg_Title_EMail_STR "EMail"
#define Msg_Title_Comment_STR "Comment"
#define Msg_Title_Alias_STR "Alias"
#define Msg_Title_Birthday_STR "Birthday"
#define Msg_Title_Anniversary_STR "Anniversary"
#define Msg_Title_Country_STR "Country"
#define Msg_Title_City_STR "City"
#define Msg_Title_Phone_STR "Home phone"
#define Msg_Title_Mobile_STR "Mobile"
#define Msg_Title_Fax_STR "Fax"
#define Msg_Title_WWW_STR "Home page"
#define Msg_Title_Company_STR "Company"
#define Msg_Title_BPhone_STR "Office phone"
#define Msg_Title_BMobile_STR "Company mobile"
#define Msg_Title_BFax_STR "Office fax"
#define Msg_Title_Site_STR "Site"
#define Msg_Title_LastVisited_STR "Last visited"
#define Msg_Title_LastModified_STR "Last modified"
#define Msg_Title_Channel_STR "Channel"
#define Msg_Title_Server_STR "Server"
#define Msg_Title_Host_STR "Host"
#define Msg_Title_Port_STR "Port"
#define Msg_Never_STR "Never"
#define Msg_Quick_UserFirst_STR "_First"
#define Msg_Quick_UserLast_STR "Las_t"
#define Msg_Quick_UserEMail_STR "EMa_il"
#define Msg_Quick_WWWName_STR "_Name"
#define Msg_Quick_WWWHost_STR "S_ite"
#define Msg_Quick_FTPName_STR "_Name"
#define Msg_Quick_FTPHost_STR "S_ite"
#define Msg_Quick_ChatChannel_STR "Cha_nnel"
#define Msg_Quick_ChatServer_STR "Ser_ver"
#define Msg_Quick_TelnetHost_STR "Hos_t"
#define Msg_Quick_TelnetPort_STR "P_ort"
#define Msg_Quick_TelnetLogin_STR "Lo_gin"
#define Msg_Quick_TelnetPass_STR "Pass_word"
#define Msg_SearchWin_Title_STR "Search database"
#define Msg_SearchWin_Pattern_STR "P_attern"
#define Msg_SearchWin_PatternHelp_STR "The string to search for."
#define Msg_SearchWin_SearchInTitle_STR "Search in"
#define Msg_SearchWin_Users_STR "_Users"
#define Msg_SearchWin_UsersHelp_STR "If seletced, include Users in searching."
#define Msg_SearchWin_WWW_STR "_WWW sites"
#define Msg_SearchWin_WWWHelp_STR "If selected, include WWW sites in searching."
#define Msg_SearchWin_FTP_STR "F_TP sites"
#define Msg_SearchWin_FTPHelp_STR "If selected, include FTP sites in searching."
#define Msg_SearchWin_Chat_STR "C_hat"
#define Msg_SearchWin_ChatHelp_STR "If selected, include Chat servers in searching."
#define Msg_SearchWin_Telnet_STR "Te_lnet"
#define Msg_SearchWin_TelnetHelp_STR "If selected, include Telnet hosts in searching."
#define Msg_SearchWin_OptionsTitle_STR "Options"
#define Msg_SearchWin_Comment_STR "Search in c_omment"
#define Msg_SearchWin_CommentHelp_STR "If selected, include comments in searching."
#define Msg_SearchWin_Case_STR "Cas_e"
#define Msg_SearchWin_CaseHelp_STR "If selected, searching is case sensitive."
#define Msg_SearchWin_Wildcards_STR "W_ildcards"
#define Msg_SearchWin_WildcardsHelp_STR "If selected, Pattern is an AmigaDOS\npattern, rather than a string."
#define Msg_SearchWin_Search_STR "_Search"
#define Msg_SearchWin_SearchHelp_STR "Search for next match."
#define Msg_SearchWin_First_STR "_First"
#define Msg_SearchWin_FirstHelp_STR "Restart searching from the beginning."
#define Msg_SearchWin_Previous_STR "_Previous"
#define Msg_SearchWin_PreviousHelp_STR "Search for previous match."
#define Msg_SearchWin_Cancel_STR "_Cancel"
#define Msg_SearchWin_CancelHelp_STR "Close the search window."
#define Msg_SearchWin_Searching_STR "Searching..."
#define Msg_SearchWin_Welcome_STR "Search database."
#define Msg_SearchWin_Found_STR "Pattern found in %s."
#define Msg_SearchWin_NotFound_STR "Pattern not found."
#define Msg_UserWin_Title_STR "Change user"
#define Msg_UserWin_User_STR "_User"
#define Msg_UserWin_UserHelp_STR "Change current Contact Manager user."
#define Msg_PrefsWin_Title_STR "Contact Manager preferences"
#define Msg_PrefsWin_TitleSettings_STR "General"
#define Msg_PrefsWin_TitleScripts_STR "Scripts"
#define Msg_PrefsWin_TitleHotlists_STR "Hotlists"
#define Msg_PrefsWin_TitleDialer_STR "Dialer"
#define Msg_PrefsWin_Save_STR "_Save"
#define Msg_PrefsWin_SaveHelp_STR "Save the preferences and exit."
#define Msg_PrefsWin_Use_STR "_Use"
#define Msg_PrefsWin_UseHelp_STR "Use the preferences and exit."
#define Msg_PrefsWin_Apply_STR "_Apply"
#define Msg_PrefsWin_ApplyHelp_STR "Use the preferences without exiting."
#define Msg_PrefsWin_Cancel_STR "_Cancel"
#define Msg_PrefsWin_CancelHelp_STR "Exit without saving the preferences."
#define Msg_PrefsWin_Settings_Locale_STR "Locale"
#define Msg_PrefsWin_Settings_Country_STR "C_ountry"
#define Msg_PrefsWin_Settings_CountryHelp_STR "The name of the country you live in."
#define Msg_PrefsWin_Settings_CountryCode_STR "Countr_y code"
#define Msg_PrefsWin_Settings_CountryCodeHelp_STR "Your country telephone prefix."
#define Msg_PrefsWin_Settings_GetFromLocale_STR "Get _from locale"
#define Msg_PrefsWin_Settings_GetFromLocaleHelp_STR "Try to obtain the country information\nfrom the system."
#define Msg_PrefsWin_Settings_Options_STR "Settings"
#define Msg_PrefsWin_Settings_DelWarning_STR "As_k when deleting"
#define Msg_PrefsWin_Settings_DelWarningHelp_STR "Ask user when deleting more\nthan this number of entries."
#define Msg_PrefsWin_Settings_DoubleClick_STR "_On double click"
#define Msg_PrefsWin_Settings_DoubleClickHelp_STR "Action to perform when double-clicking\non FTP or WWW entries."
#define Msg_PrefsWin_Settings_DCEdit_STR "Edit entry"
#define Msg_PrefsWin_Settings_DCScript_STR "Start script"
#define Msg_PrefsWin_Settings_DCScriptQuit_STR "Start script and quit"
#define Msg_PrefsWin_Settings_DCScriptHide_STR "Start script and hide"
#define Msg_PrefsWin_Settings_SilentScan_STR "S_ilent scan hotlists"
#define Msg_PrefsWin_Settings_SilentScanHelp_STR "If selected, Contact Manager imports\nyour hotlists as they change, without\nasking for confirmation."
#define Msg_PrefsWin_Settings_UseOpenURL_STR "Use O_penURL"
#define Msg_PrefsWin_Settings_UseOpenURLHelp_STR "If selected, Contact Manager uses\nOpenURL, if installed."
#define Msg_PrefsWin_Settings_BoldSections_STR "Bol_d sections"
#define Msg_PrefsWin_Settings_BoldSectionsHelp_STR "If selected, main groups are rendered bold."
#define Msg_PrefsWin_Settings_BoldTitles_STR "_Bold entry list titles"
#define Msg_PrefsWin_Settings_BoldTitlesHelp_STR "If selected, list titles are rendered bold."
#define Msg_PrefsWin_Settings_AutoReload_STR "Au_to reload database on changes"
#define Msg_PrefsWin_Settings_AutoReloadHelp_STR "If selected, Contact Manager reloads the\ndatabase anytime it changes, without\nasking for confirmation."
#define Msg_PrefsWin_Settings_SaveOnExit_STR "Sa_ve on exit"
#define Msg_PrefsWin_Settings_SaveOnExitHelp_STR "If selected, Contact Manager saves the\ndatabase when quitting, without\nasking for confirmation."
#define Msg_PrefsWin_Settings_SilentExit_STR "E_xit without confirmation"
#define Msg_PrefsWin_Settings_SilentExitHelp_STR "If selected, Contact Manager exits without\nasking the user, even if data haven't\nbeen saved."
#define Msg_PrefsWin_Settings_SaveOnChanges_STR "Sav_e on every change"
#define Msg_PrefsWin_Settings_SaveOnChangesHelp_STR "If selected, Contact Manager saves\nthe database on every change."
#define Msg_PrefsWin_Settings_NoListBar_STR "No bars among co_lumns"
#define Msg_PrefsWin_Settings_NoListBarHelp_STR "If selected, list has no bar between columns."
#define Msg_PrefsWin_Settings_NtCheckChanges_STR "Don't check _for database changes"
#define Msg_PrefsWin_Settings_NtCheckChangesHelp_STR "If selected, Contact Manager ignores database\nchanges made by other applications."
#define Msg_PrefsWin_Settings_RightMouseActive_STR "Rig_ht mouse changes active"
#define Msg_PrefsWin_Settings_RightMouseActiveHelp_STR "If selected, the right mouse button\nchanges the active entry."
#define Msg_PrefsWin_Settings_EditNew_STR "Edit ne_w entryies"
#define Msg_PrefsWin_Settings_EditNewHelp_STR "If selected, Contact Manager opens\nthe edit window for any new entry."
#define Msg_PrefsWin_Settings_ButtonsBar_STR "TheBar"
#define Msg_PrefsWin_Settings_BarPosition_STR "_Position"
#define Msg_PrefsWin_Settings_BarPositionHelp_STR "Adjust the position of the buttons bar."
#define Msg_PrefsWin_Settings_BarPositionTop_STR "Top"
#define Msg_PrefsWin_Settings_BarPositionBottom_STR "Bottom"
#define Msg_PrefsWin_Settings_BarPositionLeft_STR "Left"
#define Msg_PrefsWin_Settings_BarPositionRight_STR "Right"
#define Msg_PrefsWin_Settings_BarPositionInnertTop_STR "Innert top"
#define Msg_PrefsWin_Settings_BarPositionInnerBottom_STR "Inner bottom"
#define Msg_PrefsWin_Settings_ViewMode_STR "Appa_reance"
#define Msg_PrefsWin_Settings_ViewModeHelp_STR "Adjust the appareance of the buttons bar."
#define Msg_PrefsWin_Settings_TextGfx_STR "Icons and text"
#define Msg_PrefsWin_Settings_Gfx_STR "Icons only"
#define Msg_PrefsWin_Settings_Text_STR "Text only"
#define Msg_PrefsWin_Settings_Borderless_STR "B_orderless"
#define Msg_PrefsWin_Settings_BorderlessHelp_STR "If selected, buttons are borderless."
#define Msg_PrefsWin_Settings_Sunny_STR "_Highlight"
#define Msg_PrefsWin_Settings_SunnyHelp_STR "If selected, buttons, normally in black\nand white, are colored when the\nmouse is over them."
#define Msg_PrefsWin_Settings_Raised_STR "Ra_ised"
#define Msg_PrefsWin_Settings_RaisedHelp_STR "If selected, buttons, normally frameless,\nare framed when the mouse is over them."
#define Msg_PrefsWin_Settings_Scaled_STR "Scale_d"
#define Msg_PrefsWin_Settings_ScaledHelp_STR "If selected, buttons are scaled\naccording to MUI settings."
#define Msg_PrefsWin_Settings_Underscore_STR "Shor_t cuts"
#define Msg_PrefsWin_Settings_UnderscoreHelp_STR "If selected, buttons use\nkeyboard short-cuts."
#define Msg_PrefsWin_Settings_ButtonsPosition_STR "_Buttons position"
#define Msg_PrefsWin_Settings_ButtonsPositionHelp_STR "Adjust the position of the buttons."
#define Msg_PrefsWin_Settings_ButtonsPosition_Left_STR "Left (Top)"
#define Msg_PrefsWin_Settings_ButtonsPosition_Center_STR "Center"
#define Msg_PrefsWin_Settings_ButtonsPosition_Right_STR "Right (Bottom)"
#define Msg_PrefsWin_Settings_LabelPosition_STR "_Label position"
#define Msg_PrefsWin_Settings_LabelPositionHelp_STR "Adjust the position of the text\nfor \"Icons and text\" buttons."
#define Msg_PrefsWin_Settings_LabelPosition_Bottom_STR "Bottom"
#define Msg_PrefsWin_Settings_LabelPosition_Top_STR "Top"
#define Msg_PrefsWin_Settings_LabelPosition_Right_STR "Right"
#define Msg_PrefsWin_Settings_LabelPosition_Left_STR "Left"
#define Msg_PrefsWin_Settings_BarSpacer_STR "Bar spac_er"
#define Msg_PrefsWin_Settings_BarSpacerHelp_STR "If selected, a bar spacer is used\nas buttons separator."
#define Msg_PrefsWin_Settings_Frame_STR "Buttons bar _frame"
#define Msg_PrefsWin_Settings_FrameHelp_STR "If selected, no frame is drawn\naround the buttons."
#define Msg_PrefsWin_Settings_DragBar_STR "Dra_ggable buttons bar"
#define Msg_PrefsWin_Settings_DragBarHelp_STR "If selected, no buttons dragbar\nis available."
#define Msg_PrefsWin_Programs_UserScript_STR "---"
#define Msg_PrefsWin_Programs_MailerTitle_STR "Send EMail"
#define Msg_PrefsWin_Programs_Mailer_STR "Script"
#define Msg_PrefsWin_Programs_MailerHelp_STR "The script to execute when the\n\"Send EMail\" item is selected."
#define Msg_PrefsWin_Programs_MailerScript_STR "Name"
#define Msg_PrefsWin_Programs_MailerScriptHelp_STR "The script to start."
#define Msg_PrefsWin_Programs_MailerPath_STR "Client"
#define Msg_PrefsWin_Programs_MailerPathHelp_STR "The path to the associated application."
#define Msg_PrefsWin_Programs_WWWTitle_STR "Send to Browser"
#define Msg_PrefsWin_Programs_WWW_STR "Script"
#define Msg_PrefsWin_Programs_WWWHelp_STR "The script to execute when the\n\"Send to browser\" item is selected."
#define Msg_PrefsWin_Programs_WWWScript_STR "Name"
#define Msg_PrefsWin_Programs_WWWScriptHelp_STR "The script to start."
#define Msg_PrefsWin_Programs_WWWPath_STR "Client"
#define Msg_PrefsWin_Programs_WWWPathHelp_STR "The path to the associated application."
#define Msg_PrefsWin_Programs_FTPTitle_STR "Send to FTP"
#define Msg_PrefsWin_Programs_FTP_STR "Script"
#define Msg_PrefsWin_Programs_FTPHelp_STR "The script to execute when the\n\"Send to FTP\" item is selected."
#define Msg_PrefsWin_Programs_FTPScript_STR "Name"
#define Msg_PrefsWin_Programs_FTPScriptHelp_STR "The script to start."
#define Msg_PrefsWin_Programs_FTPPath_STR "Client"
#define Msg_PrefsWin_Programs_FTPPathHelp_STR "The path to the associated application."
#define Msg_PrefsWin_Programs_FTPLocal_STR "Drawer"
#define Msg_PrefsWin_Programs_FTPLocalHelp_STR "Define a global drawer\nfor any FTP site."
#define Msg_PrefsWin_Programs_FTPLocalUse_STR "Use"
#define Msg_PrefsWin_Programs_FTPLocalUseHelp_STR "If selected, the drawer is used."
#define Msg_PrefsWin_Programs_ChatTitle_STR "Join Chat"
#define Msg_PrefsWin_Programs_Chat_STR "Script"
#define Msg_PrefsWin_Programs_ChatHelp_STR "The script to execute when the\n\"Join Chat\" item is selected."
#define Msg_PrefsWin_Programs_ChatScript_STR "Name"
#define Msg_PrefsWin_Programs_ChatScriptHelp_STR "The script to start."
#define Msg_PrefsWin_Programs_ChatPath_STR "Client"
#define Msg_PrefsWin_Programs_ChatPathHelp_STR "The path to the associated application."
#define Msg_PrefsWin_Programs_TelnetTitle_STR "Send to Telnet"
#define Msg_PrefsWin_Programs_Telnet_STR "Script"
#define Msg_PrefsWin_Programs_TelnetHelp_STR "The script to execute when the\n\"Send to Telnet\" item is selected."
#define Msg_PrefsWin_Programs_TelnetScript_STR "Name"
#define Msg_PrefsWin_Programs_TelnetScriptHelp_STR "The script to start"
#define Msg_PrefsWin_Programs_TelnetPath_STR "Client"
#define Msg_PrefsWin_Programs_TelnetPathHelp_STR "The path to the associated application."
#define Msg_PrefsWin_Programs_FaxTitle_STR "Send Fax"
#define Msg_PrefsWin_Programs_Fax_STR "Script"
#define Msg_PrefsWin_Programs_FaxHelp_STR "The script to execute when the\n\"Send Fax\" item is selected."
#define Msg_PrefsWin_Programs_FaxScript_STR "Name"
#define Msg_PrefsWin_Programs_FaxScriptHelp_STR "The script to start"
#define Msg_PrefsWin_Programs_FaxPath_STR "Client"
#define Msg_PrefsWin_Programs_FaxPathHelp_STR "The path to the associated application."
#define Msg_PrefsWin_Programs_SavedProgTitle_STR "Application to execute on saving"
#define Msg_PrefsWin_Programs_SavedProg_STR "Application"
#define Msg_PrefsWin_Programs_SavedProgHelp_STR "Application to run anytime data are saved."
#define Msg_PrefsWin_Programs_SavedProgUse_STR "Use"
#define Msg_PrefsWin_Programs_SavedProgUseHelp_STR "If selected, the application is executed."
#define Msg_PrefsWin_Programs_SavedProgPopData_STR "%d|data"
#define Msg_PrefsWin_Dialer_Dialer_STR "_Dialer"
#define Msg_PrefsWin_Dialer_DialerHelp_STR "Select if to use the internal dialer\nor an external dialer application."
#define Msg_PrefsWin_Dialer_DExternal_STR "External application"
#define Msg_PrefsWin_Dialer_DInternal_STR "Internal"
#define Msg_PrefsWin_Dialer_Device_STR "D_evice"
#define Msg_PrefsWin_Dialer_DeviceHelp_STR "The device to use when dialing."
#define Msg_PrefsWin_Dialer_Unit_STR "U_nit"
#define Msg_PrefsWin_Dialer_UnitHelp_STR "The unit for the device."
#define Msg_PrefsWin_Dialer_Shared_STR "S_hared"
#define Msg_PrefsWin_Dialer_SharedHelp_STR "If selected, the device is opened\nin shared mode."
#define Msg_PrefsWin_Dialer_ModemType_STR "_Modem type"
#define Msg_PrefsWin_Dialer_ModemTypeHelp_STR "The type of your modem."
#define Msg_PrefsWin_Dialer_ModemInit_STR "Init st_ring"
#define Msg_PrefsWin_Dialer_ModemInitHelp_STR "The init string for your modem."
#define Msg_PrefsWin_Dialer_DialPrefix_STR "Dial _prefix"
#define Msg_PrefsWin_Dialer_DialPrefixHelp_STR "The dial prefix string for your modem."
#define Msg_PrefsWin_Dialer_DialSuffix_STR "Dial su_ffix"
#define Msg_PrefsWin_Dialer_DialSuffixHelp_STR "The dial suffix string for your modem."
#define Msg_PrefsWin_Dialer_RedialDelay_STR "De_lay"
#define Msg_PrefsWin_Dialer_RedialDelayHelp_STR "The number of seconds to wait\nbetween redial attempts."
#define Msg_PrefsWin_Dialer_RedialAttempts_STR "A_ttempts"
#define Msg_PrefsWin_Dialer_RedialAttemptsHelp_STR "The number of the redial attempts."
#define Msg_PrefsWin_Dialer_Log_STR "Log"
#define Msg_PrefsWin_Dialer_LogHelp_STR "Log dial operations."
#define Msg_PrefsWin_Dialer_Program_STR "A_pplication"
#define Msg_PrefsWin_Dialer_ProgramHelp_STR "The external application\nto use as dialer."
#define Msg_PrefsWin_Dialer_ProgramPopNumber_STR "%s|phone number"
#define Msg_PrefsWin_Hotlists_V_STR "_Voyager"
#define Msg_PrefsWin_Hotlists_VHelp_STR "The complete path to Voyager hotlist."
#define Msg_PrefsWin_Hotlists_IB_STR "_IBrowse"
#define Msg_PrefsWin_Hotlists_IBHelp_STR "The complete path to IBrowse hotlist."
#define Msg_PrefsWin_Hotlists_AWeb_STR "A_Web"
#define Msg_PrefsWin_Hotlists_AWebHelp_STR "The complete path to AWeb hotlist."
#define Msg_PrefsWin_Hotlists_MD2_STR "Micr_odot II"
#define Msg_PrefsWin_Hotlists_MD2Help_STR "The complete path to Microdot II addressbook."
#define Msg_PrefsWin_Hotlists_YAM_STR "_YAM"
#define Msg_PrefsWin_Hotlists_YAMHelp_STR "The complete path to YAM addressbook."
#define Msg_PrefsWin_Hotlists_STRICQ_STR "STRIC_Q users"
#define Msg_PrefsWin_Hotlists_STRICQHelp_STR "The complete path to STRICQ users."
#define Msg_PrefsWin_Hotlists_GoodNews_STR "UMS - _GoodNews"
#define Msg_PrefsWin_Hotlists_GoodNewsHelp_STR "The complete path to GoodNews addresses."
#define Msg_PrefsWin_Hotlists_PINT_STR "UMS - PI_NT"
#define Msg_PrefsWin_Hotlists_PINTHelp_STR "The complete path to PINT addresses."
#define Msg_PrefsWin_Hotlists_DFA_STR "_DFA"
#define Msg_PrefsWin_Hotlists_DFAHelp_STR "The complete path to DFA addresses."
#define Msg_PrefsWin_Hotlists_STFax_STR "STFa_x"
#define Msg_PrefsWin_Hotlists_STFaxHelp_STR "The complete path to STFax phonebook."
#define Msg_PrefsWin_Hotlists_Thor_STR "Thor"
#define Msg_PrefsWin_Hotlists_ThorHelp_STR "The complete path to Thor addresses."
#define Msg_PrefsWin_Hotlists_AmFTP_STR "AmF_TP"
#define Msg_PrefsWin_Hotlists_AmFTPHelp_STR "The complete path to AmFTP profiles."
#define Msg_PrefsWin_Hotlists_DOpus_STR "DO_pus FTP"
#define Msg_PrefsWin_Hotlists_DOpusHelp_STR "The complete path to DOpus FTP sites."
#define Msg_PrefsWin_Hotlists_AmIRC_STR "A_mIRC"
#define Msg_PrefsWin_Hotlists_AmIRCHelp_STR "The complete path to AmIRC servers."
#define Msg_PrefsWin_HotsFill_STR "_Fill"
#define Msg_PrefsWin_HotsFillHelp_STR "Try to obtain hotlists information\nfrom the system."
#define Msg_PrefsWin_HotsClear_STR "C_lear"
#define Msg_PrefsWin_HotsClearHelp_STR "Clear all hotlists."
#define Msg_PrefsWin_Settings_EmptyNodes_STR "Displa_y empty nodes as leaves"
#define Msg_PrefsWin_Settings_EmptyNodesHelp_STR "If selected, empty nodes are displayed as leaves."
#define Msg_PrefsWin_Settings_GroupsStatus_STR "_Groups status"
#define Msg_PrefsWin_Settings_GroupsStatusHelp_STR "Select the status of the groups at loading."
#define Msg_PrefsWin_Settings_GroupsStatusRemember_STR "As last saved"
#define Msg_PrefsWin_Settings_GroupsStatusOpened_STR "Always opened"
#define Msg_PrefsWin_Settings_GroupsStatusClosed_STR "Always closed"
#define Msg_PrefsWin_Settings_SectionsImages_STR "Use sectio_ns images"
#define Msg_PrefsWin_Settings_SectionsImagesHelp_STR "If selected, sections images are used."
#define Msg_PrefsWin_Settings_GroupsImages_STR "Use g_roups images"
#define Msg_PrefsWin_Settings_GroupsImagesHelp_STR "If selected, groups images are used."
#define Msg_PrefsWin_Settings_Codesets_STR "_Codeset"
#define Msg_PrefsWin_Settings_CodesetsHelp_STR "The codeset to use."
#define Msg_PrefsWin_Hotlists_Netscape_STR "Fi_refox"
#define Msg_PrefsWin_Hotlists_NetscapeHelp_STR "The complete path to Firefox hotlist."
#define Msg_PrefsWin_Settings_ManualSort_STR "_Manual sort"
#define Msg_PrefsWin_Settings_ManualSortHelp_STR "If selected, Contact Manager doesn't\nsort entries."
#define Msg_PrefsWin_TitleCard_STR "Card view"
#define Msg_PrefsWin_CardBack_STR "Card _background"
#define Msg_PrefsWin_CardBackHelp_STR "Select the card background."
#define Msg_PrefsWin_CardBackTitle_STR "Select the card background"
#define Msg_PrefsWin_TitlesPen_STR "_Titles pen"
#define Msg_PrefsWin_TitlesPenHelp_STR "Select the titles pen."
#define Msg_PrefsWin_TitlesPenTitle_STR "Select the titles pen"
#define Msg_PrefsWin_TitlesBackPen_STR "T_itles back pen"
#define Msg_PrefsWin_TitlesBackPenHelp_STR "Select the titles background pen."
#define Msg_PrefsWin_TitlesBackPenTitle_STR "Select the titles back pen"
#define Msg_PrefsWin_URLPen_STR "U_rl pen"
#define Msg_PrefsWin_URLPenHelp_STR "Select the URL pen."
#define Msg_PrefsWin_URLPenTitle_STR "Select the URL pen"
#define Msg_PrefsWin_URLOverPen_STR "Url _over pen"
#define Msg_PrefsWin_URLOverPenHelp_STR "Select the URL over pen."
#define Msg_PrefsWin_URLOverPenTitle_STR "Select the URL over pen"
#define Msg_PrefsWin_CardFont_STR "Card title font"
#define Msg_PrefsWin_CardFontHelp_STR "Select the card title font."
#define Msg_PrefsWin_TitlesFont_STR "Titles font"
#define Msg_PrefsWin_TitlesFontHelp_STR "Select the titles font."
#define Msg_PrefsWin_ShowMiniature_STR "Show _miniatures"
#define Msg_PrefsWin_ShowMiniatureHelp_STR "If selected, miniatures are shown."
#define Msg_PrefsWin_MiniatureSize_STR "Mi_niatures size"
#define Msg_PrefsWin_MiniatureSizeHelp_STR "Max miniatures size."
#define Msg_PrefsWin_Settings_ClosedGroups_STR "Groups closed on import"
#define Msg_PrefsWin_Settings_ClosedGroupsHelp_STR "If selected, imported groups are inserted closed."
#define Msg_PrefsWin_MiniatureFrame_STR "_Miniature frame"
#define Msg_PrefsWin_MiniatureFrameHelp_STR "Select the miniature frame."
#define Msg_PrefsWin_MiniatureFrameTitle_STR "Select the miniature frame"
#define Msg_PrefsWin_Settings_IgnoreLast_STR "Ignore last visited _& changed"
#define Msg_PrefsWin_Settings_IgnoreLastHelp_STR "If selected, last visited and last changed\nfields of WWW and FTP entries are ignored."
#define Msg_PrefsWin_Hotlists_SimpleMail_STR "SimpleMail"
#define Msg_PrefsWin_Hotlists_SimpleMailHelp_STR "The complete path to simpleMail addressbook."
#define Msg_List_Menu_Title_STR "List"
#define Msg_List_Menu_DefWidthThis_STR "Default Width: this"
#define Msg_List_Menu_DefWidthAll_STR "Default Width: all"
#define Msg_List_Menu_DefOrderThis_STR "Default Order: this"
#define Msg_List_Menu_DefOrderAll_STR "Default Order: all"
#define Msg_List_Menu_DefSort_STR "Default sorting"
#define Msg_Text_Menu_Title_STR "Text"
#define Msg_String_Menu_Title_STR "Edit"
#define Msg_String_Menu_DeleteAll_STR "Delete all"
#define Msg_String_Menu_Cut_STR "Cut"
#define Msg_String_Menu_Copy_STR "Copy"
#define Msg_String_Menu_Paste_STR "Paste"
#define Msg_String_Menu_External_STR "External edit..."
#define Msg_String_Menu_Insert_STR "Insert text file..."
#define Msg_String_Menu_WordWrap_STR "WordWrap"
#define Msg_Bar_Menu_Title_STR "TheBar"
#define Msg_Bar_Menu_ViewMode_TextGfx_STR "Icons and text"
#define Msg_Bar_Menu_ViewMode_Gfx_STR "Icons only"
#define Msg_Bar_Menu_ViewMode_Text_STR "Text only"
#define Msg_Bar_Menu_Borderless_STR "Borderless"
#define Msg_Bar_Menu_Sunny_STR "Highlight"
#define Msg_Bar_Menu_Raised_STR "Raised"
#define Msg_Menu_Configure_STR "Configure"
#define Msg_Menu_Columns_STR "Columns"
#define Msg_Card_Menu_Hide_STR "Hide"
#define Msg_Card_Menu_Title_STR "Card"
#define Msg_Edit_MenuEdit_STR "Edit"
#define Msg_Edit_MenuCopy_STR "Copy to clip"
#define Msg_Edit_MenuBrowser_STR "Send to Browser"
#define Msg_Edit_MenuFTP_STR "Send to FTP"
#define Msg_Edit_MenuJoin_STR "Join Chat"
#define Msg_Edit_MenuTelnet_STR "Send to Telnet"
#define Msg_Edit_MenuEMail_STR "Send EMail"
#define Msg_Edit_MenuEMailHome_STR "Home"
#define Msg_Edit_MenuEMailBusiness_STR "Business"
#define Msg_Edit_MenuCall_STR "Call"
#define Msg_Edit_MenuCallHome_STR "Home"
#define Msg_Edit_MenuCallMobile_STR "Mobile"
#define Msg_Edit_MenuCallBusiness_STR "Business"
#define Msg_Edit_MenuCallBusinessMobile_STR "Business mobile"
#define Msg_Edit_MenuFax_STR "Send Fax"
#define Msg_Edit_MenuFaxHome_STR "Home"
#define Msg_Edit_MenuFaxBusiness_STR "Business"
#define Msg_Edit_MenuUser_Title_STR "User"
#define Msg_Edit_MenuUserList_Title_STR "User list"
#define Msg_Edit_MenuWWW_Title_STR "WWW"
#define Msg_Edit_MenuWWWList_Title_STR "WWW list"
#define Msg_Edit_MenuFTP_Title_STR "FTP"
#define Msg_Edit_MenuFTPList_Title_STR "FTP list"
#define Msg_Edit_MenuChat_Title_STR "Chat"
#define Msg_Edit_MenuChatList_Title_STR "Chat list"
#define Msg_Edit_MenuTelnet_Title_STR "Telnet"
#define Msg_Edit_MenuTelnetList_Title_STR "Telnet list"
#define Msg_Edit_OK_STR "_Use"
#define Msg_Edit_Cancel_STR "_Cancel"
#define Msg_Edit_MenuQuickEMail_STR "Quick EMail"
#define Msg_Edit_MenuTitle_STR "User"
#define Msg_Edit_UserWinTitle_STR "Edit user"
#define Msg_Edit_UserMenuTitle_STR "User actions"
#define Msg_Edit_UserNameTitle_STR "Name"
#define Msg_Edit_UserHomeTitle_STR "Home"
#define Msg_Edit_UserBusiTitle_STR "Office"
#define Msg_Edit_UserNoteTitle_STR "Notes"
#define Msg_Edit_UserImageTitle_STR "Image"
#define Msg_Edit_UserFirst_STR "_First"
#define Msg_Edit_UserLast_STR "Las_t"
#define Msg_Edit_UserVisualName_STR "S_how as"
#define Msg_Edit_UserEMail_STR "_EMail"
#define Msg_Edit_UserEMailAdd_STR "_Add"
#define Msg_Edit_UserEMailRem_STR "_Remove"
#define Msg_Edit_UserEMailUp_STR "Move u_p"
#define Msg_Edit_UserEMailDown_STR "Move _down"
#define Msg_Edit_UserAddress_STR "_Address"
#define Msg_Edit_UserCity_STR "Cit_y"
#define Msg_Edit_UserZIP_STR "_ZIP"
#define Msg_Edit_UserCountry_STR "C_ountry"
#define Msg_Edit_UserAlias_STR "A_lias"
#define Msg_Edit_UserPGP_STR "P_GP"
#define Msg_Edit_UserPhone_STR "P_hone"
#define Msg_Edit_UserMobile_STR "_Mobile"
#define Msg_Edit_UserFax_STR "Fa_x"
#define Msg_Edit_UserICQ_STR "IC_Q"
#define Msg_Edit_UserWWW_STR "_WWW"
#define Msg_Edit_UserFTP_STR "FT_P"
#define Msg_Edit_UserCompany_STR "Compa_ny"
#define Msg_Edit_UserJob_STR "_Job"
#define Msg_Edit_UserDept_STR "_Dept"
#define Msg_Edit_UserOffice_STR "O_ffice"
#define Msg_Edit_UserOffEMail_STR "EMa_il"
#define Msg_Edit_UserOffAddress_STR "_Address"
#define Msg_Edit_UserOffCity_STR "Cit_y"
#define Msg_Edit_UserOffZIP_STR "_ZIP"
#define Msg_Edit_UserOffCountry_STR "C_ountry"
#define Msg_Edit_UserOffPhone_STR "P_hone"
#define Msg_Edit_UserOffMobile_STR "_Mobile"
#define Msg_Edit_UserOffFax_STR "Fa_x"
#define Msg_Edit_UserImage_STR "_Image"
#define Msg_Edit_UserImageTransparent_STR "_Tran"
#define Msg_Edit_UserSex_STR "_Sex"
#define Msg_Edit_UserSex_None_STR "---"
#define Msg_Edit_UserSex_Male_STR "Male"
#define Msg_Edit_UserSex_Female_STR "Female"
#define Msg_Edit_UserBirthday_STR "_Birthday"
#define Msg_Edit_UserAnniversary_STR "A_nniversary"
#define Msg_Edit_UserGetMap_STR "_GetMap"
#define Msg_Edit_WWWWinTitle_STR "Edit WWW site"
#define Msg_Edit_WWWMenuTitle_STR "WWW actions"
#define Msg_Edit_WWWName_STR "_Name"
#define Msg_Edit_WWWHost_STR "S_ite"
#define Msg_Edit_WWWAlias_STR "_Alias"
#define Msg_Edit_WWWMaster_STR "_Master"
#define Msg_Edit_WWWEMail_STR "_EMail"
#define Msg_Edit_WWWComment_STR "C_omment"
#define Msg_Edit_FTPWinTitle_STR "Edit FTP site"
#define Msg_Edit_FTPMenuTitle_STR "FTP actions"
#define Msg_Edit_FTPSettingsTitle_STR "Settings"
#define Msg_Edit_FTPAdvancedTitle_STR "Advanced"
#define Msg_Edit_FTPName_STR "_Name"
#define Msg_Edit_FTPHost_STR "S_ite"
#define Msg_Edit_FTPAlias_STR "_Alias"
#define Msg_Edit_FTPEMail_STR "_EMail"
#define Msg_Edit_FTPLocal_STR "_Drawer"
#define Msg_Edit_FTPLocalUse_STR "U_se"
#define Msg_Edit_FTPComment_STR "C_omment"
#define Msg_Edit_FTPAdvanced_STR "_Activate advanced options"
#define Msg_Edit_FTPUser_STR "U_ser"
#define Msg_Edit_FTPPass_STR "_Password"
#define Msg_Edit_FTPPort_STR "P_ort"
#define Msg_Edit_FTPRetries_STR "_Retries"
#define Msg_Edit_FTPWinType_STR "_Window"
#define Msg_Edit_FTPWinTypeDual_STR "Dual list"
#define Msg_Edit_FTPWinTypeSingle_STR "Single list"
#define Msg_Edit_FTPADT_STR "A_DT"
#define Msg_Edit_FTPAnon_STR "A_nonymous"
#define Msg_Edit_FTPQuiet_STR "_Quiet"
#define Msg_Edit_FTPCompression_STR "Co_mpression"
#define Msg_Edit_FTPOutput_STR "Ou_tput"
#define Msg_Edit_FTPHidden_STR "_Hiddens"
#define Msg_Edit_FTPNOOPs_STR "S_end NOOPs"
#define Msg_Edit_FTPUseProxy_STR "Pro_xy"
#define Msg_Edit_ChatWinTitle_STR "Edit Chat channel"
#define Msg_Edit_ChatMenuTitle_STR "Chat actions"
#define Msg_Edit_ChatChannel_STR "Cha_nnel"
#define Msg_Edit_ChatPass_STR "Passw_ord"
#define Msg_Edit_ChatServer_STR "Ser_ver"
#define Msg_Edit_ChatNick_STR "Nic_k"
#define Msg_Edit_ChatMantainer_STR "Manta_iner"
#define Msg_Edit_ChatWWW_STR "_WWW"
#define Msg_Edit_ChatEMail_STR "EMai_l"
#define Msg_Edit_ChatComment_STR "Commen_t"
#define Msg_Edit_TelnetWinTitle_STR "Edit telnet site"
#define Msg_Edit_TelnetMenuTitle_STR "Telnet actions"
#define Msg_Edit_TelnetHost_STR "Hos_t"
#define Msg_Edit_TelnetPort_STR "P_ort"
#define Msg_Edit_TelnetSSH_STR "_SSH"
#define Msg_Edit_TelnetLogin_STR "Lo_gin"
#define Msg_Edit_TelnetPass_STR "Pass_word"
#define Msg_Edit_TelnetEMail_STR "EMa_il"
#define Msg_Edit_TelnetComment_STR "Co_mment"
#define Msg_Edit_TelnetAlias_STR "_Alias"
#define Msg_Dial_WinTitle_STR "Dial"
#define Msg_Dial_Cancel_STR "_Cancel"
#define Msg_Dial_Dialling_STR "Dialling %s...\n(attempt %ld/%ld)"
#define Msg_Dial_RetryCancelGads_STR "*_Retry|_Cancel"
#define Msg_Dial_ModemNotResponding_STR "No response from the modem.\nHave you turned it on?"
#define Msg_Dial_Waiting_STR "Waiting... (%ld)"
#define Msg_Dial_NoDevice_STR "Cannot open unit %ld of %s!"
#define Msg_MiniMailWin_Title_STR "Quick EMail window"
#define Msg_About_WinTitle_STR "About Contact Manager"
#define Msg_About_Descr_STR "Contact Manager is a global bookmarks manager,\ndestributed as a Shareware product.\nAll Rights Are Reserved."
#define Msg_About_Info_STR "Information"
#define Msg_About_Version_STR "Versions:"
#define Msg_About_Authors_STR "Authors:"
#define Msg_About_Support_STR "Support:"
#define Msg_About_Database_STR "Database:"
#define Msg_About_ThirdParts_STR "Third parts"
#define Msg_About_Translation_STR ""
#define Msg_About_Translator_STR ""
#define Msg_About_TranslatorURL_STR ""
#define Msg_About_OfCourse_STR ", of course!"
#define Msg_About_NListtree_STR "NListtree class by"
#define Msg_About_NList_STR "NList classes by"
#define Msg_About_Popplaceholder_STR "Popplaceholder class by"
#define Msg_About_Textinput_STR "Textinput classes by"
#define Msg_About_PopupMenu_STR "popupmenu.library by"
#define Msg_About_GroupsImages_STR "Groups images by"
#define Msg_About_Lamp_STR "Lamp class by"
#define Msg_DelWarning_Never_STR "Never"
#define Msg_DelWarning_Always_STR "Always"
#define Msg_DelWarning_Entry_STR "More than 1 entry"
#define Msg_DelWarning_Entries_STR "More than %ld entries"
#define Msg_Card_Title_STR "\33uCard for %s\n"
#define Msg_Card_Contact_STR "Contact"
#define Msg_Card_Phone_STR "Phone"
#define Msg_Card_Home_STR "Home"
#define Msg_Card_Work_STR "Work"
#define Msg_Card_Note_STR "Note"
#define Msg_Card_GetMap_STR "GetMap"
#define Msg_Card_Name_STR "Name"
#define Msg_Card_Alias_STR "Alias"
#define Msg_Card_EMail_STR "EMail"
#define Msg_Card_Phone_Home_STR "Phone"
#define Msg_Card_Phone_Fax_STR "Fax"
#define Msg_Card_Phone_Mobile_STR "Cell"
#define Msg_Card_Phone_Work_STR "Work"
#define Msg_Card_Phone_WorkFax_STR "Work fax"
#define Msg_Card_Phone_WorkMobile_STR "Work cell"
#define Msg_Card_Channel_STR "Channel"
#define Msg_Card_Server_STR "Server"
#define Msg_Card_Host_STR "Host"

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG   cca_ID;
    STRPTR cca_Str;
};

static const struct CatCompArrayType CatCompArray[] =
{
    {Msg_GetDBDir,(STRPTR)Msg_GetDBDir_STR},
    {Msg_NOMUINoDirWarningMsg,(STRPTR)Msg_NOMUINoDirWarningMsg_STR},
    {Msg_NOMUINoDirWarningGads,(STRPTR)Msg_NOMUINoDirWarningGads_STR},
    {MSG_ErrReqTitle,(STRPTR)MSG_ErrReqTitle_STR},
    {MSG_NOMUIErrReqGadget,(STRPTR)MSG_NOMUIErrReqGadget_STR},
    {Msg_Error_NoMem,(STRPTR)Msg_Error_NoMem_STR},
    {Msg_Error_NoApp,(STRPTR)Msg_Error_NoApp_STR},
    {Msg_Error_NoWin,(STRPTR)Msg_Error_NoWin_STR},
    {Msg_Error_CantLogin,(STRPTR)Msg_Error_CantLogin_STR},
    {Msg_App_Copyright,(STRPTR)Msg_App_Copyright_STR},
    {Msg_App_Description,(STRPTR)Msg_App_Description_STR},
    {Msg_Menu_Project,(STRPTR)Msg_Menu_Project_STR},
    {Msg_Menu_About,(STRPTR)Msg_Menu_About_STR},
    {Msg_Menu_AboutMUI,(STRPTR)Msg_Menu_AboutMUI_STR},
    {Msg_Menu_Hide,(STRPTR)Msg_Menu_Hide_STR},
    {Msg_Menu_Quit,(STRPTR)Msg_Menu_Quit_STR},
    {Msg_Menu_SaveAndQuit,(STRPTR)Msg_Menu_SaveAndQuit_STR},
    {Msg_Menu_ForceQuit,(STRPTR)Msg_Menu_ForceQuit_STR},
    {Msg_Menu_Database,(STRPTR)Msg_Menu_Database_STR},
    {Msg_Menu_User,(STRPTR)Msg_Menu_User_STR},
    {Msg_Menu_Open,(STRPTR)Msg_Menu_Open_STR},
    {Msg_Menu_Save,(STRPTR)Msg_Menu_Save_STR},
    {Msg_Menu_SaveAs,(STRPTR)Msg_Menu_SaveAs_STR},
    {Msg_Menu_LastSaved,(STRPTR)Msg_Menu_LastSaved_STR},
    {Msg_Menu_Add,(STRPTR)Msg_Menu_Add_STR},
    {Msg_Menu_Clone,(STRPTR)Msg_Menu_Clone_STR},
    {Msg_Menu_Edit,(STRPTR)Msg_Menu_Edit_STR},
    {Msg_Menu_Remove,(STRPTR)Msg_Menu_Remove_STR},
    {Msg_Menu_SelectAll,(STRPTR)Msg_Menu_SelectAll_STR},
    {Msg_Menu_SelectNone,(STRPTR)Msg_Menu_SelectNone_STR},
    {Msg_Menu_SelectToggle,(STRPTR)Msg_Menu_SelectToggle_STR},
    {Msg_Menu_Search,(STRPTR)Msg_Menu_Search_STR},
    {Msg_Menu_ScanHotlists,(STRPTR)Msg_Menu_ScanHotlists_STR},
    {Msg_Menu_Import,(STRPTR)Msg_Menu_Import_STR},
    {Msg_Menu_ImportYAM,(STRPTR)Msg_Menu_ImportYAM_STR},
    {Msg_Menu_ImportMD2,(STRPTR)Msg_Menu_ImportMD2_STR},
    {Msg_Menu_ImportSTRICQ,(STRPTR)Msg_Menu_ImportSTRICQ_STR},
    {Msg_Menu_ImportDFA,(STRPTR)Msg_Menu_ImportDFA_STR},
    {Msg_Menu_ImportSTFax,(STRPTR)Msg_Menu_ImportSTFax_STR},
    {Msg_Menu_ImportPINT,(STRPTR)Msg_Menu_ImportPINT_STR},
    {Msg_Menu_ImportGoodNews,(STRPTR)Msg_Menu_ImportGoodNews_STR},
    {Msg_Menu_ImportCSV,(STRPTR)Msg_Menu_ImportCSV_STR},
    {Msg_Menu_ImportCSVFullOE,(STRPTR)Msg_Menu_ImportCSVFullOE_STR},
    {Msg_Menu_ImportThor,(STRPTR)Msg_Menu_ImportThor_STR},
    {Msg_Menu_ImportAWeb,(STRPTR)Msg_Menu_ImportAWeb_STR},
    {Msg_Menu_ImportV,(STRPTR)Msg_Menu_ImportV_STR},
    {Msg_Menu_ImportIB,(STRPTR)Msg_Menu_ImportIB_STR},
    {Msg_Menu_ImportAmFTP,(STRPTR)Msg_Menu_ImportAmFTP_STR},
    {Msg_Menu_ImportDOpus,(STRPTR)Msg_Menu_ImportDOpus_STR},
    {Msg_Menu_ImportAmIRC,(STRPTR)Msg_Menu_ImportAmIRC_STR},
    {Msg_Menu_ImportNetscape,(STRPTR)Msg_Menu_ImportNetscape_STR},
    {Msg_Menu_Export,(STRPTR)Msg_Menu_Export_STR},
    {Msg_Menu_ExportCSV,(STRPTR)Msg_Menu_ExportCSV_STR},
    {Msg_Menu_ExportCSVFullOE,(STRPTR)Msg_Menu_ExportCSVFullOE_STR},
    {Msg_Menu_ExportURLHtml,(STRPTR)Msg_Menu_ExportURLHtml_STR},
    {Msg_Menu_ExportWWWHtml,(STRPTR)Msg_Menu_ExportWWWHtml_STR},
    {Msg_Menu_ExportFTPHtml,(STRPTR)Msg_Menu_ExportFTPHtml_STR},
    {Msg_Menu_ExportYAM,(STRPTR)Msg_Menu_ExportYAM_STR},
    {Msg_Menu_ExportAWeb,(STRPTR)Msg_Menu_ExportAWeb_STR},
    {Msg_Menu_ExportIBrowse,(STRPTR)Msg_Menu_ExportIBrowse_STR},
    {Msg_Menu_ExportNetscape,(STRPTR)Msg_Menu_ExportNetscape_STR},
    {Msg_Menu_Settings,(STRPTR)Msg_Menu_Settings_STR},
    {Msg_Menu_EditPrefs,(STRPTR)Msg_Menu_EditPrefs_STR},
    {Msg_Menu_MUI,(STRPTR)Msg_Menu_MUI_STR},
    {Msg_Menu_SavePrefs,(STRPTR)Msg_Menu_SavePrefs_STR},
    {Msg_Menu_LastSavedPrefs,(STRPTR)Msg_Menu_LastSavedPrefs_STR},
    {Msg_Menu_NoBar,(STRPTR)Msg_Menu_NoBar_STR},
    {Msg_Menu_NoGroups,(STRPTR)Msg_Menu_NoGroups_STR},
    {Msg_Menu_NoQuick,(STRPTR)Msg_Menu_NoQuick_STR},
    {Msg_Menu_NoGroupsCtrl,(STRPTR)Msg_Menu_NoGroupsCtrl_STR},
    {Msg_Menu_NoCard,(STRPTR)Msg_Menu_NoCard_STR},
    {Msg_Menu_ImportSimpleMail,(STRPTR)Msg_Menu_ImportSimpleMail_STR},
    {Msg_Menu_ExportSimpleMail,(STRPTR)Msg_Menu_ExportSimpleMail_STR},
    {Msg_Menu_ImportATC,(STRPTR)Msg_Menu_ImportATC_STR},
    {Msg_Menu_ExportATC,(STRPTR)Msg_Menu_ExportATC_STR},
    {Msg_Req_Title,(STRPTR)Msg_Req_Title_STR},
    {Msg_Req_HotChangedMsg,(STRPTR)Msg_Req_HotChangedMsg_STR},
    {Msg_Req_HotChangedGads,(STRPTR)Msg_Req_HotChangedGads_STR},
    {Msg_Req_NotSavedMsg,(STRPTR)Msg_Req_NotSavedMsg_STR},
    {Msg_Req_NotSavedGads,(STRPTR)Msg_Req_NotSavedGads_STR},
    {Msg_Req_ChangedMsg,(STRPTR)Msg_Req_ChangedMsg_STR},
    {Msg_Req_ChangedGads,(STRPTR)Msg_Req_ChangedGads_STR},
    {Msg_Req_Asl,(STRPTR)Msg_Req_Asl_STR},
    {Msg_Req_AslGads,(STRPTR)Msg_Req_AslGads_STR},
    {Msg_Req_DelWarningOne,(STRPTR)Msg_Req_DelWarningOne_STR},
    {Msg_Req_DelWarningMany,(STRPTR)Msg_Req_DelWarningMany_STR},
    {Msg_Req_DelWarningGads,(STRPTR)Msg_Req_DelWarningGads_STR},
    {Msg_Req_GroupDelWarningOne,(STRPTR)Msg_Req_GroupDelWarningOne_STR},
    {Msg_Req_GroupDelWarningMany,(STRPTR)Msg_Req_GroupDelWarningMany_STR},
    {Msg_Req_GroupDelWarningGads,(STRPTR)Msg_Req_GroupDelWarningGads_STR},
    {Msg_Win_Title,(STRPTR)Msg_Win_Title_STR},
    {Msg_Gad_Add,(STRPTR)Msg_Gad_Add_STR},
    {Msg_Gad_AddHelp,(STRPTR)Msg_Gad_AddHelp_STR},
    {Msg_Gad_Clone,(STRPTR)Msg_Gad_Clone_STR},
    {Msg_Gad_CloneHelp,(STRPTR)Msg_Gad_CloneHelp_STR},
    {Msg_Gad_Edit,(STRPTR)Msg_Gad_Edit_STR},
    {Msg_Gad_EditHelp,(STRPTR)Msg_Gad_EditHelp_STR},
    {Msg_Gad_Remove,(STRPTR)Msg_Gad_Remove_STR},
    {Msg_Gad_RemoveHelp,(STRPTR)Msg_Gad_RemoveHelp_STR},
    {Msg_Gad_Search,(STRPTR)Msg_Gad_Search_STR},
    {Msg_Gad_SearchHelp,(STRPTR)Msg_Gad_SearchHelp_STR},
    {Msg_Gad_Save,(STRPTR)Msg_Gad_Save_STR},
    {Msg_Gad_SaveHelp,(STRPTR)Msg_Gad_SaveHelp_STR},
    {Msg_Gad_Prefs,(STRPTR)Msg_Gad_Prefs_STR},
    {Msg_Gad_PrefsHelp,(STRPTR)Msg_Gad_PrefsHelp_STR},
    {Msg_Gad_Login,(STRPTR)Msg_Gad_Login_STR},
    {Msg_Gad_LoginHelp,(STRPTR)Msg_Gad_LoginHelp_STR},
    {Msg_Gad_GroupName,(STRPTR)Msg_Gad_GroupName_STR},
    {Msg_Gad_GroupNameHelp,(STRPTR)Msg_Gad_GroupNameHelp_STR},
    {Msg_Gad_AddGroup,(STRPTR)Msg_Gad_AddGroup_STR},
    {Msg_Gad_AddGroupHelp,(STRPTR)Msg_Gad_AddGroupHelp_STR},
    {Msg_Gad_RemoveGroup,(STRPTR)Msg_Gad_RemoveGroup_STR},
    {Msg_Gad_RemoveGroupHelp,(STRPTR)Msg_Gad_RemoveGroupHelp_STR},
    {Msg_Gad_GroupUpHelp,(STRPTR)Msg_Gad_GroupUpHelp_STR},
    {Msg_Gad_GroupDownHelp,(STRPTR)Msg_Gad_GroupDownHelp_STR},
    {Msg_Groups_Menu_Title,(STRPTR)Msg_Groups_Menu_Title_STR},
    {Msg_Groups_Menu_OpenThis,(STRPTR)Msg_Groups_Menu_OpenThis_STR},
    {Msg_Groups_Menu_OpenAll,(STRPTR)Msg_Groups_Menu_OpenAll_STR},
    {Msg_Groups_Menu_CloseThis,(STRPTR)Msg_Groups_Menu_CloseThis_STR},
    {Msg_Groups_Menu_CloseAll,(STRPTR)Msg_Groups_Menu_CloseAll_STR},
    {Msg_Groups_Menu_Sort,(STRPTR)Msg_Groups_Menu_Sort_STR},
    {Msg_Gad_Locate,(STRPTR)Msg_Gad_Locate_STR},
    {Msg_Gad_LocateHelp,(STRPTR)Msg_Gad_LocateHelp_STR},
    {Msg_Entries,(STRPTR)Msg_Entries_STR},
    {Msg_Gad_Sort,(STRPTR)Msg_Gad_Sort_STR},
    {Msg_Gad_SortHelp,(STRPTR)Msg_Gad_SortHelp_STR},
    {Msg_Users,(STRPTR)Msg_Users_STR},
    {Msg_WWW,(STRPTR)Msg_WWW_STR},
    {Msg_FTP,(STRPTR)Msg_FTP_STR},
    {Msg_Chat,(STRPTR)Msg_Chat_STR},
    {Msg_Telnet,(STRPTR)Msg_Telnet_STR},
    {Msg_Title_Name,(STRPTR)Msg_Title_Name_STR},
    {Msg_Title_LastName,(STRPTR)Msg_Title_LastName_STR},
    {Msg_Title_EMail,(STRPTR)Msg_Title_EMail_STR},
    {Msg_Title_Comment,(STRPTR)Msg_Title_Comment_STR},
    {Msg_Title_Alias,(STRPTR)Msg_Title_Alias_STR},
    {Msg_Title_Birthday,(STRPTR)Msg_Title_Birthday_STR},
    {Msg_Title_Anniversary,(STRPTR)Msg_Title_Anniversary_STR},
    {Msg_Title_Country,(STRPTR)Msg_Title_Country_STR},
    {Msg_Title_City,(STRPTR)Msg_Title_City_STR},
    {Msg_Title_Phone,(STRPTR)Msg_Title_Phone_STR},
    {Msg_Title_Mobile,(STRPTR)Msg_Title_Mobile_STR},
    {Msg_Title_Fax,(STRPTR)Msg_Title_Fax_STR},
    {Msg_Title_WWW,(STRPTR)Msg_Title_WWW_STR},
    {Msg_Title_Company,(STRPTR)Msg_Title_Company_STR},
    {Msg_Title_BPhone,(STRPTR)Msg_Title_BPhone_STR},
    {Msg_Title_BMobile,(STRPTR)Msg_Title_BMobile_STR},
    {Msg_Title_BFax,(STRPTR)Msg_Title_BFax_STR},
    {Msg_Title_Site,(STRPTR)Msg_Title_Site_STR},
    {Msg_Title_LastVisited,(STRPTR)Msg_Title_LastVisited_STR},
    {Msg_Title_LastModified,(STRPTR)Msg_Title_LastModified_STR},
    {Msg_Title_Channel,(STRPTR)Msg_Title_Channel_STR},
    {Msg_Title_Server,(STRPTR)Msg_Title_Server_STR},
    {Msg_Title_Host,(STRPTR)Msg_Title_Host_STR},
    {Msg_Title_Port,(STRPTR)Msg_Title_Port_STR},
    {Msg_Never,(STRPTR)Msg_Never_STR},
    {Msg_Quick_UserFirst,(STRPTR)Msg_Quick_UserFirst_STR},
    {Msg_Quick_UserLast,(STRPTR)Msg_Quick_UserLast_STR},
    {Msg_Quick_UserEMail,(STRPTR)Msg_Quick_UserEMail_STR},
    {Msg_Quick_WWWName,(STRPTR)Msg_Quick_WWWName_STR},
    {Msg_Quick_WWWHost,(STRPTR)Msg_Quick_WWWHost_STR},
    {Msg_Quick_FTPName,(STRPTR)Msg_Quick_FTPName_STR},
    {Msg_Quick_FTPHost,(STRPTR)Msg_Quick_FTPHost_STR},
    {Msg_Quick_ChatChannel,(STRPTR)Msg_Quick_ChatChannel_STR},
    {Msg_Quick_ChatServer,(STRPTR)Msg_Quick_ChatServer_STR},
    {Msg_Quick_TelnetHost,(STRPTR)Msg_Quick_TelnetHost_STR},
    {Msg_Quick_TelnetPort,(STRPTR)Msg_Quick_TelnetPort_STR},
    {Msg_Quick_TelnetLogin,(STRPTR)Msg_Quick_TelnetLogin_STR},
    {Msg_Quick_TelnetPass,(STRPTR)Msg_Quick_TelnetPass_STR},
    {Msg_SearchWin_Title,(STRPTR)Msg_SearchWin_Title_STR},
    {Msg_SearchWin_Pattern,(STRPTR)Msg_SearchWin_Pattern_STR},
    {Msg_SearchWin_PatternHelp,(STRPTR)Msg_SearchWin_PatternHelp_STR},
    {Msg_SearchWin_SearchInTitle,(STRPTR)Msg_SearchWin_SearchInTitle_STR},
    {Msg_SearchWin_Users,(STRPTR)Msg_SearchWin_Users_STR},
    {Msg_SearchWin_UsersHelp,(STRPTR)Msg_SearchWin_UsersHelp_STR},
    {Msg_SearchWin_WWW,(STRPTR)Msg_SearchWin_WWW_STR},
    {Msg_SearchWin_WWWHelp,(STRPTR)Msg_SearchWin_WWWHelp_STR},
    {Msg_SearchWin_FTP,(STRPTR)Msg_SearchWin_FTP_STR},
    {Msg_SearchWin_FTPHelp,(STRPTR)Msg_SearchWin_FTPHelp_STR},
    {Msg_SearchWin_Chat,(STRPTR)Msg_SearchWin_Chat_STR},
    {Msg_SearchWin_ChatHelp,(STRPTR)Msg_SearchWin_ChatHelp_STR},
    {Msg_SearchWin_Telnet,(STRPTR)Msg_SearchWin_Telnet_STR},
    {Msg_SearchWin_TelnetHelp,(STRPTR)Msg_SearchWin_TelnetHelp_STR},
    {Msg_SearchWin_OptionsTitle,(STRPTR)Msg_SearchWin_OptionsTitle_STR},
    {Msg_SearchWin_Comment,(STRPTR)Msg_SearchWin_Comment_STR},
    {Msg_SearchWin_CommentHelp,(STRPTR)Msg_SearchWin_CommentHelp_STR},
    {Msg_SearchWin_Case,(STRPTR)Msg_SearchWin_Case_STR},
    {Msg_SearchWin_CaseHelp,(STRPTR)Msg_SearchWin_CaseHelp_STR},
    {Msg_SearchWin_Wildcards,(STRPTR)Msg_SearchWin_Wildcards_STR},
    {Msg_SearchWin_WildcardsHelp,(STRPTR)Msg_SearchWin_WildcardsHelp_STR},
    {Msg_SearchWin_Search,(STRPTR)Msg_SearchWin_Search_STR},
    {Msg_SearchWin_SearchHelp,(STRPTR)Msg_SearchWin_SearchHelp_STR},
    {Msg_SearchWin_First,(STRPTR)Msg_SearchWin_First_STR},
    {Msg_SearchWin_FirstHelp,(STRPTR)Msg_SearchWin_FirstHelp_STR},
    {Msg_SearchWin_Previous,(STRPTR)Msg_SearchWin_Previous_STR},
    {Msg_SearchWin_PreviousHelp,(STRPTR)Msg_SearchWin_PreviousHelp_STR},
    {Msg_SearchWin_Cancel,(STRPTR)Msg_SearchWin_Cancel_STR},
    {Msg_SearchWin_CancelHelp,(STRPTR)Msg_SearchWin_CancelHelp_STR},
    {Msg_SearchWin_Searching,(STRPTR)Msg_SearchWin_Searching_STR},
    {Msg_SearchWin_Welcome,(STRPTR)Msg_SearchWin_Welcome_STR},
    {Msg_SearchWin_Found,(STRPTR)Msg_SearchWin_Found_STR},
    {Msg_SearchWin_NotFound,(STRPTR)Msg_SearchWin_NotFound_STR},
    {Msg_UserWin_Title,(STRPTR)Msg_UserWin_Title_STR},
    {Msg_UserWin_User,(STRPTR)Msg_UserWin_User_STR},
    {Msg_UserWin_UserHelp,(STRPTR)Msg_UserWin_UserHelp_STR},
    {Msg_PrefsWin_Title,(STRPTR)Msg_PrefsWin_Title_STR},
    {Msg_PrefsWin_TitleSettings,(STRPTR)Msg_PrefsWin_TitleSettings_STR},
    {Msg_PrefsWin_TitleScripts,(STRPTR)Msg_PrefsWin_TitleScripts_STR},
    {Msg_PrefsWin_TitleHotlists,(STRPTR)Msg_PrefsWin_TitleHotlists_STR},
    {Msg_PrefsWin_TitleDialer,(STRPTR)Msg_PrefsWin_TitleDialer_STR},
    {Msg_PrefsWin_Save,(STRPTR)Msg_PrefsWin_Save_STR},
    {Msg_PrefsWin_SaveHelp,(STRPTR)Msg_PrefsWin_SaveHelp_STR},
    {Msg_PrefsWin_Use,(STRPTR)Msg_PrefsWin_Use_STR},
    {Msg_PrefsWin_UseHelp,(STRPTR)Msg_PrefsWin_UseHelp_STR},
    {Msg_PrefsWin_Apply,(STRPTR)Msg_PrefsWin_Apply_STR},
    {Msg_PrefsWin_ApplyHelp,(STRPTR)Msg_PrefsWin_ApplyHelp_STR},
    {Msg_PrefsWin_Cancel,(STRPTR)Msg_PrefsWin_Cancel_STR},
    {Msg_PrefsWin_CancelHelp,(STRPTR)Msg_PrefsWin_CancelHelp_STR},
    {Msg_PrefsWin_Settings_Locale,(STRPTR)Msg_PrefsWin_Settings_Locale_STR},
    {Msg_PrefsWin_Settings_Country,(STRPTR)Msg_PrefsWin_Settings_Country_STR},
    {Msg_PrefsWin_Settings_CountryHelp,(STRPTR)Msg_PrefsWin_Settings_CountryHelp_STR},
    {Msg_PrefsWin_Settings_CountryCode,(STRPTR)Msg_PrefsWin_Settings_CountryCode_STR},
    {Msg_PrefsWin_Settings_CountryCodeHelp,(STRPTR)Msg_PrefsWin_Settings_CountryCodeHelp_STR},
    {Msg_PrefsWin_Settings_GetFromLocale,(STRPTR)Msg_PrefsWin_Settings_GetFromLocale_STR},
    {Msg_PrefsWin_Settings_GetFromLocaleHelp,(STRPTR)Msg_PrefsWin_Settings_GetFromLocaleHelp_STR},
    {Msg_PrefsWin_Settings_Options,(STRPTR)Msg_PrefsWin_Settings_Options_STR},
    {Msg_PrefsWin_Settings_DelWarning,(STRPTR)Msg_PrefsWin_Settings_DelWarning_STR},
    {Msg_PrefsWin_Settings_DelWarningHelp,(STRPTR)Msg_PrefsWin_Settings_DelWarningHelp_STR},
    {Msg_PrefsWin_Settings_DoubleClick,(STRPTR)Msg_PrefsWin_Settings_DoubleClick_STR},
    {Msg_PrefsWin_Settings_DoubleClickHelp,(STRPTR)Msg_PrefsWin_Settings_DoubleClickHelp_STR},
    {Msg_PrefsWin_Settings_DCEdit,(STRPTR)Msg_PrefsWin_Settings_DCEdit_STR},
    {Msg_PrefsWin_Settings_DCScript,(STRPTR)Msg_PrefsWin_Settings_DCScript_STR},
    {Msg_PrefsWin_Settings_DCScriptQuit,(STRPTR)Msg_PrefsWin_Settings_DCScriptQuit_STR},
    {Msg_PrefsWin_Settings_DCScriptHide,(STRPTR)Msg_PrefsWin_Settings_DCScriptHide_STR},
    {Msg_PrefsWin_Settings_SilentScan,(STRPTR)Msg_PrefsWin_Settings_SilentScan_STR},
    {Msg_PrefsWin_Settings_SilentScanHelp,(STRPTR)Msg_PrefsWin_Settings_SilentScanHelp_STR},
    {Msg_PrefsWin_Settings_UseOpenURL,(STRPTR)Msg_PrefsWin_Settings_UseOpenURL_STR},
    {Msg_PrefsWin_Settings_UseOpenURLHelp,(STRPTR)Msg_PrefsWin_Settings_UseOpenURLHelp_STR},
    {Msg_PrefsWin_Settings_BoldSections,(STRPTR)Msg_PrefsWin_Settings_BoldSections_STR},
    {Msg_PrefsWin_Settings_BoldSectionsHelp,(STRPTR)Msg_PrefsWin_Settings_BoldSectionsHelp_STR},
    {Msg_PrefsWin_Settings_BoldTitles,(STRPTR)Msg_PrefsWin_Settings_BoldTitles_STR},
    {Msg_PrefsWin_Settings_BoldTitlesHelp,(STRPTR)Msg_PrefsWin_Settings_BoldTitlesHelp_STR},
    {Msg_PrefsWin_Settings_AutoReload,(STRPTR)Msg_PrefsWin_Settings_AutoReload_STR},
    {Msg_PrefsWin_Settings_AutoReloadHelp,(STRPTR)Msg_PrefsWin_Settings_AutoReloadHelp_STR},
    {Msg_PrefsWin_Settings_SaveOnExit,(STRPTR)Msg_PrefsWin_Settings_SaveOnExit_STR},
    {Msg_PrefsWin_Settings_SaveOnExitHelp,(STRPTR)Msg_PrefsWin_Settings_SaveOnExitHelp_STR},
    {Msg_PrefsWin_Settings_SilentExit,(STRPTR)Msg_PrefsWin_Settings_SilentExit_STR},
    {Msg_PrefsWin_Settings_SilentExitHelp,(STRPTR)Msg_PrefsWin_Settings_SilentExitHelp_STR},
    {Msg_PrefsWin_Settings_SaveOnChanges,(STRPTR)Msg_PrefsWin_Settings_SaveOnChanges_STR},
    {Msg_PrefsWin_Settings_SaveOnChangesHelp,(STRPTR)Msg_PrefsWin_Settings_SaveOnChangesHelp_STR},
    {Msg_PrefsWin_Settings_NoListBar,(STRPTR)Msg_PrefsWin_Settings_NoListBar_STR},
    {Msg_PrefsWin_Settings_NoListBarHelp,(STRPTR)Msg_PrefsWin_Settings_NoListBarHelp_STR},
    {Msg_PrefsWin_Settings_NtCheckChanges,(STRPTR)Msg_PrefsWin_Settings_NtCheckChanges_STR},
    {Msg_PrefsWin_Settings_NtCheckChangesHelp,(STRPTR)Msg_PrefsWin_Settings_NtCheckChangesHelp_STR},
    {Msg_PrefsWin_Settings_RightMouseActive,(STRPTR)Msg_PrefsWin_Settings_RightMouseActive_STR},
    {Msg_PrefsWin_Settings_RightMouseActiveHelp,(STRPTR)Msg_PrefsWin_Settings_RightMouseActiveHelp_STR},
    {Msg_PrefsWin_Settings_EditNew,(STRPTR)Msg_PrefsWin_Settings_EditNew_STR},
    {Msg_PrefsWin_Settings_EditNewHelp,(STRPTR)Msg_PrefsWin_Settings_EditNewHelp_STR},
    {Msg_PrefsWin_Settings_ButtonsBar,(STRPTR)Msg_PrefsWin_Settings_ButtonsBar_STR},
    {Msg_PrefsWin_Settings_BarPosition,(STRPTR)Msg_PrefsWin_Settings_BarPosition_STR},
    {Msg_PrefsWin_Settings_BarPositionHelp,(STRPTR)Msg_PrefsWin_Settings_BarPositionHelp_STR},
    {Msg_PrefsWin_Settings_BarPositionTop,(STRPTR)Msg_PrefsWin_Settings_BarPositionTop_STR},
    {Msg_PrefsWin_Settings_BarPositionBottom,(STRPTR)Msg_PrefsWin_Settings_BarPositionBottom_STR},
    {Msg_PrefsWin_Settings_BarPositionLeft,(STRPTR)Msg_PrefsWin_Settings_BarPositionLeft_STR},
    {Msg_PrefsWin_Settings_BarPositionRight,(STRPTR)Msg_PrefsWin_Settings_BarPositionRight_STR},
    {Msg_PrefsWin_Settings_BarPositionInnertTop,(STRPTR)Msg_PrefsWin_Settings_BarPositionInnertTop_STR},
    {Msg_PrefsWin_Settings_BarPositionInnerBottom,(STRPTR)Msg_PrefsWin_Settings_BarPositionInnerBottom_STR},
    {Msg_PrefsWin_Settings_ViewMode,(STRPTR)Msg_PrefsWin_Settings_ViewMode_STR},
    {Msg_PrefsWin_Settings_ViewModeHelp,(STRPTR)Msg_PrefsWin_Settings_ViewModeHelp_STR},
    {Msg_PrefsWin_Settings_TextGfx,(STRPTR)Msg_PrefsWin_Settings_TextGfx_STR},
    {Msg_PrefsWin_Settings_Gfx,(STRPTR)Msg_PrefsWin_Settings_Gfx_STR},
    {Msg_PrefsWin_Settings_Text,(STRPTR)Msg_PrefsWin_Settings_Text_STR},
    {Msg_PrefsWin_Settings_Borderless,(STRPTR)Msg_PrefsWin_Settings_Borderless_STR},
    {Msg_PrefsWin_Settings_BorderlessHelp,(STRPTR)Msg_PrefsWin_Settings_BorderlessHelp_STR},
    {Msg_PrefsWin_Settings_Sunny,(STRPTR)Msg_PrefsWin_Settings_Sunny_STR},
    {Msg_PrefsWin_Settings_SunnyHelp,(STRPTR)Msg_PrefsWin_Settings_SunnyHelp_STR},
    {Msg_PrefsWin_Settings_Raised,(STRPTR)Msg_PrefsWin_Settings_Raised_STR},
    {Msg_PrefsWin_Settings_RaisedHelp,(STRPTR)Msg_PrefsWin_Settings_RaisedHelp_STR},
    {Msg_PrefsWin_Settings_Scaled,(STRPTR)Msg_PrefsWin_Settings_Scaled_STR},
    {Msg_PrefsWin_Settings_ScaledHelp,(STRPTR)Msg_PrefsWin_Settings_ScaledHelp_STR},
    {Msg_PrefsWin_Settings_Underscore,(STRPTR)Msg_PrefsWin_Settings_Underscore_STR},
    {Msg_PrefsWin_Settings_UnderscoreHelp,(STRPTR)Msg_PrefsWin_Settings_UnderscoreHelp_STR},
    {Msg_PrefsWin_Settings_ButtonsPosition,(STRPTR)Msg_PrefsWin_Settings_ButtonsPosition_STR},
    {Msg_PrefsWin_Settings_ButtonsPositionHelp,(STRPTR)Msg_PrefsWin_Settings_ButtonsPositionHelp_STR},
    {Msg_PrefsWin_Settings_ButtonsPosition_Left,(STRPTR)Msg_PrefsWin_Settings_ButtonsPosition_Left_STR},
    {Msg_PrefsWin_Settings_ButtonsPosition_Center,(STRPTR)Msg_PrefsWin_Settings_ButtonsPosition_Center_STR},
    {Msg_PrefsWin_Settings_ButtonsPosition_Right,(STRPTR)Msg_PrefsWin_Settings_ButtonsPosition_Right_STR},
    {Msg_PrefsWin_Settings_LabelPosition,(STRPTR)Msg_PrefsWin_Settings_LabelPosition_STR},
    {Msg_PrefsWin_Settings_LabelPositionHelp,(STRPTR)Msg_PrefsWin_Settings_LabelPositionHelp_STR},
    {Msg_PrefsWin_Settings_LabelPosition_Bottom,(STRPTR)Msg_PrefsWin_Settings_LabelPosition_Bottom_STR},
    {Msg_PrefsWin_Settings_LabelPosition_Top,(STRPTR)Msg_PrefsWin_Settings_LabelPosition_Top_STR},
    {Msg_PrefsWin_Settings_LabelPosition_Right,(STRPTR)Msg_PrefsWin_Settings_LabelPosition_Right_STR},
    {Msg_PrefsWin_Settings_LabelPosition_Left,(STRPTR)Msg_PrefsWin_Settings_LabelPosition_Left_STR},
    {Msg_PrefsWin_Settings_BarSpacer,(STRPTR)Msg_PrefsWin_Settings_BarSpacer_STR},
    {Msg_PrefsWin_Settings_BarSpacerHelp,(STRPTR)Msg_PrefsWin_Settings_BarSpacerHelp_STR},
    {Msg_PrefsWin_Settings_Frame,(STRPTR)Msg_PrefsWin_Settings_Frame_STR},
    {Msg_PrefsWin_Settings_FrameHelp,(STRPTR)Msg_PrefsWin_Settings_FrameHelp_STR},
    {Msg_PrefsWin_Settings_DragBar,(STRPTR)Msg_PrefsWin_Settings_DragBar_STR},
    {Msg_PrefsWin_Settings_DragBarHelp,(STRPTR)Msg_PrefsWin_Settings_DragBarHelp_STR},
    {Msg_PrefsWin_Programs_UserScript,(STRPTR)Msg_PrefsWin_Programs_UserScript_STR},
    {Msg_PrefsWin_Programs_MailerTitle,(STRPTR)Msg_PrefsWin_Programs_MailerTitle_STR},
    {Msg_PrefsWin_Programs_Mailer,(STRPTR)Msg_PrefsWin_Programs_Mailer_STR},
    {Msg_PrefsWin_Programs_MailerHelp,(STRPTR)Msg_PrefsWin_Programs_MailerHelp_STR},
    {Msg_PrefsWin_Programs_MailerScript,(STRPTR)Msg_PrefsWin_Programs_MailerScript_STR},
    {Msg_PrefsWin_Programs_MailerScriptHelp,(STRPTR)Msg_PrefsWin_Programs_MailerScriptHelp_STR},
    {Msg_PrefsWin_Programs_MailerPath,(STRPTR)Msg_PrefsWin_Programs_MailerPath_STR},
    {Msg_PrefsWin_Programs_MailerPathHelp,(STRPTR)Msg_PrefsWin_Programs_MailerPathHelp_STR},
    {Msg_PrefsWin_Programs_WWWTitle,(STRPTR)Msg_PrefsWin_Programs_WWWTitle_STR},
    {Msg_PrefsWin_Programs_WWW,(STRPTR)Msg_PrefsWin_Programs_WWW_STR},
    {Msg_PrefsWin_Programs_WWWHelp,(STRPTR)Msg_PrefsWin_Programs_WWWHelp_STR},
    {Msg_PrefsWin_Programs_WWWScript,(STRPTR)Msg_PrefsWin_Programs_WWWScript_STR},
    {Msg_PrefsWin_Programs_WWWScriptHelp,(STRPTR)Msg_PrefsWin_Programs_WWWScriptHelp_STR},
    {Msg_PrefsWin_Programs_WWWPath,(STRPTR)Msg_PrefsWin_Programs_WWWPath_STR},
    {Msg_PrefsWin_Programs_WWWPathHelp,(STRPTR)Msg_PrefsWin_Programs_WWWPathHelp_STR},
    {Msg_PrefsWin_Programs_FTPTitle,(STRPTR)Msg_PrefsWin_Programs_FTPTitle_STR},
    {Msg_PrefsWin_Programs_FTP,(STRPTR)Msg_PrefsWin_Programs_FTP_STR},
    {Msg_PrefsWin_Programs_FTPHelp,(STRPTR)Msg_PrefsWin_Programs_FTPHelp_STR},
    {Msg_PrefsWin_Programs_FTPScript,(STRPTR)Msg_PrefsWin_Programs_FTPScript_STR},
    {Msg_PrefsWin_Programs_FTPScriptHelp,(STRPTR)Msg_PrefsWin_Programs_FTPScriptHelp_STR},
    {Msg_PrefsWin_Programs_FTPPath,(STRPTR)Msg_PrefsWin_Programs_FTPPath_STR},
    {Msg_PrefsWin_Programs_FTPPathHelp,(STRPTR)Msg_PrefsWin_Programs_FTPPathHelp_STR},
    {Msg_PrefsWin_Programs_FTPLocal,(STRPTR)Msg_PrefsWin_Programs_FTPLocal_STR},
    {Msg_PrefsWin_Programs_FTPLocalHelp,(STRPTR)Msg_PrefsWin_Programs_FTPLocalHelp_STR},
    {Msg_PrefsWin_Programs_FTPLocalUse,(STRPTR)Msg_PrefsWin_Programs_FTPLocalUse_STR},
    {Msg_PrefsWin_Programs_FTPLocalUseHelp,(STRPTR)Msg_PrefsWin_Programs_FTPLocalUseHelp_STR},
    {Msg_PrefsWin_Programs_ChatTitle,(STRPTR)Msg_PrefsWin_Programs_ChatTitle_STR},
    {Msg_PrefsWin_Programs_Chat,(STRPTR)Msg_PrefsWin_Programs_Chat_STR},
    {Msg_PrefsWin_Programs_ChatHelp,(STRPTR)Msg_PrefsWin_Programs_ChatHelp_STR},
    {Msg_PrefsWin_Programs_ChatScript,(STRPTR)Msg_PrefsWin_Programs_ChatScript_STR},
    {Msg_PrefsWin_Programs_ChatScriptHelp,(STRPTR)Msg_PrefsWin_Programs_ChatScriptHelp_STR},
    {Msg_PrefsWin_Programs_ChatPath,(STRPTR)Msg_PrefsWin_Programs_ChatPath_STR},
    {Msg_PrefsWin_Programs_ChatPathHelp,(STRPTR)Msg_PrefsWin_Programs_ChatPathHelp_STR},
    {Msg_PrefsWin_Programs_TelnetTitle,(STRPTR)Msg_PrefsWin_Programs_TelnetTitle_STR},
    {Msg_PrefsWin_Programs_Telnet,(STRPTR)Msg_PrefsWin_Programs_Telnet_STR},
    {Msg_PrefsWin_Programs_TelnetHelp,(STRPTR)Msg_PrefsWin_Programs_TelnetHelp_STR},
    {Msg_PrefsWin_Programs_TelnetScript,(STRPTR)Msg_PrefsWin_Programs_TelnetScript_STR},
    {Msg_PrefsWin_Programs_TelnetScriptHelp,(STRPTR)Msg_PrefsWin_Programs_TelnetScriptHelp_STR},
    {Msg_PrefsWin_Programs_TelnetPath,(STRPTR)Msg_PrefsWin_Programs_TelnetPath_STR},
    {Msg_PrefsWin_Programs_TelnetPathHelp,(STRPTR)Msg_PrefsWin_Programs_TelnetPathHelp_STR},
    {Msg_PrefsWin_Programs_FaxTitle,(STRPTR)Msg_PrefsWin_Programs_FaxTitle_STR},
    {Msg_PrefsWin_Programs_Fax,(STRPTR)Msg_PrefsWin_Programs_Fax_STR},
    {Msg_PrefsWin_Programs_FaxHelp,(STRPTR)Msg_PrefsWin_Programs_FaxHelp_STR},
    {Msg_PrefsWin_Programs_FaxScript,(STRPTR)Msg_PrefsWin_Programs_FaxScript_STR},
    {Msg_PrefsWin_Programs_FaxScriptHelp,(STRPTR)Msg_PrefsWin_Programs_FaxScriptHelp_STR},
    {Msg_PrefsWin_Programs_FaxPath,(STRPTR)Msg_PrefsWin_Programs_FaxPath_STR},
    {Msg_PrefsWin_Programs_FaxPathHelp,(STRPTR)Msg_PrefsWin_Programs_FaxPathHelp_STR},
    {Msg_PrefsWin_Programs_SavedProgTitle,(STRPTR)Msg_PrefsWin_Programs_SavedProgTitle_STR},
    {Msg_PrefsWin_Programs_SavedProg,(STRPTR)Msg_PrefsWin_Programs_SavedProg_STR},
    {Msg_PrefsWin_Programs_SavedProgHelp,(STRPTR)Msg_PrefsWin_Programs_SavedProgHelp_STR},
    {Msg_PrefsWin_Programs_SavedProgUse,(STRPTR)Msg_PrefsWin_Programs_SavedProgUse_STR},
    {Msg_PrefsWin_Programs_SavedProgUseHelp,(STRPTR)Msg_PrefsWin_Programs_SavedProgUseHelp_STR},
    {Msg_PrefsWin_Programs_SavedProgPopData,(STRPTR)Msg_PrefsWin_Programs_SavedProgPopData_STR},
    {Msg_PrefsWin_Dialer_Dialer,(STRPTR)Msg_PrefsWin_Dialer_Dialer_STR},
    {Msg_PrefsWin_Dialer_DialerHelp,(STRPTR)Msg_PrefsWin_Dialer_DialerHelp_STR},
    {Msg_PrefsWin_Dialer_DExternal,(STRPTR)Msg_PrefsWin_Dialer_DExternal_STR},
    {Msg_PrefsWin_Dialer_DInternal,(STRPTR)Msg_PrefsWin_Dialer_DInternal_STR},
    {Msg_PrefsWin_Dialer_Device,(STRPTR)Msg_PrefsWin_Dialer_Device_STR},
    {Msg_PrefsWin_Dialer_DeviceHelp,(STRPTR)Msg_PrefsWin_Dialer_DeviceHelp_STR},
    {Msg_PrefsWin_Dialer_Unit,(STRPTR)Msg_PrefsWin_Dialer_Unit_STR},
    {Msg_PrefsWin_Dialer_UnitHelp,(STRPTR)Msg_PrefsWin_Dialer_UnitHelp_STR},
    {Msg_PrefsWin_Dialer_Shared,(STRPTR)Msg_PrefsWin_Dialer_Shared_STR},
    {Msg_PrefsWin_Dialer_SharedHelp,(STRPTR)Msg_PrefsWin_Dialer_SharedHelp_STR},
    {Msg_PrefsWin_Dialer_ModemType,(STRPTR)Msg_PrefsWin_Dialer_ModemType_STR},
    {Msg_PrefsWin_Dialer_ModemTypeHelp,(STRPTR)Msg_PrefsWin_Dialer_ModemTypeHelp_STR},
    {Msg_PrefsWin_Dialer_ModemInit,(STRPTR)Msg_PrefsWin_Dialer_ModemInit_STR},
    {Msg_PrefsWin_Dialer_ModemInitHelp,(STRPTR)Msg_PrefsWin_Dialer_ModemInitHelp_STR},
    {Msg_PrefsWin_Dialer_DialPrefix,(STRPTR)Msg_PrefsWin_Dialer_DialPrefix_STR},
    {Msg_PrefsWin_Dialer_DialPrefixHelp,(STRPTR)Msg_PrefsWin_Dialer_DialPrefixHelp_STR},
    {Msg_PrefsWin_Dialer_DialSuffix,(STRPTR)Msg_PrefsWin_Dialer_DialSuffix_STR},
    {Msg_PrefsWin_Dialer_DialSuffixHelp,(STRPTR)Msg_PrefsWin_Dialer_DialSuffixHelp_STR},
    {Msg_PrefsWin_Dialer_RedialDelay,(STRPTR)Msg_PrefsWin_Dialer_RedialDelay_STR},
    {Msg_PrefsWin_Dialer_RedialDelayHelp,(STRPTR)Msg_PrefsWin_Dialer_RedialDelayHelp_STR},
    {Msg_PrefsWin_Dialer_RedialAttempts,(STRPTR)Msg_PrefsWin_Dialer_RedialAttempts_STR},
    {Msg_PrefsWin_Dialer_RedialAttemptsHelp,(STRPTR)Msg_PrefsWin_Dialer_RedialAttemptsHelp_STR},
    {Msg_PrefsWin_Dialer_Log,(STRPTR)Msg_PrefsWin_Dialer_Log_STR},
    {Msg_PrefsWin_Dialer_LogHelp,(STRPTR)Msg_PrefsWin_Dialer_LogHelp_STR},
    {Msg_PrefsWin_Dialer_Program,(STRPTR)Msg_PrefsWin_Dialer_Program_STR},
    {Msg_PrefsWin_Dialer_ProgramHelp,(STRPTR)Msg_PrefsWin_Dialer_ProgramHelp_STR},
    {Msg_PrefsWin_Dialer_ProgramPopNumber,(STRPTR)Msg_PrefsWin_Dialer_ProgramPopNumber_STR},
    {Msg_PrefsWin_Hotlists_V,(STRPTR)Msg_PrefsWin_Hotlists_V_STR},
    {Msg_PrefsWin_Hotlists_VHelp,(STRPTR)Msg_PrefsWin_Hotlists_VHelp_STR},
    {Msg_PrefsWin_Hotlists_IB,(STRPTR)Msg_PrefsWin_Hotlists_IB_STR},
    {Msg_PrefsWin_Hotlists_IBHelp,(STRPTR)Msg_PrefsWin_Hotlists_IBHelp_STR},
    {Msg_PrefsWin_Hotlists_AWeb,(STRPTR)Msg_PrefsWin_Hotlists_AWeb_STR},
    {Msg_PrefsWin_Hotlists_AWebHelp,(STRPTR)Msg_PrefsWin_Hotlists_AWebHelp_STR},
    {Msg_PrefsWin_Hotlists_MD2,(STRPTR)Msg_PrefsWin_Hotlists_MD2_STR},
    {Msg_PrefsWin_Hotlists_MD2Help,(STRPTR)Msg_PrefsWin_Hotlists_MD2Help_STR},
    {Msg_PrefsWin_Hotlists_YAM,(STRPTR)Msg_PrefsWin_Hotlists_YAM_STR},
    {Msg_PrefsWin_Hotlists_YAMHelp,(STRPTR)Msg_PrefsWin_Hotlists_YAMHelp_STR},
    {Msg_PrefsWin_Hotlists_STRICQ,(STRPTR)Msg_PrefsWin_Hotlists_STRICQ_STR},
    {Msg_PrefsWin_Hotlists_STRICQHelp,(STRPTR)Msg_PrefsWin_Hotlists_STRICQHelp_STR},
    {Msg_PrefsWin_Hotlists_GoodNews,(STRPTR)Msg_PrefsWin_Hotlists_GoodNews_STR},
    {Msg_PrefsWin_Hotlists_GoodNewsHelp,(STRPTR)Msg_PrefsWin_Hotlists_GoodNewsHelp_STR},
    {Msg_PrefsWin_Hotlists_PINT,(STRPTR)Msg_PrefsWin_Hotlists_PINT_STR},
    {Msg_PrefsWin_Hotlists_PINTHelp,(STRPTR)Msg_PrefsWin_Hotlists_PINTHelp_STR},
    {Msg_PrefsWin_Hotlists_DFA,(STRPTR)Msg_PrefsWin_Hotlists_DFA_STR},
    {Msg_PrefsWin_Hotlists_DFAHelp,(STRPTR)Msg_PrefsWin_Hotlists_DFAHelp_STR},
    {Msg_PrefsWin_Hotlists_STFax,(STRPTR)Msg_PrefsWin_Hotlists_STFax_STR},
    {Msg_PrefsWin_Hotlists_STFaxHelp,(STRPTR)Msg_PrefsWin_Hotlists_STFaxHelp_STR},
    {Msg_PrefsWin_Hotlists_Thor,(STRPTR)Msg_PrefsWin_Hotlists_Thor_STR},
    {Msg_PrefsWin_Hotlists_ThorHelp,(STRPTR)Msg_PrefsWin_Hotlists_ThorHelp_STR},
    {Msg_PrefsWin_Hotlists_AmFTP,(STRPTR)Msg_PrefsWin_Hotlists_AmFTP_STR},
    {Msg_PrefsWin_Hotlists_AmFTPHelp,(STRPTR)Msg_PrefsWin_Hotlists_AmFTPHelp_STR},
    {Msg_PrefsWin_Hotlists_DOpus,(STRPTR)Msg_PrefsWin_Hotlists_DOpus_STR},
    {Msg_PrefsWin_Hotlists_DOpusHelp,(STRPTR)Msg_PrefsWin_Hotlists_DOpusHelp_STR},
    {Msg_PrefsWin_Hotlists_AmIRC,(STRPTR)Msg_PrefsWin_Hotlists_AmIRC_STR},
    {Msg_PrefsWin_Hotlists_AmIRCHelp,(STRPTR)Msg_PrefsWin_Hotlists_AmIRCHelp_STR},
    {Msg_PrefsWin_HotsFill,(STRPTR)Msg_PrefsWin_HotsFill_STR},
    {Msg_PrefsWin_HotsFillHelp,(STRPTR)Msg_PrefsWin_HotsFillHelp_STR},
    {Msg_PrefsWin_HotsClear,(STRPTR)Msg_PrefsWin_HotsClear_STR},
    {Msg_PrefsWin_HotsClearHelp,(STRPTR)Msg_PrefsWin_HotsClearHelp_STR},
    {Msg_PrefsWin_Settings_EmptyNodes,(STRPTR)Msg_PrefsWin_Settings_EmptyNodes_STR},
    {Msg_PrefsWin_Settings_EmptyNodesHelp,(STRPTR)Msg_PrefsWin_Settings_EmptyNodesHelp_STR},
    {Msg_PrefsWin_Settings_GroupsStatus,(STRPTR)Msg_PrefsWin_Settings_GroupsStatus_STR},
    {Msg_PrefsWin_Settings_GroupsStatusHelp,(STRPTR)Msg_PrefsWin_Settings_GroupsStatusHelp_STR},
    {Msg_PrefsWin_Settings_GroupsStatusRemember,(STRPTR)Msg_PrefsWin_Settings_GroupsStatusRemember_STR},
    {Msg_PrefsWin_Settings_GroupsStatusOpened,(STRPTR)Msg_PrefsWin_Settings_GroupsStatusOpened_STR},
    {Msg_PrefsWin_Settings_GroupsStatusClosed,(STRPTR)Msg_PrefsWin_Settings_GroupsStatusClosed_STR},
    {Msg_PrefsWin_Settings_SectionsImages,(STRPTR)Msg_PrefsWin_Settings_SectionsImages_STR},
    {Msg_PrefsWin_Settings_SectionsImagesHelp,(STRPTR)Msg_PrefsWin_Settings_SectionsImagesHelp_STR},
    {Msg_PrefsWin_Settings_GroupsImages,(STRPTR)Msg_PrefsWin_Settings_GroupsImages_STR},
    {Msg_PrefsWin_Settings_GroupsImagesHelp,(STRPTR)Msg_PrefsWin_Settings_GroupsImagesHelp_STR},
    {Msg_PrefsWin_Settings_Codesets,(STRPTR)Msg_PrefsWin_Settings_Codesets_STR},
    {Msg_PrefsWin_Settings_CodesetsHelp,(STRPTR)Msg_PrefsWin_Settings_CodesetsHelp_STR},
    {Msg_PrefsWin_Hotlists_Netscape,(STRPTR)Msg_PrefsWin_Hotlists_Netscape_STR},
    {Msg_PrefsWin_Hotlists_NetscapeHelp,(STRPTR)Msg_PrefsWin_Hotlists_NetscapeHelp_STR},
    {Msg_PrefsWin_Settings_ManualSort,(STRPTR)Msg_PrefsWin_Settings_ManualSort_STR},
    {Msg_PrefsWin_Settings_ManualSortHelp,(STRPTR)Msg_PrefsWin_Settings_ManualSortHelp_STR},
    {Msg_PrefsWin_TitleCard,(STRPTR)Msg_PrefsWin_TitleCard_STR},
    {Msg_PrefsWin_CardBack,(STRPTR)Msg_PrefsWin_CardBack_STR},
    {Msg_PrefsWin_CardBackHelp,(STRPTR)Msg_PrefsWin_CardBackHelp_STR},
    {Msg_PrefsWin_CardBackTitle,(STRPTR)Msg_PrefsWin_CardBackTitle_STR},
    {Msg_PrefsWin_TitlesPen,(STRPTR)Msg_PrefsWin_TitlesPen_STR},
    {Msg_PrefsWin_TitlesPenHelp,(STRPTR)Msg_PrefsWin_TitlesPenHelp_STR},
    {Msg_PrefsWin_TitlesPenTitle,(STRPTR)Msg_PrefsWin_TitlesPenTitle_STR},
    {Msg_PrefsWin_TitlesBackPen,(STRPTR)Msg_PrefsWin_TitlesBackPen_STR},
    {Msg_PrefsWin_TitlesBackPenHelp,(STRPTR)Msg_PrefsWin_TitlesBackPenHelp_STR},
    {Msg_PrefsWin_TitlesBackPenTitle,(STRPTR)Msg_PrefsWin_TitlesBackPenTitle_STR},
    {Msg_PrefsWin_URLPen,(STRPTR)Msg_PrefsWin_URLPen_STR},
    {Msg_PrefsWin_URLPenHelp,(STRPTR)Msg_PrefsWin_URLPenHelp_STR},
    {Msg_PrefsWin_URLPenTitle,(STRPTR)Msg_PrefsWin_URLPenTitle_STR},
    {Msg_PrefsWin_URLOverPen,(STRPTR)Msg_PrefsWin_URLOverPen_STR},
    {Msg_PrefsWin_URLOverPenHelp,(STRPTR)Msg_PrefsWin_URLOverPenHelp_STR},
    {Msg_PrefsWin_URLOverPenTitle,(STRPTR)Msg_PrefsWin_URLOverPenTitle_STR},
    {Msg_PrefsWin_CardFont,(STRPTR)Msg_PrefsWin_CardFont_STR},
    {Msg_PrefsWin_CardFontHelp,(STRPTR)Msg_PrefsWin_CardFontHelp_STR},
    {Msg_PrefsWin_TitlesFont,(STRPTR)Msg_PrefsWin_TitlesFont_STR},
    {Msg_PrefsWin_TitlesFontHelp,(STRPTR)Msg_PrefsWin_TitlesFontHelp_STR},
    {Msg_PrefsWin_ShowMiniature,(STRPTR)Msg_PrefsWin_ShowMiniature_STR},
    {Msg_PrefsWin_ShowMiniatureHelp,(STRPTR)Msg_PrefsWin_ShowMiniatureHelp_STR},
    {Msg_PrefsWin_MiniatureSize,(STRPTR)Msg_PrefsWin_MiniatureSize_STR},
    {Msg_PrefsWin_MiniatureSizeHelp,(STRPTR)Msg_PrefsWin_MiniatureSizeHelp_STR},
    {Msg_PrefsWin_Settings_ClosedGroups,(STRPTR)Msg_PrefsWin_Settings_ClosedGroups_STR},
    {Msg_PrefsWin_Settings_ClosedGroupsHelp,(STRPTR)Msg_PrefsWin_Settings_ClosedGroupsHelp_STR},
    {Msg_PrefsWin_MiniatureFrame,(STRPTR)Msg_PrefsWin_MiniatureFrame_STR},
    {Msg_PrefsWin_MiniatureFrameHelp,(STRPTR)Msg_PrefsWin_MiniatureFrameHelp_STR},
    {Msg_PrefsWin_MiniatureFrameTitle,(STRPTR)Msg_PrefsWin_MiniatureFrameTitle_STR},
    {Msg_PrefsWin_Settings_IgnoreLast,(STRPTR)Msg_PrefsWin_Settings_IgnoreLast_STR},
    {Msg_PrefsWin_Settings_IgnoreLastHelp,(STRPTR)Msg_PrefsWin_Settings_IgnoreLastHelp_STR},
    {Msg_PrefsWin_Hotlists_SimpleMail,(STRPTR)Msg_PrefsWin_Hotlists_SimpleMail_STR},
    {Msg_PrefsWin_Hotlists_SimpleMailHelp,(STRPTR)Msg_PrefsWin_Hotlists_SimpleMailHelp_STR},
    {Msg_List_Menu_Title,(STRPTR)Msg_List_Menu_Title_STR},
    {Msg_List_Menu_DefWidthThis,(STRPTR)Msg_List_Menu_DefWidthThis_STR},
    {Msg_List_Menu_DefWidthAll,(STRPTR)Msg_List_Menu_DefWidthAll_STR},
    {Msg_List_Menu_DefOrderThis,(STRPTR)Msg_List_Menu_DefOrderThis_STR},
    {Msg_List_Menu_DefOrderAll,(STRPTR)Msg_List_Menu_DefOrderAll_STR},
    {Msg_List_Menu_DefSort,(STRPTR)Msg_List_Menu_DefSort_STR},
    {Msg_Text_Menu_Title,(STRPTR)Msg_Text_Menu_Title_STR},
    {Msg_String_Menu_Title,(STRPTR)Msg_String_Menu_Title_STR},
    {Msg_String_Menu_DeleteAll,(STRPTR)Msg_String_Menu_DeleteAll_STR},
    {Msg_String_Menu_Cut,(STRPTR)Msg_String_Menu_Cut_STR},
    {Msg_String_Menu_Copy,(STRPTR)Msg_String_Menu_Copy_STR},
    {Msg_String_Menu_Paste,(STRPTR)Msg_String_Menu_Paste_STR},
    {Msg_String_Menu_External,(STRPTR)Msg_String_Menu_External_STR},
    {Msg_String_Menu_Insert,(STRPTR)Msg_String_Menu_Insert_STR},
    {Msg_String_Menu_WordWrap,(STRPTR)Msg_String_Menu_WordWrap_STR},
    {Msg_Bar_Menu_Title,(STRPTR)Msg_Bar_Menu_Title_STR},
    {Msg_Bar_Menu_ViewMode_TextGfx,(STRPTR)Msg_Bar_Menu_ViewMode_TextGfx_STR},
    {Msg_Bar_Menu_ViewMode_Gfx,(STRPTR)Msg_Bar_Menu_ViewMode_Gfx_STR},
    {Msg_Bar_Menu_ViewMode_Text,(STRPTR)Msg_Bar_Menu_ViewMode_Text_STR},
    {Msg_Bar_Menu_Borderless,(STRPTR)Msg_Bar_Menu_Borderless_STR},
    {Msg_Bar_Menu_Sunny,(STRPTR)Msg_Bar_Menu_Sunny_STR},
    {Msg_Bar_Menu_Raised,(STRPTR)Msg_Bar_Menu_Raised_STR},
    {Msg_Menu_Configure,(STRPTR)Msg_Menu_Configure_STR},
    {Msg_Menu_Columns,(STRPTR)Msg_Menu_Columns_STR},
    {Msg_Card_Menu_Hide,(STRPTR)Msg_Card_Menu_Hide_STR},
    {Msg_Card_Menu_Title,(STRPTR)Msg_Card_Menu_Title_STR},
    {Msg_Edit_MenuEdit,(STRPTR)Msg_Edit_MenuEdit_STR},
    {Msg_Edit_MenuCopy,(STRPTR)Msg_Edit_MenuCopy_STR},
    {Msg_Edit_MenuBrowser,(STRPTR)Msg_Edit_MenuBrowser_STR},
    {Msg_Edit_MenuFTP,(STRPTR)Msg_Edit_MenuFTP_STR},
    {Msg_Edit_MenuJoin,(STRPTR)Msg_Edit_MenuJoin_STR},
    {Msg_Edit_MenuTelnet,(STRPTR)Msg_Edit_MenuTelnet_STR},
    {Msg_Edit_MenuEMail,(STRPTR)Msg_Edit_MenuEMail_STR},
    {Msg_Edit_MenuEMailHome,(STRPTR)Msg_Edit_MenuEMailHome_STR},
    {Msg_Edit_MenuEMailBusiness,(STRPTR)Msg_Edit_MenuEMailBusiness_STR},
    {Msg_Edit_MenuCall,(STRPTR)Msg_Edit_MenuCall_STR},
    {Msg_Edit_MenuCallHome,(STRPTR)Msg_Edit_MenuCallHome_STR},
    {Msg_Edit_MenuCallMobile,(STRPTR)Msg_Edit_MenuCallMobile_STR},
    {Msg_Edit_MenuCallBusiness,(STRPTR)Msg_Edit_MenuCallBusiness_STR},
    {Msg_Edit_MenuCallBusinessMobile,(STRPTR)Msg_Edit_MenuCallBusinessMobile_STR},
    {Msg_Edit_MenuFax,(STRPTR)Msg_Edit_MenuFax_STR},
    {Msg_Edit_MenuFaxHome,(STRPTR)Msg_Edit_MenuFaxHome_STR},
    {Msg_Edit_MenuFaxBusiness,(STRPTR)Msg_Edit_MenuFaxBusiness_STR},
    {Msg_Edit_MenuUser_Title,(STRPTR)Msg_Edit_MenuUser_Title_STR},
    {Msg_Edit_MenuUserList_Title,(STRPTR)Msg_Edit_MenuUserList_Title_STR},
    {Msg_Edit_MenuWWW_Title,(STRPTR)Msg_Edit_MenuWWW_Title_STR},
    {Msg_Edit_MenuWWWList_Title,(STRPTR)Msg_Edit_MenuWWWList_Title_STR},
    {Msg_Edit_MenuFTP_Title,(STRPTR)Msg_Edit_MenuFTP_Title_STR},
    {Msg_Edit_MenuFTPList_Title,(STRPTR)Msg_Edit_MenuFTPList_Title_STR},
    {Msg_Edit_MenuChat_Title,(STRPTR)Msg_Edit_MenuChat_Title_STR},
    {Msg_Edit_MenuChatList_Title,(STRPTR)Msg_Edit_MenuChatList_Title_STR},
    {Msg_Edit_MenuTelnet_Title,(STRPTR)Msg_Edit_MenuTelnet_Title_STR},
    {Msg_Edit_MenuTelnetList_Title,(STRPTR)Msg_Edit_MenuTelnetList_Title_STR},
    {Msg_Edit_OK,(STRPTR)Msg_Edit_OK_STR},
    {Msg_Edit_Cancel,(STRPTR)Msg_Edit_Cancel_STR},
    {Msg_Edit_MenuQuickEMail,(STRPTR)Msg_Edit_MenuQuickEMail_STR},
    {Msg_Edit_MenuTitle,(STRPTR)Msg_Edit_MenuTitle_STR},
    {Msg_Edit_UserWinTitle,(STRPTR)Msg_Edit_UserWinTitle_STR},
    {Msg_Edit_UserMenuTitle,(STRPTR)Msg_Edit_UserMenuTitle_STR},
    {Msg_Edit_UserNameTitle,(STRPTR)Msg_Edit_UserNameTitle_STR},
    {Msg_Edit_UserHomeTitle,(STRPTR)Msg_Edit_UserHomeTitle_STR},
    {Msg_Edit_UserBusiTitle,(STRPTR)Msg_Edit_UserBusiTitle_STR},
    {Msg_Edit_UserNoteTitle,(STRPTR)Msg_Edit_UserNoteTitle_STR},
    {Msg_Edit_UserImageTitle,(STRPTR)Msg_Edit_UserImageTitle_STR},
    {Msg_Edit_UserFirst,(STRPTR)Msg_Edit_UserFirst_STR},
    {Msg_Edit_UserLast,(STRPTR)Msg_Edit_UserLast_STR},
    {Msg_Edit_UserVisualName,(STRPTR)Msg_Edit_UserVisualName_STR},
    {Msg_Edit_UserEMail,(STRPTR)Msg_Edit_UserEMail_STR},
    {Msg_Edit_UserEMailAdd,(STRPTR)Msg_Edit_UserEMailAdd_STR},
    {Msg_Edit_UserEMailRem,(STRPTR)Msg_Edit_UserEMailRem_STR},
    {Msg_Edit_UserEMailUp,(STRPTR)Msg_Edit_UserEMailUp_STR},
    {Msg_Edit_UserEMailDown,(STRPTR)Msg_Edit_UserEMailDown_STR},
    {Msg_Edit_UserAddress,(STRPTR)Msg_Edit_UserAddress_STR},
    {Msg_Edit_UserCity,(STRPTR)Msg_Edit_UserCity_STR},
    {Msg_Edit_UserZIP,(STRPTR)Msg_Edit_UserZIP_STR},
    {Msg_Edit_UserCountry,(STRPTR)Msg_Edit_UserCountry_STR},
    {Msg_Edit_UserAlias,(STRPTR)Msg_Edit_UserAlias_STR},
    {Msg_Edit_UserPGP,(STRPTR)Msg_Edit_UserPGP_STR},
    {Msg_Edit_UserPhone,(STRPTR)Msg_Edit_UserPhone_STR},
    {Msg_Edit_UserMobile,(STRPTR)Msg_Edit_UserMobile_STR},
    {Msg_Edit_UserFax,(STRPTR)Msg_Edit_UserFax_STR},
    {Msg_Edit_UserICQ,(STRPTR)Msg_Edit_UserICQ_STR},
    {Msg_Edit_UserWWW,(STRPTR)Msg_Edit_UserWWW_STR},
    {Msg_Edit_UserFTP,(STRPTR)Msg_Edit_UserFTP_STR},
    {Msg_Edit_UserCompany,(STRPTR)Msg_Edit_UserCompany_STR},
    {Msg_Edit_UserJob,(STRPTR)Msg_Edit_UserJob_STR},
    {Msg_Edit_UserDept,(STRPTR)Msg_Edit_UserDept_STR},
    {Msg_Edit_UserOffice,(STRPTR)Msg_Edit_UserOffice_STR},
    {Msg_Edit_UserOffEMail,(STRPTR)Msg_Edit_UserOffEMail_STR},
    {Msg_Edit_UserOffAddress,(STRPTR)Msg_Edit_UserOffAddress_STR},
    {Msg_Edit_UserOffCity,(STRPTR)Msg_Edit_UserOffCity_STR},
    {Msg_Edit_UserOffZIP,(STRPTR)Msg_Edit_UserOffZIP_STR},
    {Msg_Edit_UserOffCountry,(STRPTR)Msg_Edit_UserOffCountry_STR},
    {Msg_Edit_UserOffPhone,(STRPTR)Msg_Edit_UserOffPhone_STR},
    {Msg_Edit_UserOffMobile,(STRPTR)Msg_Edit_UserOffMobile_STR},
    {Msg_Edit_UserOffFax,(STRPTR)Msg_Edit_UserOffFax_STR},
    {Msg_Edit_UserImage,(STRPTR)Msg_Edit_UserImage_STR},
    {Msg_Edit_UserImageTransparent,(STRPTR)Msg_Edit_UserImageTransparent_STR},
    {Msg_Edit_UserSex,(STRPTR)Msg_Edit_UserSex_STR},
    {Msg_Edit_UserSex_None,(STRPTR)Msg_Edit_UserSex_None_STR},
    {Msg_Edit_UserSex_Male,(STRPTR)Msg_Edit_UserSex_Male_STR},
    {Msg_Edit_UserSex_Female,(STRPTR)Msg_Edit_UserSex_Female_STR},
    {Msg_Edit_UserBirthday,(STRPTR)Msg_Edit_UserBirthday_STR},
    {Msg_Edit_UserAnniversary,(STRPTR)Msg_Edit_UserAnniversary_STR},
    {Msg_Edit_UserGetMap,(STRPTR)Msg_Edit_UserGetMap_STR},
    {Msg_Edit_WWWWinTitle,(STRPTR)Msg_Edit_WWWWinTitle_STR},
    {Msg_Edit_WWWMenuTitle,(STRPTR)Msg_Edit_WWWMenuTitle_STR},
    {Msg_Edit_WWWName,(STRPTR)Msg_Edit_WWWName_STR},
    {Msg_Edit_WWWHost,(STRPTR)Msg_Edit_WWWHost_STR},
    {Msg_Edit_WWWAlias,(STRPTR)Msg_Edit_WWWAlias_STR},
    {Msg_Edit_WWWMaster,(STRPTR)Msg_Edit_WWWMaster_STR},
    {Msg_Edit_WWWEMail,(STRPTR)Msg_Edit_WWWEMail_STR},
    {Msg_Edit_WWWComment,(STRPTR)Msg_Edit_WWWComment_STR},
    {Msg_Edit_FTPWinTitle,(STRPTR)Msg_Edit_FTPWinTitle_STR},
    {Msg_Edit_FTPMenuTitle,(STRPTR)Msg_Edit_FTPMenuTitle_STR},
    {Msg_Edit_FTPSettingsTitle,(STRPTR)Msg_Edit_FTPSettingsTitle_STR},
    {Msg_Edit_FTPAdvancedTitle,(STRPTR)Msg_Edit_FTPAdvancedTitle_STR},
    {Msg_Edit_FTPName,(STRPTR)Msg_Edit_FTPName_STR},
    {Msg_Edit_FTPHost,(STRPTR)Msg_Edit_FTPHost_STR},
    {Msg_Edit_FTPAlias,(STRPTR)Msg_Edit_FTPAlias_STR},
    {Msg_Edit_FTPEMail,(STRPTR)Msg_Edit_FTPEMail_STR},
    {Msg_Edit_FTPLocal,(STRPTR)Msg_Edit_FTPLocal_STR},
    {Msg_Edit_FTPLocalUse,(STRPTR)Msg_Edit_FTPLocalUse_STR},
    {Msg_Edit_FTPComment,(STRPTR)Msg_Edit_FTPComment_STR},
    {Msg_Edit_FTPAdvanced,(STRPTR)Msg_Edit_FTPAdvanced_STR},
    {Msg_Edit_FTPUser,(STRPTR)Msg_Edit_FTPUser_STR},
    {Msg_Edit_FTPPass,(STRPTR)Msg_Edit_FTPPass_STR},
    {Msg_Edit_FTPPort,(STRPTR)Msg_Edit_FTPPort_STR},
    {Msg_Edit_FTPRetries,(STRPTR)Msg_Edit_FTPRetries_STR},
    {Msg_Edit_FTPWinType,(STRPTR)Msg_Edit_FTPWinType_STR},
    {Msg_Edit_FTPWinTypeDual,(STRPTR)Msg_Edit_FTPWinTypeDual_STR},
    {Msg_Edit_FTPWinTypeSingle,(STRPTR)Msg_Edit_FTPWinTypeSingle_STR},
    {Msg_Edit_FTPADT,(STRPTR)Msg_Edit_FTPADT_STR},
    {Msg_Edit_FTPAnon,(STRPTR)Msg_Edit_FTPAnon_STR},
    {Msg_Edit_FTPQuiet,(STRPTR)Msg_Edit_FTPQuiet_STR},
    {Msg_Edit_FTPCompression,(STRPTR)Msg_Edit_FTPCompression_STR},
    {Msg_Edit_FTPOutput,(STRPTR)Msg_Edit_FTPOutput_STR},
    {Msg_Edit_FTPHidden,(STRPTR)Msg_Edit_FTPHidden_STR},
    {Msg_Edit_FTPNOOPs,(STRPTR)Msg_Edit_FTPNOOPs_STR},
    {Msg_Edit_FTPUseProxy,(STRPTR)Msg_Edit_FTPUseProxy_STR},
    {Msg_Edit_ChatWinTitle,(STRPTR)Msg_Edit_ChatWinTitle_STR},
    {Msg_Edit_ChatMenuTitle,(STRPTR)Msg_Edit_ChatMenuTitle_STR},
    {Msg_Edit_ChatChannel,(STRPTR)Msg_Edit_ChatChannel_STR},
    {Msg_Edit_ChatPass,(STRPTR)Msg_Edit_ChatPass_STR},
    {Msg_Edit_ChatServer,(STRPTR)Msg_Edit_ChatServer_STR},
    {Msg_Edit_ChatNick,(STRPTR)Msg_Edit_ChatNick_STR},
    {Msg_Edit_ChatMantainer,(STRPTR)Msg_Edit_ChatMantainer_STR},
    {Msg_Edit_ChatWWW,(STRPTR)Msg_Edit_ChatWWW_STR},
    {Msg_Edit_ChatEMail,(STRPTR)Msg_Edit_ChatEMail_STR},
    {Msg_Edit_ChatComment,(STRPTR)Msg_Edit_ChatComment_STR},
    {Msg_Edit_TelnetWinTitle,(STRPTR)Msg_Edit_TelnetWinTitle_STR},
    {Msg_Edit_TelnetMenuTitle,(STRPTR)Msg_Edit_TelnetMenuTitle_STR},
    {Msg_Edit_TelnetHost,(STRPTR)Msg_Edit_TelnetHost_STR},
    {Msg_Edit_TelnetPort,(STRPTR)Msg_Edit_TelnetPort_STR},
    {Msg_Edit_TelnetSSH,(STRPTR)Msg_Edit_TelnetSSH_STR},
    {Msg_Edit_TelnetLogin,(STRPTR)Msg_Edit_TelnetLogin_STR},
    {Msg_Edit_TelnetPass,(STRPTR)Msg_Edit_TelnetPass_STR},
    {Msg_Edit_TelnetEMail,(STRPTR)Msg_Edit_TelnetEMail_STR},
    {Msg_Edit_TelnetComment,(STRPTR)Msg_Edit_TelnetComment_STR},
    {Msg_Edit_TelnetAlias,(STRPTR)Msg_Edit_TelnetAlias_STR},
    {Msg_Dial_WinTitle,(STRPTR)Msg_Dial_WinTitle_STR},
    {Msg_Dial_Cancel,(STRPTR)Msg_Dial_Cancel_STR},
    {Msg_Dial_Dialling,(STRPTR)Msg_Dial_Dialling_STR},
    {Msg_Dial_RetryCancelGads,(STRPTR)Msg_Dial_RetryCancelGads_STR},
    {Msg_Dial_ModemNotResponding,(STRPTR)Msg_Dial_ModemNotResponding_STR},
    {Msg_Dial_Waiting,(STRPTR)Msg_Dial_Waiting_STR},
    {Msg_Dial_NoDevice,(STRPTR)Msg_Dial_NoDevice_STR},
    {Msg_MiniMailWin_Title,(STRPTR)Msg_MiniMailWin_Title_STR},
    {Msg_About_WinTitle,(STRPTR)Msg_About_WinTitle_STR},
    {Msg_About_Descr,(STRPTR)Msg_About_Descr_STR},
    {Msg_About_Info,(STRPTR)Msg_About_Info_STR},
    {Msg_About_Version,(STRPTR)Msg_About_Version_STR},
    {Msg_About_Authors,(STRPTR)Msg_About_Authors_STR},
    {Msg_About_Support,(STRPTR)Msg_About_Support_STR},
    {Msg_About_Database,(STRPTR)Msg_About_Database_STR},
    {Msg_About_ThirdParts,(STRPTR)Msg_About_ThirdParts_STR},
    {Msg_About_Translation,(STRPTR)Msg_About_Translation_STR},
    {Msg_About_Translator,(STRPTR)Msg_About_Translator_STR},
    {Msg_About_TranslatorURL,(STRPTR)Msg_About_TranslatorURL_STR},
    {Msg_About_OfCourse,(STRPTR)Msg_About_OfCourse_STR},
    {Msg_About_NListtree,(STRPTR)Msg_About_NListtree_STR},
    {Msg_About_NList,(STRPTR)Msg_About_NList_STR},
    {Msg_About_Popplaceholder,(STRPTR)Msg_About_Popplaceholder_STR},
    {Msg_About_Textinput,(STRPTR)Msg_About_Textinput_STR},
    {Msg_About_PopupMenu,(STRPTR)Msg_About_PopupMenu_STR},
    {Msg_About_GroupsImages,(STRPTR)Msg_About_GroupsImages_STR},
    {Msg_About_Lamp,(STRPTR)Msg_About_Lamp_STR},
    {Msg_DelWarning_Never,(STRPTR)Msg_DelWarning_Never_STR},
    {Msg_DelWarning_Always,(STRPTR)Msg_DelWarning_Always_STR},
    {Msg_DelWarning_Entry,(STRPTR)Msg_DelWarning_Entry_STR},
    {Msg_DelWarning_Entries,(STRPTR)Msg_DelWarning_Entries_STR},
    {Msg_Card_Title,(STRPTR)Msg_Card_Title_STR},
    {Msg_Card_Contact,(STRPTR)Msg_Card_Contact_STR},
    {Msg_Card_Phone,(STRPTR)Msg_Card_Phone_STR},
    {Msg_Card_Home,(STRPTR)Msg_Card_Home_STR},
    {Msg_Card_Work,(STRPTR)Msg_Card_Work_STR},
    {Msg_Card_Note,(STRPTR)Msg_Card_Note_STR},
    {Msg_Card_GetMap,(STRPTR)Msg_Card_GetMap_STR},
    {Msg_Card_Name,(STRPTR)Msg_Card_Name_STR},
    {Msg_Card_Alias,(STRPTR)Msg_Card_Alias_STR},
    {Msg_Card_EMail,(STRPTR)Msg_Card_EMail_STR},
    {Msg_Card_Phone_Home,(STRPTR)Msg_Card_Phone_Home_STR},
    {Msg_Card_Phone_Fax,(STRPTR)Msg_Card_Phone_Fax_STR},
    {Msg_Card_Phone_Mobile,(STRPTR)Msg_Card_Phone_Mobile_STR},
    {Msg_Card_Phone_Work,(STRPTR)Msg_Card_Phone_Work_STR},
    {Msg_Card_Phone_WorkFax,(STRPTR)Msg_Card_Phone_WorkFax_STR},
    {Msg_Card_Phone_WorkMobile,(STRPTR)Msg_Card_Phone_WorkMobile_STR},
    {Msg_Card_Channel,(STRPTR)Msg_Card_Channel_STR},
    {Msg_Card_Server,(STRPTR)Msg_Card_Server_STR},
    {Msg_Card_Host,(STRPTR)Msg_Card_Host_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

static const char CatCompBlock[] =
{
    "\x00\x00\x00\x01\x00\x28"
    Msg_GetDBDir_STR "\x00\x00"
    "\x00\x00\x00\x02\x00\x96"
    Msg_NOMUINoDirWarningMsg_STR "\x00"
    "\x00\x00\x00\x03\x00\x08"
    Msg_NOMUINoDirWarningGads_STR "\x00\x00"
    "\x00\x00\x00\x64\x00\x10"
    MSG_ErrReqTitle_STR "\x00\x00"
    "\x00\x00\x00\x65\x00\x04"
    MSG_NOMUIErrReqGadget_STR "\x00\x00"
    "\x00\x00\x00\x66\x00\x1C"
    Msg_Error_NoMem_STR "\x00\x00"
    "\x00\x00\x00\x67\x02\x24"
    Msg_Error_NoApp_STR "\x00\x00"
    "\x00\x00\x00\x68\x00\x14"
    Msg_Error_NoWin_STR "\x00\x00"
    "\x00\x00\x00\x69\x00\x24"
    Msg_Error_CantLogin_STR "\x00\x00"
    "\x00\x00\x00\xC8\x00\x4E"
    Msg_App_Copyright_STR "\x00\x00"
    "\x00\x00\x00\xC9\x00\x1A"
    Msg_App_Description_STR "\x00\x00"
    "\x00\x00\x00\xCA\x00\x08"
    Msg_Menu_Project_STR "\x00"
    "\x00\x00\x00\xCB\x00\x0C"
    Msg_Menu_About_STR "\x00\x00"
    "\x00\x00\x00\xCC\x00\x10"
    Msg_Menu_AboutMUI_STR "\x00\x00"
    "\x00\x00\x00\xCD\x00\x08"
    Msg_Menu_Hide_STR "\x00\x00"
    "\x00\x00\x00\xCE\x00\x08"
    Msg_Menu_Quit_STR "\x00\x00"
    "\x00\x00\x00\xCF\x00\x10"
    Msg_Menu_SaveAndQuit_STR "\x00"
    "\x00\x00\x00\xD0\x00\x0E"
    Msg_Menu_ForceQuit_STR "\x00\x00"
    "\x00\x00\x00\xD1\x00\x0A"
    Msg_Menu_Database_STR "\x00\x00"
    "\x00\x00\x00\xD2\x00\x12"
    Msg_Menu_User_STR "\x00\x00"
    "\x00\x00\x00\xD3\x00\x0A"
    Msg_Menu_Open_STR "\x00"
    "\x00\x00\x00\xD4\x00\x08"
    Msg_Menu_Save_STR "\x00\x00"
    "\x00\x00\x00\xD5\x00\x0E"
    Msg_Menu_SaveAs_STR "\x00\x00"
    "\x00\x00\x00\xD6\x00\x0E"
    Msg_Menu_LastSaved_STR "\x00\x00"
    "\x00\x00\x00\xD7\x00\x06"
    Msg_Menu_Add_STR "\x00"
    "\x00\x00\x00\xD8\x00\x08"
    Msg_Menu_Clone_STR "\x00"
    "\x00\x00\x00\xD9\x00\x08"
    Msg_Menu_Edit_STR "\x00\x00"
    "\x00\x00\x00\xDA\x00\x0A"
    Msg_Menu_Remove_STR "\x00\x00"
    "\x00\x00\x00\xDB\x00\x0E"
    Msg_Menu_SelectAll_STR "\x00\x00"
    "\x00\x00\x00\xDC\x00\x0E"
    Msg_Menu_SelectNone_STR "\x00"
    "\x00\x00\x00\xDD\x00\x10"
    Msg_Menu_SelectToggle_STR "\x00"
    "\x00\x00\x00\xDE\x00\x0C"
    Msg_Menu_Search_STR "\x00"
    "\x00\x00\x00\xDF\x00\x10"
    Msg_Menu_ScanHotlists_STR "\x00"
    "\x00\x00\x00\xE0\x00\x08"
    Msg_Menu_Import_STR "\x00\x00"
    "\x00\x00\x00\xE1\x00\x14"
    Msg_Menu_ImportYAM_STR "\x00\x00"
    "\x00\x00\x00\xE2\x00\x1C"
    Msg_Menu_ImportMD2_STR "\x00\x00"
    "\x00\x00\x00\xE3\x00\x10"
    Msg_Menu_ImportSTRICQ_STR "\x00"
    "\x00\x00\x00\xE4\x00\x10"
    Msg_Menu_ImportDFA_STR "\x00"
    "\x00\x00\x00\xE5\x00\x14"
    Msg_Menu_ImportSTFax_STR "\x00\x00"
    "\x00\x00\x00\xE6\x00\x18"
    Msg_Menu_ImportPINT_STR "\x00"
    "\x00\x00\x00\xE7\x00\x1C"
    Msg_Menu_ImportGoodNews_STR "\x00"
    "\x00\x00\x00\xE8\x00\x10"
    Msg_Menu_ImportCSV_STR "\x00"
    "\x00\x00\x00\xE9\x00\x14"
    Msg_Menu_ImportCSVFullOE_STR "\x00\x00"
    "\x00\x00\x00\xEA\x00\x14"
    Msg_Menu_ImportThor_STR "\x00"
    "\x00\x00\x00\xEB\x00\x10"
    Msg_Menu_ImportAWeb_STR "\x00"
    "\x00\x00\x00\xEC\x00\x14"
    Msg_Menu_ImportV_STR "\x00\x00"
    "\x00\x00\x00\xED\x00\x14"
    Msg_Menu_ImportIB_STR "\x00\x00"
    "\x00\x00\x00\xEE\x00\x12"
    Msg_Menu_ImportAmFTP_STR "\x00"
    "\x00\x00\x00\xEF\x00\x14"
    Msg_Menu_ImportDOpus_STR "\x00\x00"
    "\x00\x00\x00\xF0\x00\x12"
    Msg_Menu_ImportAmIRC_STR "\x00\x00"
    "\x00\x00\x00\xF1\x00\x14"
    Msg_Menu_ImportNetscape_STR "\x00\x00"
    "\x00\x00\x00\xF2\x00\x08"
    Msg_Menu_Export_STR "\x00\x00"
    "\x00\x00\x00\xF3\x00\x10"
    Msg_Menu_ExportCSV_STR "\x00"
    "\x00\x00\x00\xF4\x00\x14"
    Msg_Menu_ExportCSVFullOE_STR "\x00\x00"
    "\x00\x00\x00\xF5\x00\x10"
    Msg_Menu_ExportURLHtml_STR "\x00"
    "\x00\x00\x00\xF6\x00\x16"
    Msg_Menu_ExportWWWHtml_STR "\x00\x00"
    "\x00\x00\x00\xF7\x00\x16"
    Msg_Menu_ExportFTPHtml_STR "\x00\x00"
    "\x00\x00\x00\xF8\x00\x1C"
    Msg_Menu_ExportYAM_STR "\x00"
    "\x00\x00\x00\xF9\x00\x1E"
    Msg_Menu_ExportAWeb_STR "\x00\x00"
    "\x00\x00\x00\xFA\x00\x20"
    Msg_Menu_ExportIBrowse_STR "\x00"
    "\x00\x00\x00\xFB\x00\x20"
    Msg_Menu_ExportNetscape_STR "\x00"
    "\x00\x00\x00\xFC\x00\x0A"
    Msg_Menu_Settings_STR "\x00\x00"
    "\x00\x00\x00\xFD\x00\x0A"
    Msg_Menu_EditPrefs_STR "\x00"
    "\x00\x00\x00\xFE\x00\x0A"
    Msg_Menu_MUI_STR "\x00\x00"
    "\x00\x00\x00\xFF\x00\x08"
    Msg_Menu_SavePrefs_STR "\x00\x00"
    "\x00\x00\x01\x00\x00\x0E"
    Msg_Menu_LastSavedPrefs_STR "\x00\x00"
    "\x00\x00\x01\x01\x00\x10"
    Msg_Menu_NoBar_STR "\x00\x00"
    "\x00\x00\x01\x02\x00\x0C"
    Msg_Menu_NoGroups_STR "\x00"
    "\x00\x00\x01\x03\x00\x14"
    Msg_Menu_NoQuick_STR "\x00\x00"
    "\x00\x00\x01\x04\x00\x12"
    Msg_Menu_NoGroupsCtrl_STR "\x00\x00"
    "\x00\x00\x01\x05\x00\x12"
    Msg_Menu_NoCard_STR "\x00\x00"
    "\x00\x00\x01\x06\x00\x1A"
    Msg_Menu_ImportSimpleMail_STR "\x00"
    "\x00\x00\x01\x07\x00\x24"
    Msg_Menu_ExportSimpleMail_STR "\x00\x00"
    "\x00\x00\x01\x08\x00\x12"
    Msg_Menu_ImportATC_STR "\x00\x00"
    "\x00\x00\x01\x09\x00\x1E"
    Msg_Menu_ExportATC_STR "\x00\x00"
    "\x00\x00\x01\x2C\x00\x10"
    Msg_Req_Title_STR "\x00"
    "\x00\x00\x01\x2D\x00\x40"
    Msg_Req_HotChangedMsg_STR "\x00"
    "\x00\x00\x01\x2E\x00\x0A"
    Msg_Req_HotChangedGads_STR "\x00"
    "\x00\x00\x01\x2F\x00\x1E"
    Msg_Req_NotSavedMsg_STR "\x00"
    "\x00\x00\x01\x30\x00\x12"
    Msg_Req_NotSavedGads_STR "\x00"
    "\x00\x00\x01\x31\x00\x38"
    Msg_Req_ChangedMsg_STR "\x00\x00"
    "\x00\x00\x01\x32\x00\x0A"
    Msg_Req_ChangedGads_STR "\x00"
    "\x00\x00\x01\x33\x00\x40"
    Msg_Req_Asl_STR "\x00\x00"
    "\x00\x00\x01\x34\x00\x06"
    Msg_Req_AslGads_STR "\x00\x00"
    "\x00\x00\x01\x35\x00\x32"
    Msg_Req_DelWarningOne_STR "\x00\x00"
    "\x00\x00\x01\x36\x00\x38"
    Msg_Req_DelWarningMany_STR "\x00\x00"
    "\x00\x00\x01\x37\x00\x0A"
    Msg_Req_DelWarningGads_STR "\x00"
    "\x00\x00\x01\x38\x00\x32"
    Msg_Req_GroupDelWarningOne_STR "\x00\x00"
    "\x00\x00\x01\x39\x00\x36"
    Msg_Req_GroupDelWarningMany_STR "\x00"
    "\x00\x00\x01\x3A\x00\x0A"
    Msg_Req_GroupDelWarningGads_STR "\x00"
    "\x00\x00\x01\x90\x00\x16"
    Msg_Win_Title_STR "\x00\x00"
    "\x00\x00\x01\x91\x00\x06"
    Msg_Gad_Add_STR "\x00\x00"
    "\x00\x00\x01\x92\x00\x12"
    Msg_Gad_AddHelp_STR "\x00\x00"
    "\x00\x00\x01\x93\x00\x08"
    Msg_Gad_Clone_STR "\x00\x00"
    "\x00\x00\x01\x94\x00\x20"
    Msg_Gad_CloneHelp_STR "\x00"
    "\x00\x00\x01\x95\x00\x06"
    Msg_Gad_Edit_STR "\x00"
    "\x00\x00\x01\x96\x00\x1C"
    Msg_Gad_EditHelp_STR "\x00\x00"
    "\x00\x00\x01\x97\x00\x08"
    Msg_Gad_Remove_STR "\x00"
    "\x00\x00\x01\x98\x00\x1C"
    Msg_Gad_RemoveHelp_STR "\x00\x00"
    "\x00\x00\x01\x99\x00\x08"
    Msg_Gad_Search_STR "\x00"
    "\x00\x00\x01\x9A\x00\x18"
    Msg_Gad_SearchHelp_STR "\x00"
    "\x00\x00\x01\x9B\x00\x06"
    Msg_Gad_Save_STR "\x00"
    "\x00\x00\x01\x9C\x00\x14"
    Msg_Gad_SaveHelp_STR "\x00\x00"
    "\x00\x00\x01\x9D\x00\x08"
    Msg_Gad_Prefs_STR "\x00\x00"
    "\x00\x00\x01\x9E\x00\x1E"
    Msg_Gad_PrefsHelp_STR "\x00\x00"
    "\x00\x00\x01\x9F\x00\x06"
    Msg_Gad_Login_STR "\x00"
    "\x00\x00\x01\xA0\x00\x18"
    Msg_Gad_LoginHelp_STR "\x00\x00"
    "\x00\x00\x01\xA1\x00\x06"
    Msg_Gad_GroupName_STR "\x00\x00"
    "\x00\x00\x01\xA2\x00\x26"
    Msg_Gad_GroupNameHelp_STR "\x00\x00"
    "\x00\x00\x01\xA3\x00\x06"
    Msg_Gad_AddGroup_STR "\x00\x00"
    "\x00\x00\x01\xA4\x00\x0E"
    Msg_Gad_AddGroupHelp_STR "\x00\x00"
    "\x00\x00\x01\xA5\x00\x06"
    Msg_Gad_RemoveGroup_STR "\x00\x00"
    "\x00\x00\x01\xA6\x00\x1C"
    Msg_Gad_RemoveGroupHelp_STR "\x00\x00"
    "\x00\x00\x01\xA7\x00\x1A"
    Msg_Gad_GroupUpHelp_STR "\x00"
    "\x00\x00\x01\xA8\x00\x1C"
    Msg_Gad_GroupDownHelp_STR "\x00"
    "\x00\x00\x01\xA9\x00\x08"
    Msg_Groups_Menu_Title_STR "\x00\x00"
    "\x00\x00\x01\xAA\x00\x0A"
    Msg_Groups_Menu_OpenThis_STR "\x00"
    "\x00\x00\x01\xAB\x00\x0A"
    Msg_Groups_Menu_OpenAll_STR "\x00\x00"
    "\x00\x00\x01\xAC\x00\x0C"
    Msg_Groups_Menu_CloseThis_STR "\x00\x00"
    "\x00\x00\x01\xAD\x00\x0A"
    Msg_Groups_Menu_CloseAll_STR "\x00"
    "\x00\x00\x01\xAE\x00\x06"
    Msg_Groups_Menu_Sort_STR "\x00\x00"
    "\x00\x00\x01\xAF\x00\x08"
    Msg_Gad_Locate_STR "\x00"
    "\x00\x00\x01\xB0\x00\x26"
    Msg_Gad_LocateHelp_STR "\x00"
    "\x00\x00\x01\xB1\x00\x0C"
    Msg_Entries_STR "\x00"
    "\x00\x00\x01\xB2\x00\x06"
    Msg_Gad_Sort_STR "\x00\x00"
    "\x00\x00\x01\xB3\x00\x0E"
    Msg_Gad_SortHelp_STR "\x00"
    "\x00\x00\x01\xF4\x00\x06"
    Msg_Users_STR "\x00"
    "\x00\x00\x01\xF5\x00\x0A"
    Msg_WWW_STR "\x00"
    "\x00\x00\x01\xF6\x00\x0A"
    Msg_FTP_STR "\x00"
    "\x00\x00\x01\xF7\x00\x06"
    Msg_Chat_STR "\x00\x00"
    "\x00\x00\x01\xF8\x00\x08"
    Msg_Telnet_STR "\x00\x00"
    "\x00\x00\x01\xF9\x00\x06"
    Msg_Title_Name_STR "\x00\x00"
    "\x00\x00\x01\xFA\x00\x0A"
    Msg_Title_LastName_STR "\x00"
    "\x00\x00\x01\xFB\x00\x06"
    Msg_Title_EMail_STR "\x00"
    "\x00\x00\x01\xFC\x00\x08"
    Msg_Title_Comment_STR "\x00"
    "\x00\x00\x01\xFD\x00\x06"
    Msg_Title_Alias_STR "\x00"
    "\x00\x00\x01\xFE\x00\x0A"
    Msg_Title_Birthday_STR "\x00\x00"
    "\x00\x00\x01\xFF\x00\x0C"
    Msg_Title_Anniversary_STR "\x00"
    "\x00\x00\x02\x00\x00\x08"
    Msg_Title_Country_STR "\x00"
    "\x00\x00\x02\x01\x00\x06"
    Msg_Title_City_STR "\x00\x00"
    "\x00\x00\x02\x02\x00\x0C"
    Msg_Title_Phone_STR "\x00\x00"
    "\x00\x00\x02\x03\x00\x08"
    Msg_Title_Mobile_STR "\x00\x00"
    "\x00\x00\x02\x04\x00\x04"
    Msg_Title_Fax_STR "\x00"
    "\x00\x00\x02\x05\x00\x0A"
    Msg_Title_WWW_STR "\x00"
    "\x00\x00\x02\x06\x00\x08"
    Msg_Title_Company_STR "\x00"
    "\x00\x00\x02\x07\x00\x0E"
    Msg_Title_BPhone_STR "\x00\x00"
    "\x00\x00\x02\x08\x00\x10"
    Msg_Title_BMobile_STR "\x00\x00"
    "\x00\x00\x02\x09\x00\x0C"
    Msg_Title_BFax_STR "\x00\x00"
    "\x00\x00\x02\x0A\x00\x06"
    Msg_Title_Site_STR "\x00\x00"
    "\x00\x00\x02\x0B\x00\x0E"
    Msg_Title_LastVisited_STR "\x00\x00"
    "\x00\x00\x02\x0C\x00\x0E"
    Msg_Title_LastModified_STR "\x00"
    "\x00\x00\x02\x0D\x00\x08"
    Msg_Title_Channel_STR "\x00"
    "\x00\x00\x02\x0E\x00\x08"
    Msg_Title_Server_STR "\x00\x00"
    "\x00\x00\x02\x0F\x00\x06"
    Msg_Title_Host_STR "\x00\x00"
    "\x00\x00\x02\x10\x00\x06"
    Msg_Title_Port_STR "\x00\x00"
    "\x00\x00\x02\x11\x00\x06"
    Msg_Never_STR "\x00"
    "\x00\x00\x02\x12\x00\x08"
    Msg_Quick_UserFirst_STR "\x00\x00"
    "\x00\x00\x02\x13\x00\x06"
    Msg_Quick_UserLast_STR "\x00"
    "\x00\x00\x02\x14\x00\x08"
    Msg_Quick_UserEMail_STR "\x00\x00"
    "\x00\x00\x02\x15\x00\x06"
    Msg_Quick_WWWName_STR "\x00"
    "\x00\x00\x02\x16\x00\x06"
    Msg_Quick_WWWHost_STR "\x00"
    "\x00\x00\x02\x17\x00\x06"
    Msg_Quick_FTPName_STR "\x00"
    "\x00\x00\x02\x18\x00\x06"
    Msg_Quick_FTPHost_STR "\x00"
    "\x00\x00\x02\x19\x00\x0A"
    Msg_Quick_ChatChannel_STR "\x00\x00"
    "\x00\x00\x02\x1A\x00\x08"
    Msg_Quick_ChatServer_STR "\x00"
    "\x00\x00\x02\x1B\x00\x06"
    Msg_Quick_TelnetHost_STR "\x00"
    "\x00\x00\x02\x1C\x00\x06"
    Msg_Quick_TelnetPort_STR "\x00"
    "\x00\x00\x02\x1D\x00\x08"
    Msg_Quick_TelnetLogin_STR "\x00\x00"
    "\x00\x00\x02\x1E\x00\x0A"
    Msg_Quick_TelnetPass_STR "\x00"
    "\x00\x00\x02\x58\x00\x10"
    Msg_SearchWin_Title_STR "\x00"
    "\x00\x00\x02\x59\x00\x0A"
    Msg_SearchWin_Pattern_STR "\x00\x00"
    "\x00\x00\x02\x5A\x00\x1A"
    Msg_SearchWin_PatternHelp_STR "\x00"
    "\x00\x00\x02\x5B\x00\x0A"
    Msg_SearchWin_SearchInTitle_STR "\x00"
    "\x00\x00\x02\x5C\x00\x08"
    Msg_SearchWin_Users_STR "\x00\x00"
    "\x00\x00\x02\x5D\x00\x2A"
    Msg_SearchWin_UsersHelp_STR "\x00\x00"
    "\x00\x00\x02\x5E\x00\x0C"
    Msg_SearchWin_WWW_STR "\x00\x00"
    "\x00\x00\x02\x5F\x00\x2E"
    Msg_SearchWin_WWWHelp_STR "\x00\x00"
    "\x00\x00\x02\x60\x00\x0C"
    Msg_SearchWin_FTP_STR "\x00\x00"
    "\x00\x00\x02\x61\x00\x2E"
    Msg_SearchWin_FTPHelp_STR "\x00\x00"
    "\x00\x00\x02\x62\x00\x06"
    Msg_SearchWin_Chat_STR "\x00"
    "\x00\x00\x02\x63\x00\x30"
    Msg_SearchWin_ChatHelp_STR "\x00"
    "\x00\x00\x02\x64\x00\x08"
    Msg_SearchWin_Telnet_STR "\x00"
    "\x00\x00\x02\x65\x00\x30"
    Msg_SearchWin_TelnetHelp_STR "\x00"
    "\x00\x00\x02\x66\x00\x08"
    Msg_SearchWin_OptionsTitle_STR "\x00"
    "\x00\x00\x02\x67\x00\x14"
    Msg_SearchWin_Comment_STR "\x00\x00"
    "\x00\x00\x02\x68\x00\x2C"
    Msg_SearchWin_CommentHelp_STR "\x00"
    "\x00\x00\x02\x69\x00\x06"
    Msg_SearchWin_Case_STR "\x00"
    "\x00\x00\x02\x6A\x00\x2A"
    Msg_SearchWin_CaseHelp_STR "\x00"
    "\x00\x00\x02\x6B\x00\x0C"
    Msg_SearchWin_Wildcards_STR "\x00\x00"
    "\x00\x00\x02\x6C\x00\x44"
    Msg_SearchWin_WildcardsHelp_STR "\x00\x00"
    "\x00\x00\x02\x6D\x00\x08"
    Msg_SearchWin_Search_STR "\x00"
    "\x00\x00\x02\x6E\x00\x18"
    Msg_SearchWin_SearchHelp_STR "\x00\x00"
    "\x00\x00\x02\x6F\x00\x08"
    Msg_SearchWin_First_STR "\x00\x00"
    "\x00\x00\x02\x70\x00\x26"
    Msg_SearchWin_FirstHelp_STR "\x00"
    "\x00\x00\x02\x71\x00\x0A"
    Msg_SearchWin_Previous_STR "\x00"
    "\x00\x00\x02\x72\x00\x1C"
    Msg_SearchWin_PreviousHelp_STR "\x00\x00"
    "\x00\x00\x02\x73\x00\x08"
    Msg_SearchWin_Cancel_STR "\x00"
    "\x00\x00\x02\x74\x00\x1A"
    Msg_SearchWin_CancelHelp_STR "\x00\x00"
    "\x00\x00\x02\x75\x00\x0E"
    Msg_SearchWin_Searching_STR "\x00\x00"
    "\x00\x00\x02\x76\x00\x12"
    Msg_SearchWin_Welcome_STR "\x00\x00"
    "\x00\x00\x02\x77\x00\x16"
    Msg_SearchWin_Found_STR "\x00\x00"
    "\x00\x00\x02\x78\x00\x14"
    Msg_SearchWin_NotFound_STR "\x00\x00"
    "\x00\x00\x02\xBC\x00\x0C"
    Msg_UserWin_Title_STR "\x00"
    "\x00\x00\x02\xBD\x00\x06"
    Msg_UserWin_User_STR "\x00"
    "\x00\x00\x02\xBE\x00\x26"
    Msg_UserWin_UserHelp_STR "\x00\x00"
    "\x00\x00\x03\x20\x00\x1C"
    Msg_PrefsWin_Title_STR "\x00"
    "\x00\x00\x03\x21\x00\x08"
    Msg_PrefsWin_TitleSettings_STR "\x00"
    "\x00\x00\x03\x22\x00\x08"
    Msg_PrefsWin_TitleScripts_STR "\x00"
    "\x00\x00\x03\x23\x00\x0A"
    Msg_PrefsWin_TitleHotlists_STR "\x00\x00"
    "\x00\x00\x03\x24\x00\x08"
    Msg_PrefsWin_TitleDialer_STR "\x00\x00"
    "\x00\x00\x03\x25\x00\x06"
    Msg_PrefsWin_Save_STR "\x00"
    "\x00\x00\x03\x26\x00\x20"
    Msg_PrefsWin_SaveHelp_STR "\x00\x00"
    "\x00\x00\x03\x27\x00\x06"
    Msg_PrefsWin_Use_STR "\x00\x00"
    "\x00\x00\x03\x28\x00\x1E"
    Msg_PrefsWin_UseHelp_STR "\x00"
    "\x00\x00\x03\x29\x00\x08"
    Msg_PrefsWin_Apply_STR "\x00\x00"
    "\x00\x00\x03\x2A\x00\x26"
    Msg_PrefsWin_ApplyHelp_STR "\x00\x00"
    "\x00\x00\x03\x2B\x00\x08"
    Msg_PrefsWin_Cancel_STR "\x00"
    "\x00\x00\x03\x2C\x00\x26"
    Msg_PrefsWin_CancelHelp_STR "\x00\x00"
    "\x00\x00\x03\x2D\x00\x08"
    Msg_PrefsWin_Settings_Locale_STR "\x00\x00"
    "\x00\x00\x03\x2E\x00\x0A"
    Msg_PrefsWin_Settings_Country_STR "\x00\x00"
    "\x00\x00\x03\x2F\x00\x26"
    Msg_PrefsWin_Settings_CountryHelp_STR "\x00\x00"
    "\x00\x00\x03\x30\x00\x0E"
    Msg_PrefsWin_Settings_CountryCode_STR "\x00"
    "\x00\x00\x03\x31\x00\x20"
    Msg_PrefsWin_Settings_CountryCodeHelp_STR "\x00\x00"
    "\x00\x00\x03\x32\x00\x12"
    Msg_PrefsWin_Settings_GetFromLocale_STR "\x00\x00"
    "\x00\x00\x03\x33\x00\x38"
    Msg_PrefsWin_Settings_GetFromLocaleHelp_STR "\x00\x00"
    "\x00\x00\x03\x34\x00\x0A"
    Msg_PrefsWin_Settings_Options_STR "\x00\x00"
    "\x00\x00\x03\x35\x00\x14"
    Msg_PrefsWin_Settings_DelWarning_STR "\x00\x00"
    "\x00\x00\x03\x36\x00\x3A"
    Msg_PrefsWin_Settings_DelWarningHelp_STR "\x00\x00"
    "\x00\x00\x03\x37\x00\x12"
    Msg_PrefsWin_Settings_DoubleClick_STR "\x00\x00"
    "\x00\x00\x03\x38\x00\x3E"
    Msg_PrefsWin_Settings_DoubleClickHelp_STR "\x00"
    "\x00\x00\x03\x39\x00\x0C"
    Msg_PrefsWin_Settings_DCEdit_STR "\x00\x00"
    "\x00\x00\x03\x3A\x00\x0E"
    Msg_PrefsWin_Settings_DCScript_STR "\x00\x00"
    "\x00\x00\x03\x3B\x00\x16"
    Msg_PrefsWin_Settings_DCScriptQuit_STR "\x00"
    "\x00\x00\x03\x3C\x00\x16"
    Msg_PrefsWin_Settings_DCScriptHide_STR "\x00"
    "\x00\x00\x03\x3D\x00\x16"
    Msg_PrefsWin_Settings_SilentScan_STR "\x00"
    "\x00\x00\x03\x3E\x00\x64"
    Msg_PrefsWin_Settings_SilentScanHelp_STR "\x00"
    "\x00\x00\x03\x3F\x00\x0E"
    Msg_PrefsWin_Settings_UseOpenURL_STR "\x00\x00"
    "\x00\x00\x03\x40\x00\x3A"
    Msg_PrefsWin_Settings_UseOpenURLHelp_STR "\x00\x00"
    "\x00\x00\x03\x41\x00\x10"
    Msg_PrefsWin_Settings_BoldSections_STR "\x00\x00"
    "\x00\x00\x03\x42\x00\x2C"
    Msg_PrefsWin_Settings_BoldSectionsHelp_STR "\x00"
    "\x00\x00\x03\x43\x00\x18"
    Msg_PrefsWin_Settings_BoldTitles_STR "\x00"
    "\x00\x00\x03\x44\x00\x2C"
    Msg_PrefsWin_Settings_BoldTitlesHelp_STR "\x00"
    "\x00\x00\x03\x45\x00\x22"
    Msg_PrefsWin_Settings_AutoReload_STR "\x00\x00"
    "\x00\x00\x03\x46\x00\x68"
    Msg_PrefsWin_Settings_AutoReloadHelp_STR "\x00\x00"
    "\x00\x00\x03\x47\x00\x0E"
    Msg_PrefsWin_Settings_SaveOnExit_STR "\x00"
    "\x00\x00\x03\x48\x00\x60"
    Msg_PrefsWin_Settings_SaveOnExitHelp_STR "\x00"
    "\x00\x00\x03\x49\x00\x1C"
    Msg_PrefsWin_Settings_SilentExit_STR "\x00\x00"
    "\x00\x00\x03\x4A\x00\x5E"
    Msg_PrefsWin_Settings_SilentExitHelp_STR "\x00\x00"
    "\x00\x00\x03\x4B\x00\x16"
    Msg_PrefsWin_Settings_SaveOnChanges_STR "\x00"
    "\x00\x00\x03\x4C\x00\x42"
    Msg_PrefsWin_Settings_SaveOnChangesHelp_STR "\x00\x00"
    "\x00\x00\x03\x4D\x00\x18"
    Msg_PrefsWin_Settings_NoListBar_STR "\x00\x00"
    "\x00\x00\x03\x4E\x00\x2E"
    Msg_PrefsWin_Settings_NoListBarHelp_STR "\x00"
    "\x00\x00\x03\x4F\x00\x22"
    Msg_PrefsWin_Settings_NtCheckChanges_STR "\x00"
    "\x00\x00\x03\x50\x00\x52"
    Msg_PrefsWin_Settings_NtCheckChangesHelp_STR "\x00"
    "\x00\x00\x03\x51\x00\x1C"
    Msg_PrefsWin_Settings_RightMouseActive_STR "\x00"
    "\x00\x00\x03\x52\x00\x3E"
    Msg_PrefsWin_Settings_RightMouseActiveHelp_STR "\x00"
    "\x00\x00\x03\x53\x00\x14"
    Msg_PrefsWin_Settings_EditNew_STR "\x00\x00"
    "\x00\x00\x03\x54\x00\x46"
    Msg_PrefsWin_Settings_EditNewHelp_STR "\x00"
    "\x00\x00\x03\x55\x00\x08"
    Msg_PrefsWin_Settings_ButtonsBar_STR "\x00\x00"
    "\x00\x00\x03\x56\x00\x0A"
    Msg_PrefsWin_Settings_BarPosition_STR "\x00"
    "\x00\x00\x03\x57\x00\x28"
    Msg_PrefsWin_Settings_BarPositionHelp_STR "\x00"
    "\x00\x00\x03\x58\x00\x04"
    Msg_PrefsWin_Settings_BarPositionTop_STR "\x00"
    "\x00\x00\x03\x59\x00\x08"
    Msg_PrefsWin_Settings_BarPositionBottom_STR "\x00\x00"
    "\x00\x00\x03\x5A\x00\x06"
    Msg_PrefsWin_Settings_BarPositionLeft_STR "\x00\x00"
    "\x00\x00\x03\x5B\x00\x06"
    Msg_PrefsWin_Settings_BarPositionRight_STR "\x00"
    "\x00\x00\x03\x5C\x00\x0C"
    Msg_PrefsWin_Settings_BarPositionInnertTop_STR "\x00\x00"
    "\x00\x00\x03\x5D\x00\x0E"
    Msg_PrefsWin_Settings_BarPositionInnerBottom_STR "\x00\x00"
    "\x00\x00\x03\x5E\x00\x0C"
    Msg_PrefsWin_Settings_ViewMode_STR "\x00"
    "\x00\x00\x03\x5F\x00\x2A"
    Msg_PrefsWin_Settings_ViewModeHelp_STR "\x00"
    "\x00\x00\x03\x60\x00\x10"
    Msg_PrefsWin_Settings_TextGfx_STR "\x00\x00"
    "\x00\x00\x03\x61\x00\x0C"
    Msg_PrefsWin_Settings_Gfx_STR "\x00\x00"
    "\x00\x00\x03\x62\x00\x0A"
    Msg_PrefsWin_Settings_Text_STR "\x00"
    "\x00\x00\x03\x63\x00\x0C"
    Msg_PrefsWin_Settings_Borderless_STR "\x00"
    "\x00\x00\x03\x64\x00\x26"
    Msg_PrefsWin_Settings_BorderlessHelp_STR "\x00\x00"
    "\x00\x00\x03\x65\x00\x0C"
    Msg_PrefsWin_Settings_Sunny_STR "\x00\x00"
    "\x00\x00\x03\x66\x00\x5C"
    Msg_PrefsWin_Settings_SunnyHelp_STR "\x00"
    "\x00\x00\x03\x67\x00\x08"
    Msg_PrefsWin_Settings_Raised_STR "\x00"
    "\x00\x00\x03\x68\x00\x52"
    Msg_PrefsWin_Settings_RaisedHelp_STR "\x00"
    "\x00\x00\x03\x69\x00\x08"
    Msg_PrefsWin_Settings_Scaled_STR "\x00"
    "\x00\x00\x03\x6A\x00\x3C"
    Msg_PrefsWin_Settings_ScaledHelp_STR "\x00\x00"
    "\x00\x00\x03\x6B\x00\x0C"
    Msg_PrefsWin_Settings_Underscore_STR "\x00"
    "\x00\x00\x03\x6C\x00\x2E"
    Msg_PrefsWin_Settings_UnderscoreHelp_STR "\x00"
    "\x00\x00\x03\x6D\x00\x12"
    Msg_PrefsWin_Settings_ButtonsPosition_STR "\x00"
    "\x00\x00\x03\x6E\x00\x24"
    Msg_PrefsWin_Settings_ButtonsPositionHelp_STR "\x00"
    "\x00\x00\x03\x6F\x00\x0C"
    Msg_PrefsWin_Settings_ButtonsPosition_Left_STR "\x00\x00"
    "\x00\x00\x03\x70\x00\x08"
    Msg_PrefsWin_Settings_ButtonsPosition_Center_STR "\x00\x00"
    "\x00\x00\x03\x71\x00\x10"
    Msg_PrefsWin_Settings_ButtonsPosition_Right_STR "\x00\x00"
    "\x00\x00\x03\x72\x00\x10"
    Msg_PrefsWin_Settings_LabelPosition_STR "\x00"
    "\x00\x00\x03\x73\x00\x3E"
    Msg_PrefsWin_Settings_LabelPositionHelp_STR "\x00"
    "\x00\x00\x03\x74\x00\x08"
    Msg_PrefsWin_Settings_LabelPosition_Bottom_STR "\x00\x00"
    "\x00\x00\x03\x75\x00\x04"
    Msg_PrefsWin_Settings_LabelPosition_Top_STR "\x00"
    "\x00\x00\x03\x76\x00\x06"
    Msg_PrefsWin_Settings_LabelPosition_Right_STR "\x00"
    "\x00\x00\x03\x77\x00\x06"
    Msg_PrefsWin_Settings_LabelPosition_Left_STR "\x00\x00"
    "\x00\x00\x03\x78\x00\x0C"
    Msg_PrefsWin_Settings_BarSpacer_STR "\x00"
    "\x00\x00\x03\x79\x00\x38"
    Msg_PrefsWin_Settings_BarSpacerHelp_STR "\x00"
    "\x00\x00\x03\x7A\x00\x14"
    Msg_PrefsWin_Settings_Frame_STR "\x00\x00"
    "\x00\x00\x03\x7B\x00\x34"
    Msg_PrefsWin_Settings_FrameHelp_STR "\x00\x00"
    "\x00\x00\x03\x7C\x00\x18"
    Msg_PrefsWin_Settings_DragBar_STR "\x00\x00"
    "\x00\x00\x03\x7D\x00\x2E"
    Msg_PrefsWin_Settings_DragBarHelp_STR "\x00"
    "\x00\x00\x03\x7E\x00\x04"
    Msg_PrefsWin_Programs_UserScript_STR "\x00"
    "\x00\x00\x03\x7F\x00\x0C"
    Msg_PrefsWin_Programs_MailerTitle_STR "\x00\x00"
    "\x00\x00\x03\x80\x00\x08"
    Msg_PrefsWin_Programs_Mailer_STR "\x00\x00"
    "\x00\x00\x03\x81\x00\x3E"
    Msg_PrefsWin_Programs_MailerHelp_STR "\x00"
    "\x00\x00\x03\x82\x00\x06"
    Msg_PrefsWin_Programs_MailerScript_STR "\x00\x00"
    "\x00\x00\x03\x83\x00\x16"
    Msg_PrefsWin_Programs_MailerScriptHelp_STR "\x00\x00"
    "\x00\x00\x03\x84\x00\x08"
    Msg_PrefsWin_Programs_MailerPath_STR "\x00\x00"
    "\x00\x00\x03\x85\x00\x28"
    Msg_PrefsWin_Programs_MailerPathHelp_STR "\x00"
    "\x00\x00\x03\x86\x00\x10"
    Msg_PrefsWin_Programs_WWWTitle_STR "\x00"
    "\x00\x00\x03\x87\x00\x08"
    Msg_PrefsWin_Programs_WWW_STR "\x00\x00"
    "\x00\x00\x03\x88\x00\x44"
    Msg_PrefsWin_Programs_WWWHelp_STR "\x00\x00"
    "\x00\x00\x03\x89\x00\x06"
    Msg_PrefsWin_Programs_WWWScript_STR "\x00\x00"
    "\x00\x00\x03\x8A\x00\x16"
    Msg_PrefsWin_Programs_WWWScriptHelp_STR "\x00\x00"
    "\x00\x00\x03\x8B\x00\x08"
    Msg_PrefsWin_Programs_WWWPath_STR "\x00\x00"
    "\x00\x00\x03\x8C\x00\x28"
    Msg_PrefsWin_Programs_WWWPathHelp_STR "\x00"
    "\x00\x00\x03\x8D\x00\x0C"
    Msg_PrefsWin_Programs_FTPTitle_STR "\x00"
    "\x00\x00\x03\x8E\x00\x08"
    Msg_PrefsWin_Programs_FTP_STR "\x00\x00"
    "\x00\x00\x03\x8F\x00\x40"
    Msg_PrefsWin_Programs_FTPHelp_STR "\x00\x00"
    "\x00\x00\x03\x90\x00\x06"
    Msg_PrefsWin_Programs_FTPScript_STR "\x00\x00"
    "\x00\x00\x03\x91\x00\x16"
    Msg_PrefsWin_Programs_FTPScriptHelp_STR "\x00\x00"
    "\x00\x00\x03\x92\x00\x08"
    Msg_PrefsWin_Programs_FTPPath_STR "\x00\x00"
    "\x00\x00\x03\x93\x00\x28"
    Msg_PrefsWin_Programs_FTPPathHelp_STR "\x00"
    "\x00\x00\x03\x94\x00\x08"
    Msg_PrefsWin_Programs_FTPLocal_STR "\x00\x00"
    "\x00\x00\x03\x95\x00\x2A"
    Msg_PrefsWin_Programs_FTPLocalHelp_STR "\x00\x00"
    "\x00\x00\x03\x96\x00\x04"
    Msg_PrefsWin_Programs_FTPLocalUse_STR "\x00"
    "\x00\x00\x03\x97\x00\x22"
    Msg_PrefsWin_Programs_FTPLocalUseHelp_STR "\x00\x00"
    "\x00\x00\x03\x98\x00\x0A"
    Msg_PrefsWin_Programs_ChatTitle_STR "\x00"
    "\x00\x00\x03\x99\x00\x08"
    Msg_PrefsWin_Programs_Chat_STR "\x00\x00"
    "\x00\x00\x03\x9A\x00\x3E"
    Msg_PrefsWin_Programs_ChatHelp_STR "\x00\x00"
    "\x00\x00\x03\x9B\x00\x06"
    Msg_PrefsWin_Programs_ChatScript_STR "\x00\x00"
    "\x00\x00\x03\x9C\x00\x16"
    Msg_PrefsWin_Programs_ChatScriptHelp_STR "\x00\x00"
    "\x00\x00\x03\x9D\x00\x08"
    Msg_PrefsWin_Programs_ChatPath_STR "\x00\x00"
    "\x00\x00\x03\x9E\x00\x28"
    Msg_PrefsWin_Programs_ChatPathHelp_STR "\x00"
    "\x00\x00\x03\x9F\x00\x10"
    Msg_PrefsWin_Programs_TelnetTitle_STR "\x00\x00"
    "\x00\x00\x03\xA0\x00\x08"
    Msg_PrefsWin_Programs_Telnet_STR "\x00\x00"
    "\x00\x00\x03\xA1\x00\x42"
    Msg_PrefsWin_Programs_TelnetHelp_STR "\x00"
    "\x00\x00\x03\xA2\x00\x06"
    Msg_PrefsWin_Programs_TelnetScript_STR "\x00\x00"
    "\x00\x00\x03\xA3\x00\x14"
    Msg_PrefsWin_Programs_TelnetScriptHelp_STR "\x00"
    "\x00\x00\x03\xA4\x00\x08"
    Msg_PrefsWin_Programs_TelnetPath_STR "\x00\x00"
    "\x00\x00\x03\xA5\x00\x28"
    Msg_PrefsWin_Programs_TelnetPathHelp_STR "\x00"
    "\x00\x00\x03\xA6\x00\x0A"
    Msg_PrefsWin_Programs_FaxTitle_STR "\x00\x00"
    "\x00\x00\x03\xA7\x00\x08"
    Msg_PrefsWin_Programs_Fax_STR "\x00\x00"
    "\x00\x00\x03\xA8\x00\x3C"
    Msg_PrefsWin_Programs_FaxHelp_STR "\x00"
    "\x00\x00\x03\xA9\x00\x06"
    Msg_PrefsWin_Programs_FaxScript_STR "\x00\x00"
    "\x00\x00\x03\xAA\x00\x14"
    Msg_PrefsWin_Programs_FaxScriptHelp_STR "\x00"
    "\x00\x00\x03\xAB\x00\x08"
    Msg_PrefsWin_Programs_FaxPath_STR "\x00\x00"
    "\x00\x00\x03\xAC\x00\x28"
    Msg_PrefsWin_Programs_FaxPathHelp_STR "\x00"
    "\x00\x00\x03\xAD\x00\x22"
    Msg_PrefsWin_Programs_SavedProgTitle_STR "\x00\x00"
    "\x00\x00\x03\xAE\x00\x0C"
    Msg_PrefsWin_Programs_SavedProg_STR "\x00"
    "\x00\x00\x03\xAF\x00\x2C"
    Msg_PrefsWin_Programs_SavedProgHelp_STR "\x00\x00"
    "\x00\x00\x03\xB0\x00\x04"
    Msg_PrefsWin_Programs_SavedProgUse_STR "\x00"
    "\x00\x00\x03\xB1\x00\x2A"
    Msg_PrefsWin_Programs_SavedProgUseHelp_STR "\x00"
    "\x00\x00\x03\xB2\x00\x08"
    Msg_PrefsWin_Programs_SavedProgPopData_STR "\x00"
    "\x00\x00\x03\xB3\x00\x08"
    Msg_PrefsWin_Dialer_Dialer_STR "\x00"
    "\x00\x00\x03\xB4\x00\x48"
    Msg_PrefsWin_Dialer_DialerHelp_STR "\x00"
    "\x00\x00\x03\xB5\x00\x16"
    Msg_PrefsWin_Dialer_DExternal_STR "\x00\x00"
    "\x00\x00\x03\xB6\x00\x0A"
    Msg_PrefsWin_Dialer_DInternal_STR "\x00\x00"
    "\x00\x00\x03\xB7\x00\x08"
    Msg_PrefsWin_Dialer_Device_STR "\x00"
    "\x00\x00\x03\xB8\x00\x20"
    Msg_PrefsWin_Dialer_DeviceHelp_STR "\x00"
    "\x00\x00\x03\xB9\x00\x06"
    Msg_PrefsWin_Dialer_Unit_STR "\x00"
    "\x00\x00\x03\xBA\x00\x1A"
    Msg_PrefsWin_Dialer_UnitHelp_STR "\x00\x00"
    "\x00\x00\x03\xBB\x00\x08"
    Msg_PrefsWin_Dialer_Shared_STR "\x00"
    "\x00\x00\x03\xBC\x00\x32"
    Msg_PrefsWin_Dialer_SharedHelp_STR "\x00"
    "\x00\x00\x03\xBD\x00\x0C"
    Msg_PrefsWin_Dialer_ModemType_STR "\x00"
    "\x00\x00\x03\xBE\x00\x18"
    Msg_PrefsWin_Dialer_ModemTypeHelp_STR "\x00"
    "\x00\x00\x03\xBF\x00\x0E"
    Msg_PrefsWin_Dialer_ModemInit_STR "\x00\x00"
    "\x00\x00\x03\xC0\x00\x20"
    Msg_PrefsWin_Dialer_ModemInitHelp_STR "\x00"
    "\x00\x00\x03\xC1\x00\x0E"
    Msg_PrefsWin_Dialer_DialPrefix_STR "\x00\x00"
    "\x00\x00\x03\xC2\x00\x28"
    Msg_PrefsWin_Dialer_DialPrefixHelp_STR "\x00\x00"
    "\x00\x00\x03\xC3\x00\x0E"
    Msg_PrefsWin_Dialer_DialSuffix_STR "\x00\x00"
    "\x00\x00\x03\xC4\x00\x28"
    Msg_PrefsWin_Dialer_DialSuffixHelp_STR "\x00\x00"
    "\x00\x00\x03\xC5\x00\x08"
    Msg_PrefsWin_Dialer_RedialDelay_STR "\x00\x00"
    "\x00\x00\x03\xC6\x00\x38"
    Msg_PrefsWin_Dialer_RedialDelayHelp_STR "\x00\x00"
    "\x00\x00\x03\xC7\x00\x0A"
    Msg_PrefsWin_Dialer_RedialAttempts_STR "\x00"
    "\x00\x00\x03\xC8\x00\x24"
    Msg_PrefsWin_Dialer_RedialAttemptsHelp_STR "\x00\x00"
    "\x00\x00\x03\xC9\x00\x04"
    Msg_PrefsWin_Dialer_Log_STR "\x00"
    "\x00\x00\x03\xCA\x00\x16"
    Msg_PrefsWin_Dialer_LogHelp_STR "\x00\x00"
    "\x00\x00\x03\xCB\x00\x0E"
    Msg_PrefsWin_Dialer_Program_STR "\x00\x00"
    "\x00\x00\x03\xCC\x00\x2C"
    Msg_PrefsWin_Dialer_ProgramHelp_STR "\x00\x00"
    "\x00\x00\x03\xCD\x00\x10"
    Msg_PrefsWin_Dialer_ProgramPopNumber_STR "\x00"
    "\x00\x00\x03\xCE\x00\x0A"
    Msg_PrefsWin_Hotlists_V_STR "\x00\x00"
    "\x00\x00\x03\xCF\x00\x26"
    Msg_PrefsWin_Hotlists_VHelp_STR "\x00"
    "\x00\x00\x03\xD0\x00\x0A"
    Msg_PrefsWin_Hotlists_IB_STR "\x00\x00"
    "\x00\x00\x03\xD1\x00\x26"
    Msg_PrefsWin_Hotlists_IBHelp_STR "\x00"
    "\x00\x00\x03\xD2\x00\x06"
    Msg_PrefsWin_Hotlists_AWeb_STR "\x00"
    "\x00\x00\x03\xD3\x00\x24"
    Msg_PrefsWin_Hotlists_AWebHelp_STR "\x00\x00"
    "\x00\x00\x03\xD4\x00\x0E"
    Msg_PrefsWin_Hotlists_MD2_STR "\x00\x00"
    "\x00\x00\x03\xD5\x00\x2E"
    Msg_PrefsWin_Hotlists_MD2Help_STR "\x00"
    "\x00\x00\x03\xD6\x00\x06"
    Msg_PrefsWin_Hotlists_YAM_STR "\x00\x00"
    "\x00\x00\x03\xD7\x00\x26"
    Msg_PrefsWin_Hotlists_YAMHelp_STR "\x00"
    "\x00\x00\x03\xD8\x00\x0E"
    Msg_PrefsWin_Hotlists_STRICQ_STR "\x00"
    "\x00\x00\x03\xD9\x00\x24"
    Msg_PrefsWin_Hotlists_STRICQHelp_STR "\x00\x00"
    "\x00\x00\x03\xDA\x00\x10"
    Msg_PrefsWin_Hotlists_GoodNews_STR "\x00"
    "\x00\x00\x03\xDB\x00\x2A"
    Msg_PrefsWin_Hotlists_GoodNewsHelp_STR "\x00\x00"
    "\x00\x00\x03\xDC\x00\x0C"
    Msg_PrefsWin_Hotlists_PINT_STR "\x00"
    "\x00\x00\x03\xDD\x00\x26"
    Msg_PrefsWin_Hotlists_PINTHelp_STR "\x00\x00"
    "\x00\x00\x03\xDE\x00\x06"
    Msg_PrefsWin_Hotlists_DFA_STR "\x00\x00"
    "\x00\x00\x03\xDF\x00\x24"
    Msg_PrefsWin_Hotlists_DFAHelp_STR "\x00"
    "\x00\x00\x03\xE0\x00\x08"
    Msg_PrefsWin_Hotlists_STFax_STR "\x00\x00"
    "\x00\x00\x03\xE1\x00\x26"
    Msg_PrefsWin_Hotlists_STFaxHelp_STR "\x00"
    "\x00\x00\x03\xE2\x00\x06"
    Msg_PrefsWin_Hotlists_Thor_STR "\x00\x00"
    "\x00\x00\x03\xE3\x00\x26"
    Msg_PrefsWin_Hotlists_ThorHelp_STR "\x00\x00"
    "\x00\x00\x03\xE4\x00\x08"
    Msg_PrefsWin_Hotlists_AmFTP_STR "\x00\x00"
    "\x00\x00\x03\xE5\x00\x26"
    Msg_PrefsWin_Hotlists_AmFTPHelp_STR "\x00\x00"
    "\x00\x00\x03\xE6\x00\x0C"
    Msg_PrefsWin_Hotlists_DOpus_STR "\x00\x00"
    "\x00\x00\x03\xE7\x00\x26"
    Msg_PrefsWin_Hotlists_DOpusHelp_STR "\x00"
    "\x00\x00\x03\xE8\x00\x08"
    Msg_PrefsWin_Hotlists_AmIRC_STR "\x00\x00"
    "\x00\x00\x03\xE9\x00\x24"
    Msg_PrefsWin_Hotlists_AmIRCHelp_STR "\x00"
    "\x00\x00\x03\xEA\x00\x06"
    Msg_PrefsWin_HotsFill_STR "\x00"
    "\x00\x00\x03\xEB\x00\x34"
    Msg_PrefsWin_HotsFillHelp_STR "\x00"
    "\x00\x00\x03\xEC\x00\x08"
    Msg_PrefsWin_HotsClear_STR "\x00\x00"
    "\x00\x00\x03\xED\x00\x14"
    Msg_PrefsWin_HotsClearHelp_STR "\x00"
    "\x00\x00\x03\xEE\x00\x20"
    Msg_PrefsWin_Settings_EmptyNodes_STR "\x00\x00"
    "\x00\x00\x03\xEF\x00\x32"
    Msg_PrefsWin_Settings_EmptyNodesHelp_STR "\x00"
    "\x00\x00\x03\xF0\x00\x10"
    Msg_PrefsWin_Settings_GroupsStatus_STR "\x00\x00"
    "\x00\x00\x03\xF1\x00\x2C"
    Msg_PrefsWin_Settings_GroupsStatusHelp_STR "\x00"
    "\x00\x00\x03\xF2\x00\x0E"
    Msg_PrefsWin_Settings_GroupsStatusRemember_STR "\x00"
    "\x00\x00\x03\xF3\x00\x0E"
    Msg_PrefsWin_Settings_GroupsStatusOpened_STR "\x00"
    "\x00\x00\x03\xF4\x00\x0E"
    Msg_PrefsWin_Settings_GroupsStatusClosed_STR "\x00"
    "\x00\x00\x03\xF5\x00\x16"
    Msg_PrefsWin_Settings_SectionsImages_STR "\x00\x00"
    "\x00\x00\x03\xF6\x00\x28"
    Msg_PrefsWin_Settings_SectionsImagesHelp_STR "\x00\x00"
    "\x00\x00\x03\xF7\x00\x14"
    Msg_PrefsWin_Settings_GroupsImages_STR "\x00\x00"
    "\x00\x00\x03\xF8\x00\x26"
    Msg_PrefsWin_Settings_GroupsImagesHelp_STR "\x00\x00"
    "\x00\x00\x03\xF9\x00\x0A"
    Msg_PrefsWin_Settings_Codesets_STR "\x00\x00"
    "\x00\x00\x03\xFA\x00\x14"
    Msg_PrefsWin_Settings_CodesetsHelp_STR "\x00"
    "\x00\x00\x03\xFB\x00\x0A"
    Msg_PrefsWin_Hotlists_Netscape_STR "\x00\x00"
    "\x00\x00\x03\xFC\x00\x26"
    Msg_PrefsWin_Hotlists_NetscapeHelp_STR "\x00"
    "\x00\x00\x03\xFD\x00\x0E"
    Msg_PrefsWin_Settings_ManualSort_STR "\x00\x00"
    "\x00\x00\x03\xFE\x00\x34"
    Msg_PrefsWin_Settings_ManualSortHelp_STR "\x00\x00"
    "\x00\x00\x03\xFF\x00\x0A"
    Msg_PrefsWin_TitleCard_STR "\x00"
    "\x00\x00\x04\x00\x00\x12"
    Msg_PrefsWin_CardBack_STR "\x00\x00"
    "\x00\x00\x04\x01\x00\x1C"
    Msg_PrefsWin_CardBackHelp_STR "\x00"
    "\x00\x00\x04\x02\x00\x1C"
    Msg_PrefsWin_CardBackTitle_STR "\x00\x00"
    "\x00\x00\x04\x03\x00\x0C"
    Msg_PrefsWin_TitlesPen_STR "\x00"
    "\x00\x00\x04\x04\x00\x18"
    Msg_PrefsWin_TitlesPenHelp_STR "\x00\x00"
    "\x00\x00\x04\x05\x00\x16"
    Msg_PrefsWin_TitlesPenTitle_STR "\x00"
    "\x00\x00\x04\x06\x00\x12"
    Msg_PrefsWin_TitlesBackPen_STR "\x00\x00"
    "\x00\x00\x04\x07\x00\x22"
    Msg_PrefsWin_TitlesBackPenHelp_STR "\x00"
    "\x00\x00\x04\x08\x00\x1C"
    Msg_PrefsWin_TitlesBackPenTitle_STR "\x00\x00"
    "\x00\x00\x04\x09\x00\x0A"
    Msg_PrefsWin_URLPen_STR "\x00\x00"
    "\x00\x00\x04\x0A\x00\x14"
    Msg_PrefsWin_URLPenHelp_STR "\x00"
    "\x00\x00\x04\x0B\x00\x14"
    Msg_PrefsWin_URLPenTitle_STR "\x00\x00"
    "\x00\x00\x04\x0C\x00\x0E"
    Msg_PrefsWin_URLOverPen_STR "\x00"
    "\x00\x00\x04\x0D\x00\x1A"
    Msg_PrefsWin_URLOverPenHelp_STR "\x00\x00"
    "\x00\x00\x04\x0E\x00\x18"
    Msg_PrefsWin_URLOverPenTitle_STR "\x00"
    "\x00\x00\x04\x0F\x00\x10"
    Msg_PrefsWin_CardFont_STR "\x00"
    "\x00\x00\x04\x10\x00\x1C"
    Msg_PrefsWin_CardFontHelp_STR "\x00"
    "\x00\x00\x04\x11\x00\x0C"
    Msg_PrefsWin_TitlesFont_STR "\x00"
    "\x00\x00\x04\x12\x00\x18"
    Msg_PrefsWin_TitlesFontHelp_STR "\x00"
    "\x00\x00\x04\x13\x00\x12"
    Msg_PrefsWin_ShowMiniature_STR "\x00\x00"
    "\x00\x00\x04\x14\x00\x24"
    Msg_PrefsWin_ShowMiniatureHelp_STR "\x00\x00"
    "\x00\x00\x04\x15\x00\x12"
    Msg_PrefsWin_MiniatureSize_STR "\x00\x00"
    "\x00\x00\x04\x16\x00\x16"
    Msg_PrefsWin_MiniatureSizeHelp_STR "\x00\x00"
    "\x00\x00\x04\x17\x00\x18"
    Msg_PrefsWin_Settings_ClosedGroups_STR "\x00"
    "\x00\x00\x04\x18\x00\x32"
    Msg_PrefsWin_Settings_ClosedGroupsHelp_STR "\x00"
    "\x00\x00\x04\x19\x00\x12"
    Msg_PrefsWin_MiniatureFrame_STR "\x00\x00"
    "\x00\x00\x04\x1A\x00\x1C"
    Msg_PrefsWin_MiniatureFrameHelp_STR "\x00"
    "\x00\x00\x04\x1B\x00\x1C"
    Msg_PrefsWin_MiniatureFrameTitle_STR "\x00\x00"
    "\x00\x00\x04\x1C\x00\x20"
    Msg_PrefsWin_Settings_IgnoreLast_STR "\x00\x00"
    "\x00\x00\x04\x1D\x00\x56"
    Msg_PrefsWin_Settings_IgnoreLastHelp_STR "\x00"
    "\x00\x00\x04\x1E\x00\x0C"
    Msg_PrefsWin_Hotlists_SimpleMail_STR "\x00\x00"
    "\x00\x00\x04\x1F\x00\x2E"
    Msg_PrefsWin_Hotlists_SimpleMailHelp_STR "\x00\x00"
    "\x00\x00\x04\xB0\x00\x06"
    Msg_List_Menu_Title_STR "\x00\x00"
    "\x00\x00\x04\xB1\x00\x14"
    Msg_List_Menu_DefWidthThis_STR "\x00"
    "\x00\x00\x04\xB2\x00\x14"
    Msg_List_Menu_DefWidthAll_STR "\x00\x00"
    "\x00\x00\x04\xB3\x00\x14"
    Msg_List_Menu_DefOrderThis_STR "\x00"
    "\x00\x00\x04\xB4\x00\x14"
    Msg_List_Menu_DefOrderAll_STR "\x00\x00"
    "\x00\x00\x04\xB5\x00\x10"
    Msg_List_Menu_DefSort_STR "\x00"
    "\x00\x00\x04\xB6\x00\x06"
    Msg_Text_Menu_Title_STR "\x00\x00"
    "\x00\x00\x04\xB7\x00\x06"
    Msg_String_Menu_Title_STR "\x00\x00"
    "\x00\x00\x04\xB8\x00\x0C"
    Msg_String_Menu_DeleteAll_STR "\x00\x00"
    "\x00\x00\x04\xB9\x00\x04"
    Msg_String_Menu_Cut_STR "\x00"
    "\x00\x00\x04\xBA\x00\x06"
    Msg_String_Menu_Copy_STR "\x00\x00"
    "\x00\x00\x04\xBB\x00\x06"
    Msg_String_Menu_Paste_STR "\x00"
    "\x00\x00\x04\xBC\x00\x12"
    Msg_String_Menu_External_STR "\x00\x00"
    "\x00\x00\x04\xBD\x00\x14"
    Msg_String_Menu_Insert_STR "\x00"
    "\x00\x00\x04\xBE\x00\x0A"
    Msg_String_Menu_WordWrap_STR "\x00\x00"
    "\x00\x00\x04\xBF\x00\x08"
    Msg_Bar_Menu_Title_STR "\x00\x00"
    "\x00\x00\x04\xC0\x00\x10"
    Msg_Bar_Menu_ViewMode_TextGfx_STR "\x00\x00"
    "\x00\x00\x04\xC1\x00\x0C"
    Msg_Bar_Menu_ViewMode_Gfx_STR "\x00\x00"
    "\x00\x00\x04\xC2\x00\x0A"
    Msg_Bar_Menu_ViewMode_Text_STR "\x00"
    "\x00\x00\x04\xC3\x00\x0C"
    Msg_Bar_Menu_Borderless_STR "\x00\x00"
    "\x00\x00\x04\xC4\x00\x0A"
    Msg_Bar_Menu_Sunny_STR "\x00"
    "\x00\x00\x04\xC5\x00\x08"
    Msg_Bar_Menu_Raised_STR "\x00\x00"
    "\x00\x00\x04\xC6\x00\x0A"
    Msg_Menu_Configure_STR "\x00"
    "\x00\x00\x04\xC7\x00\x08"
    Msg_Menu_Columns_STR "\x00"
    "\x00\x00\x04\xC8\x00\x06"
    Msg_Card_Menu_Hide_STR "\x00\x00"
    "\x00\x00\x04\xC9\x00\x06"
    Msg_Card_Menu_Title_STR "\x00\x00"
    "\x00\x00\x05\x78\x00\x06"
    Msg_Edit_MenuEdit_STR "\x00\x00"
    "\x00\x00\x05\x79\x00\x0E"
    Msg_Edit_MenuCopy_STR "\x00\x00"
    "\x00\x00\x05\x7A\x00\x10"
    Msg_Edit_MenuBrowser_STR "\x00"
    "\x00\x00\x05\x7B\x00\x0C"
    Msg_Edit_MenuFTP_STR "\x00"
    "\x00\x00\x05\x7C\x00\x0A"
    Msg_Edit_MenuJoin_STR "\x00"
    "\x00\x00\x05\x7D\x00\x10"
    Msg_Edit_MenuTelnet_STR "\x00\x00"
    "\x00\x00\x05\x7E\x00\x0C"
    Msg_Edit_MenuEMail_STR "\x00\x00"
    "\x00\x00\x05\x7F\x00\x06"
    Msg_Edit_MenuEMailHome_STR "\x00\x00"
    "\x00\x00\x05\x80\x00\x0A"
    Msg_Edit_MenuEMailBusiness_STR "\x00\x00"
    "\x00\x00\x05\x81\x00\x06"
    Msg_Edit_MenuCall_STR "\x00\x00"
    "\x00\x00\x05\x82\x00\x06"
    Msg_Edit_MenuCallHome_STR "\x00\x00"
    "\x00\x00\x05\x83\x00\x08"
    Msg_Edit_MenuCallMobile_STR "\x00\x00"
    "\x00\x00\x05\x84\x00\x0A"
    Msg_Edit_MenuCallBusiness_STR "\x00\x00"
    "\x00\x00\x05\x85\x00\x10"
    Msg_Edit_MenuCallBusinessMobile_STR "\x00"
    "\x00\x00\x05\x86\x00\x0A"
    Msg_Edit_MenuFax_STR "\x00\x00"
    "\x00\x00\x05\x87\x00\x06"
    Msg_Edit_MenuFaxHome_STR "\x00\x00"
    "\x00\x00\x05\x88\x00\x0A"
    Msg_Edit_MenuFaxBusiness_STR "\x00\x00"
    "\x00\x00\x05\x89\x00\x06"
    Msg_Edit_MenuUser_Title_STR "\x00\x00"
    "\x00\x00\x05\x8A\x00\x0A"
    Msg_Edit_MenuUserList_Title_STR "\x00"
    "\x00\x00\x05\x8B\x00\x04"
    Msg_Edit_MenuWWW_Title_STR "\x00"
    "\x00\x00\x05\x8C\x00\x0A"
    Msg_Edit_MenuWWWList_Title_STR "\x00\x00"
    "\x00\x00\x05\x8D\x00\x04"
    Msg_Edit_MenuFTP_Title_STR "\x00"
    "\x00\x00\x05\x8E\x00\x0A"
    Msg_Edit_MenuFTPList_Title_STR "\x00\x00"
    "\x00\x00\x05\x8F\x00\x06"
    Msg_Edit_MenuChat_Title_STR "\x00\x00"
    "\x00\x00\x05\x90\x00\x0A"
    Msg_Edit_MenuChatList_Title_STR "\x00"
    "\x00\x00\x05\x91\x00\x08"
    Msg_Edit_MenuTelnet_Title_STR "\x00\x00"
    "\x00\x00\x05\x92\x00\x0C"
    Msg_Edit_MenuTelnetList_Title_STR "\x00"
    "\x00\x00\x06\x40\x00\x06"
    Msg_Edit_OK_STR "\x00\x00"
    "\x00\x00\x06\x41\x00\x08"
    Msg_Edit_Cancel_STR "\x00"
    "\x00\x00\x06\x42\x00\x0C"
    Msg_Edit_MenuQuickEMail_STR "\x00"
    "\x00\x00\x06\x43\x00\x06"
    Msg_Edit_MenuTitle_STR "\x00\x00"
    "\x00\x00\x07\x08\x00\x0A"
    Msg_Edit_UserWinTitle_STR "\x00"
    "\x00\x00\x07\x09\x00\x0E"
    Msg_Edit_UserMenuTitle_STR "\x00\x00"
    "\x00\x00\x07\x0A\x00\x06"
    Msg_Edit_UserNameTitle_STR "\x00\x00"
    "\x00\x00\x07\x0B\x00\x06"
    Msg_Edit_UserHomeTitle_STR "\x00\x00"
    "\x00\x00\x07\x0C\x00\x08"
    Msg_Edit_UserBusiTitle_STR "\x00\x00"
    "\x00\x00\x07\x0D\x00\x06"
    Msg_Edit_UserNoteTitle_STR "\x00"
    "\x00\x00\x07\x0E\x00\x06"
    Msg_Edit_UserImageTitle_STR "\x00"
    "\x00\x00\x07\x0F\x00\x08"
    Msg_Edit_UserFirst_STR "\x00\x00"
    "\x00\x00\x07\x10\x00\x06"
    Msg_Edit_UserLast_STR "\x00"
    "\x00\x00\x07\x11\x00\x0A"
    Msg_Edit_UserVisualName_STR "\x00\x00"
    "\x00\x00\x07\x12\x00\x08"
    Msg_Edit_UserEMail_STR "\x00\x00"
    "\x00\x00\x07\x13\x00\x06"
    Msg_Edit_UserEMailAdd_STR "\x00\x00"
    "\x00\x00\x07\x14\x00\x08"
    Msg_Edit_UserEMailRem_STR "\x00"
    "\x00\x00\x07\x15\x00\x0A"
    Msg_Edit_UserEMailUp_STR "\x00\x00"
    "\x00\x00\x07\x16\x00\x0C"
    Msg_Edit_UserEMailDown_STR "\x00\x00"
    "\x00\x00\x07\x17\x00\x0A"
    Msg_Edit_UserAddress_STR "\x00\x00"
    "\x00\x00\x07\x18\x00\x06"
    Msg_Edit_UserCity_STR "\x00"
    "\x00\x00\x07\x19\x00\x06"
    Msg_Edit_UserZIP_STR "\x00\x00"
    "\x00\x00\x07\x1A\x00\x0A"
    Msg_Edit_UserCountry_STR "\x00\x00"
    "\x00\x00\x07\x1B\x00\x08"
    Msg_Edit_UserAlias_STR "\x00\x00"
    "\x00\x00\x07\x1C\x00\x06"
    Msg_Edit_UserPGP_STR "\x00\x00"
    "\x00\x00\x07\x1D\x00\x08"
    Msg_Edit_UserPhone_STR "\x00\x00"
    "\x00\x00\x07\x1E\x00\x08"
    Msg_Edit_UserMobile_STR "\x00"
    "\x00\x00\x07\x1F\x00\x06"
    Msg_Edit_UserFax_STR "\x00\x00"
    "\x00\x00\x07\x20\x00\x06"
    Msg_Edit_UserICQ_STR "\x00\x00"
    "\x00\x00\x07\x21\x00\x06"
    Msg_Edit_UserWWW_STR "\x00\x00"
    "\x00\x00\x07\x22\x00\x06"
    Msg_Edit_UserFTP_STR "\x00\x00"
    "\x00\x00\x07\x23\x00\x0A"
    Msg_Edit_UserCompany_STR "\x00\x00"
    "\x00\x00\x07\x24\x00\x06"
    Msg_Edit_UserJob_STR "\x00\x00"
    "\x00\x00\x07\x25\x00\x06"
    Msg_Edit_UserDept_STR "\x00"
    "\x00\x00\x07\x26\x00\x08"
    Msg_Edit_UserOffice_STR "\x00"
    "\x00\x00\x07\x27\x00\x08"
    Msg_Edit_UserOffEMail_STR "\x00\x00"
    "\x00\x00\x07\x28\x00\x0A"
    Msg_Edit_UserOffAddress_STR "\x00\x00"
    "\x00\x00\x07\x29\x00\x06"
    Msg_Edit_UserOffCity_STR "\x00"
    "\x00\x00\x07\x2A\x00\x06"
    Msg_Edit_UserOffZIP_STR "\x00\x00"
    "\x00\x00\x07\x2B\x00\x0A"
    Msg_Edit_UserOffCountry_STR "\x00\x00"
    "\x00\x00\x07\x2C\x00\x08"
    Msg_Edit_UserOffPhone_STR "\x00\x00"
    "\x00\x00\x07\x2D\x00\x08"
    Msg_Edit_UserOffMobile_STR "\x00"
    "\x00\x00\x07\x2E\x00\x06"
    Msg_Edit_UserOffFax_STR "\x00\x00"
    "\x00\x00\x07\x2F\x00\x08"
    Msg_Edit_UserImage_STR "\x00\x00"
    "\x00\x00\x07\x30\x00\x06"
    Msg_Edit_UserImageTransparent_STR "\x00"
    "\x00\x00\x07\x31\x00\x06"
    Msg_Edit_UserSex_STR "\x00\x00"
    "\x00\x00\x07\x32\x00\x04"
    Msg_Edit_UserSex_None_STR "\x00"
    "\x00\x00\x07\x33\x00\x06"
    Msg_Edit_UserSex_Male_STR "\x00\x00"
    "\x00\x00\x07\x34\x00\x08"
    Msg_Edit_UserSex_Female_STR "\x00\x00"
    "\x00\x00\x07\x35\x00\x0A"
    Msg_Edit_UserBirthday_STR "\x00"
    "\x00\x00\x07\x36\x00\x0E"
    Msg_Edit_UserAnniversary_STR "\x00\x00"
    "\x00\x00\x07\x37\x00\x08"
    Msg_Edit_UserGetMap_STR "\x00"
    "\x00\x00\x07\x6C\x00\x0E"
    Msg_Edit_WWWWinTitle_STR "\x00"
    "\x00\x00\x07\x6D\x00\x0C"
    Msg_Edit_WWWMenuTitle_STR "\x00"
    "\x00\x00\x07\x6E\x00\x06"
    Msg_Edit_WWWName_STR "\x00"
    "\x00\x00\x07\x6F\x00\x06"
    Msg_Edit_WWWHost_STR "\x00"
    "\x00\x00\x07\x70\x00\x08"
    Msg_Edit_WWWAlias_STR "\x00\x00"
    "\x00\x00\x07\x71\x00\x08"
    Msg_Edit_WWWMaster_STR "\x00"
    "\x00\x00\x07\x72\x00\x08"
    Msg_Edit_WWWEMail_STR "\x00\x00"
    "\x00\x00\x07\x73\x00\x0A"
    Msg_Edit_WWWComment_STR "\x00\x00"
    "\x00\x00\x07\xD0\x00\x0E"
    Msg_Edit_FTPWinTitle_STR "\x00"
    "\x00\x00\x07\xD1\x00\x0C"
    Msg_Edit_FTPMenuTitle_STR "\x00"
    "\x00\x00\x07\xD2\x00\x0A"
    Msg_Edit_FTPSettingsTitle_STR "\x00\x00"
    "\x00\x00\x07\xD3\x00\x0A"
    Msg_Edit_FTPAdvancedTitle_STR "\x00\x00"
    "\x00\x00\x07\xD4\x00\x06"
    Msg_Edit_FTPName_STR "\x00"
    "\x00\x00\x07\xD5\x00\x06"
    Msg_Edit_FTPHost_STR "\x00"
    "\x00\x00\x07\xD6\x00\x08"
    Msg_Edit_FTPAlias_STR "\x00\x00"
    "\x00\x00\x07\xD7\x00\x08"
    Msg_Edit_FTPEMail_STR "\x00\x00"
    "\x00\x00\x07\xD8\x00\x08"
    Msg_Edit_FTPLocal_STR "\x00"
    "\x00\x00\x07\xD9\x00\x06"
    Msg_Edit_FTPLocalUse_STR "\x00\x00"
    "\x00\x00\x07\xDA\x00\x0A"
    Msg_Edit_FTPComment_STR "\x00\x00"
    "\x00\x00\x07\xDB\x00\x1C"
    Msg_Edit_FTPAdvanced_STR "\x00\x00"
    "\x00\x00\x07\xDC\x00\x06"
    Msg_Edit_FTPUser_STR "\x00"
    "\x00\x00\x07\xDD\x00\x0A"
    Msg_Edit_FTPPass_STR "\x00"
    "\x00\x00\x07\xDE\x00\x06"
    Msg_Edit_FTPPort_STR "\x00"
    "\x00\x00\x07\xDF\x00\x0A"
    Msg_Edit_FTPRetries_STR "\x00\x00"
    "\x00\x00\x07\xE0\x00\x08"
    Msg_Edit_FTPWinType_STR "\x00"
    "\x00\x00\x07\xE1\x00\x0A"
    Msg_Edit_FTPWinTypeDual_STR "\x00"
    "\x00\x00\x07\xE2\x00\x0C"
    Msg_Edit_FTPWinTypeSingle_STR "\x00"
    "\x00\x00\x07\xE3\x00\x06"
    Msg_Edit_FTPADT_STR "\x00\x00"
    "\x00\x00\x07\xE4\x00\x0C"
    Msg_Edit_FTPAnon_STR "\x00\x00"
    "\x00\x00\x07\xE5\x00\x08"
    Msg_Edit_FTPQuiet_STR "\x00\x00"
    "\x00\x00\x07\xE6\x00\x0E"
    Msg_Edit_FTPCompression_STR "\x00\x00"
    "\x00\x00\x07\xE7\x00\x08"
    Msg_Edit_FTPOutput_STR "\x00"
    "\x00\x00\x07\xE8\x00\x0A"
    Msg_Edit_FTPHidden_STR "\x00\x00"
    "\x00\x00\x07\xE9\x00\x0C"
    Msg_Edit_FTPNOOPs_STR "\x00"
    "\x00\x00\x07\xEA\x00\x08"
    Msg_Edit_FTPUseProxy_STR "\x00\x00"
    "\x00\x00\x08\x34\x00\x12"
    Msg_Edit_ChatWinTitle_STR "\x00"
    "\x00\x00\x08\x35\x00\x0E"
    Msg_Edit_ChatMenuTitle_STR "\x00\x00"
    "\x00\x00\x08\x36\x00\x0A"
    Msg_Edit_ChatChannel_STR "\x00\x00"
    "\x00\x00\x08\x37\x00\x0A"
    Msg_Edit_ChatPass_STR "\x00"
    "\x00\x00\x08\x38\x00\x08"
    Msg_Edit_ChatServer_STR "\x00"
    "\x00\x00\x08\x39\x00\x06"
    Msg_Edit_ChatNick_STR "\x00"
    "\x00\x00\x08\x3A\x00\x0C"
    Msg_Edit_ChatMantainer_STR "\x00\x00"
    "\x00\x00\x08\x3B\x00\x06"
    Msg_Edit_ChatWWW_STR "\x00\x00"
    "\x00\x00\x08\x3C\x00\x08"
    Msg_Edit_ChatEMail_STR "\x00\x00"
    "\x00\x00\x08\x3D\x00\x0A"
    Msg_Edit_ChatComment_STR "\x00\x00"
    "\x00\x00\x08\x98\x00\x12"
    Msg_Edit_TelnetWinTitle_STR "\x00\x00"
    "\x00\x00\x08\x99\x00\x10"
    Msg_Edit_TelnetMenuTitle_STR "\x00\x00"
    "\x00\x00\x08\x9A\x00\x06"
    Msg_Edit_TelnetHost_STR "\x00"
    "\x00\x00\x08\x9B\x00\x06"
    Msg_Edit_TelnetPort_STR "\x00"
    "\x00\x00\x08\x9C\x00\x06"
    Msg_Edit_TelnetSSH_STR "\x00\x00"
    "\x00\x00\x08\x9D\x00\x08"
    Msg_Edit_TelnetLogin_STR "\x00\x00"
    "\x00\x00\x08\x9E\x00\x0A"
    Msg_Edit_TelnetPass_STR "\x00"
    "\x00\x00\x08\x9F\x00\x08"
    Msg_Edit_TelnetEMail_STR "\x00\x00"
    "\x00\x00\x08\xA0\x00\x0A"
    Msg_Edit_TelnetComment_STR "\x00\x00"
    "\x00\x00\x08\xA1\x00\x08"
    Msg_Edit_TelnetAlias_STR "\x00\x00"
    "\x00\x00\x09\x60\x00\x06"
    Msg_Dial_WinTitle_STR "\x00\x00"
    "\x00\x00\x09\x61\x00\x08"
    Msg_Dial_Cancel_STR "\x00"
    "\x00\x00\x09\x62\x00\x22"
    Msg_Dial_Dialling_STR "\x00\x00"
    "\x00\x00\x09\x63\x00\x10"
    Msg_Dial_RetryCancelGads_STR "\x00"
    "\x00\x00\x09\x64\x00\x34"
    Msg_Dial_ModemNotResponding_STR "\x00\x00"
    "\x00\x00\x09\x65\x00\x12"
    Msg_Dial_Waiting_STR "\x00\x00"
    "\x00\x00\x09\x66\x00\x1C"
    Msg_Dial_NoDevice_STR "\x00"
    "\x00\x00\x09\xC4\x00\x14"
    Msg_MiniMailWin_Title_STR "\x00\x00"
    "\x00\x00\x0A\x28\x00\x16"
    Msg_About_WinTitle_STR "\x00"
    "\x00\x00\x0A\x29\x00\x6C"
    Msg_About_Descr_STR "\x00"
    "\x00\x00\x0A\x2A\x00\x0C"
    Msg_About_Info_STR "\x00"
    "\x00\x00\x0A\x2B\x00\x0A"
    Msg_About_Version_STR "\x00"
    "\x00\x00\x0A\x2C\x00\x0A"
    Msg_About_Authors_STR "\x00\x00"
    "\x00\x00\x0A\x2D\x00\x0A"
    Msg_About_Support_STR "\x00\x00"
    "\x00\x00\x0A\x2E\x00\x0A"
    Msg_About_Database_STR "\x00"
    "\x00\x00\x0A\x2F\x00\x0C"
    Msg_About_ThirdParts_STR "\x00"
    "\x00\x00\x0A\x30\x00\x02"
    Msg_About_Translation_STR "\x00\x00"
    "\x00\x00\x0A\x31\x00\x02"
    Msg_About_Translator_STR "\x00\x00"
    "\x00\x00\x0A\x32\x00\x02"
    Msg_About_TranslatorURL_STR "\x00\x00"
    "\x00\x00\x0A\x33\x00\x0E"
    Msg_About_OfCourse_STR "\x00\x00"
    "\x00\x00\x0A\x34\x00\x14"
    Msg_About_NListtree_STR "\x00\x00"
    "\x00\x00\x0A\x35\x00\x12"
    Msg_About_NList_STR "\x00\x00"
    "\x00\x00\x0A\x36\x00\x18"
    Msg_About_Popplaceholder_STR "\x00"
    "\x00\x00\x0A\x37\x00\x16"
    Msg_About_Textinput_STR "\x00\x00"
    "\x00\x00\x0A\x38\x00\x16"
    Msg_About_PopupMenu_STR "\x00\x00"
    "\x00\x00\x0A\x39\x00\x12"
    Msg_About_GroupsImages_STR "\x00\x00"
    "\x00\x00\x0A\x3A\x00\x0E"
    Msg_About_Lamp_STR "\x00"
    "\x00\x00\x0A\xF0\x00\x06"
    Msg_DelWarning_Never_STR "\x00"
    "\x00\x00\x0A\xF1\x00\x08"
    Msg_DelWarning_Always_STR "\x00\x00"
    "\x00\x00\x0A\xF2\x00\x12"
    Msg_DelWarning_Entry_STR "\x00"
    "\x00\x00\x0A\xF3\x00\x16"
    Msg_DelWarning_Entries_STR "\x00"
    "\x00\x00\x0B\xB8\x00\x10"
    Msg_Card_Title_STR "\x00\x00"
    "\x00\x00\x0B\xB9\x00\x08"
    Msg_Card_Contact_STR "\x00"
    "\x00\x00\x0B\xBA\x00\x06"
    Msg_Card_Phone_STR "\x00"
    "\x00\x00\x0B\xBB\x00\x06"
    Msg_Card_Home_STR "\x00\x00"
    "\x00\x00\x0B\xBC\x00\x06"
    Msg_Card_Work_STR "\x00\x00"
    "\x00\x00\x0B\xBD\x00\x06"
    Msg_Card_Note_STR "\x00\x00"
    "\x00\x00\x0B\xBE\x00\x08"
    Msg_Card_GetMap_STR "\x00\x00"
    "\x00\x00\x0B\xBF\x00\x06"
    Msg_Card_Name_STR "\x00\x00"
    "\x00\x00\x0B\xC0\x00\x06"
    Msg_Card_Alias_STR "\x00"
    "\x00\x00\x0B\xC1\x00\x06"
    Msg_Card_EMail_STR "\x00"
    "\x00\x00\x0B\xC2\x00\x06"
    Msg_Card_Phone_Home_STR "\x00"
    "\x00\x00\x0B\xC3\x00\x04"
    Msg_Card_Phone_Fax_STR "\x00"
    "\x00\x00\x0B\xC4\x00\x06"
    Msg_Card_Phone_Mobile_STR "\x00\x00"
    "\x00\x00\x0B\xC5\x00\x06"
    Msg_Card_Phone_Work_STR "\x00\x00"
    "\x00\x00\x0B\xC6\x00\x0A"
    Msg_Card_Phone_WorkFax_STR "\x00\x00"
    "\x00\x00\x0B\xC7\x00\x0A"
    Msg_Card_Phone_WorkMobile_STR "\x00"
    "\x00\x00\x0B\xC8\x00\x08"
    Msg_Card_Channel_STR "\x00"
    "\x00\x00\x0B\xC9\x00\x08"
    Msg_Card_Server_STR "\x00\x00"
    "\x00\x00\x0B\xCA\x00\x06"
    Msg_Card_Host_STR "\x00\x00"
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

#undef LocaleBase
#define LocaleBase li->li_LocaleBase
    
    if (LocaleBase)
        return(GetCatalogStr(li->li_Catalog,stringNum,builtIn));
#undef LocaleBase

    return(builtIn);
}


#endif /* CATCOMP_CODE */


/****************************************************************************/


#endif /* CMANAGER_LOC_H */
