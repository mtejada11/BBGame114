
//========== SEED Electronics/Coding Workshop Exercise 2 ==========

//Purpose: LEDs blink in sequence
//Date: May 22, 2018


//--- Constant definitions ---
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6


//---------- Setup routine to run once ----------

void setup()  
{ 
  //Set up pins to use
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
}


//--- Global variable ---
int counter = 1;


//---------- Loop routine that runs repeatedly ----------

void loop()  
{
  //Turn on one LED according to counter
  if(counter==1) digitalWrite(LED1,HIGH);
  if(counter==2) digitalWrite(LED2,HIGH);
  if(counter==3) digitalWrite(LED3,HIGH);
  if(counter==4) digitalWrite(LED4,HIGH);
  if(counter==5) digitalWrite(LED5,HIGH);
  delay(200);

  //Increment counter variable
  counter = counter + 1;
  if(counter > 5) counter = 1;

  //Turn off all LEDs for an instant (no delay after)
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);
  digitalWrite(LED5,LOW);
}

