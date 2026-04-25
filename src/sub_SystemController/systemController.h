// ShroomCorp AgM OS-2.0 — systemController.h
// FSM backbone. Owns all subsystem objects and sequences them through
// the measurement pipeline: INIT → WARM_UP → CALIBRATE → IDLE → MEASURE → INFER → DISPLAY

#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include <Arduino.h>
#include "agmData.h"
#include <sub_UserInterface/commonDataTypes.h>
#include <sub_UserInterface/lcdDisplayClass.h>
#include <sub_UserInterface/buttonsClass.h>
#include <sub_EnergyManagement/energyManagementClass.h>
#include <sub_SensingManagement/sensingClass.h>
#include <sub_SignalConditioning/signalConditioningClass.h>

enum AgMState : uint8_t {
    STATE_INIT           = 0,
    STATE_WARM_UP        = 1,
    STATE_CALIBRATE      = 2,
    STATE_IDLE           = 3,
    STATE_MEASURE        = 4,
    STATE_INFER          = 5,
    STATE_DISPLAY_RESULT = 6,
    STATE_LOW_BATTERY    = 7,
    STATE_ERROR          = 8
};

class SystemController {
public:
    SystemController();
    void begin();
    void run();

private:
    AgMState      _state;
    AgMData       _data;
    unsigned long _stateEntryMs;
    bool          _measuring;

    // Declared in strict init order — C++ initialises members by declaration order
    lcdDisplayClass         _lcd;
    buttonsClass            _buttons;
    energyManagementClass   _energy;
    signalConditioningClass _conditioning;
    sensingClass            _sensing;

    void transitionTo(AgMState next);

    void handleInit();
    void handleWarmUp();
    void handleCalibrate();
    void handleIdle();
    void handleMeasure();
    void handleInfer();
    void handleDisplayResult();
    void handleLowBattery();
    void handleError();

    void resetAccumulator();
    void accumulateReading(const uint16_t readings[AS7265X_NUM_CHANNELS]);
    void averageAccumulator(uint16_t dst[AS7265X_NUM_CHANNELS]);
    void printRawFrame(const uint16_t readings[AS7265X_NUM_CHANNELS]);
};

#endif // SYSTEM_CONTROLLER_H
