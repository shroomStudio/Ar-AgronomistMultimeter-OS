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
    int whiteLedAverage;
    int redLedAverage;
    int yellowLedAverage;
    int blueLedAverage;
    int greenLedAverage;
    int whiteLedMeasurements [MAX_NUMBER_OF_SAMPLES];
    int redLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int yellowLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int blueLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int greenLedMeasurements[MAX_NUMBER_OF_SAMPLES];

    //TODO next SW version
    int temperatureMeasured;
    int humidityMeasured;
    int atmosphericHighMeasuered;
    
    //Public Methods
    void macronutrientsMapping(void);

    private:
    //Private attributes

    // Private Methods 
    void LedMapping(void);

    //TODO in next SW version
    void temperatureMapping(void);
    void humidityMapping(void);
    void atmosphericHighMapping(void);
};

#endif
// end SIGNALCONDITIONINGCLASS_H