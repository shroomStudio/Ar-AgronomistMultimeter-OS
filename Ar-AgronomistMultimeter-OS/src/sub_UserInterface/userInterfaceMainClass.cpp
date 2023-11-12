// ShroomCorp
// main.cpp
// Description
// Copyright*
// System PinOut
/*
pin to turn off device    13
pin to start chargin the battery 12
Pin to reset the device 11
pin to read voltage in battery 12

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


