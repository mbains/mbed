/* 
 * File:   utils.h
 * Author: mbai
 *
 * Created on December 29, 2015, 3:17 PM
 */

#ifndef UTILS_H
#define	UTILS_H
#include "mbed.h"

void float_to_str(float val, char * buf);
void double_to_str(double val, char * buf);
void test_large_val_to_str(Serial *device);

#endif	/* UTILS_H */

