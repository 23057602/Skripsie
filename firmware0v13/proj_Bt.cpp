#include "BluetoothA2DPSink.h"
//Bluetooth variables///////////////////////////////////////////////
BluetoothA2DPSink a2dp_sink;
bool bt_playing = false;

void avrc_status(esp_avrc_playback_stat_t playback);
void blu_sampleRate(uint16_t rate);
//Bluetooth functions///////////////////////////////////////////////
void blu_init(int mck, int bck, int ws, int sdi,  int sdo){
  i2s_pin_config_t my_pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = bck,
        .ws_io_num = ws,
        .data_out_num = sdo,
        .data_in_num = sdi
    };
  a2dp_sink.set_pin_config(my_pin_config);
  ledcAttachPin(mck, 0);
  ledcSetup(0, 256*44100, 2);
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
  a2dp_sink.set_i2s_config(i2s_config);
  a2dp_sink.set_avrc_rn_playstatus_callback(avrc_status);
  a2dp_sink.set_sample_rate_callback(blu_sampleRate);
}

void blu_start(){
  char snk_name[] = "IHopeIWork2";
  bool auto_reconnect = false;
  ledcWrite(0, 2);
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

void avrc_status(esp_avrc_playback_stat_t playback){
  bt_playing = (bool)(a2dp_sink.to_str(playback) == "playing");
}

void blu_sampleRate(uint16_t rate){
  ledcSetup(0, 256*rate, 2);
}
