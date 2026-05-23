// ShroomCorp sensingClass.cpp — all interaction via serial console (inference branch)
// Button-wait loops removed; LCD used for status messages only.
// Copyright

#include "sensingClass.h"

// File-scope variables
bool          CalibrationIsDone  = false;
unsigned long lampStartTime      = 0;  // set when cmd L is received

sensingClass::sensingClass(lcdDisplayClass &lcd,
                            buttonsClass &buttons,
                            signalConditioningClass &conditioning)
    : lcdSensing(lcd),
      buttonsSensing(buttons),
      conditioningSensing(conditioning),
      as7265x()
{
    // Constructor
}

sensingClass::~sensingClass()
{
    // Destructor
}

// ── macronutrientSensingProcess ───────────────────────────────────────
// Legacy method — kept for build compatibility.
// All sensing is now triggered via inferenceProcess() through serial cmd M.
void sensingClass::macronutrientSensingProcess()
{
    lcdSensing.metadataTodisplayInLCD("Use serial cmd M", LEFT_ALIGNED_X, MIDDLE_Y, true);
}

// ── sensingProcessTakeReadings ────────────────────────────────────────
// Legacy helper — kept for build compatibility.
void sensingClass::sensingProcessTakeReadings(void)
{
    // Replaced by inferenceProcess()
}

// ── as7265xTakeReads ─────────────────────────────────────────────────
// One complete AS7265x snapshot. Initialises sensor if needed.
// Transmits $,v1,...,v18,$ frame on success.
void sensingClass::as7265xTakeReads(void)
{
    if (!isAS7265xReady)
    {
        if (!as7265x.begin()) return;
        isAS7265xReady = true;
    }

    as7265x.setIntegrationTime(157);
    as7265x.setGain(GAIN_16X);
    delay(150);

    as7265x.startMeasurement();

    // Poll for data ready — 5 s timeout
    unsigned long tStart = millis();
    bool ready = false;
    while ((millis() - tStart) < 5000)
    {
        if (as7265x.dataReady()) { ready = true; break; }
        delay(100);
    }

    if (!ready)
    {
        as7265x.drvOff();
        delay(200);
        as7265x.startMeasurement();
        for (int i = 0; i < 10; i++)
        {
            if (as7265x.dataReady()) { ready = true; break; }
            delay(200);
        }
        if (!ready) { as7265x.drvOff(); return; }
    }

    uint16_t readings[AS7265X_NUM_CHANNELS] = {0};
    as7265x.readRawValuesSequential(readings, 2500);

    Serial.print(F("$,"));
    for (int i = 0; i < AS7265X_NUM_CHANNELS; i++)
    {
        Serial.print(readings[i]);
        Serial.print(F(","));
    }
    Serial.println(F("$"));
    delay(150);
}

// ── inferenceProcess ─────────────────────────────────────────────────
// Triggered by serial cmd 'M'.
// State machine: WARMUP_CHECK -> TAKING_READS -> AVERAGING ->
//                NORMALIZING -> TRANSMITTING
// Ref: AgM_SRS_Inference_V0.5
void sensingClass::inferenceProcess()
{
    // ACK command (ARD-09)
    Serial.println(ACK_M_MSG);

    // ARD-07: validate white reference
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
    {
        if (I_WHITE_REF[i] == 0)
        {
            Serial.println(CALIB_MISSING_MSG);
            lcdSensing.metadataTodisplayInLCD("Calib missing", LEFT_ALIGNED_X, MIDDLE_Y, true);
            return;
        }
    }

    // ARD-08: WARMUP_CHECK — warn if lamp on time < 10 min
    // lampStartTime is set in main.cpp when cmd L is received
    unsigned long elapsed = millis() - lampStartTime;
    if (lampStartTime == 0 || elapsed < LAMP_WARMUP_MS)
    {
        Serial.println(LAMP_COLD_MSG);  // non-blocking — continue anyway
    }

    // TAKING_READS — 10 reads accumulated per channel (ARD-03)
    lcdSensing.metadataTodisplayInLCD("Taking reads...", LEFT_ALIGNED_X, MIDDLE_Y, true);

    uint32_t accumulator[AS7265X_NUM_CHANNELS] = {0};

    for (uint8_t r = 0; r < READS_PER_CHANNEL; r++)
    {
        if (!isAS7265xReady)
        {
            if (!as7265x.begin()) return;
            isAS7265xReady = true;
        }

        as7265x.setIntegrationTime(157);
        as7265x.setGain(GAIN_16X);
        delay(150);
        as7265x.startMeasurement();

        unsigned long tStart = millis();
        bool ready = false;
        while ((millis() - tStart) < 5000)
        {
            if (as7265x.dataReady()) { ready = true; break; }
            delay(100);
        }
        if (!ready) continue;

        uint16_t snapshot[AS7265X_NUM_CHANNELS] = {0};
        as7265x.readRawValuesSequential(snapshot, 2500);

        for (uint8_t ch = 0; ch < AS7265X_NUM_CHANNELS; ch++)
            accumulator[ch] += snapshot[ch];

        delay(150);
    }

    // AVERAGING (ARD-04)
    float iAvg[AS7265X_NUM_CHANNELS];
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
        iAvg[i] = (float)accumulator[i] / (float)READS_PER_CHANNEL;

    // NORMALIZING (ARD-05)
    float rNorm[AS7265X_NUM_CHANNELS];
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
        rNorm[i] = (iAvg[i] / (float)I_WHITE_REF[i]) * 100.0f;

    // TRANSMITTING — $,R410,...,R940,$ (ARD-06)
    lcdSensing.metadataTodisplayInLCD("Sending data...", LEFT_ALIGNED_X, MIDDLE_Y, true);

    Serial.print(F("$,"));
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
    {
        Serial.print(rNorm[i], 4);
        Serial.print(F(","));
    }
    Serial.println(F("$"));

    lcdSensing.metadataTodisplayInLCD("Done.", LEFT_ALIGNED_X, MIDDLE_Y, true);
}

// ShroomCorp — Copyright
