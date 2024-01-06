#include "userInterfaceClass.h"
#include <sub_EnergyManagement/energyManagementClass.h>
#include "lcdDisplayClass.h"
#include "buttonsClass.h"
#include "usbConecttionClass.h"
#include <sub_SensingManagement/sensingClass.h>
#include <sub_SignalConditioning/signalConditioningClass.h>

// Class instances.
energyManagementClass energy;
lcdDisplayClass lcd;
buttonsClass buttons;
usbConecttionClass usb;
sensingClass sensing;
signalConditioningClass conditioningSignals;

//Global Variables 
bool initialConfigurationDone = false;

userInterfaceClass::userInterfaceClass(){
    // Constructor class usbConecttionClass

    energy.voltageBatteryMonitor();
}

userInterfaceClass::~userInterfaceClass(){
    //Destructor class usbConecttionClass
}


//Local Functions 
void userInterfaceClass::userInitialConfiguration(void)
{
    if(false == initialConfigurationDone)
    {
        if (energy.batteryChargePercentage < MINIMUM_BATTERY_PERCENTAGE)
        {
            lcd.metadataTodisplayInLCD("System discharged please plug in to source power", LEFT_ALIGNED_X, MIDDLE_Y);
            delay(2000);
            energy.turnOffDevice();
        }
        else
        {
            lcd.metadataTodisplayInLCD("Initial Setup",CENTERED_X,TOP_Y);
            lcd.metadataTodisplayInLCD("Time & Zone",LEFT_ALIGNED_X,MIDDLE_Y);
            delay(1000);
            if (buttons.buttonPressed() == OK_BUTTON )
            {
            // time and Zone Main Menu
                lcd.metadataTodisplayInLCD("Time & Zone - Menu",CENTERED_X,TOP_Y);
                lcd.metadataTodisplayInLCDAdvanceCursor("Date",LEFT_ALIGNED_X,TOP_Y,0,3);
                lcd.metadataTodisplayInLCDAdvanceCursor("MM/DD/YYYY",LEFT_ALIGNED_X,TOP_Y,3,3);
                lcd.metadataTodisplayInLCDAdvanceCursor("TIME",LEFT_ALIGNED_X,TOP_Y,0,4);
                lcd.metadataTodisplayInLCDAdvanceCursor("00:00",LEFT_ALIGNED_X,TOP_Y,3,4);
                lcd.metadataTodisplayInLCDAdvanceCursor("TIME ZONE",LEFT_ALIGNED_X,TOP_Y,0,5);
                lcd.metadataTodisplayInLCDAdvanceCursor("AMERICAS",LEFT_ALIGNED_X,TOP_Y,3,5);
                lcd.metadataTodisplayInLCDAdvanceCursor("TIME ZONE",LEFT_ALIGNED_X,TOP_Y,0,5);
                lcd.metadataTodisplayInLCDAdvanceCursor("AMERICAS",LEFT_ALIGNED_X,TOP_Y,3,5);
                lcd.metadataTodisplayInLCDAdvanceCursor("Save and continue",LEFT_ALIGNED_X,TOP_Y,0,6);
            }
        }
    }  
}
