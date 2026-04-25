// ShroomCorp AgM OS-2.0 — buttonsClass.cpp

#include "buttonsClass.h"

// Pin order matches BUTTON_PRESSED enum mapping below
static const uint8_t kPins[4] = {
    pinOkButton, pinUpButton, pinDownButton, pinBackButton
};
static const BUTTON_PRESSED kMap[4] = {
    OK_BUTTON, UP_BUTTON, DOWN_BUTTON, BACK_BUTTON
};

buttonsClass::buttonsClass(lcdDisplayClass &lcd) :
    _lcd(lcd), _current(NO_BUTTON), _lastPressMs(0)
{
    memset(_prevStates, HIGH, sizeof(_prevStates));
}

buttonsClass::~buttonsClass() {}

void buttonsClass::initialButtonsSetup()
{
    for (uint8_t i = 0; i < 4; i++) {
        pinMode(kPins[i], INPUT_PULLUP);
        _prevStates[i] = HIGH;
    }
}

void buttonsClass::poll()
{
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t state = digitalRead(kPins[i]);

        // Falling edge = button pressed
        if (state == LOW && _prevStates[i] == HIGH) {
            if (millis() - _lastPressMs > DEBOUNCE_MS) {
                _lastPressMs = millis();
                _current     = kMap[i];
            }
        }
        _prevStates[i] = state;
    }
}

BUTTON_PRESSED buttonsClass::buttonPressed()
{
    BUTTON_PRESSED b = _current;
    _current = NO_BUTTON;
    return b;
}
