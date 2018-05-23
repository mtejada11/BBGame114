
//========== BBGAME114 HARDWARE LIBARY ==========

//Date: May 22, 2018


#ifndef LIBHARDWARE_H
#define LIBHARDWARE_H


byte display_rows[] = {8,9,11,12};
byte display_cols[] = {2,3,4,5,6,7,A5,A4,A3,A2,A1};
byte btn_pins[3] = {A0,A7,A6};

byte display_buffer[11];
byte scan_row = 0;


class Display
{
public:
    void setup(byte _numCols, byte _numRows);
    void clear();
    void setup_refresh();
    static byte MakeBit(byte y);
    byte SetBit(byte x, byte y, byte c);
    void SetByte(byte x, byte bits);
    void VLine(byte x);
    void HLine(byte y);

private:
    byte numCols;
    byte numRows;
};


void Display::setup(byte _numCols, byte _numRows)
{
  numCols = _numCols;
  numRows = _numRows;

  if(numCols > 11) numCols = 11;
  if(numRows > 4)  numRows = 4;

  int i,j;
  for(i=0; i<numCols; i++) pinMode(display_cols[i],OUTPUT);
  for(j=0; j<numRows; j++) pinMode(display_rows[j],OUTPUT);
  clear();
  setup_refresh();

}


void Display::clear()
{
  byte *p = display_buffer;
  for(byte i = 0; i<numCols; i++)
    (*(p++)) = 0;
}


static byte Display::MakeBit(byte y)
{
  return (1 << (3 - y));
}


byte Display::SetBit(byte x, byte y, byte c)
{
  if ((x<0) || (x>=numCols)) return;
  
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
  if ((x<0) || (x>=numCols)) return;
  
  noInterrupts();
  display_buffer[x] |= bits;
  interrupts();
}


void Display::VLine(byte x)
{
  for(byte i = 0; i < numRows; i++)
    SetBit(x,i,1);
}


void Display::HLine(byte y)
{
  for(byte i = 0; i < numCols; i++)
    SetBit(i,y,1);
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

  digitalWrite(display_rows[0],0);
  digitalWrite(display_rows[1],0);
  digitalWrite(display_rows[2],0);
  digitalWrite(display_rows[3],0);
  for(byte i = 0; i < 11; i++)
    digitalWrite(display_cols[i], 0);

  digitalWrite(display_rows[j],1);
  byte *p = display_buffer;
  byte mask = 8 >> j; 
  for(byte i = 0; i < 11; i++)
    digitalWrite(display_cols[i], ((*(p++)) & mask) ? 1 : 0);

  noInterrupts();
  scan_row++;
  scan_row %= 4;
  interrupts();
}


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


class Buttons
{
public:
    void Setup();
    bool BtnPressed(byte i);
};


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

