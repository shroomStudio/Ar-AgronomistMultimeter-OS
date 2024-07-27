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

    lcdConditioning.metadataTodisplayFreeCursor("Processing readings",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);

    for (int i = 0; i < MAX_NUMBER_OF_LED; i++)
    {
        LedMapping(i);
    }
    
}

void signalConditioningClass::LedMapping(int ledIndex)
{
    int sum = 0;

    Serial.begin(9600);

    switch (ledIndex)
    {
    case NUM_RED_LED:
        // Sum all the values in the array
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        sum += redLedVoltageMeasurement[i][NUM_RED_LED];

        //Printing the sensor read to grapth it before to clear the array 
        Serial.println(redLedVoltageMeasurement[i][NUM_RED_LED]);
        delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        redLedVoltageMeasurement[i][NUM_RED_LED] = {'\0'};
    }

    // Calculate the average
    redAverageRead = sum / NUMBER_OF_SAMPLES;
    sum = 0;
     
    break;
    case NUM_YELLOW_LED:
    // Sum all the values in the array
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        sum += yellowLedVoltageMeasurement[i][NUM_YELLOW_LED];
        //Printing the sensor read to grapth it before to clear the array 
        Serial.println(yellowLedVoltageMeasurement[i][NUM_YELLOW_LED]);
        delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        yellowLedVoltageMeasurement[i][NUM_YELLOW_LED] = {'\0'};
    }

    // Calculate the average
    yellowAverageRead = sum / NUMBER_OF_SAMPLES;
    sum = 0;
    break;
    case NUM_BLUE_LED:
    // Sum all the values in the array
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        sum += blueLedVoltageMeasurement[i][NUM_BLUE_LED];
        //Printing the sensor read to grapth it before to clear the array 
        Serial.println(blueLedVoltageMeasurement[i][NUM_BLUE_LED]);
        delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        blueLedVoltageMeasurement[i][NUM_BLUE_LED] = {'\0'};
    }

    // Calculate the average
    blueAverageRead = sum / NUMBER_OF_SAMPLES;
    sum = 0;
    break;
    case NUM_IR_LED:
        // Sum all the values in the array
    // Sum all the values in the array
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        sum += infraredLedVoltageMeasurement[i][NUM_IR_LED];
        //Printing the sensor read to grapth it before to clear the array 
        Serial.println(infraredLedVoltageMeasurement[i][NUM_IR_LED]);
        delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        infraredLedVoltageMeasurement[i][NUM_IR_LED] = {'\0'};
    }

    // Calculate the average
    infraRedAverageRead = sum / NUMBER_OF_SAMPLES;
    sum = 0;
    break;
    
    default:
        break;
    }   
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
