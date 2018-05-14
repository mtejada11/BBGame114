
//========== BBGAME114 GAME LIBRARY ==========

//Date: October 17, 2017



#ifndef LIBGAME_H
#define LIBGAME_H


#include "libhardware.h"



extern Display display;

extern Sound sound;
extern int score;

extern void SetStageScore();


struct Level
{
    int start;
    int end;
    int update_interval;
    byte points;
    byte (*NewAlienFunc)(int counter);
};

byte NewAlienLevel1(int counter);
byte NewAlienLevel2(int counter);
byte NewAlienLevel3(int counter);
byte NewAlienLevelEmpty(int counter);

struct Level levels[] = 
{
  {  200, 1000, 60, 1, NewAlienLevel1 },
  { 1100, 1300, 20, 2, NewAlienLevel2 },
  { 1400, 1900, 40, 4, NewAlienLevel1 },
  { 2000, 2300, 20, 8, NewAlienLevel2 },
  { 2400, 2600, 20, 8, NewAlienLevel3 },
  { 2700, 2880, 20, 8, NewAlienLevelEmpty }
};

#define NUM_LEVELS 6

class Aliens
{
public:
    Aliens();
    void AddAlien(int counter);
    void Draw(Display &display);
    void Update(int counter);

    byte aliens[12];
    byte pos[12];
    byte state[12];
    byte num;
    byte next;

    byte level;
};


#define ALIEN_HIT 2


class Ship
{
public:
    Ship();
    void Draw(Display &display);
    void Move(int8_t dir);
    void CheckAlienStrike(Aliens &aliens);

    byte pos;
    byte bitImage;
    byte state;
};


class Missile
{
public:
    Missile();
    void Draw(Display &display);
    void Update(int counter);
    void Fire(int shipPos);
    void CheckStrike(Aliens &aliens);

    bool fired;
    int8_t pos;
    int8_t altitude;
    byte bitImage; //not used yet
};



Ship::Ship()
{
    pos = 2;
    bitImage = 0;
    state = 1;
}


void Ship::Draw(Display &display)
{
    byte a = Display::MakeBit(pos);
    
    switch(state)
    {
    case 1://Normal
      display.SetBit(0,pos,1);
      bitImage = display.SetBit(1,pos,1);
      break;

    case 2:
    case 4:
    case 6:
      //Explosion 1
      //display.SetByte(0, (a<<1)|(a>>1));
      //display.SetByte(1, (a<<1)|(a>>1));
      //display.SetByte(2, a);
      state++;
      break;

    case 3:
    case 5:
      //Explosion 2
      display.SetByte(0,a);
      display.SetByte(1,a);
      state++;
      break;

    case 0:
    default:
      state = 1;
      break;
    }
}


void Ship::Move(int8_t dir)
{
  if((dir==+1) && (pos < 3)) pos++;
  if((dir==-1) && (pos > 0)) pos--;
}


void Ship::CheckAlienStrike(Aliens &aliens)
{
  if (state != 1) return;
  
  for(byte i=0; i<aliens.num; i++)
  {
    if (aliens.state[i]==1)
    {
      if ((aliens.pos[i] <= 1) && (aliens.aliens[i] & bitImage))
      {
        state = 2;
        aliens.state[i] = 0;
        aliens.pos[i] = 11;
      }
    }
  }
}


Missile::Missile()
{
    fired = false;
    pos = -1;
    altitude = -1;
    bitImage = 0;
}


void Missile::Draw(Display &display)
{
    if (fired)
        bitImage = display.SetBit(altitude,pos,1);
}


void Missile::Update(int counter)
{
  if(altitude > 10)
  {
      fired = false;
      return;
  } 

  if(counter%1==0)
  {
    if(fired) altitude++;
  }
}


void Missile::Fire(int shipPos)
{
  if(!fired)
  {
    fired = true;
    altitude = 1;
    pos = shipPos;
  }
}


