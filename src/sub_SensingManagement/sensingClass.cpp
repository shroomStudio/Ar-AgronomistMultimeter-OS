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
// New flow:
//   1. ACK
//   2. Validate white reference
//   3. WARMUP_CHECK:
//      - If lamp cold: send WARN,LAMP_COLD, wait for M (proceed) or any
//        other char (cancel, return to IDLE)
//      - If lamp ready: send READY,PRESS_M, wait for M (proceed) or 0
//        (cancel, return to IDLE)
//   4. Only on M confirmation: TAKING_READS -> AVERAGING ->
//      NORMALIZING -> TRANSMITTING
// Ref: AgM_SRS_Inference_V0.5
void sensingClass::inferenceProcess()
{
    // ARD-09: ACK command
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

    // ARD-08: WARMUP_CHECK
    unsigned long elapsed = millis() - lampStartTime;
    bool lampCold = (lampStartTime == 0 || elapsed < LAMP_WARMUP_MS);

    if (lampCold)
    {
        // Send warning and wait for user decision
        Serial.println(LAMP_COLD_MSG);
        lcdSensing.metadataTodisplayInLCD("Lamp cold warn", LEFT_ALIGNED_X, MIDDLE_Y, true);
    }
    else
    {
        // Lamp ready — send ready prompt and wait for user decision
        Serial.println(F("READY,PRESS_M"));
        lcdSensing.metadataTodisplayInLCD("Ready. Wait M...", LEFT_ALIGNED_X, MIDDLE_Y, true);
    }

    // Wait for M (proceed) or any other char (cancel) — 60 s timeout
    unsigned long waitStart = millis();
    char userCmd = 0;
    while ((millis() - waitStart) < 60000UL)
    {
        if (Serial.available() > 0)
        {
            userCmd = (char)Serial.read();
            break;
        }
        delay(50);
    }

    if (userCmd != 'M')
    {
        // User cancelled or timeout
        Serial.println(F("CANCELLED"));
        lcdSensing.metadataTodisplayInLCD("AgM Ready", LEFT_ALIGNED_X, MIDDLE_Y, true);
        return;
    }

    // User confirmed with M — proceed with measurement
    Serial.println(F("ACK,M_CONFIRMED"));

    // Turn lamp ON before taking reads (requires 2s stabilisation)
    digitalWrite(PIN_YELLOW_LED, LOW);
    digitalWrite(PIN_WHITE_LED,  LOW);
    lcdSensing.metadataTodisplayInLCD("Lamp ON...", LEFT_ALIGNED_X, MIDDLE_Y, true);
    delay(2000);   // 2s lamp stabilisation before first read

    lcdSensing.metadataTodisplayInLCD("Taking reads...", LEFT_ALIGNED_X, MIDDLE_Y, true);

    // TAKING_READS — 10 reads per channel (ARD-03)
    uint32_t accumulator[AS7265X_NUM_CHANNELS] = {0};
    uint8_t  validReads = 0;

    for (uint8_t r = 0; r < READS_PER_CHANNEL; r++)
    {
        // Progress update so Python knows Arduino is alive
        Serial.print(F("MEASURING,"));
        Serial.print(r + 1);
        Serial.print(F("/"));
        Serial.println(READS_PER_CHANNEL);

        if (!isAS7265xReady)
        {
            if (!as7265x.begin()) continue;
            isAS7265xReady = true;
        }

        as7265x.setIntegrationTime(157);
        as7265x.setGain(GAIN_16X);
        delay(150);
        as7265x.startMeasurement();

        unsigned long tStart = millis();
        bool ready = false;
        while ((millis() - tStart) < 8000)   // 8s timeout per read
        {
            if (as7265x.dataReady()) { ready = true; break; }
            delay(200);
        }
        if (!ready)
        {
            Serial.println(F("WARN,READ_TIMEOUT"));
            continue;
        }

        uint16_t snapshot[AS7265X_NUM_CHANNELS] = {0};
        as7265x.readRawValuesSequential(snapshot, 2500);

        for (uint8_t ch = 0; ch < AS7265X_NUM_CHANNELS; ch++)
            accumulator[ch] += snapshot[ch];

        validReads++;
        delay(100);
    }

    if (validReads == 0)
    {
        Serial.println(F("ERR,NO_READS"));
        // Turn lamp off — safety
        digitalWrite(PIN_YELLOW_LED, HIGH);
        digitalWrite(PIN_WHITE_LED,  HIGH);
        lcdSensing.metadataTodisplayInLCD("AgM Ready", LEFT_ALIGNED_X, MIDDLE_Y, true);
        return;
    }

    // AVERAGING (ARD-04) — divide by actual valid reads
    float iAvg[AS7265X_NUM_CHANNELS];
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
        iAvg[i] = (float)accumulator[i] / (float)validReads;

    // NORMALIZING (ARD-05)
    float rNorm[AS7265X_NUM_CHANNELS];
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
        rNorm[i] = (iAvg[i] / (float)I_WHITE_REF[i]) * 100.0f;

    // TRANSMITTING (ARD-06)
    lcdSensing.metadataTodisplayInLCD("Sending data...", LEFT_ALIGNED_X, MIDDLE_Y, true);

    Serial.print(F("$,"));
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++)
    {
        Serial.print(rNorm[i], 4);
        Serial.print(F(","));
    }
    Serial.println(F("$"));

    // Turn lamp off — lamp must be shut down at end of every sensing process
    digitalWrite(PIN_YELLOW_LED, HIGH);
    digitalWrite(PIN_WHITE_LED,  HIGH);
    lampStartTime = 0;   // reset warmup timer

    lcdSensing.metadataTodisplayInLCD("AgM Ready", LEFT_ALIGNED_X, MIDDLE_Y, true);
}

// ShroomCorp — Copyright
