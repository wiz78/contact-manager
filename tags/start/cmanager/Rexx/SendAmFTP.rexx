/*
 *
 *  $VER: SendAmFTP.rexx 1.1 (3.8.98) by Simone Tellini
 *
 */

Options Results
Parse Arg '"'site'" "'ftpprg'"'

IF ~SHOW(P,'AMFTP.1') THEN DO

    Address Command 'Run <>NIL:' ftpprg

    Address Command 'SYS:RexxC/WaitForPort' AMFTP.1

END

'GETENTRY' ftp

Address AMFTP.1

if ftp.type ~= 'FTP' then
    ftp.advanced  = 0

'CONNECTSTEM' ftp

Exit 0
