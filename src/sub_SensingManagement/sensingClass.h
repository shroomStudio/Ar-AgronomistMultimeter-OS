#ifndef SENSINGCLASS_H
#define SENSINGCLASS_H


/* GY-276x
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
#include "sub_UserInterface/commonDataTypes.h"
#include "ExternalLibraries/Adafruit_AS7341.h"
#include "ExternalLibraries/ShroomCorp_AS7265x.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"
#include "sub_SignalConditioning/signalConditioningClass.h"

namespace SensingNamespace {
    typedef enum 
    {
        MACRONUTRIENT,
        TEMPERATURE,
        HUMIDITY,
        ATMOSPHERIC_PRESSURE
    }SENSING_PROCESS;

    typedef enum 
    {
        TEMPERATURE_SERIAL,
        HUMIDITY_SERIAL,
        ATMOSPHERIC_PRESSURE_SERIAL
    }SENSOR_SERIAL;
}



class sensingClass{
    public:
    sensingClass(lcdDisplayClass &lcd, 
                buttonsClass &buttons, 
                signalConditioningClass &conditioning);
    ~sensingClass();
    //Public attributes
    String temperatureSensor;
    String atmosphericSensor;
    double pressureSensor;

    //Public Methods
    void macronutrientSensingProcess();
    void temperatureSensingProcess();
    void humiditySensingProcess();
    void atmosphericPressureSensingProcess();

    private:
    //Private attributes
    int serialTx;
    int serialRx;
    double serialSelector;
    bool isAS7341Ready = false;  // Add status flag
    bool isAS7265xReady;  // Add status flag

    uint16_t as7341Readings[12] = {0};  // AS7341 has 12 channels
    uint16_t as7265xReadings[AS7265X_NUM_CHANNELS] = {0}; // Add buffer for AS7265x readings

    // Private Methods
    void initialSensingClassSetup();
    //Global File scope methods
    void sensingProcessTakeReadings(void);
    void sendingReadingsToConditioning(void);
    void as7341TakeReads(void);
    void as7265xTakeReads(void);

    lcdDisplayClass &lcdSensing;
    buttonsClass &buttonsSensing;
    signalConditioningClass &conditioningSensing;
    Adafruit_AS7341 as7341;
    Adafruit_AS7265x as7265x;
};

#endif