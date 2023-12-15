#include "energyManagementClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"

//definition
#define MINIMUM_BATTERY_PERCENTAJE 2

//Class Instances 
lcdDisplayClass lcdEnergy;

energyManagementClass::energyManagementClass(){
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
}

energyManagementClass::~energyManagementClass(){

}

//Iniial SetUp Method
void energyManagementClass::initialSetUpEnergyManagament(void)
{
    // Disable restart Pin
    digitalWrite(activationPinRelayOff, LOW);
    //Waits for a second
    delay(1000); 
    //Keep device alive
    digitalWrite(activationPinRelayOff, HIGH);
    //Waits for a second
    delay(1000); 

    //Initial read of battery
    voltageBatteryMonitor();
   
   //Verify battery is above minimum valid value
   if(batteryChargePercentage <= MINIMUM_BATTERY_PERCENTAJE)
   {
    // Inform te user the system will be shut down
    lcdEnergy.metadataTodisplayInLCD("System Discharged, please connect device to power source");
    //Wait 2 seconds before to shut down the device
    delay(2000);  
    //shutdown device
    turnOffDevice();
   }
}

//Public Methods
void energyManagementClass::turnOffDevice(void)
{
    lcdEnergy.metadataTodisplayInLCD("Shutting down device - See you");
    //Wait 2 seconds before to shut down the device
    delay(2000);  
    //shutdown device
    digitalWrite(activationPinRelayOff, LOW);
    delay(1000);  
}
void energyManagementClass::resetDevice(void)
{
    lcdEnergy.metadataTodisplayInLCD("Restarting device - Please wait");
    delay(2000);  
    //restartng device
    digitalWrite(activationPinRelayOff, HIGH);
    //Waits for a second
    delay(1000); 
    //shutdown device
    digitalWrite(activationPinRelayOff, LOW);
    //Waits for a second
    delay(1000); 
}


 // Private Methods
void energyManagementClass::voltageBatteryMonitor(void)
{
    //The maximum value of the battery is 5v so based it 
    //we can converted thi value to percentage using 
    //percentage of battery = (voltage measured * 100) / 5 
    batteryChargePercentage = (voltageMeasuredBattery * 100) / 5;
}

void energyManagementClass::chargeBattery(void)
{
    
}