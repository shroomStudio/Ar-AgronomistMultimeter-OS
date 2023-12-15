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
 Nokia_LCD nokia5110(clockLcdPin/* CLK */, dinLcdPin /* DIN */, dcLcdPin /* DC */, ceLcdPin /* CE */, resetLcdPin /* RST */,lightLcdPin/* BL */);

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
    nokia5110.begin();
    // Turn on the backlight
    nokia5110.setBacklight(true);
    // Set the contrast
    nokia5110.setContrast(60);  // Good values are usualy between 40 and 60
    // Clear the screen by filling it with black pixels
    nokia5110.clear();
}

//Public methods
void lcdDisplayClass::metadataTodisplayInLCD(String)
{

}
void lcdDisplayClass::navigationLCD(int changeForEnumInButtons)
{

}
