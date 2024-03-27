// ShroomCorp
// main.cpp
// Description
// Copyright*
// System PinOut

//Headers 
#include <Arduino.h>
//#include "sub_UserInterface/ExternalLibraries/Nokia_LCD.h"
//#include "sub_UserInterface/ExternalLibraries/LCD_Fonts.h"
#include "sub_UserInterface/lcdDisplayClass.h"
//#include "ExternalLibraries/Nokia_LCD.h"
//#include "ExternalLibraries/LCD_Fonts.h"
/*#include <string.h>
#include "commonDataTypes.h"
#include "userInterfaceClass.h"*/

//Namespaces 
using namespace std;

lcdDisplayClass lcdMultimeter;



void setup() 
{
    lcdMultimeter.initialDisplaySetup();

}

void loop()
{
    //Serial.write("Hello putos ");
    //delay(200);
    //userInterface.userMainMenu();

}

