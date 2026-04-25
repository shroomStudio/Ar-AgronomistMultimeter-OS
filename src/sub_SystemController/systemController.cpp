// ShroomCorp AgM OS-2.0 — systemController.cpp

#include "systemController.h"

// Lamp warm-up before calibration.
// Change to 30000UL for bench testing.
#define WARM_UP_DURATION_MS 600000UL

SystemController::SystemController() :
    _state(STATE_INIT),
    _stateEntryMs(0),
    _measuring(false),
    _buttons(_lcd),
    _energy(_lcd),
    _conditioning(_lcd, _buttons),
    _sensing(_lcd)
{
    memset(&_data, 0, sizeof(_data));
}

void SystemController::begin()
{
    pinMode(PIN_BLUE_LED,   OUTPUT);
    pinMode(PIN_RED_LED,    OUTPUT);
    pinMode(PIN_YELLOW_LED, OUTPUT);
    pinMode(PIN_WHITE_LED,  OUTPUT);

    digitalWrite(PIN_BLUE_LED,   HIGH);
    digitalWrite(PIN_RED_LED,    HIGH);
    digitalWrite(PIN_YELLOW_LED, HIGH);
    digitalWrite(PIN_WHITE_LED,  HIGH);

    Serial.begin(115200);
    _lcd.initialDisplaySetup();
    _energy.initialSetUpEnergyManagament();
    _buttons.initialButtonsSetup();
    _stateEntryMs = millis();
}

void SystemController::run()
{
    _buttons.poll();

    switch (_state) {
        case STATE_INIT:           handleInit();          break;
        case STATE_WARM_UP:        handleWarmUp();        break;
        case STATE_CALIBRATE:      handleCalibrate();     break;
        case STATE_IDLE:           handleIdle();          break;
        case STATE_MEASURE:        handleMeasure();       break;
        case STATE_INFER:          handleInfer();         break;
        case STATE_DISPLAY_RESULT: handleDisplayResult(); break;
        case STATE_LOW_BATTERY:    handleLowBattery();    break;
        case STATE_ERROR:          handleError();         break;
    }
}

void SystemController::transitionTo(AgMState next)
{
    _state        = next;
    _stateEntryMs = millis();
    _measuring    = false;
}

// ── STATE HANDLERS ────────────────────────────────────────────────────────────

void SystemController::handleInit()
{
    if (_energy.batteryChargePercentage < MINIMUM_BATTERY_PERCENTAGE) {
        transitionTo(STATE_LOW_BATTERY);
        return;
    }
    transitionTo(STATE_WARM_UP);
}

void SystemController::handleWarmUp()
{
    unsigned long elapsed = millis() - _stateEntryMs;

    // Turn lamp on once at entry
    if (!_measuring) {
        digitalWrite(PIN_YELLOW_LED, LOW);
        digitalWrite(PIN_WHITE_LED,  LOW);
        _measuring = true;
    }

    if (elapsed < WARM_UP_DURATION_MS) {
        // Refresh LCD every 5 s to show countdown without flicker
        if (elapsed % 5000UL < 100UL) {
            uint16_t remaining = (uint16_t)((WARM_UP_DURATION_MS - elapsed) / 1000UL);
            _lcd.metadataTodisplayInLCD("Lamp warming up...", LEFT_ALIGNED_X, TOP_Y, true);
            _lcd.intNumberTodisplayInLCD(remaining, LEFT_ALIGNED_X, MIDDLE_Y, false);
        }
        return; // non-blocking wait
    }

    digitalWrite(PIN_YELLOW_LED, HIGH);
    digitalWrite(PIN_WHITE_LED,  HIGH);
    _lcd.metadataTodisplayInLCD("Lamp ready.", LEFT_ALIGNED_X, MIDDLE_Y, true);
    transitionTo(STATE_CALIBRATE);
}

void SystemController::handleCalibrate()
{
    if (!_measuring) {
        _lcd.metadataTodisplayInLCD("OK: take white ref", LEFT_ALIGNED_X, MIDDLE_Y, true);
        if (_buttons.buttonPressed() != OK_BUTTON) return;

        resetAccumulator();
        _lcd.metadataTodisplayInLCD("Calibrating...", LEFT_ALIGNED_X, TOP_Y, true);
        digitalWrite(PIN_YELLOW_LED, LOW);
        digitalWrite(PIN_WHITE_LED,  LOW);
        Serial.println(F("/*"));
        _measuring = true;
    }

    uint16_t readings[AS7265X_NUM_CHANNELS] = {0};
    if (_sensing.takeSingleReading(readings)) {
        accumulateReading(readings);
        _data.readCount++;
        printRawFrame(readings);
    } else {
        transitionTo(STATE_ERROR);
        return;
    }

    if (_data.readCount >= NUMBER_OF_READS_TO_TAKE) {
        averageAccumulator(_data.calibRef);
        Serial.println(F("*/"));
        digitalWrite(PIN_YELLOW_LED, HIGH);
        digitalWrite(PIN_WHITE_LED,  HIGH);
        _lcd.metadataTodisplayInLCD("Calibration done.", LEFT_ALIGNED_X, MIDDLE_Y, true);
        delay(800);
        transitionTo(STATE_IDLE);
    }
}

