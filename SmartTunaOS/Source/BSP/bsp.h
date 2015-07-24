/*
*********************************************************************************************************
*                                           Atmel ATmega128
*                                         Board Support Package
*
*                                (c) Copyright 2005, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
* File : BSP.H
* By   : Jean J. Labrosse
* Updata: wangsg
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  CPU_CLK_FREQ                  8000000L

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
enum P{
A =0,B,C,D,E,F,G};
void  BSP_Init(void);

void  BSP_TickISR(void);
void  BSP_TickISR_Handler(void);

void  LED_On(INT8U led);
void  LED_Off(INT8U led);
void  LED_Toggle(INT8U led);

void IO_Out_Bit(unsigned char ddr,unsigned char bit,unsigned char num);
unsigned char IO_In_Bit(unsigned char ddr,unsigned char bit);
void IO_Toggle(unsigned char ddr,unsigned char bit);
