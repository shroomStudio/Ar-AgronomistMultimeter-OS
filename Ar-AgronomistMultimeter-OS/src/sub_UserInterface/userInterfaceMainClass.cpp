// ShroomCorp
// main.cpp
// Description
// Copyright*
// System PinOut



//Headers 
#include <Arduino.h>
#include <string.h>
#include "commonDataTypes.h"
#include "userInterfaceClass.h"

//Namespaces 
using namespace std;

// Class instances
userInterfaceClass userInterface;

void setup() 
{
    /* User Initial Confiuration */
    userInterface.userInitialConfiguration();
}

void loop()
{

}

