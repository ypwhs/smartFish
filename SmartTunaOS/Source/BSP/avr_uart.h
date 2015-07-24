/*
*write by wsg 2015-1-7
*/


void USART0Init(void);//´®¿Ú0³õÊ¼»¯

void USART1_Init(void);

void USART0_Transmit(unsigned char dat);

void USART0_TransmitString(unsigned char *str);

void UASRT0_RXC_ISR_Handler(void);

void USART1_Transmit(unsigned char dat);
void USART1_TransmitString(char *str);
unsigned char USART1_Receive(void);