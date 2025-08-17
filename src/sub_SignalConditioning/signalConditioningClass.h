//here must be the method photodiode voltage
#ifndef SIGNALCONDITIONINGCLASS_H
#define SIGNALCONDITIONINGCLASS_H

#include <Arduino.h>
#include "sub_UserInterface/commonDataTypes.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"

enum LED_COLOR
{
    RED_LED,
    YELLOW_LED,
    BLUE_LED,
    IR_LED
};

class signalConditioningClass{
    public:
    signalConditioningClass(lcdDisplayClass &lcd, buttonsClass &buttons);  //Constructor with parameters
    ~signalConditioningClass();
    
    //Public attributes
    uint16_t raw_f1_415nm;
    uint16_t raw_f2_445nm;
    uint16_t raw_f3_480nm;
    uint16_t raw_f4_515nm;
    uint16_t clear_0;
    uint16_t nir_0;
    uint16_t raw_f5_555nm;
    uint16_t raw_f6_590nm;
    uint16_t raw_f7_630nm;
    uint16_t raw_f8_680nm;
    uint16_t clear;
    uint16_t nir;
  
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
    lcdDisplayClass &lcdConditioning;
    buttonsClass &buttonsConditioning;
};

#endif
// end SIGNALCONDITIONINGCLASS_H