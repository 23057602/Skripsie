#include "BluetoothA2DPSink.h"

void blu_init();
//void blu_i2s_init(int mck, int bck, int ws, int sdi,  int sdo);
void blu_start();
void blu_pause();
void blu_play();
void blu_stop();
bool blu_isPlaying();
void read_data_stream(const uint8_t *data, uint32_t length);
void avrc_status(esp_avrc_playback_stat_t playback);
void blu_sampleRate(uint16_t rate);
