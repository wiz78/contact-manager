/*
 *      $VER: SendAWeb.rexx 1.0 (5.8.98)
 */

parse arg '"'args'" "'BROWSER'"'
options results

if show('P','AWEB.1') then do
          address 'AWEB.1'
          OPEN args
          Exit
end

ADDRESS COMMAND 'Run ' BROWSER
ADDRESS COMMAND "SYS:RexxC/WaitForPort" 'AWEB.1'

if show('P','AWEB.1') then do
          address 'AWEB.1'
          OPEN args
          Exit
end
