#ifndef ENERGYMANAGEMENT_H
#define ENERGYMANAGEMENT_H

#include <Arduino.h>
#include "sub_UserInterface/lcdDisplayClass.h"

using namespace std;

class energyManagementClass{
    public:
    energyManagementClass(lcdDisplayClass& lcd);
    ~energyManagementClass();
    
    //Public attributes
    int batteryChargePercentage;
    //Public Methods
    void initialSetUpEnergyManagament(void);
    void voltageBatteryMonitor(void);
    void turnOffDevice(void);
    void resetDevice(void);

    private:
    //Private attributes
    int activationPinRelayOff           = 2;
    int activationPinRelayBatteryCharge = 3;
    int resetDevicePin                  = 4;
    double voltageMeasuredBattery;
    // Private Methods
    void chargeBattery(void);
    lcdDisplayClass &lcdEnergy; // Reference to the LCD display class for displaying messages
};

#endif
//end energyManagementClass