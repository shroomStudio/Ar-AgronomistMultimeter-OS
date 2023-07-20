// ShroomCorp
// Ar_DisplayConfigurationClass.cpp
// Description
// Copyright*

// Include files
#include "Ar_DisplayConfigurationClass.h"

// Name spaces
using namespace displayConfiguration;

// Static Global Variables
Nokia_LCD* Ar_DisplayConfigurationClass::lcd = nullptr;

Ar_DisplayConfigurationClass::Ar_DisplayConfigurationClass(/* args */)
{
  stateOfProcees = NO_STARTED;
}

Ar_DisplayConfigurationClass::~Ar_DisplayConfigurationClass()
{

}

// Public Methods
void Ar_DisplayConfigurationClass::initialConfigurationOfDisplay(void)
{
  if (FINISHED != stateOfProcees)
  {
    lcd = new Nokia_LCD(13 /* CLK */, 12 /* DIN */, 11 /* DC */, 10 /* CE */, 9 /* RST */, 9 /* BL */);
    lcd->begin();
    lcd->setBacklight(true);
    lcd->setContrast(60);
    lcd->clear(true);
    delay(2000);
    lcd->setCursor(0,3);
    lcd->print("ShroomCorp\n");
    lcd->print("Agronomist Multimeter\n");
    lcd->clear(true);
    lcd->setBacklight(false);
    stateOfProcees = FINISHED;
  }
}

void Ar_DisplayConfigurationClass::screenBackLightStatus(bool stateOfLight)
{
  if (FINISHED == stateOfProcees)
  {
    if (true == stateOfLight)
    {
      lcd->setBacklight(true);
    }
    else
    { 
      lcd->setBacklight(false);
    }
  }
  else
  {
    stateOfProcees = ABORTED;
  }
}
void Ar_DisplayConfigurationClass::writeInScreen(int cursorCoordinateX, int cursorCoordinateY, const char* textToPrint) 
{
  lcd->setBacklight(true);

  if(cursorCoordinateX < 0 || cursorCoordinateX > 84 || cursorCoordinateY < 0 || cursorCoordinateY > 48) 
  {
    lcd->print("Error: x or y coordinates out of boundaries.\n");
    stateOfProcees = ABORTED;
    delay(2000);
    lcd->clear(true);
  } 
  else 
  {
    lcd->setCursor(cursorCoordinateX, cursorCoordinateY);
    lcd->print(textToPrint);
    stateOfProcees = FINISHED;
  }

  lcd->setBacklight(false);
}

  void Ar_DisplayConfigurationClass::clearScreen(bool clearOrNot)
  {
    if (true == clearOrNot)
    {
      lcd->clear(true);
    }
  }


