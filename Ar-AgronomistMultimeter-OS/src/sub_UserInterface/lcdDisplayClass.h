#ifndef LCDDISPLAYCLASS_H
#define LCDDISPLAYCLASS_H
#include <Arduino.h>

using namespace std;
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
    //Private methods
    void initialDisplaySetup(void);
};

#endif
//end class LCDDISPLAYCLASS_H