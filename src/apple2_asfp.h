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
 *          values into the (zero page) accumulator (FAC).
 *
 * In more detail:
 *
 *  Byte 0: Exponent (8 bits)Uses an excess-128 (offset/biased) representation.
 *          An exponent value of $80 (128 decimal) represents \(2^0 = 1\).
 *          An exponent of $00 is reserved specifically to indicate a value of exactly zero (regardless of what is in the mantissa bytes).
 *
 *  Byte 1: Sign Bit & Mantissa High (8 bits)
 *          Bit 7 (MSB): This is the Sign bit of the number. Unlike modern IEEE-754, 0 means positive and 1 means negative.
 *          Bits 6–0: These form the highest part of the fractional mantissa.
 *
 *  Bytes 2, 3, 4: Remaining Mantissa (24 bits)
 *          These three bytes contain the lower, remaining fractional bits of the normalized mantissa
 *
 *  @note when unpacked into the FAC, the high bit from Byte 1 is copied into an extra byte.  
 *        Thus, the FAC representation of a float is 6 bytes long.
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
 * @brief A collection of useful numbers that are included in the package
 * @details These read-only constants may be passed to asfp_mem2fac() to initialize the FAC
 *          or passed to math functions like asfp_mem_mul_fac().  
 *
 * An example using two constants to compute PI:      
 * @code
   asfp_mem2fac(&AS_CONST_two_pi);   // FAC=2PI
   asfp_mem_mul_fac(&AS_CONST_half); // FAC=0.5*2PI
   printf("Computed PI: %s\n", asfp_fac2str());
 * @endcode
 *
 * @{
 */
 
extern const AS_FAC_FP AS_CONST_one;      ///< The number 1.0
extern const AS_FAC_FP AS_CONST_half;     ///< The number 0.5
extern const AS_FAC_FP AS_CONST_two;      ///< The number 2.0
extern const AS_FAC_FP AS_CONST_ten;      ///< The number 10.0
extern const AS_FAC_FP AS_CONST_sqrt_two; ///< The number 1.414213562373095 (sqrt(2))
extern const AS_FAC_FP AS_CONST_e;        ///< The number 2.718281828459045 (exp(1))
extern const AS_FAC_FP AS_CONST_pi;       ///< The number 3.141592653589793 (PI)
extern const AS_FAC_FP AS_CONST_two_pi;   ///< The number 6.283185307179586 (2*PI)
extern const AS_FAC_FP AS_CONST_ln_two;   ///< The number 0.693147180559945 (ln(2))
 

/** @} */

/**
 * @defgroup funcs Floating Point Math Functions
 * @brief Applesoft ROM floating point math routines for the Apple II.
 *
 * @details The system is designed around a virtual register located in zero page memory,
 * the FAC (Floating Accumulator) and a floating point representation in RAM.  Functions 
 * exist to copy the FAC to and from RAM (asfp_fac2mem(), asfp_mem2fac()).  Common
 * math functions can be performed directly on the FAC (e.g. sin, cos) or they can be
 * performed on the FAC and a number in RAM (e.g. asfp_mem_mul_fac()).
 * @{
 */

/**
 * @brief Add a value from memory to the FAC and leave the result in the FAC.
 * @details Implements the math operation: FAC=mem+FAC.
 * @param mem The value to be added to the FAC.
 */
extern void __fastcall__ asfp_mem_add_fac(const AS_FAC_FP* mem);

/**
 * @brief Subtract a value from memory from the FAC and leave the result in the FAC.
 * @details Implements the math operation: FAC=mem-FAC.
 * @param mem The value to subtract the FAC from.
 */
extern void __fastcall__ asfp_mem_sub_fac(const AS_FAC_FP* mem);

/**
 * @brief Multiply a value from memory by the FAC and leave the result in the FAC.
 * @details Implements the math operation: FAC=mem*FAC.
 *
 * Example:
 * @code
    asfp_mem2fac(&AS_CONST_two_pi);
    asfp_mem_mul_fac(&AS_CONST_half);
    printf("Computed PI: %s\n", asfp_fac2str());
 * @endcode
 * @param mem The value to multiplied to the FAC.
 */
extern void __fastcall__ asfp_mem_mul_fac(const AS_FAC_FP* mem);

/**
 * @brief Divide a value from memory by the FAC and leave the result in the FAC.
 * @details Implements the math operation: FAC=mem/FAC.
 * @param mem The value to be divided by the FAC.
 */
extern void __fastcall__ asfp_mem_div_fac(const AS_FAC_FP* mem);

/**
 * @brief Raise a value from memory to the power of the FAC and leave the result in the FAC.
 * @details Implements the math operation: FAC=mem^FAC.  For example
 *          mem ^ 0.5 is the square root of mem.
 *
 * Example:
 * @code
    asfp_str2fac("0.5");
    asfp_mem_pow_fac(&AS_CONST_two);      
    printf("Computed pow(2, 0.5): %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_sqrt_two);
 * @endcode
 * @param mem The value to raise to the power specified by the FAC.
 * 
 */
extern void __fastcall__ asfp_mem_pow_fac(const AS_FAC_FP* mem);

/**
 * @brief Replace FAC with its absolute value:  FAC=abs(FAC)
 */
extern void __fastcall__ asfp_abs_fac();

/**
 * @brief Convert FAC to an integer value in place:  FAC=int(FAC)
 */
extern void __fastcall__ asfp_int_fac();

/**
 * @brief Replace FAC with its square root:  FAC=sqrt(FAC)
 */
extern void __fastcall__ asfp_sqr_fac();

/**
 * @brief Replace FAC with the natural logarithm of FAC:  FAC=ln(FAC)
 */
extern void __fastcall__ asfp_log_fac();

/**
 * @brief Replace FAC with e raised to the power FAC:  FAC=exp(FAC)
 */
extern void __fastcall__ asfp_exp_fac();

/**
 * @brief Replace FAC with a random floating-point number in the range [0,1).
 */
extern void __fastcall__ asfp_rnd_fac();

/**
 * @brief Replace FAC with the cosine of FAC, measured in radians :  FAC=cos(FAC)
 */
extern void __fastcall__ asfp_cos_fac();

/**
 * @brief Replace FAC with the sine of FAC, measured in radians:  FAC=sin(FAC)
 */
extern void __fastcall__ asfp_sin_fac();

/**
 * @brief Replace FAC with the tangent of FAC, measured in radians:  FAC=tan(FAC)
 */
extern void __fastcall__ asfp_tan_fac();

/**
 * @brief Replace FAC with the arctangent of FAC:  FAC=atan(FAC)
 */
extern void __fastcall__ asfp_atn_fac();

/**
 * @brief Negate the sign of the current FAC:  FAC=-FAC
 */
extern void __fastcall__ asfp_neg_fac();

/**
 * @brief Replace FAC with its reciprocal:  FAC=1.0 / FAC
 */
extern void __fastcall__ asfp_inv_fac();

/**
 * @brief Set the FAC to the status of the sign of the FAC.  This Functions
 * set the FAC to the values:
 *  -  1 if FAC > 0
 *  -  0 if FAC == 0
 *  -  -1 if FAC < 0
 */
extern void __fastcall__ asfp_sgn_fac();

/**
 * @brief Multiply FAC by 10.0 and update the FAC to the new value.
 */
extern void __fastcall__ asfp_fac_mult_ten();

/**
 * @brief Divide FAC by 10.0 and update the FAC to the new value.
 */
extern void __fastcall__ asfp_fac_div_ten();

/**
 * @brief Compare value in memory against FAC.
 *
 * Example:
 * @code
    AS_FAC_FP ram_half = {0x80, {0x00, 0x00, 0x00, 0x00}}; 
    asfp_str2fac("0.4");
    cmp = asfp_fac_cmp_mem(&ram_half);
    printf("Compare 0.4 (FAC) to 0.5 (mem): %d\n", cmp);
 * @endcode
 * @param mem Pointer to a memory-resident Applesoft float to compare against FAC.
 * @return Negative, zero, or positive depending on the comparison between FAC and MEM 
 *  - -1 if FAC < MEM
 *  -  0 if FAC == MEM
 *  -  1 if FAC > MEM.
 * @note Equality comparison can be inaccurate due to the nature of floating-point arithmetic.
 */
extern int __fastcall__ asfp_fac_cmp_mem(const AS_FAC_FP* mem);

/**
 * @brief Return the sign of the current FAC value.
 * @return an integer related to the sign of the FAC
 *  -  1 if FAC > 0
 *  -  0 if FAC == 0
 *  -  -1 if FAC < 0
 */
extern int __fastcall__ asfp_sgn();

/* I/O */
/**
 * @brief Initialize the floating-point helper state needed by string conversion routines.
 * @note This is only required before using asfp_str2fac() or asfp_fac2str() and only if the
 *       zero page CHRGET routine is not enabled (e.g. if running w/o Applesoft initialized)
 */
extern void __fastcall__ asfp_init();

/**
 * @brief Return the current version of the library as a string.
 * @return Pointer to a const char * string null terminated buffer containing library 
 *         version number in the form "x.y.z"
 * @note This string is ephemeral and may be replaced on the next asfp_ library function.
 */
extern const char * __fastcall__ asfp_version();

/**
 * @brief Convert a null-terminated ASCII string into an Applesoft floating-point value in FAC.
 * @param str Pointer to the numeric string to parse.
 * @return 0 on success, non-zero on failure.
 * @note Simple error checking is performed.  There is a check to see if all of the characters
 *       to the first whitespace are consumed. It also supports both 'E' and 'e' unlike the ROM call. 
 */
extern int __fastcall__ asfp_str2fac(char *str);

/**
 * @brief Convert the current FAC value to its canonical string representation.
 * @return Pointer to a const char * string null terminated buffer containing the formatted number.
 * @note This string is ephemeral and may be replaced on the next asfp_ library function.
 */
extern const char * __fastcall__ asfp_fac2str();

/* data transfer */
/**
 * @brief Copy FAC to a memory-resident Applesoft float structure.
 * @param mem Destination storage for the FAC value.
 */
extern void __fastcall__ asfp_fac2mem(AS_FAC_FP* mem);

/**
 * @brief Load FAC from a memory-resident Applesoft float structure.
 * @param mem Source data to copy into FAC. 
 */
extern void __fastcall__ asfp_mem2fac(const AS_FAC_FP* mem);

/**
 * @brief Load a signed integer into FAC.
 * @details This function is used to load the FAC with an signed 16 bit integer value
 *         in the range [-32768, 32767].
 * @param v Signed integer value to load into the FAC.
 */
extern void __fastcall__ asfp_int2fac(unsigned int v);

/**
 * @brief Load a signed character value into FAC.
 * @details This function is used to load the FAC with an single byte signed value
 *         in the range [-128, 127].
 * @param v Signed byte value to load into the FAC.
 */
extern void __fastcall__ asfp_char2fac(signed char v);

/**
 * @brief Load an unsigned character value into FAC.
 * @details This function is used to load the FAC with an unsigned value
 *         in the range [0, 255].
 * @param v Unsigned byte value to load into the FAC.
 */
extern void __fastcall__ asfp_uchar2fac(unsigned char v);

/** @} */


#endif

