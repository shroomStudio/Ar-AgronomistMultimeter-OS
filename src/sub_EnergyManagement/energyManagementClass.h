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
    int activationPinRelayOff = 2;
    int activationPinRelayBatteryCharge = 3;
    int resetDevicePin = 4;
    double voltageMeasuredBattery;
    

    // Private Methods
    void initialSetUpEnergyManagament(void);
    void chargeBattery(void);
};

#endif
//end energyManagementClass