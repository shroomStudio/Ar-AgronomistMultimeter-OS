#ifndef BUTTONSCLASS_H
#define BUTTONSCLASS_H

#include <Arduino.h>

using namespace std;

enum BUTTON_PRESSED
{
    UP_BUTTON,
    DOWN_BUTTON,
    BACK_BUTTON,
    OK_BUTTON
};

class buttonsClass{
    public:
    buttonsClass();
    ~buttonsClass();    
    //Public Attributtes
    BUTTON_PRESSED buttonPressed;
    //Public Methods
    BUTTON_PRESSED navigationButtons(void);

    private:
    //Private Attributes
    int pinUpButton;
    int pinDownButton;
    int pinBackButton;
    int pinOkButton;
    //Private Methods
    void initialButtonsSetup(void);
};


#endif 
//end class BUTTONSCLASS_H