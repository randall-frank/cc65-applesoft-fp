
; Accumulator and Argument registers: 6 byte virtual registers
AS_FAC     = $9D  ; $9D-$A2
AS_ARG     = $A5  ; $A5-$AA
AS_LC_STATE = $FA

; Some operations return 16bit values via zero page memory locations
AS_TMP_L = $A0
AS_TMP_H = $A1

; The string function returns a zero terminated string located in stack memory
AS_FBUFFR  = $0100

; To work around language card issues, the library uses $200-$2ff to
; buffer input strings and AS_FAC_FP objects which could be located 
; in language card RAM.
AS_SCR_STR = $0200    ; 127(+1) byte string buffer
AS_SCR_FAC = $0280 ; 5 bytes
AS_SCR_TMP = $0285 ; 5 bytes

; Other temp zero page memory locations used by the various functions: 
; 93-97,98-9C,8A-8E,C9-CD

; Operation addresses (LDA AS_FAC first)
AS_ADDR_FADD    = $E7C1 ; FAC = ARG + FAC
AS_ADDR_FSUB    = $E7AA ; FAC = ARG - FAC
AS_ADDR_FMUL    = $E982 ; FAC = ARG × FAC
AS_ADDR_FDIV    = $EA69 ; FAC = ARG ÷ FAC
AS_ADDR_FPWRT   = $EE97 ; FAC = ARG ^ FAC
AS_ADDR_ABS     = $EBAF ; FAC = abs(FAC) Absolute value
AS_ADDR_SQR     = $EE8D ; FAC = sqrt(FAC) square root
AS_ADDR_LOG     = $E941 ; FAC = ln(FAC) Natural Logarithm
AS_ADDR_EXP     = $EF09 ; FAC = exp(FAC) Natural Exponential
AS_ADDR_RND     = $EFAE ; FAC = rnd(FAC) Random number
AS_ADDR_COS     = $EFEA ; FAC = cos(FAC) Cosine (input in radians)
AS_ADDR_SIN     = $EFF1 ; FAC = sin(FAC) Sine (input in radians)
AS_ADDR_TAN     = $F03A ; FAC = tan(FAC) Tangent (input in radians)
AS_ADDR_ATN     = $F09E ; FAC = atan(FAC) Arctangent (output in radians)
AS_ADDR_NEGOP   = $EED0 ; FAC = -FAC (Change Sign)
AS_ADDR_MUL10   = $EA39 ; FAC = FAC*10.
AS_ADDR_DIV10   = $EA55 ; FAC = FAC/10.

AS_ADDR_SGN     = $EB90 ; Same as AS_ADDR_SIGN, but store in FAC
AS_ADDR_SIGN    = $EB82 ; Set A from FAC: A=1 if FAC is positive, A=0 if FAC is zero, A=$FF if FAC is negative
AS_ADDR_INT     = $EC23 ; FAC = int(FAC)

; (h,l) register pair as pointer, usually Y=MSB, A=LSB
AS_ADDR_FCOMP   = $EBB2 ; compare FAC and number pointed to by Y,A.   A=1 if (Y,A) < FAC, A=0 if (Y,A) == FAC, A=FF if (Y,A) > FAC

; Data transfers
AS_ADDR_FLOAT   = $EB93 ; FAC = signed integer A
AS_ADDR_SNGFLT  = $E301 ; FAC = unsigned integer Y
AS_ADDR_GIVAYF  = $E2F2 ; FAC = value of 2-byte signed integer loaded in the Y and A registers (Y,A)
AS_ADDR_MOVFA   = $EB53 ; FAC = ARG  copy the ARG to FAC
AS_ADDR_MOVAF   = $EB63 ; ARG = FAC  copy the FAC to ARG
AS_ADDR_MOVMF   = $EB2B ; Pack and store FAC to RAM. X register (low byte) and Y register (high byte) point to a destination address in RAM (Y,X)
AS_ADDR_MOVFM   = $EAF9 ; Unpack and load FAC from RAM. 5 byte floating point number in memory pointed by (Y,A)
AS_ADDR_CONUPK  = $E9E3 ; Unpack and load ARG from RAM. 5 byte floating point number in memory pointed by (Y,A)

AS_ADDR_FOUT    = $ED34 ; Create null terminated string in AS_FBUFFR from FAC On exit, (Y,A) points to the string.  FAC is scrambled. 

AS_TXTPTR   = $B8   ; Zero page pointer to text string (2 bytes)
AS_CHRGET   = $B1   ; Zero page routine to get next character
AS_CHRGOT   = $B7   ; Zero page routine to re-reads current character into A
AS_ADDR_FIN = $EC4A ; ROM routine to parse string into FAC (use Apple II chars $30-$39+$2B+$2E+$2D+$05)


.export  _as_fp_init
.export  _as_fp_str2fac
.export  _as_fp_fac2str

.export  _as_fp_mem2arg
.export  _as_fp_mem2fac
.export  _as_fp_fac2mem
.export  _as_fp_swap_fac_arg
.export  _as_fp_fac2arg
.export  _as_fp_arg2fac

