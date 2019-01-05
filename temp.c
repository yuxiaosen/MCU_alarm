#include "temp.h"
#include "lcd1602.h"
uchar DS18B20_Init()
{
	uint i=0;
	WDIO=0;		   //总线拉低
//	i=70;	
//	while(i--);//延时642us
	delay_us(32);  //延时720us
	WDIO=1;		   //总线拉高
	i=0;
	while(WDIO)	   //等待是否拉低，
	{
		i++;
		if(i>5000)
			return 0;
	}
	return 1;
}
void DS18B20_WriteByte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		WDIO=0;
		_nop_();
//		j++;
		 
		WDIO=dat&0x01;
//		j=6;
//		while(j--); 
		delay_us(4);
		WDIO=1;
		dat>>=1;
	}
}
uchar DS18B20_ReadByte()
{
	 uchar byte,b;
	 uchar i;
	 for(i=0;i<8;i++)
	 {
	 	WDIO=0;
//		j++;
		_nop_();
		WDIO=1;
//		j++;
//		j++;
		delay_us(1);
		b=WDIO;
		byte=(byte>>1)|(b<<7);
//		j=4;
//		while(j--);
		delay_us(5);
	 }
	 return byte;
}
void DS18B20_ChangeTemp()
{
	DS18B20_Init();
	delay_ms(1);
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0x44);
	delay_ms(100);
}
void DS18B20_ReadTempCom()
{
	DS18B20_Init();
	delay_ms(1);
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0xbe);
}
int DS18B20_ReadTemp()
{
	int temp;
	uchar tmh,tml;
	DS18B20_ChangeTemp();
	DS18B20_ReadTempCom();
	tml=DS18B20_ReadByte();
    tmh=DS18B20_ReadByte();
	temp=tmh;
	temp<<=8;
	temp|=tml;
	return temp;
}
void LCD1602_DisplayTemp(int temp)
{
	float tp;
		uchar i;
		uchar Temp_datas[]={0,0,0,0,0};
	if(temp<0)
	{
		LCD1602_WriteCom(0xc0+10);
		LCD1602_WriteData('-');
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;
	}
	else
	{
		LCD1602_WriteCom(0xc0+10);
		LCD1602_WriteData(' ');
		temp=temp-1;
		tp=temp;
		temp=tp*0.0625*100+0.5;
	}
	Temp_datas[0]=temp%10000/1000;
	Temp_datas[1]=temp%1000/100;

	Temp_datas[3]=temp%100/10;
	Temp_datas[4]=temp%10;
	for(i=0;i<2;i++)
	{
		LCD1602_WriteData('0'+Temp_datas[i]);	
	}
	LCD1602_WriteData('C');
}








