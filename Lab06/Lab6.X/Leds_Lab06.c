#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// Include the pins for the LEDS
#define LED1 LATDbits.LATD0
#define LED2 LATDbits.LATD1
#define LED3 LATDbits.LATD2
#define LED4 LATDbits.LATD3
#define LED5 LATDbits.LATD4
#define LED6 LATDbits.LATD5
#define LED7 LATDbits.LATD6
#define LED8 LATDbits.LATD7

void LEDS_INIT(void)
{
    // Set each LED pin to "output" mode
    TRISDbits.TRISD0 = 0; // Set pin RD0 as output
    TRISDbits.TRISD1 = 0; // Set pin RD1 as output
    TRISDbits.TRISD2 = 0; // Set pin RD2 as output
    TRISDbits.TRISD3 = 0; // Set pin RD3 as output
    TRISDbits.TRISD4 = 0; // Set pin RD4 as output
    TRISDbits.TRISD5 = 0; // Set pin RD5 as output
    TRISDbits.TRISD6 = 0; // Set pin RD6 as output
    TRISDbits.TRISD7 = 0; // Set pin RD7 as output

    // Set each LED pin's output value to 0 (low voltage)
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    LED8 = 0;
}

void LEDS_SET(char newPattern) {
    // Set each LED according to the corresponding bit in the newPattern
    LED1 = (newPattern >> 0) & 0x01;
    LED2 = (newPattern >> 1) & 0x01;
    LED3 = (newPattern >> 2) & 0x01;
    LED4 = (newPattern >> 3) & 0x01;
    LED5 = (newPattern >> 4) & 0x01;
    LED6 = (newPattern >> 5) & 0x01;
    LED7 = (newPattern >> 6) & 0x01;
    LED8 = (newPattern >> 7) & 0x01;
}

char LEDS_GET(void) {
    // Read the state of each LED and pack them into an 8-bit pattern
    char pattern = 0;
    pattern |= (LED1 << 0);
    pattern |= (LED2 << 1);
    pattern |= (LED3 << 2);
    pattern |= (LED4 << 3);
    pattern |= (LED5 << 4);
    pattern |= (LED6 << 5);
    pattern |= (LED7 << 6);
    pattern |= (LED8 << 7);
    return pattern;
}

int Leds_Lab06_run(void)
{
    // Initialize LEDS
    LEDS_INIT();
    
    // Test LEDS_SET() and LEDS_GET()
    LEDS_SET(0x55); // Set alternate LEDs
    printf("LEDS_GET(): 0x%02X\n", LEDS_GET()); // Should print 0x55
    
    return 0;
}
