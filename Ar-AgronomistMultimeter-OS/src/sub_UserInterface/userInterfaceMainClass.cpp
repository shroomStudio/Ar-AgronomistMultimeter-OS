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


// Class instances
userInterfaceClass userInterface;


//Namespaces 
using namespace std;

//Global Variables 
bool initialConfigurationDone = false;


void setup() 
{
    if (false == initialConfigurationDone)
    {
        /* User Initial Confiuration */
         userInterface.userInitialConfiguration();
    }
    
}

void loop()
{

}

