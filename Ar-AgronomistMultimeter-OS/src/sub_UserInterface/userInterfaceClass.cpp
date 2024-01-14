#include <EEPROM.h>
#include <sub_SensingManagement/sensingClass.h>
#include <sub_EnergyManagement/energyManagementClass.h>
#include <sub_SignalConditioning/signalConditioningClass.h>
#include "userInterfaceClass.h"
#include "commonDataTypes.h"
#include "lcdDisplayClass.h"
#include "buttonsClass.h"
#include "usbConecttionClass.h"
#include "ExternalLibraries/LcdMenu.h"
#include "ExternalLibraries/ItemSubMenu.h"
#include "ExternalLibraries/utils/commandProccesors.h"

// Class instances.
energyManagementClass energy;
LcdMenu menu(LCD_ROWS, LCD_COLS);
lcdDisplayClass lcd;
buttonsClass buttons;
usbConecttionClass usb;
sensingClass sensing;
signalConditioningClass conditioningSignals;

// Constructionc of main menu and submenus when is the first time the 
extern MenuItem* macronutrientsSensing[];
extern MenuItem* temperatureSensing[];
extern MenuItem* atmosphericSensing[];
extern MenuItem* files[];
extern MenuItem* configurations[];


MAIN_MENU
(
    ITEM_SUBMENU("Macronutrients Sensing",macronutrientsSensing),
    ITEM_SUBMENU("Temperature Sensing"   ,temperatureSensing),
    ITEM_SUBMENU("Atmospheric Sensing"   ,atmosphericSensing),
    ITEM_SUBMENU("sensing records"       ,files),
    ITEM_SUBMENU("Configurations"        ,configurations)
);

SUB_MENU(macronutrientsSensing, mainMenu,
    ITEM_BASIC("Date"),
    ITEM_BASIC("Save and continue")
);   
SUB_MENU(temperatureSensing, mainMenu,
    ITEM_BASIC("Date"),
    ITEM_BASIC("Save and continue")
);   
SUB_MENU(atmosphericSensing, mainMenu,
    ITEM_BASIC("Date"),
    ITEM_BASIC("Save and continue")
);   
SUB_MENU(files, mainMenu,
    ITEM_BASIC("Date"),
    ITEM_BASIC("Save and continue")
);   
SUB_MENU(configurations, mainMenu,
    ITEM_BASIC("Date"),
    ITEM_BASIC("Save and continue")
);   

userInterfaceClass::userInterfaceClass(){
    initialConfigurationDone = VariableFromEEPROM_Get(0);
    energy.voltageBatteryMonitor();
}

userInterfaceClass::~userInterfaceClass(){
    //Destructor class usbConecttionClass
}

uint8_t userInterfaceClass::VariableFromEEPROM_Get(uint8_t address) 
{
  uint8_t readValue = 0;

  EEPROM.get(address, readValue);

  return readValue;
}

void userInterfaceClass::VariableToEEPROM_Set(uint8_t address, uint8_t variable) 
{
  // Save the variable to EEPROM
  EEPROM.put(address, variable);
}

void userInterfaceClass::resetEEPROM(void)
{
    uint8_t memoryLength = static_cast<uint8_t>(EEPROM.length());

    // Reset the EEPROM (for testing purposes)
    for (int i = 0; i < memoryLength; i++) 
    {
        EEPROM.write(i, 255);  // Write 255 to each EEPROM address
    }
    
    Serial.println("EEPROM Reset");
}

//Local Functions 
void userInterfaceClass::userInitialConfiguration(void)
{
    if(false == initialConfigurationDone)
    {
        if (energy.batteryChargePercentage < MINIMUM_BATTERY_PERCENTAGE)
        {
            lcd.metadataTodisplayInLCD("System discharged please plug in to source power", LEFT_ALIGNED_X, MIDDLE_Y);
            delay(2000);
            energy.turnOffDevice();
        }
        else
        {
            if (buttons.buttonPressed() == OK_BUTTON )
            {
            //TODO: time and Zone Main Menu to be done
            }
            initialConfigurationDone = true;
            VariableToEEPROM_Set(0,initialConfigurationDone);
        }
    }  
}