/*
*********************************************************************************************************
* File : lz_fish.C
* By   : wangsg
*********************************************************************************************************
*/
#include <includes.h>

extern OS_EVENT* I2CInitSem;

#define SLA_W 0xA0
#define SLA_R 0xA1
/********是否已经初始化********/
#define IsWritenAdd 0xb0
#define ISINIT 0X02
INT8U IsWritenData_Init[1]={ISINIT};//是否已经初始化数据
INT8U IsWritenData_I2C[3] = {0,0,0};
INT8U IsWritenData[1]={0};

//INT8U SAFTYCODE[6]="123456";
//#define SaftyCodeAdd 0xc0
//INT8U IsWritenData_I2C[8] = {0};
/*
*函数描述：硬件初始化
*/
void Init_Devices(void)
{
    //TIMSK = 0x04;//T/C1 TOIE1溢出中断
    ETIMSK = 0X04;              //T/C3 TOIE3溢出中断
    ICR1 = 20000; 
    ICR3 = 20000; 
    TCNT3 = 0;
    DDRB |= (1<<5)|(1<<6);      //初始第一、第二  
    TCCR1A = 0XFE;              //(1<<COM1A1)|(1<<COM1A0)|(1<<COM1B1)|1<<COM1B0)|(1<<COM1C1)|1<<COM1C0)|(1<<WGM11);
    TCCR1B = 0X1A;              //(1<<WGM13)|(1<<WGM13)|(1<<CS11);
    DDRE |= (1<<3);             //初始化第三、第四关节（第四关节备用）
    TCCR3A = 0XFE;              //(1<<COM3A1)|(1<<COM3A0)|(1<<COM3B1)|1<<COM3B0)|(1<<COM3C1)|1<<COM3C0)|(1<<WGM31);
    TCCR3B = 0X1A;              //(1<<WGM33)|(1<<WGM33)|(1<<CS31);
    PORTD=0xfc;                 //0xfc;//xxxx xx00
    DDRD=0xfc;                  //xxxx xx00
}

//********************************读写i2c****************************************
//写i2c
//将长度为data_l的data_tem数组写入地址为i2c_address的data_i2c数组
//unsigend char型
void i2c_write(unsigned char i2c_address,unsigned char *data_i2c,unsigned char *data_tem,unsigned char data_l)
{
    int i=0;
    data_i2c[0]=SLA_W;
    data_i2c[1]=i2c_address;
    for(i=0;i<data_l;i++)
    {
        data_i2c[i+2]=data_tem[i];
    }
    TWI_Start_Transceiver_With_Data(data_i2c,data_l+2);
}
//读i2c
//将长度为data_l的地址为i2c_address的data_i2c数组读出到data_tem
//unsigned char型
void i2c_read(unsigned char i2c_address,unsigned char *data_i2c,unsigned char *data_tem,unsigned char data_l)
{
    int i=0;
    data_i2c[0]=SLA_W;
    data_i2c[1]=i2c_address;
    TWI_Start_Transceiver_With_Data(data_i2c,2);
    data_i2c[0]=SLA_R;
    TWI_Start_Transceiver_With_Data(data_i2c,data_l+1);
    TWI_Get_Data_From_Transceiver(data_i2c,data_l+1);
    for(i=0;i<data_l;i++)
    {
        data_tem[i]=data_i2c[i+1];
    }				    	
}
/*
*函数描述：初始化机器鱼的数据
*/
void Init_RobotFish_Data(void)
{
    i2c_read(IsWritenAdd,IsWritenData_I2C,IsWritenData,1);//读取是否初始化过数据
    if(IsWritenData[0]==ISINIT)
    {
        system_read_i2c_data();
        USART0_Transmit(0xff);
    }
    else
    {
        i2c_write(IsWritenAdd,IsWritenData_I2C,IsWritenData_Init,1);
        system_write_i2c_data();
        system_read_i2c_data();
    }
}















