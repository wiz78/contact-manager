/* VER: SendDOpusFTP.rexx 2.0 (4.2.2003) by Alfonso Ranieri */

options results
parse arg '"'site'" "'ftpprg'"'

IF ~SHOW(P,'DOPUS.1') THEN Exit

'GETENTRY' ftp

if ftp.type ~= 'FTP' then ftp.advanced = 0

if getUseLocal() then do
    path=getLocal()
    Address 'DOPUS.1'
    'Command ScanDir "'path'" NEW'
end
else Address 'DOPUS.1'

if left(ftp.ftp, 6) == 'ftp://' then parse VAR ftp.ftp 'ftp://' site '/' dir
else parse VAR ftp.ftp site '/' dir

if ftp.anonymous = 1 | ftp.advanced = 0 then 'Command FTPConnect' site 'DIR "'dir'"'
else 'Command FTPConnect' site 'USER "'ftp.username'" PASS "'ftp.password'" DIR "'dir'"'

exit

getUseLocal:
    if ftp.USELOCAL then return 1
    'GETATTR USEFTPLOCAL'
    return result

getLocal:
    if ftp.UseLocal then return ftp.LOCAL
    'GETATTR FTPLOCAL'
    return result
