#include "signalConditioningClass.h"

//Clases instances 
signalConditioningClass::signalConditioningClass(lcdDisplayClass &lcd, buttonsClass &buttons): 
                                                lcdConditioning(lcd),
                                                buttonsConditioning(buttons),
                                                NumberOfReadings(1)
{
    //Constructor signalConditioningClass

}

signalConditioningClass::~signalConditioningClass(){
    //Destrucctor signalConditioningClass

}

//Private Methods
void signalConditioningClass::macronutrientsMapping(void)
{
    lcdConditioning.metadataTodisplayFreeCursor("Mapping macronutrients",LEFT_ALIGNED_X,TOP_Y,true);
    //Serial.println("Data from sensing class");

    //displayAverageValuesAs7341();
    //displayAverageValuesAs726x();
    delay(1000);          
}


//TODO in next SW version
void signalConditioningClass::temperatureMapping(void)
{

}

void signalConditioningClass::humidityMapping(void)
{

}

void signalConditioningClass::atmosphericHighMapping(void)
{
    
}

void signalConditioningClass::displayAverageValuesAs7341()
{
    Serial.print("F1  415nm: ");
    raw_f1_415nm += raw_f1_415nm;
    raw_f1_415nm /= NumberOfReadings;
    Serial.println(raw_f1_415nm);
    
    Serial.print("F2  445nm: ");
    raw_f2_445nm += raw_f2_445nm;
    raw_f2_445nm /= NumberOfReadings;
    Serial.println(raw_f2_445nm);
    
    Serial.print("F3  480nm: ");
    raw_f3_480nm += raw_f3_480nm;
    raw_f3_480nm /= NumberOfReadings;
    Serial.println(raw_f3_480nm);

    Serial.print("F4  515nm: ");
    raw_f4_515nm += raw_f4_515nm;
    raw_f4_515nm /= NumberOfReadings;
    Serial.println(raw_f4_515nm);
    
    Serial.print("Clear 0: ");
    clear_0 += clear_0;
    clear_0 /= NumberOfReadings;
    Serial.println(clear_0);

    Serial.print("NIR 0: ");
    nir_0 += nir_0;
    nir_0 /= NumberOfReadings;
    Serial.println(nir_0);

    Serial.print("F5  555nm: ");
    raw_f5_555nm += raw_f5_555nm;
    raw_f5_555nm /= NumberOfReadings;
    Serial.println(raw_f5_555nm);

    Serial.print("F6  590nm: ");
    raw_f6_590nm += raw_f6_590nm;
    raw_f6_590nm /= NumberOfReadings;
    Serial.println(raw_f6_590nm);

    Serial.print("F7  630nm: ");
    raw_f7_630nm += raw_f7_630nm;
    raw_f7_630nm /= NumberOfReadings;
    Serial.println(raw_f7_630nm);

    Serial.print("F8  680nm: ");
    raw_f8_680nm += raw_f8_680nm;
    raw_f8_680nm /= NumberOfReadings;
    Serial.println(raw_f8_680nm);

    Serial.print("Clear: ");
    clear += clear;
    clear /= NumberOfReadings;
    Serial.println(clear);

    Serial.print("NIR: ");
    nir += nir;
    nir /= NumberOfReadings;
    Serial.println(nir);
    delay(1000); 
}

void signalConditioningClass::displayAverageValuesAs726x()
{
    Serial.println("Data from AS726x");
    Serial.print("Violet: ");
    as726x_violet += as726x_violet;
    as726x_violet /= NumberOfReadings;
    Serial.println(as726x_violet);

    Serial.print("Blue: ");
    as726x_blue += as726x_blue;
    as726x_blue /= NumberOfReadings;
    Serial.println(as726x_blue);
    
    Serial.print("Green: "); 
    as726x_green += as726x_green;
    as726x_green /= NumberOfReadings; 
    Serial.println(as726x_green);

    Serial.print("Yellow: ");
    as726x_yellow += as726x_yellow;
    as726x_yellow /= NumberOfReadings;
    Serial.println(as726x_yellow);

    Serial.print("Orange: ");
    as726x_orange += as726x_orange;
    as726x_orange /= NumberOfReadings;
    Serial.println(as726x_orange);

    Serial.print("Red: ");
    as726x_red += as726x_red;
    as726x_red /= NumberOfReadings;
    Serial.println(as726x_red);

    NumberOfReadings++;
}