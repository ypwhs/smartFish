
/*
*write by wsg 2015-1-7
*/

#include <includes.h>

#define FLK 8 	   	  	 //晶振 crystal
#define PI 3.1416        //**圆周率
#define Joint_Num  3

//extern OS_EVENT* I2CInitSem;
extern OS_EVENT* TIMER3_Ovf_Sem;

#define PWM_Period_ms 20	     //ms
//#define PWM_Duty_Middle_us 1500  //us
#define PWM_Duty_Middle_us 1500  
//中间位置本来是1500?30度为调直余量
//#define PWM_Duty_LeftLimit_us 900  //us
//#define PWM_Duty_RightLimit_us 2100  //us

#define MiddlePosition_value 1500
#define PWM_Period_value  20000


unsigned char Instant=0;
unsigned char EX_SPEED=0; 
unsigned char EX_DIRECTION = 7;

#define D_MAX 60  //Dynamic_Offset_degree_origin[0]的最大值
#define D_Y_MAX 60  //Dynamic_Offset_degree_y[0]的最大值

#define A_MAX 20 //*0.85=Amplitude_degree_tem[0]的最大值
////需要写入IIC的数据//////
/*
鱼的ID号
鱼的频率
鱼的调直数据
鱼的游动速度
鱼的滞后角
鱼的摆动幅度
转弯偏移角度
*/
/*数据存放在24C02中的地址*/
#define ADD_i 0x10//ID*1
#define ADD_f 0x20//频率*1
#define ADD_t 0x30//调直*3
#define ADD_s1 0x40//速度*16
#define ADD_s2 0x48
#define ADD_d 0x50//滞后角*3
#define ADD_1 0x60//幅度*48
#define ADD_2 0x68
#define ADD_3 0x70
#define ADD_4 0x78
#define ADD_5 0x80
#define ADD_6 0x88
#define ADD_10 0x90//转角偏移*24
#define ADD_20 0x98
#define ADD_30 0xa0

////////////end iic address/////////////////

////////////the data need writen iic----

unsigned char FISHID[1]={0};//地址ADD_i
unsigned char FISHID_init[1]={0x01};
unsigned char FISHID_I2C[3]={0,0,0};

unsigned char freqs[1]={0};//地址ADD_f,freq[freqs[x]]
unsigned char freqs_init[1]={0x01};
unsigned char freqs_I2C[3]={0,0,0};

unsigned char Static_Offset_degree[3]={60,60,60};//地址ADD_t
unsigned char Static_Offset_degree_init[3]={60,60,60};
unsigned char degree_I2C[5]={0,0,0,0,0};

//Speed_tem[16]//地址ADD_s1,s2
unsigned char Speed_tem_init[16]={0,255,240,225,210,195,180,165,150,135,120,105,90,75,60,45};
unsigned char Speed_tem_I2C[18]={0};  

unsigned char Speed_tem_init1[8]={0};
unsigned char Speed_tem_init2[8]={0};

unsigned char Speed_tem_z[16]={0};//Dynamic_Offset_degree_origin[8][3]直接转成的1维数组
unsigned char Speed_tem_z1[8]={0};//0-7
unsigned char Speed_tem_z2[8]={0};//8-16

unsigned char Speed_tem_I2C1[10]={0};
unsigned char Speed_tem_I2C2[10]={0};

//Neighbouring_Offset_degree[3]//地址ADD_d
unsigned char Neighbouring_Offset_degree_init[3]={0,80,120};
unsigned char Neighbouring_Offset_degree_I2C[5]={0};


//Amplitude_degree_tem[16][3]//地址ADD_1,2,3,4,5,6
unsigned char Amplitude_degree_tem_init[48]=
              {
                0.1*A_MAX,0.1*A_MAX+10,0.1*A_MAX+15,//0 ，速度小摆幅小
                0.15*A_MAX,0.15*A_MAX+10,0.15*A_MAX+15,//1
                0.2*A_MAX,0.2*A_MAX+10,0.2*A_MAX+15,//
                0.25*A_MAX,0.25*A_MAX+10,0.25*A_MAX+15,//3
                0.3*A_MAX,0.3*A_MAX+10,0.3*A_MAX+15,//
                0.35*A_MAX,0.35*A_MAX+10,0.35*A_MAX+15,//5
                0.4*A_MAX,0.4*A_MAX+10,0.4*A_MAX+15,//
                0.45*A_MAX,0.45*A_MAX+10,0.45*A_MAX+15,//7
                0.5*A_MAX,0.5*A_MAX+10,0.5*A_MAX+15,//8******
                0.55*A_MAX,0.55*A_MAX+10,0.55*A_MAX+15,//9
                0.6*A_MAX,0.6*A_MAX+10,0.6*A_MAX+15,//
                0.65*A_MAX,0.65*A_MAX+10,0.65*A_MAX+15,//11
                0.7*A_MAX,0.7*A_MAX+10,0.7*A_MAX+15,//
                0.75*A_MAX,0.75*A_MAX+10,0.75*A_MAX+15,//13
                0.8*A_MAX,0.8*A_MAX+10,0.8*A_MAX+15,//
                0.85*A_MAX,0.85*A_MAX+10,0.85*A_MAX+15//15，速度大摆幅大
			  };
			  //unsigned char Amplitude_degree_tem_I2C[50]={0};
