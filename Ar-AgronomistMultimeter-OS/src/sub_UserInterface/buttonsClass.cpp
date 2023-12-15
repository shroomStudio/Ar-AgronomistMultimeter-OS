#include "buttonsClass.h"

// Global Variables for this file
const uint8_t pinOkButton   = 8;
const uint8_t pinUpButton   = 7;
const uint8_t pinDownButton = 6;
const uint8_t pinBackButton = 5;
BUTTON_PRESSED currentButtonPressed = NO_BUTTON;
const uint8_t buttonPins[4] = {pinOkButton,pinUpButton,pinDownButton,pinBackButton};
volatile uint8_t buttonstates[4] = {LOW,LOW,LOW,LOW}; 
volatile uint8_t previousButtonstates[4] = {LOW,LOW,LOW,LOW}; 

buttonsClass::buttonsClass(){
    //Constructor class buttonsClass
   initialButtonsSetup();
}

buttonsClass:: ~buttonsClass(){
    //destructor class buttonsClass
}

    
 //Private Methods
void buttonsClass::initialButtonsSetup(void)
{
    // Initial declaration for all the pins used as push buttons
    pinMode(pinOkButton,   INPUT_PULLUP);
    pinMode(pinUpButton,   INPUT_PULLUP);
    pinMode(pinDownButton, INPUT_PULLUP);
    pinMode(pinBackButton, INPUT_PULLUP);

    buttonPressed();
    // Initialization of interruptions
    for (int  i = 0; i < 4; i++)
    {
        /* code */
        pinMode(buttonPins[i], INPUT_PULLUP);
        attachInterrupt(buttonPins[i],navigationButtons,CHANGE);
    }
}

BUTTON_PRESSED buttonsClass::buttonPressed(void)
{
    return currentButtonPressed;
}

void buttonsClass::navigationButtons(void)
{
    // Initialization of interruptions
    for (int  i = 0; i < 4; i++)
    {
        // Read state of current button
        buttonstates[i] = digitalRead(buttonPins[i]);
        if ( buttonstates[i] != previousButtonstates[i])
        {
            previousButtonstates[i] = buttonstates[i];

            //evaluation of the button pressed 
            if (buttonstates[i] == HIGH)
            {
               currentButtonPressed = NO_BUTTON;
            }
            else 
            {
                currentButtonPressed = ((BUTTON_PRESSED)buttonstates[i]);
            }
        }
    }
}

