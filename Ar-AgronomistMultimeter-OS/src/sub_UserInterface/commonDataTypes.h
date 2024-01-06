#ifndef COMMONDATATYPES_H
#define COMMONDATATYPES_H

#include <Arduino.h>

/*Common denitinos used for all the files inside the USER interface 
*/

/*
Pin13   clockLcdPin
Pin12   dinLcdPin
Pin11   dcLcdPin
Pin10   LcdPin 
Pin9    resetLcdPin
Pin9    lightLcdPin
Pin8    OK Push Button      Pull-Up Resistor
Pin7    Up Push Button      Pull-Up Resistor
Pin6    Down Push Button    Pull-Up Resistor
Pin5    Back Push Button    Pull-Up Resistor
Pin4    Reset Device Pin
Pin3    Activation Pin Relay Battery Charge
Pin2    Activation Pin Relay Off
*/


//Defines
#define MINIMUM_BATTERY_PERCENTAGE 2
#define ZERO 0
#define INDEX_PLUS_ONE 1
#define INDEX_LESS_ONE -1

//Enumerations 
typedef enum 
{
    UP_BUTTON = 0,
    DOWN_BUTTON,
    BACK_BUTTON,
    OK_BUTTON,
    NO_BUTTON = 15,
}BUTTON_PRESSED;
typedef enum 
{
    //Coordinates on the x-axis, can be between 0 and 83
    LEFT_ALIGNED_X = 0, 
    CENTERED_X = 20, 
    RIGHT_ALIGNED_X = 60
}COORDINATES_X_DISCPLAY_ENUM;
typedef enum 
{
    // Coordinates on the y-axis, can be between 0 and 5
    TOP_Y = 0, 
    MIDDLE_Y = 3, 
    BOTTOM_Y = 5
}COORDINATES_Y_DISCPLAY_ENUM;

enum TYPE_USB
{
    ONLY_CHARGER,
    SHARE_FILES,
    SHARE_FILES_CHARGER
};


#endif 
//end class COMMONDATATYPES_H