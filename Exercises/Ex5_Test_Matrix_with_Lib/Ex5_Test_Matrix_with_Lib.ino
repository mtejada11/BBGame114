
//========== SEED Electronics/Coding Workshop Exercise 5 ==========

//Purpose: Test 5x4 LED matrix (with library)
//Date: May 22, 2018


#include "libhardware.h"
Display display;


void setup() 
{
  //Initialize LED matrix display
  display.setup(5,4);
}


void loop() 
{
  int d = 150;

  //Flash sequence of horizontal lines
  for(int y = 0; y < 4; y++)
  {
    display.clear();
    display.hLine(y);
    delay(d);
  }

  //Flash sequence of vertical lines
  for(int x = 0; x < 5; x++)
  {
    display.clear();
    display.vLine(x);
    delay(d);
  }
}

