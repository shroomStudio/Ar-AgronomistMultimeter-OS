#ifndef BUTTONSCLASS_H
#define BUTTONSCLASS_H

#include <Arduino.h>

using namespace std;

enum BUTTON_PRESSED
{
    UP_BUTTON = 0,
    DOWN_BUTTON,
    BACK_BUTTON,
    OK_BUTTON,
    NO_BUTTON = 15,
};

class buttonsClass{
    public:
    buttonsClass();
    ~buttonsClass();    
    //Public Attributtes
    //Public Methods
    BUTTON_PRESSED buttonPressed(void);
    

    private:
    //Private Attributes
    //Private Methods
    void initialButtonsSetup(void);
    static void navigationButtons(void);
};


#endif 
//end class BUTTONSCLASS_H