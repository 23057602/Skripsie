//v9 incorporates v8 feedback into v7
//MCK/ledc err
//Includes////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "proj_Bt.h"
#include "proj_Codec.h"
#include "proj_IO.h"
#include "proj_I2S.h"
#include <esp_task_wdt.h>
//Main Program///////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);//debug
  
  //Pin out
  byte itr = 22;
  byte scl = 33;
  byte sda = 25;
  byte mck = 27;
  byte bck = 26;
  byte ws = 13;
  byte sdin = 21;
  byte sdout = 14;
  byte play = 19;
  byte source = 18;

  //I2S setup
  i2s_init((int) mck, (int) bck, (int) ws, (int) sdin, (int) sdout);
  i2s_start();
  esp_task_wdt_init(30, false);//disables watchdog
  //Bluetooth setup
  blu_init();
  blu_start();
  //Codec setup
  codec_init( (int) itr, (int) scl, (int) sda);
  codec_setup();
  //IO setup
  btn_init( (int) play, (int) source);
}

void loop() {
  // put your main code here, to run repeatedly:
  go_btns();
}
