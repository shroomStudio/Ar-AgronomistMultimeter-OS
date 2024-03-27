#include "sensingClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"

#define NUMBER_OF_SAMPLES 10
#define ELEMENT_TO_MEASURE 3
#define NUM_RED_LED        0
#define NUM_YELLOW_LED     1
#define NUM_BLUE_LED       2
#define NUM_IR_LED         3
#define MAX_NUMBER_OF_LED 5

//Clases instances 
lcdDisplayClass lcdSensing;
buttonsClass buttonsSensing;

//Local Methods declare here to avoid ungranted access
void nitrogenSensingProcess(void);
void phosphorusSensingProcess(void);
void potassiumSensingProcess(void);
void turnOffAllElements(void);
void sensingProcessTakeReadings(void);

//Local variables 
float nitrogenPhotodiodeRead   = 0.0; //Voltage read from photodiode in nitrogen sensing
float phosphorusPhotodiodeRead = 0.0; //Voltage read from photodiode in phosphorus sensing
float potassiumPhotodiodeRead  = 0.0; //Voltage read from potassium in nitrogen sensing
uint8_t redLedVoltageMeasurement[NUMBER_OF_SAMPLES][MAX_NUMBER_OF_LED]      = {0};
uint8_t yellowLedVoltageMeasurement[NUMBER_OF_SAMPLES][MAX_NUMBER_OF_LED]   = {0};
uint8_t blueLedVoltageMeasurement[NUMBER_OF_SAMPLES][MAX_NUMBER_OF_LED]     = {0};
uint8_t infraredLedVoltageMeasurement[NUMBER_OF_SAMPLES][MAX_NUMBER_OF_LED] = {0};


sensingClass::sensingClass(){
    // Constructor sensing class
    pinMode(photodiodeOutput,OUTPUT);
    pinMode(redLedPin,OUTPUT);
    pinMode(yellowLedPin,OUTPUT);
    pinMode(blueLedPin,OUTPUT);
    pinMode(infraredLedPin,OUTPUT);
}
sensingClass::~sensingClass(){
    //destructor sensing class
}

void sensingClass::macronutrientSensingProcess()
{/*
    bool sensingProcessFinished = false;

    while (buttonsSensing.buttonPressed() != OK_BUTTON)
    {
        lcdSensing.metadataTodisplayInLCD
        ("please get ready the sample, press OK to continue", LEFT_ALIGNED_X, MIDDLE_Y);
        delay(2000);
    }
    //TODO: get and Save date and time
    while (buttonsSensing.buttonPressed() != BACK_BUTTON || sensingProcessFinished)
    {
        
        lcdSensing.metadataTodisplayInLCD
        ("sensing in process, press back to abort", LEFT_ALIGNED_X, MIDDLE_Y);
        lcdSensing.metadataTodisplayInLCDAdvanceCursor
        ("...",LEFT_ALIGNED_X,TOP_Y,0,1);
        delay(2000);
        nitrogenSensingProcess();
        phosphorusSensingProcess();
        potassiumSensingProcess();

        sensingProcessFinished = true;
        
    }*/
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
void nitrogenSensingProcess(void)
{
    // const int photodiodeOutput = A1; 
    // Ensure all the leds and photodiode are off
    turnOffAllElements();
    //(analogRead(A0)*5)/1023.0
    sensingProcessTakeReadings();


    
}

void phosphorusSensingProcess(void)
{

}
void potassiumSensingProcess(void)
{

}
void turnOffAllElements(void)
{
    digitalWrite(photodiodeOutput, LOW);
    delay(200);
    digitalWrite(yellowLedPin, LOW);
    delay(200);
    digitalWrite(blueLedPin, LOW);
    delay(200);
    digitalWrite(infraredLedPin, LOW);
    delay(200);
    digitalWrite(redLedPin, LOW);
    delay(200);
}

void sensingProcessTakeReadings(void)
{
   
    //Turn On photodiode
    digitalWrite(photodiodeOutput, HIGH);
    delay(200);
    //turn on red LED diode
    digitalWrite(redLedPin, HIGH);
    delay(200);
    //reading values for Red LED
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        /* //Start reading photodiode */
        redLedVoltageMeasurement[i][NUM_RED_LED] =  analogRead(photodiodeInput);
        delay(200);
    }
    //Turn Off red LED
    digitalWrite(redLedPin, LOW);
    delay(200);

    //turn on yellow LED diode
    digitalWrite(yellowLedPin, HIGH);
    delay(200);
    //reading values for yellow LED
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        /* //Start reading photodiode */
        redLedVoltageMeasurement[i][NUM_YELLOW_LED] =  analogRead(photodiodeInput);
        delay(200);
    }
    //Turn Off yellow LED
    digitalWrite(yellowLedPin, LOW);
    delay(200);

    //turn on blue LED diode
    digitalWrite(blueLedPin, HIGH);
    delay(200);
    //reading values for blue LED
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        /* //Start reading photodiode */
        redLedVoltageMeasurement[i][NUM_BLUE_LED] =  analogRead(photodiodeInput);
        delay(200);
    }
    //Turn Off blue LED
    digitalWrite(blueLedPin, LOW);
    delay(200);

    //turn on IR LED diode
    digitalWrite(infraredLedPin, HIGH);
    delay(200);
    //reading values for IR LED
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        /* //Start reading photodiode */
        redLedVoltageMeasurement[i][NUM_IR_LED] =  analogRead(photodiodeInput);
        delay(200);
    }
    //Turn Off IR LED
    digitalWrite(infraredLedPin, LOW);
    delay(200);
}