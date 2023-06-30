// ShroomCorp
// main.cpp
// Description
// Copyright*

//Headers 
#include <Arduino.h>
#include "HardwareConfiguration/Ar_HardwareConfigurationMainClass.h"

//Namespaces 
using namespace hardwareConfiguration;

void setup() 
{
// Class instances
hardwareConfigurationMainClass HwConfigInstance;

// Digital Ports Configuration
HwConfigInstance.digitalPortConfiguration(2,INPUT_PORT);

// Analog Ports Configuration

// Serial ports Configuration 


}

void loop()
{


}
