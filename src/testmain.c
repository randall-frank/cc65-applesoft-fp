/*
 * @file testmain.c
 * @details Simple example that exercises the Applesoft floating point library
 * routines.  It serves as a basic test case.
 *
 * @author Randall Frank
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <apple2.h>

#include "build/apple2_asfp.h"

void page1();
void page2();
void page3();
void keypress(const char *prompt);
void title();
void dump_fac_arg();
void dump_mem_flt(const AS_FAC_FP *flt);

int main(void)
{
    asfp_init();

    page1();
    keypress("Press any key to continue...");

    page2();
    keypress("Press any key to continue...");

    page3();
    keypress("Press any key to reboot...");

    // Exit does not always reset the language card/ROM state correctly.
    rebootafterexit();
}

void keypress(const char *prompt)
{
    /* Use conio for specific positioning (x, y) */
    gotoxy(0, 23);
    cprintf(prompt);
    cgetc();
}

void title()
{
    /* Clear the text screen */
    clrscr();

    printf("Applesoft Floating Point from C (cc65)\n");
    printf("Copyright (C) 2026 Randall Frank\n");
    printf("Version: %s\n", asfp_version());
    printf("---------- Simple Test Cases ----------\n\n");
}

void page3()
{
    title();

    asfp_mem2fac(&AS_CONST_one);
    printf("Constant AS_CONST_one: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_half);
    printf("Constant AS_CONST_half: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_two);
    printf("Constant AS_CONST_two: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_ten);
    printf("Constant AS_CONST_ten: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_sqrt_two);
    printf("Constant AS_CONST_sqrt_two: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_e);
    printf("Constant AS_CONST_e: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_pi);
    printf("Constant AS_CONST_pi: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_two_pi);
    printf("Constant AS_CONST_two_pi: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_ln_two);
    printf("Constant AS_CONST_ln_two: %s\n", asfp_fac2str());
}

void page2()
{
    AS_FAC_FP e, rad_45_deg;

    title();

    // Transcendentals
    // compute 45degrees in radians
    asfp_str2fac("8.0");
    asfp_mem_div_fac(&AS_CONST_two_pi);
    asfp_fac2mem(&rad_45_deg);
    printf("45 degrees in radians: %s\n", asfp_fac2str());

    asfp_mem2fac(&rad_45_deg);
    asfp_cos_fac();
    printf("cos(45 degrees): %s\n", asfp_fac2str());
    
    asfp_mem2fac(&rad_45_deg);
    asfp_neg_fac();
    asfp_sin_fac();
    printf("sin(-45 degrees): %s\n", asfp_fac2str());
    
    asfp_mem2fac(&rad_45_deg);
    asfp_tan_fac();
    printf("tan(45 degrees): %s\n", asfp_fac2str());
    
    asfp_mem2fac(&AS_CONST_one);
    asfp_atn_fac();
    printf("arctan(1.0): %s\n", asfp_fac2str());

    // Other functions add, subtract, sqrt, abs
    asfp_mem2fac(&AS_CONST_one);
    asfp_exp_fac();
    asfp_fac2mem(&e);
    printf("exp(1.0): %s\n", asfp_fac2str());
    asfp_int_fac();
    printf("int(e): %s\n", asfp_fac2str());
    asfp_mem2fac(&e);
    asfp_log_fac();
    printf("log(e): %s\n", asfp_fac2str());
    asfp_mem2fac(&e);
    asfp_inv_fac();
    printf("1.0/e: %s\n", asfp_fac2str());
    asfp_mem2fac(&e);
    asfp_fac_mult_ten();
    printf("10*e: %s\n", asfp_fac2str());
    asfp_fac_div_ten();
    asfp_fac_div_ten();
    printf("e/10: %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_ten);
    asfp_sqr_fac();
    printf("sqrt(10.): %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_ten);
    asfp_mem_sub_fac(&e);
    printf("e-10: %s\n", asfp_fac2str());
    asfp_abs_fac();
    printf("abs(e-10): %s\n", asfp_fac2str());
}

void dump_fac_arg() 
{
    int i;
    unsigned char *p = (unsigned char *)0x009d;
    printf("FAC:");
    for (i = 0; i < 6; i++) {
        printf("%02x", *p++);
    }
    printf(" ARG:");
    p = (unsigned char *)0x00a5;
    for (i = 0; i < 6; i++) {
        printf("%02x", *p++);
    }
    printf("\n");
}

void dump_mem_flt(const AS_FAC_FP *flt) 
{
    int i;
    unsigned char *p = (unsigned char *)flt;
    printf("Mem:");
    for (i = 0; i < 5; i++) {
        printf("%02x", *p++);
    }
    printf("\n");
}

void page1()
{
    int err, cmp, sgn;
    AS_FAC_FP ram_half = {0x80, {0x00, 0x00, 0x00, 0x00}}; /* 0.5 */
    AS_FAC_FP ram_temp;

    title();

    asfp_mem2fac(&AS_CONST_two_pi);
    asfp_mem_mul_fac(&AS_CONST_half);
    printf("Computed PI: %s\n", asfp_fac2str());

    asfp_str2fac("1.234e-5");
    printf("Parsed from '1.234e-5': %s\n", asfp_fac2str());

    err = asfp_str2fac("Invalid");
    if (err) printf("'Invalid' correctly returned an error.\n");
    err = asfp_str2fac("2.0");
    if (err) printf("Error parsing: '2.0' : %d\n", err);

    asfp_mem2fac(&ram_half);
    asfp_mem_pow_fac(&AS_CONST_two);      
    printf("Computed pow(2.0, 0.5): %s\n", asfp_fac2str());
    asfp_mem2fac(&AS_CONST_sqrt_two);
    printf("Const sqrt(2): %s\n", asfp_fac2str());
    
    // Comparison
    asfp_str2fac("0.4");
    cmp = asfp_fac_cmp_mem(&ram_half);
    printf("Compare 0.4 (FAC) to 0.5 (mem): %d\n", cmp);
    asfp_mem2fac(&AS_CONST_half);
    cmp = asfp_fac_cmp_mem(&ram_half);
    printf("Compare 0.5 (FAC) to 0.5 (mem): %d\n", cmp);

    // 'sign' functions
    asfp_str2fac("-1002.4");
    sgn = asfp_sgn();
    printf("C sign check: %d (%s)\n", sgn, asfp_fac2str());
    asfp_sgn_fac();
    printf("FAC sign check: %s\n", asfp_fac2str());

    // Alternative FAC load routines
    asfp_char2fac(0xff);
    printf("Signed byte load: %s\n", asfp_fac2str());
    asfp_uchar2fac(0xff);
    printf("Unsigned byte load: %s\n", asfp_fac2str());
    asfp_int2fac(-30000);
    printf("Int load: %s\n", asfp_fac2str());

    // Random number (Note: first number in the sequence)
    asfp_rnd_fac();
    printf("Random number: %s\n", asfp_fac2str());
}
