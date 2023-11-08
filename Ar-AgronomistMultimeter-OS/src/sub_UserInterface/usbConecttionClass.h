#ifndef USBCONECTTIONCLASS_H
#define USBCONECTTIONCLASS_H

#include <Arduino.h>

enum TYPE_USB
{
    ONLY_CHARGER,
    SHARE_FILES,
    SHARE_FILES_CHARGER
};

class usbConecttionClass{
    public:
    usbConecttionClass();
    ~usbConecttionClass();

    //Public Attributtes 
    TYPE_USB typeOfUsbConecttion;
    //Public Attributes
    
    private:
    //Private Attributes
    int usdVoltagePin;
    int usbGroundPin;

    //Private Methods
    void initialUsbConfiguration(void);
    void usbConnectionType(TYPE_USB connectionType);


};

#endif
//end class USBCONECTTIONCLASS_H