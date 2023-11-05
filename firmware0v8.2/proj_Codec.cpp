#include "Wire.h"
#include "Arduino.h"
//I2C variables////////////////////////////////////////////////////


//codec variables//////////////////////////////////////////////////////////////////////////////////
char i2c_address = 0x20 >> 1;
volatile char i2c_flgs;

//I2C functions////////////////////////////////////////////////////
void itr_init(int itr);
void IRAM_ATTR codec_itr();

void codec_init(int itr, int scl, int sda){
  int i2c_clk_frq = 100000;
  Wire.begin(sda, scl, i2c_clk_frq);
  //itr_init(itr);
}

char i2c_read(char addr, char reg){
  //set register
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission(false);
  //read byte
  char b;
  Wire.requestFrom((uint8_t)addr , (uint8_t) 1);
  while(Wire.available()){
    b = Wire.read();
  }
  return b;
}

void i2c_write(char addr, char reg, char b){
  Wire.beginTransmission(addr);//start tx
  Wire.write(reg);//set register
  Wire.write(b);//write register
  Wire.endTransmission();//stop
  delay(1000);
  if( b == i2c_read(addr, reg)){
    Serial.println("Write was successful.");
  }else{
    Serial.println("Write failed.");
  }
}

//Codec function//////////////////////////////////////////////////////////////////////////////////
void itr_init(int itr){
  //setup interrupt line
  pinMode(itr,INPUT);
  attachInterrupt(itr, codec_itr, FALLING);
}

void IRAM_ATTR codec_itr(){
  char codec_itr_reg = 0x00;
  i2c_flgs = i2c_read(i2c_address, codec_itr_reg);
}

char codec_heartbeat(){
  char codec_rev_reg = 0xFF;
  return i2c_read(i2c_address, codec_rev_reg);
}

void codec_setup(){
  char regist;
  char val;
  
  //Power and config
  regist = 0x4D;//Headphone enable
  val = 0xC0;//left and right enable
  i2c_write(i2c_address, regist, val);

  regist = 0x4E;//Headphone enable
  val = 0x30;//left and right enable
  i2c_write(i2c_address, regist, val);

  regist = 0x51;//Headphone enable
  val = 0x80;//left and right enable
  i2c_write(i2c_address, regist, val);
  
  //clock setup
  regist = 0x10;//mck prescaler
  val = 0x10; //set as 01
  i2c_write(i2c_address, regist, val);
  
  regist = 0x11;//MCU sample rate
  val = 0x70;//44.1kHz and freq1 disabled
  i2c_write(i2c_address, regist, val);

  regist = 0x12;//MCU LRCK diveder
  val = 0x60;//11.2896MHz to 44.1kHz
  i2c_write(i2c_address, regist, val);
  
  regist = 0x19;//ANC sample rate
  val = 0x70;//44.1kHz
  i2c_write(i2c_address, regist, val);

  regist = 0x1A;//ANC LRCK divider
  val = 0x70;//11.2896MHz to 44.1kHz
  i2c_write(i2c_address, regist, val);

  //I2S setup
  regist = 0x14;//MCU I2S
  val = 0x10;//set as I2S
  i2c_write(i2c_address, regist, val);

  regist = 0x1C;//ANC I2S
  val = 0x91;//set as I2S
  i2c_write(i2c_address, regist, val);

  regist = 0x1D;//ANC BCK
  val = 0x01;//set as 64x
  i2c_write(i2c_address, regist, val);

  regist = 0x16;//MCU I2S port setup
  val = 0x43;//enable outputs and loopbacks
  i2c_write(i2c_address, regist, val);

  regist = 0x1E;//ANC I2S port setup
  val = 0x91;//enable outputs and loopbacks
  i2c_write(i2c_address, regist, val);
  
  //Signal path setup
  regist = 0x22;//DAC mix
  val = 0xA5;//right to right,left to left
  i2c_write(i2c_address, regist, val);

  regist = 0x39;//headphone level left
  val = 0x1A;//set amp to 0dB
  i2c_write(i2c_address, regist, val);

  regist = 0x3A;//headphone level right
  val = 0x1A;//set amp to 0dB
  i2c_write(i2c_address, regist, val);
}
