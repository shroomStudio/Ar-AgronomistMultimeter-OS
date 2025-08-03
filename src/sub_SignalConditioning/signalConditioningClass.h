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
    uint16_t raw_f2_nitrogen;
    uint16_t raw_f3_nitrogen;
    uint16_t raw_f4_phosphorus;
    uint16_t raw_f5_phosphorus;
    uint16_t raw_f7_potassium;
    uint16_t raw_f8_potassium;
    uint32_t raw_timestamp; 

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