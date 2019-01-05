#include<reg51.h>
#define uint unsigned int 
#define uchar unsigned char
sbit led0=P1^0;
sbit led1=P1^1;
sbit key0=P3^2;
sbit key1=P3^3;
void delay_ms(uint ms)
{
	uchar ns;
	while(ms--)
	for(ns=0;ns<120;ns++) ;
}
void TNI0() interrupt 0 using 1
{
	delay_ms(10);
 	if(key0==0)
	{
		led0=~led0;
	}
	while(!key0) ;
}
void TNI1() interrupt 2 using 3
{
	delay_ms(10);
 	if(key1==0)
	{
		led0=~led0;
		led1=~led1;
	}
	while(!key1) ;
}
void TNI_Init()
{
	EA=1;
	EX0=1;
	EX1=1;
	IT0=1;
	IT1=1;
}
void main()
{
  led0=0;
  led1=0;
  TNI_Init();
  while(1) ;


}


