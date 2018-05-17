
//========== BBGAME114 EXERCISE 4 TEST MATRIX ==========

//Date: May 17, 2018


#include "libhardware.h"


HwSetup hwsetup;
Display display;


void setup() 
{
  hwsetup.ReadFromEeprom();
 
  display.setup();
  display.clear();
  display.setup_refresh();
}


void loop() 
{
  int d = 120;  
  Demo_Swipes(4,5,d,d);
}


void Demo_Swipes(byte nR, byte nC, int d1, int d2) 
{
  byte x, y;

  for(x = 0; x < nC; x++)
  {
    display.clear();
    for(y = 0; y < nR; y++)
      display.SetBit(x,y,1);
    delay(d1);
  }
  
  for(x = nC; x > 0; x--)
  {
    display.clear();
    for(y = 0; y < nR; y++)
      display.SetBit(x-1,y,1);
    delay(d1);
  }
  
  display.clear();
  delay(500);

  for(y = nR; y > 0; y--)
  {
    display.clear();
    for(x = 0; x < nC; x++)
      display.SetBit(x,y-1,1);
    delay(d2);
  }
  
  for(y = 0; y < nR; y++)
  {
    display.clear();
    for(x = 0; x < nC; x++)
      display.SetBit(x,y,1);
    delay(d2);
  }

  display.clear();
  delay(d2*4);
}

