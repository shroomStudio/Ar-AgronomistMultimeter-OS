// ShroomCorp sensingClass.cpp all the code here is property of ShroomCorp and cannot be used
// or distributed without written permission from ShroomCorp  

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
bool CalibrationIsDone;
unsigned long lampStartTime = 0;  // ARD-08: captures millis() when inference starts

sensingClass::sensingClass(lcdDisplayClass &lcd, 
                            buttonsClass &buttons, 
                            signalConditioningClass &conditioning): 
                            lcdSensing(lcd),
                            buttonsSensing(buttons),
                            conditioningSensing(conditioning),
                            as7265x()
{
    //Constructor sensing class
}   

sensingClass::~sensingClass()
{
    //destructor sensing class
}

void sensingClass::macronutrientSensingProcess()
{
    bool sensingProcessFinished = false;

    while (buttonsSensing.buttonPressed() != OK_BUTTON)
    {
        lcdSensing.metadataTodisplayInLCD("please get ready the sample, press OK to continue", LEFT_ALIGNED_X, MIDDLE_Y,true);
        buttonsSensing.navigationButtons();
        delay(1000);
    }

    while (buttonsSensing.buttonPressed() != BACK_BUTTON)
    {
        if (!sensingProcessFinished)
        {
            digitalWrite(PIN_YELLOW_LED, LOW);
            digitalWrite(PIN_WHITE_LED, LOW);
            delay(250);

            sensingProcessTakeReadings();
            //Process of sensing finished values 
            sensingProcessFinished = true;
        }
        if (true == sensingProcessFinished)
        {
            //Sendind readings to conditioning class
            //sendingReadingsToConditioning();
            break;
        }
        // Serial.println("sensing in process in loop press back to stop");
        buttonsSensing.navigationButtons();
        delay(200);
    }
    
    // Turn off lamp 
    digitalWrite(PIN_YELLOW_LED, HIGH);
    digitalWrite(PIN_WHITE_LED, HIGH);
    delay(500);

    lcdSensing.metadataTodisplayFreeCursor("Sensing process finished \n",LEFT_ALIGNED_X,TOP_Y,true);
    delay(200);
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

    if (!CalibrationIsDone)
    {
        do {
            buttonsSensing.navigationButtons();
            lcdSensing.metadataTodisplayInLCD("Press and Hold-OK to take reads", LEFT_ALIGNED_X, MIDDLE_Y,true);
        } while (buttonsSensing.buttonPressed() != OK_BUTTON);

        lcdSensing.metadataTodisplayFreeCursor("Taking readings... \n",LEFT_ALIGNED_X,TOP_Y,true);
        
        Serial.println(F("/*"));
        delay(200);
        
        for (int i=0 ; i < NUMBER_OF_READS_TO_TAKE; ++i )
        {
            as7265xTakeReads(); 
            delay(250); 
        }

        Serial.println(F("*/"));
        delay(500);
        CalibrationIsDone = true;
    }

    /*if (CalibrationIsDone) 
    {
        do {
            buttonsSensing.navigationButtons();
            lcdSensing.metadataTodisplayInLCD("Press and Hold-OK to continue Measure", LEFT_ALIGNED_X, MIDDLE_Y,true);
        } while (buttonsSensing.buttonPressed() != OK_BUTTON);

        Serial.println(F("@"));
        delay(200);

        for (int i=0 ; i < NUMBER_OF_READS_TO_TAKE; ++i )
        {
            as7265xTakeReads(); 
            delay(250); 
        }
        Serial.println(F("@/"));
        delay(250);
    }*/
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

    as7265x.setIntegrationTime(157);
    as7265x.setGain(GAIN_16X);
    delay(150);
    //digitalWrite(PIN_YELLOW_LED, LOW);
    //digitalWrite(PIN_WHITE_LED, LOW);
    //delay(1000);

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
        // as7265x.drvOn();
        
        // delay(200);
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
    //as7265x.drvOff();
    //digitalWrite(PIN_YELLOW_LED, HIGH);
    //digitalWrite(PIN_WHITE_LED, HIGH);
    //delay(500);

    Serial.print(F("$,"));
    for (int i = 0; i < AS7265X_NUM_CHANNELS; i++) 
    {
        Serial.print(readings[i]);
        Serial.print(F(","));
    }
    Serial.println(F("$"));
    delay(150);

    // Additional health/debug info
    //Serial.println(F("[AS7265x]temperature read:"));
    //Serial.print(F(" (dec="));
    //Serial.print(as7265x.readTemperature());
    //Serial.println(F(")"));
    Serial.println(F("[AS7265] read complete")); 
}

