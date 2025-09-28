#include "sensingClass.h"

   
// AS7341 readings
uint16_t f1_415nm; 
uint16_t f2_445nm;
uint16_t f3_480nm;
uint16_t f4_515nm;
uint16_t clear_0;
uint16_t nir_0;
uint16_t f5_555nm;
uint16_t f6_590nm;
uint16_t f7_630nm;
uint16_t f8_680nm;
uint16_t clear;
uint16_t nir;

    // AS726x readings
uint16_t as726x_violet;
uint16_t as726x_blue;
uint16_t as726x_green;
uint16_t as726x_yellow;
uint16_t as726x_orange;
uint16_t as726x_red;
uint8_t as726x_temperature;

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
    while (buttonsSensing.buttonPressed() != BACK_BUTTON)
    {
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
            //sendingReadingsToConditioning();
        }
        
       // Serial.println("sensing in process in loop press back to stop");
        delay(1500);
        sensingProcessFinished = false;
        buttonsSensing.navigationButtons();
        delay(1000);
    }
    
    lcdSensing.metadataTodisplayFreeCursor("Sensing process finished \n",LEFT_ALIGNED_X,TOP_Y,false);
    delay(1000);
}

void sensingClass::temperatureSensingProcess()
{
}
void sensingClass::humiditySensingProcess()
{
}
void sensingClass::atmosphericPressureSensingProcess()
{
}

void sensingClass::sensingProcessTakeReadings(void)
{    
   //Serial.println("sensingProcessTakeReadings");

   // Initial setup for sensing class
    while (!Serial) 
    {
        delay(3); // Wait for serial port to connect. Needed for native USB
    }

    delay(500);

    as7341TakeReads();
    as726xTakeReads();
    //temperatureSensingProcess();
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
        as7341.setGain(AS7341_GAIN_256X);
        Serial.println("AS7341 initialized successfully");
        as7341.enableLED(false);
        delay(500);
    }

    Serial.println("&");
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_415nm_F1);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_445nm_F2);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_480nm_F3);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_515nm_F4);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_CLEAR_0);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_NIR_0);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_555nm_F5);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_630nm_F7);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_680nm_F8);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_CLEAR);
    takeReadingForSpecificChannelAs7341(AS7341_CHANNEL_NIR);
    Serial.println("&");
   
    buttonsSensing.navigationButtons(); 
    delay(500); // Allow some time for button processing
    as7341.disableAll(); 
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
        delay(500);
        as726x.setGain(GAIN_3X7);
        as726x.setIntegrationTime(200);   
    }

    Serial.println("$");
    takeReadingForSpecificChannelAs726x(AS7262_VIOLET);
    takeReadingForSpecificChannelAs726x(AS7262_BLUE);
    takeReadingForSpecificChannelAs726x(AS7262_GREEN);
    takeReadingForSpecificChannelAs726x(AS7262_YELLOW);
    takeReadingForSpecificChannelAs726x(AS7262_ORANGE);
    takeReadingForSpecificChannelAs726x(AS7262_RED);
    Serial.println("$");

    delay(500);
}

void sensingClass::takeReadingForSpecificChannelAs7341(as7341_color_channel_t channel)
{ 
    as7341.enableLED(true);
    delay(500); 
    as7341.readAllChannels();   
    as7341.delayForData(1500); 

    switch (channel)
    {
        case AS7341_CHANNEL_415nm_F1:
            f1_415nm = as7341.getChannel(AS7341_CHANNEL_415nm_F1);
            if (f1_415nm < 0) f1_415nm = 0;
            Serial.print(f1_415nm);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_445nm_F2:
            f2_445nm = as7341.getChannel(AS7341_CHANNEL_445nm_F2);
            if (f2_445nm < 0) f1_415nm = 0;
            Serial.print(f2_445nm);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_480nm_F3:
            f3_480nm = as7341.getChannel(AS7341_CHANNEL_480nm_F3);
            if (f3_480nm < 0) f1_415nm = 0;
            Serial.print(f3_480nm);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_515nm_F4:
            f4_515nm = as7341.getChannel(AS7341_CHANNEL_515nm_F4);
            if (f4_515nm < 0) f1_415nm = 0;
            Serial.print(f4_515nm);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_CLEAR_0:
            clear_0 = as7341.getChannel(AS7341_CHANNEL_CLEAR_0);
            if (clear_0 < 0) f1_415nm = 0;
            Serial.print(clear_0);
            Serial.print(",");
            break;
            
        case AS7341_CHANNEL_NIR_0:
            nir_0 = as7341.getChannel(AS7341_CHANNEL_NIR_0);
            if (nir_0 < 0) f1_415nm = 0;
            Serial.print(nir_0);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_555nm_F5:
            f5_555nm = as7341.getChannel(AS7341_CHANNEL_555nm_F5);
            if (f5_555nm < 0) f1_415nm = 0;
            Serial.print(f5_555nm);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_590nm_F6:
            f6_590nm = as7341.getChannel(AS7341_CHANNEL_590nm_F6);
            if (f6_590nm < 0) f1_415nm = 0;
            Serial.print(f6_590nm);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_630nm_F7:
            f7_630nm = as7341.getChannel(AS7341_CHANNEL_630nm_F7);
            if (f7_630nm < 0) f1_415nm = 0;
            Serial.print(f7_630nm);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_680nm_F8:
            f8_680nm = as7341.getChannel(AS7341_CHANNEL_680nm_F8);
            if (f8_680nm < 0) f1_415nm = 0;
            Serial.print(f8_680nm);
            Serial.print(",");
            break;

        case AS7341_CHANNEL_CLEAR:
            clear = as7341.getChannel(AS7341_CHANNEL_CLEAR);
            if (clear < 0) f1_415nm = 0;
            Serial.print(clear);
            Serial.print(",");
            break;
        case AS7341_CHANNEL_NIR:
            nir = as7341.getChannel(AS7341_CHANNEL_NIR);
            if (nir < 0) f1_415nm = 0;
            Serial.print(nir);
            Serial.print(",");
            break;
        
        default:
            break;
    }
    
    //delay(500);
    //as726x.drvOff();
    as7341.enableLED(false);
    delay(500); 
}

