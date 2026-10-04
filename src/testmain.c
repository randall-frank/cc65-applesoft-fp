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
    as_fp_init();

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

    printf("Applesoft Floating Point from C\n");
    printf("Copyright (C) 2026 Randall Frank\n");
    printf("Version: %s\n", as_fp_version());
    printf("---------- Simple Test Cases ----------\n\n");
}

void page3()
{
    title();

    as_fp_mem2fac(&AS_CONST_one);
    printf("Constant AS_CONST_one: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_half);
    printf("Constant AS_CONST_half: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_two);
    printf("Constant AS_CONST_two: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_ten);
    printf("Constant AS_CONST_ten: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_sqrt_two);
    printf("Constant AS_CONST_sqrt_two: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_e);
    printf("Constant AS_CONST_e: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_pi);
    printf("Constant AS_CONST_pi: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_two_pi);
    printf("Constant AS_CONST_two_pi: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_ln_two);
    printf("Constant AS_CONST_ln_two: %s\n", as_fp_fac2str());
}

void page2()
{
    AS_FAC_FP e, rad_45_deg;

    title();

    // Transcendentals
    // compute 45degrees in radians
    as_fp_mem2fac(&AS_CONST_two_pi);
    as_fp_swap_fac_arg();
    as_fp_str2fac("8.0");
    as_fp_arg_div_fac();
    as_fp_fac2mem(&rad_45_deg);
    printf("45 degrees in radians: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&rad_45_deg);
    as_fp_cos_fac();
    printf("cos(45 degrees): %s\n", as_fp_fac2str());
    as_fp_mem2fac(&rad_45_deg);
    as_fp_neg_fac();
    as_fp_sin_fac();
    printf("sin(-45 degrees): %s\n", as_fp_fac2str());
    as_fp_mem2fac(&rad_45_deg);
    as_fp_tan_fac();
    printf("tan(45 degrees): %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_one);
    as_fp_atn_fac();
    printf("arctan(1.0): %s\n", as_fp_fac2str());

    // Other functions add, subtract, sqrt, abs
    as_fp_mem2fac(&AS_CONST_one);
    as_fp_exp_fac();
    as_fp_fac2mem(&e);
    printf("exp(1.0): %s\n", as_fp_fac2str());
    as_fp_int_fac();
    printf("int(e): %s\n", as_fp_fac2str());
    as_fp_mem2fac(&e);
    as_fp_log_fac();
    printf("log(e): %s\n", as_fp_fac2str());
    as_fp_mem2fac(&e);
    as_fp_inv_fac();
    printf("1.0/e: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&e);
    as_fp_fac_mult_ten();
    printf("10*e: %s\n", as_fp_fac2str());
    as_fp_fac_div_ten();
    as_fp_fac_div_ten();
    printf("e/10: %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_ten);
    as_fp_sqr_fac();
    printf("sqrt(10.): %s\n", as_fp_fac2str());
    as_fp_mem2arg(&e);
    as_fp_mem2fac(&AS_CONST_ten);
    as_fp_arg_sub_fac();
    printf("e-10: %s\n", as_fp_fac2str());
    as_fp_abs_fac();
    printf("abs(e-10): %s\n", as_fp_fac2str());
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
    as_fp_mem2fac(&AS_CONST_two_pi);
    as_fp_mem2arg(&AS_CONST_half);
    as_fp_arg_mul_fac();
    printf("Computed PI: %s\n", as_fp_fac2str());

    as_fp_str2fac("1.234e-5");
    printf("Parsed from '1.234e-5': %s\n", as_fp_fac2str());

    err = as_fp_str2fac("Invalid");
    if (err) printf("'Invalid' correctly returned an error.\n");
    err = as_fp_str2fac("2.0");
    if (err) printf("Error parsing: '2.0' : %d\n", err);
    as_fp_mem2arg(&ram_half); 
    as_fp_swap_fac_arg();
    as_fp_arg_pow_fac();      
    printf("Computed pow(2.0, 0.5): %s\n", as_fp_fac2str());
    as_fp_mem2fac(&AS_CONST_sqrt_two);
    printf("Const sqrt(2): %s\n", as_fp_fac2str());

    // to/from ram
    as_fp_str2fac("0.4");
    as_fp_fac2mem(&ram_temp);
    as_fp_mem2fac(&ram_temp);
    printf("Check on FAC (0.4): %s\n", as_fp_fac2str());
    
    // Comparison
    as_fp_mem2fac(&ram_temp);
    cmp = as_fp_fac_cmp_mem(&ram_half);
    printf("Compare 0.4 (FAC) to 0.5 (mem): %d\n", cmp);
    as_fp_mem2fac(&AS_CONST_half);
    cmp = as_fp_fac_cmp_mem(&ram_half);
    printf("Compare 0.5 (FAC) to 0.5 (mem): %d\n", cmp);

    // 'sign' functions
    as_fp_str2fac("-1002.4");
    sgn = as_fp_sgn();
    printf("C sign check: %d (%s)\n", sgn, as_fp_fac2str());
    as_fp_sgn_fac();
    printf("FAC sign check: %s\n", as_fp_fac2str());

    // Alternative FAC load routines
    as_fp_char2fac(0xff);
    printf("Signed byte load: %s\n", as_fp_fac2str());
    as_fp_uchar2fac(0xff);
    printf("Unsigned byte load: %s\n", as_fp_fac2str());
    as_fp_int2fac(-30000);
    printf("Int load: %s\n", as_fp_fac2str());

    // Random number (Note: first number in the sequence)
    as_fp_rnd_fac();
    printf("Random number: %s\n", as_fp_fac2str());

    // Copy arg/fac
    as_fp_mem2fac(&AS_CONST_one);
    as_fp_mem2arg(&AS_CONST_two);
    as_fp_arg2fac();
    printf("ARG(2)->FAC: %s\n", as_fp_fac2str());

    as_fp_mem2fac(&AS_CONST_one);
    as_fp_mem2arg(&AS_CONST_two);
    as_fp_fac2arg();
    as_fp_mem2fac(&AS_CONST_ten);
    as_fp_arg2fac();
    printf("FAC(1)->ARG->FAC: %s\n", as_fp_fac2str());

}
