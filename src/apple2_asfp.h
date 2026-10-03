/**
 * @file apple2_asfp.h
 * @brief Header description of an assembly language glue interface to the Applesoft floating point routines
 */
 
 /* 
  * Copyright (C) 2026 Randall Frank
  * Released under the MIT OpenSource license.  See the file LICENSE for details.
  */

#ifndef _APPLE2ASFP_H
#define _APPLE2ASFP_H

/**
 * @brief The Applesoft 5 byte floating point memory representation 
 * @details This structure is how an Applesoft float is stored in RAM. 
 *          The core interface uses this representation for loading/saving
 *          values into the (zero page) accumulator (FAC) and argument (ARG).
 * In more detail:
 *
 *  Byte 0: Exponent (8 bits)Uses an excess-128 (offset/biased) representation.
 *          An exponent value of $80 (128 decimal) represents \(2^0 = 1\).
 *          An exponent of $00 is reserved specifically to indicate a value of exactly zero (regardless of what is in the mantissa bytes).
 *  Byte 1: Sign Bit & Mantissa High (8 bits)
 *          Bit 7 (MSB): This is the Sign bit of the number. Unlike modern IEEE-754, 0 means positive and 1 means negative.
 *          Bits 6–0: These form the highest part of the fractional mantissa.
 *  Bytes 2, 3, 4: Remaining Mantissa (24 bits)
 *          These three bytes contain the lower, remaining fractional bits of the normalized mantissa
 *
 *  @note when unpacked into the FAC, the high bit from Byte 1 is copied into an extra byte.  
 *        Thus, the FAC and ARG representations are 6 bytes long.
 *
 */
typedef struct {
    /** 
     * @brief The exponent stored offset by $80. 
     */
    unsigned char exponent;
    /** 
     * @brief The 4 byte (31 bit) mantissa.  
     * @details The field is 31 bits, normalized with hidden most-significant bit.  The mantissa is stored little endian.
     * @note The high bit of mantissa[0] is the sign bit (0=+, 1=-) 
     */   
    unsigned char mantissa[4];
} AS_FAC_FP;


/**
 * @defgroup consts Numeric Constants
 * @brief A collection of useful numbers that are included in the ROM
 * @details These read-only constants may be passed as_fp_rom2arg() or
 *          as_fp_rom2fac() to initialize the FAC or ARG registers.  
 *          An example using two constants to compute PI:
 *       
 * @code
   as_fp_rom2fac(AS_CONST_two_PI);  // FAC=2PI
   as_fp_rom2arg(AS_CONST_half);    // ARG=0.5
   as_fp_arg_mul_fac();             // FAC=0.5*2PI
   printf("Computed PI: %s\n", as_fp_fac2str());
 * @endcode
 *
 * @note These constants cannot be used with as_fp_mem2arg() or as_fp_mem2fac().
 *
 * @{
 */
const AS_FAC_FP *AS_CONST_quarter   = (AS_FAC_FP *)0xF070;  ///< The number 0.25
const AS_FAC_FP *AS_CONST_half      = (AS_FAC_FP *)0xEE64;  ///< The number 0.5
const AS_FAC_FP *AS_CONST_neghalf   = (AS_FAC_FP *)0xE937;  ///< The number -0.5
const AS_FAC_FP *AS_CONST_one       = (AS_FAC_FP *)0xE913;  ///< The number 1.0
const AS_FAC_FP *AS_CONST_ten       = (AS_FAC_FP *)0xEA50;  ///< The number 10.0
const AS_FAC_FP *AS_CONST_sqrt_half = (AS_FAC_FP *)0xE92D;  ///< The number 0.2236067 (sqrt(0.5))
const AS_FAC_FP *AS_CONST_sqrt_two  = (AS_FAC_FP *)0xE932;  ///< The number 1.4142135 (sqrt(2.0))
const AS_FAC_FP *AS_CONST_ln_two    = (AS_FAC_FP *)0xE93C;  ///< The number 0.6931471 (ln(2) natural log of 2.0)
const AS_FAC_FP *AS_CONST_log_2_e   = (AS_FAC_FP *)0xEEDB;  ///< The number 1.4426950 (log base 2 of e)
const AS_FAC_FP *AS_CONST_half_PI   = (AS_FAC_FP *)0xF063;  ///< The number 1.5707963 (PI*0.5)
const AS_FAC_FP *AS_CONST_two_PI    = (AS_FAC_FP *)0xF06B;  ///< The number 6.2831853 (PI*2.0)
/** @} */

