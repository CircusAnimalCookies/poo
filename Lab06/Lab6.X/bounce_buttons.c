// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

// CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h" // Include Buttons library

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

// LED macros
#define LED1 LATGbits.LATG2
#define LED2 LATGbits.LATG3
#define LED3 LATGbits.LATG2
#define LED4 LATGbits.LATG2
#define LED5 LATGbits.LATG6
#define LED6 LATGbits.LATG7
#define LED7 LATDbits.LATD4
#define LED8 LATDbits.LATD5

// **** Define global, module-level, or external variables here ****
static uint8_t buttonEvents = BUTTON_EVENT_NONE; // Module-level variable to store button events

// **** Declare function prototypes ****

int bounce_buttons_run(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; // clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of 4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to CRUZID's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);

    // Initialize Buttons library
    ButtonsInit();

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) bounce_buttons_Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/

    // Check for button events and store the result
    buttonEvents = ButtonsCheckEvents();

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    
    // Toggle LEDs based on button events
    if (buttonEvents != BUTTON_EVENT_NONE) {
        // Check the state of switch 1
        if (SWITCH_STATES() & SWITCH_STATE_SW1) { // Switch 1 is down
            // Toggle LEDs based on button events
            if (buttonEvents & BUTTON_EVENT_1DOWN) {
                LED1 = !LED1;
                LED2 = !LED2;
            }
            if (buttonEvents & BUTTON_EVENT_2DOWN) {
                LED3 = !LED3;
                LED4 = !LED4;
            }
            if (buttonEvents & BUTTON_EVENT_3DOWN) {
                LED5 = !LED5;
                LED6 = !LED6;
            }
            if (buttonEvents & BUTTON_EVENT_4DOWN) {
                LED7 = !LED7;
                LED8 = !LED8;
            }
        } else { // Switch 1 is up
            // Toggle LEDs based on button events
            if (buttonEvents & BUTTON_EVENT_1UP) {
                LED1 = !LED1;
                LED2 = !LED2;
            }
            if (buttonEvents & BUTTON_EVENT_2UP) {
                LED3 = !LED3;
                LED4 = !LED4;
            }
            if (buttonEvents & BUTTON_EVENT_3UP) {
                LED5 = !LED5;
                LED6 = !LED6;
            }
            if (buttonEvents & BUTTON_EVENT_4UP) {
                LED7 = !LED7;
                LED8 = !LED8;
            }
        }
    }
}
