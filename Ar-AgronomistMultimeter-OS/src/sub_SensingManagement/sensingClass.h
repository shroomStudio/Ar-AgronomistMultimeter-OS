#ifndef SENSINGCLASS_H
#define SENSINGCLASS_H

#include <Arduino.h>
#include "sub_UserInterface/commonDataTypes.h"

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

class sensingClass{
    public:
    sensingClass();
    ~sensingClass();
    //Public attributes
    String temperatureSensor;
    String atmosphericSensor;
    double pressureSensor;

    //Public Methods
    void macronutrientSensingProcess();
    void temperatureSensingProcess();
    void humiditySensingProces();
    void atmosphericPressureSensingProcess();

    private:
    //Private attributes
    int serialTx;
    int serialRx;
    double serialSelector;

    // Private Methods
    void serialMiltiplexor(SENSOR_SERIAL sensor);
};

#endif