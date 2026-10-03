#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <apple2.h>

#include "build/apple2_asfp.h"

int main(void)
{
    int err, cmp, sgn;
    AS_FAC_FP ram_half = {0x80, {0x00, 0x00, 0x00, 0x00}}; /* 0.5 */
    AS_FAC_FP ram_temp;

    /* Clear the text screen */
    clrscr();

    printf("Applesoft Floating Point from C\n");
    printf("Copyright (C) 2026 Randall Frank\n");
    printf("Version: %s\n", as_fp_version());
    printf("---------- Simple Test Cases ----------\n\n");

    as_fp_init();

    as_fp_rom2fac(AS_CONST_two_PI);
    as_fp_rom2arg(AS_CONST_half);
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
    as_fp_rom2fac(AS_CONST_sqrt_two);
    printf("Const sqrt(2): %s\n", as_fp_fac2str());

    // too/from ram
    as_fp_str2fac("0.4");
    as_fp_fac2mem(&ram_temp);
    as_fp_mem2fac(&ram_temp);
    printf("Check on FAC (0.4): %s\n", as_fp_fac2str());
    
    // Comparision
    cmp = as_fp_fac_cmp_mem(&ram_half);
    printf("Compare 0.4 (FAC) to 0.5 (mem): %d\n", cmp);

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

    /* Use conio for specific positioning (x, y) */
    gotoxy(0, 23);
    cprintf("Press any key to exit...\n");
    cgetc();

    // Exit does not always reset the language card/ROM state correctly.
    rebootafterexit();
}
