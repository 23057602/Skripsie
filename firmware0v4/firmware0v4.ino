//v4 solves v3 space issue
//Includes////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "proj_Bt.h"
#include "proj_Codec.h"
#include "proj_IO.h"
//Main Program///////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  //Pin out
  byte itr = 19;
  byte scl = 22;
  byte sda = 21;
  byte mck = 18;
  byte bck = 16;
  byte ws = 27;
  byte sdin = 17;
  byte sdout = 4;
  byte play = 26;
  byte source = 25;
  
  Serial.begin(115200);
  blu_init();
  blu_i2s_init((int) mck, (int) bck, (int) ws, (int) sdin, (int) sdout);
  blu_start();
  codec_init( (int) itr, (int) scl, (int) sda);
  btn_init( (int) play, (int) source);
}

void loop() {
  // put your main code here, to run repeatedly:
  go_btns();
  sendData();
}
