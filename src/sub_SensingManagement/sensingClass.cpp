#include "sensingClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"
#include "sub_SignalConditioning/signalConditioningClass.h"
//#include "ExternalLibraries/SoftwareWire.h"

//Clases instances 
lcdDisplayClass lcdSensing;
buttonsClass buttonsSensing;
signalConditioningClass conditioningSensing;

//Adafruit_AS7341 as7341Sensing;

// SoftwareWire instances for I2C communication
/*SoftwareWire myWire1(2, 3); // SDA = D2, SCL = D3
SoftwareWire myWire2(4, 5); // SDA = D4, SCL = D5
SoftwareWire myWire3(6, 7); // SDA = D6, SCL = D7*/

//Global File scope Variables
    int whiteLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int blueLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int redLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int yellowLedMeasurements[MAX_NUMBER_OF_SAMPLES];
    int greenLedMeasurements[MAX_NUMBER_OF_SAMPLES];
   
sensingClass::sensingClass()
{
  //initialSensingClassSetup();
}   

sensingClass::~sensingClass()
{
    //destructor sensing class
}

void sensingClass::initialSensingClassSetup()
{
    /*myWire1.begin();
    myWire2.begin();
    myWire3.begin();*/
//as7341Sensing.begin(AS7341_I2CADDR_DEFAULT, &Wire, -1);
    /*if (!as7341Sensing.begin(AS7341_I2CADDR_DEFAULT, &Wire, -1))
    {
        Serial.println("Could not find AS7341");
        while (1) { delay(10); }
    }*/
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
    //sensingProcessSendingReadingsToConditioning();
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

}

void sensingClass::turnOffAllElements(void)
{

}

void sensingClass::sensingProcessTakeReadings(void)
{
   while (!Serial) 
   {
    delay(1);
   }
    
   Serial.println("sensingProcessTakeReadings");

    if (!as7341.begin())
    {
        Serial.println("Could not find AS7341");
        while (1) { delay(10); }
    }
    as7341.setATIME(100);
    as7341.setASTEP(999);
    as7341.setGain(AS7341_GAIN_256X);

    uint16_t readings[12] = {0}; // Array to hold readings from the AS7341 sensor

// Loop to keep taking readings until the STOP button is pressed
    while (buttonsSensing.buttonPressed() != BACK_BUTTON) 
    {
        if (!as7341.readAllChannels(readings))
        {
            Serial.println("Error reading all channels!");
            return;
        }



        lcdSensing.metadataTodisplayFreeCursor(" \n \n \n \n \n \n",LEFT_ALIGNED_X,TOP_Y,true);
        delay(1000);


        Serial.print("ADC0/F1 415nm : ");
        Serial.println(readings[0]);
        Serial.print("ADC1/F2 445nm : ");
        Serial.println(readings[1]);
        Serial.print("ADC2/F3 480nm : ");
        Serial.println(readings[2]);
        Serial.print("ADC3/F4 515nm : ");
        Serial.println(readings[3]);
        Serial.print("ADC0/F5 555nm : ");

        // we skip the first set of duplicate clear/NIR readings
        Serial.print("ADC4/Clear-");
        Serial.println(readings[4]);
        Serial.print("ADC5/NIR-");
        Serial.println(readings[5]);
        
        Serial.println(readings[6]);
        Serial.print("ADC1/F6 590nm : ");
        Serial.println(readings[7]);
        Serial.print("ADC2/F7 630nm : ");
        Serial.println(readings[8]);
        Serial.print("ADC3/F8 680nm : ");
        Serial.println(readings[9]);
        Serial.print("ADC4/Clear    : ");
        Serial.println(readings[10]);
        Serial.print("ADC5/NIR      : ");
        Serial.println(readings[11]);

        Serial.println();

        delay(500); // Optional: add a small delay to avoid flooding output
        buttonsSensing.navigationButtons(); // Update button state
        delay(1000);
    }
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