#include <string.h>
#include "lcdDisplayClass.h"
#include "sub_UserInterface/ExternalLibraries/Nokia_LCD.h"
#include "sub_UserInterface/ExternalLibraries/LCD_Fonts.h"
//LCD pinOut
int clockLcdPin = 13;
int dinLcdPin   = 12;
int dcLcdPin    = 11;
int ceLcdPin    = 10;
int resetLcdPin = 9;
int lightLcdPin = 9;


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
    lcdNokia.setCursor(0, 5);
    lcdNokia.setDefaultFont();
    lcdNokia.print("hello putos!");
    //nokia5110.print("THello Putos\n");
   //metadataTodisplayInLCD("ShroomCorp",CENTERED_X,MIDDLE_Y);*/
}

//Public methods
void lcdDisplayClass::metadataTodisplayInLCD(String text, COORDINATES_X_DISCPLAY_ENUM xCoordinate, COORDINATES_Y_DISCPLAY_ENUM yCoordinate)
{
    /*
    const char* textInChar = text.c_str();
    nokia5110.setCursor(xCoordinate,yCoordinate);
    nokia5110.println(*textInChar);*/
}
/*

void lcdDisplayClass::metadataTodisplayInLCDAdvanceCursor(String text, COORDINATES_X_DISCPLAY_ENUM xCoordinate, 
                                            COORDINATES_Y_DISCPLAY_ENUM yCoordinate, uint8_t advanceXcoordinate, uint8_t advanceYCoordinate)
{
    uint8_t customXCoordinates = (uint8_t)xCoordinate + advanceXcoordinate;
    uint8_t customYCoordinates = (uint8_t)yCoordinate + advanceYCoordinate;

    const char* textInChar = text.c_str();
    nokia5110.setCursor(customXCoordinates,customYCoordinates);
    nokia5110.println(*textInChar);
}

*/
/*
void lcdDisplayClass::moveCursor(uint8_t xCoordinates, uint8_t yCoordinates)
{
    nokia5110.setCursor(xCoordinates,yCoordinates);
}
*/