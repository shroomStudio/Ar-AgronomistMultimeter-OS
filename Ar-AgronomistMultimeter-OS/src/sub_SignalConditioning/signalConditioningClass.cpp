#include "signalConditioningClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"


//Clases instances 
lcdDisplayClass lcdConditioning;
buttonsClass buttonsConditioning;

signalConditioningClass::signalConditioningClass(){
    //Constructor signalConditioningClass

}

signalConditioningClass::~signalConditioningClass(){
    //Destrucctor signalConditioningClass

}

//Public Methods

//Private Methods
void signalConditioningClass::macronutrientsMapping(void)
{
    lcdConditioning.metadataTodisplayFreeCursor("Processing readings lease ",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    redLedMapping();
    yellowLedMapping();
    blueLedMapping();
    infraredLedMapping();
    
}

void signalConditioningClass::redLedMapping(void)
{

}

void signalConditioningClass::yellowLedMapping(void)
{

}

void signalConditioningClass::blueLedMapping(void)
{

}

void signalConditioningClass::infraredLedMapping(void)
{

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
