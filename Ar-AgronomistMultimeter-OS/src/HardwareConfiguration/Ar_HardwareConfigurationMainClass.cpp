// ShroomCorp
// Ar_HardwareConfigurationMainClass.cpp
// Description
// Copyright*

#include "Ar_HardwareConfigurationMainClass.h"

using namespace hardwareConfiguration;
using namespace displayConfiguration;


Ar_DisplayConfigurationClass screenClass;
// Constructor for hardwareConfiguration class
hardwareConfigurationMainClass::hardwareConfigurationMainClass()
{
  processState = PROCESS_NOT_STARTED;

 
}

// Destructor for hardwareconfiguration class
hardwareConfigurationMainClass::~hardwareConfigurationMainClass()
{
}

void initialConfigurationOfDevice(void)
{
  // screenClass.initialConfigurationOfDisplay();

}

// Method implementation for digital port configuration
void hardwareConfigurationMainClass::digitalPortConfiguration(int pinNumber, KING_OF_PORT_T readWrite) 
{
  evaluationOfnewValue(pinNumber,DIGITAL_PORT);

  if (PROCESS_STARTED_VALID_INPUT == processState)
  {
    if (readWrite == INPUT_PORT) 
    {
        pinMode(pinNumber, INPUT);
    } 
    else if (readWrite == OUTPUT_PORT) 
    {
        pinMode(pinNumber, OUTPUT);
    }
    processState = PROCESS_COMPLETED;
  }
  else
  {
      processState = PROCESS_NOT_STARTED;

  }

}


// Method implementation for analog port configuration
void hardwareConfigurationMainClass::analogPortConfiguration(int pinNumber, KING_OF_PORT_T readWrite) 
{
  evaluationOfnewValue(pinNumber,ANALOG_PORT);

  if (PROCESS_STARTED_VALID_INPUT == processState)
  {
    switch (pinNumber)
    {
      case 0:
        if (readWrite == INPUT_PORT)
        {
          pinMode(A0,INPUT);
        }
        else
        {
          pinMode(A0,OUTPUT);
        }
      break;
      case 1:
        if (readWrite == INPUT_PORT)
        {
          pinMode(A1,INPUT);
        }
        else
        {
          pinMode(A1,OUTPUT);
        }
      break;
      case 2:
        if (readWrite == INPUT_PORT)
        {
          pinMode(A2,INPUT);
        }
        else
        {
          pinMode(A2,OUTPUT);
        }
      break;
      case 3:
        if (readWrite == INPUT_PORT)
        {
          pinMode(A3,INPUT);
        }
        else
        {
          pinMode(A3,OUTPUT);
        }
      break;
      case 4:
        if (readWrite == INPUT_PORT)
        {
          pinMode(A4,INPUT);
        }
        else
        {
          pinMode(A4,OUTPUT);
        }
      break;
      case 5:
        if (readWrite == INPUT_PORT)
        {
          pinMode(A5,INPUT);
        }
        else
        {
          pinMode(A5,OUTPUT);
        }
      break;
    }

    processState = PROCESS_COMPLETED;
  }
  else
  {
    processState = PROCESS_NOT_STARTED;
  } 
}


// Method implementation for serial port configuration
void hardwareConfigurationMainClass::serialPortConfiguration(BAUD_RATE_SPEED_T baudRate)
 {
    if (baudRate == HIGH_SPEED)
    {
      Serial.begin(115200);
    }
    else 
    {
      Serial.begin(9600);
    }
    processState = PROCESS_COMPLETED;
 }

// Private method Validation  of a new value
STATE_OF_PROCESS_T hardwareConfigurationMainClass::evaluationOfnewValue(int newValue, TYPE_OF_PORT_T portType)
{
  processState = PROCESS_NOT_STARTED_INVALID_INPUT;

  if (newValue >= ZERO)
  {
    switch (portType) 
    {
     case DIGITAL_PORT:
     if (newValue >= MIN_VALUE_DIGITAL_PIN   && newValue <= MAX_VALUE_DIGITAL_PIN)
     {
       processState = PROCESS_STARTED_VALID_INPUT;
     }
     break;
     case ANALOG_PORT:
     if (newValue >= MIN_VALUE_ANALOG_PIN   && newValue <= MAX_VALUE_ANALOG_PIN)
     {
       processState = PROCESS_STARTED_VALID_INPUT;
     }
     break;
     default:
     break;
    }
  }
  
    return processState;
}
