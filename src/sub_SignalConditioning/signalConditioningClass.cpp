#include "signalConditioningClass.h"

//Clases instances 
signalConditioningClass::signalConditioningClass(lcdDisplayClass &lcd, buttonsClass &buttons): 
                                                lcdConditioning(lcd),
                                                buttonsConditioning(buttons)
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
    Serial.println("Data from sensing class");
    Serial.print("F1  415nm: ");
    Serial.println(raw_f1_415nm);
    Serial.print("F2  445nm: ");
    Serial.println(raw_f2_445nm);
    Serial.print("F3  480nm: ");
    Serial.println(raw_f3_480nm);
    Serial.print("F4  515nm: ");
    Serial.println(raw_f4_515nm);
    Serial.print("Clear 0: ");
    Serial.println(clear_0);
    Serial.print("NIR 0: ");
    Serial.println(nir_0);
    Serial.print("F5  555nm: ");
    Serial.println(raw_f5_555nm);
    Serial.print("F6  590nm: ");
    Serial.println(raw_f6_590nm);
    Serial.print("F7  630nm: ");
    Serial.println(raw_f7_630nm);
    Serial.print("F8  680nm: ");
    Serial.println(raw_f8_680nm);
    Serial.print("Clear: ");
    Serial.println(clear);
    Serial.print("NIR: ");
    Serial.println(nir);
    delay(1000); 
    
    Serial.println("Data from AS726x");
    Serial.print("Violet: ");
    Serial.println(as726x_violet);
    Serial.print("Blue: ");
    Serial.println(as726x_blue);
    Serial.print("Green: ");  
    Serial.println(as726x_green);
    Serial.print("Yellow: ");
    Serial.println(as726x_yellow);
    Serial.print("Orange: ");
    Serial.println(as726x_orange);
    Serial.print("Red: ");
    Serial.println(as726x_red);
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
