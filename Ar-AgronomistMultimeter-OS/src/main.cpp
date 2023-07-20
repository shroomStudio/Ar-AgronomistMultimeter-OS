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


/*
PIN out map register
digital pins 
from 
2-13
2  - Red LED
3  - Yellow LED
4  - Blue LED
5  - IR LED
6  - Enter Button 
7  - UP Button
8  - Down Button
9  - Display
10 - Display 
11 - Display
12 - Display 
Analog Pins
A1-A5
A1 - Photodiode
Serial Ports
0 - Tx 
1 - RX
*/

