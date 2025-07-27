#include "sensingClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"
#include "sub_SignalConditioning/signalConditioningClass.h"

//Clases instances 
lcdDisplayClass lcdSensing;
buttonsClass buttonsSensing;
signalConditioningClass conditioningSensing;

const uint8_t MaxNumberOfSamples = 30; 
const uint8_t ZeroValue = 0; 
const uint8_t MaxNumberOfReadingsAS7341 = 12; // Number of readings for AS7341 sensor
static uint16_t As7341Wavelenght415nm;
static uint16_t As7341Wavelenght445nm;
static uint16_t As7341Wavelenght480nm;
static uint16_t As7341Wavelenght515nm;
static uint16_t As7341WavelenghtClear;
static uint16_t As7341WavelenghtNIR;
static uint16_t As7341Wavelenght555nm;
static uint16_t As7341Wavelenght590nm;
static uint16_t As7341Wavelenght630nm;
static uint16_t As7341Wavelenght680nm;

// SoftwareWire instances for I2C communication

   
sensingClass::sensingClass(): as7341(), 
                              as726x()
{
  initialSensingClassSetup();
}   

sensingClass::~sensingClass()
{
    //destructor sensing class
}

void sensingClass::initialSensingClassSetup()
{
    
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
    /*if (1)
    {
        auto temperatureAmbient = 0;

        Serial.print("Temperature Ambient: ");
        Serial.println(temperatureAmbient);
    }
    else
    {
        Serial.println("No temperature data available.");
    }*/
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
   Serial.println("sensingProcessTakeReadings");

   // Initial setup for sensing class
    while (!Serial) 
    {
        delay(3); // Wait for serial port to connect. Needed for native USB
    }

    as7341TakeReads();
    as726xTakeReads();
    temperatureSensingProcess();
}

void sensingClass::sensingProcessSendingReadingsToConditioning(void)
{

}

void sensingClass::as7341TakeReads(void)
{
    // Initialize sensors
    if (!as7341.begin(AS7341_I2CADDR_DEFAULT, &Wire))
    {
        Serial.println("Could not find AS7341");
        while (1) 
        {
            delay(3); 
        }
    }
    else
    {
        Serial.println("AS7341 initialized successfully");
        as7341.setATIME(100);
        as7341.setASTEP(999);
        as7341.setGain(AS7341_GAIN_256X);
    }
    
    //Loop to keep taking readings until the STOP button is pressed
    for (int totalReadings = ZeroValue; totalReadings < MaxNumberOfSamples; totalReadings++)
    {
        uint16_t as7341Readings[MaxNumberOfReadingsAS7341] = {ZeroValue}; // Array to hold readings from the AS7341 sensor

        if (buttonsSensing.buttonPressed() == BACK_BUTTON)
        {
            Serial.println("Sensing process aborted by user.");
            break;
        }
        // Read all channels from the AS7341 sensor
        if (!as7341.readAllChannels(as7341Readings))
        {
            Serial.println("Error reading all channels!");
            break;
        }

        As7341Wavelenght415nm += as7341Readings[0];
        As7341Wavelenght445nm += as7341Readings[1];
        As7341Wavelenght480nm += as7341Readings[2];
        As7341Wavelenght515nm += as7341Readings[3];
        As7341WavelenghtClear += as7341Readings[4];
        As7341WavelenghtNIR += as7341Readings[5];
        As7341Wavelenght555nm += as7341Readings[6];
        As7341Wavelenght590nm += as7341Readings[7];
        As7341Wavelenght630nm += as7341Readings[8];
        As7341Wavelenght680nm += as7341Readings[9];
        As7341WavelenghtClear += as7341Readings[10];
        As7341WavelenghtNIR += as7341Readings[11];

        delay(100); 
        buttonsSensing.navigationButtons(); 
    }

    As7341Wavelenght415nm /= MaxNumberOfSamples;
    As7341Wavelenght445nm /= MaxNumberOfSamples;
    As7341Wavelenght480nm /= MaxNumberOfSamples;
    As7341Wavelenght515nm /= MaxNumberOfSamples;
    As7341WavelenghtClear /= MaxNumberOfSamples;
    As7341WavelenghtNIR /= MaxNumberOfSamples;
    As7341Wavelenght555nm /= MaxNumberOfSamples;
    As7341Wavelenght590nm /= MaxNumberOfSamples;
    As7341Wavelenght630nm /= MaxNumberOfSamples;
    As7341Wavelenght680nm /= MaxNumberOfSamples;

}

void sensingClass::as726xTakeReads(void)
{
    if (!as726x.begin(&Wire))
    {
        Serial.println("Could not find AS726x");
        while (!as726x.begin()) { delay(3); }
    }
    else
    {
        Serial.println("AS726x initialized successfully");
        as726x.setGain(GAIN_1X);
    }
    /*

    do {
        as726x.startMeasurement();
        Serial.print("Violet: ");
        Serial.println(as726x.readViolet());
        Serial.print("Blue: ");
        Serial.println(as726x.readBlue());      
        Serial.print("Green: ");
        Serial.println(as726x.readGreen());
        Serial.print("Yellow: ");
        Serial.println(as726x.readYellow());
        Serial.print("Orange: ");
        Serial.println(as726x.readOrange());
        Serial.print("Red: ");
        Serial.println(as726x.readRed());
        
        delay(500); 
        buttonsSensing.navigationButtons(); 
        delay(500);
    } while (buttonsSensing.buttonPressed() != BACK_BUTTON);*/ 
}