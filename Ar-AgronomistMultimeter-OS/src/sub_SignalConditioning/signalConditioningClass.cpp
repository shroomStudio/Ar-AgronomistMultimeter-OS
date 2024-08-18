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
    long int sum = 0;

    whiteLedAverage = 0;
    redLedAverage = 0;
    yellowLedAverage = 0;
    blueLedAverage = 0;
    greenLedAverage = 0;


    lcdConditioning.metadataTodisplayFreeCursor("Processing readings",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);

    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        sum += whiteLedMeasurements[i];

        //Printing the sensor read to grapth it before to clear the array 
        //Serial.println(whiteLedMeasurements[i]);
        //delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        whiteLedMeasurements[i] = 0;
    }
    // Calculate the average
    //Serial.println("N sum: ");
    //Serial.println(sum);
    //delay(100);
    whiteLedAverage = static_cast<int>(sum / MAX_NUMBER_OF_SAMPLES);
    Serial.println("\n White average: ");
    Serial.print(whiteLedAverage);
    delay(100);
    //whiteLedAverage = 0;
    //Serial.println("N average: ");
    //Serial.print(whiteLedAverage);
    //delay(100);

    //Printing the information to user
    lcdConditioning.metadataTodisplayFreeCursor("White average: \t",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    lcdConditioning.intNumberTodisplayInLCD(whiteLedAverage,LEFT_ALIGNED_X,MIDDLE_Y,false);
    delay(1000);

    //Clearing the temp variable sum to start the calculation of another element
    sum = 0;

    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        sum += redLedMeasurements[i];

        //Printing the sensor read to grapth it before to clear the array 
        //Serial.println(redLedMeasurements[i]);
        //delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        redLedMeasurements[i] = 0;
    }
    //Serial.println("K sum: ");
    //Serial.println(sum);
    //delay(100);
    // Calculate the average
    redLedAverage = static_cast<int>(sum / MAX_NUMBER_OF_SAMPLES);
    Serial.println("\n Red average: ");
    Serial.print(redLedAverage);
    delay(100);

    //Printing the information to user
    lcdConditioning.metadataTodisplayFreeCursor("Red average: \t",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    lcdConditioning.intNumberTodisplayInLCD(redLedAverage,LEFT_ALIGNED_X,MIDDLE_Y,false);
    delay(1000);
    
    //Clearing the temp variable sum to start the calculation of another element
    sum = 0;

    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        sum += yellowLedMeasurements[i];

        //Printing the sensor read to grapth it before to clear the array 
        //Serial.println(yellowLedMeasurements[i]);
        //delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        yellowLedMeasurements[i] = 0;
    }
   // Serial.println("P sum: ");
   // Serial.println(sum);
   // delay(100);
    // Calculate the average
    yellowLedAverage = static_cast<int>(sum / MAX_NUMBER_OF_SAMPLES);
    Serial.println("\n Yellow average: ");
    Serial.print(yellowLedAverage);
    delay(100);

    //Printing the information to user
    lcdConditioning.metadataTodisplayFreeCursor("yellow average: \t",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    lcdConditioning.intNumberTodisplayInLCD(yellowLedAverage,LEFT_ALIGNED_X,MIDDLE_Y,false);
    delay(1000);

    //Clearing the temp variable sum to start the calculation of another element
    sum = 0;

    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        sum += blueLedMeasurements[i];

        //Printing the sensor read to grapth it before to clear the array 
        //Serial.println(yellowLedMeasurements[i]);
        //delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        blueLedMeasurements[i] = 0;
    }
   // Serial.println("P sum: ");
   // Serial.println(sum);
   // delay(100);
    // Calculate the average
    blueLedAverage = static_cast<int>(sum / MAX_NUMBER_OF_SAMPLES);
    Serial.println("\n Blue average: ");
    Serial.print(blueLedAverage);
    delay(100);

    //Printing the information to user
    lcdConditioning.metadataTodisplayFreeCursor("blue average: \t",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    lcdConditioning.intNumberTodisplayInLCD(blueLedAverage,LEFT_ALIGNED_X,MIDDLE_Y,false);
    delay(1000);

    //Clearing the temp variable sum to start the calculation of another element
    sum = 0;

    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        sum += greenLedMeasurements[i];

        //Printing the sensor read to grapth it before to clear the array 
        //Serial.println(yellowLedMeasurements[i]);
        //delay(100);
        //Cleaning the array to leave it ready beore the next sensing
        greenLedMeasurements[i] = 0;
    }
   // Serial.println("P sum: ");
   // Serial.println(sum);
   // delay(100);
    // Calculate the average
    greenLedAverage = static_cast<int>(sum / MAX_NUMBER_OF_SAMPLES);
    Serial.println("\n Green average: ");
    Serial.print(greenLedAverage);
    delay(100);

    //Printing the information to user
    lcdConditioning.metadataTodisplayFreeCursor("green average: \t",LEFT_ALIGNED_X,TOP_Y,true);
    delay(1000);
    lcdConditioning.intNumberTodisplayInLCD(greenLedAverage,LEFT_ALIGNED_X,MIDDLE_Y,false);
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
