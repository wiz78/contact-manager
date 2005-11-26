/*
 *      $VER: SendIB.rexx 1.0 (5.8.98)
 */

parse arg '"'args'" "'BROWSER'"'
options results

if show('P','IBROWSE') then do
          address 'IBROWSE'
          SHOW
          GOTOURL args
          Exit
end

ADDRESS COMMAND 'Run ' BROWSER
ADDRESS COMMAND "SYS:RexxC/WaitForPort" 'IBROWSE'

if show('P','IBROWSE') then do
          address 'IBROWSE'
          SHOW
          GOTOURL args
          Exit
end

