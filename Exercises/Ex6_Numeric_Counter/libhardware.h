
//========== BBGAME114 HARDWARE LIBARY ==========

//Date: May 22, 2018


#ifndef LIBHARDWARE_H
#define LIBHARDWARE_H


//---------- DISPLAY CLASS ----------

// Purpose: To abstract the BBGame114 LED matrix as a display, 
// with methods/functions to clear the display, draw a dot,
// draw horizontal or vertical lines and draw text.


byte displayRows[4] = { 8, 9, 11, 12 };
byte displayCols[11] = { 2, 3, 4, 5, 6, 7, A5, A4, A3, A2, A1 };

byte displayBuffer[11];
byte scanRow = 0;


class Display
{
public:
    void setup(byte _numCols, byte _numRows);
    void clear();
    static byte makeBit(byte y);
    byte setBit(byte x, byte y, byte c);
    void setByte(byte x, byte bits);
    void vLine(byte x);
    void hLine(byte y);

private:
    byte numCols;
    byte numRows;
    void setupTimedRefresh();
};


void Display::setup(byte _numCols, byte _numRows)
{
  numCols = _numCols;
  numRows = _numRows;
  if(numCols > 11) numCols = 11;
  if(numRows > 4)  numRows = 4;

  int i,j;
  for(i = 0; i < numCols; i++) pinMode(displayCols[i], OUTPUT);
  for(j = 0; j < numRows; j++) pinMode(displayRows[j], OUTPUT);
  clear();

  setupTimedRefresh();
}


void Display::clear()
{
  byte *p = displayBuffer;
  for(byte i = 0; i < numCols; i++)
    (*(p++)) = 0;
}


static byte Display::makeBit(byte y)
{
  return (1 << (3 - y));
}


byte Display::setBit(byte x, byte y, byte c)
{
  if ((x<0) || (x>=numCols)) return;
  
  byte mask = ~(8 >> y);
  byte bits = (c << (3 - y));
  noInterrupts();
  displayBuffer[x] &= mask;
  displayBuffer[x] |= bits;
  interrupts();

  return bits;
}


void Display::setByte(byte x, byte bits)
{
  if ((x<0) || (x>=numCols)) return;
  
  noInterrupts();
  displayBuffer[x] |= bits;
  interrupts();
}


void Display::vLine(byte x)
{
  for(byte i = 0; i < numRows; i++)
    setBit(x, i, 1);
}


void Display::hLine(byte y)
{
  for(byte i = 0; i < numCols; i++)
    setBit(i, y, 1);
}


void Display::setupTimedRefresh()
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
  byte j = scanRow;
  byte on_value;

  digitalWrite(displayRows[0], 0);
  digitalWrite(displayRows[1], 0);
  digitalWrite(displayRows[2], 0);
  digitalWrite(displayRows[3], 0);
  for(byte i = 0; i < 11; i++)
    digitalWrite(displayCols[i], 0);

  digitalWrite(displayRows[j], 1);
  byte *p = displayBuffer;
  byte mask = 8 >> j; 
  for(byte i = 0; i < 11; i++)
    digitalWrite(displayCols[i], ((*(p++)) & mask) ? 1 : 0);

  noInterrupts();
  scanRow++;
  scanRow %= 4;
  interrupts();
}


//---------- BUTTONS CLASS ----------

// Purpose: To simplify the use of the pushbuttons on the 
// BBGame114 board. The btnPressed() method/function only 
// returns true one time after a buttons is pressed, and it 
// is not reset until the button is released.


byte buttonPins[3] = { A0, A7, A6 };


class Buttons
{
public:
    void setup();
    bool btnPressed(byte i);

private:
  bool btnWasNotPressed[3];
};


void Buttons::setup()
{
    for(byte i = 0; i < 3; i++)
    {
        pinMode(buttonPins[i], INPUT);
        btnWasNotPressed[i] = true;
    }
}


