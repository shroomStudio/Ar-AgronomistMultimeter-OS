// ShroomCorp
// main.cpp
// Description
// Copyright*
// System PinOut

//Headers 
#include <Arduino.h>
//#include <string.h>
//#include "sub_UserInterface/commonDataTypes.h"
//#include "sub_UserInterface/userInterfaceClass.h"

//Namespaces 
using namespace std;

// Class instances
//userInterfaceClass userInterface;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write("Hello Puto");
  delay(200);
}



