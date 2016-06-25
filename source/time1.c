#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

volatile unsigned long DELAY_TIME;
volatile unsigned long DELAY_SLEEP;//ÿ�೤ʱ�����һ�α��ⴥ��
volatile unsigned long DELAY_Blight;//����
volatile unsigned long DELAY_ASKTIME=500000;//ÿ�೤ʱ��ѯ��һ�µ�ǰʱ��
volatile unsigned long DELAY_DSIskate;//״̬��ʾ
volatile unsigned long DELAY_CAN[8];//CANͨѶ��ʱ
volatile unsigned long DELAY_run_time=0;//�����豸����ʱ��
volatile unsigned long DELAY_refresh_data;//����ˢ��ʱ��
volatile unsigned long DELAY_GSM;//������Ϣ��ʱ
volatile unsigned long DELAY_send_message;//������Ϣ����ʱ����ʱ
volatile unsigned long DELAY_message;
volatile unsigned long DELAY_fist_page=0;
volatile unsigned long DELAY_GSM_error;
volatile unsigned long DELAY_time_test;
volatile unsigned long DELAY_elect_synch;//ͬ��ʱ�����
volatile unsigned int  MODBUS_TIME;
volatile unsigned long DELAY_GSM_NUM=0;//�ϵ緢�Ͷ������ĺ�����ʱ

volatile unsigned long  DELAY2_time_test;//****
//������*******
volatile unsigned long DELAY_refresh_data_com;

volatile unsigned int run_time_sign=0;//����ʱ��ı��ʶ


void time1_config(unsigned int time)//us ���177us
{	
	
	PR1=time*40;;//1us ��Ҫ���ٸ���ʱ������
	
	_TSIDL=0;//���м�������
	_TGATE=0;//��ֹ�ſ�
	_TCKPS=0;//��Ƶ1:1
	_TSYNC=0;//��ͬ���ⲿʱ��
	_TCS=0;   //ʹ���ڲ�ʱ��	
	
	//�ж�����
	
	_T1IE=1;//�����ж�
	_T1IP=7;//���ȼ����
	
}

void time2_config(unsigned int time)//us ���
{	
	
	PR2=time*40;//1us ��Ҫ���ٸ���ʱ������
	
	
	T2CONbits.T32=0; //����16λ
	T2CONbits.TCKPS=0;//��Ƶ1:1
	T2CONbits.TCS=0;//�ڲ�ʱ��
	T2CONbits.TGATE=0;//��ֹ�ſ�
	T2CONbits.TSIDL=0;//���м�������	
	//�ж�����
	
	_T2IE=1;//�����ж�
	_T2IP=4;//���ȼ��ϵ�
	
	T2CONbits.TON=1;//ֱ������
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)//��ʱ��1 �ж�
{
	unsigned int m;
		DELAY_TIME++;
		DELAY_SLEEP++; //������ѯ��������ʱ
		DELAY_Blight++;//����ʱ��
		DELAY_ASKTIME++;
		DELAY_DSIskate++;
		for(m=0;m<8;m++) DELAY_CAN[m]++;//CANͨѶ��ʱ
		DELAY_run_time++;//��������ʱ��
		DELAY_refresh_data++;
		DELAY_GSM++;
		DELAY_send_message++;
		DELAY_message++;
		DELAY_fist_page++;
		DELAY_GSM_error++;
		DELAY_elect_synch++;
		MODBUS_TIME++;
		DELAY_GSM_NUM++;
		
		DELAY_time_test++;
		
//������*******
		DELAY_refresh_data_com++;//ˢ��ʱ��
		
//****************ι��**********************************************
		clearWdog();
		
//**********modbus**************************************************
//		modbus_in();

//********������ָ�����********************************************
		RS232_Slcd();
		RS232_Rlcd();

//*******************GSM����è����*************************************
#if compileGSM
                RS232_Rdata();//����
		RS232_Sdata();
#endif
//********����ʱ��**************************************************		
		if(DELAY_run_time>=600000)//100us*600000=60s
		{					
			if(APF_state==1)//����豸��������
			{
				P_E2PROM[3]++;

				if(P_E2PROM[3]>=1440)
				{
					P_E2PROM[4]++;
					P_E2PROM[3]=0;
				}				
				run_time_sign++;//˵������ʱ��仯��
			}
			
			if(((equipment_skate[0][3]>>2)&0x01)==1)
			{
				P_E2PROM[111]++;

				if(P_E2PROM[111]>=1440)
				{
					P_E2PROM[112]++;
					P_E2PROM[111]=0;
				}
				run_time_sign++;//˵��ʱ��仯��	
			}
	
			DELAY_run_time=0;	
		 }
			
	IFS0bits.T1IF=0;//���㣬׼����һ���ж�	
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)//��ʱ��1 �ж�
{
	DELAY2_time_test++;
#if compilemodbus
	modbus_in();

	modbus_Shandle();
#endif
	IFS0bits.T2IF=0;
} 

