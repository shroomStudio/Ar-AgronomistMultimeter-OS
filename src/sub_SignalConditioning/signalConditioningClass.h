//here must be the method photodiode voltage
#ifndef SIGNALCONDITIONINGCLASS_H
#define SIGNALCONDITIONINGCLASS_H

#include <Arduino.h>

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