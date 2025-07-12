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
    BUTTON_PRESSED buttonPressed(void);
    

    private:
    //Private Attributes
    //Private Methods
    void initialButtonsSetup(void);
    static void navigationButtons(void);
};


#endif 
//end class BUTTONSCLASS_H