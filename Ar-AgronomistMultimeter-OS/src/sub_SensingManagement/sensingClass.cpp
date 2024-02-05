#include "sensingClass.h"
#include "sub_UserInterface/lcdDisplayClass.h"
#include "sub_UserInterface/buttonsClass.h"

//Clases instances 
lcdDisplayClass lcdSensing;
buttonsClass buttonsSensing;

//Local Methods declare here to avoid ungranted access
void nitrogenSensingProcess(void);
void phosphorusSensingProcess(void);
void potassiumSensingProcess(void);

sensingClass::sensingClass(){
    // Constructor sensing class
    pinMode(photodiodeInput,INPUT);
    pinMode(photodiodeOutput,OUTPUT);
    pinMode(redLedPin,OUTPUT);
    pinMode(yellowLedPin,OUTPUT);
    pinMode(blueLedPin,OUTPUT);
    pinMode(infraredLedPin,OUTPUT);
}
sensingClass::~sensingClass(){
    //destructor sensing class
}

void sensingClass::macronutrientSensingProcess()
{
    bool sensingProcessFinished = false;

    while (buttonsSensing.buttonPressed() != OK_BUTTON)
    {
        lcdSensing.metadataTodisplayInLCD
        ("please get ready the sample, press OK to continue", LEFT_ALIGNED_X, MIDDLE_Y);
        delay(2000);
    }
    //TODO: get and Save date and time
    while (buttonsSensing.buttonPressed() != BACK_BUTTON || sensingProcessFinished)
    {
        lcdSensing.metadataTodisplayInLCD
        ("sensing in process, press back to abort", LEFT_ALIGNED_X, MIDDLE_Y);
        lcdSensing.metadataTodisplayInLCDAdvanceCursor
        ("...",LEFT_ALIGNED_X,TOP_Y,0,1);
        delay(2000);
        nitrogenSensingProcess();
        phosphorusSensingProcess();
        potassiumSensingProcess();

        sensingProcessFinished = true;
    }
}
void sensingClass::temperatureSensingProcess()
{

}
void sensingClass::humiditySensingProces()
{

}
void sensingClass::atmosphericPressureSensingProcess()
{

}
void sensingClass::serialMiltiplexor(SENSOR_SERIAL sensor)
{

}
void nitrogenSensingProcess(void)
{

}
void phosphorusSensingProcess(void)
{

}
void potassiumSensingProcess(void)
{

}