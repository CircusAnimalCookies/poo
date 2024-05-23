// **** Include libraries here ****
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


// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
static struct Timer TimerA = {0, 2000}; // TimerA triggers an event every 2 seconds
static struct Timer TimerB = {0, 3000}; // TimerB triggers an event every 3 seconds
static struct Timer TimerC = {0, 5000}; // TimerC triggers an event every 5 seconds

// **** Declare function prototypes ****
void TimerInit(void);

int bounce_timers_run(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    // Initialize LEDs
    LEDS_INIT();

    // Initialize Timer
    TimerInit();

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to CRUZID's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);

    while (1) {
        // Do nothing here, LED toggling is handled in the timer interrupt
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
void __ISR(_TIMER_1_VECTOR, ipl4auto) BounceTimer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    
    // Update TimerA
    TimerA.timeRemaining--;

    // Check if TimerA event
    if (TimerA.timeRemaining <= 0) {
        TimerA.event = 1; // Set TimerA event flag
        TimerA.timeRemaining = 2000; // Reset TimerA countdown
    }
    
    // Update TimerB
    TimerB.timeRemaining--;

    // Check if TimerB event
    if (TimerB.timeRemaining <= 0) {
        TimerB.event = 1; // Set TimerB event flag
        TimerB.timeRemaining = 3000; // Reset TimerB countdown
    }

    // Update TimerC
    TimerC.timeRemaining--;

    // Check if TimerC event
    if (TimerC.timeRemaining <= 0) {
        TimerC.event = 1; // Set TimerC event flag
        TimerC.timeRemaining = 5000; // Reset TimerC countdown
    }
    
    // Poll TimerA event and toggle LED1 if it occurs
    if (TimerA.event) {
        LEDS_SET(LEDS_GET() ^ 0x01); // Toggle LED1 on TimerA event
        TimerA.event = 0; // Reset the event flag
    }

    // Poll TimerB event and toggle LED2 if it occurs
    if (TimerB.event) {
        LEDS_SET(LEDS_GET() ^ 0x02); // Toggle LED2 on TimerB event
        TimerB.event = 0; // Reset the event flag
    }

    // Poll TimerC event and toggle LED3 if it occurs
    if (TimerC.event) {
        LEDS_SET(LEDS_GET() ^ 0x03); // Toggle LED3 on TimerC event
        TimerC.event = 0; // Reset the event flag
    }
    
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
}

/**
 * Initialize Timer 1 to trigger interrupts every millisecond.
 */
void TimerInit(void)
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
