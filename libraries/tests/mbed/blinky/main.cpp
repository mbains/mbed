
#include "mbed.h"
#include "hx711.h"
#include "x_utils.h"
#include "mfrc522.h"
#include <stdio.h>


//DigitalOut myled(PB_3);


Ticker ticker;

/**
 * 
 *  signals
 *  PA_10 -> RST
 *  PA_4 -> CS
 *  PB_3 -> SCK
 *  PB_4 -> MISO
 *  PB_5 -> MOSI
 * 
 */

int main()
{
    MFRC522 rfid(PA_4, PA_10);
    rfid.PCD_Init();

    while (1) {

        wait(0.1);
        //myled = 1;
        int present = rfid.PICC_IsNewCardPresent();
        printf("reading: %d \r\n", present);


        if (!present) {
            continue;
        }

        // Select one of the cards
        if (!rfid.PICC_ReadCardSerial()) {
            continue;
        }

        // Dump debug info about the card; PICC_HaltA() is automatically called
        rfid.PICC_DumpToSerial(&(rfid.uid));

        wait(0.1);
        //myled = 0;
    }
}
