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

static unsigned char DisNum = 0; //��ʾ��ָ��				  
       unsigned int  time=0;
	   unsigned long S=0;
	   bit      flag =0;
	   unsigned char disbuff[4]	   ={ 0,0,0,0,};

void UART_Init()
{		
		//����1	��ʼ��
    SCON = 0x50;   
    AUXR &= 0xBE;  
    AUXR |= 0x00;    
    TMOD &= 0x0F;	
    TMOD |= 0x20;           
    TH1 = 0xfd;                 
    TL1 = 0xfd;                 
    TR1 = 1;                         //��ʱ��1����
    ES = 1;                         //���Ŵ����ж�
    EA = 1;	
 
        	
	                         
}


void Conut(void)
	
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/100;     //�������CM
	 if((S>=700)||flag==1) //����������Χ��ʾ��-��
	 {	 
	  flag=0;
	 
	  DisplayOneChar(0, 1, ASCII[11]);
	  DisplayOneChar(1, 1, ASCII[10]);	//��ʾ��
	  DisplayOneChar(2, 1, ASCII[11]);
	  DisplayOneChar(3, 1, ASCII[11]);
	  DisplayOneChar(4, 1, ASCII[12]);	//��ʾM
	 }
	 else
	 {
	  disbuff[0]=S%1000/100;
	  disbuff[1]=S%1000%100/10;
	  disbuff[2]=S%1000%10 %10;
	  DisplayOneChar(0, 1, ASCII[disbuff[0]]);
	  DisplayOneChar(1, 1, ASCII[10]);	//��ʾ��
	  DisplayOneChar(2, 1, ASCII[disbuff[1]]);
	  DisplayOneChar(3, 1, ASCII[disbuff[2]]);
	  DisplayOneChar(4, 1, ASCII[12]);	//��ʾM
	 }
	}


	void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
  {
    flag=1;							 //�ж������־
  }

	
 void  StartModule() 		         //����ģ��
  {
	  Trig=1;			                     //����һ��ģ��
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
//ǰ��/
void forward()
{
	    INPUT1=1;INPUT2=0;INPUT3=1;INPUT4=0;LED0=0;LED1=1;LED2=1;LED3=1;num1=1;t=0;
}

//����/
void back()
{
	INPUT1=0;INPUT2=1;INPUT3=0;INPUT4=1;LED1=0;LED0=1;LED2=1;LED3=1;num1=0;t=0;
}

//�� 
void left()
{
	INPUT1=1;INPUT2=0;INPUT3=0;INPUT4=0;num1=0;LED2=0;LED0=1;LED1=1;LED3=1;t=0;
		mDelay(2000);
					forward();
}

//��
void right()
{
	INPUT1=0;INPUT2=0;INPUT3=1;INPUT4=0;num1=0;LED3=0;LED0=1;LED1=1;LED2=1;t=0;
			mDelay(2000);
					forward();
	
}
//ͣ
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
	 ET0=1;             //����T0�ж�
   EA=1;
 
  UART_Init(); //���ڳ�ʼ��
	LcdInit();
	LcdShowStr(0,0,PuZh);

while(1){



	   StartModule();
	     while(!Echo);		//��RXΪ��ʱ�ȴ�
	     TR0=1;			    //��������
	     while(Echo);			//��RXΪ1�������ȴ�
	     TR0=0;				//�رռ���
         Conut();			//����
		 delayms(10);
	if(S<=80&&INPUT1==1&&INPUT3==1&&num1)
	{	INPUT1=1;INPUT2=0;INPUT3=0;INPUT4=0;t=1;
	  mDelay(1000);
	     StartModule();
	     while(!Echo);		//��RXΪ��ʱ�ȴ�
	     TR0=1;			    //��������
	     while(Echo);			//��RXΪ1�������ȴ�
	     TR0=0;				//�رռ���
       Conut();			//����
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


