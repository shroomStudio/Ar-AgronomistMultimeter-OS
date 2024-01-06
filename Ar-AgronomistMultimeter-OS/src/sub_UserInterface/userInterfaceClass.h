#ifndef USERINTERFACECLASS_H
#define USERINTERFACECLASS_H

#include <Arduino.h>
#include <EEPROM.h>

class userInterfaceClass{

    public:
    userInterfaceClass();
    ~userInterfaceClass();

    //Public Attributtes 
    //Public Attributes
    void userInitialConfiguration(void);
    
    private:
    //Private Attributes
    bool initialConfigurationDone = false;
    //Private Methods
    uint8_t VariableFromEEPROM_Get(uint8_t address);
    void VariableToEEPROM_Set(uint8_t address, uint8_t variable);
    void resetEEPROM(void); 
};

#endif
//end class USERINTERFACECLASS_H