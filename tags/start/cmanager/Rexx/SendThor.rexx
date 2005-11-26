/*
 *  SendEMail.rexx 1.0 (4.2.98) by Simone Tellini
 */

MailSys = 'Demon'   /*  Set this two variables according to your needs */
MailCnf = 'EMail'

/*
    DON'T TOUCH THE FOLLOWING LINES
*/

OPTIONS RESULTS

EVE_ENTERMSG = 0

PARSE ARG '"'Recipient'"'

GETENTRY Entry

IF ~SHOW('P', 'THOR.01') THEN
DO
    ADDRESS COMMAND
      "run <>nil: Thor:Thor PUBSCREEN Workbench SYS" MailSys
      "sys:rexxc/WaitForPort THOR.01"
END

IF ~SHOW('P', 'BBSREAD') THEN
DO
    ADDRESS COMMAND
      "run <>nil: `GetEnv THOR/THORPath`bin/LoadBBSRead"
      "sys:rexxc/WaitForPort BBSREAD"
END

Address 'BBSREAD'

UNIQUEMSGFILE MailSys UNIQUE

Address 'THOR.01'

REQUESTSTRING TITLE '"Mail subject:"' BT '"_Ok|_Cancel"' MAXCHARS 256

Subj = result

if(rc ~= 0) then do
    address command 'Delete >nil: '||'"'||UNIQUE.NAME||'"'
    Exit
End

Address Command 'Echo "" >'||UNIQUE.NAME

MAKESIGNATURE UNIQUE.NAME INCTAG

Drop EVENT.

EVENT.CONFERENCE = MailCnf
EVENT.TONAME     = Entry.Name
EVENT.TOADDR     = Recipient
EVENT.SUBJECT    = Subj
EVENT.MSGFILE    = UNIQUE.FILEPART

Address 'BBSREAD' WRITEBREVENT BBSNAME '"'MailSys'"' EVENT EVE_ENTERMSG STEM EVENT

if(rc ~= 0) then do
    REQUESTNOTIFY TEXT '"'BBSREAD.LASTERROR'"' BT "_Ok"
    Address COMMAND 'Delete >nil: '||'"'||UNIQUE.NAME||'"'
    Exit
end

STARTEDITOR '"'UNIQUE.NAME'"'

Exit
