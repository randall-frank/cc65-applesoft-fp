
; Accumulator and argument registers: 6-byte virtual registers
AS_FAC        = $9D   ; $9D-$A2
AS_ARG        = $A5   ; $A5-$AA

; TODO Do we need to handle these
AS_FAC_EX     = $AC
AS_ARG_EX     = $9C   

; temp usage for indexing ($60,$61 are used by Applesoft for temp operations)
AS_ZP         = $60

; The string function returns a zero-terminated string located in stack memory
AS_FBUFFR     = $0100

; To work around language-card issues, the library uses $200-$2FF to
; buffer input strings and AS_FAC_FP objects which could be located
; in language-card RAM.
AS_SCR_STR    = $0200 ; 127(+1) byte string buffer
AS_SCR_FAC    = $0280 ; 6 bytes clone of the unpacked FAC
AS_SCR_FAC_EX = $0286 ; 1 byte clone of AS_FAC_EX
AS_SCR_ARG    = $0287 ; 6 bytes clone of the unpacked ARG
AS_SCR_ARG_EX = $028d ; 1 byte clone of AS_ARG_EX
AS_SCR_TMP    = $028e ; 5 bytes cache of a mem argument
AS_SCR_TMPA   = $0293 ; 1 byte  LSB of AS_SCR_TMP target
AS_SCR_TMPX   = $0294 ; 1 byte  MSB of AS_SCR_TMP target
AS_LC_STATE   = $0295

; Other temporary zero-page memory locations used by the various functions:
; 93-97, 98-9C, 8A-8E, C9-CD

; Operation addresses (LDA AS_FAC first)
AS_ADDR_FADD  = $E7C1 ; FAC = ARG + FAC
AS_ADDR_FSUB  = $E7AA ; FAC = ARG - FAC
AS_ADDR_FMUL  = $E982 ; FAC = ARG * FAC
AS_ADDR_FDIV  = $EA69 ; FAC = ARG / FAC
AS_ADDR_FPWRT = $EE97 ; FAC = ARG ^ FAC
AS_ADDR_ABS   = $EBAF ; FAC = abs(FAC) absolute value
AS_ADDR_SQR   = $EE8D ; FAC = sqrt(FAC) square root
AS_ADDR_LOG   = $E941 ; FAC = ln(FAC) natural logarithm
AS_ADDR_EXP   = $EF09 ; FAC = exp(FAC) natural exponential
AS_ADDR_RND   = $EFAE ; FAC = rnd(FAC) random number
AS_ADDR_COS   = $EFEA ; FAC = cos(FAC) cosine (input in radians)
AS_ADDR_SIN   = $EFF1 ; FAC = sin(FAC) sine (input in radians)
AS_ADDR_TAN   = $F03A ; FAC = tan(FAC) tangent (input in radians)
AS_ADDR_ATN   = $F09E ; FAC = atan(FAC) arctangent (output in radians)
AS_ADDR_NEGOP = $EED0 ; FAC = -FAC (change sign)
AS_ADDR_MUL10 = $EA39 ; FAC = FAC * 10.
AS_ADDR_DIV10 = $EA55 ; FAC = FAC / 10.

AS_ADDR_SGN   = $EB90 ; Same as AS_ADDR_SIGN, but store in FAC
AS_ADDR_SIGN  = $EB82 ; Set A from FAC: A=1 if FAC is positive, 
                      ;                 A=0 if FAC is zero, 
                      ;                 A=$FF if FAC is negative
AS_ADDR_INT   = $EC23 ; FAC = int(FAC)

; (h,l) register pair as pointer, usually Y=MSB, A=LSB
AS_ADDR_FCOMP = $EBB2 ; Compare FAC and number pointed to by (Y,A). 
                      ;         A=1 if (Y,A)<FAC,
                      ;         A=0 if (Y,A)==FAC, 
                      ;         A=$FF if (Y,A)>FAC.

; Data transfers
AS_ADDR_FLOAT  = $EB93 ; FAC = signed integer A
AS_ADDR_SNGFLT = $E301 ; FAC = unsigned integer Y
AS_ADDR_GIVAYF = $E2F2 ; FAC=value of 2-byte signed int from Y & A registers (A,Y)
AS_ADDR_MOVFA  = $EB53 ; FAC = ARG  copy the ARG to FAC
AS_ADDR_MOVAF  = $EB63 ; ARG = FAC  copy the FAC to ARG
AS_ADDR_MOVMF  = $EB2B ; Pack and store FAC to RAM. 
                       ; X register (low byte) and Y register (high byte)
                       ; point to a destination address in RAM (Y,X)
