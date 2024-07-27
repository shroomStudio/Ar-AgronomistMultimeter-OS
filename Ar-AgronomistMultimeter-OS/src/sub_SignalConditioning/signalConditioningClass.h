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
    int nitrogenMeasurements [MAX_NUMBER_OF_SAMPLES];
    int potassiumMeasurements[MAX_NUMBER_OF_SAMPLES];
    int phosphorusMeasurement[MAX_NUMBER_OF_SAMPLES];

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