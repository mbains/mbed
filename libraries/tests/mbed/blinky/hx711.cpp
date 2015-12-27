/* 
 * File:   hx711.cpp
 * Author: mbai
 * 
 * Created on December 3, 2015, 8:44 PM
 */

#include "hx711.h"
#define LOW  (0)
#define HIGH (1)

/*
 * TODO: see bitWrite Note
 */
HX711::HX711(PinName dout, PinName pd_sck, uint8_t gain)
    :PD_SCK(pd_sck),
     DOUT(dout)

{
    
    set_gain(gain);
}

HX711::~HX711()
{

}

bool HX711::is_ready()
{
    return  (DOUT == LOW);
}

void HX711::set_gain(uint8_t gain)
{
    switch (gain) {
        case 128: // channel A, gain factor 128
            GAIN = 1;
            break;
        case 64: // channel A, gain factor 64
            GAIN = 3;
            break;
        case 32: // channel B, gain factor 32
            GAIN = 2;
            break;
    }

    PD_SCK = LOW;
    read();
}

long HX711::read()
{
    // wait for the chip to become ready
    while (!is_ready());

    int8_t data[3];

    // pulse the clock pin 24 times to read the data
    for (int8_t j = 3; j--;) {
        for (char i = 8; i--;) {
            			PD_SCK = HIGH;
                        //TODO: This needs to represent bitWrite 
            			data[j] |= (DOUT & (1<<i));
            			PD_SCK = LOW;
        }
    }

    // set the channel and the gain factor for the next reading using the clock pin
    for (int i = 0; i < GAIN; i++) {
        		PD_SCK = HIGH;
        		PD_SCK = LOW;
    }

    data[2] ^= 0x80;

    return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8) | (uint32_t) data[0];
}

long HX711::read_average(int8_t times)
{
    long sum = 0;
    for (int8_t i = 0; i < times; i++) {
        sum += read();
    }
    return sum / times;
}

double HX711::get_value(int8_t times)
{
    return read_average(times) - OFFSET;
}

float HX711::get_units(int8_t times)
{
    return get_value(times) / SCALE;
}

void HX711::tare(int8_t times)
{
    double sum = read_average(times);
    set_offset(sum);
}

void HX711::set_scale(float scale)
{
    SCALE = scale;
}

void HX711::set_offset(long offset)
{
    OFFSET = offset;
}

void HX711::power_down()
{
    PD_SCK = LOW;
    PD_SCK = HIGH;	
}

void HX711::power_up()
{
    //	digitalWrite(PD_SCK, LOW);	
}

