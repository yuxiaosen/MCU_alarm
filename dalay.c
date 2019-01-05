#include "delay.h"
void delay_ms(uint i)
{
	uchar n;
 	while(i--)
	for(n=0;n<80;n++)
	;
}
void delay_us(uchar x)	//x*10 us
{
    uchar a,b;
    for(b=x;b>0;b--)
        for(a=1;a>0;a--);
}
