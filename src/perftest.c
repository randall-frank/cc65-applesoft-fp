/* 
 * @file perftest.c
 * @details  Port to C of the Applesoft performance test: PERFTEST
 * A simple benchmark to test the speed of the Applesoft 
 * floating point routines by computing a logistic map
 * function.
 * 
 * @author Randall Frank
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <apple2.h>

#include "build/apple2_asfp.h"

int main(void)
{
    AS_FAC_FP x = {0x7F, {0x4C, 0xCC, 0xCC, 0xCC}}; /* 0.4 */
    const AS_FAC_FP scale = {0x83, {0x00, 0x00, 0x00, 0x00}}; /* 4.0 */
    int i;

    asfp_init();

    clrscr();
    printf("Logistic map simple FP benchmark\n");
    printf("Press any key to begin\n");
    cgetc();
    printf("Running test...\n");

    asfp_mem2fac(&x);
    for (i = 0; i < 1000; i++) {
        // X = 4.0 * X * (1.0 - X)
        asfp_neg_fac(); // compute term as (-X + 1.0)
        asfp_mem_add_fac(&AS_CONST_one);
        asfp_mem_mul_fac(&x);
        asfp_mem_mul_fac(&scale);
        asfp_fac2mem(&x); // store result back to x
        // printf("Step %d: X = %s\n", i, asfp_fac2str());
    }

    printf("Done!");
    cgetc();

    exit(0);
}