AS_ADDR_MOVFM  = $EAF9 ; Unpack and load FAC from RAM. 
                       ; 5-byte floating point number in memory pointed by (Y,A)
AS_ADDR_CONUPK = $E9E3 ; Unpack and load ARG from RAM. 
                       ; 5-byte floating point number in memory pointed by (Y,A)

AS_ADDR_FOUT   = $ED34 ; Create null-terminated string in AS_FBUFFR from FAC. 
                       ; On exit, (Y,A) points to the string. FAC is scrambled.

AS_TXTPTR      = $B8   ; Zero-page pointer to text string (2 bytes)
AS_CHRGET      = $B1   ; Zero-page routine to get next character
AS_CHRGOT      = $B7   ; Zero-page routine to re-read current character into A
AS_ADDR_FIN    = $EC4A ; ROM routine to parse string into FAC 
                       ; (Apple II char encoding $30-$39+$2B+$2E+$2D+$05)

.include "asfp_vers.inc"

.export _as_fp_init
.export _as_fp_version
.export _as_fp_str2fac
.export _as_fp_fac2str

.export _as_fp_mem2arg
.export _as_fp_mem2fac
.export _as_fp_fac2mem
.export _as_fp_swap_fac_arg
.export _as_fp_fac2arg
.export _as_fp_arg2fac

.export _as_fp_sgn
.export _as_fp_int2fac
.export _as_fp_char2fac
.export _as_fp_uchar2fac

.export _as_fp_arg_add_fac
.export _as_fp_arg_sub_fac
.export _as_fp_arg_mul_fac
.export _as_fp_arg_div_fac
.export _as_fp_arg_pow_fac
.export _as_fp_abs_fac
.export _as_fp_int_fac
.export _as_fp_sqr_fac
.export _as_fp_log_fac
.export _as_fp_exp_fac
.export _as_fp_rnd_fac
.export _as_fp_cos_fac
.export _as_fp_sin_fac
.export _as_fp_tan_fac
.export _as_fp_atn_fac
.export _as_fp_neg_fac
.export _as_fp_inv_fac
.export _as_fp_sgn_fac
.export _as_fp_fac_mult_ten
.export _as_fp_fac_div_ten
.export _as_fp_fac_cmp_mem
.export _AS_CONST_one
.export _AS_CONST_half
.export _AS_CONST_two
.export _AS_CONST_ten
.export _AS_CONST_sqrt_two
.export _AS_CONST_e
.export _AS_CONST_pi
.export _AS_CONST_two_pi
.export _AS_CONST_ln_two

.segment "RODATA"

; Many of the functions have identical signatures
; Use a dispatch table for these
AS_IDX_FADD  =  0
AS_IDX_FSUB  =  1
AS_IDX_FMUL  =  2
AS_IDX_FDIV  =  3
AS_IDX_FPWRT =  4
AS_IDX_ABS   =  5
AS_IDX_INT   =  6
AS_IDX_SQR   =  7
AS_IDX_LOG   =  8
AS_IDX_EXP   =  9
AS_IDX_RND   =  10
AS_IDX_COS   =  11
AS_IDX_SIN   =  12
AS_IDX_TAN   =  13
AS_IDX_ATN   =  14
AS_IDX_NEGOP =  15
AS_IDX_MOVAF =  16
AS_IDX_MOVFA =  17

simple_addrs:
    .addr    AS_ADDR_FADD  
    .addr    AS_ADDR_FSUB  
    .addr    AS_ADDR_FMUL  
    .addr    AS_ADDR_FDIV  
    .addr    AS_ADDR_FPWRT  
    .addr    AS_ADDR_ABS  
    .addr    AS_ADDR_INT  
    .addr    AS_ADDR_SQR  
    .addr    AS_ADDR_LOG  
    .addr    AS_ADDR_EXP  
    .addr    AS_ADDR_RND  
    .addr    AS_ADDR_COS  
    .addr    AS_ADDR_SIN  
    .addr    AS_ADDR_TAN  
    .addr    AS_ADDR_ATN  
    .addr    AS_ADDR_NEGOP
    .addr    AS_ADDR_MOVAF
    .addr    AS_ADDR_MOVFA  

.segment "CODE"

_as_common_dispatch:
    asl
    tax
    lda simple_addrs,x
    sta dispatch+1
    lda simple_addrs+1,x
    sta dispatch+2
    jsr _as_save_lc_state
    lda AS_FAC
dispatch:
    jsr $ffff
    jmp _as_restore_lc_state

; FAC = ARG + FAC
_as_fp_arg_add_fac:
    lda #AS_IDX_FADD
    jmp _as_common_dispatch