unsigned char Amplitude_degree_tem_init1[8]={0};
unsigned char Amplitude_degree_tem_init2[8]={0};
unsigned char Amplitude_degree_tem_init3[8]={0};
unsigned char Amplitude_degree_tem_init4[8]={0};
unsigned char Amplitude_degree_tem_init5[8]={0};
unsigned char Amplitude_degree_tem_init6[8]={0};
unsigned char Amplitude_degree_tem_z[48]={0};//Amplitude_degree_tem[16][3]直接转成的1维数组
unsigned char Amplitude_degree_tem_z1[8]={0};//0-11
unsigned char Amplitude_degree_tem_z2[8]={0};//12-23
unsigned char Amplitude_degree_tem_z3[8]={0};//24-35
unsigned char Amplitude_degree_tem_z4[8]={0};//36-47
unsigned char Amplitude_degree_tem_z5[8]={0};//36-47
unsigned char Amplitude_degree_tem_z6[8]={0};//36-47
unsigned char Amplitude_degree_tem_I2C1[10]={0};
unsigned char Amplitude_degree_tem_I2C2[10]={0};
unsigned char Amplitude_degree_tem_I2C3[10]={0};
unsigned char Amplitude_degree_tem_I2C4[10]={0};
unsigned char Amplitude_degree_tem_I2C5[10]={0};
unsigned char Amplitude_degree_tem_I2C6[10]={0};

//Dynamic_Offset_degree_origin [8][3],地址ADD_10,20,30,40
unsigned char Dynamic_Offset_degree_origin_init[24]=
{
    D_MAX       ,D_MAX-5        ,D_MAX,         //右14，右15，左0，急//加负号是左
    0.8*D_MAX   ,0.8*D_MAX-5    ,0.9*D_MAX,     //右13，左1，q
    0.6*D_MAX   ,0.6*D_MAX-5    ,0.8*D_MAX,     //右12，左2,w
    0.45*D_MAX  ,0.45*D_MAX-5   ,0.7*D_MAX,     //右11，左3,e
    0.3*D_MAX   ,0.3*D_MAX-5    ,0.5*D_MAX,     //右10，左4.r
    0.15*D_MAX  ,0.15*D_MAX-5   ,0.3*D_MAX,     //右9，左5.t
    0.1*D_MAX   ,0.1*D_MAX-5    ,0.1*D_MAX,     //右8，左6，缓.y
    0           ,0              ,0              //中7，不转
};
unsigned char Dynamic_Offset_degree_origin_init1[8]={0};
unsigned char Dynamic_Offset_degree_origin_init2[8]={0};
unsigned char Dynamic_Offset_degree_origin_init3[8]={0};
unsigned char Dynamic_Offset_degree_origin_z[24]={0};//Dynamic_Offset_degree_origin[8][3]直接转成的1维数组
unsigned char Dynamic_Offset_degree_origin_z1[8]={0};//0-11
unsigned char Dynamic_Offset_degree_origin_z2[8]={0};//12-23
unsigned char Dynamic_Offset_degree_origin_z3[8]={0};//12-23
unsigned char Dynamic_Offset_degree_origin_I2C1[10]={0};//
unsigned char Dynamic_Offset_degree_origin_I2C2[10]={0};//
unsigned char Dynamic_Offset_degree_origin_I2C3[10]={0};//
//////////////----需要写入i2c的数据--END----//////////////////
//16档速度值,0-15,Speed值越大，速度越小
unsigned char Speed_tem[16]=  
{
    0
};      //最大255//最慢150比较合适

