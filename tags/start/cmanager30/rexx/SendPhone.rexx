/*
 *
 *  $VER: SendPhone.rexx 1.2 (5.8.98) by Simone Tellini
 *
 */

Options Results
Parse Arg args

IF ~SHOW(P,'STFAX.1') THEN DO

    Address Command 'Run <>NIL: STFax:STFax'

    Address Command 'WaitForPort STFAX.1'   /* let's give it some time */
    Address Command 'WaitForPort STFAX.1'
    Address Command 'WaitForPort STFAX.1'

END

Address 'STFAX.1'

'SHOW'
'DIAL' args

Exit 0
