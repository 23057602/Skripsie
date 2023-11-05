#include "BluetoothA2DPSink.h"
#include <Arduino.h>
//Bluetooth variables///////////////////////////////////////////////
BluetoothA2DPSink a2dp_sink;
//int16_t* bt_rx_buf;
bool bt_playing = false;
//uint32_t buf_len;
//bool unRead = false;

void read_data_stream(const uint8_t *data, uint32_t length);
void avrc_status(esp_avrc_playback_stat_t playback);
//Bluetooth functions///////////////////////////////////////////////
void blu_init(){
  a2dp_sink.set_stream_reader(read_data_stream, false);
  a2dp_sink.set_avrc_rn_playstatus_callback(avrc_status);
}

void blu_i2s_init(int mck, int bck, int ws, int sdi,  int sdo){
  /*i2s_pin_config_t my_pin_config = {
        .mck_io_num = mck,
        .bck_io_num = bck,
        .ws_io_num = ws,
        .data_out_num = sdo,
        .data_in_num = sdi
    };
    a2dp_sink.set_pin_config(my_pin_config);

    static i2s_config_t i2s_config = {
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 44100, // updated automatically by A2DP
      .bits_per_sample = (i2s_bits_per_sample_t)32,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0, // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = true,
      .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
  };
  a2dp_sink.set_i2s_config(i2s_config);*/
}

void blu_start(){
  char snk_name[] = "IHopeIWork2";
  bool auto_reconnect = false;
  a2dp_sink.start(snk_name, auto_reconnect);
}

void blu_pause(){
  a2dp_sink.pause();
}

void blu_play(){
  a2dp_sink.play();
}

void blu_stop(){
  a2dp_sink.end();
}

bool blu_isPlaying(){
  return bt_playing;
}

/*int16_t* blu_getData(){
  unRead = false;
  return bt_rx_buf;
}

uint32_t blu_getLen(){
  return buf_len;
}
*/
void read_data_stream(const uint8_t *data, uint32_t length) {
  for(int i = 0; i < length/2; i++){
    Serial.println(*((int*)(data + 2*i)));
  }
  //unRead = true;
}
/*
bool blu_unRead(){
  return unRead;
}*/

void avrc_status(esp_avrc_playback_stat_t playback){
  bt_playing = (bool)(a2dp_sink.to_str(playback) == "playing");
}
