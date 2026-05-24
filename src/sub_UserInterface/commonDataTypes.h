// ShroomCorp
// commonDataTypes.h
// Common denitinos used for all the files inside the USER interface 
// Copyright

#ifndef COMMONDATATYPES_H
#define COMMONDATATYPES_H

#include <Arduino.h>

/* PinOut is necesary to update it. 
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
PinA1   White LED
PinA2   Red LED
PinA3   Yellow LED
PinA4   Blue LED
PinA5   Green LED
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
#define NUMBER_OF_ITEMS_MAIN_MENU 5
#define MAX_NUMBER_OF_SAMPLES 40
#define MEASURES_PER_LED 10
#define N_MEASURE 0
#define K_MEASURE 1
#define P_MEASURE 2
#define MAX_NUMBER_OF_ELEMENTS 3
#define NITROGEN_MEASURE 0
#define ELEMENT_TO_MEASURE 3
// Common annode leds 
#define PIN_RED_LED        5
#define PIN_GREEN_LED      3
#define PIN_BLUE_LED       4
// Common cathode leds
#define PIN_YELLOW_LED     6
#define PIN_WHITE_LED      7
#define MAX_NUMBER_OF_LED  5
#define NUMBER_OF_READS_TO_TAKE 5

// Constant definitions 
const int pinDownButton = A0;
const int pinBackButton = A1;
const int pinUpButton   = A2;
const int pinOkButton   = A3;

// Enumerations 
typedef enum 
{
    UP_BUTTON   = 56,
    DOWN_BUTTON = 50,
    BACK_BUTTON = 55,
    OK_BUTTON   = 53,
    NO_BUTTON   = 255,
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

// ── AgM Inference Feature (ARD-01 to ARD-10) ─────────────────────────
// Calibration05 white-reference counts from AgM_MasterDataset_25-04-26.xlsx
// Sheet: Norm_Reflactance — protocol: PRESSED / 10MM / FIELD
// Channel order: 410,435,460,485,510,5351,560,585,610,645,680,705,730,760,810,860,900,940 nm
const uint16_t I_WHITE_REF[18] = {
    1897, 3705, 3536, 3678, 2220, 1532,   // 410–535 nm
    1900, 3709, 1900, 3541, 3707, 3683,   // 560–705 nm
    3541, 3684, 2221, 1534, 2222, 1534    // 730–940 nm
};

// Number of AS7265x reads to take per inference cycle (ARD-03)
const uint8_t READS_PER_CHANNEL = 10;

// Lamp warm-up duration: 10 minutes in milliseconds (ARD-08)
const unsigned long LAMP_WARMUP_MS = 600000UL;

// Serial frame strings
const char CALIB_MISSING_MSG[] = "ERR,CALIB_MISSING";   // ARD-07
const char LAMP_COLD_MSG[]     = "WARN,LAMP_COLD";       // ARD-08
const char ACK_M_MSG[]         = "ACK,M";                // ARD-09

#endif 
//end class COMMONDATATYPES_H

// ShroomCorp
// commonDataTypes.h
// Common denitinos used for all the files inside the USER interface 
// Copyright