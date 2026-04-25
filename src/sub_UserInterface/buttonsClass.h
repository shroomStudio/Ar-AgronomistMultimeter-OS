// ShroomCorp AgM OS-2.0 — buttonsClass.h
// Non-blocking button polling with falling-edge debounce.

#ifndef BUTTONSCLASS_H
#define BUTTONSCLASS_H

#include <Arduino.h>
#include <sub_UserInterface/commonDataTypes.h>
#include <sub_UserInterface/lcdDisplayClass.h>

#define DEBOUNCE_MS 50

class buttonsClass {
public:
    buttonsClass(lcdDisplayClass &lcd);
    ~buttonsClass();

    void initialButtonsSetup();

    // Call once per loop() tick — detects falling-edge press with debounce
    void poll();

    // Returns latched press and clears it (consume-once semantics)
    BUTTON_PRESSED buttonPressed();

private:
    lcdDisplayClass &_lcd;
    uint8_t         _prevStates[4];
    BUTTON_PRESSED  _current;
    unsigned long   _lastPressMs;
};

#endif // BUTTONSCLASS_H
