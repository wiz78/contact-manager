VERSION EQU 34
REVISION EQU 4
DATE MACRO
    dc.b '23.7.2005'
    ENDM
VERS MACRO
    dc.b 'CManager.mcc 34.4'
    ENDM
VSTRING MACRO
    dc.b '$VER: CManager.mcc 34.4 (23.7.2005) 1998-2005 Ranieri/Tellini <alforan@tin.it>',13,10,0
    ENDM
VERSTAG MACRO
    dc.b 0,'$VER: CManager.mcc 34.4 (23.7.2005)',0
    ENDM
