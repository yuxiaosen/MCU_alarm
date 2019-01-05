#include "lcd1602.h"
#ifndef 	LCD1602_4PINS	 //当没有定义这个LCD1602_4PINS时
void LCD1602_WriteCom(uchar com)	  //写入命令
{
	LCD1602_E=0;
	LCD1602_RS=0;
	LCD1602_RW=0;
	LCD1602_DATAPINS=com;
	delay_ms(1);
	LCD1602_E=1;
	delay_ms(1);
	LCD1602_E=0;
}
#else 
void LCD1602_WriteCom(uchar com)	  //写入命令
{
	LCD1602_E=0;
	LCD1602_RS=0;
	LCD1602_RW=0;
	LCD1602_DATAPINS=com;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	delay_ms(1);
	LCD1602_E=1;
	delay_ms(1);
	LCD1602_E=0;
	delay_ms(1);
	LCD1602_DATAPINS=com<<4;
	delay_ms(1);
	LCD1602_E=1;
	delay_ms(1);
	LCD1602_E=0;
}
#endif
#ifndef 	LCD1602_4PINS
void LCD1602_WriteData(uchar dat)			//写入数据
{
	LCD1602_E=0;
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_DATAPINS=dat;
	delay_ms(1);
	LCD1602_E=1;
	delay_ms(1);
	LCD1602_E=0;
}
#else
void LCD1602_WriteData(uchar dat)			//写入数据
{
	LCD1602_E=0;
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_DATAPINS=dat;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	delay_ms(1);
	LCD1602_E=1;
	delay_ms(1);
	LCD1602_E=0;
	LCD1602_DATAPINS=dat<<4;
	delay_ms(1);
	LCD1602_E=1;
	delay_ms(1);
	LCD1602_E=0;
}
#endif
#ifndef		LCD1602_4PINS
void LCD1602_Init()						     //LCD初始化子程序
{
 	LCD1602_WriteCom(0x38);  				 //开显示
	LCD1602_WriteCom(0x0c); 				 //开显示不显示光标
	LCD1602_WriteCom(0x06); 				 //写一个指针加1
	LCD1602_WriteCom(0x01); 				 //清屏
	LCD1602_WriteCom(0x80);  				 //设置数据指针起点
}
#else
void LCD1602_Init()
{
	LCD1602_WriteCom(0x32);  
    LCD1602_WriteCom(0x28);  				 //开显示
	LCD1602_WriteCom(0x0c); 				 //开显示不显示光标
	LCD1602_WriteCom(0x06); 				 //写一个指针加1
	LCD1602_WriteCom(0x01); 				 //清屏
	LCD1602_WriteCom(0x80);  				 //设置数据指针起点
}
#endif
void LCD1602_Cursor(uchar CF,uchar addr)
{
	 LCD1602_WriteCom(CF);
	 LCD1602_WriteCom(addr);
}