bool Buttons::btnPressed(byte i)
{
    bool btnPressedNow = analogRead(buttonPins[i])>30;
    if(btnWasNotPressed[i] && btnPressedNow)
    {
      btnWasNotPressed[i] = false;
      return true;
    }
    else
    {
      if(!btnPressedNow)
        btnWasNotPressed[i] = true;
      return false;
    }
}


//---------- SOUND CLASS ----------

// Purpose: To output sound tones through the piezoelectric
// speaker on the the BBGame114 board.
// NOTE: This class still needs to be reworked to remove references
// to the Space Invaders game.


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




//---------- TEXTDISPLAY CLASS ----------

// Purpose: To draw alphanumeric characters and punctuation on the
// BBGame114 display.


class TextDisplay
{
public:
  TextDisplay(const Display& _display);
  void displayText(char *s);
  int getWidthPixels(char *s, int n);

private:
  const Display& display;
  void getCharPixelData(char c, byte *np, byte **p);
};


TextDisplay::TextDisplay(const Display& _display):
  display(_display)
{
}


void TextDisplay::displayText(char *s)
{
  String str = String(s);
  byte n = str.length();
  byte np = getWidthPixels(s, n);
  int pos = (5 - np) / 2;
  byte *p;

  for(byte i=0; i< n; i++)
  {
    getCharPixelData(s[i],&np,&p);
    bool nsp = false;
    if(np>=0x10) 
    {
      nsp = true;
      np = np & 0xF;
    }
    for(byte j=0; j<np; j++)
    {
      display.setByte(pos+j, p[j]);
    }
    pos += np;
    if(!nsp && (i<(n-1)))pos += 1;
  }
}


int TextDisplay::getWidthPixels(char *s, int n)
{
  byte np;
  byte *p;

  int pos = 0;
  for(int i = 0; i < n; i++)
  {
    getCharPixelData(s[i],&np,&p);
    bool nsp = false;
    if(np>=0x10) 
    {
      nsp = true;
      np = np & 0xF;
    }
    pos += np;
    if(!nsp && (i<(n-1)))pos += 1;
  }

  return pos;
}