void SystemController::handleIdle()
{
    _lcd.metadataTodisplayInLCD("OK: measure sample", LEFT_ALIGNED_X, MIDDLE_Y, true);
    if (_buttons.buttonPressed() == OK_BUTTON) {
        transitionTo(STATE_MEASURE);
    }
}

void SystemController::handleMeasure()
{
    if (!_measuring) {
        resetAccumulator();
        _lcd.metadataTodisplayInLCD("Measuring...", LEFT_ALIGNED_X, TOP_Y, true);
        digitalWrite(PIN_YELLOW_LED, LOW);
        digitalWrite(PIN_WHITE_LED,  LOW);
        Serial.println(F("@"));
        _measuring = true;
    }

    uint16_t readings[AS7265X_NUM_CHANNELS] = {0};
    if (_sensing.takeSingleReading(readings)) {
        accumulateReading(readings);
        _data.readCount++;
        printRawFrame(readings);
    } else {
        transitionTo(STATE_ERROR);
        return;
    }

    if (_data.readCount >= NUMBER_OF_READS_TO_TAKE) {
        averageAccumulator(_data.sampleAvg);
        Serial.println(F("@/"));
        digitalWrite(PIN_YELLOW_LED, HIGH);
        digitalWrite(PIN_WHITE_LED,  HIGH);
        transitionTo(STATE_INFER);
    }
}

void SystemController::handleInfer()
{
    // Compute R(λ) = sampleAvg / calibRef * 100 for the 4 feature channels only
    auto normalize = [&](uint8_t ch) -> float {
        if (_data.calibRef[ch] == 0) return 0.0f;
        return (float)_data.sampleAvg[ch] / (float)_data.calibRef[ch] * 100.0f;
    };

    float r410 = normalize(CH_410NM);
    float r460 = normalize(CH_460NM);
    float r535 = normalize(CH_535NM);
    float r645 = normalize(CH_645NM);
    float r810 = normalize(CH_810NM);

    _data.nIndex   = (r645 > 0.0f) ? r460 / r645 : 0.0f;
    _data.pIndex   = (r535 > 0.0f) ? r460 / r535 : 0.0f;
    _data.kIndex   = (r645 > 0.0f) ? r810 / r645 : 0.0f;
    _data.darkness = r410;

    // TODO: Decision Tree + Centroid Matching inference
    // Placeholder: forward indices to display
    Serial.print(F("#N=")); Serial.print(_data.nIndex, 4);
    Serial.print(F(",P="));  Serial.print(_data.pIndex, 4);
    Serial.print(F(",K="));  Serial.print(_data.kIndex, 4);
    Serial.print(F(",D="));  Serial.println(_data.darkness, 4);

    transitionTo(STATE_DISPLAY_RESULT);
}

void SystemController::handleDisplayResult()
{
    // TODO: display classified N/P/K levels once inference is implemented
    _lcd.metadataTodisplayInLCD("N/P/K computed.", LEFT_ALIGNED_X, TOP_Y, true);
    _lcd.metadataTodisplayFreeCursor("OK: new sample", LEFT_ALIGNED_X, BOTTOM_Y, false);

    if (_buttons.buttonPressed() == OK_BUTTON) {
        transitionTo(STATE_IDLE);
    }
}

void SystemController::handleLowBattery()
{
    _lcd.metadataTodisplayInLCD("Low battery.", LEFT_ALIGNED_X, MIDDLE_Y, true);
    delay(1500);
    _energy.turnOffDevice();
}

void SystemController::handleError()
{
    _lcd.metadataTodisplayInLCD("Sensor error.", LEFT_ALIGNED_X, TOP_Y, true);
    _lcd.metadataTodisplayFreeCursor("Reset device.", LEFT_ALIGNED_X, BOTTOM_Y, false);
    // Stay here — hardware reset required
}

// ── PIPELINE HELPERS ──────────────────────────────────────────────────────────

void SystemController::resetAccumulator()
{
    memset(_data.accumulator, 0, sizeof(_data.accumulator));
    _data.readCount = 0;
}

void SystemController::accumulateReading(const uint16_t readings[AS7265X_NUM_CHANNELS])
{
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) {
        _data.accumulator[i] += readings[i];
    }
}

void SystemController::averageAccumulator(uint16_t dst[AS7265X_NUM_CHANNELS])
{
    if (_data.readCount == 0) return;
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) {
        dst[i] = (uint16_t)(_data.accumulator[i] / _data.readCount);
    }
}

void SystemController::printRawFrame(const uint16_t readings[AS7265X_NUM_CHANNELS])
{
    Serial.print(F("$,"));
    for (uint8_t i = 0; i < AS7265X_NUM_CHANNELS; i++) {
        Serial.print(readings[i]);
        Serial.print(F(","));
    }
    Serial.println(F("$"));
}
