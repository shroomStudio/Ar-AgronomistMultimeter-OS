// ShroomCorp AgM OS-2.0 — sensingClass.h
// Thin OOP wrapper over the AS7265x driver.
// Multi-read orchestration and UI interaction belong to SystemController.

/*
 AS7265x wiring (GY-276x breakout):
   VIN → 3.3V or 5V    GND → GND
   SCL → A5 (I2C clock)    SDA → A4 (I2C data)
   SET → GND (selects I2C mode)
*/

#ifndef SENSINGCLASS_H
#define SENSINGCLASS_H

#include <Arduino.h>
#include <sub_UserInterface/lcdDisplayClass.h>
#include "ShroomCorp_AS7265x.h"

class sensingClass {
public:
    sensingClass(lcdDisplayClass &lcd);
    ~sensingClass();

    // Initialise the AS7265x; safe to call multiple times.
    bool begin();

    // Block until one 18-channel reading completes (~500 ms).
    // Returns false on sensor timeout or init failure.
    bool takeSingleReading(uint16_t readings[AS7265X_NUM_CHANNELS]);

private:
    bool               _isReady;
    lcdDisplayClass   &_lcd;
    ShroomCorp_AS7265x _as7265x;
};

#endif // SENSINGCLASS_H