//16档转弯值，0-15,7不转
unsigned char Dynamic_Offset_degree_origin[8][Joint_Num]=
{
    {D_MAX,D_MAX-5,D_MAX},  //右14，右15，左0，急//加负号是左
    {0.9*D_MAX,0.9*D_MAX-5,0.9*D_MAX}, //右13，左1，q
    {0.8*D_MAX,0.8*D_MAX-5,0.8*D_MAX},  //右12，左2,w
    {0.7*D_MAX,0.7*D_MAX-5,0.7*D_MAX}, //右11，左3,e
    {0.5*D_MAX,0.5*D_MAX-5,0.5*D_MAX},  //右10，左4.r
    {0.3*D_MAX,0.3*D_MAX-5,0.3*D_MAX}, //右9，左5.t
    {0.1*D_MAX,0.1*D_MAX-5,0.1*D_MAX},  //右8，左6，缓.y
    {0,  0,  0} //中7，不转
};//常量数组，给出Dynamic_Offset_degree的值

//原始转弯方式，16档转弯值，0-15,7不转
int Dynamic_Offset_degree_y[16][Joint_Num] =
{0
};//常量数组，给出Dynamic_Offset_degree的值
int Joint_Angle_value [Joint_Num] =     //the final value to be written in duty cycle register
{
  1500, 1500, 1500
};//初始化值

int Static_Offset_value [Joint_Num] =//调直用, 单位为度
{
  0, 0, 0
};
int Dynamic_Offset_value [Joint_Num] =
{
 0, 0, 0
};//根据Dynamic_Offset_degree计算得到,存储收到的转弯命令的Dynamic_Offset_value值，也就是要达到的值
int Dynamic_Offset_degree [Joint_Num] =// modified through communication转弯用
{
 0, 0, 0           
};//用来计算Dynamic_Offset_value
unsigned char Neighbouring_Offset_degree[Joint_Num] =// modified through communication 每关节相位差
{
  0,0,0
// 0,60,120
 // 0,80,120
};
//常量，绝对相位差
unsigned char Amplitude_degree[Joint_Num] =// modified through communication 各关节摆幅
{0,0,0
 // 15,20,25
//   90,20,25
};//常量,控制摆幅，Asin(),最大90度
unsigned char Amplitude_degree_tem[16][Joint_Num] =//16档摆幅，对应速度，0-15
{0
};
//*********************sin计算***************************
double Sin(unsigned char instant, unsigned char speed, int Offset_degree)
{

	double x,y;
	double temp;
        if(speed == 0)
            return 0;
        else
        {
          	x = PI *2.0 *  ((double)instant / (double)speed) - PI * (double)Offset_degree / 180.0;       
		 	//100115 解决滞后角超过一定范围后，电机的抖动问题         
         	if(x>(2.0*PI))
            	x=x-2.0*PI;
        	else if(x<0.0)
           	 	x=x+2.0*PI;

		  	if(x<=(PI/2.0))
				y=x-x*x*x/6.0;//sinx,正数

          	if(x>(PI/2.0)&&x<=PI)
	   		{
				temp=PI-x;
				y=temp-temp*temp*temp/6.0;//正数
	   		}

         	if(x>PI&&x<=(PI*3.0/2.0))
	   		{
				temp=x-PI;
				y=temp*temp*temp/6.0-temp;//负数
	  		 }

	  		if(x>(PI*3.0/2.0))
	   		{
	        	temp=2.0*PI-x;
	        	y=temp*temp*temp/6.0-temp;//负数
	   		}
	 		return y;
        }
}

//**********************************静态偏移转换为控制量*************************
void init_static_offset(void)//括号内添加void
{
    unsigned char i;

    for(i=0; i<Joint_Num; i++)
    {
        Static_Offset_value[i] =((int) ((Static_Offset_degree[i] * 10) * FLK)>>3)-1;//1度10us
    }
}

//**********************************动态偏移转换为控制量*************************
void init_dynamic_offset(void)//括号内添加void
{
    unsigned char i;
  
    for(i=0; i<Joint_Num; i++)
    {
        Dynamic_Offset_value[i] = ((int) ((Dynamic_Offset_degree[i] * 10) * FLK)>>3)-1;
    }
}
//***********************************计算最终控制量******************************

