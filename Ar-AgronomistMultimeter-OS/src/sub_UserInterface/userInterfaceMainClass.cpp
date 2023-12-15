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

// Class instances
energyManagementClass energy;
lcdDisplayClass lcd;
buttonsClass buttons;
usbConecttionClass usb;
sensingClass sensing;
signalConditioningClass conditioningSignals;

//Namespaces 
using namespace std;



void setup() 
{
    //Verifying state of battery first start 
    //or Hardreboot
    energy.initialSetUpEnergyManagament();
    
}

void loop()
{

    energy.voltageBatteryMonitor();


}


