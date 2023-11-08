#ifndef ENERGYMANAGEMENT_H
#define ENERGYMANAGEMENT_H

#include <Arduino.h>

using namespace std;
class energyManagementClass{
    public:
    energyManagementClass();
    ~energyManagementClass();
    
    //Public attributes
    int batteryChargePercentage;

    //Public Methods
    void turnOffDevice(bool trueFalse);

    private:
    //Private attributes
    int activationPinRelayOff;
    int activationPinRelayBatteryCharge;
    int voltageMeasuredBattery;
    double voltageMeasured;

    // Private Methods
    void voltageBatteryMonitor(void);
    void chargeBattery(void);
};

#endif
//end energyManagementClass