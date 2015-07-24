#include "MPU6050.h"
#define MPUAddress 0xD0

uc WriteBuffer[32];
uc ReadBuffer[32];
uc MPUData[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void TWIWriteData(uc SLA,uc Address,uc *WriteData,uc WriteByteCount)
{
    int i=0;
    WriteBuffer[0]=SLA;
    WriteBuffer[1]=Address;
    for(i=0;i<WriteByteCount;i++)
    {
        WriteBuffer[i+2]=WriteData[i];
    }
    TWI_Start_Transceiver_With_Data(WriteBuffer,WriteByteCount+2);
}

void TWIReadData(uc SLA,uc Address,uc *ReadData,uc ReadByteCount)
{
    int i=0;
    ReadBuffer[0]=SLA;
    ReadBuffer[1]=Address;
    TWI_Start_Transceiver_With_Data(ReadBuffer,2);
    ReadBuffer[0]=SLA|0x01;
    TWI_Start_Transceiver_With_Data(ReadBuffer,ReadByteCount+1);
    TWI_Get_Data_From_Transceiver(ReadBuffer,ReadByteCount+1);
    for(i=0;i<ReadByteCount;i++)
    {
        ReadData[i]=ReadBuffer[i+1];
    }		
}

void MPUInitialize()
{
    uc MPUBuffer;
    MPUBuffer=0x80;
    TWIWriteData(0xd0,0x6B,&MPUBuffer,1);
    OSTimeDly(200);
    MPUBuffer=0x10;
    TWIWriteData(0xd0,0x6A,&MPUBuffer,1);
    OSTimeDly(200);
    MPUBuffer=0x03;
    TWIWriteData(0xd0,0x6B,&MPUBuffer,1);
    OSTimeDly(200);
    MPUBuffer=0x01;
    TWIWriteData(0xd0,0x19,&MPUBuffer,1);		//采样率500HZ
    OSTimeDly(200);
    MPUBuffer=0x05;
    TWIWriteData(0xd0,0x1A,&MPUBuffer,1);		//低通滤波10HZ
    OSTimeDly(200);
    MPUBuffer=0x00;
    TWIWriteData(0xd0,0x1B,&MPUBuffer,1);		//量程250度
    OSTimeDly(200);
    MPUBuffer=0x00;
    TWIWriteData(0xd0,0x1C,&MPUBuffer,1);		//量程8g
    OSTimeDly(200);
    MPUBuffer=0x01;
    TWIWriteData(0xd0,0x38,&MPUBuffer,1);		//
    OSTimeDly(200);
    MPUBuffer=0x10;
    TWIWriteData(0xd0,0x37,&MPUBuffer,1);		//
}
void ReadMPU(uc *MPUData)
{
    TWIReadData(0xd0,0x3B,MPUData,14);		//
}