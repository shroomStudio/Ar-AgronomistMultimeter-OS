#include "sensingClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"
#include "sub_SignalConditioning/signalConditioningClass.h"


//Clases instances 
lcdDisplayClass lcdSensing;
buttonsClass buttonsSensing;
signalConditioningClass conditioningSensing;

//Global File scope Variables
    int whiteLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int blueLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int redLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int yellowLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int greenLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    
sensingClass::sensingClass(){
    // Constructor sensing class
    pinMode(redLedPin,OUTPUT);
    pinMode(yellowLedPin,OUTPUT);
    pinMode(blueLedPin,OUTPUT);
    pinMode(whiteLedPin,OUTPUT);
    pinMode(greenLedPin, OUTPUT);
}

sensingClass::~sensingClass(){
    //destructor sensing class
}

void sensingClass::macronutrientSensingProcess()
{
    bool sensingProcessFinished = false;

    while (buttonsSensing.buttonPressed() != OK_BUTTON)
    {
        lcdSensing.metadataTodisplayInLCD("please get ready the sample, press OK to continue", LEFT_ALIGNED_X, MIDDLE_Y,true);
        delay(2000);
        buttonsSensing.navigationButtons();
    }
    //TODO: get and Save date and time
    if (!sensingProcessFinished)
    {
        lcdSensing.metadataTodisplayInLCD("sensing in process, press back to abort... \n", LEFT_ALIGNED_X, MIDDLE_Y,true);
        delay(3000);
        sensingProcessTakeReadings();
        //Process of sensing finished values 
        sensingProcessFinished = true;  
    }

    if (true == sensingProcessFinished)
    {
    lcdSensing.metadataTodisplayFreeCursor("Readings Saved",LEFT_ALIGNED_X,TOP_Y,true);
    delay(2000);
    lcdSensing.metadataTodisplayFreeCursor("Sensing process finished \n",LEFT_ALIGNED_X,TOP_Y,false);
    delay(2000);

     //Sendind readings to conditioning class
    sensingProcessSendingReadingsToConditioning();
    }
    
}

void sensingClass::temperatureSensingProcess()
{

}
void sensingClass::humiditySensingProces()
{

}
void sensingClass::atmosphericPressureSensingProcess()
{

}
void sensingClass::serialMiltiplexor(SENSOR_SERIAL sensor)
{

}

void sensingClass::turnOnAllElements(void)
{
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(blueLedPin, HIGH);
    digitalWrite(whiteLedPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, HIGH);
    delay(200);
}

void sensingClass::turnOffAllElements(void)
{
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    digitalWrite(whiteLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    delay(200);
}

void sensingClass::sensingProcessTakeReadings(void)
{
    lcdSensing.metadataTodisplayFreeCursor("Sensing in Process",LEFT_ALIGNED_X,TOP_Y,true);
    delay(2000);

    //Before Any process turn on all the elements 
    turnOnAllElements();
    delay(2000);
    //Ensure all the leds and photodiode are off
    turnOffAllElements();

    //turn on white LED diode
    digitalWrite(whiteLedPin, HIGH);
    delay(200);
    //reading values for White LED
    for(int i=0; i < MAX_NUMBER_OF_SAMPLES; i++ )
    {
        /* //Start reading photodiode */
        whiteLedMeasurements[i] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD((whiteLedMeasurements[i]),LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off White LED
    digitalWrite(whiteLedPin, LOW);
    delay(200);
    
    //turn on Red LED diode
    digitalWrite(redLedPin, HIGH);
    delay(200);
    //reading values for Red LED
    for(int i=0; i < MAX_NUMBER_OF_SAMPLES; i++ )
    {
        /* //Start reading photodiode */
        redLedMeasurements[i] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD((redLedMeasurements[i]),LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off Red LED
    digitalWrite(redLedPin, LOW);
    delay(200);

    //turn on Yellow LED diode
    digitalWrite(yellowLedPin, HIGH);
    delay(200);
    //reading values for Yellow LED
    for(int i=0; i < MAX_NUMBER_OF_SAMPLES; i++ )
    {
        /* //Start reading photodiode */
        yellowLedMeasurements[i] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD(yellowLedMeasurements[i],LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off Yellow LED
    digitalWrite(yellowLedPin, LOW);
    delay(200);


    //turn on Blue LED diode
    digitalWrite(blueLedPin, HIGH);
    delay(200);
    //reading values for IR LED
   for(int i=0; i < MAX_NUMBER_OF_SAMPLES; i++ )
    {
        /* //Start reading photodiode */
        blueLedMeasurements[i] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD((blueLedMeasurements[i]),LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off Blue LED
    digitalWrite(blueLedPin, LOW);
    delay(200);

    //turn on Green LED diode
    digitalWrite(greenLedPin, HIGH);
    delay(200);

     for(int i=0; i < MAX_NUMBER_OF_SAMPLES; i++ )
    {
        /* //Start reading photodiode */
        greenLedMeasurements[i] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD((greenLedMeasurements[i]),LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off Green LED
    digitalWrite(greenLedPin, LOW);
    delay(200);
}

void sensingClass::sensingProcessSendingReadingsToConditioning(void)
{
    // Copy White led readings to condition class arrays
    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        conditioningSensing.whiteLedMeasurements[i] = whiteLedMeasurements[i];
        //Clearing global scope file array to be ready for next process 
        whiteLedMeasurements[i] = 0;
    }

    // Copy Red LEd readings to condition class arrays
    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        conditioningSensing.redLedMeasurements[i] = redLedMeasurements[i];
        //Clearing global scope file array to be ready for next process 
        redLedMeasurements[i] = 0;
    }

    // Copy Yellow Led readings to condition class arrays
    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        conditioningSensing.yellowLedMeasurements[i] = yellowLedMeasurements[i];
        //Clearing global scope file array to be ready for next process 
        yellowLedMeasurements[i] = 0;
    }

    // Copy Blue readings to condition class arrays
    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        conditioningSensing.blueLedMeasurements[i] = blueLedMeasurements[i];
        //Clearing global scope file array to be ready for next process 
        blueLedMeasurements[i] = 0;
    }

    // Copy Green readings to condition class arrays
    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        conditioningSensing.greenLedMeasurements[i] = greenLedMeasurements[i];
        //Clearing global scope file array to be ready for next process 
        greenLedMeasurements[i] = 0;
    }
    
    //Starting signal conditioning process
    conditioningSensing.macronutrientsMapping();
}