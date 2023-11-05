#include <I2S.h>
//I2S variables////////////////////////////////////////////////////


//I2S function/////////////////////////////////////////////////////
void i2s_init(int mck, int bck, int ws, int sdi,  int sdo){
  I2S.setDuplex();
  I2S.setAllPins(bck, ws, sdo, sdo, sdi);
  ledcAttachPin(mck, 0);
  
}

void i2s_setSampleRate(uint16_t s){//dynamically change sample rate 
  ledcSetup(0, 256*s, 2);
}

void i2s_start(){
  int sample_rate = 44100;// starts mck
  i2s_setSampleRate(sample_rate);
  ledcWrite(0, 2);
  
  i2s_mode_t mode = I2S_PHILIPS_MODE;//starts i2s slave
  byte bit_depth = 16;
  I2S.begin(mode, sample_rate, bit_depth);
}

void i2s_stop(){
  I2S.end();
}

uint32_t i2s_read(int16_t* rx_buff, uint32_t length){
  uint32_t len = I2S.available();//check available bytes
  uint32_t red;
  if(len < 2*length){ //get max available or allocated
    red = I2S.read((uint8_t*) rx_buff, len);
  }else{
    red = I2S.read((uint8_t*) rx_buff, 2*length);
  }
  return red/2;//return samples read, must be even
}

void i2s_write(int16_t* tx_buff, uint32_t length){
  int i = 0;
  while (i < length){
    for(int j = 0; j < I2S.availableForWrite(); j++){
      if(i < length){
        I2S.write(*(tx_buff + i));//send data
        i++;
      }else{
        return;
      }
    }
  }
}
