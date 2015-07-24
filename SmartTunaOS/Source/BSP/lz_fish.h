/******************************************************************************
* File : lz_fish.H
* By   : wangsg
********************************************************************************
*/
void Init_Devices(void);
void Init_RobotFish_Data(void);
void i2c_write(unsigned char i2c_address,unsigned char *data_i2c,unsigned char *data_tem,unsigned char data_l);
void i2c_read(unsigned char i2c_address,unsigned char *data_i2c,unsigned char *data_tem,unsigned char data_l);