; FAC = ARG - FAC
_as_fp_arg_sub_fac:
    lda #AS_IDX_FSUB
    jmp _as_common_dispatch

; FAC = ARG * FAC
_as_fp_arg_mul_fac:
    lda #AS_IDX_FMUL
    jmp _as_common_dispatch

; FAC = ARG / FAC
_as_fp_arg_div_fac:
    lda #AS_IDX_FDIV
    jmp _as_common_dispatch

; FAC = ARG ^ FAC
_as_fp_arg_pow_fac:
    lda #AS_IDX_FPWRT
    jmp _as_common_dispatch

; FAC = abs(FAC)
_as_fp_abs_fac:
    lda #AS_IDX_ABS
    jmp _as_common_dispatch

; FAC = int(FAC)
_as_fp_int_fac:
    lda #AS_IDX_INT
    jmp _as_common_dispatch

; FAC = sqrt(FAC)  (square root)
_as_fp_sqr_fac:
    lda #AS_IDX_SQR
    jmp _as_common_dispatch

; FAC = log(FAC) (natural, base e, log)
_as_fp_log_fac:
    lda #AS_IDX_LOG
    jmp _as_common_dispatch

; FAC = exp(FAC) (e to the FAC power)
_as_fp_exp_fac:
    lda #AS_IDX_EXP
    jmp _as_common_dispatch

; FAC = random() (semi)random number
_as_fp_rnd_fac:
    lda #AS_IDX_RND
    jmp _as_common_dispatch

; FAC = cos(FAC) (radians)
_as_fp_cos_fac:
    lda #AS_IDX_COS
    jmp _as_common_dispatch

; FAC = sin(FAC) (radians)
_as_fp_sin_fac:
    lda #AS_IDX_SIN
    jmp _as_common_dispatch

; FAC = tan(FAC) (radians)
_as_fp_tan_fac:
    lda #AS_IDX_TAN
    jmp _as_common_dispatch

; FAC = arctan(FAC) (radians)
_as_fp_atn_fac:
    lda #AS_IDX_ATN
    jmp _as_common_dispatch

; FAC = -FAC
_as_fp_neg_fac:
    lda #AS_IDX_NEGOP
    jmp _as_common_dispatch

; ARG = FAC
_as_fp_fac2arg:
    lda #AS_IDX_MOVAF
    jmp _as_common_dispatch

; FAC = ARG
_as_fp_arg2fac:
    lda #AS_IDX_MOVFA
    jmp _as_common_dispatch

; FAC = 1.0 / FAC
AS_CONST_ADDR_ONE = $E913 ; 1.0
_as_fp_inv_fac:
    jsr _as_save_lc_state
    ldy #>AS_CONST_ADDR_ONE ; ARG = 1.0 by loading ARG from ROM
    lda #<AS_CONST_ADDR_ONE
    jsr AS_ADDR_CONUPK ; Y=MSB, A=LSB
    lda AS_FAC
    jsr AS_ADDR_FDIV ; FAC = ARG / FAC
    jmp _as_restore_lc_state

; FAC = SGN(FAC) - FAC=1 if FAC>0, FAC=0 if FAC==0, FAC=-1 if FAC<0
_as_fp_sgn_fac:
    jsr _as_save_lc_state
    jsr AS_ADDR_SGN
    jmp _as_restore_lc_state

; FAC = FAC * 10.0
_as_fp_fac_mult_ten:
    jsr _as_save_lc_state
    jsr AS_ADDR_MUL10
    jmp _as_restore_lc_state

; FAC = FAC / 10.0
_as_fp_fac_div_ten:
    jsr _as_save_lc_state
    jsr AS_ADDR_DIV10
    jmp _as_restore_lc_state

; Returned int is the output of SGN(FAC)
_as_fp_sgn:
    jsr _as_save_fac
    jsr _as_save_lc_state
    jsr AS_ADDR_SIGN
    jsr _as_restore_lc_state
    jsr _as_restore_fac
    ldx #$00
    cmp #$00 ; return 16-bit signed number (X,A)
    bpl @positive
    ldx #$FF
@positive:
    rts

; FAC = passed signed int value
_as_fp_int2fac:
    ; A = low byte of the int (LSB)
    ; X = high byte of the int (MSB)
    tay
    txa
    jsr _as_save_lc_state
    jsr AS_ADDR_GIVAYF    ; A,Y = signed integer (Note: A is MSB, Y is LSB)
    jmp _as_restore_lc_state