/**
 * @defgroup funcs Floating Point Math Functions
 * @brief Applesoft ROM floating point math routines for the Apple II.
 *
 * @details The system is designed around a a pair of virtual registers located in zero page memory,
 * the FAC (Floating Accumulator) and ARG (Floating Argument).
 * There are routines to load data into AS_FAC or the AS_ARG and to perform math operations
 * on those registers.
 * @{
 */

 /* Operations */
/**
 * @brief Add ARG to FAC and leave the result in FAC.
 * @details Implements the math operation: FAC=ARG+FAC.
 */
extern void __fastcall__ as_fp_arg_add_fac();

/**
 * @brief Subtract FAC from ARG and leave the result in FAC.
 * @details Implements the math operation: FAC=ARG-FAC.
 */
extern void __fastcall__ as_fp_arg_sub_fac();

/**
 * @brief Multiply ARG by FAC and leave the result in FAC.
 * @details Implements the math operation: FAC=ARG*FAC.
 */
extern void __fastcall__ as_fp_arg_mul_fac();

/**
 * @brief Divide ARG by FAC and leave the result in FAC.
 * @details Implements the math operation: ARG=ARG/FAC.
 */
extern void __fastcall__ as_fp_arg_div_fac();

/**
 * @brief Raise ARG to the power FAC and leave the result in ARG.
 * @details Implements the math operation: FAC=ARG^FAC.  For example
 *          ARG ^ 0.5 is the square root of ARG.
* @code
    as_fp_str2fac("2.0");
    as_fp_mem2arg(&ram_half); 
    as_fp_swap_fac_arg();
    as_fp_arg_pow_fac();      
    printf("Computed pow(2, 0.5): %s\n", as_fp_fac2str());
    as_fp_rom2fac(AS_CONST_sqrt_two);
 * @endcode
 * 
 */
extern void __fastcall__ as_fp_arg_pow_fac();

/**
 * @brief Replace FAC with its absolute value:  FAC=abs(FAC)
 */
extern void __fastcall__ as_fp_abs_fac();

/**
 * @brief Convert FAC to an integer value in place:  FAC=int(FAC)
 */
extern void __fastcall__ as_fp_int_fac();

/**
 * @brief Replace FAC with its square root:  FAC=sqrt(FAC)
 */
extern void __fastcall__ as_fp_sqr_fac();

/**
 * @brief Replace FAC with the natural logarithm of FAC:  FAC=ln(FAC)
 */
extern void __fastcall__ as_fp_log_fac();

/**
 * @brief Replace FAC with e raised to the power FAC:  FAC=exp(FAC)
 */
extern void __fastcall__ as_fp_exp_fac();

/**
 * @brief Replace FAC with a random floating-point number in the range [0,1).
 */
extern void __fastcall__ as_fp_rnd_fac();

/**
 * @brief Replace FAC with the cosine of FAC, measured in radians :  FAC=cos(FAC)
 */
extern void __fastcall__ as_fp_cos_fac();

/**
 * @brief Replace FAC with the sine of FAC, measured in radians:  FAC=sin(FAC)
 */
extern void __fastcall__ as_fp_sin_fac();

/**
 * @brief Replace FAC with the tangent of FAC, measured in radians:  FAC=tan(FAC)
 */
extern void __fastcall__ as_fp_tan_fac();

/**
 * @brief Replace FAC with the arctangent of FAC:  FAC=atan(FAC)
 */
extern void __fastcall__ as_fp_atn_fac();

/**
 * @brief Negate the sign of the current FAC:  FAC=-FAC
 */
extern void __fastcall__ as_fp_neg_fac();

/**
 * @brief Replace FAC with its reciprocal:  FAC=1.0 / FAC
 */
extern void __fastcall__ as_fp_inv_fac();

/**
 * @brief Set the FAC to the status of the sign of the FAC.  This Functions
 * set the FAC to the values:
 *  -  1 if FAC > 0
 *  -  0 if FAC == 0
 *  -  -1 if FAC < 0
 */
extern void __fastcall__ as_fp_sgn_fac();

/**
 * @brief Multiply FAC by 10.0 and update the FAC to the new value.
 */
extern void __fastcall__ as_fp_fac_mult_ten();

/**
 * @brief Divide FAC by 10.0 and update the FAC to the new value.
 */
extern void __fastcall__ as_fp_fac_div_ten();

/**
 * @brief Compare value in memory against FAC.
 * @param mem Pointer to a memory-resident Applesoft float to compare against FAC.
 * @return Negative, zero, or positive depending on the comparison between FAC and MEM 
 *  - -1 if FAC < MEM
 *  -  0 if FAC == MEM
 *  -  1 if FAC > MEM.
 *
 * @note Equality comparison can be inaccurate due to the nature of floating-point arithmetic.
 */
