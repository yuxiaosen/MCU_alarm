#include "sys.h"
#include "delay.h"
#include "lcd1602.h"
#include "temp.h"
#include "ds1302.h"
#include "keyProcess.h"
#include "otherProcess.h"
#include "musicCode.h"
#define RldTmr(fr) 65536 - (11059200 / 12) / (fr << 1)
#define FuDian(n) (n << 1) / 3    //附点n分音符的换算
sbit beep = P2^0;

uchar Temp_TH0,Temp_TL0;
uchar Temp_TH1,Temp_TL1; 
uchar tmrFlg=0,enable = 1;
extern uint mflag;	  //音符计算
extern bit nflag;
extern bit aflag;	  //闹钟开关
extern uchar a;
extern uchar b;

void PlayMusic(uchar *Sound,uchar *lengthlen,uint speed);    //固定标准为4分音符的速度：例如speed = 108 表示一分钟扫过108个4分音符
void sound_Init();
uint length(uchar *Sound);			 //判断音符个数
uint mflag;							 //音符循环个数
bit nflag=0;						 //1分钟关闭闹钟暂停标志
bit aflag=1;	//总开关
uchar stop[7];	//暂停标志位

uchar a;	   //闹钟x
uchar b;	   //重复检测
void main()
{
	uchar mode;	   
	stop[0]=1;	   //暂停标志位
	stop[1]=1;
	stop[2]=1;
	stop[3]=1;
	stop[4]=1;
	stop[5]=1;
	stop[6]=1;
	Clock=0;	   //初始闹钟个数
	
	LCD1602_Init();	   //1602初始化
	DS1302_Init();	   //1302初始化
	sound_Init();	   //定时器初始化
	
	while(1)
	{	
		/**********按键处理***********/	
		if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)
		{				
			switch (key_scan())
			{	
				case K1: mode=MODE0;break;		//MODE0进入菜单
				case K2: mode=MODE1;break;		//MODE0设置日期
				case K3: mode=MODE2;break;		//MODE0设置闹钟
				case K4: mode=MODE3;aflag=!aflag;break;		//MODE3退出
			}
			key_process(mode);				   //按键处理函数
		}
		/**********主屏幕显示**********/		
		LCD1602_DisplayTemp(DS18B20_ReadTemp());	 //温度
		DS1302_ReadTime();							 //读时间
		LCD1602_DisplayData();						 //显示时间
		if(AM_PM==0&&hour12_24==1)					 //显示上午
		{
			LCD1602_WriteCom(0x80+13);
			LCD1602_WriteData('A');
		}
		if(AM_PM==1&&hour12_24==1)					 //显示下午
		{
			LCD1602_WriteCom(0x80+13);
			LCD1602_WriteData('P');
		}											 //24小时制
		if(hour12_24==0)
		{
			LCD1602_WriteCom(0x80+13);
			LCD1602_WriteData(' ');
		}
		LCD1602_WriteCom(0xc0+15);					 //显示闹钟开关
		if(aflag)
			LCD1602_WriteData('N');
		else 
			LCD1602_WriteData('F');
		/**********闹钟设置**********/
		if(aflag==1)
		{
		for(a=0;a<=Clock;a++)			 //检测闹钟
		{
			if(alarm[a][2]==1)		 //判断闹钟是否开启
			{
				if(alarm[a][3]==1)	  //判断是否重复 alarm[a][3]==1为重复
				{
					for(b=0;b<7;b++)		//判断周是否重复
					{
					 if(TIME[5]==b+1&&repeat[a][b]==1)	  //	repeat[a][0]		  重复天数星期一到星期日
					 {
						 if((alarm[a][0]==TIME[1]&&alarm[a][1]==TIME[2]))	   //alarm[0][0]=分钟 alarm[0][1]=小时	
						{
							nflag=1;
							switch(alarm[a][4])
							{
								case 0:	   PlayMusic(musicNote5,noteDuration5,108);	  break;
								case 1:	   PlayMusic(musicNote1,noteDuration1,108);	  break;
								case 2:	   PlayMusic(musicNote2,noteDuration2,108);	  break;
								case 3:	   PlayMusic(musicNote3,noteDuration3,108);	  break;
								case 4:	   PlayMusic(musicNote4,noteDuration4,108);	  break;
							
							}
					
						}
						else
							stop[a]=1;
					}
					}
				} 
				else
				{
					for(b=0;b<7;b++)
					{
					 if(TIME[5]==b+1&&repeat[a][b]==1)	  //	repeat[a][0]		  重复天数星期一到星期日
					 {
						 if((alarm[a][0]==TIME[1]&&alarm[a][1]==TIME[2]))	   //alarm[0][0]=分钟 alarm[0][1]=小时	
						{
							switch(alarm[a][4])
							{
								case 0:	   PlayMusic(musicNote5,noteDuration5,108);	  break;
								case 1:	   PlayMusic(musicNote1,noteDuration1,108);	  break;
								case 2:	   PlayMusic(musicNote2,noteDuration2,108);	  break;
								case 3:	   PlayMusic(musicNote3,noteDuration3,108);	  break;
								case 4:	   PlayMusic(musicNote4,noteDuration4,108);	  break;
							
							}
					
						}
						else
							stop[a]=1;
					}
					}
				}
		}
		}
		}
		else
		{
			 beep=1;TR0=0;
		}
//		LCD1602_WriteCom(0xc0+15);
//		if(stop[a]==1)
//		{
//			LCD1602_WriteData('1');	
//		}
//		else
//			LCD1602_WriteData('0');
		delay_ms(10);
	}

}

