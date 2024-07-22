//here must be the method photodiode voltage
#ifndef SIGNALCONDITIONINGCLASS_H
#define SIGNALCONDITIONINGCLASS_H

#include <Arduino.h>
#include "sub_UserInterface/commonDataTypes.h"

enum LED_COLOR
{
    RED_LED,
    YELLOW_LED,
    BLUE_LED,
    IR_LED
};

class signalConditioningClass{
    public:
    signalConditioningClass();
    ~signalConditioningClass();
    
    //Public attributes
    int phosphorusMeasured;
    int nitrogenMeasured;
    int potassiumMeasured;
    int temperatureMeasured;
    int humidityMeasured;
    int atmosphericHighMeasuered;
    int redLedVoltageMeasurement[NUMBER_OF_SAMPLES][NUM_RED_LED];
    int yellowLedVoltageMeasurement[NUMBER_OF_SAMPLES][NUM_YELLOW_LED];
    int blueLedVoltageMeasurement[NUMBER_OF_SAMPLES][NUM_BLUE_LED];
    int infraredLedVoltageMeasurement[NUMBER_OF_SAMPLES][NUM_IR_LED];

    //Public Methods
    void photodiodeVoltage( LED_COLOR led, double voltage);

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
    void macronutrientsMapping(LED_COLOR led, double voltage);
    void redLedMapping(double voltage);
    void yellowLedMapping(double voltage);
    void blueLedMapping(double voltage);
    void infraredLedMapping(double voltage);
    void temperatureMapping(String tValues);
    void humidityMapping(String hValues);
    void atmosphericHighMapping(String aValues);
};

#endif
// end SIGNALCONDITIONINGCLASS_H