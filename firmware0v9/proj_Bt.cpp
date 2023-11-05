#include "BluetoothA2DPSink.h"
#include "proj_I2S.h"
//Bluetooth variables///////////////////////////////////////////////
BluetoothA2DPSink a2dp_sink;
bool bt_playing = false;

void read_data_stream(const uint8_t *data, uint32_t length);
void avrc_status(esp_avrc_playback_stat_t playback);
void blu_sampleRate(uint16_t rate);
//Bluetooth functions///////////////////////////////////////////////
void blu_init(){
  a2dp_sink.set_stream_reader(read_data_stream, false);
  a2dp_sink.set_avrc_rn_playstatus_callback(avrc_status);
  a2dp_sink.set_sample_rate_callback(blu_sampleRate);
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

void read_data_stream(const uint8_t *data, uint32_t length) {
  i2s_write((int16_t*)data, length/2);
}

void avrc_status(esp_avrc_playback_stat_t playback){
  bt_playing = (bool)(a2dp_sink.to_str(playback) == "playing");
}

void blu_sampleRate(uint16_t rate){
  i2s_setSampleRate(rate);
}
