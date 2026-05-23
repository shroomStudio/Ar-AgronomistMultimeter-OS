// ShroomCorp
// main.cpp
// Description
// Copyright

//Headers 
#include <Arduino.h>
#include <sub_UserInterface/commonDataTypes.h>
#include <sub_EnergyManagement/energyManagementClass.h>
#include <sub_UserInterface/lcdDisplayClass.h>
#include <sub_EnergyManagement/energyManagementClass.h>
#include <sub_UserInterface/buttonsClass.h>
#include <sub_UserInterface/userInterfaceClass.h>
#include <sub_SensingManagement/sensingClass.h>
#include <sub_SignalConditioning/signalConditioningClass.h>

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
    // Device General setup
    pinMode(PIN_BLUE_LED, OUTPUT);
    pinMode(PIN_RED_LED, OUTPUT);
    pinMode(PIN_YELLOW_LED, OUTPUT);
    pinMode(PIN_WHITE_LED, OUTPUT);

    digitalWrite(PIN_BLUE_LED, HIGH);
    digitalWrite(PIN_RED_LED, HIGH);
    digitalWrite(PIN_YELLOW_LED, HIGH);
    digitalWrite(PIN_WHITE_LED, HIGH);
     
    Serial.begin(115200);
    lcdMain.initialDisplaySetup();
    energyMain.initialSetUpEnergyManagament();
    buttonsMain.initialButtonsSetup();
    userInterfaceMain.userInitialConfiguration();
}

void loop()
{
    // ARD-02: Serial command dispatcher — poll for inference trigger
    if (Serial.available() > 0)
    {
        char cmd = Serial.read();
        if (cmd == 'M')
        {
            sensingMain.inferenceProcess();
        }
    }
}

// ShroomCorp
// main.cpp
// Description
// Copyright