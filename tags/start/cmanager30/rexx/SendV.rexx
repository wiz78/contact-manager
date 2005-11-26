/*
 *      $VER: SendV.rexx 1.0 (5.8.98)
 */

parse arg '"'args'" "'BROWSER'"'
options results

if show('P','VOYAGER') then do
          address 'VOYAGER'
          SHOW
          OPENURL args
          Exit
end

if show('P','VOYAGER.1') then do
          address 'VOYAGER.1'
          SHOW
          OPENURL args
          Exit
end


ADDRESS COMMAND 'Run ' BROWSER
ADDRESS COMMAND "SYS:RexxC/WaitForPort" 'VOYAGER'

if show('P','VOYAGER') then do
          address 'VOYAGER'
          SHOW
          OPENURL args
          Exit
end

if show('P','VOYAGER.1') then do
          address 'VOYAGER.1'
          SHOW
          OPENURL args
          Exit
end