void Missile::CheckStrike(Aliens &aliens)
{
  if(!fired) return;

  for(byte i=0; i<aliens.num; i++)
    if((aliens.pos[i]==altitude) && (aliens.state[i]!=0))
    {
      if(bitImage & aliens.aliens[i])
      {
        aliens.state[i] = ALIEN_HIT;
        pos = 11;
        fired = false;
        sound.alienExplosionSound();
        score++;
      }
    }
}


int counter2 = 0;


byte NewAlienLevel1(int counter)
{
    byte y = random(4);
    return Display::MakeBit(y);
}

byte NewAlienLevel2(int counter)
{
    byte y = (counter2++ % 2) + 1;
    return Display::MakeBit(y);
}

byte NewAlienLevel3(int counter)
{
    counter2++;
    byte y;
    if (counter2 == 1) y = 0x6;
    if (counter2 >= 2) y = 0xF;
    if (counter2 >= 5) y = 0x6;
    if (counter2 >= 8) y = 0x0;
    return y;
}

byte NewAlienLevelEmpty(int counter)
{
    return 0;
}

Aliens::Aliens()
{
    num = 0;
    next = 0;
    level = 0;
}

void Aliens::AddAlien(int counter)
{
    Level *L = &(levels[level]);
    if (counter > L->end) 
    {
      counter2 = 0;
      level++;
    }
    if (level >= NUM_LEVELS)
    {
      SetStageScore();
      return;
    }
    L = &(levels[level]);
    if ((counter < L->start) || (counter > L->end)) return;
    
    byte i = next;
    if (num < 12) num++; 

    aliens[i] = L->NewAlienFunc(counter);
    pos[i] = 11;
    state[i] = 1;

    next++;
    next %= 12;
}


void Aliens::Draw(Display &display)
{
  for(byte i=0; i<num; i++)
  {
    if(state[i] != 0)
    {
      byte a = aliens[i];
      switch(state[i])
      {
      case 1://Normal
        display.SetByte(pos[i],a);
        break;

      case 2:
      case 4:
      case 6:
        //Explosion 1
        display.SetByte(pos[i]-1, a);
        display.SetByte(pos[i], (a<<1)|(a>>1));
        display.SetByte(pos[i]+1, a);
        state[i]++;
        break;

      case 3:
      case 5:
        //Explosion 2
        display.SetByte(pos[i],a);
        state[i]++;
        break;

      case 0:
      default:
        state[i] = 0;
        pos[i] = 11;
        break;
      }
    }
    display.SetByte(pos[i],aliens[i]);
  }
}


void Aliens::Update(int counter)
{
  for(byte i=0; i<num; i++)
  {
    if(state[i]==1)
    {
        if(pos[i]>=0) 
            pos[i]--;
        else
        {
            pos[i] = 11;
            state[i] = 0;
        }
    }
  }
}




class TextDisplay
{
public:
  void DisplayText(char *s);
  int GetPixelWidth(char *s, int n);
  void GetCharPixelData(char c, byte *np, byte **p);
};


void TextDisplay::DisplayText(char *s)
{
  String str = String(s);
  byte n = str.length();
  byte np = GetPixelWidth(s, n);
  int pos = (11 - np) / 2;
  byte *p;

  for(byte i=0; i< n; i++)
  {
    GetCharPixelData(s[i],&np,&p);
    bool nsp = false;
    if(np>=0x10) 
    {
      nsp = true;
      np = np & 0xF;
    }
    for(byte j=0; j<np; j++)
    {
      display.SetByte(pos+j, p[j]);
    }
    pos += np;
    if(!nsp && (i<(n-1)))pos += 1;
  }
}


int TextDisplay::GetPixelWidth(char *s, int n)
{
  byte np;
  byte *p;

  int pos = 0;
  for(int i = 0; i < n; i++)
  {
    GetCharPixelData(s[i],&np,&p);
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


void TextDisplay::GetCharPixelData(char c, byte *np, byte **p)
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


#endif //LIBGAME_H
