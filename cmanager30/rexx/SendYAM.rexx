/********************************************************/
/*  SendEMail.rexx 1.1 (10.3.98) by Jon Bristow         */
/*                                                      */
/* Script to send address to Yam2                       */
/********************************************************/

OPTIONS RESULTS

EVE_ENTERMSG = 0

PARSE ARG '"'Recipient'" "'Path'"'  /* parse args */

IF ~SHOW('P', 'YAM') THEN           /* check for yam2 */
DO
    ADDRESS COMMAND                 /* if not running,  */
      'run <>nil:' Path             /* run and wait for */
      "sys:rexxc/WaitForPort YAM"   /* for port         */
END

Address 'YAM'

/* 'MAILCHECK' */               /* comment if you want yam to check mail */

'SHOW'                          /* uniconify */
'MAILWRITE'                     /* mimic write button */
'WRITEMAILTO "'||Recipient||'"' /* to address */

Exit

