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

    // as7265x readings
uint16_t as7265x_410nm;
uint16_t as7265x_435nm;
uint16_t as7265x_460nm;
uint16_t as7265x_485nm;
uint16_t as7265x_510nm;
uint16_t as7265x_535nm;
uint16_t as7265x_560nm;
uint16_t as7265x_585nm;
uint16_t as7265x_610nm;
uint16_t as7265x_645nm;
uint16_t as7265x_680nm;
uint16_t as7265x_705nm;
uint16_t as7265x_730nm;
uint16_t as7265x_760nm;
uint16_t as7265x_810nm;
uint16_t as7265x_860nm;
uint16_t as7265x_900nm;
uint16_t as7265x_940nm;

sensingClass::sensingClass(lcdDisplayClass &lcd, 
                            buttonsClass &buttons, 
                            signalConditioningClass &conditioning): 
                            lcdSensing(lcd),
                            buttonsSensing(buttons),
                            conditioningSensing(conditioning),
                            as7341(), 
                            as7265x()
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

    // Take readings from both sensors
    as7341TakeReads();
    delay(50);  // Give I2C bus time to settle
    as7265xTakeReads();  // Changed from as726xTakeReads
    delay(50);
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
        as7341.setATIME(120); // Larger = more sensitivity, slower
        as7341.setASTEP(2500); // Larger = longer exposure, more light captured
        as7341.setGain(AS7341_GAIN_64X);
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
 
    as7341.disableAll(); 
}

void sensingClass::as7265xTakeReads(void)
{   
    if (!isAS7265xReady) {
        if (!as7265x.begin()) {
            Serial.println(F("[AS7265x] Failed to initialize"));
            return;
        }
        Serial.println(F("[AS7265x] Initialized successfully"));
        isAS7265xReady = true;
    }

    // Configure sensor
    as7265x.setIntegrationTime(20);
    as7265x.setGain(GAIN_64X);
    as7265x.setConversionType(ONE_SHOT);
    delay(100);

    // Start measurement
    as7265x.drvOn();
    delay(300);
    
    Serial.println(F("[AS7265x] Starting conversion..."));
    as7265x.startMeasurement();
    
    // Wait with timeout
    unsigned long startTime = millis();
    bool success = false;
    
    while ((millis() - startTime) < 1000) {
        if (as7265x.dataReady()) {
            success = true;
            break;
        }
        delay(10);
    }

    if (!success) {
        Serial.println(F("[AS7265x] Timeout waiting for data"));
        as7265x.drvOff();
        return;
    }

    // Read values immediately after data ready
    uint16_t readings[AS7265X_NUM_CHANNELS];
    as7265x.readRawValues(readings);
    
    // Turn off LED
    as7265x.drvOff();
    
    // Store and print readings
    Serial.println(F("$"));
    for (int i = 0; i < AS7265X_NUM_CHANNELS; i++) {
        Serial.print(F("[AS7265x] Channel "));
        Serial.print(i);
        Serial.print(F(": "));
        Serial.println(readings[i]);
    }
    Serial.println(F("$"));
}

void sensingClass::takeReadingForSpecificChannelAs7341(as7341_color_channel_t channel)
{ 
    as7341.enableLED(true);
    delay(200); 
    as7341.readAllChannels();   
    as7341.delayForData(2000); 

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
    
    as7341.enableLED(false);
    delay(300);
}

void sensingClass::takeReadingForSpecificChannelAS7265x(uint8_t channel)
{
    if (channel >= AS7265X_NUM_CHANNELS) {
        Serial.println(F("[AS7265x] Invalid channel"));
        return;
    }
    
    Serial.print(as7265xReadings[channel]);
    Serial.print(F(","));
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

    // Update AS7265x readings
    conditioningSensing.as7265x_410nm = as7265x_410nm;
    conditioningSensing.as7265x_435nm = as7265x_435nm;
    conditioningSensing.as7265x_460nm = as7265x_460nm;
    conditioningSensing.as7265x_485nm = as7265x_485nm;
    conditioningSensing.as7265x_510nm = as7265x_510nm;
    conditioningSensing.as7265x_535nm = as7265x_535nm;
    conditioningSensing.as7265x_560nm = as7265x_560nm;
    conditioningSensing.as7265x_585nm = as7265x_585nm;
    conditioningSensing.as7265x_610nm = as7265x_610nm;
    conditioningSensing.as7265x_645nm = as7265x_645nm;
    conditioningSensing.as7265x_680nm = as7265x_680nm;
    conditioningSensing.as7265x_705nm = as7265x_705nm;
    conditioningSensing.as7265x_730nm = as7265x_730nm;
    conditioningSensing.as7265x_760nm = as7265x_760nm;
    conditioningSensing.as7265x_810nm = as7265x_810nm;
    conditioningSensing.as7265x_860nm = as7265x_860nm;
    conditioningSensing.as7265x_900nm = as7265x_900nm;
    conditioningSensing.as7265x_940nm = as7265x_940nm;

    // Reset AS7265x readings
    as7265x_410nm = 0;
    as7265x_435nm = 0;
    as7265x_460nm = 0;
    as7265x_485nm = 0;
    as7265x_510nm = 0;
    as7265x_535nm = 0;
    as7265x_560nm = 0;
    as7265x_585nm = 0;
    as7265x_610nm = 0;
    as7265x_645nm = 0;
    as7265x_680nm = 0;
    as7265x_705nm = 0;
    as7265x_730nm = 0;
    as7265x_760nm = 0;
    as7265x_810nm = 0;
    as7265x_860nm = 0;
    as7265x_900nm = 0;
    as7265x_940nm = 0;

    //timestamp = millis(); // Use current time as timestamp
    conditioningSensing.macronutrientsMapping();
}

