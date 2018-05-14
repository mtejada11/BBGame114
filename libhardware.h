
//========== BBGAME114 HW LIBARY ==========

//Date: October 17, 2017


#ifndef LIBHARDWARE_H
#define LIBHARDWARE_H

#include <EEPROM.h>


class HwSetup
{
public:
  byte id;
  bool simpleMode;
  byte rndSeed;
  void ReadFromEeprom();
  void StoreRndSeed(byte x);
};


extern byte *display_rows;
extern byte *display_cols;
extern byte *btn_pins;
extern bool inv_polarity_rows23;


//03 Project proposal
byte display_rows_03[] = {8,9,11,12};
byte display_cols_03[] = {2,3,4,5,6,7,A5,A4,A3,A2,A1};
byte btn_pins_03[3] = {A0,A7,A6};


void HwSetup::ReadFromEeprom()
{
  //PROM.get(0,'A');
  //PROM.get(1,'L');
  //PROM.get(2,'I');
  EEPROM.get(3, id);
  id = 3;
  EEPROM.get(4, simpleMode);
  EEPROM.get(5, rndSeed);
  randomSeed(rndSeed);

  switch(id)
  {
  case 3:
  default:
    display_rows = display_rows_03;
    display_cols = display_cols_03;
    btn_pins = btn_pins_03;
    inv_polarity_rows23 = false;
    break;
  }
}


void HwSetup::StoreRndSeed(byte x)
{
  EEPROM.put(5, x);
  randomSeed(x);
}


#ifndef DISPLAY_H
#define DISPLAY_H


class Display
{
public:
    void setup();
    void clear();
    void setup_refresh();
    static byte MakeBit(byte y);
    byte SetBit(byte x, byte y, byte c);
    void SetByte(byte x, byte bits);

    void Test();

private:
};


byte *display_rows; //display_rows[] = {8,6,9,11};
byte *display_cols; //display_cols[] = {12,7,5,4,3,2,A5,A4,A3,A2,A1};
byte display_buffer[11];

bool inv_polarity_rows23 = false;

byte scan_row = 0;
byte scan_count = 0;


void Display::setup()
{
  int i,j;
  for(i=0; i<11; i++) pinMode(display_cols[i],OUTPUT);
  for(j=0; j<4;  j++) pinMode(display_rows[j],OUTPUT);
}


void Display::clear()
{
  byte *p = display_buffer;
  for(byte i = 0; i<11; i++)
    (*(p++)) = 0;
}


static byte Display::MakeBit(byte y)
{
  return (1 << (3 - y));
}


byte Display::SetBit(byte x, byte y, byte c)
{
  if ((x<0) || (x>10)) return;
  
  byte mask = ~(8 >> y);
  byte bits = (c << (3 - y));
  noInterrupts();
  display_buffer[x] &= mask;
  display_buffer[x] |= bits;
  interrupts();
  return bits;
}


void Display::SetByte(byte x, byte bits)
{
  if ((x<0) || (x>10)) return;
  
  noInterrupts();
  display_buffer[x] |= bits;
  interrupts();
}


void Display::Test()
{
  byte i;
  byte x, y;
  int d = 500;

  while(true)
  {
    clear();
    for(y=0; y<4; y++)
    {
      for(x=0; x<11; x++) SetBit(x, y-1, 0);
      for(x=0; x<11; x++) SetBit(x, y, 1);
      delay(d);
    }
  
    clear();
    for(x=0; x<11; x++)
    {
      for(y=0; y<4; y++) SetBit(x-1, y, 0);
      for(y=0; y<4; y++) SetBit(x, y, 1);
      delay(d);
    }
  }
}


void Display::setup_refresh()
{
  cli();//stop interrupts

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0

  // set compare match register for 8khz increments
  OCR2A = 63;// = n / (8000*8) - 1 (must be <256)

  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS20); //100 - 64 scaler, 1 ms
  TCCR2B |= (1 << CS21); //110 - 256 scaler, 4 ms
  TCCR2B |= (1 << CS22); //111 - 1024 scaler, 16 ms

  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei();//allow interrupts
}


SIGNAL(TIMER2_COMPA_vect) 
{
  byte j = scan_row;
  byte on_value;
  if(!inv_polarity_rows23)
    on_value = 1;
  else
    on_value = ((j==0)||(j==3)) ? 1 : 0;
  byte off_value = (on_value) ? 0 : 1;

  digitalWrite(display_rows[0],0);
  digitalWrite(display_rows[1],0);
  digitalWrite(display_rows[2],0);
  digitalWrite(display_rows[3],0);
  for(byte i = 0; i < 11; i++)
    digitalWrite(display_cols[i], off_value);

  //if(scan_count == 1)
  {
    digitalWrite(display_rows[j],1);
    byte *p = display_buffer;
    byte mask = 8 >> j; 
    for(byte i = 0; i < 11; i++)
      digitalWrite(display_cols[i], ((*(p++)) & mask) ? on_value : off_value);
  }

  noInterrupts();
  scan_row++;
  scan_row %= 4;
  scan_count++;
  scan_count %= 2;
  interrupts();
}


#endif //MISSILE_H

#ifndef SOUND_H
#define SOUND_H


#define SNDPIN 10

class Sound
{
public:
    void setup();
    void update(int counter);
    void missileFiringSound();
    void alienExplosionSound();
    void shipHitSound();
    byte missileFiring;
    byte alienExplosion;
    byte shipHit;
};


void Sound::setup()
{
  pinMode(SNDPIN,OUTPUT);
  missileFiring = 0;
  alienExplosion = 0;
  shipHit = 0;
}


void Sound::update(int counter)
{
  if (missileFiring>0) missileFiring--;
  if (alienExplosion>0) alienExplosion--;
  if (shipHit>0) shipHit--;

  if (missileFiring)
    analogWrite(SNDPIN,128);
  else if (alienExplosion)
  {
    if (counter%2 == 0) 
      analogWrite(SNDPIN,192);
    else
      analogWrite(SNDPIN,0);
  }
  else if (shipHit)
    analogWrite(SNDPIN,64);
  else
    analogWrite(SNDPIN,0);    
}


void Sound::missileFiringSound()
{
  missileFiring = 5;
}


void Sound::alienExplosionSound()
{
  alienExplosion = 15;
}


void Sound::shipHitSound()
{
  shipHit = 10;
}


#endif //SOUND_H


class Buttons
{
public:
    void Setup();
    bool BtnPressed(byte i);
};


byte *btn_pins; //btn_pins[3] = {A0,A6,A7};
bool btn_not_pressed[3] = { true, true, true };


void Buttons::Setup()
{
    for(byte i=0; i<3; i++)
        pinMode(btn_pins[i],INPUT);
}


bool Buttons::BtnPressed(byte i)
{
    bool btn_pressed = analogRead(btn_pins[i])>30;
    if(btn_not_pressed[i] && btn_pressed)
    {
      btn_not_pressed[i] = false;
      return true;
    }
    else
    {
      if(!btn_pressed)
        btn_not_pressed[i] = true;
      return false;
    }
}


#endif //LIBHARDWARE_H

