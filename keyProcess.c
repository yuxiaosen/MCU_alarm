#include"keyProcess.h"
#include "lcd1602.h"
#include "temp.h"
#include "ds1302.h"
#include "otherProcess.h"
 

/****************************************************************************************************
函数名称：key_process(uchar mode)
函数功能：按键处理函数（调节日期,时间,闹钟）
输入参数：mode		用来选择模式，是修改日期，时间还是闹钟
返回值：无
****************************************************************************************************/



uchar xdata alarm[7][5]={0}; //0-4个闹钟//alarm[0][0] 分钟设置，alarm[0][1] 小时设置，alarm[0][2] 重复/不重复，alarm[0][3] 选择音乐
uchar repeat[7][7]={0};			//重复
int Clock;					//闹钟个数
uchar code timeSet[]={"1:timeSet"};
uchar code alarmSet[]={"2:alarmSet"};
void key_process(uchar mode)
{
	bit flag=0;	   //退出标志位
	int Wch=0;   //时间设置标志
	uchar AlarmWch=0; 	//闹钟设置标志
	uchar n;
	uchar t;
	bit year=1;		   //闰年判断
	int function=0;    //功能选择
	int ClockWch=0;	   //位置选择
	int ClockNum=0;	   //闹钟x
	switch(mode)
	{
		case MODE0:		 	//打开菜单
			LCD1602_WriteCom(0x01);
			LCD1602_WriteCom(0x80);
			for(t=0;t<9;t++)
				LCD1602_WriteData(timeSet[t]);
			LCD1602_WriteCom(0xC0);
			for(t=0;t<10;t++)
				LCD1602_WriteData(alarmSet[t]);		
			LCD1602_Cursor(0x0F,0x80);
			while(1)
			 {
			 	 delay_ms(500);
				 if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)//检测有没有按键按下，有按键按下才执行操作
				 {
					
				 	switch(key_scan())
					{
				 	case K1:
					/****************设置时间***************/         
							if(function==0)   	    
							{
							/***********时间显示***********/
								DS1302_ReadTime();
								LCD1602_DisplayData();
								if(AM_PM==0&&hour12_24==1)
								{
									LCD1602_WriteCom(0x80+13);
									LCD1602_WriteData('A');
									LCD1602_WriteCom(0xC0+13);
									LCD1602_WriteData('1');
									LCD1602_WriteData('2');
								}
								if(AM_PM==1&&hour12_24==1)
								{
									LCD1602_WriteCom(0x80+13);
									LCD1602_WriteData('P');
									LCD1602_WriteCom(0xC0+13);
									LCD1602_WriteData('1');
									LCD1602_WriteData('2');
								}
								if(hour12_24==0)
								{
									LCD1602_WriteCom(0xC0+13);
									LCD1602_WriteData('2');
									LCD1602_WriteData('4');
								}
								LCD1602_Cursor(0x0F,0xC0+9);
					/**********按键处理修改时间**********/
								while(1)
								{
								     delay_ms(500);
									 if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)//检测有没有按键按下，有按键按下才执行操作
									 {
									 	 switch(key_scan())
										 {
										 	case K1:	Wch++;if(Wch>7) Wch=0; 						  //选择时间的哪一位被更改
												/***********光标显示哪个被选中*********/
														if(Wch<3)
														LCD1602_Cursor(0x0F,0xC0+9-3*Wch);
														if(Wch==3)
														LCD1602_Cursor(0x0F,0x80+11);
														if(Wch==4)
														LCD1602_Cursor(0x0F,0x80+8);
														if(Wch==5)
														LCD1602_Cursor(0x0F,0x80+15);
														if(Wch==6)
														LCD1602_Cursor(0x0F,0x80+5);
														if(Wch==7)
														LCD1602_Cursor(0x0F,0xC0+14);
														break;			  
											case K2:				   								//K2按下，数字加一
														TIME[Wch]++;
												/********时间修改上限*********/			
														if((TIME[Wch]&0x0f)>9)					 //换成BCD码。
														{
															TIME[Wch]=TIME[Wch]+6;
														}
														if((TIME[Wch]>=0x60)&&(Wch<2))		//分秒只能到59
														{
															TIME[Wch]=0;
														}
														if((TIME[Wch]>=0x24)&&(Wch==2))		//小时只能到23
														{
															TIME[Wch]=0;
														}														
														if((TIME[Wch]>=0x32)&&(TIME[4]==1||TIME[4]==3||TIME[4]==5||TIME[4]==7||TIME[4]==8||TIME[4]==0x10||TIME[4]==0x12)&&(Wch==3))		//日只能到31
														{
															TIME[Wch]=1;	
														}
														if((TIME[Wch]>=0x31)&&(TIME[4]==4||TIME[4]==6||TIME[4]==9||TIME[4]==0x11)&&(Wch==3))		//日只能到30
														{
															TIME[Wch]=1;	
														}
														if(TIME[4]==2&&(((uint)(TIME[6]+2000)%4==0&&(uint)(TIME[6]+2000)%100!=0)||((uint)(TIME[6]+2000)%400==0)))	 //润年判断
															year=1;					 //1为闰年
														else
															year=0;					 //0不是闰年
														if((TIME[Wch]>=0x30)&&(TIME[4]==2)&&(Wch==3)&&year==1)		//日只能到29
														{
															TIME[Wch]=1;	
														}
														if((TIME[Wch]>=0x29)&&(TIME[4]==2)&&(Wch==3)&&year==0)		//日只能到28
														{
															TIME[Wch]=1;	
														}
														if((TIME[Wch]>=0x13)&&(Wch==4))		//月只能到12
														{
															TIME[Wch]=1;
														}	
														if((TIME[Wch]>0x7)&&(Wch==5))		//周只能到7
														{
															TIME[Wch]=1;
														}
														if(Wch==7)							//改变时间制
														{
															hour12_24=!hour12_24;
														}
														if(hour12_24==0)					//0为24小时制
														{
															 LCD1602_WriteCom(0x80+13);
															 LCD1602_WriteData(' ');
														}
														LCD1602_DisplayData();				//显示修改时间
														if(AM_PM==0&&hour12_24==1)			//判断12小时制上午
														{
															LCD1602_WriteCom(0x80+13);
															LCD1602_WriteData('A');
															LCD1602_WriteCom(0xC0+13);
															LCD1602_WriteData('1');
															LCD1602_WriteData('2');
														}
														if(AM_PM==1&&hour12_24==1)			//判断12小时制下午
														{
															LCD1602_WriteCom(0x80+13);
															LCD1602_WriteData('P');
															LCD1602_WriteCom(0xC0+13);
															LCD1602_WriteData('1');
															LCD1602_WriteData('2');
														}
														if(hour12_24==0)					  //显示24小时制
														{
															LCD1602_WriteCom(0xC0+13);
															LCD1602_WriteData('2');
															LCD1602_WriteData('4');
														}
														/**********开光标显示修改位置*********/
														if(Wch<3)
														LCD1602_Cursor(0x0F,0xC0+9-3*Wch);
														if(Wch==3)
														LCD1602_Cursor(0x0F,0x80+11);
														if(Wch==4)
														LCD1602_Cursor(0x0F,0x80+8);
														if(Wch==5)
														LCD1602_Cursor(0x0F,0x80+15);
														if(Wch==6)
														LCD1602_Cursor(0x0F,0x80+5);
														if(Wch==7)
														LCD1602_Cursor(0x0F,0xC0+14);
														DS1302_Init();
														break;
														break;		
											case K3:		TIME[Wch]--;					 //K3按下，数字减一	//有毒，待改
														if((TIME[Wch]==0x00-1)&&(Wch<2))		//分秒只能到59
														{
															TIME[Wch]=0x59;
														}
														if((TIME[Wch]==0x00-1)&&(Wch==2))		//小时只能到23
														{
															TIME[Wch]=0x23;
														}
//														if((TIME[Wch]==0x00-1)&&(Wch==2)&&hour12_24==1)		//小时只能到12		  待改
//														{
//															TIME[Wch]=0x12;
//														}
														if((TIME[Wch]==0x00)&&(TIME[4]==1||TIME[4]==3||TIME[4]==5||TIME[4]==7||TIME[4]==8||TIME[4]==0x10||TIME[4]==0x12)&&(Wch==3))		//日只能到31
														{
															TIME[Wch]=0x31;	
														}
														if((TIME[Wch]==0x00)&&(TIME[4]==4||TIME[4]==6||TIME[4]==9||TIME[4]==0x11)&&(Wch==3))		//日只能到30
														{
															TIME[Wch]=0x30;	
														}
														if(TIME[4]==2&&((((TIME[6]/16*10+TIME[6]&0x0f)+2000)%4==0&&((TIME[6]/16*10+TIME[6]&0x0f)+2000)%100!=0)))
														{
															year=1;
														}
														else if(TIME[4]==2&&(((TIME[6]/16*10+TIME[6]&0x0f)+2000)%400==0))
														{
															year=1;
														}
														else
															year=0;
														
														if((TIME[Wch]==0x00)&&(TIME[4]==2)&&(Wch==3)&&year==1)		//日只能到29
														{
															TIME[Wch]=0x29;	
														}
														if((TIME[Wch]==0x00)&&(TIME[4]==2)&&(Wch==3)&&year==0)		//日只能到28
														{
															TIME[Wch]=0x28;	
														}
														if((TIME[Wch]==0x00)&&(Wch==4))		//月只能到12
														{
															TIME[Wch]=0x12;
														}	
														if((TIME[Wch]==0x00)&&(Wch==5))		//周只能到7
														{
															TIME[Wch]=0x07;
														} 
														if((TIME[Wch]&0x0f)>9)					 //换成BCD码。
														{
															TIME[Wch]=TIME[Wch]-6;
														}
													//	DS1302_ReadTime();
														if(Wch==7)
														{
															hour12_24=!hour12_24;
														}
														if(hour12_24==0)
														{
															 LCD1602_WriteCom(0x80+13);
															 LCD1602_WriteData(' ');
														}
														LCD1602_DisplayData();
														if(AM_PM==0&&hour12_24==1)
														{
															LCD1602_WriteCom(0x80+13);
															LCD1602_WriteData('A');
															LCD1602_WriteCom(0xC0+13);
															LCD1602_WriteData('1');
															LCD1602_WriteData('2');
														}
														if(AM_PM==1&&hour12_24==1)
														{
															LCD1602_WriteCom(0x80+13);
															LCD1602_WriteData('P');
															LCD1602_WriteCom(0xC0+13);
															LCD1602_WriteData('1');
															LCD1602_WriteData('2');
														}
														if(hour12_24==0)
														{
															LCD1602_WriteCom(0xC0+13);
															LCD1602_WriteData('2');
															LCD1602_WriteData('4');
														}
														if(Wch<3)
														LCD1602_Cursor(0x0F,0xC0+9-3*Wch);
														if(Wch==3)
														LCD1602_Cursor(0x0F,0x80+11);
														if(Wch==4)
														LCD1602_Cursor(0x0F,0x80+8);
														if(Wch==5)
														LCD1602_Cursor(0x0F,0x80+15);
														if(Wch==6)
														LCD1602_Cursor(0x0F,0x80+5);
														if(Wch==7)
														LCD1602_Cursor(0x0F,0xC0+14);
														DS1302_Init();
														break;			  
											case K4:	flag=1;break;			   //保存退出
										 }
									 }
									 /********退出后显示**********/	
									 if(flag==1)
									 {
//									 		DS1302_Init();
//									 	  	LCD1602_DisplayTemp(DS18B20_ReadTemp());
//											DS1302_ReadTime();
//											LCD1602_DisplayData();
											//	delay_ms(1000);
											LCD1602_WriteCom(0x01);
											LCD1602_WriteCom(0x80);
											for(t=0;t<9;t++)
												LCD1602_WriteData(timeSet[t]);
											LCD1602_WriteCom(0xC0);
											for(t=0;t<10;t++)
												LCD1602_WriteData(alarmSet[t]);											
											if(function==0)
												LCD1602_Cursor(0x0F,0x80);
											if(function==1)
												LCD1602_Cursor(0x0F,0xC0);
											flag=0;
											Wch=0;
											break;
									 }
								}
												 
							}
							/********************************闹钟设置*********************************/
							if(function==1)								 
							{
								LCD1602_WriteCom(0x01);
								LCD1602_WriteCom(0x80);
								for(n=0;n<=Clock;n++)
								{	
									if(n>6)
										break;
									LCD1602_WriteData('1'+n);
									LCD1602_WriteData(':');
									if(alarm[n][2]==0)
										LCD1602_WriteData('F');
									else
										LCD1602_WriteData('N');
									LCD1602_WriteData(' ');
									if(n==3)
										LCD1602_WriteCom(0xC0);
									
								}
								LCD1602_WriteCom(0xC0+13);
								LCD1602_WriteData('+');
								LCD1602_WriteCom(0xC0+14);
								LCD1602_WriteData('-');
								if(ClockWch==2)
									LCD1602_Cursor(0x0F,0x80);
								if(ClockWch==3)
									LCD1602_Cursor(0x0F,0x80+4);
								if(ClockWch==4)
									LCD1602_Cursor(0x0F,0x80+8);
								if(ClockWch==5)
									LCD1602_Cursor(0x0F,0x80+12);
								if(ClockWch==6)
									LCD1602_Cursor(0x0F,0xC0);
								if(ClockWch==7)
									LCD1602_Cursor(0x0F,0xC0+4);
								if(ClockWch==8)
									LCD1602_Cursor(0x0F,0xC0+8);	
								if(ClockWch==0)
									LCD1602_Cursor(0x0F,0xC0+13);
								if(ClockWch==1)
									LCD1602_Cursor(0x0F,0xC0+14);				   //开显示光标
								while(1)
								{
									delay_ms(500);
							   		if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)//检测有没有按键按下，有按键按下才执行操作
				           			{
										switch(key_scan())
										{
											case K1: 							  //确定选择
														if(ClockWch==0)
														{
															Clock++;
															LCD1602_WriteCom(0x01);
															if(Clock>6)
															{
																Clock=6;
															}
														}
														if(ClockWch==1)
														{
															Clock--;
															LCD1602_WriteCom(0x01);
															if(Clock<0)
															{
																Clock=0;
															}
														}		
														if(ClockWch>1)
														{
															ClockNum=ClockWch-2;
															while(1)
															{
																delay_ms(500);
							   									if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)//检测有没有按键按下，有按键按下才执行操作
																{
																	switch(key_scan())
																	{
																			case K1:					 //进入闹钟x设置
																					LCD1602_WriteCom(0x01);
																					LCD1602_alarm(0x80,alarm[ClockNum][1],alarm[ClockNum][0]);	//显示闹钟时分
																					LCD1602_Data(0x80+7,'S');					   //开关说明
																					LCD1602_Data(0x80+8,'W');
																					LCD1602_Data(0x80+9,':');
																					if(alarm[ClockNum][2]==0)								//开关闹钟
																						LCD1602_Data(0x80+10,'F');
																					else
																						LCD1602_Data(0x80+10,'N');
																					LCD1602_Data(0x80+12,'R');					   //重复说明
																					LCD1602_Data(0x80+13,'P');
																					LCD1602_Data(0x80+14,':');
																					if(alarm[ClockNum][3]==0)						//显示重复/不重复闹钟
																						LCD1602_Data(0x80+15,'0');				//0不重复
																					else
																						LCD1602_Data(0x80+15,'1');				//1重复
																					LCD1602_Data(0xC0+0,'M');					   //音乐说明
																					LCD1602_Data(0xC0+1,'U');
																					LCD1602_Data(0xC0+2,'S');
																					LCD1602_Data(0xC0+3,'I');
																					LCD1602_Data(0xC0+4,'C');
																					LCD1602_Data(0xC0+5,':');
																					for(n=0;n<5;n++)								   //显示音乐
																					{
																						LCD1602_Data(0xC0+n*2+6,'0'+n);
																					}
																					LCD1602_Cursor(0x0F,0x80+4);					   //开显示光标
																					while(1)
																					{
																						delay_ms(500);
							   															if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)//检测有没有按键按下，有按键按下才执行操作
																						{
																							switch(key_scan())
																							{
																								case K1: AlarmWch++;					  // 选择设置 重复/不重复，时间，选择音乐
																										 if(AlarmWch>4)
																										 AlarmWch=0;
																										 if(AlarmWch==3)
																											LCD1602_Cursor(0x0F,0x80+13);
																									 	 break;	
																								case K2: alarm[ClockNum][AlarmWch]++;					  		// 闹钟时间加1
																										 if((alarm[ClockNum][AlarmWch]&0x0f)>9&&AlarmWch<2)		//换成BCD码。
																										 {
																											alarm[ClockNum][AlarmWch]=alarm[Clock][AlarmWch]+6;
																										   	 }	
																									     if((alarm[ClockNum][AlarmWch]>=0x60)&&AlarmWch==0)		//分秒只能到59
																										 {
																											alarm[ClockNum][AlarmWch]=0;
																										 }
																										 if((alarm[ClockNum][AlarmWch]>=0x24)&&(AlarmWch==1))	//小时只能到23
																										 {
																											alarm[ClockNum][AlarmWch]=0;
																										 } 
																										 if(AlarmWch==2&&alarm[ClockNum][AlarmWch]>1)				//开关闹钟
																										 	alarm[ClockNum][AlarmWch]=0;
																										 if(AlarmWch==3&&alarm[ClockNum][AlarmWch]>1)				//重复/不重复
																										 	alarm[ClockNum][AlarmWch]=1;
																										 if(AlarmWch==4&&alarm[ClockNum][AlarmWch]>4)				//选择音乐
																										 	alarm[ClockNum][AlarmWch]=0;
																										 if(AlarmWch==3)						   //选择重复
																										 {	
																											 	LCD1602_WriteCom(0x01);
																												LCD1602_WriteCom(0x80);
																												for(n=0;n<7;n++)
																												{
																													LCD1602_WriteData('n');
																													LCD1602_WriteData(':');
																													LCD1602_WriteData('0'+repeat[Clock][n]);
																													LCD1602_WriteData(' ');
																													if(n==3)
																														LCD1602_WriteCom(0xC0);
																												}
																												LCD1602_Data(0xC0+12,'n');
																												LCD1602_Data(0xC0+13,'=');
																												LCD1602_Data(0xC0+14,'1');
																												LCD1602_Cursor(0x0F,0x80);
																												while(1)
																												{
																				
																													delay_ms(500);
							   																						if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)//检测有没有按键按下，有按键按下才执行操作
																													{
																														switch(key_scan())
																														{
																															case K1:   Wch++;if(Wch>6) Wch=0;break;
																															case K2:   repeat[ClockNum][Wch]=1;break;
																															case K3:   repeat[ClockNum][Wch]=0;break;
																															case K4:   flag=1;Wch=0;break;
																														}
																														LCD1602_WriteCom(0x80);
																														for(n=0;n<7;n++)
																														{
																															LCD1602_WriteData('n');
																															LCD1602_WriteData(':');
																															LCD1602_WriteData('0'+repeat[ClockNum][n]);
																															LCD1602_WriteData(' ');
																															if(n==3)
																															LCD1602_WriteCom(0xC0);
																														}
																														LCD1602_Data(0xC0+12,'n');
																														LCD1602_Data(0xC0+13,'=');
																														LCD1602_Data(0xC0+14,'0'+Wch+1);
																														if(Wch==0)
																															LCD1602_Cursor(0x0F,0x80);
																														if(Wch==1)
																															LCD1602_Cursor(0x0F,0x80+4);
																														if(Wch==2)
																															LCD1602_Cursor(0x0F,0x80+8);
																														if(Wch==3)
																															LCD1602_Cursor(0x0F,0x80+12);
																														if(Wch==4)
																															LCD1602_Cursor(0x0F,0xC0);
																														if(Wch==5)
																															LCD1602_Cursor(0x0F,0xC0+4);
																														if(Wch==6)
																															LCD1602_Cursor(0x0F,0xC0+8);
																													}
																													if(flag==1)
																													{
																														flag=0;
																														LCD1602_WriteCom(0x01);
																														LCD1602_alarm(0x80,alarm[Clock][1],alarm[Clock][0]);			   //显示闹钟界面
																														//显示光标
																														LCD1602_Data(0x80+7,'S');					   //开关说明
																														LCD1602_Data(0x80+8,'W');
																														LCD1602_Data(0x80+9,':');
																														if(alarm[ClockNum][2]==0)	 					 //显示开关闹钟
																															LCD1602_Data(0x80+10,'F');				 //0关
																														else
																															LCD1602_Data(0x80+10,'N');				 //1开
																														LCD1602_Data(0x80+12,'R');					   //重复说明
																														LCD1602_Data(0x80+13,'P');
																														LCD1602_Data(0x80+14,':');
																														if(alarm[ClockNum][3]==0)						//显示重复/不重复闹钟
																															LCD1602_Data(0x80+15,'0');				//0不重复
																														else
																															LCD1602_Data(0x80+15,'1');				//1重复
																														LCD1602_Data(0xC0+0,'M');					   //音乐说明
																														LCD1602_Data(0xC0+1,'U');
																														LCD1602_Data(0xC0+2,'S');
																														LCD1602_Data(0xC0+3,'I');
																														LCD1602_Data(0xC0+4,'C');
																														LCD1602_Data(0xC0+5,':');
																														for(n=0;n<5;n++)
																														{
																															 LCD1602_Data(0xC0+n*2+6,'0'+n);
																														}
																														if(AlarmWch==0)								//开显示光标
																															LCD1602_Cursor(0x0F,0x80+4);
																														if(AlarmWch==1)
																															LCD1602_Cursor(0x0F,0x80+1);
																														if(AlarmWch==2)
																															LCD1602_Cursor(0x0F,0x80+10);
																														if(AlarmWch==3)
																															LCD1602_Cursor(0x0F,0x80+14);
																														if(AlarmWch==4)
																														{
																															LCD1602_Cursor(0x0F,0xC0+alarm[ClockNum][4]*2+6);
																														}
																														break;
																													}
																												}						
																											 }
																										 break;
																								case K3: alarm[ClockNum][AlarmWch]--;					 		 // 闹钟时间减1
																										if((alarm[ClockNum][AlarmWch]==0x00-1)&&(AlarmWch==0))	 //分秒只能到59
																										{
																											alarm[ClockNum][AlarmWch]=0x59;
																										}
																										if((alarm[ClockNum][AlarmWch]==0x00-1)&&(AlarmWch==1))		//小时只能到23
																										{
																											alarm[ClockNum][AlarmWch]=0x23;
																										}
																										if((alarm[ClockNum][AlarmWch]&0x0f)>9&&AlarmWch<2)		//换成BCD码。
																										{
																											alarm[ClockNum][AlarmWch]=alarm[ClockNum][AlarmWch]-6;
																										}
																										if(AlarmWch==2&&alarm[ClockNum][AlarmWch]<0)			//开关闹钟
																										 	alarm[ClockNum][AlarmWch]=1;
																										if(AlarmWch==3)									//重复/不重复
																										 	alarm[ClockNum][AlarmWch]=0;
																								    	if(AlarmWch==4&&alarm[ClockNum][AlarmWch]<0)				//选择音乐
																										 	alarm[ClockNum][AlarmWch]=4;

																										if(AlarmWch==3)						   //选择重复
																										 {	
																											 	LCD1602_WriteCom(0x01);
																												LCD1602_WriteCom(0x80);
																												for(n=0;n<7;n++)
																												{
																													LCD1602_WriteData('n');
																													LCD1602_WriteData(':');
																													LCD1602_WriteData('0'+repeat[Clock][n]);
																													LCD1602_WriteData(' ');
																													if(n==3)
																														LCD1602_WriteCom(0xC0);
																												}
																													LCD1602_Data(0xC0+12,'n');
																													LCD1602_Data(0xC0+13,'=');
																													LCD1602_Data(0xC0+14,'1');
																												LCD1602_Cursor(0x0F,0x80);
																												while(1)
																												{
																				
																													delay_ms(500);
							   																						if(key_scan()==K1||key_scan()==K2||key_scan()==K3||key_scan()==K4)//检测有没有按键按下，有按键按下才执行操作
																													{
																														switch(key_scan())
																														{
																															case K1:   Wch++;if(Wch>6) Wch=0;break;
																															case K2:   repeat[ClockNum][Wch]=1;break;
																															case K3:   repeat[ClockNum][Wch]=0;break;
																															case K4:   flag=1;Wch=0;break;
																														}
																														LCD1602_WriteCom(0x80);
																														for(n=0;n<7;n++)
																														{
																															LCD1602_WriteData('n');
																															LCD1602_WriteData(':');
																															LCD1602_WriteData('0'+repeat[ClockNum][n]);
																															LCD1602_WriteData(' ');
																															if(n==3)
																															LCD1602_WriteCom(0xC0);
																														}
																														LCD1602_Data(0xC0+12,'n');
																														LCD1602_Data(0xC0+13,'=');
																														LCD1602_Data(0xC0+14,'0'+Wch+1);
																														if(Wch==0)
																															LCD1602_Cursor(0x0F,0x80);
																														if(Wch==1)
																															LCD1602_Cursor(0x0F,0x80+4);
																														if(Wch==2)
																															LCD1602_Cursor(0x0F,0x80+8);
																														if(Wch==3)
																															LCD1602_Cursor(0x0F,0x80+12);
																														if(Wch==4)
																															LCD1602_Cursor(0x0F,0xC0);
																														if(Wch==5)
																															LCD1602_Cursor(0x0F,0xC0+4);
																														if(Wch==6)
																															LCD1602_Cursor(0x0F,0xC0+8);
																													}
																													if(flag==1)
																													{
																														flag=0;
																														LCD1602_WriteCom(0x01);
																														LCD1602_alarm(0x80,alarm[Clock][1],alarm[Clock][0]);			   //显示闹钟界面
																														//显示光标
																														LCD1602_Data(0x80+7,'S');					   //开关说明
																														LCD1602_Data(0x80+8,'W');
																														LCD1602_Data(0x80+9,':');
																														if(alarm[ClockNum][2]==0)	 					 //显示开关闹钟
																															LCD1602_Data(0x80+10,'F');				 //0关
																														else
																															LCD1602_Data(0x80+10,'N');				 //1开
																														LCD1602_Data(0x80+12,'R');					   //重复说明
																														LCD1602_Data(0x80+13,'P');
																														LCD1602_Data(0x80+14,':');
																														if(alarm[ClockNum][3]==0)						//显示重复/不重复闹钟
																															LCD1602_Data(0x80+15,'0');				//0不重复
																														else
																															LCD1602_Data(0x80+15,'1');				//1重复
																														LCD1602_Data(0xC0+0,'M');					   //音乐说明
																														LCD1602_Data(0xC0+1,'U');
																														LCD1602_Data(0xC0+2,'S');
																														LCD1602_Data(0xC0+3,'I');
																														LCD1602_Data(0xC0+4,'C');
																														LCD1602_Data(0xC0+5,':');
																														for(n=0;n<5;n++)
																														{
																															 LCD1602_Data(0xC0+n*2+6,'0'+n);
																														}
																														if(AlarmWch==0)								//开显示光标
																															LCD1602_Cursor(0x0F,0x80+4);
																														if(AlarmWch==1)
																															LCD1602_Cursor(0x0F,0x80+1);
																														if(AlarmWch==2)
																															LCD1602_Cursor(0x0F,0x80+10);
																														if(AlarmWch==3)
																															LCD1602_Cursor(0x0F,0x80+15);
																														if(AlarmWch==4)
																														{
																															LCD1602_Cursor(0x0F,0xC0+alarm[ClockNum][4]*2+6);
																														}
																														break;
																													}
																												}						
																											 }
																										  break;
																								case K4: flag=1;break;					  // 退出设置
																							}
																							LCD1602_alarm(0x80,alarm[ClockNum][1],alarm[ClockNum][0]);			   //显示闹钟界面
																							//显示光标
																							LCD1602_Data(0x80+7,'S');					   //开关说明
																							LCD1602_Data(0x80+8,'W');
																							LCD1602_Data(0x80+9,':');
																							if(alarm[ClockNum][2]==0)	 					 //显示开关闹钟
																								LCD1602_Data(0x80+10,'F');				 //0关
																							else
																								LCD1602_Data(0x80+10,'N');				 //1开
																							LCD1602_Data(0x80+12,'R');					   //重复说明
																							LCD1602_Data(0x80+13,'P');
																							LCD1602_Data(0x80+14,':');
																							if(alarm[ClockNum][3]==0)						//显示重复/不重复闹钟
																								LCD1602_Data(0x80+15,'0');				//0不重复
																							else
																								LCD1602_Data(0x80+15,'1');				//1重复
																							LCD1602_Data(0xC0+0,'M');					   //音乐说明
																							LCD1602_Data(0xC0+1,'U');
																							LCD1602_Data(0xC0+2,'S');
																							LCD1602_Data(0xC0+3,'I');
																							LCD1602_Data(0xC0+4,'C');
																							LCD1602_Data(0xC0+5,':');
																							for(n=0;n<5;n++)
																							{
																								 LCD1602_Data(0xC0+n*2+6,'0'+n);
																							}
																							if(AlarmWch==0)								//开显示光标
																								LCD1602_Cursor(0x0F,0x80+4);
																							if(AlarmWch==1)
																								LCD1602_Cursor(0x0F,0x80+1);
																							if(AlarmWch==2)
																								LCD1602_Cursor(0x0F,0x80+10);
																							if(AlarmWch==3)
																								LCD1602_Cursor(0x0F,0x80+15);
																							if(AlarmWch==4)
																							{
																								LCD1602_Cursor(0x0F,0xC0+alarm[ClockNum][4]*2+6);
																							}
																						}
																						if(flag==1)											  //退出显示闹钟选择界面
																						{
																						
																							AlarmWch=0;
																						
																							LCD1602_WriteCom(0x01);
																							LCD1602_WriteCom(0x80);
													 										for(n=0;n<=Clock;n++)
																							{	
																								if(n>6)
																							 		 break;
																								LCD1602_WriteData('1'+n);
																								LCD1602_WriteData(':');
																								if(alarm[n][2]==0)
																									LCD1602_WriteData('F');
																								else
																									LCD1602_WriteData('N');
																									LCD1602_WriteData(' ');
																								if(n==3)
																									 LCD1602_WriteCom(0xC0);
																
																							}
																							LCD1602_WriteCom(0xC0+13);
																							LCD1602_WriteData('+');
																							LCD1602_WriteCom(0xC0+14);
																							LCD1602_WriteData('-');
																							if(ClockWch==2)
																								LCD1602_Cursor(0x0F,0x80);
																							if(ClockWch==3)
																								LCD1602_Cursor(0x0F,0x80+4);
																							if(ClockWch==4)
																								LCD1602_Cursor(0x0F,0x80+8);
																							if(ClockWch==5)
																								LCD1602_Cursor(0x0F,0x80+12);
																							if(ClockWch==6)
																								LCD1602_Cursor(0x0F,0xC0);
																							if(ClockWch==7)
																								LCD1602_Cursor(0x0F,0xC0+4);
																							if(ClockWch==8)
																								LCD1602_Cursor(0x0F,0xC0+8);	
																							if(ClockWch==0)
																								LCD1602_Cursor(0x0F,0xC0+13);
																							if(ClockWch==1)
																								LCD1602_Cursor(0x0F,0xC0+14);
																							break;
																						}
																					}
																					break;
																		case K2:  ClockWch++;  break;	 //选择设置位置加1
																		case K3:  ClockWch--;  break;	 //选择设置位置减1
																		case K4:  flag=1;break;
																	}
																}
																if(flag==1||ClockWch!=(ClockNum+2))
																{
																	flag=0;
																	ClockNum=0;
																	LCD1602_WriteCom(0x01);
																	LCD1602_WriteCom(0x80);
													 				for(n=0;n<=Clock;n++)
																	{	
																		if(n>6)
																	 		 break;
																		LCD1602_WriteData('1'+n);
																		LCD1602_WriteData(':');
																		if(alarm[n][2]==0)
																			LCD1602_WriteData('F');
																		else
																			LCD1602_WriteData('N');
																		LCD1602_WriteData(' ');
																		if(n==3)
																			 LCD1602_WriteCom(0xC0);
																
																	}
																	LCD1602_WriteCom(0xC0+13);
																	LCD1602_WriteData('+');
																	LCD1602_WriteCom(0xC0+14);
																	LCD1602_WriteData('-');
																	if(ClockWch==2)
																		LCD1602_Cursor(0x0F,0x80);
																	if(ClockWch==3)
																		LCD1602_Cursor(0x0F,0x80+4);
																	if(ClockWch==4)
																		LCD1602_Cursor(0x0F,0x80+8);
																	if(ClockWch==5)
																		LCD1602_Cursor(0x0F,0x80+12);
																	if(ClockWch==6)
																		LCD1602_Cursor(0x0F,0xC0);
																	if(ClockWch==7)
																		LCD1602_Cursor(0x0F,0xC0+4);
																	if(ClockWch==8)
																		LCD1602_Cursor(0x0F,0xC0+8);	
																	if(ClockWch==0)
																		LCD1602_Cursor(0x0F,0xC0+13);
																	if(ClockWch==1)
																		LCD1602_Cursor(0x0F,0xC0+14);
																	break;
																}
															}
														}
														LCD1602_WriteCom(0x80);
														for(n=0;n<=Clock;n++)
														{	
															if(n>6)
																break;
															LCD1602_WriteData('1'+n);
															LCD1602_WriteData(':');
															if(alarm[n][2]==0)
																LCD1602_WriteData('F');
															else
																LCD1602_WriteData('N');
															LCD1602_WriteData(' ');
															if(n==3)
																LCD1602_WriteCom(0xC0);
														}
														LCD1602_WriteCom(0xC0+13);
														LCD1602_WriteData('+');
														LCD1602_WriteCom(0xC0+14);
														LCD1602_WriteData('-');

														if(ClockWch==2)		   //显示光标
															LCD1602_Cursor(0x0F,0x80);
														if(ClockWch==3)
															LCD1602_Cursor(0x0F,0x80+4);
														if(ClockWch==4)
															LCD1602_Cursor(0x0F,0x80+8);
														if(ClockWch==5)
															LCD1602_Cursor(0x0F,0x80+12);
														if(ClockWch==6)
															LCD1602_Cursor(0x0F,0xC0);
														if(ClockWch==7)
															LCD1602_Cursor(0x0F,0xC0+4);
														if(ClockWch==8)
															LCD1602_Cursor(0x0F,0xC0+8);	
														if(ClockWch==0)
															LCD1602_Cursor(0x0F,0xC0+13);
														if(ClockWch==1)
															LCD1602_Cursor(0x0F,0xC0+14);
														break;
											case K2:	
														ClockWch++;		 //选择设置位置加1
														
														if(ClockWch>Clock+2)
															 ClockWch=0;
														break;
											case K3:	ClockWch--;		 //选择设置位置减1
														if(ClockWch<0)
															 ClockWch=Clock+2;
														break;
											case K4:	flag=1;break;
										}
									}
//									LCD1602_WriteCom(0x01);				  //显示闹钟个数
									LCD1602_WriteCom(0x80);
									for(n=0;n<=Clock;n++)
									{	
										if(n>6)
											break;
										LCD1602_WriteData('1'+n);
										LCD1602_WriteData(':');
										if(alarm[n][2]==0)
											LCD1602_WriteData('F');
										else
											LCD1602_WriteData('N');
											LCD1602_WriteData(' ');
										if(n==3)
											LCD1602_WriteCom(0xC0);
									
									}
									LCD1602_WriteCom(0xC0+13);
									LCD1602_WriteData('+');
									LCD1602_WriteCom(0xC0+14);
									LCD1602_WriteData('-');

									if(ClockWch==2)		   //显示光标
											LCD1602_Cursor(0x0F,0x80);
										if(ClockWch==3)
											LCD1602_Cursor(0x0F,0x80+4);
										if(ClockWch==4)
											LCD1602_Cursor(0x0F,0x80+8);
										if(ClockWch==5)
											LCD1602_Cursor(0x0F,0x80+12);
										if(ClockWch==6)
											LCD1602_Cursor(0x0F,0xC0);
										if(ClockWch==7)
											LCD1602_Cursor(0x0F,0xC0+4);
										if(ClockWch==8)
											LCD1602_Cursor(0x0F,0xC0+8);	
										if(ClockWch==0)
											LCD1602_Cursor(0x0F,0xC0+13);
										if(ClockWch==1)
											LCD1602_Cursor(0x0F,0xC0+14);
										if(flag==1)								//退出显示主菜单 
										{
											flag=0;
											ClockWch=0;							//位置选择置0  
											LCD1602_WriteCom(0x01);
											LCD1602_WriteCom(0x80);
											for(t=0;t<9;t++)
												LCD1602_WriteData(timeSet[t]);
											LCD1602_WriteCom(0xC0);
											for(t=0;t<10;t++)
												LCD1602_WriteData(alarmSet[t]);
											if(function==0)
												LCD1602_Cursor(0x0F,0x80);
											if(function==1)
												LCD1602_Cursor(0x0F,0xC0);
											break;
										 } 
									}
								}
								break;	
					case K2:	function++;	 									 //功能选择
								if(function>1)
									function=0;	
								if(function==0)
									LCD1602_Cursor(0x0F,0x80);
								if(function==1)
									LCD1602_Cursor(0x0F,0xC0);							
								break;
					case K3:    function--;	 									 //功能选择
								if(function<0)
									function=1;
								if(function==0)
									LCD1602_Cursor(0x0F,0x80);
								if(function==1)
									LCD1602_Cursor(0x0F,0xC0);
								break;
					case K4:	flag=1;break;	  //退出设置
				
					}
				 }
				 if(flag==1)										  //退出开主显示界面
				 {
				 			LCD1602_DisplayTemp(DS18B20_ReadTemp());
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
						//	delay_ms(1000);
							flag=0;
							break;
				 }
			 }
			 LCD1602_WriteCom(0x0C);				  //取消光标
			 break;
		case MODE1:		hour12_24=!hour12_24;break;	  //更改时制
							
		case MODE2:		if(hour12_24==1)
						{
						AM_PM=!AM_PM;
						if(AM_PM==1)
						{
							if(TIME[2]==0x08)
								TIME[2]=0x20;
							else if(TIME[2]==0x09)
								TIME[2]=0x21;
							else
								TIME[2]+=0x12;
							DS1302_Init();
						}
						
						if(AM_PM==0)
						{
							if(TIME[2]==0x20)
								TIME[2]=0x08;
							else if(TIME[2]==0x21)
								TIME[2]=0x09;
							else
								TIME[2]-=0x12;
							DS1302_Init();
						}
						}
						break;	//无
		case MODE3:		break;	//开关闹钟
	}
}