; FAC = passed signed char value
_as_fp_char2fac:
    ; A = signed char
    jsr _as_save_lc_state
    jsr AS_ADDR_FLOAT ; A = signed integer value
    jmp _as_restore_lc_state

; FAC = passed unsigned char value
_as_fp_uchar2fac:
    ; A = unsigned char
    tay
    jsr _as_save_lc_state
    jsr AS_ADDR_SNGFLT ; Y = unsigned integer value
    jmp _as_restore_lc_state


; Returns the result of comparing a number in memory to the FAC as a signed char.
; TODO: include a raw bitwise comparison to work around mantissa differences
_as_fp_fac_cmp_mem:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    jsr _as_cache_float  ; (X,A) -> AS_SCR_TMP
    jsr _as_save_lc_state
    lda #<AS_SCR_TMP
    ldy #>AS_SCR_TMP
    jsr AS_ADDR_FCOMP ; compare FAC and number pointed to by (Y,A). A=1 if (Y,A)<FAC,
                      ; A=0 if (Y,A)==FAC, A=$FF if (Y,A)>FAC.
    jsr _as_restore_lc_state
    ldx #$00
    cmp #$FF ; return 16-bit signed number (X,A)
    bne @skip
    ldx #$FF
@skip:
    rts

; Convert text pointed to by a C string into a number and store into FAC.
; Returns 0 on success, non-zero on error.
_as_fp_str2fac:
    jsr _as_cache_str
    lda #<AS_SCR_STR
    sta AS_TXTPTR
    lda #>AS_SCR_STR
    sta AS_TXTPTR+1
    jsr _as_save_lc_state
    jsr _as_save_arg
    jsr AS_CHRGOT
    jsr AS_ADDR_FIN
    jsr _as_restore_arg
    jsr _as_restore_lc_state
    ldy #0
    lda (AS_TXTPTR),y ; are we at the end of the string???
    bne @notnumber
    ldx #$00
    txa
    rts
@notnumber:
    ldx #$ff
    txa
    rts

; Convert FAC to a temp string (stored at bottom of FBUFFER, aka hardware stack).
_as_fp_fac2str:
    jsr _as_save_lc_state
    jsr _as_save_fac
    jsr _as_save_arg
    jsr AS_ADDR_FOUT ; Y=MSB, A=LSB
    jsr _as_restore_arg
    jsr _as_restore_fac
    jsr _as_restore_lc_state
    pha
    tya ; Y = high
    tax
    pla
    ; On return:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    ; lda #<AS_FBUFFR
    ; ldx #>AS_FBUFFR
    rts

; Load ARG from memory (AS_FAC_FP struct)
_as_fp_mem2arg:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    jsr _as_cache_float  ; (X,A) -> AS_SCR_TMP
    jsr _as_save_lc_state
    lda #<AS_SCR_TMP
    ldy #>AS_SCR_TMP
    jsr AS_ADDR_CONUPK ; Y=MSB, A=LSB
    jmp _as_restore_lc_state

; Load FAC from memory (AS_FAC_FP struct)
_as_fp_mem2fac:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    jsr _as_cache_float  ; (X,A) -> AS_SCR_TMP
    jsr _as_save_lc_state
    lda #<AS_SCR_TMP
    ldy #>AS_SCR_TMP
    jsr AS_ADDR_MOVFM ; Y=MSB, A=LSB
    jmp _as_restore_lc_state

; Convert the FAC into memory (AS_FAC_FP struct)
_as_fp_fac2mem:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    sta AS_SCR_TMPA
    stx AS_SCR_TMPX
    jsr _as_save_lc_state
    ldx #<AS_SCR_TMP
    ldy #>AS_SCR_TMP
    jsr AS_ADDR_MOVMF ; Y=MSB, X=LSB
    jsr _as_restore_lc_state
    jmp _as_restore_float

; Swap the FAC and ARG
_as_fp_swap_fac_arg:
    ldx #5   ; FAC and ARG are 6 byte representations
@loop:
    lda AS_FAC,x
    ldy AS_ARG,x
    sta AS_ARG,x
    sty AS_FAC,x
    dex
    bpl @loop
    lda AS_FAC_EX
    ldy AS_ARG_EX
    sta AS_ARG_EX
    sty AS_FAC_EX
    rts

; Generally, this is not needed, but if the zero-page CHRGET code
; is not set up, this can be called to initialize it.
AS_CHRGET_ORIG = $F10B ; The 'template' CHRGET routine in Applesoft
AS_INIT_APPLESOFT = $E40C

_as_fp_init:
    jsr _as_save_lc_state
    jsr AS_INIT_APPLESOFT
    ; Set up the CHRGET routine from the template in ROM
    ldx #23 ; 24 bytes
