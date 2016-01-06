/* 
 * File:   hx711.h
 * Author: mbai
 *
 * Created on December 3, 2015, 8:44 PM
 */

#ifndef HX711_H
#define	HX711_H
#include <stdint.h>
#include "mbed.h"

class HX711 {
private:

    DigitalOut PD_SCK; // Power Down and Serial Clock Input Pin
    DigitalIn DOUT; // Serial Data Output Pin
    uint8_t GAIN; // amplification factor
    int OFFSET; // used for tare weight
    double SCALE; // used to return weight in grams, kg, ounces, whatever

public:
    // define clock and data pin, channel, and gain factor
    // channel selection is made by passing the appropriate gain: 128 or 64 for channel A, 32 for channel B
    // gain: 128 or 64 for channel A; channel B works with 32 gain factor only
    HX711(PinName dout, PinName pd_sck, uint8_t gain = 128);

    virtual ~HX711();

    // check if HX711 is ready
    // from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
    // input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
    bool is_ready();

    // set the gain factor; takes effect only after a call to read()
    // channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
    // depending on the parameter, the channel is also set to either A or B
    void set_gain(uint8_t gain = 128);

    // waits for the chip to be ready and returns a reading
    int read();

    // returns an average reading; times = how many times to read
    double read_average(int8_t times = 10);

    // returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
    double get_value(int8_t times = 1);

    // returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
    // times = how many readings to do
    double get_units(int8_t times = 1);

    // set the OFFSET value for tare weight; times = how many times to read the tare value
    void tare(int8_t times = 10);

    // set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
    void set_scale(double scale = 1.f);

    // set OFFSET, the value that's subtracted from the actual reading (tare weight)
    void set_offset(int offset = 0);

    // puts the chip into power down mode
    void power_down();

    // wakes up the chip after power down mode
    void power_up();
};




#endif	/* HX711_H */

