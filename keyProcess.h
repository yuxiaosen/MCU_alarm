#ifndef __KEYPROCESS_H
#define __KEYPROCESS_H
 
#include "sys.h"
#include "stdio.h"
#include "delay.h"
#include "key.h"
#include "ds1302.h"
#include "lcd1602.h"
 

extern uchar xdata alarm[7][5];
extern uchar repeat[7][7];
extern int Clock;
#define MODE0 		0
#define MODE1		1
#define MODE2 		2
#define MODE3		3
#define TIMESET		MODE0	
#define DATESET		MODE1
#define ALARMSET	MODE2
#define SECCON		MODE3
 
void key_process(uchar mode);
 
#endif
