//v2 adds codec I2C
//Includes////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "BluetoothA2DPSink.h"
#include "I2SStream.h"
#include "AudioTools.h"
#include "Wire.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Global variables
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Bluetooth variables///////////////////////////////////////////////
BluetoothA2DPSink a2dp_sink;
const char*  bt_name = "IHopeIWork2";
bool auto_reconnect = false;
int16_t* bt_rx_buf;
bool bt_isPlaying = false;

//I2S variables////////////////////////////////////////////////////
I2SStream i2s_stream;
I2SConfig i2s_config;
I2SFormat i2s_format = I2S_STD_FORMAT;
int i2s_port = 0;
int i2s_channels = 2;
int i2s_sample_rate = 44100;
int i2s_bit_depth = 16;
int16_t* i2s_tx_mxDn_buf;
int16_t* i2s_rx_buf;

//i2s pins
int i2s_bclk_pin;
int i2s_LRclk_pin;
int i2s_SDin_pin;
int i2s_SDout_pin;
int i2s_mclk_pin;

//I2C variables////////////////////////////////////////////////////
int i2c_clk_frq = 100000;
int i2c_itr_pin;

//i2c pins
int i2c_scl_pin;
int i2c_sda_pin;

//codec variables//////////////////////////////////////////////////////////////////////////////////
char i2c_address = 0x20;
volitile char i2c_flgs;

//codec registers
char codec_itr_reg = 0x00;

//IO variables//////////////////////////////////////////////////////
bool isBt = true;
uint32_t buf_len;
volatile int playPauseFlg = 0;
volatile int modeFlg = 0;
//IO pin
int playPause_pin;
int mode_pin;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Bluetooth functions///////////////////////////////////////////////
void bt_init(){
  a2dp_sink.set_stream_reader(read_data_stream, false);
  a2dp_sink.set_avrc_rn_playstatus_callback(avrc_status);
}

void bt_start(){
  a2dp_sink.start(bt_name, auto_reconnect);
}

void bt_stop(){
  a2dp_sink.end();
}

void read_data_stream(const uint8_t *data, uint32_t length) {
  bt_rx_buf = (int16_t*) data;
  bt_rx_buf_len = length/2;
}

void avrc_status(esp_avrc_playback_stat_t playback){
  bt_isPlaying = (bool)(a2dp_sink.to_str(playback) == "playing");
}

//I2S function/////////////////////////////////////////////////////
void i2s_init(){
  i2s_config.I2SConfig(RXTX_MODE);
  i2s_config.i2s_format = i2s_format;
  i2s_config.port_no = i2s_port;
  i2s_config.bits_per_sample = i2s_bit_depth;
  i2s_config.sample_rate = i2s_sample_rate;
  i2s_config.channels = i2s_channels;
  i2s_config.pin_mck = i2s_mclk_pin;
  i2s_config.pin_bck = i2s_bclk_pin;
  i2s_config.pin_ws = i2s_LRclk_pin;
  i2s_config.pin_data = i2s_SDout_pin;
  i2s_config.pin_data_rx = i2s_SDin_pin;
}

void i2s_start(){
  i2s_stream.begin(i2s_config);
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

//I2C functions////////////////////////////////////////////////////
void i2c_init(){
  Wire.begin(i2c_sda_pin, i2c_scl_pin, i2c_clk_frq);
}

char i2c_read(char adddr, char reg){
  //set register
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  //read byte
  char b;
  Wire.requestFrom(addr,1);
  while(Wire.available()){
    b = Wire.read()
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
void codec_init(){
  //setup interrupt line
  pinMode(i2c_itr_pin,INPUT);
  attachInterrupt(i2c_itr_pin, codec_itr, FALLING);
}

void IRAM_ATTR codec_itr(){
  i2c_flgs = i2c_read(i2c_address, codec_itr_reg);
}

//IO functions/////////////////////////////////////////////////////
void btn_init(){
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
  if (millis() - lastCall < 50){//debounce
    playPauseFlg = 0;
    return;
    }

  //toggle play or pause
  if(isBt){//check mode
    if(bt_isPlaying){//bluetooth play/pause
      a2dp_sink.pause();
    }else{
      a2dp_sink.play();
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
  if (millis() - lastCall < 50){//debounce
    modeFlg = 0;
    return;
    }

  //toggle modes
  if(isBt){
    bt_stop();
    isBt = false;
  }else{
    bt_start();
    isBt = true;
  }
  
  //reset flags
  modeFlg = 0;
  lastCall = millis();
}

void dsp_mixDown(int16_t* tx_buff, uint32_t length){
  for(int i = 0; i < length; i++){
    *(i2s_tx_mxDn_buf[i]) = *(tx_buff[i]) + *(i2s_rx_buf[i]);
  }
}

void sendData(){
  if(isBt){
    dsp_mixDown(bt_rx_buf, buf_len);
  }
  i2s_write(i2s_tx_mxDn_buf, buf_len);
}

//Main Program///////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  bt_init();
  bt_start();
  i2s_init();
  i2s_start();
  i2c_init();
  codec_init();
  btn_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(playPauseFlg > 0){playPause();}
  if(modeFlg > 0){changeMode();}

}
