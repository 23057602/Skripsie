void i2s_init(int mck, int bck, int ws, int sdi,  int sdo);
void i2s_start();
void i2s_stop();
uint32_t i2s_read(int16_t* rx_buff, uint32_t length);
void i2s_write(int16_t* tx_buff, uint32_t length);
void i2s_setSampleRate(uint16_t s);
