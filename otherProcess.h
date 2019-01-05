#ifndef __OTHERPROCESS_H
#define __OTHERPROCESS_H
#include "sys.h"
#include "lcd1602.h"
extern bit hour12_24;
extern bit AM_PM;
extern bit alarm_AM_PM;
void LCD1602_alarm(uchar addr,uchar hour,uchar min);
void LCD1602_Data(uchar addr,uchar dat);
void LCD1602_DisplayData();
#endif

