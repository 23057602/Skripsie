#include <WiFi.h>
#include <WiFiUdp.h>
#include <I2S.h>
#include <Arduino.h>
//WiFi variables/////////////////////////////////////////////////////////////
WiFiUDP udp;
int udpPort = 3333;
byte bclk;
byte lr;
byte sdin;
byte sdout;
uint32_t len = 1000;
uint8_t buf[1000] = {0};

//WiFi Prototypes////////////////////////////////////////////////////////////
void buf_endCb();
//WiFi functions////////////////////////////////////////////////////////////
void wifi_init(int bck, int ws, int sdi,  int sdo){
  bclk = (char)bck;
  lr = (char)ws;
  sdin = (char)sdi;
  sdout = (char)sdo;
}

void wifi_start(){
  char * ssid = "wifi-name";
  char * pwrd = "password";
  //IPAddress ip(10, 0, 0, 60);
  //IPAddress df(10, 0, 0, 2);
  
  WiFi.disconnect(true);
  //WiFi.config(ip,df,df);
  WiFi.begin(ssid);//, pwrd);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  udp.begin(udpPort);
  I2S.setDuplex();
  I2S.setAllPins((int)bclk, (int)lr, (int)sdout, (int)sdout, sdin);
  I2S.onTransmit(buf_endCb);
  int sample_rate = 44100;
  i2s_mode_t mode = I2S_PHILIPS_MODE;
  byte bit_depth = 16;
  I2S.begin(mode, sample_rate, bit_depth);

  while(!udp.parsePacket()){
    delay(500);
  }
  while(udp.available() < len){
    delay(500);
  }
  udp.read(buf,len);
  I2S.write(buf, len >> 1);
}

void buf_endCb(){
  static byte i = 0;
  if(i = 0){
    I2S.write(buf + (len >> 1), len >> 1);
    udp.read(buf,len >> 1);
    i = 1;
  }else{
    I2S.write(buf, len >> 1);
    udp.read(buf + (len >> 1),len >> 1);
    i = 0;
  } 
}

void wifi_stop(){
  udp.stop();
  WiFi.disconnect(true);
  I2S.end();
}
