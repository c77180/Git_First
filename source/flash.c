#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"
#include "..\include\string_h.h"


#define K9F_CLE   LATDbits.LATD13   //��������ʹ��
#define K9F_ALE   LATDbits.LATD12	 //��ַ����ʹ��
#define K9F_CE	   LATCbits.LATC13   //оƬʹ��
#define K9F_RE	   LATGbits.LATG2   //��дʹ��
#define K9F_WE	   LATGbits.LATG3   //дʹ��ʵ��

#define K9F_RB	   PORTAbits.RA2  //��æ DSPIC����

#define  K9F_O_data    LATDbits.LATD0_7  //DSPIC���
#define  K9F_I_data    PORTDbits.RD0_7   //DSPIC����
#define  K9F_Idata 	 TRISDbits.TRISD0_7=0xff;//��K9F��DSPIC
#define  K9F_Odata   TRISDbits.TRISD0_7=0x00;//��DSPIC��K9F


unsigned char flash_buffer[512];

void K9F1208_Config(void)//flash ��ʼ��
{
	_TRISD13=0;
	_TRISD12=0;
	_TRISC13=0;
	_TRISG2=0;
	_TRISG3=0;	
	_TRISA2=1;
	
	K9F_CE=1;
	K9F_RE=1;
	K9F_WE=1;
	K9F_CLE=0;
	K9F_ALE=0;
}	

void K9F1208_Wdata(unsigned char data)//д����
{
	K9F_Odata;//���
	K9F_CLE=0;
	K9F_ALE=0;
	K9F_WE=0;
	K9F_O_data=data;
	Nop();
	K9F_WE=1;
}	

unsigned char K9F1208_Rdata(void)//������
{
	unsigned char data;
	K9F_Idata;//����
	K9F_CLE=0;
	K9F_ALE=0;
	K9F_RE=1;	
	K9F_RE=0;
	Nop();Nop();
	data=K9F_I_data;
	
	return data;
}	

void K9F1208_Wcmd(unsigned char cmd)//д����
{
	K9F_Odata;//���
	K9F_CLE=1;
	K9F_ALE=0;
	K9F_RE=1;
	K9F_WE=0;
	K9F_O_data=cmd;
	Nop();
	K9F_WE=1;	
}

void K9F1208_Wadd(unsigned long page,unsigned char byte)//д��ַ
{
	unsigned int i;
	K9F_Odata;//���
	K9F_CLE=0;
	K9F_ALE=1;
	
	K9F_WE=0;
	K9F_O_data=byte;
	Nop();
	K9F_WE=1;

	for(i=0;i<3;i++)
	{
			K9F_ALE=0;
			K9F_ALE=1;
			K9F_WE=0;
			K9F_O_data=page & 0xff;
			page>>=8;
			K9F_WE=1;
	}
}

void K9F1208_Wadd_block(unsigned int addr)//д������ַ
{
	unsigned int i;
	K9F_Odata;//���
	K9F_CLE=0;
	K9F_ALE=1;
	
	for(i=0;i<3;i++)
	{			
			K9F_ALE=1;
			K9F_WE=0;
			K9F_O_data=addr & 0xff;
			addr>>=8;
			K9F_WE=1;
			K9F_ALE=0;
	}
}


void K9F1208_Wpage(unsigned long page,unsigned int num,unsigned char *character)//дһҳ
{
	unsigned int i;
		
	K9F_CE=0;
	K9F1208_Wcmd(0x80);
	K9F1208_Wadd(page,0);
	
	for(i=0;i<num;i++)
		K9F1208_Wdata(*(character+i));
	
	K9F1208_Wcmd(0x10);//����
	
	while(K9F_RB==0);  //�ȴ�д�꣬������Դ���ɲ��е�
	
	K9F_CE=1;
}



void K9F1208_Rpage(unsigned long page,unsigned int byte,unsigned int num,unsigned char *character)//��һҳ
{
	unsigned int i;
	unsigned char data;
	
	K9F_CE=0;
	
	if(byte<256) K9F1208_Wcmd(0x00);
	else         K9F1208_Wcmd(0x01);
		
	data=byte;
	K9F1208_Wadd(page,data);	
	while(K9F_RB==0);
		
	for(i=0;i<num;i++)
			*(character++)=K9F1208_Rdata();	
	K9F_CE=1;
	K9F_Odata;
	K9F_RE=1;
}

void erase_block(unsigned long BLOCK)//������(0--4095)
{
	K9F_CE=0;
	K9F1208_Wcmd(0x60);
	K9F1208_Wadd_block(BLOCK);
	K9F1208_Wcmd(0xD0);
	while(K9F_RB==0);
	K9F_CE=1;
}


void erase_block_all(void)//ɾ�����п�
{
	unsigned int i;	
	for(i=0;i<4096;i++)
		erase_block(i*32);		
}	

