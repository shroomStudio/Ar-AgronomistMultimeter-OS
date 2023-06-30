// ShroomCorp
// Ar_HardwareConfigurationMainClass.h
// Description
// Copyright*

#ifndef AR_HARDWARECONFIGURATIONMAINCLASS_H
#define AR_HARDWARECONFIGURATIONMAINCLASS_H

// Include files
#include <Arduino.h>
#include "Ar_DisplayConfigurationClass.h"

// Definitions 
#define ZERO                  0
#define MIN_VALUE_ANALOG_PIN  0
#define MAX_VALUE_ANALOG_PIN  5
#define MIN_VALUE_DIGITAL_PIN 2
#define MAX_VALUE_DIGITAL_PIN 8

//Namespace Hardware configuration 
namespace hardwareConfiguration
{
typedef enum 
{
  PROCESS_NOT_STARTED = 0,
  PROCESS_NOT_STARTED_INVALID_INPUT,
  PROCESS_STARTED_VALID_INPUT,
  PROCESS_COMPLETED
}STATE_OF_PROCESS_T;

typedef enum 
{
  NO_TYPE = 0,
  DIGITAL_PORT =0,
  ANALOG_PORT,
}TYPE_OF_PORT_T;

typedef enum  
{
    INPUT_PORT = 0,
    OUTPUT_PORT
}KING_OF_PORT_T;

typedef enum
{
  LOW_SPEED = 0,
  HIGH_SPEED
}BAUD_RATE_SPEED_T;

// Class definition
  class hardwareConfigurationMainClass
  {
    public:
      hardwareConfigurationMainClass();
      ~hardwareConfigurationMainClass();
          
      // Public Methods 
      void initialConfigurationOfDevice();
      void digitalPortConfiguration(int pinNumber, KING_OF_PORT_T readWrite);
      void analogPortConfiguration(int pinNumber, KING_OF_PORT_T readWrite);
      void serialPortConfiguration(BAUD_RATE_SPEED_T baudRate);


    private:
      // Private Variables
      STATE_OF_PROCESS_T processState;

      // Private Methods
      STATE_OF_PROCESS_T evaluationOfnewValue(int newValue, TYPE_OF_PORT_T portType);
  };
}// Namespace Hardware Configuration

#endif //AR_HARDWARE_CONFIGURATION_MAIN_CLASS_H