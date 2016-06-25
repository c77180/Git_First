#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

void Init_data_buffer(void)
{
	unsigned int i,j;
	
	for(i=0;i<7;i++)
	{
		for(j=0;j<3;j++)
		{
			WIRE_cost_buffer[i][j][20]=0;
			WIRE_cost_buffer[i][j][21]=0;
		}
	}	
	
	for(i=0;i<9;i++)
	{
		for(j=0;j<3;j++)
		{
			LOAD_cost_buffer[i][j][20]=0;
			LOAD_cost_buffer[i][j][21]=0;
		}
	}
	
	for(i=0;i<2;i++)
	 for(j=0;j<64;j++)
	   zero_wave[i][j]=0;//��������
}	

void Init_array(void)//��ʼ������
{
	unsigned int i,j;
	
	Init_data_buffer();
#if compilemodbus
	modbus_init();
#endif
	for(i=0;i<8;i++)//�豸����״̬
	{
		equipment_skate[i][0]=0;
		equipment_skate[i][1]=0;
		equipment_skate[i][2]=3;
		equipment_skate[i][3]=0;
	}	
	
	for(i=0;i<12;i++)//���γ�ʼ��
		for(j=0;j<64;j++)
			record_wave[i][j]=0;	
	
	for(i=0;i<7;i++)//ϵͳ���ݳ�ʼ��
		for(j=0;j<3;j++)
			WIRE_cost[i][j]=0;
			
	for(i=0;i<8;i++)//�������ݳ�ʼ��
		for(j=0;j<3;j++)
			LOAD_cost[i][j]=0;
	
	equiment_set[2]=P_E2PROM[13];//һ�����ٸ����ص�ַ  �����ʱ����
}	

