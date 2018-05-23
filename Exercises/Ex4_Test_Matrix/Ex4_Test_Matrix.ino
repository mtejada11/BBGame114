
//========== SEED Electronics/Coding Workshop Exercise 4 ==========

//Purpose: Test 5x4 LED matrix
//Date: May 22, 2018


//--- Constant definitions ---
#define C1 2
#define C2 3
#define C3 4
#define C4 5
#define C5 6
#define R1 8
#define R2 9
#define R3 11
#define R4 12


//---------- Setup routine to run once ----------

void setup()  
{ 
  //Set up pins to use
  pinMode(C1,OUTPUT);
  pinMode(C2,OUTPUT);
  pinMode(C3,OUTPUT);
  pinMode(C4,OUTPUT);
  pinMode(C5,OUTPUT);
  pinMode(R1,OUTPUT);
  pinMode(R2,OUTPUT);
  pinMode(R3,OUTPUT);
  pinMode(R4,OUTPUT);
}


//--- Global variable ---
int counter = 1;


//---------- Loop routine that runs repeatedly ----------

void loop()  
{
  //Turn on one LED matrix row according to counter...
  if(counter==1) digitalWrite(R1,HIGH);
  if(counter==2) digitalWrite(R2,HIGH);
  if(counter==3) digitalWrite(R3,HIGH);
  if(counter==4) digitalWrite(R4,HIGH);

  if(counter < 5)
  {
    //...and turn on all LED matrix columns
    digitalWrite(C1,HIGH);
    digitalWrite(C2,HIGH);
    digitalWrite(C3,HIGH);
    digitalWrite(C4,HIGH);
    digitalWrite(C5,HIGH);
  }
  
  //Or, turn on one LED matrix column according to counter
  if(counter==5) digitalWrite(C1,HIGH);
  if(counter==6) digitalWrite(C2,HIGH);
  if(counter==7) digitalWrite(C3,HIGH);
  if(counter==8) digitalWrite(C4,HIGH);
  if(counter==9) digitalWrite(C5,HIGH);

  if(counter >= 5)
  {
    //...and turn on all LED matrix rows
    digitalWrite(R1,HIGH);
    digitalWrite(R2,HIGH);
    digitalWrite(R3,HIGH);
    digitalWrite(R4,HIGH);
  }

  delay(150);

  //Increment counter variable
  counter = counter + 1;
  if(counter > 9) counter = 1;

  //Turn off all LEDs for an instant (no delay after)
  digitalWrite(C1,LOW);
  digitalWrite(C2,LOW);
  digitalWrite(C3,LOW);
  digitalWrite(C4,LOW);
  digitalWrite(C5,LOW);
  digitalWrite(R1,LOW);
  digitalWrite(R2,LOW);
  digitalWrite(R3,LOW);
  digitalWrite(R4,LOW);
}

