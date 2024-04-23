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
PinA0   Photodiode Input
PinA1   Photoiode OutPut
PinA2   red Led Pin
PinA3   yellow Led Pin
PinA4   blue Led Pin
PinA5   infrared Led Pin
*/


//Defines
#define MINIMUM_BATTERY_PERCENTAGE 2
#define ZERO 0
#define INDEX_PLUS_ONE 1
#define INDEX_LESS_ONE -1
#define LCD_ROWS 60
#define LCD_COLS 5
#define MINIMUM_TO_SENSING 10
#define clockLcdPin 13
#define dinLcdPin   12
#define dcLcdPin    11
#define ceLcdPin    10
#define resetLcdPin 9
#define lightLcdPin resetLcdPin

//Enumerations 
typedef enum 
{
    UP_BUTTON = 56,
    DOWN_BUTTON = 50,
    BACK_BUTTON = 55,
    OK_BUTTON = 53,
    NO_BUTTON = 255,
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
    MIDDLE_Y = 2, 
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