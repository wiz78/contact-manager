/*
 *
 *  $VER: SendMD.rexx 1.0 (12.8.98)
 *
 */

Options Results

Parse Arg '"'email'" "'microdotPath'"'

IF ~SHOW(P,'MD.1') THEN DO

    Address Command 'Run <NIL: >NIL:' microdotPath

    Address Command 'SYS:RexxC/WaitForPort' MD.1

END

Address MD.1

'NEWMSGWINDOW TO "' || email || '"'
  
Exit 0

