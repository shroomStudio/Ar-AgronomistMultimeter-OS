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
#include "ExternalLibraries/ItemInput.h"

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
// Local Methods Prototypes
void displaySensingInstruction(char* input);

MAIN_MENU
(
    ITEM_SUBMENU("Macronutrients Sensing",macronutrientsSensing),
    ITEM_SUBMENU("Temperature Sensing"   ,temperatureSensing),
    ITEM_SUBMENU("Atmospheric Sensing"   ,atmosphericSensing),
    ITEM_SUBMENU("sensing records"       ,files),
    ITEM_SUBMENU("Configurations"        ,configurations)
);

SUB_MENU(macronutrientsSensing, mainMenu,
    ITEM_INPUT("Sensing instrucctions", displaySensingInstruction),
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

//Local Methods
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

//Local Methods
void userInterfaceClass::userMainMenu(void)
{
    // Variable to Read the user Input
    char command = static_cast<char>(buttons.buttonPressed());
    
    //Setting up the main menu in the LCD
    menu.setupLcdWithMenu(0x27, mainMenu);

    //Method to interact whit the user 
    processMenuCommand(menu, command, UP_BUTTON, DOWN_BUTTON, OK_BUTTON, BACK_BUTTON);
}


void displaySensingInstruction(char* input)
{
    /* TODO: To be implemented in User interface creation
    int lineNumber = 0;
    int lastLIne = 0;
    char addressOfInstructtions[] = "Hello, Arduino!\nThis is a text file content.";
    char* fileContentCopy = strdup(addressOfInstructtions);
    const char* token = strtok(fileContentCopy, "\n");

    while (token != NULL) 
    {
        lcd.metadataTodisplayInLCDAdvanceCursor(token,LEFT_ALIGNED_X,TOP_Y,0,(lineNumber + 1));
        delay(1000);  // Adjust delay based on your preferences
        lineNumber++;
        token = strtok(NULL, "\n");
    }
   
    if (lastLIne ==lineNumber)
    {

        
    }
    else 
    {

        lastLIne++;
    }

  free(fileContentCopy);
  */
}

