#ifndef LCDDISPLAYCLASS_H
#define LCDDISPLAYCLASS_H
#include <Arduino.h>
#include <Nokia_5110_LCD_library-2.6.2/src/Nokia_LCD.h>
#include <Nokia_5110_LCD_library-2.6.2/src/Nokia_LCD_Fonts.h>

using namespace std;
using namespace nokia_lcd;

class lcdDisplayClass{
    public:
    lcdDisplayClass();
    ~lcdDisplayClass();
    //Public attributes 
    //Public methods
    void metadataTodisplayInLCD(String);
    void navigationLCD(int changeForEnumInButtons);

    private:
    //Private attributes
    int resetLcdPin;
    int ceLcdPin;
    int dcLcdPin;
    int clockLcdPin;
    int lightLcdPin;

    //Private methods
    void initialDisplaySetup(void);
};

#endif
//end class LCDDISPLAYCLASS_H