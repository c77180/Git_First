#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

//#define SPI_send_over SPI1STATbits.SPITBF   //���ͻ�������״̬1
//#define SPI_receive_over SPI1STATbits.SPIRBF  //���ջ�������״̬1

volatile unsigned int CH432T_reset=0;// Ϊ1 �򴮿ڸ�λ
volatile unsigned int RS485_receive=0;// DATARDY Ϊ1 ��˵�������ݿ��Զ���  0���յ�����
volatile unsigned int RS485_send_fifo=1;// THRE 1���Լ�����fifo�������ݣ�0,fifo���� 16��
volatile unsigned int RS485_send_over=1;// TEMT 1����ȫ�����꣬ 0
volatile unsigned int BSP_reset=0;

volatile unsigned int  MODBUS_T1_5=20;  
volatile unsigned int  MODBUS_T3_5=40;  

void RS485_Config(void)
{
//*****SPI����**********
	SPI1CON1bits.SPRE=7; //����Ƶ1:1
	SPI1CON1bits.PPRE=2; //����Ƶ 4:1 //10M ������

	SPI1CON1bits.MSTEN=1;//����ģʽʱ��
	SPI1CON1bits.MODE16=1; //16λ����
	SPI1CON1bits.CKE=1;//
	SPI1CON1bits.SMP=1;
	
	SPI1STATbits.SPIROV=0;//��������־
	SPI1STATbits.SPIEN=1;//ʹ������
	
//*********���ſ���*****************	
	_TRISB2=0;
	
//****SPI�жϽ�ֹ*******
	_SPI1IE=0;
	_SPI1IP=0;
}

void CH432T_config(void)//��ʼ�� CH432T
{
	unsigned int i;	
	unsigned int sp,r_bit;
	unsigned long num=115200,speed;
	
	speed=P_E2PROM[117];
	speed<<=16;
	speed+=P_E2PROM[118];	
	sp=num/speed;
	
	if(speed<=19200)
	{
		num=165000;	
		MODBUS_T1_5=num/speed;
		
		num=385000;	
		MODBUS_T3_5=num/speed;
	}
	else
	{
		MODBUS_T1_5=8;
		MODBUS_T3_5=18;	
	}	
	
	
	  if(P_E2PROM[119]==0)    r_bit=0x07;
	else if(P_E2PROM[119]==1) r_bit=0x0b;
	else 				      r_bit=0x1b;

			while(SPI1STATbits.SPITBF==1);
			SS1=0;
			SPI1BUF=CN_IER_reset;
			while(SPI1STATbits.SPIRBF==0);
			i=SPI1BUF;
			SS1=1;
			
			while(SPI1STATbits.SPITBF==1);
			SS1=0;
			SPI1BUF=CN_LCR_write_hz|r_bit;
			while(SPI1STATbits.SPIRBF==0);
			i=SPI1BUF;
			SS1=1;
			
			while(SPI1STATbits.SPITBF==1);
			SS1=0;
			SPI1BUF=CN_DLL|(sp&0xff);
			while(SPI1STATbits.SPIRBF==0);
			i=SPI1BUF;
			SS1=1;
			
			while(SPI1STATbits.SPITBF==1);
			SS1=0;
			SPI1BUF=CN_DLM|(sp>>8);
			while(SPI1STATbits.SPIRBF==0);
			i=SPI1BUF;
			SS1=1;	
			
			while(SPI1STATbits.SPITBF==1);
			SS1=0;
			SPI1BUF=CN_LCR_write|r_bit;
			while(SPI1STATbits.SPIRBF==0);
			i=SPI1BUF;
			SS1=1;
			
			while(SPI1STATbits.SPITBF==1);
			SS1=0;
			SPI1BUF=CN_FCR_write;
			while(SPI1STATbits.SPIRBF==0);
			i=SPI1BUF;
			SS1=1;	
}	

