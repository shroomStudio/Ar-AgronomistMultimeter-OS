#ifndef SENSINGCLASS_H
#define SENSINGCLASS_H

#include <Arduino.h>

enum SENSING_PROCESS
{
    MACRONUTRIENT,
    TEMPERATURE,
    HUMIDITY,
    ATMOSPHERIC_PRESSURE
};
enum SENSOR_SERIAL
{
    TEMPERATURE_SERIAL,
    HUMIDITY_SERIAL,
    ATMOSPHERIC_PRESSURE_SERIAL
};

class sensingClass{
    public:
    sensingClass();
    ~sensingClass();
    //Public attributes
    String temperatureSensor;
    String atmosphericSensor;
    double pressureSensor;

    //Public Methods
    void sensingProcessSelected(SENSING_PROCESS process);

    private:
    //Private attributes
    int redLedPin;
    int yellowLedPin;
    int blueLedPin;
    int infraredLedPin;
    int photodiodePinOut;
    int photodiodePinIn;
    int serialTx;
    int serialRx;
    double serialSelector;

    // Private Methods
    void macronutrientSensingProcess();
    void temperatureSensingProcess();
    void humiditySensingProces();
    void atmosphericPressureSensingProcess();
    void serialMiltiplexor(SENSOR_SERIAL sensor);
};

#endif