//v13 tests wifi
//Includes////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "proj_Bt.h"
#include "proj_Codec.h"
#include "proj_IO.h"
#include "proj_WiFi.h"
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

  esp_task_wdt_init(30, false);//disables watchdog
  //Bluetooth setup
  //blu_init((int) mck, (int) bck, (int) ws, (int) sdin, (int) sdout);
  //blu_start();
  //WiFi
  ledcAttachPin(mck, 0);
  ledcSetup(0, 256*44100, 2);
  ledcWrite(0, 2);
  
  //Codec setup
  codec_init( (int) itr, (int) scl, (int) sda);
  codec_setup();
  wifi_init((int) bck, (int) ws, (int) sdin,  (int) sdout);
  wifi_start();
  //IO setup
  //btn_init( (int) play, (int) source);
}

void loop() {
  // put your main code here, to run repeatedly:
  //go_btns();
}
