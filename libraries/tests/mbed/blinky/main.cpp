
#include "mbed.h"
#include "hx711.h"

DigitalOut myled(PB_3);

Ticker ticker;


int main() {

    while(1) {
        myled = 1;
        wait(1);
        myled = 0;
        wait(0.2);
    }
}
