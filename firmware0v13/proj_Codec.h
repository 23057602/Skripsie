void codec_init(int itr, int scl, int sda);
char i2c_read(char addr, char reg);
void i2c_write(char addr, char reg, char b);
void itr_init(int itr);
void IRAM_ATTR codec_itr();
char codec_heartbeat();
void codec_setup();
