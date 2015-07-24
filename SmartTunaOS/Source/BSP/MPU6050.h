#ifndef MPU
#define MPU
#include <twi_master.h>
#include <ucos_ii.h>
#define uc unsigned char 
extern uc MPUData[14];
extern void MPUInitialize();
extern void ReadMPU(uc *);
#endif