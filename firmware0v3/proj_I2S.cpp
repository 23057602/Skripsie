#include "AudioTools.h"
//#include "I2SStream.h"
//I2S variables////////////////////////////////////////////////////
I2SStream i2s_stream;
auto i2s_cfg = i2s_stream.defaultConfig(RXTX_MODE);

//I2S function/////////////////////////////////////////////////////
void i2s_init(int mck, int bck, int ws, int sdi,  int sdo){
  I2SFormat i2s_format = I2S_STD_FORMAT;
  int i2s_port = 0;
  int i2s_channels = 2;
  int sample_rate = 44100;
  int bit_depth = 16;
  
  i2s_cfg.i2s_format = i2s_format;
  i2s_cfg.port_no = i2s_port;
  i2s_cfg.bits_per_sample = bit_depth;
  i2s_cfg.sample_rate = sample_rate;
  i2s_cfg.channels = i2s_channels;
  i2s_cfg.pin_mck = mck;
  i2s_cfg.pin_bck = bck;
  i2s_cfg.pin_ws = ws;
  i2s_cfg.pin_data = sdo;
  i2s_cfg.pin_data_rx = sdi;
}

void i2s_start(){
  i2s_stream.begin(i2s_cfg);
}

void i2s_stop(){
  i2s_stream.end();
}

void i2s_read(int16_t* rx_buff, uint32_t length){
  i2s_stream.readBytes((uint8_t*) rx_buff, length*2);
}

void i2s_write(int16_t* tx_buff, uint32_t length){
  i2s_stream.write((uint8_t*) tx_buff, length*2);
}
