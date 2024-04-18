// ShroomCorp
// main.cpp
// Description
// Copyright*
// System PinOut

//Headers 
#include <Arduino.h>
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/commonDataTypes.h"
#include "sub_EnergyManagement/energyManagementClass.h"
/*#include <string.h>
#include "userInterfaceClass.h"*/

//Namespaces 
using namespace std;

lcdDisplayClass lcdMultimeter;
energyManagementClass energyMultimeter;


void setup() 
{
    lcdMultimeter.initialDisplaySetup();
    energyMultimeter.initialSetUpEnergyManagament();
}

void loop()
{
    //Serial.write("Hello putos ");
    //delay(200);
    //userInterface.userMainMenu();

}

