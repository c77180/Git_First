#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

void RS232dis_Config(void)//����������
{
	_U2EIE=0;	//U1,�����ж���������������жϸ�λ
	_U2TXIE=0;	//U1�������ж�����
	_U2RXIE=0;	//U1�������ж�����
	
	U2MODE=0x8000;//���������������ţ�8���ݣ�1ֹͣ����У��
//	U2MODE=0x8002;//���������������ţ�8���ݣ�1ֹͣ��żУ��
//	U2BRG=0x103;   //9600    40M/16/9600-1
	U2BRG=21;   //115200   40M/16/115200-1
	
//	U1BRG=0x81;   //19200    40M/16/19200-1
	
//	U1STA=0x0400; //�����жϱ�־������ʹ��
	U2STA=0x0400; //�����жϱ�־������ʹ��
}

void RS232dis_Sdata(void)//����
{	
	while(RS232dis_Sfifo==0) //��������������������
		U2TXREG=0xaa;	
}	

void RS232dis_Rdata(void)//��������ݾͶ�����
{
	unsigned int i=0,j=0,data[10];	
	while(1)
	{
			if(RS232dis_Rfifo==1) 
			{
				data[j++]=U2RXREG;
				if(j==10) 
				i++;//��ϵ㣬���յ��������Ƿ���ȷ
			}	
	}
}	

















