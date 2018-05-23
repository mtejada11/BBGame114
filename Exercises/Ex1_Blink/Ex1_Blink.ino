
//========== SEED Electronics/Coding Workshop Exercise 1 ==========

//Purpose: Blink one LED
//Date: May 22, 2018


#define LED1 2


//---------- Setup routine to run once ----------

void setup()  
{ 
  //Set up pin to use
  pinMode(LED1,OUTPUT);
}


//---------- Loop routine that runs repeatedly ----------

void loop()  
{
  //Turn LED on
  digitalWrite(LED1,HIGH);
  delay(200);

  //Turn LED off
  digitalWrite(LED1,LOW);
  delay(200);
}

