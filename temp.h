#ifndef __TEMP_H_
#define __TEMP_H_
#include "reg51.h"
#include "delay.h"
#include"intrins.h"
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif



sbit WDIO=P3^7;
uchar DS18B20_Init();
void DS18B20_WriteByte(uchar dat);
uchar DS18B20_ReadByte();
void DS18B20_ChangeTemp();
void DS18B20_ReadTempCom();
int DS18B20_ReadTemp();
void LCD1602_DisplayTemp(int temp);
#endif



