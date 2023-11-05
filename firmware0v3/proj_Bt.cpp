#include "BluetoothA2DPSink.h"
//Bluetooth variables///////////////////////////////////////////////
BluetoothA2DPSink a2dp_sink;
int16_t* bt_rx_buf;
bool bt_playing = false;
uint32_t buf_len;
bool unRead = false;

void read_data_stream(const uint8_t *data, uint32_t length);
void avrc_status(esp_avrc_playback_stat_t playback);
//Bluetooth functions///////////////////////////////////////////////
void blu_init(){
  a2dp_sink.set_stream_reader(read_data_stream, false);
  a2dp_sink.set_avrc_rn_playstatus_callback(avrc_status);
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

int16_t* blu_getData(){
  unRead = false;
  return bt_rx_buf;
}

uint32_t blu_getLen(){
  return buf_len;
}

void read_data_stream(const uint8_t *data, uint32_t length) {
  bt_rx_buf = (int16_t*) data;
  buf_len = length/2;
  unRead = true;
}

bool blu_unRead(){
  return unRead;
}

void avrc_status(esp_avrc_playback_stat_t playback){
  bt_playing = (bool)(a2dp_sink.to_str(playback) == "playing");
}
