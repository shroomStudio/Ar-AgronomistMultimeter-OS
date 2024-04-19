#include "buttonsClass.h"
#include "lcdDisplayClass.h"

// Global Variables for this file
const uint8_t pinOkButton   = 8;
const uint8_t pinUpButton   = 7;
const uint8_t pinDownButton = 6;
const uint8_t pinBackButton = 5;
BUTTON_PRESSED currentButtonPressed = NO_BUTTON;
const uint8_t buttonPins[4] = {pinOkButton,pinUpButton,pinDownButton,pinBackButton};
volatile uint8_t buttonstates[4] = {LOW,LOW,LOW,LOW}; 
volatile uint8_t previousButtonstates[4] = {LOW,LOW,LOW,LOW}; 

lcdDisplayClass lcdButtons;

buttonsClass::buttonsClass(){
    //Constructor class buttonsClass
}

buttonsClass:: ~buttonsClass(){
    //destructor class buttonsClass
}

    
 //Private Methods
void buttonsClass::initialButtonsSetup(void)
{
    lcdButtons.metadataTodisplayInLCD("Initial SetUpButtons",LEFT_ALIGNED_X,MIDDLE_Y,true);
    delay(2000);
    // Initial declaration for all the pins used as push buttons
    pinMode(pinOkButton,   INPUT_PULLUP);
    pinMode(pinUpButton,   INPUT_PULLUP);
    pinMode(pinDownButton, INPUT_PULLUP);
    pinMode(pinBackButton, INPUT_PULLUP);

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
                switch (i)
                {
                    case 0:
                        /* code */
                        currentButtonPressed = UP_BUTTON;
                    break;
                    case 1:
                        /* code */
                        currentButtonPressed = DOWN_BUTTON;
                    break;
                    case 2:
                        /* code */
                        currentButtonPressed = BACK_BUTTON;
                    break;
                    case 3:
                        /* code */
                        currentButtonPressed = OK_BUTTON;
                    break;
                    default:
                        currentButtonPressed = NO_BUTTON;
                    break;
                }
            }
        }
    }
}

