// ShroomCorp sensingClass.h all the code here is property of ShroomCorp and cannot be used
// or distributed without written permission from ShroomCorp    

#ifndef SENSINGCLASS_H
#define SENSINGCLASS_H


/* Technical Notes GY-276x
VIN   - 3.3V or 5V input
GND   - Ground
SCL   - I2C Clock
SDA   - I2C Data
INT   - Interrupt (optional)
RST   - Reset (optional)
SET   - Mode select (I2C or UART)
GND   - (second ground pin)

Red: 620-700 nm  
Orange: 590-620 nm 
Yellow: 570-590 nm 
Green: 495-570 nm 
Blue: 450-495 nm 
White: While not a single wavelength, white LEDs are characterized by their color temperature, measured in Kelvin (K). Lower Kelvin values (e.g., 2700K) indicate warm white, while higher values (e.g., 6000K) indicate cool white. 
Other colors: Purple LEDs typically have wavelengths between 370-410 nm, and ultraviolet (UV) LEDs have wavelengths less than 380 nm. 
*/
#include <Arduino.h>
#include <sub_UserInterface/commonDataTypes.h>
#include <sub_UserInterface/lcdDisplayClass.h>
#include <sub_UserInterface/buttonsClass.h>
#include <sub_SignalConditioning/signalConditioningClass.h>
#include "ShroomCorp_AS7265x.h"

class sensingClass{
    public:
    sensingClass(lcdDisplayClass &lcd, 
                buttonsClass &buttons,
                signalConditioningClass &conditioning);
    ~sensingClass();

    //Public Methods
    void macronutrientSensingProcess();
    void inferenceProcess();  // ARD-02: triggered by serial cmd 'M'

// File-scope variable accessible from main.cpp for lamp start time (ARD-08)
extern unsigned long lampStartTime;

    private:
    //Private attributes
    bool isAS7265xReady;  // Add status flag
    uint16_t as7265xReadings[AS7265X_NUM_CHANNELS] = {0}; // Add buffer for AS7265x readings

    // Private Methods
    //Global File scope methods
    void sensingProcessTakeReadings(void);
    bool calibrationProcess(void);
    void as7265xTakeReads(void);

    lcdDisplayClass &lcdSensing;
    buttonsClass &buttonsSensing;
    signalConditioningClass &conditioningSensing;
    ShroomCorp_AS7265x as7265x;
};

#endif 

// ShroomCorp sensingClass.h all the code here is property of ShroomCorp and cannot be used
// or distributed without written permission from ShroomCorp 