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
    void metadataTodisplayInLCD(String text, COORDINATES_X_DISCPLAY_ENUM xCoordinate, COORDINATES_Y_DISCPLAY_ENUM yCoordinate);
    void metadataTodisplayInLCDAdvanceCursor(String text, COORDINATES_X_DISCPLAY_ENUM xCoordinate, 
                                            COORDINATES_Y_DISCPLAY_ENUM yCoordinate, uint8_t advanceXcoordinate, uint8_t advanceYCoordinate);
    void moveCursor(uint8_t xCoordinates, uint8_t yCoordinates);
    
    private:
    //Private attributes
    //Private methods
    void initialDisplaySetup(void);
};

#endif
//end class LCDDISPLAYCLASS_H