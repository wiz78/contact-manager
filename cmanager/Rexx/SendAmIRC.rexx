/*
 *
 *  $VER: SendAmIRC.rexx 1.1 (5.8.98) by Simone Tellini
 *
 */

Options Results
Parse Arg '"'server'" "'channel'" "'AmIRCPath'"'

IF ~SHOW(P,'AMIRC.1') THEN DO

    Address Command 'Run <>NIL:' AmIRCPath

    Address Command 'SYS:RexxC/WaitForPort' AMIRC.1

END

Address AMIRC.1

serverok = 0

'ISCONNECTED'

if rc == 0 then do

    'GETSERVERNAME'

    if result = server then
        serverok = 1
end

if serverok = 0 then
    'SAY /SERVER' server

Address Command "Wait 5 secs"

'SAY /JOIN' channel

Exit 0
