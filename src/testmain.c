#include <stdio.h>
#include <conio.h>
#include <apple2.h>

#include "build/apple2_asfp.h"

int main(void)
{
    /* Clear the text screen */
    clrscr();

    printf("Applesoft Floating Point from C\n");
    printf("Copyright (c) 2026 Randall Frank\n");
    printf("Version: 0.0.1\n\n\n");

    as_fp_init();
    
    as_fp_mem2fac(AS_CONST_two_PI);
    as_fp_mem2arg(AS_CONST_half);
    as_fp_arg_mul_fac();
    printf("Computed PI: %s\n", as_fp_fac2str());

    as_fp_str2fac("1.234E-5");
    printf("Parsed from string: %s\n", as_fp_fac2str());

    /* Use conio for specific positioning (x, y) */
    gotoxy(0, 23);
    cprintf("Press any key to exit...\n");
    cgetc();

    return 0;
}
