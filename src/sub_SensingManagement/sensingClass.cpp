#include "sensingClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"
#include "sub_SignalConditioning/signalConditioningClass.h"
#include "ExternalLibraries/SoftwareWire.h"

//Clases instances 
lcdDisplayClass lcdSensing;
buttonsClass buttonsSensing;
signalConditioningClass conditioningSensing;

//Adafruit_AS7341 as7341Sensing;

// SoftwareWire instances for I2C communication
SoftwareWire myWire1(2, 3); // SDA = D2, SCL = D3
SoftwareWire myWire2(4, 5); // SDA = D4, SCL = D5
   
sensingClass::sensingClass()
{
  initialSensingClassSetup();
}   

sensingClass::~sensingClass()
{
    //destructor sensing class
}

void sensingClass::initialSensingClassSetup()
{
    myWire1.begin();
    myWire2.begin();
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
    if (1)
    {
        auto temperatureAmbient = myWire1.read();

        Serial.print("Temperature Ambient: ");
        Serial.println(temperatureAmbient);
    }
    else
    {
        Serial.println("No temperature data available.");
    }
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



     Serial.println("\n\n\n\n\n\n");
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

        temperatureSensingProcess();
        
        delay(500); // Optional: add a small delay to avoid flooding output
        buttonsSensing.navigationButtons(); // Update button state
        delay(1000);
    }
}

void sensingClass::sensingProcessSendingReadingsToConditioning(void)
{

}