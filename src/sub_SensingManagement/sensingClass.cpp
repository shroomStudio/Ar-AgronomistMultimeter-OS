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
   uint8_t waitForSerial = 0;

   // Initial setup for sensing class
    if (!Serial || waitForSerial > 3) 
    {
        delay(150); // Wait for serial port to connect. Needed for native USB
        waitForSerial++;
    }
    
    // Take readings from both sensors
    as7341TakeReads();
    delay(250);
    as7265xTakeReads(); 
    delay(250); 
}

void sensingClass::as7341TakeReads(void) 
{
    /*    
    if (!isAS7341Ready) {
        if (!as7341.begin()) {
            Serial.println(F("[AS7341] Failed to initialize"));
            return;
        }
        Serial.println(F("[AS7341] Initialized successfully"));
        isAS7341Ready = true;
    }

    Serial.print(F("[AS7341] read start @")); 
    Serial.println(millis());

    // Force clean state and power up
    as7341.disableAll();
    as7341.enableLED(false);  // Ensure LED starts off
    as7341.powerEnable(true);
    delay(50);

    // Configure timing
    as7341.setATIME(50);
    as7341.setASTEP(999);
    as7341.setGain(AS7341_GAIN_16X);
    delay(10);

    uint16_t readings[12] = {0};
    bool success = false;

    // Turn LED on before measurements
    as7341.enableLED(true);
    delay(50);  // Let LED stabilize

    // Read F1-F4 (low channels)
    as7341.setup_F1F4_Clear_NIR();
    delay(50);
    as7341.enableSpectralMeasurement(true);
    
    // Read low channels with timeout
    unsigned long startTime = millis();
    while ((millis() - startTime) < 1000 && !success) {
        if (as7341.getIsDataReady()) {
            for (int i = 0; i < 6; i++) {
                readings[i] = as7341.readChannel((as7341_adc_channel_t)i);
            }
            success = true;
            break;
        }
        delay(10);
    }

    if (!success) {
        Serial.println(F("[AS7341] Timeout reading low channels"));
        as7341.enableLED(false);  // Ensure LED off on error
        as7341.disableAll();
        return;
    }

    // Read F5-F8 (high channels)
    success = false;
    as7341.enableSpectralMeasurement(false);
    delay(50);
    as7341.setup_F5F8_Clear_NIR();
    as7341.enableSpectralMeasurement(true);
    delay(50);
    
    startTime = millis();
    while ((millis() - startTime) < 1000 && !success) {
        if (as7341.getIsDataReady()) {
            for (int i = 0; i < 6; i++) {
                readings[i + 6] = as7341.readChannel((as7341_adc_channel_t)i);
            }
            success = true;
            break;
        }
        delay(10);
    }

    // Always turn LED off before cleanup
    as7341.enableLED(false);
    as7341.disableAll();
    delay(50);
    
    if (!success) 
    {
        Serial.println(F("[AS7341] Timeout reading high channels"));
        return;
    }

    // Store results once only
    memcpy(as7341Readings, readings, sizeof(readings));
    
    // Print final data block once
    Serial.print("&,");
    for (int i = 0; i < 12; i++) {
        Serial.print(as7341Readings[i]);
        Serial.print(F(","));
    }
    Serial.println("&");
    delay(150);
    
    Serial.print(F("[AS7341] read complete @")); 
    Serial.println(millis());
    */
}

void sensingClass::as7265xTakeReads(void)
{   
    Serial.println(F("[AS7265x] as7265xTakeReads"));

    if (!isAS7265xReady) 
    {
        if (!as7265x.begin()) return;
        isAS7265xReady = true;
    } 
    else 
    {
        Serial.println(F("[AS7265x] sensor already initialized"));
    }

    as7265x.setIntegrationTime(166);
    as7265x.setGain(GAIN_16X);
    as7265x.setConversionType(ONE_SHOT);
    delay(150);
    // Turn on LED driver and wait
    as7265x.drvOn();
    delay(100);

    // Start measurement and log exact timestamp
    unsigned long tStart = millis();
    as7265x.startMeasurement();

    // Poll for dataReady with verbose logging each iteration
    unsigned long timeoutMs = 5000; // extended timeout for debugging
    unsigned long pollInterval = 100;
    bool ready = false;
    uint16_t attempt = 0;

    while ((millis() - tStart) < timeoutMs) 
    {
        attempt++;
        bool dr = as7265x.dataReady();

        if (dr) 
        {
            ready = true;
            break;
        }
        delay(pollInterval);
    }

    if (!ready) 
    {
        // extra diagnostics: try a soft re-init attempt
        as7265x.drvOff();
        delay(200);
        as7265x.drvOn();
        delay(200);
        as7265x.startMeasurement();
        
        bool ready2 = false;

        for (int i = 0; i < 10; i++) 
        {
            bool dr = as7265x.dataReady();
            if (dr) { ready2 = true; break; }
            delay(200);
        }
        if (!ready2) 
        {
            as7265x.drvOff();
            return;
        }
    }

    // Read raw values
    uint16_t readings[AS7265X_NUM_CHANNELS] = {0};
    as7265x.readRawValuesSequential(readings, 2500);
    as7265x.drvOff();
    delay(150);

    Serial.print(F("$,"));
    for (int i = 0; i < AS7265X_NUM_CHANNELS; i++) 
    {
        Serial.print(readings[i]);
        Serial.print(F(","));
    }
    Serial.println(F("$"));
    delay(150);

    // Additional health/debug info
    Serial.println(F("[AS7265x]temperature read:"));
    Serial.print(F(" (dec="));
    Serial.print(as7265x.readTemperature());
    Serial.println(F(")"));
    Serial.print(F("[AS7265] read complete")); 
}

/// @brief ////////////////////////
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

