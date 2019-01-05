#include "ds1302.h"
#include "lcd1602.h"

//DS1302写入和读取时分秒的地址命令
uchar code READ_RTC_ADDR[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d}; //秒分时日月周年 最低位读写位};
uchar code WRITE_RTC_ADDR[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
//DS1302时钟初始化2013年1月1日星期二12点00分00秒。存储顺序是秒分时日月周年,存储格式是用BCD码
uchar TIME[7]={0,0x00,0x20,0x07,0x12,0x05,0x18};
//uchar code T[]={"T "};
//uchar code D[]={"D "};
void DS1302Write(uchar addr,uchar dat)
{
	uchar i;
	DSCE=0;
	_nop_();
	SCLK=0;
	_nop_();
	DSCE=1;
	_nop_();
	for(i=0;i<8;i++)
	{
		DSIO=addr&0x01;
		addr>>=1;
		SCLK=1;
		_nop_();
		SCLK=0;
		_nop_();
	}
	for(i=0;i<8;i++)
	{
		DSIO=dat&0x01;
		dat>>=1;
		SCLK=1;
		_nop_();
		SCLK=0;
		_nop_();
	}
	DSCE=0;
	_nop_();
}
uchar DS1302Read(uchar addr)
{
	uchar i,dat,dat1;
	DSCE=0;
	_nop_();
	SCLK=0;
	_nop_();
	DSCE=1;
	_nop_();
	for(i=0;i<8;i++)
	{
		DSIO=addr&0x01;
		addr>>=1;
		SCLK=1;
		_nop_();
		SCLK=0;
		_nop_();
	}
	_nop_();
	for(i=0;i<8;i++)
	{
		dat1=DSIO;		
		dat=(dat>>1)|(dat1<<7);
		SCLK=1;
		_nop_();
		SCLK=0;
		_nop_();
	}
//	DSCE=0;
	_nop_();
	SCLK=1;
	DSIO=0;
	_nop_();
	DSIO=1;
	_nop_();
	return dat;
}

 void DS1302_lock()	 	// 开启写保护
 {
	DS1302Write(0x8E,0x80);
 }
 void DS1302_unlock() 		//取消写保护
 {
 	DS1302Write(0x8E,0X00);	
 }
void DS1302_Init()
{
	uchar n;
	DS1302_unlock();		 //禁止写保护，就是关闭写保护功能
	for(n=0;n<7;n++)//写入7个字节的时钟信号：分秒时日月周年
	{
//		DS1302Write(0x84,TIME[2]|0x80);
		DS1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	DS1302_lock();		 //打开写保护功能
}
void DS1302_ReadTime()
{
	uchar n;
	for(n=0;n<7;n++)//读取7个字节的时钟信号：分秒时日月周年
	{
//		TIME[2]=DS1302Read(0x85);
//		TIME[2]<<=3;
//		TIME[2]>>=3;//前三位置0
		TIME[n]=DS1302Read(READ_RTC_ADDR[n]);
	}
		
}
//void LCD1602_DisplayData()
//{
////	uchar i;
//	LCD1602_WriteCom(0x80+0X40);
////	for(i=0;i<2;i++)
////	LCD1602_WriteData(T[i]);
//	LCD1602_WriteData('T');
//	LCD1602_WriteData(' ');
//	if(hour12_24==0)
//	{
//	LCD1602_WriteData('0'+TIME[2]/16);				//时
//	LCD1602_WriteData('0'+(TIME[2]&0x0f));			
//	}	 
//	LCD1602_WriteData(':');
//	LCD1602_WriteData('0'+TIME[1]/16);				//分
//	LCD1602_WriteData('0'+(TIME[1]&0x0f));	
//	LCD1602_WriteData(':');
//	LCD1602_WriteData('0'+TIME[0]/16);				//秒
//	LCD1602_WriteData('0'+(TIME[0]&0x0f));
//
//	LCD1602_WriteCom(0x80);
////	for(i=0;i<2;i++)
////	LCD1602_WriteData(D[i]);
//	LCD1602_WriteData('D');
//	LCD1602_WriteData(' ');
//	LCD1602_WriteData('2');
//	LCD1602_WriteData('0');
//	LCD1602_WriteData('0'+TIME[6]/16);			//年
//	LCD1602_WriteData('0'+(TIME[6]&0x0f));
//	LCD1602_WriteData('-');
//	LCD1602_WriteData('0'+TIME[4]/16);			//月
//	LCD1602_WriteData('0'+(TIME[4]&0x0f));
//	LCD1602_WriteData('-');
//	LCD1602_WriteData('0'+TIME[3]/16);			//日
//	LCD1602_WriteData('0'+(TIME[3]&0x0f));
//	LCD1602_WriteCom(0x8f);
//	LCD1602_WriteData('0'+(TIME[5]&0x07));	//星期			
//}


