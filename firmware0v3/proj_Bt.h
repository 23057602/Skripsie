#include "BluetoothA2DPSink.h"

void blu_init();
void blu_start();
void blu_pause();
void blu_play();
bool blu_isPlaying();
int16_t* blu_getData();
uint32_t blu_getLen();
void blu_stop();
void read_data_stream(const uint8_t *data, uint32_t length);
bool blu_unRead();
void avrc_status(esp_avrc_playback_stat_t playback);