/********音乐代码*********/
void sound_Init()
{
	
    Temp_TH1 = (65535-1000)/256; // 计算TL1应装入的初值 (1ms的初装值) 
	Temp_TL1 = (65535-1000)%256; // 计算TH1应装入的初值 
	beep=1;
	EA=1;
	TMOD|=0x11;
	TH1=Temp_TH1;
	TL1=Temp_TL1;
	TH0=0xff;
	TL0=0x00;
	ET0=1;
	ET1=0;
	TR0=0;
	TR1=0;
} 
uint length(uchar *Sound)
{
	uint SoundLength=0;
	while(Sound[SoundLength] != 0) //计算歌曲长度 
	{ 
		SoundLength+=1; 
	} 	
	return SoundLength;
}
void PlayMusic(uchar *Sound,uchar *music,uint speed) {
    uint i;
    uchar idx;
	uint Tone,Temp_T,durationCnt=0,soundCnt=0,NO_soundCnt=0,cnt=0;

	TR0 = 1; 
	TR1 = 1; 
	Tone=length( Sound);
	mflag=0;
	if((alarm[a][0]==TIME[1]&&alarm[a][1]==TIME[2]))
		nflag=1;
	else
	{
		nflag=0;
		if(alarm[a][3]==0)
			repeat[a][b]=0;
	}
	while(mflag!=Tone&&nflag==1&&stop[a]==1)
	{
		if((alarm[a][0]==TIME[1]&&alarm[a][1]==TIME[2]))
			nflag=1;
		else
		{
			nflag=0;
			stop[a]=1;
			if(alarm[a][3]==0)
				repeat[a][b]=0;
		}
		if(nflag==0)
		{	TR0=0;
			break;
		}
		if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)
		{				
			switch (key_scan())
			{
				case K1: break;
				case K2: break;
				case K3: break;
				case K4: stop[a]=0;delay_ms(100);
						 if(alarm[a][3]==0)
							repeat[a][b]=0;
						 break;
			}
		}
       	while (!tmrFlg) ;
        tmrFlg = 0;
            idx = Sound[mflag] - 1;
	  		Temp_T=	tmrRld[idx];
            Temp_TH0 = Temp_T/256; 
			Temp_TL0 = Temp_T%256; 
			TH0 = Temp_TH0; 
			TL0 = Temp_TL0 + 12; //加12是对中断延时的补偿
			durationCnt = (ulong)240 * (ulong)noteFreq[idx] / ((ulong)music[mflag] * (ulong)speed);
			soundCnt=durationCnt-(durationCnt >> 2);//发声
            NO_soundCnt =  durationCnt >> 2;	//当前音符时值的前3/4发声，后1/4静音
			for(i=soundCnt;i>0;i--) //发规定长度的音 
			{ 
				while(TF1==0); 
				TH1 = Temp_TH1; 
				TL1 = Temp_TL1; 
				TF1=0; 				
			}  
 			enable=0;
			for(i=NO_soundCnt-20;i>0;i--) //音符间的间隔 
			{ 
				while(TF1==0); 
				TH1 = Temp_TH1; 
				TL1 = Temp_TL1+50; 
				TF1=0; 
			}
		if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)
		{				
			switch (key_scan())
			{
				case K1: break;
				case K2: break;
				case K3: break;
				case K4: stop[a]=0;delay_ms(100);
						 if(alarm[a][3]==0)
							repeat[a][b]=0;
						 break;
			}
		}
		/*********闹钟响显示时间*********/		
		DS1302_ReadTime();
		LCD1602_DisplayData();
		if(AM_PM==0&&hour12_24==1)
		{
			LCD1602_WriteCom(0x80+13);
			LCD1602_WriteData('A');
		}
		if(AM_PM==1&&hour12_24==1)
		{
			LCD1602_WriteCom(0x80+13);
			LCD1602_WriteData('P');
		}
		if(hour12_24==0)
		{
			LCD1602_WriteCom(0x80+13);
			LCD1602_WriteData(' ');
		}
		LCD1602_WriteCom(0xc0+15);
		if(aflag)
			LCD1602_WriteData('N');
		else 
			LCD1602_WriteData('F');
		/********判断响一分钟的时间********/
		if((alarm[a][0]==TIME[1]&&alarm[a][1]==TIME[2]))
			nflag=1;
		else
		{
			nflag=0;
		    stop[a]=1;
			if(alarm[a][3]==0)
				repeat[a][b]=0;
			
		}
		enable=1;
		mflag+=1;		
	}	 
	beep=1;enable=0;TR0=0;
	if(!(alarm[a][0]==TIME[1]&&alarm[a][1]==TIME[2]))
		stop[a]=1;
 }
void InterruptTmr0() interrupt 1 
{  
	TH0=Temp_TH0;
	TL0=Temp_TL0;
	tmrFlg = 1;
	if (enable)
    beep=!beep;
	else
	beep=1;
	
}
