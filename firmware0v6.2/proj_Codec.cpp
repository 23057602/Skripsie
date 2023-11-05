#include "Wire.h"
//I2C variables////////////////////////////////////////////////////


//codec variables//////////////////////////////////////////////////////////////////////////////////
char i2c_address = 0x20;
volatile char i2c_flgs;

//codec registers
char codec_itr_reg = 0x00;

//I2C functions////////////////////////////////////////////////////
void itr_init(int itr);
void IRAM_ATTR codec_itr();

void codec_init(int itr, int scl, int sda){
  int i2c_clk_frq = 100000;
  Wire.begin(sda, scl, i2c_clk_frq);
  itr_init(itr);
}

char i2c_read(char addr, char reg){
  //set register
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  //read byte
  char b;
  Wire.requestFrom(addr,1);
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
}

//Codec function//////////////////////////////////////////////////////////////////////////////////
void itr_init(int itr){
  //setup interrupt line
  pinMode(itr,INPUT);
  attachInterrupt(itr, codec_itr, FALLING);
}

void IRAM_ATTR codec_itr(){
  i2c_flgs = i2c_read(i2c_address, codec_itr_reg);
}
