#include "Buttons.h"

void ButtonsInit(void)
{
    // Set the pins connected to the buttons as inputs
    TRISF |= 0b1111; // Set pins RF0-RF3 as inputs
}

static uint8_t previousButtonState = 0;

uint8_t ButtonsCheckEvents(void)
{
    // Read the current state of the buttons
    uint8_t currentButtonState = (PORTF & 0b1111);
    
    // Calculate the button events
    uint8_t buttonEvents = BUTTON_EVENT_NONE;
    
    // Check if any button is pressed down
    if (currentButtonState != previousButtonState) {
        // Iterate through each button
        for (int i = 0; i < 4; i++) {
            // Check if the i-th button has changed state
            uint8_t mask = (1 << i);
            if ((currentButtonState & mask) != (previousButtonState & mask)) {
                // Determine the event type based on the current and previous state
                if (currentButtonState & mask) {
                    // Button is pressed down
                    buttonEvents |= (1 << (2 * i + 1)); // Set the corresponding down event bit
                } else {
                    // Button is released
                    buttonEvents |= (1 << (2 * i)); // Set the corresponding up event bit
                }
            }
        }
        // Update the previous button state
        previousButtonState = currentButtonState;
    }
    
    // Return the button events
    return buttonEvents;
}

