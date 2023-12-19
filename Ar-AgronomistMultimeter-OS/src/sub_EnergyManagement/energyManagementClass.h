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
    void voltageBatteryMonitor(void);
    void turnOffDevice(void);
    void resetDevice(void);

    private:
    //Private attributes
    int activationPinRelayOff = 13;
    int activationPinRelayBatteryCharge = 12;
    int resetDevicePin = 11;
    double voltageMeasuredBattery = 10;
    

    // Private Methods
    void initialSetUpEnergyManagament(void);
    void chargeBattery(void);
};

#endif
//end energyManagementClass