byte font3x4[] = 
{
//  .X.  XX.  .XX  XX.  XXX  XXX  .XX  X.X  XXX  . X  X.X  X..  X...X
//  X.X  XX.  X..  X.X  XX.  XX.  X..  XXX  .X.  ..X  XX.  X..  XX.XX
//  XXX  X.X  X..  X.X  X..  X..  X.X  X.X  .X.  ..X  X.X  X..  X.X.X
//  X.X  XXX  .XX  XX.  XXX  X..  .XX  X.X  XXX  XX.  X.X  XXX  X...X

3, 0x7, 0xA, 0x7, 0x0, 0x0,
3, 0xF, 0xD, 0x3, 0x0, 0x0,
3, 0x6, 0x9, 0x9, 0x0, 0x0,
3, 0xF, 0x9, 0x6, 0x0, 0x0,
3, 0xF, 0xD, 0x9, 0x0, 0x0,
3, 0xF, 0xC, 0x8, 0x0, 0x0,
3, 0x6, 0x9, 0xB, 0x0, 0x0,
3, 0xF, 0x4, 0xF, 0x0, 0x0,
1, 0xF, 0x0, 0x0, 0x0, 0x0,
3, 0x1, 0x1, 0xE, 0x0, 0x0,
3, 0xF, 0x4, 0xB, 0x0, 0x0,
3, 0xF, 0x1, 0x1, 0x0, 0x0,
5, 0xF, 0x4, 0x2, 0x4, 0xF,
 
//  XX.  XXX  XX.  XXX.  XX.  .XX  XXX  X.X  X.X  X...X  X.X  X.X  XXX
//  X.X  X.X  X.X  X.X.  X.X  XX.  .X.  X.X  X.X  X.X.X  .X.  X.X  ..X
//  X.X  X.X  XX.  X.X.  XX.  ..X  .X.  X.X  X.X  X.X.X  .X.  .X.  XX.
//  X.X  XXX  X..  XX.X  X.X  XX.  .X.  XXX  .X.  .X X.  X.X  .X.  XXX

3, 0xF, 0x8, 0x7, 0x0, 0x0,
3, 0xF, 0x9, 0xF, 0x0, 0x0,
3, 0xF, 0xA, 0x4, 0x0, 0x0,
4, 0xF, 0x9, 0xE, 0x1, 0x0,
3, 0xF, 0xA, 0x5, 0x0, 0x0,
3, 0x5, 0xD, 0xA, 0x0, 0x0,
3, 0x8, 0xF, 0x8, 0x0, 0x0,
3, 0xF, 0x1, 0xF, 0x0, 0x0,
3, 0xE, 0x1, 0xE, 0x0, 0x0,
5, 0xE, 0x1, 0x6, 0x1, 0xE,
3, 0x9, 0x6, 0x9, 0x0, 0x0,
3, 0xC, 0x3, 0xC, 0x0, 0x0,
3, 0xB, 0xB, 0xD, 0x0, 0x0,
//0x15, 0xA, 0xE, 0xE, 0xA, 0xA,
//0x15, 0x6, 0x6, 0x6, 0x9, 0x0,
//0x15, 0x1, 0xF, 0xA, 0xF, 0x1,
//0x15, 0x2, 0x2, 0x6, 0x6, 0xA,
//0x15, 0xA, 0x6, 0x6, 0x6, 0x9,
 
//  .X.  .X.  XX.  XX.  X.X  XXX  .XX  XXX  XXX  XXX
//  X.X  XX.  .XX  .XX  X.X  XX.  XX.  ..X  XXX  X.X
//  X.X  .X.  X..  ..X  XXX  ..X  X.X  .X.  X.X  XXX
//  .X.  .X.  XXX  XX.  ..X  XX.  XXX  .X.  XXX  ..X

3, 0x6, 0x9, 0x6, 0x0, 0x0,
2, 0x4, 0xF, 0x0, 0x0, 0x0,
3, 0xB, 0xD, 0x5, 0x0, 0x0,
3, 0x9, 0xD, 0x6, 0x0, 0x0,
3, 0xE, 0x2, 0xF, 0x0, 0x0,
3, 0xD, 0xD, 0xA, 0x0, 0x0,
3, 0x7, 0xD, 0xB, 0x0, 0x0,
3, 0x8, 0xB, 0xC, 0x0, 0x0,
3, 0xF, 0xD, 0xF, 0x0, 0x0,
3, 0xE, 0xA, 0xF, 0x0, 0x0,

//        !    "    #    $    %    &    '    (    )    *    +    ,    -    .    /
//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...
//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...
//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...
//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...

2, 0x0, 0x0, 0x0, 0x0, 0x0,

//  ...  ...  .X.  XXX  ...  ...  ...  ...  ...  ...
//  ...  ...  .X.  .XZ  XXX  ...  ...  ...  ...  ...
//  ...  .X.  ...  ...  ...  ...  ...  ...  ...  ...
//  .X.  X..  .X.  .X.  ...  ...  ...  ...  ...  ...

1, 0x0, 0x0, 0x0, 0x0, 0x0,
2, 0x0, 0x0, 0x0, 0x0, 0x0,
1, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,
3, 0x0, 0x0, 0x0, 0x0, 0x0,

};


void TextDisplay::getCharPixelData(char c, byte *np, byte **p)
{
  int index = 0;
  if((c >= 'A') && (c <= 'Z'))
    index = c - 'A';
  else if((c >= '0') && (c <= '9'))
    index = c - '0' + 26;
  else if(c >= ' ')
    index = c - ' ' + 36;
  index *= 6;
  (*np) = font3x4[index];
  (*p) = font3x4 + index + 1;
}


#endif //LIBHARDWARE_H

