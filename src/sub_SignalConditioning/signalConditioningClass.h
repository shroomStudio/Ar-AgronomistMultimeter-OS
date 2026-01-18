// ShroomCorp
// signalConditioningClass.h
// Description
// Copyright

#ifndef SIGNALCONDITIONINGCLASS_H
#define SIGNALCONDITIONINGCLASS_H

#include <Arduino.h>
#include <sub_UserInterface/commonDataTypes.h>
#include <sub_UserInterface/lcdDisplayClass.h>
#include <sub_UserInterface/buttonsClass.h>

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

    uint16_t as726x_violet;
    uint16_t as726x_blue;
    uint16_t as726x_green;
    uint16_t as726x_yellow;
    uint16_t as726x_orange;
    uint16_t as726x_red;

    // Add AS7265x members
    uint16_t as7265x_410nm;
    uint16_t as7265x_435nm;
    uint16_t as7265x_460nm;
    uint16_t as7265x_485nm;
    uint16_t as7265x_510nm;
    uint16_t as7265x_535nm;
    uint16_t as7265x_560nm;
    uint16_t as7265x_585nm;
    uint16_t as7265x_610nm;
    uint16_t as7265x_645nm;
    uint16_t as7265x_680nm;
    uint16_t as7265x_705nm;
    uint16_t as7265x_730nm;
    uint16_t as7265x_760nm;
    uint16_t as7265x_810nm;
    uint16_t as7265x_860nm;
    uint16_t as7265x_900nm;
    uint16_t as7265x_940nm;
  
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
    void displayAverageValuesAs7341(void);
    void displayAverageValuesAs726x(void);
    lcdDisplayClass &lcdConditioning;
    buttonsClass &buttonsConditioning;
    uint8_t NumberOfReadings;

};

#endif
// end SIGNALCONDITIONINGCLASS_H

// ShroomCorp
// signalConditioningClass.h
// Description
// Copyright