extern int __fastcall__ as_fp_fac_cmp_mem(const AS_FAC_FP* mem);

/**
 * @brief Return the sign of the current FAC value.
 * @return an integer related to the sign of the FAC
 *  -  1 if FAC > 0
 *  -  0 if FAC == 0
 *  -  -1 if FAC < 0
 */
extern int __fastcall__ as_fp_sgn();

/* I/O */
/**
 * @brief Initialize the floating-point helper state needed by string conversion routines.
 * @note This is only required before using as_fp_str2fac() or as_fp_fac2str() and only if the
 *       zero page CHRGET routine is not enabled (e.g. if running w/o Applesoft initialized)
 */
extern void __fastcall__ as_fp_init();

/**
 * @brief Return the current version of the library as a string.
 * @return Pointer to a const char * string null terminated buffer containing library 
 *         version number in the form "x.y.z"
 * @note This string is ephemeral and may be replaced on the next as_fp_ library function.
 */
extern const char * __fastcall__ as_fp_version();

/**
 * @brief Convert a null-terminated ASCII string into an Applesoft floating-point value in FAC.
 * @param str Pointer to the numeric string to parse.
 * @return 0 on success, non-zero on failure.
 * @note Simple error checking is performed.  There is a check to see if all of the characters
 *       to the first whitespace are consumed. It also supports both 'E' and 'e' unlike the ROM call. 
 */
extern int __fastcall__ as_fp_str2fac(char *str);

/**
 * @brief Convert the current FAC value to its canonical string representation.
 * @return Pointer to a const char * string null terminated buffer containing the formatted number.
 * @note This string is ephemeral and may be replaced on the next as_fp_ library function.
 */
extern const char * __fastcall__ as_fp_fac2str();

/* data transfer */
/**
 * @brief Copy FAC to a memory-resident Applesoft float structure.
 * @param mem Destination storage for the FAC value.
 */
extern void __fastcall__ as_fp_fac2mem(AS_FAC_FP* mem);

/**
 * @brief Load FAC from a memory-resident Applesoft float structure.
 * @param mem Source data to copy into FAC. Note: AS_CONST_ values are not legal.
 */
extern void __fastcall__ as_fp_mem2fac(const AS_FAC_FP* mem);

/**
 * @brief Load FAC from a ROM-resident Applesoft float structure.
 * @param rom Source data to copy into FAC.  Valid values will be AS_CONST_* values.
 */
extern void __fastcall__ as_fp_rom2fac(const AS_FAC_FP* rom);

/**
 * @brief Load ARG from a memory-resident Applesoft float structure.
 * @param mem Source data to copy into ARG. Note: AS_CONST_ values are not legal.
 */
extern void __fastcall__ as_fp_mem2arg(const AS_FAC_FP* mem);

/**
 * @brief Load ARG from a ROM-resident Applesoft float structure.
 * @param rom Source data to copy into ARG. Valid values will be AS_CONST_* values.
 */
extern void __fastcall__ as_fp_rom2arg(const AS_FAC_FP* rom);

/**
 * @brief Swap the contents of FAC and ARG.
 * @details The values of the FAC and ARG registers will be swapped.  This can be
 * useful to avoid unnecessary register load/store operations, for example to meet
 * the ordering requirements of as_fp_arg_sub_fac() and as_fp_arg_div_fac(). 
 */
extern void __fastcall__ as_fp_swap_fac_arg();

/**
 * @brief Copy FAC to ARG.
 * @details After calling this function, the ARG register will have
 * the same value as is in the FAC register. 
 */
extern void __fastcall__ as_fp_fac2arg();

/**
 * @brief Copy ARG to FAC.
 * @details After calling this function, the FAC register will have
 * the same value as is in the ARG register. 
 */
extern void __fastcall__ as_fp_arg2fac();

/**
 * @brief Load a signed integer into FAC.
 * @details This function is used to load the FAC with an signed 16 bit integer value
 *         in the range [-32768, 32767].
 * @param v Signed integer value to load into the FAC.
 */
extern void __fastcall__ as_fp_int2fac(unsigned int v);

/**
 * @brief Load a signed character value into FAC.
 * @details This function is used to load the FAC with an single byte signed value
 *         in the range [-128, 127].
 * @param v Signed byte value to load into the FAC.
 */
extern void __fastcall__ as_fp_char2fac(signed char v);

/**
 * @brief Load an unsigned character value into FAC.
 * @details This function is used to load the FAC with an unsigned value
 *         in the range [0, 255].
 * @param v Unsigned byte value to load into the FAC.
 */
extern void __fastcall__ as_fp_uchar2fac(unsigned char v);

/** @} */


#endif