.export _as_fp_sgn
.export _as_fp_int2fac
.export _as_fp_char2fac
.export _as_fp_uchar2fac

.export  _as_fp_arg_add_fac
.export  _as_fp_arg_sub_fac
.export  _as_fp_arg_mul_fac
.export  _as_fp_arg_div_fac
.export  _as_fp_arg_pow_fac
.export  _as_fp_abs_fac
.export  _as_fp_int_fac
.export  _as_fp_sqr_fac
.export  _as_fp_log_fac
.export  _as_fp_exp_fac
.export  _as_fp_rnd_fac
.export  _as_fp_cos_fac
.export  _as_fp_sin_fac
.export  _as_fp_tan_fac
.export  _as_fp_atn_fac
.export  _as_fp_neg_fac
.export  _as_fp_inv_fac
.export  _as_fp_sgn_fac
.export  _as_fp_fac_mult_ten
.export  _as_fp_fac_div_ten
.export  _as_fp_arg_cmp_fac

.segment "CODE"

; FAC = ARG + FAC
_as_fp_arg_add_fac:
    jsr _as_save_lc_state 
    lda AS_FAC
    jsr AS_ADDR_FADD
    jmp _as_restore_lc_state

; FAC = ARG - FAC
_as_fp_arg_sub_fac:
    jsr _as_save_lc_state 
    lda AS_FAC
    jsr AS_ADDR_FSUB
    jmp _as_restore_lc_state

; FAC = ARG * FAC
_as_fp_arg_mul_fac:
    jsr _as_save_lc_state 
    lda AS_FAC
    jsr AS_ADDR_FMUL
    jmp _as_restore_lc_state

; FAC = ARG / FAC
_as_fp_arg_div_fac:
    jsr _as_save_lc_state 
    lda AS_FAC
    jsr AS_ADDR_FDIV
    jmp _as_restore_lc_state

; FAC = ARG ^ FAC
_as_fp_arg_pow_fac:
    jsr _as_save_lc_state 
    lda AS_FAC
    jsr AS_ADDR_FPWRT
    jmp _as_restore_lc_state

; FAC = abs(FAC)    
_as_fp_abs_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_ABS
    jmp _as_restore_lc_state

; FAC = int(FAC)    
_as_fp_int_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_INT
    jmp _as_restore_lc_state

; FAC = sqrt(FAC)  (square root)
_as_fp_sqr_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_SQR
    jmp _as_restore_lc_state

; FAC = log(FAC) (natural, base e, log)
_as_fp_log_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_LOG
    jmp _as_restore_lc_state

; FAC = exp(FAC) (e to the FAC power)
_as_fp_exp_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_EXP
    jmp _as_restore_lc_state

; FAC = random() (semi)random number
_as_fp_rnd_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_RND
    jmp _as_restore_lc_state

; FAC = cos(FAC) (radians)
_as_fp_cos_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_COS
    jmp _as_restore_lc_state

; FAC = sin(FAC) (radians)
_as_fp_sin_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_SIN
    jmp _as_restore_lc_state

; FAC = tan(FAC) (radians)
_as_fp_tan_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_TAN
    jmp _as_restore_lc_state

; FAC = arctan(FAC) (radians)
_as_fp_atn_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_ATN
    jmp _as_restore_lc_state

; FAC = -FAC
_as_fp_neg_fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_NEGOP
    jmp _as_restore_lc_state

; FAC = 1.0 / FAC
AS_CONST_ADDR_ONE = $E913 ; 1.0
_as_fp_inv_fac:
    jsr _as_save_lc_state 
    ldy #>AS_CONST_ADDR_ONE ; ARG = 1.0 by loading ARG from ROM
    lda #<AS_CONST_ADDR_ONE
    jsr AS_ADDR_CONUPK ; Y=MSB, A=LSB
    lda AS_FAC
    jsr AS_ADDR_FDIV   ; FAC = ARG / FAC
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

; returned int is the output of SGN(FAC)
_as_fp_sgn:
    jsr _as_save_lc_state
    jsr AS_ADDR_SIGN
    jsr _as_restore_lc_state
    ldx #$00
    cmp #0    ; return 16bit signed number (X,A)
    bpl @positive
    ldx #$FF
@positive:
    rts

; FAC = passed signed int value
_as_fp_int2fac:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    pha
    txa
    tay
    pla
    jsr _as_save_lc_state 
    jsr AS_ADDR_GIVAYF  ; Y,A = signed integer value
    jmp _as_restore_lc_state

; FAC = passed signed char value
_as_fp_char2fac:
    ; A = signed char
    jsr _as_save_lc_state 
    jsr AS_ADDR_FLOAT  ; A = signed integer value
    jmp _as_restore_lc_state

; FAC = passed unsigned char value
_as_fp_uchar2fac:
    ; A = unsigned char
    tay
    jsr _as_save_lc_state 
    jsr AS_ADDR_SNGFLT ; Y = unsigned integer value
    jmp _as_restore_lc_state


