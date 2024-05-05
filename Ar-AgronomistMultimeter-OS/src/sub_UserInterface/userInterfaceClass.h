#ifndef USERINTERFACECLASS_H
#define USERINTERFACECLASS_H

#include <Arduino.h>
#include "commonDataTypes.h"
class userInterfaceClass{

    public:
    userInterfaceClass();
    ~userInterfaceClass();

    //Public Attributtes 
    //Public Attributes
    void userInitialConfiguration(void);
    void displayMainMenu(void);
    void navigateUp(void);
    void navigateDown(void);
    void selectOption(void);

    private:
    //Private Attributes
    int currentItemIndex = 0;
    bool initialConfigurationDone;
    const char* mainMenuItems[NUMBER_OF_ITEMS_MAIN_MENU] = 
                                    {"Macronutrients Sensing \n",
                                     "Temperature Sensing \n",
                                     "Atmospheric Sensing\n", 
                                     "Files \n", 
                                     "Configurations \n" };
    //Private Methods
    uint8_t VariableFromEEPROM_Get(uint8_t address);
    void VariableToEEPROM_Set(uint8_t address, uint8_t variable);
    void resetEEPROM(void); 
    void displayMenu(const char* menu[], int startIndex);
};

#endif
//end class USERINTERFACECLASS_H