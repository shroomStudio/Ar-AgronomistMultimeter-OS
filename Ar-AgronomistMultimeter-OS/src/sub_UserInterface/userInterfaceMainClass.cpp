// ShroomCorp
// main.cpp
// Description
// Copyright*
// System PinOut

/*
Pin13  clockLcdPin = 13;
Pin12  dinLcdPin   = 12;
Pin11  dcLcdPin    = 11;
Pin10  LcdPin      = 10;
Pin9   resetLcdPin = 9;
Pin9   lightLcdPin = 9;
*/

//Headers 
#include <Arduino.h>
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


