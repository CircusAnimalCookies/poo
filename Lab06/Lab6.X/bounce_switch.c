//**** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdint.h>

// CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"

// **** Define global, module-level, or external variables here ****

#define LEFT 0
#define RIGHT 1

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

static struct Timer timer = {0, 1000}; // Initialize with a default time remaining
static uint8_t direction = RIGHT; // Start by moving to the right
static uint8_t currentLED = 1; // Start with LED1

#define TRUE 1
#define FALSE 0

// Define constants for different speeds based on switch states
#define SPEED_FAST 200
#define SPEED_MEDIUM 500
#define SPEED_SLOW 1000

// **** Declare function prototypes ****
void TimerInit(void); // Corrected function name
uint16_t GetSwitchStates(void);

int bounce_switch_run(void)
{
    BOARD_Init();

    // Initialize LEDs
    LEDS_INIT();

    // Initialize Timer
    TimerInit(); // Corrected function name

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to CRUZID's lab6 part2 (bounce_switch).  Compiled on %s %s.\n", __TIME__, __DATE__);

    while (1) {
        // Poll the timer event
        if (timer.event) {
            // Update the speed based on switch states
            uint16_t switchStates = SWITCH_STATES();
            if (switchStates == 0xFF) {
                timer.timeRemaining = SPEED_SLOW;
            } else if (switchStates == 0x00) {
                timer.timeRemaining = SPEED_FAST;
            } else {
                timer.timeRemaining = SPEED_MEDIUM;
            }

            // Clear the current LED
            LEDS_SET(0);

            // Update the LED position based on direction
            if (direction == RIGHT) {
                currentLED <<= 1;
                if (currentLED == 0x80) {
                    direction = LEFT;
                }
            } else {
                currentLED >>= 1;
                if (currentLED == 0x01) {
                    direction = RIGHT;
                }
            }

            // Set the new LED
            LEDS_SET(currentLED);

            // Clear the event flag
            timer.event = FALSE;
        }
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) SwitchTimer1Handler(void) // Corrected function name
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    // Decrement timeRemaining for the timer
    timer.timeRemaining--;

    // Check if timer event
    if (timer.timeRemaining <= 0) {
        timer.event = TRUE; // Set timer event flag
        timer.timeRemaining = 1; // Keep the timer running, value will be updated in main
    }
}

/**
 * Initialize Timer 1 to trigger interrupts every millisecond.
 */
void TimerInit(void) // Corrected function name
{
    // Set up Timer 1
    T1CON = 0;
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 5000; // Set PR1 to 5000 to trigger interrupts every millisecond
    T1CONbits.ON = 1; // Turn on Timer 1

    // Set up Timer 1 interrupt
    IFS0bits.T1IF = 0; // Clear Timer 1 interrupt flag
    IPC1bits.T1IP = 4; // Set Timer 1 interrupt priority level to 4
    IPC1bits.T1IS = 0; // Set Timer 1 interrupt subpriority level to 0
    IEC0bits.T1IE = 1; // Enable Timer 1 interrupt
}

/**
 * Get the current state of the switches.
 * 
 * @return uint16_t: A bitmask representing the switch states.
 */
uint16_t GetSwitchStates(void)
{
    return SWITCH_STATES();
}
