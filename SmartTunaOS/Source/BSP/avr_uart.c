/*
* write by wsg 2015-1-7
* 
*/
#include <includes.h>

extern OS_EVENT* FishUartDataSem;

void USART0Init(void)
{
	DDRE &= ~(1 << 0);
	DDRE |= (1 << 1);
	PORTE |= (1 << 0);

	UCSR0B = 0x00;                      //设置前要清零
	UCSR0A = 0x00;                      // 没有校验位
	UCSR0C = 0x06;//(1<<2) | (1<<1);//(1<<UCSZ01) | (1<<UCSZ00); //0x06设定数据位为8位
	UBRR0L = 0x33; //set baud rate lo
	UBRR0H = 0x00; //set baud rate hi  / //UBRR=5f 而外部晶振是14.7456MHZ 故波特率由表格知是9600b/s
	UCSR0B = 0x98;//(1<<7)| (1<<4) | (1<<3);//(1<<RXEN0)| (1<<TXEN0) | (1<<RXCIE0);
}
/************************************************************************/
/* 函数名称：
 * 函数功能：初始化串口1                                                 */
/************************************************************************/
void USART1_Init(void)
{
	DDRD &= ~(1 << 2);  //RXD1 = PD2
	DDRD |= (1 << 3);    //TXD1 = PD3
	PORTD |= (1 << 2);  //PD2 RXD1 上拉

	UCSR1B = 0x00;                      //设置前要清零
	UCSR1A = 0x00;                        // 没有校验位
	UCSR1C = 0x06;//(1<<2) | (1<<1);//(1<<UCSZ11) | (1<<UCSZ10); //0x06设定数据位为8位
	UBRR1L = 0x33; //set baud rate lo 波特率9600 外部晶振14.7456
	UBRR1H = 0x00; //set baud rate hi
	UCSR1B = 0x98;//(1<<7)| (1<<4) | (1<<3);//(1<<RXEN1)| (1<<TXEN1) | (1<<RXCIE1);
}
/*
 * 函数名称：
 * 函数功能：串口0向PC发送一个字符
 * 输入：一个字符 dat
 * 输出：无
 */
void USART0_Transmit(unsigned char dat)
{
	// while(!)
	while(!(UCSR0A&(1<<5)));//(!(UCSR0A&(1<<UDRE0)));
	UDR0=dat;
}
/*
 * 函数名称：void USART0TransmitString(unsigned char *str)
 * 函数功能：串口0向PC发送一个字符串
 * 输入：一个字符 dat
 * 输出：无
 */
void USART0_TransmitString(unsigned char *str)
{
	while(*str)
	{
		USART0_Transmit(*str);
		str++;
	}
	//USART0Transmit(0x0a);
	//USART0Transmit(0x0d);//回车换行
}
/************************************************************************/
/*函数名称;
 *函数功能：串口0接收一个数据 
 *                                                                     */
/************************************************************************/
unsigned char USART0_Receive(void)
{
	while(!(UCSR0A&(1<<7)));//(1<<RXC0)
            return UDR0;
}
/*
 * 函数名称：
 * 函数功能：串口1向PC发送一个字符
 * 输入：一个字符 dat
 * 输出：无
 */
void USART1_Transmit(unsigned char dat)
{
	// while(!)
	while(!(UCSR1A&(1<<5)));//(1<<UDRE1)
            UDR1=dat;
}
/*
 * 函数名称：
 * 函数功能：串口1向PC发送一个字符串
 * 输入：一个字符 dat
 * 输出：无
 */
void USART1_TransmitString(char *str)
{
	while(*str)
	{
		USART1_Transmit(*str);
		str++;
	}
}
/************************************************************************/
/* 串口1接收一个字符
 *                                                                      */
/************************************************************************/
unsigned char USART1_Receive(void)
{
    while(!(UCSR1A&(1<<7)));//(1<<RXC1)
        return UDR1;
}
/*
 *串口接收中断函数
 */
/*****************************************************************/
INT8U UART0_FLAG = 0;
/***************************
*UART0_ARRY[0] :0XAA start bit
*UART0_ARRY[1] :id
*UART0_ARRY[2] cmd
*UART0_ARRY[3]:0xfc end bit
****************************/
INT8U UART0_ARRY[4] = {0};
INT8U UART0_ARRY_Count = 0;
void UASRT0_RXC_ISR_Handler(void)
{
    unsigned char myStr;
    myStr = UDR0;
	
    if(!UART0_FLAG)
    {
      if(myStr == 0xaa)
      {
        UART0_FLAG = 1;
        UART0_ARRY_Count = 0;
      }
    }
    if(UART0_FLAG)
    {
        UART0_ARRY[UART0_ARRY_Count++]=myStr;
        if(myStr == 0xfc)
        {
            UART0_FLAG = 0;
            if(UART0_ARRY_Count==4)
                OSSemPost(FishUartDataSem);//发布信号量
            UART0_ARRY_Count = 0;
            
        }
    }
}

 