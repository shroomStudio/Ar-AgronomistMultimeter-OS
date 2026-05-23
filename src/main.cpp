// ShroomCorp
// main.cpp
// AgM Inference Branch — serial console driven, LCD status only
// Copyright

#include <Arduino.h>
#include <sub_UserInterface/commonDataTypes.h>
#include <sub_EnergyManagement/energyManagementClass.h>
#include <sub_UserInterface/lcdDisplayClass.h>
#include <sub_UserInterface/buttonsClass.h>
#include <sub_UserInterface/userInterfaceClass.h>
#include <sub_SensingManagement/sensingClass.h>
#include <sub_SignalConditioning/signalConditioningClass.h>

using namespace std;

lcdDisplayClass        lcdMain;
buttonsClass           buttonsMain(lcdMain);
energyManagementClass  energyMain(lcdMain);
signalConditioningClass conditioningMain(lcdMain, buttonsMain);
sensingClass           sensingMain(lcdMain, buttonsMain, conditioningMain);
userInterfaceClass     userInterfaceMain(lcdMain, buttonsMain, energyMain, sensingMain, conditioningMain);

void setup()
{
    pinMode(PIN_BLUE_LED,   OUTPUT);
    pinMode(PIN_RED_LED,    OUTPUT);
    pinMode(PIN_YELLOW_LED, OUTPUT);
    pinMode(PIN_WHITE_LED,  OUTPUT);

    // All LEDs off at start (active LOW relay shield: HIGH = off)
    digitalWrite(PIN_BLUE_LED,   HIGH);
    digitalWrite(PIN_RED_LED,    HIGH);
    digitalWrite(PIN_YELLOW_LED, HIGH);
    digitalWrite(PIN_WHITE_LED,  HIGH);

    Serial.begin(115200);
    lcdMain.initialDisplaySetup();
    energyMain.initialSetUpEnergyManagament();

    // LCD: ready status — no button interaction required
    lcdMain.metadataTodisplayInLCD("AgM Ready", LEFT_ALIGNED_X, MIDDLE_Y, true);
}

void loop()
{
    // Serial command dispatcher — all interaction via serial console
    if (Serial.available() > 0)
    {
        char cmd = (char)Serial.read();

        if (cmd == 'L')
        {
            // Lamp ON command (ARD-L): turn on tungsten lamp via relay pins
            digitalWrite(PIN_YELLOW_LED, LOW);
            digitalWrite(PIN_WHITE_LED,  LOW);
            lampStartTime = millis();   // start warm-up countdown (ARD-08)
            lcdMain.metadataTodisplayInLCD("Lamp ON", LEFT_ALIGNED_X, MIDDLE_Y, true);
            Serial.println(F("L"));   // ACK: lamp on confirmed
        }
        else if (cmd == 'M')
        {
            // Measurement command: run full inference pipeline
            lcdMain.metadataTodisplayInLCD("Measuring...", LEFT_ALIGNED_X, MIDDLE_Y, true);
            sensingMain.inferenceProcess();
            lcdMain.metadataTodisplayInLCD("AgM Ready", LEFT_ALIGNED_X, MIDDLE_Y, true);
        }
    }
}

// ShroomCorp
// main.cpp
// Copyright
