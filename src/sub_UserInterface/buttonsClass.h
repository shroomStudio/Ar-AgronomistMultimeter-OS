#ifndef BUTTONSCLASS_H
#define BUTTONSCLASS_H

#include <Arduino.h>
#include "commonDataTypes.h"

using namespace std;

class buttonsClass{
    public:
    buttonsClass();
    ~buttonsClass();    
    //Public Attributtes
    //Public Methods
    void initialButtonsSetup(void);
    BUTTON_PRESSED buttonPressed(void);
    static void navigationButtons(void);

    
    private:
    //Private Attributes
    //Private Methods
};


#endif 
//end class BUTTONSCLASS_H