// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

// CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries


// **** Set macros and preprocessor directives ****
#define WINDOW_SIZE 5

// **** Define global, module-level, or external variables here ****
typedef struct {
    uint8_t event;
    uint16_t voltage;
} AdcResult;

AdcResult adcResult;

// **** Declare function prototypes ****

int bounce_adc_run(void)
{
    BOARD_Init();
    OledInit();

    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to CRUZID's lab6 part3 (bounce_adc).  Compiled on %s %s.\n", __TIME__, __DATE__);

    // Initialize ADC result window center
    adcResult.voltage = 0;

    // Main loop
    while (1) {
        // Display the raw ADC reading on OLED
        OledClear(OLED_COLOR_BLACK);
        char adcReadingStr[20];
        sprintf(adcReadingStr, "Raw ADC: %d", adcResult.voltage);
        OledDrawString(adcReadingStr);
        
        // Calculate and display percentage
        float percentage = (float)adcResult.voltage / 1023 * 100;
        char percentageStr[20];
        sprintf(percentageStr, "Percentage: %.2f%%", percentage);
        OledDrawString(percentageStr);
        
        // Update OLED
        OledUpdate();
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 *
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    // Read the 8 buffered ADC values in the ADC1BUF0 through ADC1BUF7 SFRs and average all of them.
    uint32_t adcValue = 0;
for (int i = 0; i < 8; i++) {
    adcValue += ADC1BUF0 + i * 4; // Each ADC buffer element is 4 bytes apart
}
adcValue /= 8;

    // Check if the ADC reading exits the window
    if (adcValue < (adcResult.voltage - WINDOW_SIZE) || adcValue > (adcResult.voltage + WINDOW_SIZE)) {
        // Generate an ADC event
        adcResult.event = 1;
        // Update the window center
        adcResult.voltage = adcValue;
    }
}
