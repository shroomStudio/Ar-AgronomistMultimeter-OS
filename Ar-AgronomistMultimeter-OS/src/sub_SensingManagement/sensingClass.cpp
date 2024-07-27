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
    int nitrogenMeasurements[MAX_NUMBER_OF_SAMPLES];
    int potassiumMeasurements[MAX_NUMBER_OF_SAMPLES];
    int phosphorusMeasurement[MAX_NUMBER_OF_SAMPLES];
    
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
        delay(3000);
        
        nitrogenSensingProcess();
        phosphorusSensingProcess();
        potassiumSensingProcess();

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
void sensingClass::nitrogenSensingProcess(void)
{
    lcdSensing.metadataTodisplayFreeCursor("Nitrogren",LEFT_ALIGNED_X,TOP_Y,true);
    delay(2000);
    //Before Any process turn on all the elements 
    turnOnAllElements();
    delay(2000);
    //Ensure all the leds and photodiode are off
    turnOffAllElements();
    //Starting The sensing process
    sensingProcessTakeReadings(N_MEASURE);  
}

void sensingClass::phosphorusSensingProcess(void)
{
    lcdSensing.metadataTodisplayFreeCursor("Phosphorus",LEFT_ALIGNED_X,TOP_Y,true);
    delay(2000);
    //Before Any process turn on all the elements 
    turnOnAllElements();
    delay(2000);
    //Ensure all the leds and photodiode are off
    turnOffAllElements();
    //Starting The sensing process
    sensingProcessTakeReadings(P_MEASURE);  
}
void sensingClass::potassiumSensingProcess(void)
{
    
    lcdSensing.metadataTodisplayFreeCursor("Potassium",LEFT_ALIGNED_X,TOP_Y,true);
    delay(2000);
    //Before Any process turn on all the elements 
    turnOnAllElements();
    delay(2000);
    //Ensure all the leds and photodiode are off
    turnOffAllElements();
    //Starting The sensing process
    sensingProcessTakeReadings(K_MEASURE);  
}

void sensingClass::turnOnAllElements(void)
{
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(blueLedPin, HIGH);
    digitalWrite(infraredLedPin, HIGH);
    digitalWrite(redLedPin, HIGH);
    delay(200);
}

void sensingClass::turnOffAllElements(void)
{
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
    digitalWrite(infraredLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    delay(200);
}

void sensingClass::sensingProcessTakeReadings(int indexElement)
{
    int sample = 0;
    int tempArray[MAX_NUMBER_OF_SAMPLES];

    //turn on red LED diode
    digitalWrite(redLedPin, HIGH);
    delay(200);
    //reading values for Red LED
     for(int i=0; i < MEASURES_PER_LED; i++ )
    {
        /* //Start reading photodiode */
        tempArray[(sample +i)] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD((tempArray[(sample +i)]),LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off red LED
    digitalWrite(redLedPin, LOW);
    delay(200);
    sample += MEASURES_PER_LED;
    //turn on yellow LED diode
    digitalWrite(yellowLedPin, HIGH);
    delay(200);
    //reading values for yellow LED
    for(int i=0; i < MEASURES_PER_LED; i++ )
    {
        /* //Start reading photodiode */
        tempArray[(sample +i)] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD((tempArray[(sample +i)]),LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off yellow LED
    digitalWrite(yellowLedPin, LOW);
    delay(200);
    sample += MEASURES_PER_LED;
    //turn on blue LED diode
    digitalWrite(blueLedPin, HIGH);
    delay(200);
    //reading values for blue LED
    for(int i=0; i < MEASURES_PER_LED; i++ )
    {
        /* //Start reading photodiode */
        tempArray[(sample +i)] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD((tempArray[(sample +i)]),LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off blue LED
    digitalWrite(blueLedPin, LOW);
    delay(200);

    sample += MEASURES_PER_LED;

    //turn on IR LED diode
    digitalWrite(infraredLedPin, HIGH);
    delay(200);
    //reading values for IR LED
   for(int i=0; i < MEASURES_PER_LED; i++ )
    {
        /* //Start reading photodiode */
        tempArray[(sample +i)] = analogRead(photodiodeInput);
        delay(100);
        lcdSensing.intNumberTodisplayInLCD((tempArray[(sample +i)]),LEFT_ALIGNED_X,TOP_Y,true);
        delay(200);
    }
    //Turn Off IR LED
    digitalWrite(infraredLedPin, LOW);
    delay(200);

    //Copy the information took to the correct array
    switch (indexElement)
    {
    case N_MEASURE:
        /* code */
        for (int i =0; i < MAX_NUMBER_OF_SAMPLES; i++)
        {
            nitrogenMeasurements[i] = tempArray[i];
        }
        break;
    case K_MEASURE:
        /* code */
        for (int i =0; i < MAX_NUMBER_OF_SAMPLES; i++)
        {
            potassiumMeasurements[i] = tempArray[i];
        }
        break;
    case P_MEASURE:
        /* code */
        for (int i =0; i < MAX_NUMBER_OF_SAMPLES; i++)
        {
            
            phosphorusMeasurement[i] = tempArray[i];
        }
        break;
    default:
        break;
    }
}

void sensingClass::sensingProcessSendingReadingsToConditioning(void)
{
    // Copy Nitrogen readings to condition class arrays
    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        conditioningSensing.nitrogenMeasurements[i] = nitrogenMeasurements[i];
        //Clearing global scope file array to be ready for next process 
        nitrogenMeasurements[i] = {'\0'};
    }
    // Copy Potassium readings to condition class arrays
    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        conditioningSensing.potassiumMeasurements[i] = potassiumMeasurements[i];
        //Clearing global scope file array to be ready for next process 
        potassiumMeasurements[i] = {'\0'};
    }
    // Copy Phosphorus readings to condition class arrays
    for (int i = 0; i < MAX_NUMBER_OF_SAMPLES; i++)
    {
        conditioningSensing.phosphorusMeasurement[i] = phosphorusMeasurement[i];
        //Clearing global scope file array to be ready for next process 
        phosphorusMeasurement[i] = {'\0'};
    }
    //Starting signal conditioning process
    conditioningSensing.macronutrientsMapping();
}