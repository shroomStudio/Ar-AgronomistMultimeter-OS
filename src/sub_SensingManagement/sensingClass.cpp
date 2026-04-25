// ShroomCorp AgM OS-2.0 — sensingClass.cpp

#include "sensingClass.h"
#include <sub_UserInterface/commonDataTypes.h>

sensingClass::sensingClass(lcdDisplayClass &lcd) :
    _isReady(false),
    _lcd(lcd),
    _as7265x()
{}

sensingClass::~sensingClass() {}

bool sensingClass::begin()
{
    if (_isReady) return true;
    if (!_as7265x.begin()) return false;

    _as7265x.setIntegrationTime(157);
    _as7265x.setGain(GAIN_64X);
    _isReady = true;
    return true;
}

bool sensingClass::takeSingleReading(uint16_t readings[AS7265X_NUM_CHANNELS])
{
    if (!_isReady && !begin()) return false;

    _as7265x.startMeasurement();

    unsigned long t0 = millis();
    while (!_as7265x.dataReady()) {
        if (millis() - t0 > 5000UL) {
            _as7265x.drvOff();
            return false;
        }
        delay(100);
    }

    _as7265x.readRawValuesSequential(readings, 2500);
    return true;
}
