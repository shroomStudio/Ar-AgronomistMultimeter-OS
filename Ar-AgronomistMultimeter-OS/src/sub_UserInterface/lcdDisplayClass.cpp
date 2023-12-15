#include "lcdDisplayClass.h"
#include "ExternalLibraries/Nokia_LCD.h"

//LCD pinOut
int clockLcdPin = 13;
int dinLcdPin   = 12;
int dcLcdPin    = 11;
int ceLcdPin    = 10;
int resetLcdPin = 9;
int lightLcdPin = 9;

//LCD Nokia instance declare inside this file to avoid any unauthorizated access
 Nokia_LCD display(clockLcdPin/* CLK */, dinLcdPin /* DIN */, dcLcdPin /* DC */, ceLcdPin /* CE */, resetLcdPin /* RST */,lightLcdPin/* BL */);

//LCD class
lcdDisplayClass::lcdDisplayClass(){
    //Constructor lcdDisplayClass
}
lcdDisplayClass::~lcdDisplayClass(){
    //destructor lcdDisplayClass
}

//Private methods
void lcdDisplayClass::initialDisplaySetup(void)
{
    display.begin();
    // Turn on the backlight
    display.setBacklight(true);
    // Set the contrast
    display.setContrast(60);  // Good values are usualy between 40 and 60
    // Clear the screen by filling it with black pixels
    display.clear();
}

//Public methods
void lcdDisplayClass::metadataTodisplayInLCD(String)
{

}
void lcdDisplayClass::navigationLCD(int changeForEnumInButtons)
{

}