cpy_loop:
    lda AS_CHRGET_ORIG,x
    sta AS_CHRGET,x
    dex
    bpl cpy_loop
    jmp _as_restore_lc_state

_as_fp_version:
    ldy        #0
@loop:
    lda AS_VERSION,y
    sta AS_FBUFFR,y
    iny
    cmp #0
    bne @loop
    ; On return:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    lda #<AS_FBUFFR
    ldx #>AS_FBUFFR
    rts

; c65 tends to enable the Apple II language-card bank 2 for extra RAM.
; We are calling ROM routines, so we need to save/restore the language-card state.
; TODO: should this lock out interrupts?

_as_save_lc_state:
    ; Save the language-card state and enable Applesoft ROM
    pha
    lda $c012
    sta AS_LC_STATE
    lda $c081
    lda $c081
    pla
    rts

_as_restore_lc_state:
    ; Restore the language-card state
    pha
    lda AS_LC_STATE
    bpl leave_rom_enabled
    lda $c083
    lda $c083
leave_rom_enabled:
    pla
    rts

; Save and restore FAC routines
_as_save_fac:
    pha
    ldx #5
@loop:
    lda AS_FAC,x
    sta AS_SCR_FAC,x
    dex
    bpl @loop
    lda AS_FAC_EX
    sta AS_SCR_FAC_EX
    pla
    rts

_as_restore_fac:
    pha
    ldx #5
@loop:
    lda AS_SCR_FAC,x
    sta AS_FAC,x
    dex
    bpl @loop
    lda AS_SCR_FAC_EX
    sta AS_FAC_EX
    pla
    rts

; Save and restore ARG routines
_as_save_arg:
    pha
    ldx #5
@loop:
    lda AS_ARG,x
    sta AS_SCR_ARG,x
    dex
    bpl @loop
    lda AS_ARG_EX
    sta AS_SCR_ARG_EX
    pla
    rts

_as_restore_arg:
    pha
    ldx #5
@loop:
    lda AS_SCR_ARG,x
    sta AS_ARG,x
    dex
    bpl @loop
    lda AS_SCR_ARG_EX
    sta AS_ARG_EX
    pla
    rts

; Copy data from a C pointer to a scratch location, either a float or a string.
; This works around RAM/language-card paging.
_as_cache_float:  ; Copy the 5 bytes from (X,A) to AS_SCR_TMP
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    sta AS_ZP
    stx AS_ZP+1
    ldy #4
@loop:
    lda (AS_ZP),y
    sta AS_SCR_TMP,y
    dey
    bpl @loop
    rts

_as_restore_float:  ; Copy the 5 bytes from AS_SCR_TMP to (AS_SCR_TMPX,AS_SCR_TMPA) 
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    lda AS_SCR_TMPA
    sta AS_ZP
    lda AS_SCR_TMPX
    sta AS_ZP+1
    ldy #4
@loop:
    lda AS_SCR_TMP,y
    sta (AS_ZP),y
    dey
    bpl @loop
    rts

; Cache a string representing a number in AS_SCR_STR, converting 
; lowercase 'e' to uppercase 'E' and terminating the string at
; a space character.
_as_cache_str:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    sta AS_ZP
    stx AS_ZP+1
    ldy #0
@loop:
    lda (AS_ZP),y
    and #$7F
    cmp #$65 ; lowercase 'e'
    bne @not_e
    lda #$45 ; uppercase 'E'
@not_e:
    cmp #$20 ; space
    bne @not_space
    lda #$00
@not_space:
    sta AS_SCR_STR,y
    iny
    cmp #0
    bne @loop
    rts

; Table of useful constants
_AS_CONST_one:
    .byte $81, $00, $00, $00, $00  ; 1.0 
_AS_CONST_half:
    .byte $80, $00, $00, $00, $00  ; 0.5
_AS_CONST_two:
    .byte $82, $00, $00, $00, $00  ; 2.0
_AS_CONST_ten:
    .byte $84, $20, $00, $00, $00  ; 10.0
_AS_CONST_sqrt_two:
    .byte $81, $35, $04, $F3, $00  ; 1.4142135623730951
_AS_CONST_e:
    .byte $82, $2D, $F8, $54, $58  ; 2.718281828459045
_AS_CONST_pi:
    .byte $82, $49, $0F, $DA, $A2  ; 3.141592653589793
_AS_CONST_two_pi:
    .byte $83, $49, $0F, $DA, $A2  ; 6.283185307179586
_AS_CONST_ln_two:
    .byte $80, $31, $72, $17, $F7  ; 0.6931471805599453

