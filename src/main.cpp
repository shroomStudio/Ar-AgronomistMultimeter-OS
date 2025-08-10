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
#include "sub_SensingManagement/sensingClass.h"
#include "sub_SignalConditioning/signalConditioningClass.h"

//Namespaces 
using namespace std;

lcdDisplayClass lcdMain;
buttonsClass buttonsMain (lcdMain);
energyManagementClass energyMain(lcdMain);
signalConditioningClass conditioningMain (lcdMain, buttonsMain);
sensingClass sensingMain (lcdMain, buttonsMain, conditioningMain);
userInterfaceClass userInterfaceMain (lcdMain, buttonsMain, energyMain, sensingMain, conditioningMain);

void setup() 
{
    Serial.begin(115200);
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

