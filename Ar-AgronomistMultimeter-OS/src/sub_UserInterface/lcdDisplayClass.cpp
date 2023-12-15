#include "lcdDisplayClass.h"
#include "ExternalLibraries/Nokia_LCD.h"


 Nokia_LCD display(13 /* CLK */, 12 /* DIN */, 11 /* DC */, 10 /* CE */, 9 /* RST */,
              9 /* BL */);

lcdDisplayClass::lcdDisplayClass(){
    //Constructor lcdDisplayClass
}
lcdDisplayClass::~lcdDisplayClass(){
    //destructor lcdDisplayClass
}

//Private methods
void lcdDisplayClass::initialDisplaySetup(void)
{
    
}

//Public methods
void lcdDisplayClass::metadataTodisplayInLCD(String)
{

}
void lcdDisplayClass::navigationLCD(int changeForEnumInButtons)
{

}