void calculate_data(void)   //算出每次溢出中断改动的OCRnX值
{
    unsigned char i;
    if(EX_SPEED==0)
    {
        for(i=0; i< Joint_Num; i++){
            Joint_Angle_value[i] = MiddlePosition_value + Static_Offset_degree[i]*5 - 300;
        }
    }
    else if(EX_DIRECTION>=7)//右转
    {
        if(EX_DIRECTION>14)
            EX_DIRECTION=14;
        for(i=0; i< Joint_Num; i++){
            Joint_Angle_value[i] =
                  MiddlePosition_value                                  //中间位置
                + (Static_Offset_degree[i]*5) - 300                     //调直的数据
                - 10*(Dynamic_Offset_degree_origin[14-EX_DIRECTION][i]) //Dynamic_Offset_value[i]//转弯时要到达的位置
                +  ((int) (Amplitude_degree_tem[EX_SPEED][i] * Sin( Instant, Speed_tem_z[EX_SPEED], Neighbouring_Offset_degree[i]) *10))-1;
        }
    }
    else//左转
    {
        //if(EX_DIRECTION>5)
        //    EX_DIRECTION=5;
        for(i=0; i< Joint_Num; i++){
            Joint_Angle_value[i] =
                  MiddlePosition_value                                  //中间位置
                + (Static_Offset_degree[i]*5) - 300                     //调直的数据
                + 10*(Dynamic_Offset_degree_origin[EX_DIRECTION][i])
                + ((int) (Amplitude_degree_tem[EX_SPEED][i] * Sin( Instant, Speed_tem_z[EX_SPEED], Neighbouring_Offset_degree[i]) *10))-1;
        }
    }
    if(Joint_Angle_value[i] < 600)
    {
        Joint_Angle_value[i] = 600;
    }else if(Joint_Angle_value[i] > 2400){
        Joint_Angle_value[i] = 2400;
    }
}

