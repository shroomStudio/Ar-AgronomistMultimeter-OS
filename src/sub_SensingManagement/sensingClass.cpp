#include "sensingClass.h"


//Clases instances 
uint16_t f2_nitrogen;
uint16_t f3_nitrogen;
uint16_t f4_phosphorus;
uint16_t f5_phosphorus;
uint16_t f7_potassium;
uint16_t f8_potassium;
uint32_t timestamp; 

sensingClass::sensingClass(lcdDisplayClass &lcd, 
                            buttonsClass &buttons, 
                            signalConditioningClass &conditioning): 
                            lcdSensing(lcd),
                            buttonsSensing(buttons),
                            conditioningSensing(conditioning),
                            as7341(), 
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
void sensingClass::humiditySensingProcess()
{

}
void sensingClass::atmosphericPressureSensingProcess()
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
        as7341.setGain(AS7341_GAIN_128X);
        Serial.println("AS7341 initialized successfully");
    }

    delay(1000); // Wait for sensor to stabilize
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_445nm_F2);
    delay(1000); // Wait for sensor to stabilize
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_480nm_F3);
    delay(1000); // Wait for sensor to stabilize
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_515nm_F4);
    delay(1000); // Wait for sensor to stabilize
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_555nm_F5);
    delay(1000); // Wait for sensor to stabilize
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_630nm_F7);
    delay(1000); // Wait for sensor to stabilize
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_680nm_F8);
    // Wait for sensor to stabilize
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
        
        as7341.readAllChannels();
        delay(1000);

        f2_nitrogen = as7341.getChannel(AS7341_CHANNEL_445nm_F2);

        digitalWrite(PIN_BLUE_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_480nm_F3:

        digitalWrite(PIN_BLUE_LED, LOW);
        
        as7341.readAllChannels();
        delay(1000);

        f3_nitrogen = as7341.getChannel(AS7341_CHANNEL_480nm_F3);

        digitalWrite(PIN_BLUE_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_515nm_F4:
        
        digitalWrite(PIN_GREEN_LED, LOW);
 
        as7341.readAllChannels();
               delay(1000);

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
   
        as7341.readAllChannels();        
        delay(1000);

        f5_phosphorus = as7341.getChannel(AS7341_CHANNEL_555nm_F5);

        digitalWrite(PIN_GREEN_LED, HIGH);
        delay(500);

        break;

    case AS7341_CHANNEL_590nm_F6:
        /* code */
        break;

    case AS7341_CHANNEL_630nm_F7:

        digitalWrite(PIN_RED_LED, LOW);
 
        as7341.readAllChannels();
        delay(500);

        f7_potassium = as7341.getChannel(AS7341_CHANNEL_630nm_F7);

        digitalWrite(PIN_RED_LED, HIGH);
        delay(1000);

        break;

    case AS7341_CHANNEL_680nm_F8:

        digitalWrite(PIN_RED_LED, LOW);

        as7341.readAllChannels();
                delay(1000);

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

void sensingClass::sendingReadingsToConditioning(void)
{
    // Send readings to signal conditioning class
    conditioningSensing.raw_f2_nitrogen = f2_nitrogen;
    conditioningSensing.raw_f3_nitrogen = f3_nitrogen;
    conditioningSensing.raw_f4_phosphorus = f4_phosphorus;
    conditioningSensing.raw_f5_phosphorus = f5_phosphorus;
    conditioningSensing.raw_f7_potassium = f7_potassium;
    conditioningSensing.raw_f8_potassium = f8_potassium;
    //conditioningSensing.raw_timestamp = millis(); // Use current time as timestamp
    f2_nitrogen = 0; // Reset readings after sending
    f3_nitrogen = 0;
    f4_phosphorus = 0;
    f5_phosphorus = 0;
    f7_potassium = 0;
    f8_potassium = 0;
    //timestamp = millis(); // Use current time as timestamp
    conditioningSensing.macronutrientsMapping();
}

