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
    int sum = 0;

    lcdConditioning.metadataTodisplayFreeCursor("Processing readings",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);

    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        sum += nitrogenMeasurements[i];

        //Printing the sensor read to grapth it before to clear the array 
        Serial.println(nitrogenMeasurements[i]);
        delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        nitrogenMeasurements[i] = {'\0'};
    }
    // Calculate the average
    nitrogenAverage = sum / MAX_NUMBER_OF_SAMPLES;

    //Printing the information to user
    lcdConditioning.metadataTodisplayFreeCursor("Nitrogen average: \t",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    lcdConditioning.intNumberTodisplayInLCD(nitrogenAverage,LEFT_ALIGNED_X,MIDDLE_Y,false);
    delay(1000);
    Serial.println("N average: ");
    Serial.print(nitrogenAverage);
    delay(100);

    //Clearing the temp variable sum to start the calculation of another element
    sum = 0;

    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        sum += potassiumMeasurements[i];

        //Printing the sensor read to grapth it before to clear the array 
        Serial.println(potassiumMeasurements[i]);
        delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        potassiumMeasurements[i] = {'\0'};
    }
    // Calculate the average
    potassiumAverage = sum / MAX_NUMBER_OF_SAMPLES;

    //Printing the information to user
    lcdConditioning.metadataTodisplayFreeCursor("Potassium average: \t",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    lcdConditioning.intNumberTodisplayInLCD(potassiumAverage,LEFT_ALIGNED_X,MIDDLE_Y,false);
    delay(1000);
    Serial.println("K average: ");
    Serial.print(potassiumAverage);
    delay(100);

    //Clearing the temp variable sum to start the calculation of another element
    sum = 0;

    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        sum += phosphorusMeasurement[i];

        //Printing the sensor read to grapth it before to clear the array 
        Serial.println(phosphorusMeasurement[i]);
        delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        phosphorusMeasurement[i] = {'\0'};
    }
    // Calculate the average
    phosphorusAverage = sum / MAX_NUMBER_OF_SAMPLES;

    //Printing the information to user
    lcdConditioning.metadataTodisplayFreeCursor("Phosphorus average: \t",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    lcdConditioning.intNumberTodisplayInLCD(phosphorusAverage,LEFT_ALIGNED_X,MIDDLE_Y,false);
    delay(1000);
    Serial.println("P average: ");
    Serial.print(phosphorusAverage);
    delay(100);
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
