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
    int redLedVoltageMeasurement[NUMBER_OF_SAMPLES][NUM_RED_LED];
    int yellowLedVoltageMeasurement[NUMBER_OF_SAMPLES][NUM_YELLOW_LED];
    int blueLedVoltageMeasurement[NUMBER_OF_SAMPLES][NUM_BLUE_LED];
    int infraredLedVoltageMeasurement[NUMBER_OF_SAMPLES][NUM_IR_LED];

    //TODO next SW version
    int temperatureMeasured;
    int humidityMeasured;
    int atmosphericHighMeasuered;
    
    //Public Methods
    void macronutrientsMapping(void);

    private:
    //Private attributes
    double redAverageRead;
    double yellowAverageRead;
    double blueAverageRead;
    double infraRedAverageRead;
   
    // Private Methods 
    void LedMapping(int ledIndex);

    //TODO in next SW version
    void temperatureMapping(void);
    void humidityMapping(void);
    void atmosphericHighMapping(void);
};

#endif
// end SIGNALCONDITIONINGCLASS_H