#include <STC15F2K60S2.h>
#include <delay.h>
#include <intrins.h>	
#include "lcd.h"
#include <string.h>
sbit K1=P3^4;
sbit K2=P3^5;
sbit INPUT1=P0^4;
sbit INPUT2=P0^5;
sbit INPUT3=P0^6;
sbit INPUT4=P0^7;
sbit LED0=P0^0;
sbit LED1=P0^1;
sbit LED2=P0^2;
sbit LED3=P0^3;
sbit PWM1 = P1^0;
sbit PWM2 = P1^1;
sbit Trig = P3^6;
sbit Echo = P3^7;
unsigned char t;
unsigned char num1;
unsigned char CYCLE;
unsigned char PWM_ON ;
unsigned char ReceiveData;
//chaoshengbo
unsigned char PuZh[]=" Pechin Science ";
unsigned char code ASCII[15] =    {'0','1','2','3','4','5','6','7','8','9','.','-','M'};

static unsigned char DisNum = 0; //显示用指针				  
       unsigned int  time=0;
	   unsigned long S=0;
	   bit      flag =0;
	   unsigned char disbuff[4]	   ={ 0,0,0,0,};

void UART_Init()
{		
		//串口1	初始化
    SCON = 0x50;   
    AUXR &= 0xBE;  
    AUXR |= 0x00;    
    TMOD &= 0x0F;	
    TMOD |= 0x20;           
    TH1 = 0xfd;                 
    TL1 = 0xfd;                 
    TR1 = 1;                         //定时器1启动
    ES = 1;                         //开放串口中断
    EA = 1;	
 
        	
	                         
}


void Conut(void)
	
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/100;     //算出来是CM
	 if((S>=700)||flag==1) //超出测量范围显示“-”
	 {	 
	  flag=0;
	 
	  DisplayOneChar(0, 1, ASCII[11]);
	  DisplayOneChar(1, 1, ASCII[10]);	//显示点
	  DisplayOneChar(2, 1, ASCII[11]);
	  DisplayOneChar(3, 1, ASCII[11]);
	  DisplayOneChar(4, 1, ASCII[12]);	//显示M
	 }
	 else
	 {
	  disbuff[0]=S%1000/100;
	  disbuff[1]=S%1000%100/10;
	  disbuff[2]=S%1000%10 %10;
	  DisplayOneChar(0, 1, ASCII[disbuff[0]]);
	  DisplayOneChar(1, 1, ASCII[10]);	//显示点
	  DisplayOneChar(2, 1, ASCII[disbuff[1]]);
	  DisplayOneChar(3, 1, ASCII[disbuff[2]]);
	  DisplayOneChar(4, 1, ASCII[12]);	//显示M
	 }
	}


	void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
  {
    flag=1;							 //中断溢出标志
  }

	
 void  StartModule() 		         //启动模块
  {
	  Trig=1;			                     //启动一次模块
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  Trig=0;
  }
//前进/
void forward()
{
	    INPUT1=1;INPUT2=0;INPUT3=1;INPUT4=0;LED0=0;LED1=1;LED2=1;LED3=1;num1=1;t=0;
}

//后退/
void back()
{
	INPUT1=0;INPUT2=1;INPUT3=0;INPUT4=1;LED1=0;LED0=1;LED2=1;LED3=1;num1=0;t=0;
}

//左 
void left()
{
	INPUT1=1;INPUT2=0;INPUT3=0;INPUT4=0;num1=0;LED2=0;LED0=1;LED1=1;LED3=1;t=0;
		mDelay(2000);
					forward();
}

//右
void right()
{
	INPUT1=0;INPUT2=0;INPUT3=1;INPUT4=0;num1=0;LED3=0;LED0=1;LED1=1;LED2=1;t=0;
			mDelay(2000);
					forward();
	
}
//停
void stop()
{INPUT1=0;INPUT2=0;INPUT3=0;INPUT4=0;LED1=1;LED0=1;LED2=1;LED3=1;num1=0;t=0;}


void delayms(unsigned int j)
{
	uchar i=250;
	for(;j>0;j--)
	{
		while(--i);
		i=249;
		while(--i);
		i=250;
 
	}
}
void delay(unsigned int cnt)
{
while(--cnt);
}

		 

 void main(){
	 
	 bit Flag;
	 TMOD=0X01;
	 TH0=0;
	 TL0=0;          
	 ET0=1;             //允许T0中断
   EA=1;
 
  UART_Init(); //串口初始化
	LcdInit();
	LcdShowStr(0,0,PuZh);

while(1){



	   StartModule();
	     while(!Echo);		//当RX为零时等待
	     TR0=1;			    //开启计数
	     while(Echo);			//当RX为1计数并等待
	     TR0=0;				//关闭计数
         Conut();			//计算
		 delayms(10);
	if(S<=80&&INPUT1==1&&INPUT3==1&&num1)
	{	INPUT1=1;INPUT2=0;INPUT3=0;INPUT4=0;t=1;
	  mDelay(1000);
	     StartModule();
	     while(!Echo);		//当RX为零时等待
	     TR0=1;			    //开启计数
	     while(Echo);			//当RX为1计数并等待
	     TR0=0;				//关闭计数
       Conut();			//计算
		   delayms(10);
	}
//	if(S<S1&&t==1)
//	{
//	  INPUT1=0;INPUT2=0;INPUT3=1;INPUT4=0;FX=0;
//	  mDelay(4000);
//	}
//	if(S>S1&&t==1)
//	{
//	  INPUT1=1;INPUT2=0;INPUT3=0;INPUT4=0;FX=1;
//	  mDelay(3200);
//	}
	
	if(S>50&&t==1){
		forward();
	  mDelay(2000);
 		INPUT1=0;INPUT2=0;INPUT3=1;INPUT4=0;	
		mDelay(1500);
    forward();
	  t=0;
		}
 }
}

void UART1_Routine()interrupt 4
{
  
	
  EA = 0;
	
  if(RI == 1) 
	{ 		
		RI = 0;
		INPUT1=0;INPUT2=0;INPUT3=0;INPUT4=0;
		ReceiveData= SBUF;
			
		if(ReceiveData== '1')	 
		{
				forward();
		}
		
		else if(ReceiveData== '2')
		{
				back();
		}
		
		
			else if(ReceiveData== '3')
		{
			left();
		}
		
		
			else if(ReceiveData== '4')
		{
				right(); 
		}
		
		
		else if(ReceiveData== '0')
		{
				stop();
		}
		
		
}
		
		
		EA = 1;

}