// ShroomCorp sensingClass.cpp all the code here is property of ShroomCorp and cannot be used
// or distributed without written permission from ShroomCorp    

// ── inferenceProcess — AgM Inference Feature ─────────────────────────
// Implements state machine: IDLE -> WARMUP_CHECK -> TAKING_READS ->
// AVERAGING -> NORMALIZING -> TRANSMITTING -> IDLE
// Triggered by serial cmd 'M' (ARD-02). See SRS AgM_SRS_Inference_V0.4.1
void sensingClass::inferenceProcess()
{
    // ARD-09: acknowledge command immediately
    Serial.println(ACK_M_MSG);

    // ARD-07: validate white reference — abort if any channel is zero
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
    {
        if (I_WHITE_REF[i] == 0)
        {
            Serial.println(CALIB_MISSING_MSG);
            return;
        }
    }

    // ARD-08: WARMUP_CHECK — warn if lamp has not reached operating temperature
    lampStartTime = millis();
    if (lampStartTime < LAMP_WARMUP_MS)
    {
        // Non-blocking: transmit warning and continue
        Serial.println(LAMP_COLD_MSG);
    }

    // TAKING_READS — accumulate READS_PER_CHANNEL reads per channel (ARD-03)
    uint32_t accumulator[AS7265X_NUM_CHANNELS] = {0};

    for (uint8_t read = 0; read < READS_PER_CHANNEL; read++)
    {
        // Re-use existing private helper: one full AS7265x snapshot
        uint16_t snapshot[AS7265X_NUM_CHANNELS] = {0};

        if (!isAS7265xReady)
        {
            if (!as7265x.begin()) return;
            isAS7265xReady = true;
        }

        as7265x.setIntegrationTime(157);
        as7265x.setGain(GAIN_16X);
        delay(150);

        as7265x.startMeasurement();

        // Poll for data ready (5 s timeout)
        unsigned long tStart = millis();
        bool ready = false;
        while ((millis() - tStart) < 5000)
        {
            if (as7265x.dataReady()) { ready = true; break; }
            delay(100);
        }
        if (!ready) continue;  // skip this read on timeout

        as7265x.readRawValuesSequential(snapshot, 2500);

        for (uint8_t ch = 0; ch < AS7265X_NUM_CHANNELS; ch++)
            accumulator[ch] += snapshot[ch];

        delay(150);
    }

    // AVERAGING — I_avg[i] = accumulator[i] / READS_PER_CHANNEL (ARD-04)
    float iAvg[AS7265X_NUM_CHANNELS];
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
        iAvg[i] = (float)accumulator[i] / (float)READS_PER_CHANNEL;

    // NORMALIZING — R[i] = (I_avg[i] / I_WHITE_REF[i]) * 100.0 (ARD-05)
    float rNorm[AS7265X_NUM_CHANNELS];
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
        rNorm[i] = (iAvg[i] / (float)I_WHITE_REF[i]) * 100.0f;

    // TRANSMITTING — $,R410,...,R940,$ frame with 4 decimal places (ARD-06)
    Serial.print(F("$,"));
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
    {
        Serial.print(rNorm[i], 4);
        Serial.print(F(","));
    }
    Serial.println(F("$"));
}