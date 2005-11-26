/*
 *
 *  $VER: SendFax.rexx 1.0 (4.2.98) by Simone Tellini
 *
 */

Options Results
Parse Arg '"'number'" "'faxprg'"'


IF ~SHOW(P,'STFAX.1') THEN DO

    Address Command 'Run <>NIL: STFax:STFax'

    Address Command 'WaitForPort STFAX.1'   /* let's give it some time */
    Address Command 'WaitForPort STFAX.1'
    Address Command 'WaitForPort STFAX.1'

END

Address 'STFAX.1'

'NEW FORCE'
'SEND' number 'EDIT'

Exit 0
