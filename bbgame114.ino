
//========== BBGAME114 MAIN FILE ==========

//Date: October 17, 2017


#include "libhardware.h"
#include "libgame.h"


Display display;
Sound sound;
Buttons buttons;
Ship ship;
Missile missile;
Aliens aliens;
TextDisplay textDisplay(display);


int score = 0;


void setup() 
{
  //hwsetup.ReadFromEeprom();
  display.setup(11, 4);
  buttons.setup();
  sound.setup();

  //if (buttons.BtnPressed(0))
  //  display.Test();
}

int d = 20;
int counter1 = 0;
int counterb2 = 0;

typedef enum { INTRO, PLAY, SCORE } GameStage;
GameStage stage = PLAY;

void SetStageScore()
{
  stage = SCORE;
  counterb2 = 0;
}


void loop() 
{
  switch(stage)
  {
  case PLAY:
    loop_GamePlay();
    break;
  case SCORE:
    loop_GameScore();
    break;
  case INTRO:
  default:
    break;
  }

  counter1++;
  delay(d);
}


void loop_GamePlay() 
{
  display.clear();

  ship.Draw(display);
  missile.Draw(display);
  aliens.Draw(display);
  sound.update(counter1);

  missile.CheckStrike(aliens);
  ship.CheckAlienStrike(aliens);
  missile.Update(counter1);

  if (counter1%(levels[aliens.level].update_interval)==0) aliens.AddAlien(counter1);
  if (counter1%20==0) aliens.Update(counter1);

  if(buttons.btnPressed(0)) { missile.Fire(ship.pos); sound.missileFiringSound(); };
  if(buttons.btnPressed(1)) ship.Move(+1);
  if(buttons.btnPressed(2)) ship.Move(-1);
}


void loop_GameScore() 
{
  if (counterb2 == 0)
  {
    display.clear();
    char scoreText[6];
    //score = 97;
    itoa(score, scoreText, 10);
    textDisplay.displayText(scoreText);
    //hwsetup.StoreRndSeed((byte)score);
  }
  
  counterb2++;
}

