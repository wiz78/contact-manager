/* VER: SendAmTelnet.rexx 1.0 (5.2.2003) by Alfonso Ranieri */

options results
call addlib("rexxsupport.library",0,-30)

call delay 1000
parse arg '"'host'" "'telnetprg'"'

/* Find a AMTELNET.X port */
p=getport("AMTELNET")
if p="" then do
    /* Not found, launch AmTelnet */
    address command 'run <>NIL:' telnetprg
    call portwait("AMTELNET.1")
    p="AMTELNET.1"
end
if ~show("p",p) then exit

/* Get the Telnet host port */
'GETENTRY' telnet

/* Changes default host */
address value(p)

/* Connect */
'CONNECT HOST' host 'PORT' telnet.port
exit

getport: procedure
parse arg base
    base=upper(base)
    ports=show('p',,'a'x)
    do while ports~=""
        parse var ports port "a"x ports
        if abbrev(upper(port),base) then return port
    end
    return ""

portwait: procedure
parse arg p
    do i=0 to 34
        if show("p",p) then leave
        call delay(10)
    end
    return
