/* VER: SendMiamiTelnet.rexx 1.0 (5.2.2003) by Alfonso Ranieri */

options results
parse arg '"'h'" "'telnetprg'"'

/* Get the Telnet entry */
'GETENTRY' telnet

telnetprg=telnetprg h telnet.port
/* Launch MiamiTelnet */
address command 'run <>NIL:' telnetprg
