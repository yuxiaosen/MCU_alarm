#ifndef __LCD1602_H
#define __LCD1602_H
#define LCD1602_4PINS
#include "delay.h"
#define LCD1602_DATAPINS P0

sbit LCD1602_E=P2^7;
sbit LCD1602_RW=P2^5;
sbit LCD1602_RS=P2^6;


void LCD1602_WriteCom(uchar com);			  //写入命令
void LCD1602_WriteData(uchar dat);			 //写入数据
void LCD1602_Init();						     //LCD初始化子程序
void LCD1602_Cursor(uchar addr,uchar Data);
#endif
