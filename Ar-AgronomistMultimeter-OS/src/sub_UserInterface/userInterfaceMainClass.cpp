// ShroomCorp
// main.cpp
// Description
// Copyright*
// System PinOut

/*
Pin13   clockLcdPin
Pin12   dinLcdPin
Pin11   dcLcdPin
Pin10   LcdPin 
Pin9    resetLcdPin
Pin9    lightLcdPin
Pin8    OK Push Button      Pull-Up Resistor
Pin7    Up Push Button      Pull-Up Resistor
Pin6    Down Push Button    Pull-Up Resistor
Pin5    Back Push Button    Pull-Up Resistor
Pin4    Reset Device Pin
Pin3    Activation Pin Relay Battery Charge
Pin2    Activation Pin Relay Off
*/

//Headers 
#include <Arduino.h>
#include <string.h>
#include <sub_EnergyManagement/energyManagementClass.h>
#include "lcdDisplayClass.h"
#include "buttonsClass.h"
#include "usbConecttionClass.h"
#include <sub_SensingManagement/sensingClass.h>
#include <sub_SignalConditioning/signalConditioningClass.h>

//Defines
#define MINIMUM_BATTERY_PERCENTAGE 2
#define ZERO 0
#define INDEX_PLUS_ONE 1
#define INDEX_LESS_ONE -1


// Class instances
energyManagementClass energy;
lcdDisplayClass lcd;
buttonsClass buttons;
usbConecttionClass usb;
sensingClass sensing;
signalConditioningClass conditioningSignals;

//Namespaces 
using namespace std;

//Local Function Prototypes
void userInitialConfiguration(void);

//Global Variables 
bool initialConfigurationDone = false;


void setup() 
{
    if (false == initialConfigurationDone)
    {
        /* User Initial Confiuration */
         userInitialConfiguration();
    }
    
}

void loop()
{

    energy.voltageBatteryMonitor();


}




//Local Functions 
void userInitialConfiguration(void)
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
             lcd.metadataTodisplayInLCD("Time & Zone - Menu",CENTERED_X,TOP_Y);
             lcd.metadataTodisplayInLCDAdvanceCursor("Date",LEFT_ALIGNED_X,TOP_Y,0,3);
             lcd.metadataTodisplayInLCDAdvanceCursor("Date",LEFT_ALIGNED_X,TOP_Y,0,3);
        }
    }


}