/**
*函数描述：T/C3溢出中断
*/
void Timer3_OVF_ISR_Handler(void)
{
    if(Speed_tem_z[EX_SPEED]!=0)
    {
        Instant++;
        Instant++;
        Instant = Instant%Speed_tem_z[EX_SPEED];
    }
    OSSemPost(TIMER3_Ovf_Sem);
}
void fish_write_align_data(void)
{
   i2c_write(ADD_t,degree_I2C,Static_Offset_degree,3);
}
void fish_reset_align_data(void){
    i2c_write(ADD_t,degree_I2C,Static_Offset_degree_init,3);
    OSTimeDly(2);
    i2c_read(ADD_t,degree_I2C,Static_Offset_degree,3);
}
void fish_write_id_data(void)
{
   i2c_write(ADD_i,FISHID_I2C,FISHID,1);
}
INT8U FREQUENCY_ARRY[6] = {0X02,0X16,0X2A,0X3B,0X33,0X0C};
void fish_write_frequency(INT8U count)
{
    PORTD = PORTD&0XF4;
    INT8U send_change_frequency[5] = {0xaa,0x49,0xff,count,0xbb};
    for(INT8U i = 0;i<5;i++)
    {
        USART0_Transmit(send_change_frequency[i]);
    }
}
void fish_write_parameter(void)
{
    one2two(Speed_tem_z,Speed_tem_init1,Speed_tem_init2);
    i2c_write(ADD_s1,Speed_tem_I2C1,Speed_tem_init1,8);
    OSTimeDly(2);			  //delay_nms(200);
    i2c_write(ADD_s2,Speed_tem_I2C2,Speed_tem_init2,8);
    OSTimeDly(2);
    for(INT8U i=0;i<=15;i++)
    {
        USART0_Transmit(Speed_tem_z[i]);
    }
    i2c_write(ADD_t,degree_I2C,Static_Offset_degree_init,3);
    OSTimeDly(2);
    i2c_write(ADD_d,Neighbouring_Offset_degree_I2C,Neighbouring_Offset_degree,3);	
    for(INT8U i=0;i<3;i++)
    {
       USART0_Transmit(Neighbouring_Offset_degree[i]);
    }
    for(INT8U i=0;i<=47;i++)
    {
            Amplitude_degree_tem_z[i] = Amplitude_degree_tem[i/3][i%3];
    }
    for(INT8U i=0;i<=47;i++)
    {
       USART0_Transmit(Amplitude_degree_tem_z[i]);
    }
    one2six(Amplitude_degree_tem_z,Amplitude_degree_tem_init1,Amplitude_degree_tem_init2,Amplitude_degree_tem_init3,Amplitude_degree_tem_init4,Amplitude_degree_tem_init5,Amplitude_degree_tem_init6);
    OSTimeDly(2);
    i2c_write(ADD_1,Amplitude_degree_tem_I2C1,Amplitude_degree_tem_init1,8);//+++++++
    OSTimeDly(2);			 
    i2c_write(ADD_2,Amplitude_degree_tem_I2C2,Amplitude_degree_tem_init2,8);//+++++++
    OSTimeDly(2);
    i2c_write(ADD_3,Amplitude_degree_tem_I2C3,Amplitude_degree_tem_init3,8);//+++++++
    OSTimeDly(2);
    i2c_write(ADD_4,Amplitude_degree_tem_I2C4,Amplitude_degree_tem_init4,8);
    OSTimeDly(2);
    i2c_write(ADD_5,Amplitude_degree_tem_I2C5,Amplitude_degree_tem_init5,8);
    OSTimeDly(2);
    i2c_write(ADD_6,Amplitude_degree_tem_I2C5,Amplitude_degree_tem_init6,8);
    for(INT8U i=0;i<=23;i++)
    {
       Dynamic_Offset_degree_origin_z[i]=Dynamic_Offset_degree_origin[i/3][i%3];
    }	
     for(INT8U i=0;i<=23;i++)
    {
       USART0_Transmit(Dynamic_Offset_degree_origin_z[i]);
    }
    one2three(Dynamic_Offset_degree_origin_z,Dynamic_Offset_degree_origin_init1,Dynamic_Offset_degree_origin_init2,Dynamic_Offset_degree_origin_init3);
    OSTimeDly(2);
    i2c_write(ADD_10,Dynamic_Offset_degree_origin_I2C1,Dynamic_Offset_degree_origin_init1,8);//+++++++
    OSTimeDly(2);
    i2c_write(ADD_20,Dynamic_Offset_degree_origin_I2C2,Dynamic_Offset_degree_origin_init2,8);//+++++++
    OSTimeDly(2);		 // delay_nms(200);
    i2c_write(ADD_30,Dynamic_Offset_degree_origin_I2C3,Dynamic_Offset_degree_origin_init3,8);	
}
void system_read_i2c_data(void)
{
    OSTimeDly(2);
    i2c_read(ADD_i,FISHID_I2C,FISHID,1);
    //USART0_TransmitString("fishid:");b
    USART0_Transmit(FISHID[0]);
    USART0_Transmit('\n');
    // ;
    OSTimeDly(2);
    i2c_read(ADD_f,freqs_I2C,freqs,1);
    // USART0_TransmitString("freqs:");
    USART0_Transmit(freqs[0]);
    USART0_Transmit('\n');
    OSTimeDly(2);
    
	i2c_read(ADD_t,degree_I2C,Static_Offset_degree,3);
	for(INT8U i=0;i<3;i++)
	{
            USART0_Transmit(Static_Offset_degree[i]);
	}
	USART0_Transmit('\n');
	OSTimeDly(2);
	i2c_read(ADD_s1,Speed_tem_I2C1,Speed_tem_z1,8);
	OSTimeDly(2);
	i2c_read(ADD_s2,Speed_tem_I2C2,Speed_tem_z2,8);
	OSTimeDly(2);
	two2one(Speed_tem_z,Speed_tem_z1,Speed_tem_z2);
	for(INT8U i=0;i<=15;i++)
	{
        Speed_tem[i]=Speed_tem_z[i];
	}
	
	for(INT8U i=0;i<=15;i++)
	{
	 USART0_Transmit(Speed_tem_z[i]);
	}
	USART0_Transmit('\n');
	i2c_read(ADD_d,Neighbouring_Offset_degree_I2C,Neighbouring_Offset_degree,3);
	i2c_read(ADD_1,Amplitude_degree_tem_I2C1,Amplitude_degree_tem_z1,8);
	i2c_read(ADD_2,Amplitude_degree_tem_I2C2,Amplitude_degree_tem_z2,8);
	i2c_read(ADD_3,Amplitude_degree_tem_I2C3,Amplitude_degree_tem_z3,8);
	i2c_read(ADD_4,Amplitude_degree_tem_I2C4,Amplitude_degree_tem_z4,8);
	i2c_read(ADD_5,Amplitude_degree_tem_I2C5,Amplitude_degree_tem_z5,8);
	i2c_read(ADD_6,Amplitude_degree_tem_I2C6,Amplitude_degree_tem_z6,8);
	six2one(Amplitude_degree_tem_z,Amplitude_degree_tem_z1,Amplitude_degree_tem_z2,Amplitude_degree_tem_z3,Amplitude_degree_tem_z4,Amplitude_degree_tem_z5,Amplitude_degree_tem_z6);  
	for(INT8U i=0;i<=47;i++)
	{
	   	Amplitude_degree_tem[i/3][i%3]=Amplitude_degree_tem_z[i];
	 }	
	for(INT8U i=0;i<=47;i++)
	{
	   USART0_Transmit(Amplitude_degree_tem_z[i]);
	}
	USART0_Transmit('\n');
        i2c_read(ADD_10,Dynamic_Offset_degree_origin_I2C1,Dynamic_Offset_degree_origin_z1,8);
        i2c_read(ADD_20,Dynamic_Offset_degree_origin_I2C2,Dynamic_Offset_degree_origin_z2,8);
        i2c_read(ADD_30,Dynamic_Offset_degree_origin_I2C3,Dynamic_Offset_degree_origin_z3,8);
        three2one(Dynamic_Offset_degree_origin_z,Dynamic_Offset_degree_origin_z1,Dynamic_Offset_degree_origin_z2,Dynamic_Offset_degree_origin_z3);  
        for(INT8U i=0;i<=23;i++)
        {
            //Dynamic_Offset_degree_origin[i/3][i%3]=Dynamic_Offset_degree_origin_init[i];
        }	
        for(INT8U i=0;i<=23;i++)
	{
            USART0_Transmit(Dynamic_Offset_degree_origin_z[i]);
	}
	USART0_Transmit('\n');
        OSTimeDly(2);
}

