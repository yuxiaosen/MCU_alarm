#ifndef __DS1302_H_
#define __DS1302_H_

#include "reg51.h"
#include "intrins.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif


sbit DSIO=P3^4;
sbit DSCE=P3^5;
sbit SCLK=P3^6;
 
 void DS1302_lock();	 	// 开启写保护
 void DS1302_unlock(); 		//取消写保护
 void DS1302Write(uchar addr,uchar dat);
 uchar DS1302Read(uchar addr);
 void DS1302_Init();
 void DS1302_ReadTime();
 extern uchar TIME[7];	//加入全局变量
 void LCD1602_DisplayData();
#endif
