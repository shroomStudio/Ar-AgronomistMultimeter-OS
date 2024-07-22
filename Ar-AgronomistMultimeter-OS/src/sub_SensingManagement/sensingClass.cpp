#include "sensingClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"
#include "sub_SignalConditioning/signalConditioningClass.h"


//Clases instances 
lcdDisplayClass lcdSensing;
buttonsClass buttonsSensing;
signalConditioningClass conditioningSensing;
//Global File scope Variables
    float nitrogenPhotodiodeRead   = 0.0; //Voltage read from photodiode in nitrogen sensing
    float phosphorusPhotodiodeRead = 0.0; //Voltage read from photodiode in phosphorus sensing
    float potassiumPhotodiodeRead  = 0.0; //Voltage read from potassium in nitrogen sensing
    int redLedVoltageMeasurement[NUMBER_OF_SAMPLES][MAX_NUMBER_OF_LED]      = {0};
    int yellowLedVoltageMeasurement[NUMBER_OF_SAMPLES][MAX_NUMBER_OF_LED]   = {0};
    int blueLedVoltageMeasurement[NUMBER_OF_SAMPLES][MAX_NUMBER_OF_LED]     = {0};
    int infraredLedVoltageMeasurement[NUMBER_OF_SAMPLES][MAX_NUMBER_OF_LED] = {0};
    //Global File scope methods
    void nitrogenSensingProcess(void);
    void phosphorusSensingProcess(void);
    void potassiumSensingProcess(void);
    void turnOffAllElements(void);
    void turnOnAllElements(void);
    void sensingProcessTakeReadings(void);
    void sensingProcessSendingReadingsToConditioning(void);

sensingClass::sensingClass(){
    // Constructor sensing class
    pinMode(redLedPin,OUTPUT);
    pinMode(yellowLedPin,OUTPUT);
    pinMode(blueLedPin,OUTPUT);
    pinMode(infraredLedPin,OUTPUT);

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
        delay(2000);
        
        nitrogenSensingProcess();
        phosphorusSensingProcess();
        potassiumSensingProcess();

        sensingProcessFinished = true;  
    }

    lcdSensing.metadataTodisplayFreeCursor("Sensing processs finished \n",LEFT_ALIGNED_X,TOP_Y,false);
    delay(2000);
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
    lcdSensing.metadataTodisplayFreeCursor("Nitrogren",LEFT_ALIGNED_X,TOP_Y,true);
    delay(2000);
    //Before Any process turn on all the elements 
    turnOnAllElements();
    delay(2000);
    //Ensure all the leds and photodiode are off
    turnOffAllElements();
    //Starting The sensing process
    sensingProcessTakeReadings();  
}

void phosphorusSensingProcess(void)
{
    lcdSensing.metadataTodisplayFreeCursor("Phosphorus",LEFT_ALIGNED_X,TOP_Y,true);
    delay(2000);
    //Before Any process turn on all the elements 
    turnOnAllElements();
    delay(2000);
    //Ensure all the leds and photodiode are off
    turnOffAllElements();
    //Starting The sensing process
    sensingProcessTakeReadings();  
}
void potassiumSensingProcess(void)
{
    lcdSensing.metadataTodisplayFreeCursor("Potassium",LEFT_ALIGNED_X,TOP_Y,true);
    delay(2000);
    //Before Any process turn on all the elements 
    turnOnAllElements();
    delay(2000);
    //Ensure all the leds and photodiode are off
    turnOffAllElements();
    //Starting The sensing process
    sensingProcessTakeReadings();  
}

void turnOnAllElements(void)
{
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(blueLedPin, HIGH);
    digitalWrite(infraredLedPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    delay(200);
}

void turnOffAllElements(void)
{
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    digitalWrite(infraredLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    delay(200);
}

void sensingProcessTakeReadings(void)
{
    //turn on red LED diode
    digitalWrite(redLedPin, HIGH);
    delay(200);
    //reading values for Red LED
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        /* //Start reading photodiode */
        redLedVoltageMeasurement[i][NUM_RED_LED] = analogRead(photodiodeInput);
        delay(700);
        lcdSensing.intNumberTodisplayInLCD(redLedVoltageMeasurement[i][NUM_RED_LED],LEFT_ALIGNED_X,TOP_Y,true);
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
        yellowLedVoltageMeasurement[i][NUM_YELLOW_LED] = analogRead(photodiodeInput);
        delay(700);
        lcdSensing.intNumberTodisplayInLCD(yellowLedVoltageMeasurement[i][NUM_YELLOW_LED],LEFT_ALIGNED_X,TOP_Y,true);
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
        blueLedVoltageMeasurement[i][NUM_BLUE_LED] = analogRead(photodiodeInput);
        delay(700);
        lcdSensing.intNumberTodisplayInLCD(blueLedVoltageMeasurement[i][NUM_BLUE_LED],LEFT_ALIGNED_X,TOP_Y,true);
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
        infraredLedVoltageMeasurement[i][NUM_IR_LED] = analogRead(photodiodeInput);
        delay(700);
        lcdSensing.intNumberTodisplayInLCD(infraredLedVoltageMeasurement[i][NUM_IR_LED],LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off IR LED
    digitalWrite(infraredLedPin, LOW);
    delay(200);

    //Sendind readings to conditioning class
    lcdSensing.metadataTodisplayFreeCursor("Saving readings",LEFT_ALIGNED_X,TOP_Y,true);
    sensingProcessSendingReadingsToConditioning();
    lcdSensing.metadataTodisplayFreeCursor("Readings Saved",LEFT_ALIGNED_X,TOP_Y,true);
    delay(200);
}

void sensingProcessSendingReadingsToConditioning(void)
{
    // Copy readings to condition class arrays using memcpy
    memcpy(conditioningSensing.redLedVoltageMeasurement,      redLedVoltageMeasurement,      sizeof(redLedVoltageMeasurement));
    memcpy(conditioningSensing.yellowLedVoltageMeasurement,   yellowLedVoltageMeasurement,   sizeof(yellowLedVoltageMeasurement));
    memcpy(conditioningSensing.blueLedVoltageMeasurement,     blueLedVoltageMeasurement,     sizeof(blueLedVoltageMeasurement));
    memcpy(conditioningSensing.infraredLedVoltageMeasurement, infraredLedVoltageMeasurement, sizeof(infraredLedVoltageMeasurement));
}