void system_write_i2c_data(void)
{
  	OSTimeDly(2);
        //ID初始化
  	i2c_write(ADD_i,FISHID_I2C,FISHID_init,1); 
  	OSTimeDly(2);
        //频率初始化
  	i2c_write(ADD_f,freqs_I2C,freqs_init,1);
  	OSTimeDly(2);
        //偏移量初始化
  	i2c_write(ADD_t,degree_I2C,Static_Offset_degree_init,3);
 	OSTimeDly(2);
  	one2two(Speed_tem_init,Speed_tem_init1,Speed_tem_init2);
        //SpeedTem1初始化
        i2c_write(ADD_s1,Speed_tem_I2C1,Speed_tem_init1,8);
 	OSTimeDly(2);
        //SpeedTem2初始化
	i2c_write(ADD_s2,Speed_tem_I2C2,Speed_tem_init2,8);
	OSTimeDly(2);
	i2c_write(ADD_d,Neighbouring_Offset_degree_I2C,Neighbouring_Offset_degree_init,3);	
	one2six(Amplitude_degree_tem_init,Amplitude_degree_tem_init1,Amplitude_degree_tem_init2,Amplitude_degree_tem_init3,Amplitude_degree_tem_init4,Amplitude_degree_tem_init5,Amplitude_degree_tem_init6);
	OSTimeDly(2);
	i2c_write(ADD_1,Amplitude_degree_tem_I2C1,Amplitude_degree_tem_init1,8);//+++++++
	OSTimeDly(2);			 // delay_nms(200);
        i2c_write(ADD_2,Amplitude_degree_tem_I2C2,Amplitude_degree_tem_init2,8);//+++++++
	OSTimeDly(2);
	i2c_write(ADD_3,Amplitude_degree_tem_I2C3,Amplitude_degree_tem_init3,8);//+++++++
				 // delay_nms(200);
	OSTimeDly(2);
 	i2c_write(ADD_4,Amplitude_degree_tem_I2C4,Amplitude_degree_tem_init4,8);
	OSTimeDly(2);
	i2c_write(ADD_5,Amplitude_degree_tem_I2C5,Amplitude_degree_tem_init5,8);
	OSTimeDly(2);
	i2c_write(ADD_6,Amplitude_degree_tem_I2C5,Amplitude_degree_tem_init6,8);
	one2three(Dynamic_Offset_degree_origin_init,Dynamic_Offset_degree_origin_init1,Dynamic_Offset_degree_origin_init2,Dynamic_Offset_degree_origin_init3);
	OSTimeDly(2);
	i2c_write(ADD_10,Dynamic_Offset_degree_origin_I2C1,Dynamic_Offset_degree_origin_init1,8);//+++++++
	OSTimeDly(2);
	i2c_write(ADD_20,Dynamic_Offset_degree_origin_I2C2,Dynamic_Offset_degree_origin_init2,8);//+++++++
	OSTimeDly(2);		 // delay_nms(200);
	i2c_write(ADD_30,Dynamic_Offset_degree_origin_I2C3,Dynamic_Offset_degree_origin_init3,8);
        OSTimeDly(2);
}

