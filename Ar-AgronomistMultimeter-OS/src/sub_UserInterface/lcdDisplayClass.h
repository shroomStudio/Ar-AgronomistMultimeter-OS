#ifndef LCDDISPLAYCLASS_H
#define LCDDISPLAYCLASS_H
#include <Arduino.h>

using namespace std;

typedef enum 
{
    //Coordinates on the x-axis, can be between 0 and 83
    LEFT_ALIGNED_X = 0, 
    CENTERED_X = 20, 
    RIGHT_ALIGNED_X = 60
}COORDINATES_X_DISCPLAY_ENUM;

typedef enum 
{
    // Coordinates on the y-axis, can be between 0 and 5
    TOP_Y = 0, 
    MIDDLE_Y = 3, 
    BOTTOM_Y = 5
}COORDINATES_Y_DISCPLAY_ENUM;
class lcdDisplayClass{
    public:
    lcdDisplayClass();
    ~lcdDisplayClass();
    //Public attributes 
    //Public methods
    void metadataTodisplayInLCD(String text, COORDINATES_X_DISCPLAY_ENUM xCoordinate, COORDINATES_Y_DISCPLAY_ENUM yCoordinate);
    void navigationLCD(int changeForEnumInButtons);

    private:
    //Private attributes
    //Private methods
    void initialDisplaySetup(void);
};

#endif
//end class LCDDISPLAYCLASS_H