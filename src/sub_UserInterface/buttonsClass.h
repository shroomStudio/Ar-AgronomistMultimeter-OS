// ShroomCorp
// buttonsClass.h
// Description
// Copyright

#ifndef BUTTONSCLASS_H
#define BUTTONSCLASS_H

#include <Arduino.h>
#include "commonDataTypes.h"
#include "lcdDisplayClass.h"

using namespace std;

class buttonsClass{
    public:
    buttonsClass(lcdDisplayClass &lcd);  //Constructor with parameter
    ~buttonsClass();
        
    //Public Attributtes
    //Public Methods
    void initialButtonsSetup(void);
    BUTTON_PRESSED buttonPressed(void);
    static void navigationButtons(void);

    
    private:
    //Private Attributes
    lcdDisplayClass &lcdButtons; 
    //Private Methods
};


#endif 
//end class BUTTONSCLASS_H

// ShroomCorp
// buttonsClass.h
// Description
// Copyright