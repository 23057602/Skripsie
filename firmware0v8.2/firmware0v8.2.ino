//v8.2 tests Codec power on
//Includes////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include "proj_Bt.h"
#include "proj_Codec.h"
//#include "proj_IO.h"
//#include "proj_I2S.h"
#include <esp_task_wdt.h>
//Main Program///////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  esp_task_wdt_init(30, false);
  //Pin out
  byte itr = 32;
  byte scl = 33;
  byte sda = 25;
  byte mck = 27;
  //byte bck = 33;
  //byte ws = 25;
  //byte sdin = 26;
  //byte sdout = 14;
  //byte play = 26;
  //byte source = 25;

  //i2s_init((int) mck, (int) bck, (int) ws, (int) sdin, (int) sdout);
  //i2s_start();
  //blu_init();
  //blu_start();
  codec_init( (int) itr, (int) scl, (int) sda);
  //btn_init( (int) play, (int) source);
  //ledcAttachPin(mck, 0);
  //ledcSetup(0, 256*44100, 2);
  //ledcWrite(0, 2);
  codec_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  ///go_btns();
}
