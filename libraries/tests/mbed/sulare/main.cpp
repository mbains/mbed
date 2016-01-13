#include "mbed.h"
// Serial TX & RX interrupt loopback test using formatted IO - sprintf and sscanf
// Connect TX to RX (p9 to p10)
// or can also use USB and type back in the number printed out in a terminal window
// Sends out ASCII numbers in a loop and reads them back
// If not the same number LED4 goes on
// LED1 and LED2 indicate RX and TX interrupt routine activity
// LED3 changing indicate main loop running
 
 
Serial device(PA_9, PA_10);  // tx, rx
// Can also use USB and type back in the number printed out in a terminal window
// Serial monitor_device(USBTX, USBRX);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
 
 
void Tx_interrupt();
void Rx_interrupt();
void send_line();
void read_line();
 
 
// Circular buffers for serial TX and RX data - used by interrupt routines
const int buffer_size = 255;
// might need to increase buffer size for high baud rates
char tx_buffer[buffer_size+1];
char rx_buffer[buffer_size+1];
// Circular buffer pointers
// volatile makes read-modify-write atomic 
volatile int tx_in=0;
volatile int tx_out=0;
volatile int rx_in=0;
volatile int rx_out=0;
// Line buffers for sprintf and sscanf
char tx_line[80];
char rx_line[80];
 
 
// main test program
int main() {
    int i=0;
    int rx_i=0;
    device.baud(9600);
 
// Setup a serial interrupt function to receive data
    device.attach(&Rx_interrupt, Serial::RxIrq);
// Setup a serial interrupt function to transmit data
    device.attach(&Tx_interrupt, Serial::TxIrq);
 
// Formatted IO test using send and receive serial interrupts
// with sprintf and sscanf
    while (1) {
// Loop to generate different test values - send value in hex, decimal, and octal and  then read back
        for (i=0; i<0xFFFF; i++) {
            led3=1;
// Print ASCII number to tx line buffer in hex
            sprintf(tx_line,"%x\r\n",i);
// Copy tx line buffer to large tx buffer for tx interrupt routine
            send_line();
// Print ASCII number to tx line buffer in decimal
            sprintf(tx_line,"%d\r\n",i);
// Copy tx line buffer to large tx buffer for tx interrupt routine
            send_line();
// Print ASCII number to tx line buffer in octal
            sprintf(tx_line,"%o\r\n",i);
// Copy tx line buffer to large tx buffer for tx interrupt routine
            send_line();
            led3=0;
 
// Read a line from the large rx buffer from rx interrupt routine
            read_line();
// Read ASCII number from rx line buffer
            sscanf(rx_line,"%x",&rx_i);
// Check that numbers are the same
            if (i != rx_i) led4=1;
// Read a line from the large rx buffer from rx interrupt routine
            read_line();
// Read ASCII number from rx line buffer
            sscanf(rx_line,"%d",&rx_i);
// Check that numbers are the same
            if (i != rx_i) led4=1;
// Read a line from the large rx buffer from rx interrupt routine
            read_line();
// Read ASCII number from rx line buffer
            sscanf(rx_line,"%o",&rx_i);
// Check that numbers are the same
            if (i != rx_i) led4=1;
        }
    }
}
 
 
// Copy tx line buffer to large tx buffer for tx interrupt routine
void send_line() {
    int i;
    char temp_char;
    bool empty;
    i = 0;
// Start Critical Section - don't interrupt while changing global buffer variables
    NVIC_DisableIRQ(USART1_IRQn);
    empty = (tx_in == tx_out);
    while ((i==0) || (tx_line[i-1] != '\n')) {
// Wait if buffer full
        if (((tx_in + 1) % buffer_size) == tx_out) {
// End Critical Section - need to let interrupt routine empty buffer by sending
            NVIC_EnableIRQ(USART1_IRQn);
            while (((tx_in + 1) % buffer_size) == tx_out) {
            }
// Start Critical Section - don't interrupt while changing global buffer variables
            NVIC_DisableIRQ(USART1_IRQn);
        }
        tx_buffer[tx_in] = tx_line[i];
        i++;
        tx_in = (tx_in + 1) % buffer_size;
    }
    if (device.writeable() && (empty)) {
        temp_char = tx_buffer[tx_out];
        tx_out = (tx_out + 1) % buffer_size;
// Send first character to start tx interrupts, if stopped
        device.putc(temp_char);
    }
// End Critical Section
    NVIC_EnableIRQ(USART1_IRQn);
    return;
}
 
 
// Read a line from the large rx buffer from rx interrupt routine
void read_line() {
    int i;
    i = 0;
// Start Critical Section - don't interrupt while changing global buffer variables
    NVIC_DisableIRQ(USART1_IRQn);
// Loop reading rx buffer characters until end of line character
    while ((i==0) || (rx_line[i-1] != '\r')) {
// Wait if buffer empty
        if (rx_in == rx_out) {
// End Critical Section - need to allow rx interrupt to get new characters for buffer
            NVIC_EnableIRQ(USART1_IRQn);
            while (rx_in == rx_out) {
            }
// Start Critical Section - don't interrupt while changing global buffer variables
            NVIC_DisableIRQ(USART1_IRQn);
        }
        rx_line[i] = rx_buffer[rx_out];
        i++;
        rx_out = (rx_out + 1) % buffer_size;
    }
// End Critical Section
    NVIC_EnableIRQ(USART1_IRQn);
    rx_line[i-1] = 0;
    return;
}
 
 
// Interupt Routine to read in data from serial port
void Rx_interrupt() {
    led1=1;
// Loop just in case more than one character is in UART's receive FIFO buffer
// Stop if buffer full
    while ((device.readable()) && (((rx_in + 1) % buffer_size) != rx_out)) {
        rx_buffer[rx_in] = device.getc();
// Uncomment to Echo to USB serial to watch data flow
//        monitor_device.putc(rx_buffer[rx_in]);
        rx_in = (rx_in + 1) % buffer_size;
    }
    led1=0;
    return;
}
 
 
// Interupt Routine to write out data to serial port
void Tx_interrupt() {
    led2=1;
// Loop to fill more than one character in UART's transmit FIFO buffer
// Stop if buffer empty
    while ((device.writeable()) && (tx_in != tx_out)) {
        device.putc(tx_buffer[tx_out]);
        tx_out = (tx_out + 1) % buffer_size;
    }
    led2=0;
    return;
}