
            ;   TAG.ASM
            ;
            ;   Library tag.  This replaces the normal startup code, c.o,
            ;   but still must perform certain startup functions such as
            ;   the clearing of BSS and small-data model setup & auto
            ;   calls.  HOWEVER, we do not include any resident startup
            ;   code meaning you CANNOT compile the shared library -r
            ;   (which doesn't make sense to do anyway).
            ;
            ;   Further, no C startup or exit functions are included
            ;   since this is a library, not a normal program.  Refer
            ;   to the source code LIB/AMIGA/C.A for a fully operational
            ;   startup module.

;Prototype ALibReserved();

            section text,code

            xref    _LibInit

            xref    _UserLibId
            xref    _UserLibName

            moveq   #-1,D0
            rts

InitDesc:   dc.w    $4AFC           ;RTC_MATCHWORD
            dc.l    InitDesc        ;Pointer to beginning
            dc.l    EndCode         ;Note sure it matters
            dc.b    0               ;flags (NO RTF_AUTOINIT)
            dc.b    14              ;version
            dc.b    9               ;NT_LIBRARY
            dc.b    0               ;priority (doesn't matter)
            dc.l    _UserLibName    ;Name of library
            dc.l    _UserLibId      ;ID string (note CR-LF at end)
            dc.l    _LibInit        ;Pointer to init routine

            ds.w    0
EndCode:


            END

