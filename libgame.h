
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
    byte a = Display::makeBit(pos);
    
    switch(state)
    {
    case 1://Normal
      display.setBit(0,pos,1);
      bitImage = display.setBit(1,pos,1);
      break;

    case 2:
    case 4:
    case 6:
      //Explosion 1
      //display.setByte(0, (a<<1)|(a>>1));
      //display.setByte(1, (a<<1)|(a>>1));
      //display.setByte(2, a);
      state++;
      break;

    case 3:
    case 5:
      //Explosion 2
      display.setByte(0,a);
      display.setByte(1,a);
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
        bitImage = display.setBit(altitude,pos,1);
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
    return Display::makeBit(y);
}

byte NewAlienLevel2(int counter)
{
    byte y = (counter2++ % 2) + 1;
    return Display::makeBit(y);
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
        display.setByte(pos[i],a);
        break;

      case 2:
      case 4:
      case 6:
        //Explosion 1
        display.setByte(pos[i]-1, a);
        display.setByte(pos[i], (a<<1)|(a>>1));
        display.setByte(pos[i]+1, a);
        state[i]++;
        break;

      case 3:
      case 5:
        //Explosion 2
        display.setByte(pos[i],a);
        state[i]++;
        break;

      case 0:
      default:
        state[i] = 0;
        pos[i] = 11;
        break;
      }
    }
    display.setByte(pos[i],aliens[i]);
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


#endif //LIBGAME_H
