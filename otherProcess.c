#include "otherProcess.h"
#include "ds1302.h"
bit hour12_24=0;
bit AM_PM=0;
bit alarm_AM_PM;
void LCD1602_alarm(uchar addr,uchar hour,uchar min)
{
	uchar i;
	if(hour12_24==0)
	{
	LCD1602_WriteCom(addr);
	LCD1602_WriteData('0'+hour/16);				//时
	LCD1602_WriteData('0'+(hour&0x0f));
	}
	if(hour12_24==1)
	{
		if(hour>0x12)
		{
			i=hour-0x12;
				alarm_AM_PM=1;
			if(hour==0x20)
				i=0x08;
			if(hour==0x21)
				i=0x09;
		}
		else											  //早上
		{		
			alarm_AM_PM=0;
			i=hour;
			if(hour==0x12)
				alarm_AM_PM=1;
			if(hour==0x00)
				i=0x12;
		}
		LCD1602_WriteCom(addr);
		LCD1602_WriteData('0'+i/16);				//时
		LCD1602_WriteData('0'+(i&0x0f));
		if(alarm_AM_PM==0)
		{
			LCD1602_WriteCom(addr+5);
			LCD1602_WriteData('A');
		}
		else
		{
			LCD1602_WriteCom(addr+5);
			LCD1602_WriteData('P');
		}
	}	
	LCD1602_WriteCom(addr+2);
	LCD1602_WriteData(':');
	LCD1602_WriteData('0'+min/16);				//分
	LCD1602_WriteData('0'+(min&0x0f));	

}
void LCD1602_Data(uchar addr,uchar dat)		   //写一个地址和数据
{
	LCD1602_WriteCom(addr);
	LCD1602_WriteData(dat);
}
void LCD1602_DisplayData()
{
	uchar i;
	LCD1602_WriteCom(0x80+0X40);
//	for(i=0;i<2;i++)
//	LCD1602_WriteData(T[i]);
	LCD1602_WriteData('T');
	LCD1602_WriteData(' ');
	if(hour12_24==0)
	{
		LCD1602_WriteCom(0x80+0X40+2);
		LCD1602_WriteData('0'+TIME[2]/16);				//时
		LCD1602_WriteData('0'+(TIME[2]&0x0f));			
	}
	if(hour12_24==1)
	{
		if(TIME[2]>0x12)								   //下午
		{	
			i=TIME[2]-0x12;
			AM_PM=1;
			if(TIME[2]==0x20)
				i=0x08;
			if(TIME[2]==0x21)
				i=0x09;
		}
		else											  //早上
		{		
			AM_PM=0;
			i=TIME[2];
			if(TIME[2]==0x12)
				AM_PM=1;
			if(TIME[2]==0x00)
				i=0x12;
		}
			LCD1602_WriteCom(0x80+0X40+2);
			LCD1602_WriteData('0'+i/16);				//时
			LCD1602_WriteData('0'+(i&0x0f));
					
	}
	LCD1602_WriteCom(0x80+0X40+4);	 
	LCD1602_WriteData(':');
	LCD1602_WriteData('0'+TIME[1]/16);				//分
	LCD1602_WriteData('0'+(TIME[1]&0x0f));	
	LCD1602_WriteData(':');
	LCD1602_WriteData('0'+TIME[0]/16);				//秒
	LCD1602_WriteData('0'+(TIME[0]&0x0f));

	LCD1602_WriteCom(0x80);
//	for(i=0;i<2;i++)
//	LCD1602_WriteData(D[i]);
	LCD1602_WriteData('D');
	LCD1602_WriteData(' ');
	LCD1602_WriteData('2');
	LCD1602_WriteData('0');
	LCD1602_WriteData('0'+TIME[6]/16);			//年
	LCD1602_WriteData('0'+(TIME[6]&0x0f));
	LCD1602_WriteData('-');
	LCD1602_WriteData('0'+TIME[4]/16);			//月
	LCD1602_WriteData('0'+(TIME[4]&0x0f));
	LCD1602_WriteData('-');
	LCD1602_WriteData('0'+TIME[3]/16);			//日
	LCD1602_WriteData('0'+(TIME[3]&0x0f));
	LCD1602_WriteCom(0x8f);
	LCD1602_WriteData('0'+(TIME[5]&0x07));	//星期			
}
