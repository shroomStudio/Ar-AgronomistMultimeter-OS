#include "sensingClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"
#include "sub_SignalConditioning/signalConditioningClass.h"

//Clases instances 
lcdDisplayClass lcdSensing;
buttonsClass buttonsSensing;
signalConditioningClass conditioningSensing;

sensingClass::sensingClass(): as7341(), 
                             as726x(),
                             f2_nitrogen(0),
                             f3_nitrogen(0),
                             f4_phosphorus(0),
                             f5_phosphorus(0),
                             f7_potassium(0),
                             f8_potassium(0),
                             timestamp(0)
{
  initialSensingClassSetup();
}   

sensingClass::~sensingClass()
{
    //destructor sensing class
}

void sensingClass::initialSensingClassSetup()
{
    //pinMode(3, OUTPUT); 
    pinMode(PIN_RED_LED, OUTPUT);
    pinMode(PIN_GREEN_LED, OUTPUT);
    pinMode(PIN_BLUE_LED, OUTPUT);
    pinMode(PIN_YELLOW_LED, OUTPUT);
    pinMode(PIN_WHITE_LED, OUTPUT);

    digitalWrite(PIN_RED_LED, HIGH);
    digitalWrite(PIN_GREEN_LED, HIGH);
    digitalWrite(PIN_BLUE_LED, HIGH);
    digitalWrite(PIN_YELLOW_LED, LOW);
    digitalWrite(PIN_WHITE_LED, LOW);
}


void sensingClass::macronutrientSensingProcess()
{
    bool sensingProcessFinished = false;

    while (buttonsSensing.buttonPressed() != OK_BUTTON)
    {
        lcdSensing.metadataTodisplayInLCD("please get ready the sample, press OK to continue", LEFT_ALIGNED_X, MIDDLE_Y,true);
        delay(1000);
        buttonsSensing.navigationButtons();
    }
    //TODO: get and Save date and time
    if (!sensingProcessFinished)
    {
        lcdSensing.metadataTodisplayInLCD("sensing in process, press back to abort... \n", LEFT_ALIGNED_X, MIDDLE_Y,true);
        delay(1500);
        sensingProcessTakeReadings();
        //Process of sensing finished values 
        sensingProcessFinished = true;  
    }
    if (true == sensingProcessFinished)
    {
        //Sendind readings to conditioning class
        sendingReadingsToConditioning();
    }

    lcdSensing.metadataTodisplayFreeCursor("Sensing process finished \n",LEFT_ALIGNED_X,TOP_Y,false);
    delay(1000);
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
    // Sequence of turn each element on
    digitalWrite(PIN_RED_LED, LOW);
    delay(500);
    digitalWrite(PIN_GREEN_LED, LOW);
    delay(500);
    digitalWrite(PIN_BLUE_LED, LOW);
    delay(500);
    digitalWrite(PIN_YELLOW_LED, HIGH);
    delay(500);
    digitalWrite(PIN_WHITE_LED, HIGH);
    delay(500);
    
}

void sensingClass::turnOffAllElements(void)
{
    digitalWrite(PIN_RED_LED, HIGH);
    delay(500);
    digitalWrite(PIN_GREEN_LED, HIGH);
    delay(500);
    digitalWrite(PIN_BLUE_LED, HIGH);
    delay(500);
    digitalWrite(PIN_YELLOW_LED, LOW);
    delay(500);
    digitalWrite(PIN_WHITE_LED, LOW);
    delay(500);
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

void sensingClass::sendingReadingsToConditioning(void)
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
        // Read all channels from the AS7341 sensor
        // Set integration time to 50 ms (ATIME and ASTEP calculation)
        // AS7341 integration time = (ATIME + 1) * (ASTEP + 1) * 2.78us
        // For 50ms: (ATIME + 1) * (ASTEP + 1) = 50,000 / 2.78 ≈ 17986
        // Example: ATIME = 17, ASTEP = 1057  => (17+1)*(1057+1) = 19044 (close to 17986)
        as7341.setATIME(17);
        as7341.setASTEP(1057);
        as7341.setGain(AS7341_GAIN_64X);
        Serial.println("AS7341 initialized successfully");
    }

    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_445nm_F2);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_480nm_F3);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_515nm_F4);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_555nm_F5);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_630nm_F7);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_680nm_F8);
    
    delay(100); 
    buttonsSensing.navigationButtons(); 
    turnOffAllElements();
}

void sensingClass::as726xTakeReads(void)
{
   /* if (!as726x.begin(&Wire))
    {
        Serial.println("Could not find AS726x");
        while (!as726x.begin()) { delay(3); }
    }
    else
    {
        Serial.println("AS726x initialized successfully");
        as726x.setGain(GAIN_1X);
    }
    

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

void sensingClass::takeReadingForSpecificChannelAs7341(as7341_color_channel_t channel)
{
    switch (channel)
    {
    case AS7341_CHANNEL_415nm_F1:

        break;

    case AS7341_CHANNEL_445nm_F2:

        digitalWrite(PIN_BLUE_LED, LOW);
        delay(500);

        f2_nitrogen = as7341.getChannel(AS7341_CHANNEL_445nm_F2);
        
        digitalWrite(PIN_BLUE_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_480nm_F3:

        digitalWrite(PIN_BLUE_LED, LOW);
        delay(500);

        f3_nitrogen = as7341.getChannel(AS7341_CHANNEL_480nm_F3);

        digitalWrite(PIN_BLUE_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_515nm_F4:
        
        digitalWrite(PIN_GREEN_LED, LOW);
        delay(500);

        f4_phosphorus = as7341.getChannel(AS7341_CHANNEL_515nm_F4);

        digitalWrite(PIN_GREEN_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_CLEAR_0:
        /* code */
        break;
    case AS7341_CHANNEL_NIR_0:
        /* code */
        break;

    case AS7341_CHANNEL_555nm_F5:

        digitalWrite(PIN_GREEN_LED, LOW);
        delay(500);

        f5_phosphorus = as7341.getChannel(AS7341_CHANNEL_555nm_F5);

        digitalWrite(PIN_GREEN_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_590nm_F6:
        /* code */
        break;

    case AS7341_CHANNEL_630nm_F7:

        digitalWrite(PIN_RED_LED, LOW);
        delay(500);

        f7_potassium = as7341.getChannel(AS7341_CHANNEL_630nm_F7);

        digitalWrite(PIN_RED_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_680nm_F8:

        digitalWrite(PIN_RED_LED, LOW);
        delay(500);

        f8_potassium = as7341.getChannel(AS7341_CHANNEL_680nm_F8);

        digitalWrite(PIN_RED_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_CLEAR:
        /* code */
        break;
    case AS7341_CHANNEL_NIR:
        /* code */
        break;
    
    default:
        turnOffAllElements();
        break;
    }   
}

