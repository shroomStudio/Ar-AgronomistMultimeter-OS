#include <EEPROM.h>
#include <string.h>
#include <sub_SensingManagement/sensingClass.h>
#include <sub_EnergyManagement/energyManagementClass.h>
#include <sub_SignalConditioning/signalConditioningClass.h>
#include "userInterfaceClass.h"
#include "lcdDisplayClass.h"
#include "buttonsClass.h"
#include "usbConecttionClass.h"


#define RS_PIN dinLcdPin
#define EN_PIN dcLcdPin
#define D4_PIN clockLcdPin
#define D5_PIN ceLcdPin
#define D6_PIN resetLcdPin
#define D7_PIN lightLcdPin

// Class instances.
energyManagementClass energyUser;
lcdDisplayClass lcdUser;
buttonsClass buttonsUser;
usbConecttionClass usbUser;
sensingClass sensingUser;
signalConditioningClass conditioningSignalsUser;

/*
SUB_MENU(macronutrientsSensing, mainMenu,
    ITEM_INPUT("Sensing instrucctions", displaySensingInstruction),
    ITEM_INPUT("Sensing Process", startSensingProcess)
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
);  */

userInterfaceClass::userInterfaceClass(){
    VariableToEEPROM_Set(0,false);
    initialConfigurationDone = VariableFromEEPROM_Get(0);
    //energy.voltageBatteryMonitor();
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
        if (energyUser.batteryChargePercentage < MINIMUM_BATTERY_PERCENTAGE)
        {
            lcdUser.metadataTodisplayInLCD("System discharged please plug in to source power", LEFT_ALIGNED_X, MIDDLE_Y,true);
            delay(2000);
            energyUser.turnOffDevice();
        }
        else
        {
            if (buttonsUser.buttonPressed() == OK_BUTTON )
            {
            //TODO: time and Zone Main Menu to be done
            }
            initialConfigurationDone = true;
            VariableToEEPROM_Set(0,initialConfigurationDone);
            displayMainMenu();
            while (buttonsUser.buttonPressed() != OK_BUTTON)
            {
            lcdUser.metadataTodisplayInLCD("Press Ok to start Sensing Process  \n", LEFT_ALIGNED_X, MIDDLE_Y, true);
            buttonsUser.navigationButtons();
            delay(3000);
            }
            //Starting sensing process
            startSensingProcess();
        }
    }
}

//Local Methods
void userInterfaceClass::displayMainMenu() 
{
    displayMenu(mainMenuItems, 0);
}

void userInterfaceClass::navigateUp() 
{
    if (currentItemIndex > 0) 
    {
        // Invert colors of the current line before moving the cursor
        lcdUser.invertTextColor();
        lcdUser.metadataTodisplayFreeCursor(mainMenuItems[currentItemIndex], LEFT_ALIGNED_X, TOP_Y + currentItemIndex + 1, false);

        currentItemIndex--;
        displayMenu(mainMenuItems, currentItemIndex);
    }
}

void userInterfaceClass::navigateDown() 
{
    if (currentItemIndex < NUMBER_OF_ITEMS_MAIN_MENU) 
    {
        // Invert colors of the current line before moving the cursor
        lcdUser.invertTextColor();
        lcdUser.metadataTodisplayFreeCursor(mainMenuItems[currentItemIndex], LEFT_ALIGNED_X, TOP_Y + currentItemIndex + 1, false);

        currentItemIndex++;
        displayMenu(mainMenuItems, currentItemIndex);
    }
}

void userInterfaceClass::menuSelectedOption(int buttonPressed)
{
    switch((BUTTON_PRESSED)buttonPressed)
    {
        case OK_BUTTON:
           
            break;
        case BACK_BUTTON:
            // Handle going back if needed
            break;
        case DOWN_BUTTON:
            navigateDown();
            break;
        case UP_BUTTON:
            navigateUp();
            break;
        default:
            // Handle other button states or no button press
            break;
    }
}

void userInterfaceClass::displayMenu(const char* menu[], int startIndex) 
{
    String menuItem = "\0";

    delay(2000);
    lcdUser.metadataTodisplayInLCD("Main Menu: \n", LEFT_ALIGNED_X, TOP_Y, true);

    for (int i = startIndex; i < startIndex + 3; i++) 
    {
        if (i == currentItemIndex)
        {
            menuItem += menu[i];
            lcdUser.metadataTodisplayFreeCursor(menuItem.c_str(), LEFT_ALIGNED_X, TOP_Y + (i - startIndex + 1), false);
        } 
        else 
        {
            lcdUser.metadataTodisplayFreeCursor(menu[i], LEFT_ALIGNED_X, TOP_Y + (i - startIndex + 1), false);
        }
    }
   delay(5000);
}

void userInterfaceClass::displaySensingInstruction(void)
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

void userInterfaceClass::startSensingProcess(void)
{
    if ( energyUser.batteryChargePercentage >= MINIMUM_TO_SENSING)
    {
        sensingUser.macronutrientSensingProcess();
    }
    else 
    {
        lcdUser.metadataTodisplayInLCD("the battery must has at least 10 percent of charge \n", LEFT_ALIGNED_X, MIDDLE_Y, true);
        delay(2000);
    }
}