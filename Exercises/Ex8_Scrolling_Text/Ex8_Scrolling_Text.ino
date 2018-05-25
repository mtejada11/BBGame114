
//========== SEED Electronics/Coding Workshop Exercise 6 ==========

//Purpose: Numeric counter on 5x4 LED matrix
//Date: May 24, 2018


#include "libhardware.h"
Display display;
TextDisplay textDisplay(display);


int counter = 0;


void setup() 
{
  //Initialize LED matrix display
  display.setup(11,4);
}

int pos = 16;

void loop() 
{
  //Create text string from counter value
  char text[21] = "ABCDEF abcdef 012345";

  //Draw text string to display
  display.clear();
  textDisplay.displayText(text, 21, pos);

  pos--;
  delay(150);
}




