#include "x_utils.h"
#include "mbed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void float_to_str(float val, char * buf)
{
    int d1 = val; // Get the integer part (678).
    float f2 = val - d1; // Get fractional part (0.01234567).
    int d2 = trunc(f2 * 10000); // Turn into integer (123).
    float f3 = f2 * 10000 - d2; // Get next fractional part (0.4567).
    int d3 = trunc(f3 * 10000); // Turn into integer (4567).

    sprintf(buf, "%d.%04d%04d", d1, d2, d3);
}

/*
 * convert double to string buf. 
 * @buf nominally a 30 byte wide buffer
 */
void double_to_str(double val, char * buf)
{
    long d1 = val; // Get the integer part (678).
    float f2 = val - d1; // Get fractional part (0.01234567).
    int d2 = trunc(f2 * 10000); // Turn into integer (123).
    float f3 = f2 * 10000 - d2; // Get next fractional part (0.4567).
    int d3 = trunc(f3 * 10000); // Turn into integer (4567).

    sprintf(buf, "%ld.%04d%04d", d1, d2, d3);
}

void test_float_to_str()
{
    DigitalOut myled(PB_3);
    int divisor = 0;
    int d_mod = 0;
    char str[20];
    float answer = 0;
    while (1) {
        myled = 1;
        d_mod = divisor % 10000;
        printf("1/%u = ", d_mod);
        if (d_mod) {
            answer = 1. / d_mod;
        } else {
            answer = 0;
        }
        float_to_str(answer, str);
        printf("<%20s>\r\n", str);
        divisor++;
        wait(0.01);
        myled = 0;
        wait(0.01);
    }
}

void test_large_val_to_str(Serial *device)
{
    char str[20];
    int mult = 1;
    float initial, i_large = 8342333;
    initial = i_large;
    //i_large = 30;
    while (mult < 25) {
        i_large += (initial);
        mult++;
        float_to_str(i_large, str);
        device->printf(" * %d ", mult);
        device->printf("answer = <%30s>\r\n", str);
        wait(0.01);
    }
}
