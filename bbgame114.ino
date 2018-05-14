
//========== BBGAME114 MAIN FILE ==========

//Date: October 17, 2017


#include "libgame.h"
#include "libhardware.h"


HwSetup hwsetup;
Display display;
Sound sound;
Buttons buttons;
Ship ship;
Missile missile;
Aliens aliens;
TextDisplay textDisplay;


int score = 0;


void setup() 
{
  hwsetup.ReadFromEeprom();
  
  display.setup();
  display.clear();
  display.setup_refresh();
  sound.setup();

  buttons.Setup();

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

  if(buttons.BtnPressed(0)) { missile.Fire(ship.pos); sound.missileFiringSound(); };
  if(buttons.BtnPressed(1)) ship.Move(+1);
  if(buttons.BtnPressed(2)) ship.Move(-1);
}


void loop_GameScore() 
{
  if (counterb2 == 0)
  {
    display.clear();
    char scoreText[6];
    //score = 97;
    itoa(score, scoreText, 10);
    textDisplay.DisplayText(scoreText);
    hwsetup.StoreRndSeed((byte)score);
  }
  
  counterb2++;
}

