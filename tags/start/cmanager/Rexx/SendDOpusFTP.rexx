/*
 *
 *  $VER: SendDOpusFTP.rexx 1.0 (29.6.98) by Simone Tellini
 *
 */

Options Results
Parse Arg lister" "path

IF ~SHOW(P,'DOPUS.1') THEN
    Exit

'GETENTRY' ftp

if ftp.type ~= 'FTP' then
    ftp.advanced = 0

Address 'DOPUS.1'

if lister = 1 then
    'Command ScanDir "'path'" NEW'

if left(ftp.ftp, 6) == 'ftp://' then do
    parse VAR ftp.ftp 'ftp://' site '/' dir
end
else do
    parse VAR ftp.ftp site '/' dir
end

if ftp.anonymous = 1 | ftp.advanced = 0 then
    'Command FTPConnect' site 'DIR "'dir'"'
else
    'Command FTPConnect' site 'USER "'ftp.username'" PASS "'ftp.password'" DIR "'dir'"'

Exit 0