void sensingClass::takeReadingForSpecificChannelAs726x(uint8_t channel)
{
    as726x.drvOn(); 
    delay(300);
    as726x.startMeasurement();
    delay(1500);
    
    switch (channel)
    {
        case AS7262_VIOLET:
            as726x_violet = as726x.readViolet();
            Serial.print(as726x_violet);
            Serial.print(",");
            break;
        case AS7262_BLUE:
            as726x_blue = as726x.readBlue();
            Serial.print(as726x_blue);
            Serial.print(",");
            break;
        case AS7262_GREEN:
            as726x_green = as726x.readGreen();
            Serial.print(as726x_green);
            Serial.print(",");
            break;
        case AS7262_YELLOW:
            as726x_yellow = as726x.readYellow();
            Serial.print(as726x_yellow);
            Serial.print(",");
            break;
        case AS7262_ORANGE:
            as726x_orange = as726x.readOrange();
            Serial.print(as726x_orange);
            Serial.print(",");
            break;
        case AS7262_RED:
            as726x_red = as726x.readRed();
            Serial.print(as726x_red);
            Serial.print(",");
            break;
        default:
            Serial.println("Invalid channel");
            break;
    }

    as726x.drvOff(); 
    delay(500);
}

void sensingClass::sendingReadingsToConditioning(void)
{
    // Send readings to signal conditioning class
    conditioningSensing.raw_f1_415nm = f1_415nm;
    conditioningSensing.raw_f2_445nm = f2_445nm;
    conditioningSensing.raw_f3_480nm = f3_480nm;
    conditioningSensing.raw_f4_515nm = f4_515nm;
    conditioningSensing.clear_0 = clear_0;
    conditioningSensing.nir_0 = nir_0;
    conditioningSensing.raw_f5_555nm = f5_555nm;
    conditioningSensing.raw_f6_590nm = f6_590nm;
    conditioningSensing.raw_f7_630nm = f7_630nm;
    conditioningSensing.raw_f8_680nm = f8_680nm;
    conditioningSensing.clear = clear;
    conditioningSensing.nir = nir;

    // Reset readings after sending
    f1_415nm = 0; 
    f2_445nm = 0;
    f3_480nm = 0;
    f4_515nm = 0;
    clear_0 = 0;
    nir_0 = 0;
    f5_555nm = 0;
    f6_590nm = 0;
    f7_630nm = 0;
    f8_680nm = 0;
    clear = 0;
    nir = 0;

    conditioningSensing.as726x_violet = as726x_violet;
    conditioningSensing.as726x_blue = as726x_blue;
    conditioningSensing.as726x_green = as726x_green;
    conditioningSensing.as726x_yellow = as726x_yellow;
    conditioningSensing.as726x_orange = as726x_orange;
    conditioningSensing.as726x_red = as726x_red;

    // Reset AS726x readings after sending
    as726x_violet = 0;
    as726x_blue = 0;
    as726x_green = 0;
    as726x_yellow = 0;
    as726x_orange = 0;
    as726x_red = 0;

    //timestamp = millis(); // Use current time as timestamp
    conditioningSensing.macronutrientsMapping();
}

