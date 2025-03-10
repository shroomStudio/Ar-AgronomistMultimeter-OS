// ShroomCorp
// main.cpp
// Description
// Copyright*
// System PinOut

//Headers 
#include <Arduino.h>
#include "sub_UserInterface/commonDataTypes.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_EnergyManagement/energyManagementClass.h"
#include "sub_UserInterface/buttonsClass.h"
#include "sub_UserInterface/userInterfaceClass.h"
/*#include <string.h>
#include "userInterfaceClass.h"*/

//Namespaces 
using namespace std;

lcdDisplayClass lcdMain;
energyManagementClass energyMain;
buttonsClass buttonsMain;
userInterfaceClass userInterfaceMain;


void setup() 
{
    Serial.begin(9600);
    lcdMain.initialDisplaySetup();
    energyMain.initialSetUpEnergyManagament();
    buttonsMain.initialButtonsSetup();
    userInterfaceMain.userInitialConfiguration();
}

void loop()
{
    //int buttonPressed = ((int)buttonsMain.buttonPressed());
    //Method that as to be running all the time to detect the user interation
    //buttonsMain.navigationButtons();
    //userInterfaceMain.menuSelectedOption(buttonPressed);
}

