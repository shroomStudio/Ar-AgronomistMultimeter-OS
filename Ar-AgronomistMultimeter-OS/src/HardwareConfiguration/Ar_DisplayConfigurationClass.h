// ShroomCorp
// Ar_DisplayConfigurationClass.h
// Description
// Copyright*

#ifndef AR_DISPLAYCONFIGURATIONCLASS_H
#define AR_DISPLAYCONFIGURATIONCLASS_H

#include <Nokia_5110_LCD_library-2.6.2/src/Nokia_LCD.h>

namespace displayConfiguration
{
  typedef enum
  {
    NO_STARTED =0,
    FINISHED,
    ABORTED
  }DISPLAY_PROCESS_STATE_T;

}//End Namespace
  
  class Ar_DisplayConfigurationClass
  {
  public:
  Ar_DisplayConfigurationClass(/* args */);
  ~Ar_DisplayConfigurationClass();

  //Public methods
  void initialConfigurationOfDisplay(void);
  void screenBackLightStatus(bool stateOfLight);
  void writeInScreen(int cursorCoordinateX, int cursorCoordinateY, const char* textToPrint);
  void clearScreen(bool clearOrNot);

  private:
  // Private static member declaration
  displayConfiguration::DISPLAY_PROCESS_STATE_T stateOfProcees;
  static Nokia_LCD* lcd;  
  };



#endif //End AR_DISPLAYCONFIGURATIONCLASS_H



