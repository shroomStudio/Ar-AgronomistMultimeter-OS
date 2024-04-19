#ifndef LCDDISPLAYCLASS_H
#define LCDDISPLAYCLASS_H

#include <Arduino.h>
#include "commonDataTypes.h"

using namespace std;

class lcdDisplayClass{
    public:
    lcdDisplayClass();
    ~lcdDisplayClass();
    //Public attributes 
    //Public methods
    void initialDisplaySetup(void);
    void intNumberTodisplayInLCD(int number,COORDINATES_X_DISCPLAY_ENUM xCoordinates, COORDINATES_Y_DISCPLAY_ENUM yCoordinates,bool clearScreen);
    void metadataTodisplayInLCD(const char* text,COORDINATES_X_DISCPLAY_ENUM xCoordinates, COORDINATES_Y_DISCPLAY_ENUM yCoordinates,bool clearScreen);
    void moveCursor(COORDINATES_X_DISCPLAY_ENUM xCoordinates, COORDINATES_Y_DISCPLAY_ENUM yCoordinates);
    
    private:
    //Private attributes
    //Private methods 
};

#endif
//end class LCDDISPLAYCLASS_H