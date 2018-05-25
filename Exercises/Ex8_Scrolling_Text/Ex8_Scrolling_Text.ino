
//========== SEED Electronics/Coding Workshop Exercise 8 ==========

//Purpose: Scrolling text on 11x4 LED matrix
//Date: May 24, 2018


#include "libhardware.h"
Display display;
TextDisplay textDisplay(display);


void setup() 
{
  //Initialize LED matrix display
  display.setup(11,4);
}


int counter = 0;
String text = "ABCDEF 012345";

void loop() 
{
  //Draw text using scrolling text function and counter for position
  textDisplay.scrollText(text, counter);
  counter++;
  delay(100);
}




