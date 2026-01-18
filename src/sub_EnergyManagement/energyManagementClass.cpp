// ShroomCorp energyManagementClass.cpp all the code here is property of ShroomCorp and cannot be used
// or distributed without written permission from ShroomCorp

#include "energyManagementClass.h"

//definition
#define MINIMUM_BATTERY_PERCENTAJE 2

energyManagementClass::energyManagementClass(lcdDisplayClass& lcd):
    lcdEnergy(lcd)
{
    //Constructor energyManagementClass
    //Pin to keep the system On it works in conjuntion with
    //the external button ON/OFF
    pinMode(activationPinRelayOff, OUTPUT);
    //Pin to start chargin the battery using USB charger
    pinMode(activationPinRelayBatteryCharge, OUTPUT);
    //Pin to reset the device this pin must be connected to PC6
    pinMode(resetDevicePin, OUTPUT);
    //Pin to read the voltage in the battery
    pinMode(voltageMeasuredBattery, INPUT);

    //Start the monitoring of the battery 
    voltageBatteryMonitor();
}

energyManagementClass::~energyManagementClass()
{
    //Destructor energyManagementClass
}

//Iniial SetUp Method
void energyManagementClass::initialSetUpEnergyManagament(void)
{

    lcdEnergy.metadataTodisplayInLCD("Verifying Battery charge\n",LEFT_ALIGNED_X,MIDDLE_Y,true);
    delay(150);
    digitalWrite(activationPinRelayOff, LOW);
    delay(100); 
    digitalWrite(activationPinRelayOff, HIGH);
    delay(100);    
   
    if(batteryChargePercentage <= MINIMUM_BATTERY_PERCENTAJE)
   {
    // Inform te user the system will be shut down
    lcdEnergy.metadataTodisplayInLCD("ystem Discharged, please connect device to power source\n",LEFT_ALIGNED_X,MIDDLE_Y,true);
    delay(500);  
    turnOffDevice();
   }
   
   lcdEnergy.metadataTodisplayInLCD("",LEFT_ALIGNED_X,MIDDLE_Y,true);
}

//Public Methods
void energyManagementClass::turnOffDevice(void)
{
    delay(500);  
    digitalWrite(activationPinRelayOff, LOW);
    delay(500);  
}

void energyManagementClass::resetDevice(void)
{
    delay(200);  
    digitalWrite(activationPinRelayOff, HIGH);
    delay(500); 
    digitalWrite(activationPinRelayOff, LOW);
    delay(500); 
}

void energyManagementClass::voltageBatteryMonitor(void)
{
    batteryChargePercentage = 80; //(voltageMeasuredBattery * 100) / 5;
}

void energyManagementClass::chargeBattery(void)
{
    
}

// ShroomCorp energyManagementClass.cpp all the code here is property of ShroomCorp and cannot be used
// or distributed without written permission from ShroomCorp