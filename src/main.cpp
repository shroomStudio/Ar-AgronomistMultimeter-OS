// ShroomCorp AgM OS-2.0
// main.cpp — entry point. All logic lives in SystemController.

#include <Arduino.h>
#include <sub_SystemController/systemController.h>

SystemController controller;

void setup()
{
    controller.begin();
}

void loop()
{
    controller.run();
}
