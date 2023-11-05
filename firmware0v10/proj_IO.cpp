#include "proj_Bt.h"
//IO variables//////////////////////////////////////////////////////
bool isBt = true;
volatile int playPauseFlg = 0;
volatile int modeFlg = 0;
//IO pin
int playPause_pin;
int mode_pin;

//IO functions/////////////////////////////////////////////////////
void IRAM_ATTR playPause_pressed();
void IRAM_ATTR mode_pressed();

void btn_init(int play, int source){
  playPause_pin = play;
  mode_pin = source;
  
  pinMode(playPause_pin,INPUT);
  attachInterrupt(playPause_pin, playPause_pressed, RISING);
  pinMode(mode_pin,INPUT);
  attachInterrupt(mode_pin, mode_pressed, RISING);
}

void IRAM_ATTR playPause_pressed(){
   playPauseFlg++;
}

void playPause(){
  static unsigned long lastCall = 0;
  if (millis() - lastCall < 100){//debounce
    playPauseFlg = 0;
    return;
    }

  //toggle play or pause
  if(isBt){//check mode
    if(blu_isPlaying()){//bluetooth play/pause
      blu_pause();
    }else{
      blu_play();
    }
  }

  //reset flags
  playPauseFlg = 0;
  lastCall = millis();
}

void IRAM_ATTR mode_pressed(){
   modeFlg++;
}

void changeMode(){
  static unsigned long lastCall = 0;
  if (millis() - lastCall < 100){//debounce
    modeFlg = 0;
    return;
    }

  //toggle modes
  if(isBt){
    blu_stop();
    isBt = false;
  }else{
    blu_start();
    isBt = true;
  }
  
  //reset flags
  modeFlg = 0;
  lastCall = millis();
}

void go_btns(){
  if(playPauseFlg > 0){playPause();}
  if(modeFlg > 0){changeMode();}
}
