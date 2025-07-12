#include <string.h>
#include "lcdDisplayClass.h"
#include "sub_UserInterface/ExternalLibraries/Nokia_LCD.h"
#include "sub_UserInterface/ExternalLibraries/LCD_Fonts.h"

//LCD Nokia instance declare inside this file to avoid any unauthorizated access
Nokia_LCD lcdNokia(clockLcdPin/* CLK */, dinLcdPin /* DIN */, dcLcdPin /* DC */, ceLcdPin /* CE */, resetLcdPin /* RST */,lightLcdPin/* BL */);
//Nokia_LCD lcdNokia(13 /* CLK */, 12 /* DIN */, 11 /* DC */, 10 /* CE */, 9 /* RST */,9 /* BL */);

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
     // Initialize the screen
    lcdNokia.begin();
    // Turn on the backlight
    lcdNokia.setBacklight(true);
    // Set the contrast
    lcdNokia.setContrast(60);  // Good values are usualy between 40 and 60
    // Clear the screen by filling it with black pixels
    lcdNokia.clear();

   // Bold font example
    lcdNokia.setCursor(0, 2);
    lcdNokia.setDefaultFont();
    
    metadataTodisplayInLCD("ShroomCorp\n",LEFT_ALIGNED_X,MIDDLE_Y,true);
    delay(2000);
    metadataTodisplayInLCD("loading",LEFT_ALIGNED_X,BOTTOM_Y,false);
    delay(2000);
}

//Public methods
void lcdDisplayClass::metadataTodisplayInLCD(const char* text,COORDINATES_X_DISCPLAY_ENUM xCoordinates, COORDINATES_Y_DISCPLAY_ENUM yCoordinates,bool clearScreen)
{
    if (true == clearScreen)
    {
        lcdNokia.clear();
    }
    
    lcdNokia.setCursor(xCoordinates,yCoordinates);
    lcdNokia.println(text);
}

void lcdDisplayClass::intNumberTodisplayInLCD(int number,COORDINATES_X_DISCPLAY_ENUM xCoordinates, COORDINATES_Y_DISCPLAY_ENUM yCoordinates,bool clearScreen)
{
    if (true == clearScreen)
    {
        lcdNokia.clear();
    }

    lcdNokia.setCursor(xCoordinates,yCoordinates);
    lcdNokia.print(number); 
}

void lcdDisplayClass::metadataTodisplayFreeCursor(const char* text, int x, int y, bool clearScreen)
 {
    if (clearScreen) {
        lcdNokia.clear();
    }
    
    lcdNokia.setCursor(x, y);
    lcdNokia.println(text);
}

void lcdDisplayClass::moveCursor(COORDINATES_X_DISCPLAY_ENUM xCoordinates, COORDINATES_Y_DISCPLAY_ENUM yCoordinates)
{
    lcdNokia.setCursor(xCoordinates,yCoordinates);
}

void lcdDisplayClass::invertTextColor() 
{
    // Invert text color
    lcdNokia.setInverted(true);
}

void lcdDisplayClass::restoreTextColor() 
{
    // Restore text color to default
    lcdNokia.setInverted(false);
}
