
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
  display.setup(5,4);
}


void loop() 
{
  //Create text string from counter value
  String text(counter);

  //Draw text string to display
  display.clear();
  textDisplay.displayTextCentered(text);
  delay(500);

  //Increment counter
  counter++;
  if (counter > 9) counter = 0;
}