; returns the result of comparing a number in memory to the FAC as a signed char
_as_fp_arg_cmp_fac:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    pha
    txa
    tay
    pla
    jsr _as_save_lc_state
    jsr AS_ADDR_FCOMP ; compare FAC and number pointed to by Y,A.   A=1 if (Y,A)<FAC, A=0 if (Y,A)==FAC, A=FF if (Y,A)>FAC
    jsr _as_restore_lc_state
    ldx #$00
    cmp #0    ; return 16bit signed number (X,A)
    bpl @positive
    ldx #$FF
@positive:
    rts

; Convert text pointed to by a C string into a number and store into FAC
; TODO: currently errors are fatal.  Need to override Applesoft error handling.  
_as_fp_str2fac: 
    jsr _as_cache_str
    lda #<AS_SCR_STR
    sta AS_TXTPTR
    lda #>AS_SCR_STR
    sta AS_TXTPTR+1
    jsr _as_save_lc_state 
    jsr AS_CHRGOT
    jsr AS_ADDR_FIN
    jmp _as_restore_lc_state

; Convert FAC to a temp string (stored at bottom of FBUFFER aka hw stack)
_as_fp_fac2str:
    jsr _as_save_lc_state
    jsr _as_save_fac 
    jsr  AS_ADDR_FOUT ; Y=MSB, A=LSB
    jsr _as_restore_fac
    jsr _as_restore_lc_state
    pha
    tya      ; Y = high
    tax 
    pla
    ; on return:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    ; lda       #<AS_FBUFFR
    ; ldx       #>AS_FBUFFR
    rts

; Load ARG from memory (AS_FAC_FP struct)
_as_fp_mem2arg:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    pha
    txa
    tay
    pla
    jsr _as_save_lc_state  
    jsr AS_ADDR_CONUPK ; Y=MSB, A=LSB
    jmp _as_restore_lc_state

; Load FAC from memory (AS_FAC_FP struct)
_as_fp_mem2fac:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    pha
    txa
    tay
    pla
    jsr _as_save_lc_state    
    jsr AS_ADDR_MOVFM ; Y=MSB, A=LSB
    jmp _as_restore_lc_state

; Convert the FAC into memory (AS_FAC_FP struct)
_as_fp_fac2mem:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    pha
    txa
    tay
    pla
    tax
    jsr _as_save_lc_state
    jsr AS_ADDR_MOVMF ; Y=MSB, X=LSB
    jmp _as_restore_lc_state

; Swap the FAC and ARG
_as_fp_swap_fac_arg:
    ldx #4
swap_loop:
    lda AS_FAC,x
    ldy AS_ARG,x
    sta AS_ARG,x
    sty AS_FAC,x
    dex
    bpl swap_loop
    rts

; ARG = FAC
_as_fp_fac2arg:
    jsr _as_save_lc_state 
    jsr AS_ADDR_MOVAF
    jmp _as_restore_lc_state

; FAC = ARG
_as_fp_arg2fac:
    jsr _as_save_lc_state 
    jsr AS_ADDR_MOVFA
    jmp _as_restore_lc_state

; Generally, this is not needed, but if the zero page CHRGET code
; is not set up, this can be called to init it.
AS_CHRGET_ORIG = $F10B ; The 'template' chrget routine in Applesoft
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

; c65 tends to enable the Apple 2 language card, bank 2 for extra
; RAM. We are calling ROM routines, so we need to save/restore the 
; language card state.  TODO: should this lock out interrupts???

_as_save_lc_state:
    ; Save the language card state and enable Applesoft ROM
    pha
    lda $c012
    sta AS_LC_STATE
    lda $c081
    lda $c081
    pla
    rts

_as_restore_lc_state:
    ; restore the language card state
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
    ldx #4
@loop:
    lda AS_FAC,x
    sta AS_SCR_FAC,x
    dex
    bpl @loop
    pla
    rts

_as_restore_fac:
    pha
    ldx #4
@loop:
    lda AS_SCR_FAC,x
    sta AS_FAC,x
    dex
    bpl @loop
    pla
    rts

; Copy data from a C pointer to a scratch location, either a float or a string
; This works around RAM/RAM language card paging
_as_cache_float:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    sta AS_TMP_L
    stx AS_TMP_H
    ldy #5
@loop:
    lda (AS_TMP_L),y
    sta AS_SCR_TMP,y
    dey
    bpl @loop
    rts

_as_cache_str:
    ; A = low byte of the pointer (LSB)
    ; X = high byte of the pointer (MSB)
    sta AS_TMP_L
    stx AS_TMP_H
    ldy #0
@loop:
    lda (AS_TMP_L),y
    and #$7f
    sta AS_SCR_STR,y
    iny
    cmp #0
    bne @loop
    rts


; TODO: 
; as_fp_str2fac - done
; as_fp_arg_cmp_fac
; as_fp_fac2mem
; as_fp_mem2arg