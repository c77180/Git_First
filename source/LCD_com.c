#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

volatile unsigned int LCD_page_sign=0;//��ʼ��ʱ��ʲô������ʾ
volatile unsigned int LCD_page_dis=0;//�Ƿ��һ�ν���
volatile unsigned int LCD_page_dis_b=0;//��������
volatile unsigned int LCD_page_dis_bb=0;//��������
volatile unsigned int LCD_page_dis_bbb=0;//��������
volatile unsigned int LCD_page_dis_bbbb=0;//��������
volatile int *LCD_page_array;             //��ַ
volatile unsigned int LCD_page101=0;
volatile unsigned int LCD_page102=0;
volatile unsigned int LCD_page103=0;
volatile unsigned int LCD_page201=0;
volatile unsigned int LCD_page202=0;
volatile unsigned int LCD_page203=0;
volatile unsigned int LCD_page204=0;
volatile unsigned int LCD_page205=0;
volatile unsigned int LCD_page206=0;
volatile unsigned int LCD_page301=0;
volatile unsigned int LCD_page401=0;
volatile unsigned int LCD_page501=0;
volatile unsigned int LCD_page502=0;
volatile unsigned int LCD_page503=0;
volatile unsigned int LCD_page504=0;
volatile unsigned int LCD_page505=0;
volatile unsigned int LCD_page601=0;
volatile unsigned int LCD_page602=0;
volatile unsigned int LCD_page603=0;
volatile unsigned int LCD_page604=0;
volatile unsigned int LCD_page605=0;
volatile unsigned int LCD_page606=0;
volatile unsigned int LCD_page701=0;
volatile unsigned int LCD_page702=0;
volatile unsigned int LCD_page703=0;

volatile unsigned int LCD_Ddata_buffer[255];//���ڲ�����ʾ����ʹ��
volatile unsigned char __attribute__((__far__))  LCD_Ddata_tran[20];//���ڹ���ʹ��
volatile unsigned int KEY_com=0;//���ܰ�ť

volatile unsigned int ERROR_num;//���ϵ���Ŀ���ڹ�����Ŀ�ı��ʱ����ʾҪ��֮�ı� ������

volatile unsigned int change_data;//���ݸı�ʱ���±���
volatile unsigned int change_data_h;//�������ذ�ť��ˢ�±��
volatile unsigned int change_data_only;//ֻ������һ���ط�

volatile unsigned int message_num=0;//��ʾ��Ϣ�������е�λ��

volatile unsigned int LED_state=1;//���⵱ǰ��״̬���ϵ�Ĭ��Ϊ�ǿ���;

volatile unsigned int equipment_skate_sign=100;//�ϵ�������жϵ�ǰ״̬

volatile unsigned int RUN_stop=0;//����ֹͣ���е�ҳ��

volatile unsigned int TIME_NOW[]={0,0,0,0,0,1};//��ǰʱ�� 0:�꣬1���£�2�գ�3����  4ʱ 5��

volatile unsigned int fault_data_num;//��ǰ��ʾ�Ĺ��ϴ���ĵ�ַ,

volatile unsigned long password_one;//��һ������
volatile unsigned long password_two;//�ڶ�������

volatile unsigned char K_num;//���뿪��״̬

volatile unsigned int  his_type[4]={1,0,0,0};

volatile unsigned int  equipment_activate=0;//��ǰ������豸��@@@@

volatile unsigned int  ALL_cap;

volatile unsigned int __attribute__((__far__)) error_addr[501],error_four_data[4][10]; 
volatile unsigned int __attribute__((__far__)) error_dis,error_dis_last;
volatile unsigned int __attribute__((__far__)) error_dis_now[5];
volatile unsigned int lcd_version=0;//&&&&�������ں˰汾  
//*****�ײ㷢�ͽ���**********************************************
void RS232_Slcd(void)//�жϣ�����lcdָ��֡�����ͽ�����ʶ��һ
{
	while(RS232dis_Sfifo==0&&LCD_Tbuffer_now<LCD_Tbuffer_num) //��������������������
	{
		U2TXREG=LCD_Tbuffer[LCD_Tbuffer_now++];
		if(LCD_Tbuffer_now==LCD_Tbuffer_num)
		{
			LCD_Tbuffer_now=0;
			LCD_Tbuffer_num=0;
			LCD_Tbuffer_sign=1;//���ͽ������ⲿ����
		}
	}
}

void RS232_Rlcd(void)//�жϣ�����LCDָ��֡�����ս�����ʶ��һ
{
	while(RS232dis_Rfifo==1)
	{
		LCD_Rbuffer[LCD_Rbuffer_now++]=U2RXREG;
		
		if(LCD_Rbuffer_now==1)//��һ���յ������ݱ�����AA
		{
			if(LCD_Rbuffer[0]!=0xcc) LCD_Rbuffer_now=0; //�����CCҲ���Դ�������
		}
		else if(LCD_Rbuffer_now==2)//��һ���յ������ݱ�����AA
		{
			if(LCD_Rbuffer[1]==0xdd) LCD_Rbuffer_sign=0;//���忪ʼ�����ݣ����ܴ����ϴ�������
			else					 LCD_Rbuffer_now=0; 
		}
		else if(LCD_Rbuffer_now>=3)
		{
			if(LCD_Rbuffer_now-3>=LCD_Rbuffer[2])//���ڼ���
			{
				LCD_Rbuffer_sign=1;//��ʾ���Զ�ȡ������
				LCD_Rbuffer_now=0;//������������½���			
			}	
			else
			if(LCD_Rbuffer[LCD_Rbuffer_now-1]==0xdd&&LCD_Rbuffer[LCD_Rbuffer_now-2]==0xcc)//��ֹ�����ж�
			{
				LCD_Rbuffer[0]=0xcc;
				LCD_Rbuffer[1]=0xdd;
				LCD_Rbuffer_sign=0;//�����յ�������ȡ��
				LCD_Rbuffer_now=2;//�յ���������	
			}
		}
	}
}
//*****************���մ���**************************************************************************

void button_e2prom(unsigned int e2_num,volatile unsigned char *character)//�����ݴ���E2
{
	unsigned int tran_data;
	
		tran_data=character[0];
		tran_data=(tran_data<<8)|character[1];
		P_E2PROM[e2_num]=tran_data;
		E2ROM_WR(e2_num,1,P_E2PROM+e2_num);	
}
void button_e2prom_diff(unsigned int P_E2,unsigned int e2_num,volatile unsigned char *character)//�����ݴ���E2
{
	unsigned int tran_data;
	
		tran_data=character[0];
		tran_data=(tran_data<<8)|character[1];
		P_E2PROM[P_E2]=tran_data;
		E2ROM_WR(e2_num,1,P_E2PROM+P_E2);	
}

unsigned int button_return(volatile unsigned char *character,const int *range)//����һ������
{
	int tran_data;
	unsigned int data;
	
		data=character[0];
		data=(data<<8)|character[1];
		tran_data=data;//��ֵ���з�����		
		
		if(tran_data<range[0]){tran_data=range[0];message_num=5;KEY_com=7;}//��ʾ���ֵ
		else
		if(tran_data>range[1]){tran_data=range[1];message_num=4;KEY_com=7;}//��ʾ���ֵ
		
		return tran_data;
}

void RS232_Rlcd_button(void)//����ת����
{								 //����յ������ݲ����򷵻���
	unsigned int tran_data,tran_data1;
	unsigned long long_data;
	unsigned int KEY_mode,i,j;
	
	if(LCD_Rbuffer_sign==1)//�յ������ˣ�����
	{		
		if(LCD_Rbuffer[3]==0x83)
		{		
			KEY_mode=LCD_Rbuffer[4];
			KEY_mode=(KEY_mode<<8)|LCD_Rbuffer[5];//����
//***********ҳ��֮���л��ļ���********************************
			if(KEY_mode==0x1000)//������ϴ��ļ���
			{
				 DELAY_message=100000;//�����ʾ������
				 
				 if(LCD_Rbuffer[8]==0)//�Ҳఴť ��λ�Ƿ�Ϊ��
				 {
					 LCD_page_sign=KEY_main_page[LCD_Rbuffer[7]-1];//���ص��ϴ����һ�εĽ���
					 LCD_page_dis=0;
				 }
				 else
				 if(LCD_Rbuffer[8]<0x10)//�ڲ��л���ť
				 {
					 LCD_page_sign=LCD_Rbuffer[7];
					 LCD_page_sign=(LCD_page_sign<<8)|LCD_Rbuffer[8];
					 KEY_main_page[LCD_Rbuffer[7]-1]=LCD_page_sign;
					 LCD_page_dis=0;
				 }
				 else//����������ť
				 {
			 
				 }
			}
			else
//***********��Ҫ�������������˵�������********************************
			if(KEY_mode==0x1001)//����ǽ������ò˵� 
			{
				 	DELAY_message=100000;//�����ʾ������
				 	
				 	tran_data=LCD_Rbuffer[7];
					tran_data=(tran_data<<8)|LCD_Rbuffer[8];
					
					tran_data1=LCD_Rbuffer[9];
					tran_data1=(tran_data1<<8)|LCD_Rbuffer[10];
					
					if(P_E2PROM[103]==0)//����Ѿ�����
					{
						password_one=P_E2PROM[23];
						password_one*=1000000;
						long_data=P_E2PROM[24];
						long_data*=10000;
						password_one=password_one+long_data+P_E2PROM[25]+P_E2PROM[23]+P_E2PROM[24];
						
						
						password_two=P_E2PROM[23];
						password_two*=10000000;
						long_data=P_E2PROM[25];
						long_data*=10000;
						password_two=password_two+long_data+P_E2PROM[25]+P_E2PROM[23]+P_E2PROM[24]-111;
	
						if(tran_data==0&&tran_data1==1234)//������������һ�� ����һ��
						{
							LCD_page_sign=KEY_main_page[4];//���ص��ϴ����һ�εĽ���
						 	LCD_page_dis=0;
						}
                     
						else if(tran_data==(password_one>>16)&&tran_data1==(password_one&0xffff))//������������һ�£��������84201378 0x0504CFA2
						{
							LCD_page_sign=KEY_main_page[5];//���ص��ϴ����һ�εĽ���
						 	LCD_page_dis=0;
						}
						else
						if(tran_data==(password_two>>16)&&tran_data1==(password_two&0xffff))//������������һ�£���������3166549527 0xBCBD B617
						{
							LCD_page_sign=KEY_main_page[6];//���ص��ϴ����һ�εĽ���
						 	LCD_page_dis=0;
						}
						else
						{
							message_num=1;//��ʾ�����������	
						}
					}
					else//���δ����
					{
						  DELAY_message=0;//������ַ�
						  password_one=P_E2PROM[23]; //11
						  password_one+=P_E2PROM[24];//1
						  password_one+=P_E2PROM[25];//1						  
						  password_two=P_E2PROM[25];
						  password_one=(password_one+123)*10000+password_one+456+password_two*1234+password_two*234+password_two*34;
						  
						  if(tran_data==(password_one>>16)&&tran_data1==(password_one&0xffff))
						  {
							  P_E2PROM[103]=0;//��Ļ����
							  E2ROM_WR(954,1,P_E2PROM+103);
							  LCD_page_sign=KEY_main_page[0];//���ص���ҳ
						 	  LCD_page_dis=0;
						 	  message_num=11;//��ʾ����ɹ�
						  }	
					}	
			}
			else
//************ֹͣ���м���ֵ*********************************************************			
			if(KEY_mode==0x1003)//����ֹͣ�л�
			{
				DELAY_message=100000;//�����ʾ������
			
				tran_data=LCD_Rbuffer[7];
				tran_data=(tran_data<<8)|LCD_Rbuffer[8];
				
					if(tran_data==1)//�жϵ�ǰ������״̬����ֹͣ״̬
					{
						if(equipment_skate[0][2]==0)//�����ǰֹͣ
						{
							RUN_stop=1;//��������ҳ��							
							LCD_page_dis=0;
						}
						else
						if(equipment_skate[0][2]==1)//�����ǰ����
						{
							RUN_stop=2;//����ֹͣҳ��	
							LCD_page_dis=0;
						}
						else
						if(equipment_skate[0][2]==2)//�����ǰ���� ����
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0005,0,0,0);//��������
							C1TR01CONbits.TXREQ1=1;
							WRITE_fault_one(41,0);
#if compilemodbus
                                                        Modbus_electric[0]=41;Modbus_electric[1]=1;
#endif
							DELAY(1000);
							//message_num=7;//��ǰ���ϣ�������Ч
						}
						else
						if(equipment_skate[0][2]==3)//�����ǰ���� ��ʾ
						{
							message_num=8;//
						}
						else
						if(equipment_skate[0][2]==4)//�����ǰ��ͣ ��ʾ
						{
							message_num=9;//
						}
						else
						if(equipment_skate[0][2]==5)//�����ǰ����
						{
							RUN_stop=3;//����ֹͣ����ҳ��	
							LCD_page_dis=0;
						}
					}
					else
					if(tran_data==2)//ֹͣ�ź�
					{
						ecan1WriteTxMsgBufId(1,0x201,0,0);
						ecan1WriteTxMsgBufData(1,8,0x0001,0,0,0);//ֹͣ����
						C1TR01CONbits.TXREQ1=1;
						WRITE_fault_one(34,0);
#if compilemodbus
                                                Modbus_electric[0]=34;Modbus_electric[1]=1;
#endif
						DELAY(5000);
						
						RUN_stop=0;//��������ҳ��							
						LCD_page_dis=0;//����������ҳ
					}
					else
					if(tran_data==3)//�����ź�
					{
						ecan1WriteTxMsgBufId(1,0x201,0,0);
						ecan1WriteTxMsgBufData(1,8,0x0002,0,0,0);//��������
						C1TR01CONbits.TXREQ1=1;	
						WRITE_fault_one(33,0);
#if compilemodbus
                                                Modbus_electric[0]=33;Modbus_electric[1]=1;
#endif
						DELAY(5000);
						
						RUN_stop=0;//��������ҳ��							
						LCD_page_dis=0;//����������ҳ
					}
					else
					if(tran_data==4)//ȡ���ź�
					{
						RUN_stop=0;//��������ҳ��							
						LCD_page_dis=0;//����������ҳ	
					}
			}
			else
//**********��ʷ��¼�е���ҳ����ҳ��ɾ���ϴ���Ȱ�ť********************************���ܰ�ťҲ�����****	
			if(KEY_mode==0x1004)//���ܰ�ť ��ҳ��
			{
				tran_data=LCD_Rbuffer[7];
				tran_data=(tran_data<<8)|LCD_Rbuffer[8];
								
				KEY_com=tran_data;//���ܰ�ť��ֵ��ȫ�ֱ���
			}
			else
//*********ϵͳʱ���еı�������********************************	
      		if(KEY_mode>=0x017C&&KEY_mode<=0x017E)
      		{
					if(KEY_mode==0x017C) button_e2prom(5,LCD_Rbuffer+7);//���⿪������
				else if(KEY_mode==0x017D) button_e2prom(6,LCD_Rbuffer+7);//����ر�����
				else if(KEY_mode==0x017E) {button_e2prom(7,LCD_Rbuffer+7);DELAY_BLIGHT_SIGN=P_E2PROM[7];DELAY_BLIGHT_SIGN*=600000;}//����ʱ��//����ʱ������
			}
			else
//*********ͨѶ�����еĺ��룬��ַ����********************************	
			if(KEY_mode>=0x0186&&KEY_mode<=0x018d)
			{
				if(KEY_mode==0x0186)//������ĺ���ǰ��λ
				{
					tran_data=LCD_Rbuffer[7];
					tran_data=(tran_data<<8)|LCD_Rbuffer[8];//�߰�λ
					long_data=tran_data;
					long_data<<=16;      //����ʮ��λ
					
					tran_data=LCD_Rbuffer[9];
					tran_data=(tran_data<<8)|LCD_Rbuffer[10];//�ڰ�λ
					long_data+=tran_data;//��������������
					
					P_E2PROM[10]=long_data/10000;
					P_E2PROM[11]=long_data%10000;
	
					E2ROM_WR(10,2,P_E2PROM+10);	
				}
				else if(KEY_mode==0x0188) button_e2prom(12,LCD_Rbuffer+7);//������ĺ������λ
				else if(KEY_mode==0x0189) button_e2prom(9,LCD_Rbuffer+7);//�豸վ��ַ
				else if(KEY_mode==0x018c) 
				{
					button_e2prom_diff(119,970,LCD_Rbuffer+7);//��żУ��
					BSP_reset=1;
				}
				else if(KEY_mode==0x018a)
				{
					button_e2prom_diff(117,968,LCD_Rbuffer+7);
					button_e2prom_diff(118,969,LCD_Rbuffer+9);
					BSP_reset=1;
				}//������
			}
			else
//*********��ʱ����*****************************************
			if(KEY_mode>=0x0190&&KEY_mode<=0x01B9)
			{
					if(KEY_mode==0x0190) button_e2prom(32,LCD_Rbuffer+7); //����һ ����ʹ��
				else if(KEY_mode==0x0191) button_e2prom(38,LCD_Rbuffer+7);//���ڶ� ����ʹ��
				else if(KEY_mode==0x0192) button_e2prom(44,LCD_Rbuffer+7);//������ ����ʹ��
				else if(KEY_mode==0x0193) button_e2prom(50,LCD_Rbuffer+7);//������ ����ʹ��
				else if(KEY_mode==0x0194) button_e2prom(56,LCD_Rbuffer+7);//����������ʹ��
				else if(KEY_mode==0x0195) button_e2prom(62,LCD_Rbuffer+7);//������ ����ʹ��
				else if(KEY_mode==0x0196) button_e2prom(68,LCD_Rbuffer+7);//������ ����ʹ��
				else if(KEY_mode==0x0197) button_e2prom(33,LCD_Rbuffer+7);//����һ ����ʱ
				else if(KEY_mode==0x0198) button_e2prom(39,LCD_Rbuffer+7);//���ڶ� ����ʱ
				else if(KEY_mode==0x0199) button_e2prom(45,LCD_Rbuffer+7);//������ ����ʱ
				else if(KEY_mode==0x019A) button_e2prom(51,LCD_Rbuffer+7);//������ ����ʱ
				else if(KEY_mode==0x019B) button_e2prom(57,LCD_Rbuffer+7);//������ ����ʱ
				else if(KEY_mode==0x019C) button_e2prom(63,LCD_Rbuffer+7);//������ ����ʱ
				else if(KEY_mode==0x019D) button_e2prom(69,LCD_Rbuffer+7);//������ ����ʱ
				else if(KEY_mode==0x019E) button_e2prom(34,LCD_Rbuffer+7);//����һ ������
				else if(KEY_mode==0x019F) button_e2prom(40,LCD_Rbuffer+7);//���ڶ� ������
				else if(KEY_mode==0x01A0) button_e2prom(46,LCD_Rbuffer+7);//������ ������
				else if(KEY_mode==0x01A1) button_e2prom(52,LCD_Rbuffer+7);//������ ������
				else if(KEY_mode==0x01A2) button_e2prom(58,LCD_Rbuffer+7);//������ ������
				else if(KEY_mode==0x01A3) button_e2prom(64,LCD_Rbuffer+7);//������ ������
				else if(KEY_mode==0x01A4) button_e2prom(70,LCD_Rbuffer+7);//������ ������
				else if(KEY_mode==0x01A5) button_e2prom(35,LCD_Rbuffer+7);//����һ ֹͣʹ��
				else if(KEY_mode==0x01A6) button_e2prom(41,LCD_Rbuffer+7);//���ڶ� ֹͣʹ��
				else if(KEY_mode==0x01A7) button_e2prom(47,LCD_Rbuffer+7);//������ ֹͣʹ��
				else if(KEY_mode==0x01A8) button_e2prom(53,LCD_Rbuffer+7);//������ ֹͣʹ��
				else if(KEY_mode==0x01A9) button_e2prom(59,LCD_Rbuffer+7);//������ ֹͣʹ��
				else if(KEY_mode==0x01AA) button_e2prom(65,LCD_Rbuffer+7);//������ ֹͣʹ��
				else if(KEY_mode==0x01AB) button_e2prom(71,LCD_Rbuffer+7);//������ ֹͣʹ��
				else if(KEY_mode==0x01AC) button_e2prom(36,LCD_Rbuffer+7);//����һ ֹͣʹ�� ʱ
				else if(KEY_mode==0x01AD) button_e2prom(42,LCD_Rbuffer+7);//���ڶ� ֹͣʹ�� ʱ
				else if(KEY_mode==0x01AE) button_e2prom(48,LCD_Rbuffer+7);//������ ֹͣʹ�� ʱ
				else if(KEY_mode==0x01AF) button_e2prom(54,LCD_Rbuffer+7);//������ ֹͣʹ�� ʱ
				else if(KEY_mode==0x01B0) button_e2prom(60,LCD_Rbuffer+7);//������ ֹͣʹ�� ʱ
				else if(KEY_mode==0x01B1) button_e2prom(66,LCD_Rbuffer+7);//������ ֹͣʹ�� ʱ
				else if(KEY_mode==0x01B2) button_e2prom(72,LCD_Rbuffer+7);//������ ֹͣʹ�� ʱ
				else if(KEY_mode==0x01B3) button_e2prom(37,LCD_Rbuffer+7);//����һ ֹͣʹ�� ��
				else if(KEY_mode==0x01B4) button_e2prom(43,LCD_Rbuffer+7);//���ڶ� ֹͣʹ�� ��
				else if(KEY_mode==0x01B5) button_e2prom(49,LCD_Rbuffer+7);//������ ֹͣʹ�� ��
				else if(KEY_mode==0x01B6) button_e2prom(55,LCD_Rbuffer+7);//������ ֹͣʹ�� ��
				else if(KEY_mode==0x01B7) button_e2prom(61,LCD_Rbuffer+7);//������ ֹͣʹ�� ��
				else if(KEY_mode==0x01B8) button_e2prom(67,LCD_Rbuffer+7);//������ ֹͣʹ�� ��
				else if(KEY_mode==0x01B9) button_e2prom(73,LCD_Rbuffer+7);//������ ֹͣʹ�� ��
			}		
			else
//*********�������ֵ����********************************		
				 if(KEY_mode==0x0050)   button_e2prom(74,LCD_Rbuffer+7);//������ѹ���� ������������� P_E2PROM[29]����ʵ�ʵ�*****
			else if(KEY_mode==0x0064)   button_e2prom(75,LCD_Rbuffer+7);//������������ ������������� P_E2PROM[29]����ʵ�ʵ�*****
			else if(KEY_mode==0x0065)   button_e2prom_diff(113,964,LCD_Rbuffer+7);//�������������
			else if(KEY_mode==0x0078)   button_e2prom(76,LCD_Rbuffer+7);//���ص������� ������������� P_E2PROM[29]����ʵ�ʵ�*****
			else if(KEY_mode==0x0090)   button_e2prom(77,LCD_Rbuffer+7);//������ѹƵ�� ������������� P_E2PROM[29]����ʵ�ʵ�*****
			else if(KEY_mode==0x00a9)   button_e2prom(78,LCD_Rbuffer+7);//������������ ������������� P_E2PROM[29]����ʵ�ʵ�*****
			else if(KEY_mode==0x00c2)   button_e2prom(79,LCD_Rbuffer+7);//���ص���Ƶ�� ������������� P_E2PROM[29]����ʵ�ʵ�*****
			else
//********ϵͳ����-ϵͳ����*******************************
			if(KEY_mode>=0x01C9&&KEY_mode<=0x01D2)
			{
				if(which_equ==2)	 //3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
				{           
						if(KEY_mode==0x01CC)  protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER1_range[0]);
					else if(KEY_mode==0x01CD) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER1_range[1]);
					else if(KEY_mode==0x01CE) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER1_range[2]);
					else if(KEY_mode==0x01CF) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER1_range[3]);			
					else if(KEY_mode==0x01D0) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER1_range[4]);
					else if(KEY_mode==0x01D1) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER1_range[5]);
					else if(KEY_mode==0x01D2)//�������ݻ��壬����change_data��׼����������
					 {					 
						 	//tran_data=LCD_Rbuffer[7];
							tran_data=LCD_Rbuffer[8];						
							tran_data=(tran_data>>4)*10+(tran_data&0x0f);
						 	change_data=tran_data;// 	
						 	protect_parameter_b[(tran_data-1)/16+6]=protect_parameter_b[(tran_data-1)/16+6]^(0x0001<<(tran_data-1)%16);//��������
					 }
				}
				else if(which_equ==3)//3.5 SVG// SVG����
				{
						if(KEY_mode==0x01CC)  protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER5_Srange[0]);
					else if(KEY_mode==0x01CD) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER5_Srange[1]);
					else if(KEY_mode==0x01CE) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER5_Srange[2]);
					else if(KEY_mode==0x01CF) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER5_Srange[3]);			
					else if(KEY_mode==0x01D0) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER5_Srange[4]);
					else if(KEY_mode==0x01D1) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER5_Srange[5]);
					else if(KEY_mode==0x01D2)//�������ݻ��壬����change_data��׼����������
					 {					 
						 	//tran_data=LCD_Rbuffer[7];
							tran_data=LCD_Rbuffer[8];						
							tran_data=(tran_data>>4)*10+(tran_data&0x0f);
						 	change_data=tran_data;// 	
						 	protect_parameter_b[(tran_data-1)/16+6]=protect_parameter_b[(tran_data-1)/16+6]^(0x0001<<(tran_data-1)%16);//��������
					 }	
				}				
				
							 
			} 
			else
//**********ϵͳ����-ϵͳ��ֵ********************************		
		 	if(KEY_mode>=0x020A&&KEY_mode<=0x0212)
		 	{
				if(which_equ==2)	 //3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
				{
					if(LCD_page_sign==0x607)//���Բ�һ
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER2_range[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER2_range[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER2_range[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER2_range[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER2_range[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER2_range[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER2_range[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER2_range[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER2_range[8]);	
					}
					else
					if(LCD_page_sign==0x608)//���Բ���
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER3_range[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER3_range[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER3_range[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER3_range[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER3_range[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER3_range[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER3_range[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER3_range[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER3_range[8]);	
	
					}
					else
					if(LCD_page_sign==0x606)//�������
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER8_range[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER8_range[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER8_range[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER8_range[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER8_range[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER8_range[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER8_range[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER8_range[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER8_range[8]);		
					}
					else
					if(LCD_page_sign==0x605)//������ֵ
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER4_range[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER4_range[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER4_range[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER4_range[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER4_range[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER4_range[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER4_range[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER4_range[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER4_range[8]);		
					}
					else
					if(LCD_page_sign==0x602||LCD_page_sign==0x603||LCD_page_sign==0x801||LCD_page_sign==0x802)//ָ������
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER5_range[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER5_range[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER5_range[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER5_range[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER5_range[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER5_range[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER5_range[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER5_range[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER5_range[8]);	
						
					}
					else
					if(LCD_page_sign==0x604||LCD_page_sign==0x803)//ǰ7����ָ�����������������Ƿ�CT���򱣻��ͻ�������������
					{
						if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER6_range[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER6_range[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER6_range[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER6_range[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER6_range[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER6_range[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER6_range[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER6_range[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER6_range[8]);		
					}
					
				}
				else if(which_equ==3)//3.5 SVG
				{
					if(LCD_page_sign==0x601)
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER1_Srange[8]);							
					}
					else
					if(LCD_page_sign==0x602)
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER2_Srange[8]);							
					}
					else
					if(LCD_page_sign==0x603)
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER3_Srange[8]);							
					}
					else
					if(LCD_page_sign==0x604)
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER4_Srange[8]);							
					}
					else
					if(LCD_page_sign==0x606)
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER6_Srange[8]);							
					}
					else
					if(LCD_page_sign==0x607)
					{
							if(KEY_mode==0x020A) protect_parameter_b[0]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[0]);
						else if(KEY_mode==0x020B) protect_parameter_b[1]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[1]);
						else if(KEY_mode==0x020C) protect_parameter_b[2]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[2]);
						else if(KEY_mode==0x020D) protect_parameter_b[3]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[3]);		
						else if(KEY_mode==0x020E) protect_parameter_b[4]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[4]);
						else if(KEY_mode==0x020F) protect_parameter_b[5]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[5]);
						else if(KEY_mode==0x0210) protect_parameter_b[6]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[6]);
						else if(KEY_mode==0x0211) protect_parameter_b[7]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[7]);
						else if(KEY_mode==0x0212) protect_parameter_b[8]=button_return(LCD_Rbuffer+7,PARAMETER7_Srange[8]);							
					}
				}	 
			}
			else
//**********ϵͳ����-�������*********************************			
			if(KEY_mode>=0x023A&&KEY_mode<=0x023E)
			{
				 if(KEY_mode==0x023A) button_e2prom(23,LCD_Rbuffer+7);//������ ֹͣʹ�� ��
				else if(KEY_mode==0x023B) button_e2prom(24,LCD_Rbuffer+7);//������ ֹͣʹ�� ��
				else if(KEY_mode==0x023C) button_e2prom(25,LCD_Rbuffer+7);//������ ֹͣʹ�� ��
				else if(KEY_mode==0x023D) //��Ļ���� ����
				{
						tran_data=LCD_Rbuffer[7];
						tran_data=(tran_data<<8)|LCD_Rbuffer[8];
						
						if(tran_data==1234)//������������һ�£�������Ļ
						{
							P_E2PROM[103]=1;//��Ļ������
							E2ROM_WR(954,1,P_E2PROM+103);
							message_num=10;//��Ļ������
							LCD_page_sign=0;//�ص���ҳ����
							LCD_page_dis=0;//��λ
							LCD_page_dis_b=0;
						}
						else
						{
							message_num=1;//��ʾ�����������	
						}	
				}
				else if(KEY_mode==0x023E) //�ָ���������
				{
					tran_data=LCD_Rbuffer[7];
					tran_data=(tran_data<<8)|LCD_Rbuffer[8];
						
						if(tran_data==1234)//������������һ�£��ָ���������
						{
							CLEAR_fault_all(0);//�ָ���������
							LCD_page_dis=0;//��λ
							LCD_page_dis_b=0;
							message_num=3;//��ʾ�ָ���������
						}
						else
						{
							message_num=1;//��ʾ�����������	
						}	
				}
			}
//*************��������1***************************************
			if(KEY_mode>=0x024A&&KEY_mode<=0x0253)
			{
				 if(KEY_mode==0x024A) button_e2prom(14,LCD_Rbuffer+7);//
				else if(KEY_mode==0x024B) button_e2prom(17,LCD_Rbuffer+7);//
				else if(KEY_mode==0x024C) button_e2prom(18,LCD_Rbuffer+7);//
				else if(KEY_mode==0x024D) button_e2prom(26,LCD_Rbuffer+7);//
				else if(KEY_mode==0x024E) button_e2prom(15,LCD_Rbuffer+7);//
				else if(KEY_mode==0x024F) button_e2prom(16,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0250) button_e2prom(20,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0251) button_e2prom(29,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0252) button_e2prom(27,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0253) button_e2prom(31,LCD_Rbuffer+7);//
			}
			else	
//*************��������2***************************************	
			if(KEY_mode>=0x025A&&KEY_mode<=0x0263)
			{
				 if(KEY_mode==0x025A) button_e2prom(21,LCD_Rbuffer+7);//
				else if(KEY_mode==0x025B) button_e2prom(22,LCD_Rbuffer+7);//
				else if(KEY_mode==0x025C) button_e2prom(30,LCD_Rbuffer+7);//
				else if(KEY_mode==0x025D) button_e2prom_diff(104,955,LCD_Rbuffer+7);//
				else if(KEY_mode==0x025E) button_e2prom_diff(105,956,LCD_Rbuffer+7);//
				else if(KEY_mode==0x025F) button_e2prom_diff(106,957,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0260) button_e2prom_diff(107,958,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0261) button_e2prom_diff(109,960,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0262) button_e2prom_diff(108,959,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0263) button_e2prom_diff(110,961,LCD_Rbuffer+7);//	
			}
			else
//*************ģ����ʾ1***************************************
			if(KEY_mode>=0x026A&&KEY_mode<=0x0275)
			{
				 if(KEY_mode==0x026A) button_e2prom(80,LCD_Rbuffer+7);//
				else if(KEY_mode==0x026B) button_e2prom(81,LCD_Rbuffer+7);//
				else if(KEY_mode==0x026C) button_e2prom(82,LCD_Rbuffer+7);//
				else if(KEY_mode==0x026D) button_e2prom(83,LCD_Rbuffer+7);//
				else if(KEY_mode==0x026E) button_e2prom(84,LCD_Rbuffer+7);//
				else if(KEY_mode==0x026F) button_e2prom(85,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0270) button_e2prom(86,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0271) button_e2prom(87,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0272) button_e2prom(88,LCD_Rbuffer+7);//
				else if(KEY_mode==0x0273) button_e2prom(89,LCD_Rbuffer+7);//
                                else if(KEY_mode==0x0275)
                                {
                                    tran_data=LCD_Rbuffer[7];
				    tran_data=(tran_data<<8)|LCD_Rbuffer[8];
                                    if(tran_data==0x0001)
                                    {
                                        ecan1WriteTxMsgBufId(1,0x201,0,0);
				        ecan1WriteTxMsgBufData(1,8,0x0007,0,0,0);//��������
					C1TR01CONbits.TXREQ1=1;
                                    }
                                }
			}
			else
//*************ģ����ʾ2***************************************
			if(KEY_mode>=0x027A&&KEY_mode<=0x0283)
			{
				 if(KEY_mode==0x027A) button_e2prom(90,LCD_Rbuffer+7);//
				else if(KEY_mode==0x027B) button_e2prom_diff(123,974,LCD_Rbuffer+7);//
				else if(KEY_mode==0x027C) button_e2prom(92,LCD_Rbuffer+7);//
				else if(KEY_mode==0x027D) button_e2prom_diff(115,966,LCD_Rbuffer+7);//
				else if(KEY_mode==0x027E) button_e2prom_diff(114,965,LCD_Rbuffer+7);//
				else if(KEY_mode==0x027F) button_e2prom_diff(116,967,LCD_Rbuffer+7);//
                                else if(KEY_mode==0x0281)
                                {
                                        tran_data=LCD_Rbuffer[7];
					tran_data=(tran_data<<8)|LCD_Rbuffer[8];//�߰�λ
					long_data=tran_data;
					long_data<<=16;      //����ʮ��λ

					tran_data=LCD_Rbuffer[9];
					tran_data=(tran_data<<8)|LCD_Rbuffer[10];//�ڰ�λ
					long_data+=tran_data;//��������������

					P_E2PROM[120]=long_data/10000;
					P_E2PROM[121]=long_data%10000;

					E2ROM_WR(971,2,P_E2PROM+120);
                                }
                                else if(KEY_mode==0x0283) button_e2prom_diff(122,973,LCD_Rbuffer+7);//
			}
			else
//*************��������***************************************
			if(KEY_mode>=0x028A&&KEY_mode<=0x0290)
			{
                            if(LCD_page_sign==0x705)
                            {
                                if(KEY_mode==0x028a)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=1;//�޸�
                                   P_E2PROM[124]^=tran_data&0x0001;
                                   E2ROM_WR(975,1,P_E2PROM+124);
                               }
                               else
                               if(KEY_mode==0x028b)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=2;//�޸�
                                   P_E2PROM[125]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(976,1,P_E2PROM+125);
                               }
                               else
                               if(KEY_mode==0x028c)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=3;//�޸�
                                   P_E2PROM[126]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(977,1,P_E2PROM+126);
                               }
                               else
                               if(KEY_mode==0x028d)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=4;//�޸�
                                   P_E2PROM[127]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(978,1,P_E2PROM+127);
                               }
                               else
                               if(KEY_mode==0x028e)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=5;//�޸�
                                   P_E2PROM[128]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(979,1,P_E2PROM+128);
                               }
                               else
                               if(KEY_mode==0x028f)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=6;//�޸�
                                   P_E2PROM[129]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(980,1,P_E2PROM+129);
                               }
                               else
                               if(KEY_mode==0x0290)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=7;//�޸�
                                   P_E2PROM[130]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(981,1,P_E2PROM+130);
                               }
                            }
                            else if(LCD_page_sign==0x706)
                            {
                                if(KEY_mode==0x028a)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=1;//�޸�
                                   P_E2PROM[131]^=tran_data&0x0001;
                                   E2ROM_WR(982,1,P_E2PROM+131);
                               }
                               else
                               if(KEY_mode==0x028b)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=2;//�޸�
                                   P_E2PROM[132]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(983,1,P_E2PROM+132);
                               }
                               else
                               if(KEY_mode==0x028c)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=3;//�޸�
                                   P_E2PROM[133]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(984,1,P_E2PROM+133);
                               }
                               else
                               if(KEY_mode==0x028d)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=4;//�޸�
                                   P_E2PROM[134]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(985,1,P_E2PROM+134);
                               }
                               else
                               if(KEY_mode==0x028e)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=5;//�޸�
                                   P_E2PROM[135]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(986,1,P_E2PROM+135);
                               }
                               else
                               if(KEY_mode==0x028f)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=6;//�޸�
                                   P_E2PROM[136]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(987,1,P_E2PROM+136);
                               }
                               else
                               if(KEY_mode==0x0290)
                               {
                                   tran_data=LCD_Rbuffer[8];
                                   change_data_h=7;//�޸�
                                   P_E2PROM[137]^=0x0001<<(tran_data&0x0f);
                                   E2ROM_WR(988,1,P_E2PROM+137);
                               }
                            }
			}
//****************���ϴ���**********************
			else
			if(KEY_mode>=0x2328&&KEY_mode<0x2710)//����״̬
			{
				j=KEY_mode-0x2328;
				
				for(i=0;i<100;i++)
				{
						tran_data=LCD_Rbuffer[7+(i<<1)];
						tran_data=(tran_data<<8)|LCD_Rbuffer[8+(i<<1)];
						error_addr[j+i]=tran_data;
				}
				
				change_data=0xaaaa;//֪ͨ�Ѿ���ȫ��ȡ			
			}
			else
			if(KEY_mode>=0x2710&&KEY_mode<0x4e20)//���ϼ�¼
			{
				for(i=0;i<10;i++)
				{
						tran_data=LCD_Rbuffer[7+(i<<1)];
						tran_data=(tran_data<<8)|LCD_Rbuffer[8+(i<<1)];
						error_four_data[LCD_page_dis-7][i]=tran_data;
				}
				
				change_data=0xaaaa;//֪ͨ�Ѿ���ȫ��ȡ
			}
			else
			if(KEY_mode==0x4e20)
			{
				change_data=0xaaaa;
			}	
			
		}
		else
		if(LCD_Rbuffer[3]==0x81)//����ǼĴ�������
		{			
			if(LCD_Rbuffer[4]==5)//����ǼĴ���5�������ϴ�
			{
				if(LCD_Rbuffer[6]==0x5A)//����д���
				{					
					DELAY_Blight=0;//���¼�ʱ��	
					LED_state=0x81;//��Ļ������ʶ	
				}	
			}
			else
			if(LCD_Rbuffer[4]==0X20)//����ǼĴ���20 ��ǰʱ��
			{				
				TIME_NOW[0]=(LCD_Rbuffer[6]>>4)*10+(LCD_Rbuffer[6]&0x0f);
				TIME_NOW[1]=(LCD_Rbuffer[7]>>4)*10+(LCD_Rbuffer[7]&0x0f);
				TIME_NOW[2]=(LCD_Rbuffer[8]>>4)*10+(LCD_Rbuffer[8]&0x0f);
				TIME_NOW[3]=LCD_Rbuffer[9];//����
				TIME_NOW[4]=(LCD_Rbuffer[10]>>4)*10+(LCD_Rbuffer[10]&0x0f);
				TIME_NOW[5]=(LCD_Rbuffer[11]>>4)*10+(LCD_Rbuffer[11]&0x0f);
			}
			else
			if(LCD_Rbuffer[4]==0X40)//5A��ʾδ���falsh ��д��0����ʾ��д���
			{
				change_data=LCD_Rbuffer[6];	
				change_data_only=change_data;//ֻ������һ���ط�
			}
			else
			if(LCD_Rbuffer[4]==0X00)//���մ������ں˰汾��
			{
			   lcd_version=LCD_Rbuffer[6];
			}
		}	
		
		LCD_Rbuffer_sign=0;//����ǰ�ť����������
	 }	
}


//*******���ʹ���**************************************************************************
unsigned int RS232_Slcd_data(unsigned int fist_num,unsigned int fist_addr,unsigned char num,volatile unsigned int *data)
{                   //��fist_addr��ʼ�ĵ�ַ��д��data��ʼ��num������
	unsigned int i,j=0;	
	if(LCD_Tbuffer_sign==1)
	{
		j=fist_num;
		LCD_Tbuffer[j++]=0xcc;
		LCD_Tbuffer[j++]=0xdd;		
		LCD_Tbuffer[j++]=(num<<1)+3;
		LCD_Tbuffer[j++]=0x82;
		LCD_Tbuffer[j++]=fist_addr>>8;
		LCD_Tbuffer[j++]=fist_addr&0xff;//��ʼ��ַ
		for(i=0;i<num;i++)    //д��num������
		{
			LCD_Tbuffer[j++]=data[i]>>8;
			LCD_Tbuffer[j++]=data[i]&0xff;	
		}
		
		LCD_Tbuffer_now=0;
		LCD_Tbuffer_num=j;	
		LCD_Tbuffer_sign=0;		
		LCD_page_dis++;
	}
	
	return j;	
}	

unsigned int RS232_Slcd_data_nolast(unsigned int fist_num,unsigned int fist_addr,unsigned char num,volatile unsigned int *data)
{                   //��fist_addr��ʼ�ĵ�ַ��д��data��ʼ��num������
	unsigned int i,j=0;	
	if(LCD_Tbuffer_sign==1)
	{
		j=fist_num;
		LCD_Tbuffer[j++]=0xcc;
		LCD_Tbuffer[j++]=0xdd;		
		LCD_Tbuffer[j++]=(num<<1)+3;
		LCD_Tbuffer[j++]=0x82;
		LCD_Tbuffer[j++]=fist_addr>>8;
		LCD_Tbuffer[j++]=fist_addr&0xff;//��ʼ��ַ
		for(i=0;i<num;i++)    //д��num������
		{
			LCD_Tbuffer[j++]=data[i]>>8;
			LCD_Tbuffer[j++]=data[i]&0xff;	
		}
	}
	
	return j;	
}


unsigned int RS232_Slcd_record(unsigned int fist_num,unsigned int which_order,unsigned int data)//�л����ĸ�����
{             //which_order:0:�л�ҳ�棬1���޸�LED���ȣ�2���޸ķ��������� 3:���Ƿ��д���
	unsigned int j=0;
	
	if(LCD_Tbuffer_sign==1)
	{
		j=fist_num;
		LCD_Tbuffer[j++]=0xcc;
		LCD_Tbuffer[j++]=0xdd;	
		
		if(which_order==0)//�޸�ҳ��
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=data>>8;
			LCD_Tbuffer[j++]=data&0xff;			
		}
		else
		if(which_order==1)//led����
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x01;
			LCD_Tbuffer[j++]=data&0xff;	
		}	
		else
		if(which_order==2)//�޸ķ�����������
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x02;
			LCD_Tbuffer[j++]=data&0xff;	
		}
		else
		if(which_order==3)//���Ƿ��д���
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x81;
			LCD_Tbuffer[j++]=0x05;
			LCD_Tbuffer[j++]=0x01;	
		}
		else
		if(which_order==4)//�崥��
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x05;
			LCD_Tbuffer[j++]=data&0xff;	
		}	
		else
		if(which_order==5)//��ʱ��
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x81;
			LCD_Tbuffer[j++]=0x20;
			LCD_Tbuffer[j++]=0x06;
		}
		else
		if(which_order==6)//����ʱָ�� д
		{
			LCD_Tbuffer[j++]=0x0c;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x5a;//ִ��
			LCD_Tbuffer[j++]=0xa0;//��
			LCD_Tbuffer[j++]=0x40;//�ֿ�
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0x70;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0xcf;
		}
		else
		if(which_order==7)//д��ʱָ�� д
		{
			LCD_Tbuffer[j++]=0x0c;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x5a;//ִ��
			LCD_Tbuffer[j++]=0x50;//д��Ĵ���
			LCD_Tbuffer[j++]=0x40;//�ֿ�
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0x70;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0xcf;
		}
		else
		if(which_order==8)// ��0x40 ��дfalsh�Ƿ����5Aδ���  0���
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x81;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x01;
		}
		else
		if(which_order==9)// ��ȡ0x2170��ʼ�ģ�207��0xCF)  0x73������
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0x70;
			LCD_Tbuffer[j++]=0x73;
		}
		else
		if(which_order==10)// ��ȡ0x21e3��ʼ�ģ�207��0xCF)������
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0xe3;
			LCD_Tbuffer[j++]=0x5c;
		}
		else
		if(which_order==11)//����ʱָ�� �����еĹ���״̬1000���Ӻ����һ�������ϴ���
		{
			LCD_Tbuffer[j++]=0x0c;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x5a;//ִ��
			LCD_Tbuffer[j++]=0xa0;//��
			LCD_Tbuffer[j++]=0x41;//�ֿ�
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x23;
			LCD_Tbuffer[j++]=0x28;
			LCD_Tbuffer[j++]=0x17;
			LCD_Tbuffer[j++]=0x70;
		}
		else
		if(which_order==12)//д��ʱָ�� ����flash
		{
			LCD_Tbuffer[j++]=0x0c;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x5a;//ִ��
			LCD_Tbuffer[j++]=0x50;//д��Ĵ���
			LCD_Tbuffer[j++]=0x41;//�ֿ�
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x23;
			LCD_Tbuffer[j++]=0x28;
			LCD_Tbuffer[j++]=0x17;
			LCD_Tbuffer[j++]=0x70;
		}
		else
		if(which_order==13)//������״̬
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=data>>8;
			LCD_Tbuffer[j++]=data&0xff;
			LCD_Tbuffer[j++]=0x64;
		}
		else
		if(which_order==14)//��ʵ�ʹ���
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=data>>8;
			LCD_Tbuffer[j++]=data&0xff;
			LCD_Tbuffer[j++]=0x0a;
		}
        else
		if(which_order==15)//&&&&���ں˵İ汾��
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x81;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x01;			
		}
		LCD_Tbuffer_now=0;
		LCD_Tbuffer_num=j;
		LCD_Tbuffer_sign=0;
		LCD_page_dis++;	
	  } 
	  
	  return j;
}

unsigned int RS232_Slcd_record_nolast(unsigned int fist_num,unsigned int which_order,unsigned int data)//�л����ĸ�����
{             //which_order:0:�л�ҳ�棬1���޸�LED���ȣ�2���޸ķ���������
	unsigned int j=0;
	
	if(LCD_Tbuffer_sign==1)
	{
		j=fist_num;
		LCD_Tbuffer[j++]=0xcc;
		LCD_Tbuffer[j++]=0xdd;	
		
		if(which_order==0)//�޸�ҳ��
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=data>>8;
			LCD_Tbuffer[j++]=data&0xff;			
		}
		else
		if(which_order==1)//led����
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x01;
			LCD_Tbuffer[j++]=data&0xff;	
		}	
		else
		if(which_order==2)//�޸ķ�����������
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x02;
			LCD_Tbuffer[j++]=data&0xff;	
		}
		else
		if(which_order==3)//���Ƿ��д���
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x81;
			LCD_Tbuffer[j++]=0x05;
			LCD_Tbuffer[j++]=0x01;	
		}
		else
		if(which_order==4)//�崥��
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x05;
			LCD_Tbuffer[j++]=data&0xff;	
		}
		else
		if(which_order==5)//��ʱ��
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x81;
			LCD_Tbuffer[j++]=0x20;
			LCD_Tbuffer[j++]=0x06;
		}
				else
		if(which_order==6)//����ʱָ�� д
		{
			LCD_Tbuffer[j++]=0x0c;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x5a;//ִ��
			LCD_Tbuffer[j++]=0xa0;//��
			LCD_Tbuffer[j++]=0x40;//�ֿ�
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0x70;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0xcf;
		}
		else
		if(which_order==7)//д��ʱָ�� д
		{
			LCD_Tbuffer[j++]=0x0c;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x5a;//ִ��
			LCD_Tbuffer[j++]=0x50;//д��Ĵ���
			LCD_Tbuffer[j++]=0x40;//�ֿ�
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0x70;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0xcf;
		}
		else
		if(which_order==8)// ��0x40 ��дfalsh�Ƿ����5Aδ���  0���
		{
			LCD_Tbuffer[j++]=0x03;
			LCD_Tbuffer[j++]=0x81;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x01;
		}
		else
		if(which_order==9)// ��ȡ0x2170��ʼ�ģ�115��0x73)������
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0x70;
			LCD_Tbuffer[j++]=0x73;
		}
		else
		if(which_order==9)// ��ȡ0x2170��ʼ�ģ�207��0xCF)  0x73������
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0x70;
			LCD_Tbuffer[j++]=0x73;
		}
		else
		if(which_order==10)// ��ȡ0x21e3��ʼ�ģ�207��0xCF)������
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=0x21;
			LCD_Tbuffer[j++]=0xe3;
			LCD_Tbuffer[j++]=0x5c;
		}
		else
		if(which_order==11)//����ʱָ�� �����еĹ���״̬1000���Ӻ����һ�������ϴ���
		{
			LCD_Tbuffer[j++]=0x0c;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x5a;//ִ��
			LCD_Tbuffer[j++]=0xa0;//��
			LCD_Tbuffer[j++]=0x41;//�ֿ�
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x23;
			LCD_Tbuffer[j++]=0x28;
			LCD_Tbuffer[j++]=0x17;
			LCD_Tbuffer[j++]=0x70;
		}
		else
		if(which_order==12)//д��ʱָ�� ����flash
		{
			LCD_Tbuffer[j++]=0x0c;
			LCD_Tbuffer[j++]=0x80;
			LCD_Tbuffer[j++]=0x40;
			LCD_Tbuffer[j++]=0x5a;//ִ��
			LCD_Tbuffer[j++]=0x50;//д��Ĵ���
			LCD_Tbuffer[j++]=0x41;//�ֿ�
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x00;
			LCD_Tbuffer[j++]=0x23;
			LCD_Tbuffer[j++]=0x28;
			LCD_Tbuffer[j++]=0x17;
			LCD_Tbuffer[j++]=0x70;
		}
		else
		if(which_order==13)//������״̬
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=data>>8;
			LCD_Tbuffer[j++]=data&0xff;
			LCD_Tbuffer[j++]=0x64;
		}
		else
		if(which_order==14)//��ʵ�ʹ���
		{
			LCD_Tbuffer[j++]=0x04;
			LCD_Tbuffer[j++]=0x83;
			LCD_Tbuffer[j++]=data>>8;
			LCD_Tbuffer[j++]=data&0xff;
			LCD_Tbuffer[j++]=0x0a;
		}
			
	}
	  
	  return j;
}

//���ݴ���-����ͼ�괦��-����ָ���ڴ�
unsigned int ADD_icon_guide(unsigned int fist_add,unsigned int buffer_add,unsigned int x,unsigned int y,unsigned int icon_id,unsigned int dis_yes_no)//����ͼ�����ݼ���
{		
	LCD_Ddata_buffer[fist_add++]=buffer_add;//��������ַ	
	LCD_Ddata_buffer[fist_add++]=x;
	LCD_Ddata_buffer[fist_add++]=y;
	if(dis_yes_no!=0) LCD_Ddata_buffer[fist_add++]=1; //����  ��ʾ1������ʾ0
 	else              LCD_Ddata_buffer[fist_add++]=3; //����  ��ʾ1������ʾ0 //��ô��������ʾ
	LCD_Ddata_buffer[fist_add++]=3;//����
	LCD_Ddata_buffer[fist_add++]=icon_id;//ͼ��
	LCD_Ddata_buffer[fist_add++]=icon_id;//ͼ��
	LCD_Ddata_buffer[fist_add++]=0x2901;//���
		
	return fist_add;
}

//���ݴ���-�������ش���-����ָ���ڴ�
unsigned int ADD_data_guide(unsigned int fist_add,unsigned int x,unsigned int y,unsigned int colour,unsigned int size,unsigned int point,unsigned int dis_yes_no)
{
 	LCD_Ddata_buffer[fist_add++]=LCD_page_dis_b;
 	LCD_page_dis_b++;//�Զ��ӣ�����
 	LCD_Ddata_buffer[fist_add++]=x;
 	LCD_Ddata_buffer[fist_add++]=y;
 	LCD_Ddata_buffer[fist_add++]=colour;
 	if(dis_yes_no!=0) LCD_Ddata_buffer[fist_add++]=size&0xff; //0x0010;
 	else              LCD_Ddata_buffer[fist_add++]=(size&0xff)|0x4000;//0x1610;//��Ӧ�����ֿ�Ͳ���ʾ ��Ӧ���ֿ�ø�
 	LCD_Ddata_buffer[fist_add++]=0x0005;
 	LCD_Ddata_buffer[fist_add++]=point<<8;//С���㣬����
 	LCD_Ddata_buffer[fist_add++]=0;	
 	
 	LCD_Ddata_buffer[fist_add++]=0;
 	LCD_Ddata_buffer[fist_add++]=0;
 	LCD_Ddata_buffer[fist_add++]=0;
 	LCD_Ddata_buffer[fist_add++]=0;
 	LCD_Ddata_buffer[fist_add++]=0;

 	return fist_add;
}


unsigned int ADD_WAVE_ONE_no(unsigned int fist_add,unsigned int channel,volatile int *data)
{   //��ʼ��ַ��ַ��ͨ�����ݣ���Ŀ������ͨ��һ��
	unsigned int i,j=0;	
	
	if(LCD_Tbuffer_sign==1)
	{
		j=fist_add;
		LCD_Tbuffer[j++]=0xcc;
		LCD_Tbuffer[j++]=0xdd;		
		LCD_Tbuffer[j++]=196;
		LCD_Tbuffer[j++]=0x84;
		LCD_Tbuffer[j++]=channel;
		for(i=0;i<64;i++)    //д��num������
		{
			LCD_Tbuffer[j++]=data[i]>>8;
			LCD_Tbuffer[j++]=data[i]&0xff;	
		}		
		for(i=0;i<33;i++)    //д��num������
		{
			LCD_Tbuffer[j++]=data[i]>>8;
			LCD_Tbuffer[j++]=data[i]&0xff;	
		}
	}
	
	return j;
}

//**************Ƶ�����ݷ��ͣ�����******************************
unsigned int ADD_FRE_fist_no(unsigned int fist_num,unsigned int fist_addr,unsigned int fist_coor,volatile int *data)
{   //��ʼ��ַ��ַ�������׵�ַ�����ݻ�����
	unsigned int i,j=0;	  
	unsigned int colour,data_tran;
	
		 if(fist_coor==50) colour=0xffe0;
	else if(fist_coor==51) colour=0x25A0;
	else if(fist_coor==52) colour=0xf986;
	
	if(LCD_Tbuffer_sign==1)
	{
		j=fist_num;
		LCD_Tbuffer[j++]=0xcc;
		LCD_Tbuffer[j++]=0xdd;		
		LCD_Tbuffer[j++]=163;    //16*10+3;
		LCD_Tbuffer[j++]=0x82;
		LCD_Tbuffer[j++]=fist_addr>>8;
		LCD_Tbuffer[j++]=fist_addr&0xff;//��ʼ��ַ

		LCD_Tbuffer[j++]=fist_coor>>8;
		LCD_Tbuffer[j++]=fist_coor&0xff;		
		LCD_Tbuffer[j++]=data[0]>>8;
		LCD_Tbuffer[j++]=data[0]&0xff;//���Ͻ�
		
		data_tran=fist_coor+0;    //�����
		LCD_Tbuffer[j++]=data_tran>>8;
		LCD_Tbuffer[j++]=data_tran&0xff;
		LCD_Tbuffer[j++]=0;
		LCD_Tbuffer[j++]=199;//	//���½�
		
		LCD_Tbuffer[j++]=colour>>8;
		LCD_Tbuffer[j++]=colour&0xff;
		
		data_tran+=3;//Ϊ�˷��������ѭ��
		for(i=1;i<16;i++)    //д��num������
		{		
			data_tran+=4;
			LCD_Tbuffer[j++]=data_tran>>8;
			LCD_Tbuffer[j++]=data_tran&0xff;		
			LCD_Tbuffer[j++]=data[i]>>8;
			LCD_Tbuffer[j++]=data[i]&0xff;
			
			//data_tran+=0;
			LCD_Tbuffer[j++]=data_tran>>8;
			LCD_Tbuffer[j++]=data_tran&0xff;
			LCD_Tbuffer[j++]=0;
			LCD_Tbuffer[j++]=199;//380
			
			LCD_Tbuffer[j++]=colour>>8;
			LCD_Tbuffer[j++]=colour&0xff;
		}
	}
	
	return j;
}

unsigned int ADD_FRE_second_no(unsigned int fist_num,unsigned int fist_addr,unsigned int fist_coor,volatile int *data)
{   //��ʼ��ַ��ַ�������׵�ַ�����ݻ�����
	unsigned int i,j=0;	
	unsigned int colour,data_tran;
	
		 if(fist_coor==50) colour=0xffe0;
	else if(fist_coor==51) colour=0x25A0;
	else if(fist_coor==52) colour=0xf986;
	
	if(LCD_Tbuffer_sign==1)
	{
		j=fist_num;
		LCD_Tbuffer[j++]=0xcc;
		LCD_Tbuffer[j++]=0xdd;		
		LCD_Tbuffer[j++]=163;
		LCD_Tbuffer[j++]=0x82;
		LCD_Tbuffer[j++]=(fist_addr+80)>>8;
		LCD_Tbuffer[j++]=(fist_addr+80)&0xff;//��ʼ��ַ
		
		data_tran=fist_coor+63;//Ϊ�˷��������ѭ��
		for(i=16;i<32;i++)    //д��num������
		{		
			data_tran+=4;
			LCD_Tbuffer[j++]=data_tran>>8;
			LCD_Tbuffer[j++]=data_tran&0xff;		
			LCD_Tbuffer[j++]=data[i]>>8;
			LCD_Tbuffer[j++]=data[i]&0xff;
			
			//data_tran+=0;
			LCD_Tbuffer[j++]=data_tran>>8;
			LCD_Tbuffer[j++]=data_tran&0xff;
			LCD_Tbuffer[j++]=0;
			LCD_Tbuffer[j++]=199;//380
			
			LCD_Tbuffer[j++]=colour>>8;
			LCD_Tbuffer[j++]=colour&0xff;
		}		
	}
	
	return j;
}

//*********���ϴ���*********************************************
void READ_four_error(unsigned int add_sub,volatile unsigned int *error_dis_now,volatile unsigned int *data)//�ҳ�һҳ��ʾ���ĸ����ϴ���
{		
	unsigned int i,j;	
	
		error_dis_now[0]=1000;
		error_dis_now[1]=1000;
		error_dis_now[2]=1000;
		error_dis_now[3]=1000;

	if(data[500]!=0)
	{
		if(add_sub==0)
		{
				error_dis_last=error_dis;
								
				j=error_dis;				
				error_dis+=4;				
				if(error_dis>=data[500]) 
						error_dis=0;
		}
		else
		{			
				error_dis=error_dis_last;
			
				if(error_dis_last==0)
				{
					j=data[500]&0x03;//
					if(j==0) j=4;
					error_dis_last=data[500]-j;
				}
				else
				{
					error_dis_last-=4;				
				}
				j=error_dis_last;		
		}		
		
		
			for(i=0;i<4;i++)
			{
					error_dis_now[i]=data[j++];				
					if(j>=data[500]) 			
						break;		
			}	
	}
}

void FAULT_num(volatile unsigned int place,volatile unsigned int num,volatile unsigned int mode,volatile unsigned int *data)//����ǰ������豸�Ĺ��ϴ���
{
	unsigned int i,j=0,fault,x;
	unsigned int data_b[500];

	for(i=0;i<num;i++)
	{
	    if((data[place]&0x00ff)==mode||mode==0)
	    {
                if(P_E2PROM[124]==1)
                {
                    fault=(data[place]&0xff00)>>8;
                   if(fault>=1&&fault<=16)
                   {
                       x=P_E2PROM[125]&(0x0001<<(fault-1));
                       if(x!=0) data_b[j++]=place;
                   }
                   else
                   if(fault>=17&&fault<=32)
                   {
                       x=P_E2PROM[126]&(0x0001<<(fault-17));
                       if(x!=0)data_b[j++]=place;
                   }
                   else
                   if(fault>=33&&fault<=48)
                   {
                       x=P_E2PROM[127]&(0x0001<<(fault-33));
                       if(x!=0)data_b[j++]=place;
                   }
                   else
                   if(fault>=49&&fault<=64)
                  {
                       x=P_E2PROM[128]&(0x0001<<(fault-49));
                       if(x!=0)data_b[j++]=place;
                  }
                  else
                  if(fault>=65&&fault<=80)
                  {
                      x=P_E2PROM[129]&(0x0001<<(fault-65));
                      if(x!=0)data_b[j++]=place;
                  }
                  else
                  if(fault>=81&&fault<=96)
                  {
                      x=P_E2PROM[130]&(0x0001<<(fault-81));
                      if(x!=0)data_b[j++]=place;
                  }
                   /*  for(x=0;x<6;x++)
                    {
                        if(fault>=16*x+1&&fault<=16*(x+1))
                        {
                            if(P_E2PROM[140+x]&(0x0001<<(fault%16-1))!=0)
                              data_b[j++]=place;
                        }
                    }*/
               }
               else
               {
                  data_b[j++]=place;
               }
	    }


		if(place==0) place=499;
		else           place--;
	}

	for(i=0;i<j;i++)
		data[i]=data_b[i];

	data[500]=j;
}

	
unsigned int READ_fault_com(unsigned int fist_num,unsigned int num)//�����ϴ���
{
	unsigned int i,k,m;

	for(m=0;m<4;m++)
	{
		if(error_dis_now[m]!=1000)//�����ʾ��
		{
				k=(num+m+1)%100;
				
				LCD_Ddata_buffer[fist_num++]=(((num+m+1)/100+0x30)<<8)|(k/10+0x30);
				LCD_Ddata_buffer[fist_num++]=(k%10<<8)+0x302e;				
		
		
				LCD_Ddata_buffer[fist_num++]=0x3230;//��
				
				i=error_four_data[m][1];i=(i/10<<4)+i%10;
				LCD_Ddata_buffer[fist_num++]=(((i>>4)<<8)|(i&0xf))+0x3030;//��
		
				i=error_four_data[m][2];i=(i/10<<4)+i%10;
				LCD_Ddata_buffer[fist_num++]=(i>>4)+0x5f30;//_��	
				k=i&0x0f;//�µ��°벿��
				LCD_Ddata_buffer[fist_num++]=(k<<8)+0x305f;//��_
			
				i=error_four_data[m][3];i=(i/10<<4)+i%10;
				LCD_Ddata_buffer[fist_num++]=(((i>>4)<<8)|(i&0xf))+0x3030;//��
		
				i=error_four_data[m][4];i=(i/10<<4)+i%10;
				LCD_Ddata_buffer[fist_num++]=(i>>4)+0x2030;//// ʱ
				k=i&0x0f;//�µ��°벿��
				LCD_Ddata_buffer[fist_num++]=(k<<8)+0x303a;//ʱ:
				
				i=error_four_data[m][5];i=(i/10<<4)+i%10;
				LCD_Ddata_buffer[fist_num++]=(((i>>4)<<8)|(i&0xf))+0x3030;//��
		
		
//				i=error_four_data[m][7];//ģ���
//				for(k=0;k<4;k++)
//					LCD_Ddata_buffer[fist_num++]=MODE_num_com[i][k];
				
				i=error_four_data[m][6];//���ϴ���	
				
				if((i&0x8000)==0x8000)
				{
					LCD_Ddata_buffer[fist_num++]=0x323a;
					i&=0x0fff;
					
					for(k=0;k<9;k++)
						LCD_Ddata_buffer[fist_num++]=ERR_code_com[i][k];
				}
				else
				{
		
					for(k=0;k<10;k++)
						LCD_Ddata_buffer[fist_num++]=ERR_code_com[i][k];
				}
				
				for(k=0;k<4;k++)
					LCD_Ddata_buffer[fist_num++]=0x2020;
		}
		else
		{
			for(i=0;i<24;i++) 
				LCD_Ddata_buffer[fist_num++]=0x2020;	
		}
	}	
	
	LCD_Ddata_buffer[fist_num++]=error_addr[500];//һ�����ٸ�����

	return fist_num;
}

//***************�汾��Ϣ**********************
unsigned int ADD_edit(unsigned int fist_num,unsigned int fist_data,unsigned int second_data)//�汾��Ϣ//&&&&
{
	unsigned int i,j,n=0;
	unsigned int buffer=0;//
	
	n=fist_num;
		
	for(i=0;i<4;i++)//ǰ��С����
	{	
		j=fist_data>>12;
		fist_data<<=4;		
			if(j>9) j+=0x37;
			else    j+=0x30;			
			if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|j;buffer=0;}//����
			else          buffer=j<<8;				
		if(i==1||i==3)//С����
		{
			if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|0x2e;buffer=0;}//С����
			else          buffer=0x2e00;
		}
		
	}

	for(i=0;i<4;i++)//���λ
	{	
		j=second_data>>12;
		second_data<<=4;
		
		if(j>9) j+=0x37;
		else    j+=0x30;		
		if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|j;buffer=0;}//����
		else          buffer=j<<8;
		
		if(i==1)
		{			
			LCD_Ddata_buffer[fist_num++]=0x2e42;//д��.B
			buffer=0;
		}			
	}
	
	while((fist_num-n)<6)//
		LCD_Ddata_buffer[fist_num++]=0x2020;	//д��
		
		return fist_num;
} 

unsigned int ADD_edit_inside(unsigned int fist_num,unsigned int fist_data,unsigned int second_data)//�ڲ��汾��Ϣ//&&&&
{
  	unsigned int i,j,n=0;
	unsigned int buffer=0x5600;//
	
	n=fist_num;
	for(i=0;i<4;i++)//ǰ��С����
	{	
		j=fist_data>>12;
		fist_data<<=4;		
			if(j>9) j+=0x37;
			else    j+=0x30;			
			if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|j;buffer=0;}//����
			else          buffer=j<<8;				
		if(i==1)//С����
		{
			if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|0x2e;buffer=0;}//С����
			else          buffer=0x2e00;
		}
		
	}

	for(i=0;i<4;i++)//���λ
	{	
		j=second_data>>12;
		second_data<<=4;
		
		if(j>9) j+=0x37;
		else    j+=0x30;		
		if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|j;buffer=0;}//����
		else          buffer=j<<8;
		
		if(i==0)
		{			
			if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|0x2e;buffer=0;}//С����
			else          buffer=0x2e00;
		}	
		if(i==3)
		{
		  if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|0x20;buffer=0;}//���һλ���油һ���ո�Ȼ����ʾ
			
		}		
	}
	
	while((fist_num-n)<6)//
		LCD_Ddata_buffer[fist_num++]=0x2020;	//д��
		
		return fist_num;
}
//**************�����������**********************
unsigned int ADD_Pnum(unsigned int fist_num,unsigned int char_num,const unsigned char *character)//�汾��Ϣ   
{				
	unsigned int i,j;
	unsigned int buffer,tran_data;//
	
		j=fist_num;
	
				LCD_Ddata_buffer[fist_num++]=((P_E2PROM[23]/10<<8)|(P_E2PROM[23]%10))+0x3030;
				buffer=0;
				
				for(i=0;i<char_num;i++)
				{
					tran_data=character[i];
					if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|tran_data;buffer=0;}//����
					else          buffer=tran_data<<8;
				}
				
					tran_data=(P_E2PROM[24]/10)+0x30;
					if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|tran_data;buffer=0;}//����
					else          buffer=tran_data<<8;
					
					tran_data=(P_E2PROM[24]%10)+0x30;
					if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|tran_data;buffer=0;}//����
					else          buffer=tran_data<<8;
					
					tran_data=(P_E2PROM[25]/100)+0x30;
					if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|tran_data;buffer=0;}//����
					else          buffer=tran_data<<8;
					
					tran_data=((P_E2PROM[25]%100)/10)+0x30;
					if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|tran_data;buffer=0;}//����
					else          buffer=tran_data<<8;
					
					tran_data=(P_E2PROM[25]%10)+0x30;
					if(buffer!=0) {LCD_Ddata_buffer[fist_num++]=buffer|tran_data;buffer=0;}//����
					else          buffer=tran_data<<8;

				while(fist_num-j<8)
				{
					if(buffer==0) LCD_Ddata_buffer[fist_num++]=0x2020;
					else          {LCD_Ddata_buffer[fist_num++]=buffer|0x20;buffer=0;}
				}
				
				return fist_num;
}

//************��ʾ��Ϣ****************************************************
void pro_message(void)//��ʾ��Ϣmessage_num��λ��һ��ȫ�ֱ�����һ��ȫ����ʾ����
{                     //��fist_addr��ʼ�ĵ�ַ��д��data��ʼ��num������
	unsigned int i,j,z,x;//z������Ϊ��ʾ���ش���ı�ʶ
	if(LCD_Tbuffer_sign==1&&message_num!=0)//����ʾ��ϢҪ����
	{
		j=message_num&0x00ff;		
		if(j!=0)//������µ�����Ҫ��ʾ
		{					
			if((message_num&0x8000)==0)//��ʾ��Ϣ
			{
				for(i=0;i<12;i++)
					LCD_Ddata_buffer[i]=MESSAGE_com[j][i];	//  ��ʾ��Ϣ
					
				message_num<<=8;//�´β�������
                                RS232_Slcd_data(0,0x1af4,12,LCD_Ddata_buffer);LCD_page_dis--;//��������
				DELAY_message=0;
			}
			else   //���ϴ���
			{
                            if(P_E2PROM[124]==1)
                            {
                                z=0;
                                if(j>=1&&j<=16)
                                {
                                    x=P_E2PROM[125]&(0x0001<<(j-1));
                                   if(x!=0) z=1;
                                }
                                else
                                if(j>=17&&j<=32)
                                {
                                    x=P_E2PROM[126]&(0x0001<<(j-17));
                                    if(x!=0)z=1;
                                }
                                else
                                if(j>=33&&j<=48)
                                {
                                    x=P_E2PROM[127]&(0x0001<<(j-33));
                                   if(x!=0)z=1;
                                }
                                else
                                if(j>=49&&j<=64)
                                {
                                    x=P_E2PROM[128]&(0x0001<<(j-49));
                                    if(x!=0)z=1;

                                }
                                else
                                if(j>=65&&j<=80)
                                {
                                    x=P_E2PROM[129]&(0x0001<<(j-65));
                                    if(x!=0)z=1;
                                }
                                else
                                if(j>=81&&j<=96)
                                {
                                    x=P_E2PROM[130]&(0x0001<<(j-81));
                                    if(x!=0)z=1;
                                }
                                /*for(x=0;x<6;x++)
                               {
                                   if(j>=16*x+1&&j<=16*(x+1))
                                   {
                                      if(P_E2PROM[140+x]&(0x0001<<(fault%16-1))!=0)
                                      z=1;
                                   }
                               }*/
                                if(z==1)
                                {
				   for(i=0;i<10;i++)
					LCD_Ddata_buffer[i]=ERR_code_com[j][i];	//   ���ϴ���
				   LCD_Ddata_buffer[i++]=0x2020;
				   LCD_Ddata_buffer[i++]=0x2020;


                                   RS232_Slcd_data(0,0x1af4,12,LCD_Ddata_buffer);LCD_page_dis--;//��������
				   DELAY_message=0;
                                }
                                message_num<<=8;//�´β�������
				message_num|=0x8000;

		            }
                            else
                            {
                                for(i=0;i<10;i++)
				LCD_Ddata_buffer[i]=ERR_code_com[j][i];	//   ���ϴ���
				LCD_Ddata_buffer[i++]=0x2020;
				LCD_Ddata_buffer[i++]=0x2020;

				message_num<<=8;//�´β�������
				message_num|=0x8000;
                                RS232_Slcd_data(0,0x1af4,12,LCD_Ddata_buffer);LCD_page_dis--;//��������
				DELAY_message=0;
                            }


			}

		}
		else
		if(DELAY_message>=100000)//�Զ����
		{
			for(i=0;i<12;i++)
				LCD_Ddata_buffer[i]=0x2020;			
			RS232_Slcd_data(0,0x1af4,12,LCD_Ddata_buffer);LCD_page_dis--;//��������	
						
			message_num=0;	
		}	
		else
		{
			if(message_num==0x0600||message_num==0x0A00||message_num==0x0C00)//��ʾ������
			{
				DELAY_message=0;	
			}
			else
			if((message_num&0x8000)!=0)
			{
				if(equipment_skate[0][2]==2)//������ڹ���״̬
				{
					DELAY_message=0;	
				}	
			}	
		}		
	}	
}

//******************��״̬����ѯ��**********************************
void LCD_habit(void)//����״̬���  ���⣬ָʾ��
{
	unsigned int j;
	
	if(LCD_Tbuffer_sign==1)
	{
		if(equipment_skate[0][2]!=equipment_skate_sign)//���״̬�ı�
		{	
			equipment_skate_sign=equipment_skate[0][2];
			LCD_Ddata_buffer[0]=equipment_skate[0][2];
			
			LCD_Ddata_buffer[1]=0;
			
			if(which_equ==3) LCD_Ddata_buffer[2]=1;
			else			 LCD_Ddata_buffer[2]=0;

			RS232_Slcd_data(0,0x1B58,3,LCD_Ddata_buffer);LCD_page_dis--;
			
			LCD_page_dis=0;//������ʾ
			
			if(equipment_skate_sign==3)
			{
				if(P_E2PROM[30]==1) message_num=6;//��ʾ��ͣ
				else
				{
					message_num=0x8018;	//ͨѶ����
				}	
			}
			else DELAY_message=100000;//���״̬�仯���ü�ʱ��ȥ��ʾ��Ϣ
		}
		else
		if(DELAY_SLEEP>8000)//����䰵��800msѯ���Ƿ��д���
		{
			DELAY_SLEEP=0;	
			RS232_Slcd_record(0,3,0);LCD_page_dis--;//ѯ���Ƿ��д���			
		}
		else
		if(LED_state==1&&DELAY_Blight>=DELAY_BLIGHT_SIGN)//�����ʱʱ���Ѿ�����
		{
			LED_state=0;//��Ļд��
			RS232_Slcd_record(0,1,P_E2PROM[6]);LCD_page_dis--;//д��ر�����
		}
		else
		if(LED_state==0x81)//������Ļ  ��Ƶ�״�����ʶ
		{
			LED_state=1;//��Ļ����
			j=RS232_Slcd_record_nolast(0,1,P_E2PROM[5]);//д�뿪������
			RS232_Slcd_record(j,4,0);LCD_page_dis--;
		}
		else
		if(DELAY_ASKTIME>250000)//25s
		{
                        LCD_Ddata_buffer[0]=equipment_skate[0][2];//��״̬��ʾˢ�·�ֹʵ��״̬����ʾ״̬��һ��

	                LCD_Ddata_buffer[1]=0;

		        if(which_equ==3) LCD_Ddata_buffer[2]=1;
		        else			 LCD_Ddata_buffer[2]=0;

		        j=RS232_Slcd_data_nolast(0,0x1B58,3,LCD_Ddata_buffer);
			DELAY_ASKTIME=0;//��ʱ����
			RS232_Slcd_record(j,5,0);LCD_page_dis--;//ѯ�ʵ�ǰʱ��
		}
		else//��¼���ϴ���
		if(error_write!=error_read)//�й��ϴ�����Ҫ��¼
		{			
			if(change_data_only==0) 
			{						
				j=0;
				while(error_write!=error_read)
				{					
					LCD_Ddata_buffer[0]=((add_error_data[error_read][6]&0xff)<<8)|(add_error_data[error_read][7]&0xff);		
					j=RS232_Slcd_data_nolast(j,9000+add_error_data[error_read][10],1,LCD_Ddata_buffer);
					j=RS232_Slcd_data_nolast(j,10*add_error_data[error_read][10]+10000,10,add_error_data[error_read]);
					
					if(LCD_page_sign==0x401)  
					{
						//LCD_page_dis=0;//����ڹ��ϲ˵��������¸���
						LCD_page_dis=0xaaaa;
					}
					else		
					if((add_error_data[error_read][6]&0xff)==60)//��Ҫ��������ҳ��
					{
						LCD_page_sign=0x401;
						//LCD_page_dis=0;
						LCD_page_dis=0xaaaa;
					}
		
					if(error_read==14)	 error_read=0;
					else     			 error_read++;						
				}	
	
				RS232_Slcd_record(j,12,0);LCD_page_dis--;//д�붨ʱ����
				if(LCD_page_sign==0x401)  LCD_page_dis=0xaaaa;//LCD_page_dis=0;//����ڹ��ϲ˵��������¸���

				change_data_only=0x5a;
				DELAY_TIME=0;
			}
			else
			if(change_data_only==0x5a)
			{
				RS232_Slcd_record(0,8,0);LCD_page_dis--;//���Ƿ����
				change_data_only=0xaaaa;	
			}
			else
			{
				if(DELAY_TIME>=5000)//50ms
				{
					message_num=13;							
				}						
			}
		}
	
		//else
		//if(�豸״̬�Ƿ�仯)����仯д�µ�״̬
	}
}	

//**************ҳ�����********************************************
void RUN_stop_1(void)//�����ж�ֹͣ����
{
//	unsigned int i,j,k=0;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{		
		if(LCD_Tbuffer_sign==1)
		{
				if(RUN_stop==1) RS232_Slcd_record(0,0,53);//��ת��ҳ��
			else if(RUN_stop==2) RS232_Slcd_record(0,0,54);	
			else if(RUN_stop==3) RS232_Slcd_record(0,0,56);
		}
	}
	else
	{	 

	}
}
void LCD_page_101(void)//��ҳ-ϵͳ����
{
	unsigned int i,j,k=0;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
				if(equipment_skate_sign!=3) j=RS232_Slcd_record_nolast(0,0,2);
				else 						j=RS232_Slcd_record_nolast(0,0,73);				
							
				i=0;
				LCD_page_dis_b=0x12;//����			
				i=ADD_data_guide(i,245,89,0x0010,9,0,P_E2PROM[20]);
				i=ADD_data_guide(i,245,113,0x0010,9,0,P_E2PROM[20]);
				i=ADD_data_guide(i,245,137,0x0010,9,0,P_E2PROM[29]);
								
				j=RS232_Slcd_data(j,0x1200,i,LCD_Ddata_buffer);	//д����������Ƿ���ʾ
		}				
	}
	else
	{	 
		 if((DELAY_refresh_data_com>P_E2PROM[21]*10)&&LCD_Tbuffer_sign==1)//��ʾ��ʱ
		 {
			 DELAY_refresh_data_com=0;//��ʱ
			 k=0;
			 for(j=0;j<3;j++)
				LCD_Ddata_buffer[k++]=WIRE_cost[0][j];//������ѹ
			for(j=0;j<3;j++)
				LCD_Ddata_buffer[k++]=WIRE_cost[1][j];//��������
			for(j=0;j<3;j++)
				LCD_Ddata_buffer[k++]=WIRE_cost[2][j];//���ص���
			for(j=0;j<3;j++)
				LCD_Ddata_buffer[k++]=WIRE_cost[3][j];//��������		
			for(j=0;j<3;j++)
				LCD_Ddata_buffer[k++]=WIRE_cost[4][j];//��������
			for(j=0;j<3;j++)
				LCD_Ddata_buffer[k++]=WIRE_cost[5][j];//���ع���
//			for(j=0;j<3;j++)
//				LCD_Ddata_buffer[k++]=LOAD_cost[0][0];//����Ƶ��
			
			LCD_Ddata_buffer[k++]=WIRE_cost[6][0];
			LCD_Ddata_buffer[k++]=WIRE_cost[6][1];//�������
			LCD_Ddata_buffer[k++]=WIRE_cost[6][2];//�������
			 			 
			 RS232_Slcd_data(0,0,k,LCD_Ddata_buffer);
			 LCD_page_dis=1;	 
		 }
	}
}

void LCD_page_102(void)//��ҳ-��������
{
	unsigned int i,j,k=0;
	unsigned int xx,yy;
	unsigned int place,y_num;//������
	unsigned int size;//�����С
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			if(equipment_skate_sign!=3) i=RS232_Slcd_record_nolast(0,0,3);//ҳ����ת
			else 						i=RS232_Slcd_record_nolast(0,0,74);		
			
				xx=20;
				yy=40;//��һ��λ�ÿ�ʼ
				j=0;					
			j=ADD_icon_guide(j,0x18,xx,yy+P_E2PROM[15]*24,1,P_E2PROM[15]);//��������THDֵ
			j=ADD_icon_guide(j,0x19,xx,yy+P_E2PROM[16]*24,2,P_E2PROM[16]);//���ص���THDֵ
			j=ADD_icon_guide(j,0x1A,xx,yy+P_E2PROM[26]*24,3,P_E2PROM[26]);//������ѹTHDֵ
			j=ADD_icon_guide(j,0x1B,xx,yy+P_E2PROM[17]*24,4,P_E2PROM[17]);//������������
			j=ADD_icon_guide(j,0x1C,xx,yy+P_E2PROM[18]*24,5,P_E2PROM[18]);//���ع�������
			j=ADD_icon_guide(j,0x1D,xx,yy+P_E2PROM[106]*24,6,P_E2PROM[106]);//��ʾ���ع���	
				
			i=RS232_Slcd_data_nolast(i,0x12f0,48,LCD_Ddata_buffer);	//д����ʮ�˸�����		
			
			LCD_Ddata_buffer[0]=P_E2PROM[15]!=0;
			LCD_Ddata_buffer[1]=P_E2PROM[16]!=0;
			LCD_Ddata_buffer[2]=P_E2PROM[26]!=0;
			LCD_Ddata_buffer[3]=P_E2PROM[17]!=0;
			LCD_Ddata_buffer[4]=P_E2PROM[18]!=0;
			LCD_Ddata_buffer[5]=P_E2PROM[106]!=0;
			
			RS232_Slcd_data(i,0x18,6,LCD_Ddata_buffer);	//д�����������Ƿ���ʾ	
		}	
	}
	else
	if(LCD_page_dis==1)//
	{	
		if(LCD_Tbuffer_sign==1)
		{
				xx=135;
				yy=66;
				i=0;		
				size=9;
				LCD_page_dis_b=0x22;//����
				
			while(i<234)  
		 	{
			 	i=ADD_data_guide(i,xx,yy,0xffe0,size,1,0);
			 	i=ADD_data_guide(i,xx+45,yy,0x37c7,size,1,0);
			 	i=ADD_data_guide(i,xx+90,yy,0xf986,size,1,0);	
			 }
			 
			if(P_E2PROM[106]!=0)
		 	{
			 	place=(P_E2PROM[106]-1)*39;	
				y_num=yy+(P_E2PROM[106]-1)*24;
				LCD_page_dis_b=0x2e;						
				i=ADD_data_guide(place,xx,y_num,0xffe0,size,1,1);
				i=ADD_data_guide(place+13,xx+45,y_num,0x37c7,size,1,1);
				i=ADD_data_guide(place+26,xx+90,y_num,0xf986,size,1,1);
		 	}
			
		if(WIRE_cost[1][0]>=P_E2PROM[92]||WIRE_cost[1][1]>=P_E2PROM[92]||WIRE_cost[1][2]>=P_E2PROM[92])
		{			 	
		 	if(P_E2PROM[15]!=0)
		 	{			 	
				place=(P_E2PROM[15]-1)*39;
				y_num=yy+(P_E2PROM[15]-1)*24;
				LCD_page_dis_b=0x1f;						
				i=ADD_data_guide(place,xx,y_num,0xffe0,size,1,1);
				i=ADD_data_guide(place+13,xx+45,y_num,0x37c7,size,1,1);
				i=ADD_data_guide(place+26,xx+90,y_num,0xf986,size,1,1);
		 	}	
			
		 	if(P_E2PROM[16]!=0)
		 	{
			 	place=(P_E2PROM[16]-1)*39;	
				y_num=yy+(P_E2PROM[16]-1)*24;
				LCD_page_dis_b=0x22;					
				i=ADD_data_guide(place,xx,y_num,0xffe0,size,1,1);
				i=ADD_data_guide(place+13,xx+45,y_num,0x37c7,size,1,1);
				i=ADD_data_guide(place+26,xx+90,y_num,0xf986,size,1,1);
		 	}
		 	
		 	if(P_E2PROM[26]!=0)
		 	{
			 	place=(P_E2PROM[26]-1)*39;	
				y_num=yy+(P_E2PROM[26]-1)*24;
				LCD_page_dis_b=0x25;						
				i=ADD_data_guide(place,xx,y_num,0xffe0,size,1,1);
				i=ADD_data_guide(place+13,xx+45,y_num,0x37c7,size,1,1);
				i=ADD_data_guide(place+26,xx+90,y_num,0xf986,size,1,1);
		 	}
		 	
		 	if(P_E2PROM[17]!=0)
		 	{
			 	place=(P_E2PROM[17]-1)*39;	
				y_num=yy+(P_E2PROM[17]-1)*24;	
				LCD_page_dis_b=0x28;					
				i=ADD_data_guide(place,xx,y_num,0xffe0,size,2,1);
				i=ADD_data_guide(place+13,xx+45,y_num,0x37c7,size,2,1);
				i=ADD_data_guide(place+26,xx+90,y_num,0xf986,size,2,1);
		 	}
		 	
		 	if(P_E2PROM[18]!=0)
		 	{
			 	place=(P_E2PROM[18]-1)*39;	
				y_num=yy+(P_E2PROM[18]-1)*24;	
				LCD_page_dis_b=0x2b;					
				i=ADD_data_guide(place,xx,y_num,0xffe0,size,2,1);
				i=ADD_data_guide(place+13,xx+45,y_num,0x37c7,size,2,1);
				i=ADD_data_guide(place+26,xx+90,y_num,0xf986,size,2,1);
		 	}

		 	change_data=1;//��ʾ
		}
		else
		{
			change_data=0;//�͵�����ʾ����ʾ����
		}

		 	j=RS232_Slcd_data_nolast(0,0x1330,117,LCD_Ddata_buffer);	//д����������Ƿ���ʾ
		 	RS232_Slcd_data(j,0x13A5,117,LCD_Ddata_buffer+117);	//д����������Ƿ���ʾ
		} 
	}
	else
	if(LCD_page_dis==2)//
	{
		 
		 if((DELAY_refresh_data_com>P_E2PROM[21]*10)&&LCD_Tbuffer_sign==1)//��ʾ��ʱ
		 {
			 	DELAY_refresh_data_com=0;//��ʱ	
			 	k=0;			 	
			 	
			 	for(j=0;j<3;j++)
					LCD_Ddata_buffer[k++]=LOAD_cost[4][j];//��������������
				for(j=0;j<3;j++)
					LCD_Ddata_buffer[k++]=LOAD_cost[5][j];//���ص���������
				for(j=0;j<3;j++)
					LCD_Ddata_buffer[k++]=LOAD_cost[3][j];//������ѹ������
				for(j=0;j<3;j++)
					LCD_Ddata_buffer[k++]=LOAD_cost[1][j];//������������
				for(j=0;j<3;j++)
					LCD_Ddata_buffer[k++]=LOAD_cost[2][j];//���ع�������
				for(j=0;j<3;j++)
					LCD_Ddata_buffer[k++]=LOAD_cost[0][0];//����Ƶ��
		 
			  	RS232_Slcd_data(0,0x1f,18,LCD_Ddata_buffer);//����
			  	
			  	
			  	//******ȷ���´������Ƿ���ʾ*********
			  	if(WIRE_cost[1][0]>=P_E2PROM[92]||WIRE_cost[1][1]>=P_E2PROM[92]||WIRE_cost[1][2]>=P_E2PROM[92])//��ʾ
			  	{
				    if(change_data==0) LCD_page_dis=1;//��ǰ����ʾ״̬
				    else               LCD_page_dis=2; 
				    
				 }
				 else//����ʾ
				 {
					if(change_data==0) LCD_page_dis=2;//��ǰ����ʾ״̬
				    else          	   LCD_page_dis=1; 
				 } 		  	  	 
		 }
	}
}

void LCD_page_103(void)//��ҳ-��������
{
	unsigned int i,j,k=0;
	unsigned int xx,yy;
//	unsigned int LCD_num;//��Ļ�ߴ�
	unsigned int place,y_num;//������
	unsigned int size;//���¼��
	
	if(LCD_page_dis==0)//�����һ�ν���
	{			
		if(LCD_Tbuffer_sign==1)
		{
			i=RS232_Slcd_record_nolast(0,0,4);//ҳ����ת
			
			if(equipment_skate_sign!=3) j=RS232_Slcd_record_nolast(0,0,4);
			else 						j=RS232_Slcd_record_nolast(0,0,75);

				xx=35;
				yy=40;//��һ��λ�ÿ�ʼ
				j=0;					
			j=ADD_icon_guide(j,0x33,xx,yy+P_E2PROM[107]*24,9,P_E2PROM[107]);//ֱ����
			j=ADD_icon_guide(j,0x34,xx,yy+P_E2PROM[108]*24,10,P_E2PROM[108]);//�Ƿ���ʾ�¶�
			j=ADD_icon_guide(j,0x35,xx,yy+P_E2PROM[31]*24,7,P_E2PROM[31]);//�Ƿ���ʾ˫�¶�
			j=ADD_icon_guide(j,0x36,xx,yy+P_E2PROM[27]*24,8,P_E2PROM[27]);//�Ƿ���ʾװ�����
			j=ADD_icon_guide(j,0x37,xx,yy+P_E2PROM[109]*24,11,P_E2PROM[109]);//�Ƿ���ʾֱ�����
			
			i=RS232_Slcd_data_nolast(i,0x1ff8,40,LCD_Ddata_buffer);	//д����ʮ�˸�����		
			
			LCD_Ddata_buffer[0]=P_E2PROM[107]!=0;
			LCD_Ddata_buffer[1]=P_E2PROM[108]!=0;
			LCD_Ddata_buffer[2]=P_E2PROM[31]!=0;
			LCD_Ddata_buffer[3]=P_E2PROM[27]!=0;
			LCD_Ddata_buffer[4]=P_E2PROM[109]!=0;		
			RS232_Slcd_data(i,0x33,5,LCD_Ddata_buffer);	//д�����������Ƿ���ʾ	
		}
	}
	else
	if(LCD_page_dis==1)//
	{	
		if(LCD_Tbuffer_sign==1)
		{
		 		xx=210;
				yy=66;
				i=0;		
				size=9;
				
			while(i<65)  
			 	i=ADD_data_guide(i,xx,yy,0x0010,size,1,0);
		 	
		 	
		 	if(P_E2PROM[107]!=0)//ֱ����
		 	{			 	
				place=(P_E2PROM[107]-1)*13;
				y_num=yy+(P_E2PROM[107]-1)*24;
				LCD_page_dis_b=0x39;						
				i=ADD_data_guide(place,xx,y_num,0x0010,size,0,1);
		 	}	
		 	
		 	if(P_E2PROM[108]!=0)//�Ƿ���ʾһ���¶�
		 	{			 	
				place=(P_E2PROM[108]-1)*13;
				y_num=yy+(P_E2PROM[108]-1)*24;
				LCD_page_dis_b=0x3A;						
				i=ADD_data_guide(place,xx,y_num,0x0010,size,0,1);
		 	}
		 	
		 	if(P_E2PROM[31]!=0)//�Ƿ���ʾ�����¶�
		 	{			 	
				place=(P_E2PROM[31]-1)*13;
				y_num=yy+(P_E2PROM[31]-1)*24;
				LCD_page_dis_b=0x3A;						
				i=ADD_data_guide(place,xx,y_num,0x0010,size,0,1);
				i=ADD_data_guide(place+13,xx,y_num+24,0x0010,size,0,1);
		 	}
		 	
		 	if(P_E2PROM[27]!=0)//�Ƿ���ʾװ�����
		 	{			 	
				place=(P_E2PROM[27]-1)*13;
				y_num=yy+(P_E2PROM[27]-1)*24;
				LCD_page_dis_b=0x3C;						
				i=ADD_data_guide(place,xx,y_num,0x0010,size,3,1);
		 	}
		 	
		 	if(P_E2PROM[109]!=0)//�Ƿ���ʾ�����¶�
		 	{			 	
				place=(P_E2PROM[109]-1)*13;
				y_num=yy+(P_E2PROM[109]-1)*24;
				LCD_page_dis_b=0x39;						
				i=ADD_data_guide(place,xx,y_num,0x0010,size,0,1);
				LCD_page_dis_b=0x3D;
				i=ADD_data_guide(place+13,xx,y_num+24,0x0010,size,0,1);
		 	}

		 	RS232_Slcd_data(0,0x2030,65,LCD_Ddata_buffer);	//д����������Ƿ���ʾ
		} 
	}
	else
	if(LCD_page_dis==2)//
	{		 
		 if((DELAY_refresh_data_com>P_E2PROM[21]*10)&&LCD_Tbuffer_sign==1)//��ʾ��ʱ
		 {
			 	DELAY_refresh_data_com=0;//��ʱ	
			 	k=0;				 	
			 	
				LCD_Ddata_buffer[k++]=LOAD_cost[6][0];//ֱ�����ѹһ
				LCD_Ddata_buffer[k++]=LOAD_cost[6][1];//�¶�һ
				LCD_Ddata_buffer[k++]=LOAD_cost[7][1];//�¶ȶ�
				LCD_Ddata_buffer[k++]=LOAD_cost[7][0];//װ�����
				LCD_Ddata_buffer[k++]=record_single[5][0];//ֱ�����ѹ��
				
			  	RS232_Slcd_data(0,0x39,5,LCD_Ddata_buffer);//����
			  				  	
			  	LCD_page_dis=2;			  	  	 
		 }
	}	
}

void LCD_page_201(void)//����Ƶ��-������ѹ����
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			if((which_equ==3||which_equ==4)&&P_E2PROM[80]==0) j=76;
			else             j=6;
		
			i=RS232_Slcd_record_nolast(0,0,j);
			RS232_Slcd_data(i,0x50,1,P_E2PROM+74);
				

//			RS232_Slcd_data(i,0x14e0,30,LCD_Ddata_buffer);
		}
	}
	else
	if(LCD_page_dis==1)//
	{	
		if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
		{			 
			if(P_E2PROM[80]==1||which_equ==2)	 //3.5 80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
				LCD_wave_calculation(P_E2PROM[74],record_wave[0],record_wave[0],record_wave_b,record_wave[1],record_wave_b1,record_wave[2],record_wave_b2,zero_wave[0],zero_wave[1]);
			else if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
				LCD_wave_calculation(P_E2PROM[74],record_wave[0],record_wave[0],record_wave_b,record_wave[3],record_wave_b1,record_wave[6],record_wave_b2,record_wave[9],zero_wave[1]);
					
			j=ADD_WAVE_ONE_no(0,1,record_wave_b);
			j=ADD_WAVE_ONE_no(j,2,record_wave_b1);
			j=ADD_WAVE_ONE_no(j,4,record_wave_b2);	//���η���
			
			if(which_equ==3&&P_E2PROM[80]==0) j=ADD_WAVE_ONE_no(j,8,zero_wave[1]);//���Ͳ�������
	
			i=0;
			LCD_Ddata_buffer[i++]=coordinate_now;
			LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
			LCD_Ddata_buffer[i++]=coordinate_now>>1;
			LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
			LCD_Ddata_buffer[i++]=0;
			LCD_Ddata_buffer[i++]=-((coordinate_now>>1)-(coordinate_now>>2));
			LCD_Ddata_buffer[i++]=-(coordinate_now>>1);
			LCD_Ddata_buffer[i++]=-((coordinate_now>>1)+(coordinate_now>>2));
			LCD_Ddata_buffer[i++]=-coordinate_now;	
			
			if(P_E2PROM[80]==1||which_equ==2)	 //3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
			{
				LCD_Ddata_buffer[i++]=WIRE_cost[0][0];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[0][1];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[0][2];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=LOAD_cost[3][0];
				LCD_Ddata_buffer[i++]=LOAD_cost[3][1];
				LCD_Ddata_buffer[i++]=LOAD_cost[3][2];	
			}	
			else 
			if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
			{
				LCD_Ddata_buffer[i++]=LOAD_cost[1][0];
				LCD_Ddata_buffer[i++]=LOAD_cost[2][0];;
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;	
			}	
			RS232_Slcd_data(j,0x41,15,LCD_Ddata_buffer);
			
			LCD_page_dis=1;
			electric_receive_sign=0;	
		}
	}	
}

void LCD_page_202(void)//����Ƶ��-������������
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			if((which_equ==3||which_equ==4)&&P_E2PROM[80]==0)
                        {
                            j=77;
                        }
                        else
			{
				if(P_E2PROM[20]!=0)	j=7;
				else				j=70;
			}	
			             		

			i=RS232_Slcd_record_nolast(0,0,j);

			RS232_Slcd_data(i,0x64,1,P_E2PROM+75);
		}
	}
	else
	if(LCD_page_dis==1)//
	{	
		if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
		{		
			if(P_E2PROM[80]==1||which_equ==2)//3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
					LCD_wave_calculation(P_E2PROM[75],record_wave[3],record_wave[3],record_wave_b,record_wave[4],record_wave_b1,record_wave[5],record_wave_b2,zero_wave[0],zero_wave[1]);
			else if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
					LCD_wave_calculation(P_E2PROM[75],record_wave[1],record_wave[1],record_wave_b,record_wave[4],record_wave_b1,record_wave[7],record_wave_b2,record_wave[10],zero_wave[1]);	
			
			j=ADD_WAVE_ONE_no(0,1,record_wave_b);
			j=ADD_WAVE_ONE_no(j,2,record_wave_b1);
			j=ADD_WAVE_ONE_no(j,4,record_wave_b2);
			
			if(which_equ==3&&P_E2PROM[80]==0)
				j=ADD_WAVE_ONE_no(j,8,zero_wave[1]);//���Ͳ�������
			else			
			if(P_E2PROM[20]!=0)//����е����������
				j=ADD_WAVE_ONE_no(j,8,zero_wave[1]);
			
			i=0;
			LCD_Ddata_buffer[i++]=WIRE_cost[6][0];
			LCD_Ddata_buffer[i++]=coordinate_now;
			LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
			LCD_Ddata_buffer[i++]=coordinate_now>>1;
			LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
			LCD_Ddata_buffer[i++]=0;
			LCD_Ddata_buffer[i++]=-((coordinate_now>>1)-(coordinate_now>>2));
			LCD_Ddata_buffer[i++]=-(coordinate_now>>1);
			LCD_Ddata_buffer[i++]=-((coordinate_now>>1)+(coordinate_now>>2));
			LCD_Ddata_buffer[i++]=-coordinate_now;	
			
			if(P_E2PROM[80]==1||which_equ==2)	 //3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
				{
				LCD_Ddata_buffer[i++]=WIRE_cost[1][0];//����������Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[1][1];//����������Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[1][2];//����������Чֵ
				LCD_Ddata_buffer[i++]=LOAD_cost[4][0];
				LCD_Ddata_buffer[i++]=LOAD_cost[4][1];
				LCD_Ddata_buffer[i++]=LOAD_cost[4][2];
			}	
			else 
			if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
			{
				LCD_Ddata_buffer[i++]=LOAD_cost[1][1];
				LCD_Ddata_buffer[i++]=LOAD_cost[2][1];
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;	
			}	
			RS232_Slcd_data(j,0x54,16,LCD_Ddata_buffer);
			
			LCD_page_dis=1;
			electric_receive_sign=0;	
		}
	}	
}


void LCD_page_203(void)//����Ƶ��-���ص�������
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			if((which_equ==3||which_equ==4)&&P_E2PROM[80]==0)
                        {
                            j=78;
                        }
                        else
			{
				if(P_E2PROM[20]!=0)	j=8;
				else				j=71;
			}	
			             		
			
			i=RS232_Slcd_record_nolast(0,0,j);
					
			RS232_Slcd_data(i,0x78,1,P_E2PROM+76);
		}
	}
	else
	if(LCD_page_dis==1)//
	{	
		if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
		{		
			if(P_E2PROM[80]==1||which_equ==2)//3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
			LCD_wave_calculation(P_E2PROM[76],record_wave[6],record_wave[6],record_wave_b,record_wave[7],record_wave_b1,record_wave[8],record_wave_b2,zero_wave[1],zero_wave[0]);
			else if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
			LCD_wave_calculation(P_E2PROM[76],record_wave[2],record_wave[2],record_wave_b,record_wave[5],record_wave_b1,record_wave[8],record_wave_b2,record_wave[11],zero_wave[1]);
			j=ADD_WAVE_ONE_no(0,1,record_wave_b);
			j=ADD_WAVE_ONE_no(j,2,record_wave_b1);
			j=ADD_WAVE_ONE_no(j,4,record_wave_b2);	
			
			if(which_equ==3&&P_E2PROM[80]==0)
				j=ADD_WAVE_ONE_no(j,8,zero_wave[1]);//���Ͳ�������
			else
			if(P_E2PROM[20]!=0)//����и����������
				j=ADD_WAVE_ONE_no(j,8,zero_wave[0]);
			
			i=0;
			LCD_Ddata_buffer[i++]=WIRE_cost[6][1];
			LCD_Ddata_buffer[i++]=coordinate_now;
			LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
			LCD_Ddata_buffer[i++]=coordinate_now>>1;
			LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
			LCD_Ddata_buffer[i++]=0;
			LCD_Ddata_buffer[i++]=-((coordinate_now>>1)-(coordinate_now>>2));
			LCD_Ddata_buffer[i++]=-(coordinate_now>>1);
			LCD_Ddata_buffer[i++]=-((coordinate_now>>1)+(coordinate_now>>2));
			LCD_Ddata_buffer[i++]=-coordinate_now;	
			
			if(P_E2PROM[80]==1||which_equ==2)	 //3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
			{
				LCD_Ddata_buffer[i++]=WIRE_cost[2][0];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[2][1];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[2][2];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=LOAD_cost[5][0];
				LCD_Ddata_buffer[i++]=LOAD_cost[5][1];
				LCD_Ddata_buffer[i++]=LOAD_cost[5][2];
			}	
			else 
			if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
			{
				LCD_Ddata_buffer[i++]=LOAD_cost[1][2];
				LCD_Ddata_buffer[i++]=LOAD_cost[2][2];
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=0;	
			}				
			RS232_Slcd_data(j,0x68,16,LCD_Ddata_buffer);
			
			LCD_page_dis=1;
			electric_receive_sign=0;	
		}
	}	
}


void LCD_page_204(void)//����Ƶ��-������ѹƵ��
{
	unsigned int i,j;
	
	if(P_E2PROM[80]==1||which_equ==2)	 //3.5 80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
	{
		if(LCD_page_dis==0)//�����һ�ν���
		{
			if(LCD_Tbuffer_sign==1)
			{
				
				LCD_Ddata_buffer[0]=4;//����
				LCD_Ddata_buffer[1]=0;//����				
				
				i=RS232_Slcd_record_nolast(0,0,9);					
					
				i=RS232_Slcd_data_nolast(i,0x1540,2,LCD_Ddata_buffer);
				RS232_Slcd_data(i,0x90,1,P_E2PROM+77);//���������д��
			}
		}
		else
		{	
			if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
			{			
				LCD_histogram_calculation(P_E2PROM[77],record_wave[0],record_wave_b,record_wave[1],record_wave_b1,record_wave[2],record_wave_b2);		
	
				j=ADD_FRE_fist_no(0,0x1542,50,record_wave_b);
				j=ADD_FRE_second_no(j,0x1542,50,record_wave_b);
				
				j=ADD_FRE_fist_no(j,0x15e2,51,record_wave_b1);
				j=ADD_FRE_second_no(j,0x15e2,51,record_wave_b1);
				
				j=ADD_FRE_fist_no(j,0x1682,52,record_wave_b2);
				j=ADD_FRE_second_no(j,0x1682,52,record_wave_b2);
				
				i=0;
				LCD_Ddata_buffer[i++]=coordinate_now;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=coordinate_now>>1;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=WIRE_cost[0][0];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[0][1];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[0][2];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=LOAD_cost[3][0];
				LCD_Ddata_buffer[i++]=LOAD_cost[3][1];
				LCD_Ddata_buffer[i++]=LOAD_cost[3][2];	
				//*******************************************************
				LCD_Ddata_buffer[i++]=MAX_data[0][0];
				LCD_Ddata_buffer[i++]=MAX_data[0][1];
				LCD_Ddata_buffer[i++]=MAX_data[1][0];
				LCD_Ddata_buffer[i++]=MAX_data[1][1];
				LCD_Ddata_buffer[i++]=MAX_data[2][0];
				LCD_Ddata_buffer[i++]=MAX_data[2][1];
				LCD_Ddata_buffer[i++]=MAX_data[3][0];
				LCD_Ddata_buffer[i++]=MAX_data[3][1];
				
				j=RS232_Slcd_data_nolast(j,0x7d,19,LCD_Ddata_buffer);
				
				LCD_Ddata_buffer[0]=4;//����
				LCD_Ddata_buffer[1]=96;//����	
					
				j=RS232_Slcd_data(j,0x1540,2,LCD_Ddata_buffer);
	
				LCD_page_dis=1;
				electric_receive_sign=0;
			}
		}
	}
	else 
	if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
	{
		if(LCD_page_dis==0)//�����һ�ν���
		{
			if(LCD_Tbuffer_sign==1)
			{
				i=RS232_Slcd_record_nolast(0,0,79);
				RS232_Slcd_data(i,0x90,1,P_E2PROM+77);
			}
		}
		else
		if(LCD_page_dis==1)//
		{	
			if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
			{			 
				LCD_wave_calculation(P_E2PROM[77],record_wave[0],record_wave[0],record_wave_b,record_wave[1],record_wave_b1,record_wave[2],record_wave_b2,zero_wave[0],zero_wave[1]);
			
				j=ADD_WAVE_ONE_no(0,1,record_wave_b);
				j=ADD_WAVE_ONE_no(j,2,record_wave_b1);
				j=ADD_WAVE_ONE_no(j,4,record_wave_b2);	//���η���
		
				i=0;
				LCD_Ddata_buffer[i++]=coordinate_now;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=coordinate_now>>1;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=-((coordinate_now>>1)-(coordinate_now>>2));
				LCD_Ddata_buffer[i++]=-(coordinate_now>>1);
				LCD_Ddata_buffer[i++]=-((coordinate_now>>1)+(coordinate_now>>2));
				LCD_Ddata_buffer[i++]=-coordinate_now;	
					
				RS232_Slcd_data(j,0x41,i,LCD_Ddata_buffer);
				
				LCD_page_dis=1;
				electric_receive_sign=0;	
			}
		}	
	}	
}

void LCD_page_205(void)//����Ƶ��-��������Ƶ��
{
	unsigned int i,j;
	
	if(P_E2PROM[80]==1||which_equ==2)	 //3.5 80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
	{	
		if(LCD_page_dis==0)//�����һ�ν���
		{
			if(LCD_Tbuffer_sign==1)
			{
				
				LCD_Ddata_buffer[0]=4;//����
				LCD_Ddata_buffer[1]=0;//����	

				i=RS232_Slcd_record_nolast(0,0,10);		
								
				i=RS232_Slcd_data_nolast(i,0x1722,2,LCD_Ddata_buffer);
				RS232_Slcd_data(i,0xA9,1,P_E2PROM+78);//���������д��
			}
		}
		else
		{	
			if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
			{			
				LCD_histogram_calculation(P_E2PROM[78],record_wave[3],record_wave_b,record_wave[4],record_wave_b1,record_wave[5],record_wave_b2);		
	
				j=ADD_FRE_fist_no(0,0x1724,50,record_wave_b);
				j=ADD_FRE_second_no(j,0x1724,50,record_wave_b);
				
				j=ADD_FRE_fist_no(j,0x17c4,51,record_wave_b1);
				j=ADD_FRE_second_no(j,0x17c4,51,record_wave_b1);
				
				j=ADD_FRE_fist_no(j,0x1864,52,record_wave_b2);
				j=ADD_FRE_second_no(j,0x1864,52,record_wave_b2);
				
				i=0;
				LCD_Ddata_buffer[i++]=coordinate_now;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=coordinate_now>>1;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=WIRE_cost[0][0];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[0][1];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[0][2];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=LOAD_cost[3][0];
				LCD_Ddata_buffer[i++]=LOAD_cost[3][1];
				LCD_Ddata_buffer[i++]=LOAD_cost[3][2];	
				//*******************************************************
				LCD_Ddata_buffer[i++]=MAX_data[0][0];
				LCD_Ddata_buffer[i++]=MAX_data[0][1];
				LCD_Ddata_buffer[i++]=MAX_data[1][0];
				LCD_Ddata_buffer[i++]=MAX_data[1][1];
				LCD_Ddata_buffer[i++]=MAX_data[2][0];
				LCD_Ddata_buffer[i++]=MAX_data[2][1];
				LCD_Ddata_buffer[i++]=MAX_data[3][0];
				LCD_Ddata_buffer[i++]=MAX_data[3][1];
				
				j=RS232_Slcd_data_nolast(j,0x96,19,LCD_Ddata_buffer);
				
				LCD_Ddata_buffer[0]=4;//����
				LCD_Ddata_buffer[1]=96;//����		
				j=RS232_Slcd_data(j,0x1722,2,LCD_Ddata_buffer);
	
				LCD_page_dis=1;
				electric_receive_sign=0;
			}
		}
	}
	else 
	if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
	{
		if(LCD_page_dis==0)//�����һ�ν���
		{
			if(LCD_Tbuffer_sign==1)
			{
				i=RS232_Slcd_record_nolast(0,0,80);
				RS232_Slcd_data(i,0xA9,1,P_E2PROM+78);
			}
		}
		else
		if(LCD_page_dis==1)//
		{	
			if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
			{			 
				LCD_wave_calculation(P_E2PROM[78],record_wave[3],record_wave[3],record_wave_b,record_wave[4],record_wave_b1,record_wave[5],record_wave_b2,zero_wave[0],zero_wave[1]);
							
				j=ADD_WAVE_ONE_no(0,1,record_wave_b);
				j=ADD_WAVE_ONE_no(j,2,record_wave_b1);
				j=ADD_WAVE_ONE_no(j,4,record_wave_b2);	//���η���
		
				i=0;
				LCD_Ddata_buffer[i++]=coordinate_now;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=coordinate_now>>1;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=-((coordinate_now>>1)-(coordinate_now>>2));
				LCD_Ddata_buffer[i++]=-(coordinate_now>>1);
				LCD_Ddata_buffer[i++]=-((coordinate_now>>1)+(coordinate_now>>2));
				LCD_Ddata_buffer[i++]=-coordinate_now;	
					
				RS232_Slcd_data(j,0x41,i,LCD_Ddata_buffer);
				
				LCD_page_dis=1;
				electric_receive_sign=0;	
			}
		}	
	}	
}

void LCD_page_206(void)//����Ƶ��-���ص���Ƶ��
{
	unsigned int i,j;

	if(P_E2PROM[80]==1||which_equ==2)	 //3.5 80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
	{	
		if(LCD_page_dis==0)//�����һ�ν���
		{
			if(LCD_Tbuffer_sign==1)
			{			
				LCD_Ddata_buffer[0]=4;//����
				LCD_Ddata_buffer[1]=0;//����	

				i=RS232_Slcd_record_nolast(0,0,11);	
	
				i=RS232_Slcd_data_nolast(i,0x1904,2,LCD_Ddata_buffer);
				RS232_Slcd_data(i,0xC2,1,P_E2PROM+79);//���������д��
			}
		}
		else
		{	
			if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
			{			
				LCD_histogram_calculation(P_E2PROM[79],record_wave[6],record_wave_b,record_wave[7],record_wave_b1,record_wave[8],record_wave_b2);		
	
				j=ADD_FRE_fist_no(0,0x1906,50,record_wave_b);
				j=ADD_FRE_second_no(j,0x1906,50,record_wave_b);
				
				j=ADD_FRE_fist_no(j,0x19a6,51,record_wave_b1);
				j=ADD_FRE_second_no(j,0x19a6,51,record_wave_b1);
				
				j=ADD_FRE_fist_no(j,0x1A46,52,record_wave_b2);
				j=ADD_FRE_second_no(j,0x1A46,52,record_wave_b2);
				
				i=0;
				LCD_Ddata_buffer[i++]=coordinate_now;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=coordinate_now>>1;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=WIRE_cost[0][0];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[0][1];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[0][2];//������ѹ��Чֵ
				LCD_Ddata_buffer[i++]=LOAD_cost[3][0];
				LCD_Ddata_buffer[i++]=LOAD_cost[3][1];
				LCD_Ddata_buffer[i++]=LOAD_cost[3][2];	
				//*******************************************************
				LCD_Ddata_buffer[i++]=MAX_data[0][0];
				LCD_Ddata_buffer[i++]=MAX_data[0][1];
				LCD_Ddata_buffer[i++]=MAX_data[1][0];
				LCD_Ddata_buffer[i++]=MAX_data[1][1];
				LCD_Ddata_buffer[i++]=MAX_data[2][0];
				LCD_Ddata_buffer[i++]=MAX_data[2][1];
				LCD_Ddata_buffer[i++]=MAX_data[3][0];
				LCD_Ddata_buffer[i++]=MAX_data[3][1];
				
				j=RS232_Slcd_data_nolast(j,0xAF,19,LCD_Ddata_buffer);
				
				LCD_Ddata_buffer[0]=4;//����
				LCD_Ddata_buffer[1]=96;//����	
				j=RS232_Slcd_data(j,0x1904,2,LCD_Ddata_buffer);
	
				LCD_page_dis=1;
				electric_receive_sign=0;
			}
	
		}
	}
	else 
	if(which_equ==3&&P_E2PROM[80]==0)//3.5 SVG
	{
		if(LCD_page_dis==0)//�����һ�ν���
		{
			if(LCD_Tbuffer_sign==1)
			{
				i=RS232_Slcd_record_nolast(0,0,81);
				RS232_Slcd_data(i,0xC2,1,P_E2PROM+79);
			}
		}
		else
		if(LCD_page_dis==1)//
		{	
			if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
			{			 
				LCD_wave_calculation(P_E2PROM[79],record_wave[6],record_wave[6],record_wave_b,record_wave[7],record_wave_b1,record_wave[8],record_wave_b2,zero_wave[1],zero_wave[0]);
							
				j=ADD_WAVE_ONE_no(0,1,record_wave_b);
				j=ADD_WAVE_ONE_no(j,2,record_wave_b1);
				j=ADD_WAVE_ONE_no(j,4,record_wave_b2);	//���η���
		
				i=0;
				LCD_Ddata_buffer[i++]=coordinate_now;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=coordinate_now>>1;
				LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
				LCD_Ddata_buffer[i++]=0;
				LCD_Ddata_buffer[i++]=-((coordinate_now>>1)-(coordinate_now>>2));
				LCD_Ddata_buffer[i++]=-(coordinate_now>>1);
				LCD_Ddata_buffer[i++]=-((coordinate_now>>1)+(coordinate_now>>2));
				LCD_Ddata_buffer[i++]=-coordinate_now;	
					
				RS232_Slcd_data(j,0x41,i,LCD_Ddata_buffer);
				
				LCD_page_dis=1;
				electric_receive_sign=0;	
			}
		}			
	}	
}

void LCD_page_207(void)//����Ƶ��-������������
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
				if(P_E2PROM[29]!=0)
					i=RS232_Slcd_record_nolast(0,0,72);//��������
				else
					i=RS232_Slcd_record_nolast(0,0,69);//��������
				RS232_Slcd_data(i,0x65,1,P_E2PROM+113);
		}
	}
	else
	if(LCD_page_dis==1)//
	{	
		if(electric_receive_sign==2&&LCD_Tbuffer_sign==1)
		{
                    if(which_equ!=2&&P_E2PROM[80]==0)
                    {
                      RS232_Slcd_record(0,0,76);
                    }
                    else
                    {
			if(which_equ==2||P_E2PROM[80]==1)//3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
					LCD_wave_calculation(P_E2PROM[113],record_wave[3],record_wave[9],record_wave_b,record_wave[10],record_wave_b1,record_wave[11],record_wave_b2,zero_wave[2],zero_wave[1]);
			
			j=ADD_WAVE_ONE_no(0,1,record_wave_b);
			j=ADD_WAVE_ONE_no(j,2,record_wave_b1);
			j=ADD_WAVE_ONE_no(j,4,record_wave_b2);
			
			if(P_E2PROM[29]!=0)//��ʾ�������ನ��
				j=ADD_WAVE_ONE_no(j,8,zero_wave[1]);
			
			i=0;
			LCD_Ddata_buffer[i++]=WIRE_cost[6][2];
			LCD_Ddata_buffer[i++]=coordinate_now;
			LCD_Ddata_buffer[i++]=(coordinate_now>>1)+(coordinate_now>>2);
			LCD_Ddata_buffer[i++]=coordinate_now>>1;
			LCD_Ddata_buffer[i++]=(coordinate_now>>1)-(coordinate_now>>2);
			LCD_Ddata_buffer[i++]=0;
			LCD_Ddata_buffer[i++]=-((coordinate_now>>1)-(coordinate_now>>2));
			LCD_Ddata_buffer[i++]=-(coordinate_now>>1);
			LCD_Ddata_buffer[i++]=-((coordinate_now>>1)+(coordinate_now>>2));
			LCD_Ddata_buffer[i++]=-coordinate_now;	
			
			if(which_equ==2||P_E2PROM[80]==1)	 //3.5 80APF80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
			{
				LCD_Ddata_buffer[i++]=WIRE_cost[3][0];//������Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[3][1];//������Чֵ
				LCD_Ddata_buffer[i++]=WIRE_cost[3][2];//������Чֵ
			}	
			
			RS232_Slcd_data(j,0x54,i,LCD_Ddata_buffer);

			LCD_page_dis=1;
			electric_receive_sign=0;
                    }
		}
	}	
}



void LCD_page_301(void)//����״̬
{
	unsigned int i,j,xx,yy,size;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			i=0;
			i=RS232_Slcd_record_nolast(0,0,13);	
			
				xx=30;
				yy=165;//���������ʼ��λ��	
				j=0;				
				j=ADD_icon_guide(j,0xD5,xx,yy,12,P_E2PROM[110]);//ֱ����				
				i=RS232_Slcd_data_nolast(i,0x2100,8,LCD_Ddata_buffer);	//д��˸�����				
				
				LCD_Ddata_buffer[0]=P_E2PROM[110]!=0;	
				i=RS232_Slcd_data_nolast(i,0xD5,1,LCD_Ddata_buffer);	//�Ƿ���ʾ
					
				if(P_E2PROM[110]!=0)//�������
			 	{			 	
					xx=145;
					yy=164;
					size=9;
					LCD_page_dis_b=0xD0;						
					j=ADD_data_guide(0,xx,yy,0x0010,size,0,1);
			 	}
			 	else
			 	{
				 	xx=145;
					yy=164;
					size=9;
					LCD_page_dis_b=0xD0;						
					j=ADD_data_guide(0,xx,yy,0x0010,size,0,0);
				 }	
				 i=RS232_Slcd_data_nolast(i,0x2110,13,LCD_Ddata_buffer); 
		
			
			j=0;
			LCD_Ddata_buffer[j++]=equipment_skate[0][2];//״̬
			LCD_Ddata_buffer[j++]=equipment_skate[0][3]&0x01;//���Ӵ���
			LCD_Ddata_buffer[j++]=(equipment_skate[0][3]>>1)&0x01;//���Ӵ���
			if(equipment_skate[0][2]==3) LCD_Ddata_buffer[j++]=3;
			else						 LCD_Ddata_buffer[j++]=(equipment_skate[0][3]>>2)&0x01;//���	
			LCD_Ddata_buffer[j++]=P_E2PROM[4];
			LCD_Ddata_buffer[j++]=P_E2PROM[3]/60;
			LCD_Ddata_buffer[j++]=P_E2PROM[112];
			LCD_Ddata_buffer[j++]=P_E2PROM[111]/60;
			
			LCD_Ddata_buffer[j++]=P_E2PROM[116];
			LCD_Ddata_buffer[j++]=WIRE_cost[7][0];
			LCD_Ddata_buffer[j++]=WIRE_cost[7][1];
			LCD_Ddata_buffer[j++]=WIRE_cost[7][2];			

			RS232_Slcd_data(i,0xC8,j,LCD_Ddata_buffer);			
		}
	}
	else
	{	
		 if((DELAY_refresh_data_com>P_E2PROM[21]*10)&&LCD_Tbuffer_sign==1)//��ʾ��ʱ
		 {
			 DELAY_refresh_data_com=0;//��ʱ
			 j=0;		

			LCD_Ddata_buffer[j++]=equipment_skate[0][2];//״̬
			LCD_Ddata_buffer[j++]=equipment_skate[0][3]&0x01;//���Ӵ���
			LCD_Ddata_buffer[j++]=(equipment_skate[0][3]>>1)&0x01;//���Ӵ���
			if(equipment_skate[0][2]==3) LCD_Ddata_buffer[j++]=3;
			else						 LCD_Ddata_buffer[j++]=(equipment_skate[0][3]>>2)&0x01;//���	
			LCD_Ddata_buffer[j++]=P_E2PROM[4];
			LCD_Ddata_buffer[j++]=P_E2PROM[3]/60;
			LCD_Ddata_buffer[j++]=P_E2PROM[112];
			LCD_Ddata_buffer[j++]=P_E2PROM[111]/60;
			
			LCD_Ddata_buffer[j++]=P_E2PROM[116];
			LCD_Ddata_buffer[j++]=WIRE_cost[7][0];
			LCD_Ddata_buffer[j++]=WIRE_cost[7][1];
			LCD_Ddata_buffer[j++]=WIRE_cost[7][2];
	
			 RS232_Slcd_data(0,0xC8,j,LCD_Ddata_buffer);	
			 LCD_page_dis=1;	 
		 }
	}	
}

void LCD_page_401(void)//��ʷ��Ϣ
{
	unsigned int i,j;
	
	if(LCD_page_dis>=0xaaaa&&LCD_page_dis<=0xffff)//��ʷ��¼ �ϵ������
	{
		LCD_page_dis++;
		if(LCD_page_dis==0xffff) LCD_page_dis=0;
	}	
	else
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			j=RS232_Slcd_record_nolast(0,0,15);
			RS232_Slcd_record(j,13,0x2328);//�������й�������100��
			change_data=0;
		}
	}
	else
	if(LCD_page_dis<=4)//�������ϴ���
	{
		if(LCD_Tbuffer_sign==1&&change_data==0xaaaa)
		{
				RS232_Slcd_record(0,13,0x2328+LCD_page_dis*100);//�������й�������100�� ���
				change_data=0;		
		}
	}
	else
	if(LCD_page_dis==5)//���й��ϴ��룬���������ĸ�
	{	
		if(LCD_Tbuffer_sign==1&&change_data==0xaaaa)
		{
			FAULT_num(P_E2PROM[2],P_E2PROM[1],equipment_activate,error_addr);//	�������й��ϴ���error_addr  ���һ���ǹ��ϵĸ���
			
			error_dis_last=0;
			error_dis=0;			
			
			READ_four_error(0,error_dis_now,error_addr);//�����ĸ����ϴ���
			LCD_page_dis++;
		}
	}
	else
	if(LCD_page_dis<=9)//�����ĸ����ϴ���
	{
		if(LCD_Tbuffer_sign==1&&change_data==0xaaaa)
		{
			RS232_Slcd_record(0,14,error_dis_now[LCD_page_dis-6]*10+10000);//������ʱ���� 
			change_data=0;
		}	
	}
	else
	if(LCD_page_dis==10)//��ʾ�ĸ����ϴ���
	{
		if(LCD_Tbuffer_sign==1&&change_data==0xaaaa)
		{
			i=READ_fault_com(0,error_dis_last);
			RS232_Slcd_data(0,0x0f0,i,LCD_Ddata_buffer);	
		}
	}
	else
	{
		if(LCD_Tbuffer_sign==1)
		{
			if(KEY_com!=0)
			{
				if(KEY_com==1)//��ҳ
				{
					READ_four_error(0,error_dis_now,error_addr);//�����ĸ����ϴ���
					RS232_Slcd_record(0,14,0x4e20);	change_data=0;
					LCD_page_dis=6;					
				}
				else
				if(KEY_com==2)//��ҳ
				{
					READ_four_error(1,error_dis_now,error_addr);//�����ĸ����ϴ���
					RS232_Slcd_record(0,14,0x4e20);	change_data=0;
					LCD_page_dis=6;	
				}
				else
				if(KEY_com==0x0D)//ɾ������
				{		
					CLEAR_fault_all(1);
					LCD_page_dis=0;
				}
			
				KEY_com=0;
			}	
		}	
	}
}

void LCD_page_501(void)//ϵͳ����-�汾��Ϣ
{
	unsigned int i,j,lcd_version_i;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{						
				change_data=record_single[2][2];//�����豸����
				
				if(which_equ==2) j=RS232_Slcd_record_nolast(0,0,17);	
				else             j=RS232_Slcd_record_nolast(0,0,83);

				i=ADD_edit(0,soft_APF_SVG[0][0],soft_APF_SVG[0][1]);//��ʾ�汾
				i=ADD_edit(i,record_single[1][2],record_single[0][1]);//���ذ汾//&&&&
				
				if(which_equ==2) i=ADD_Pnum(i,3,APF_CHAR);
				else
				if(which_equ==3)	i=ADD_Pnum(i,3,SVG_CHAR);

				LCD_Ddata_buffer[i++]=record_single[2][2];//
				j=RS232_Slcd_data_nolast(j,0x015E,i,LCD_Ddata_buffer);
				i=0;
			    lcd_version_i=(lcd_version>>4)*10+lcd_version%16;
				LCD_Ddata_buffer[i++]=lcd_version_i;												  
			    RS232_Slcd_data(j,0x0173,i,LCD_Ddata_buffer);
		}
	}
	else
	{	
			if(change_data!=record_single[2][2]&&LCD_Tbuffer_sign==1)//�����һ��
			{
				change_data=record_single[2][2];//�����豸����
				i=ADD_edit(0,soft_APF_SVG[0][0],soft_APF_SVG[0][1]);//��ʾ�汾				
				i=ADD_edit(i,record_single[1][2],record_single[0][1]);//���ư汾//&&&&				
				LCD_Ddata_buffer[i++]=((P_E2PROM[23]/10<<8)|(P_E2PROM[23]%10))+0x3030;
				LCD_Ddata_buffer[i++]=0x4150;//A				
				LCD_Ddata_buffer[i++]=0x4600|(P_E2PROM[24]/10+0x30)  ;//F
				LCD_Ddata_buffer[i++]=((P_E2PROM[24]%10<<8)|(P_E2PROM[25]/100))+0x3030;//���������				
				LCD_Ddata_buffer[i++]=(((P_E2PROM[25]%100)/10<<8)|(P_E2PROM[25]%10))+0x3030;
				LCD_Ddata_buffer[i++]=0x2020;//
				LCD_Ddata_buffer[i++]=0x2020;//
				LCD_Ddata_buffer[i++]=0x2020;//
				LCD_Ddata_buffer[i++]=record_single[2][2];//				
				RS232_Slcd_data(0,0x015E,i,LCD_Ddata_buffer);		
			}	
	}	
}

void LCD_page_502(void)//ϵͳ����-ϵͳʱ��
{
	unsigned int j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
				j=RS232_Slcd_record_nolast(0,0,18);
				
				LCD_Ddata_buffer[0]=P_E2PROM[5];
				LCD_Ddata_buffer[1]=P_E2PROM[6];
				LCD_Ddata_buffer[2]=P_E2PROM[7];
				RS232_Slcd_data(j,0x017c,3,LCD_Ddata_buffer);//��������ʱ������ֵ		
		}
	}
	else
	{	
		
	}	
}

void LCD_page_503(unsigned int page,unsigned int order)//ϵͳ����-ͨ�Ų���
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			j=RS232_Slcd_record_nolast(0,0,19); //ҳ����ת			
			for(i=0;i<61;i++) LCD_Ddata_buffer[i]=0;
			RS232_Slcd_data(j,0x1C9,61,LCD_Ddata_buffer);

			ecan1WriteTxMsgBufId(1,0x201,0,0);
			ecan1WriteTxMsgBufData(1,8,order,equipment_ID[0][1],0,0);//ϵͳ����
			C1TR01CONbits.TXREQ1=1;
		}
	}
	else
	{	
		if(LCD_Tbuffer_sign==1)
		{
			if((parameter_receive_sign+1)%3==0)//���һ�����ݷ������
			{
				for(i=0;i<9;i++)//���յ������ݸ�������
						protect_parameter_b[i]=hide_parameter_b[i/3][(i%3)+1];

					for(i=0;i<6;i++)//���յ������ݸ�������
							LCD_Ddata_buffer[i]=protect_parameter_b[i];									
					LCD_Ddata_buffer[i++]=0;
									
					for(j=0;j<16;j++)					
						LCD_Ddata_buffer[i++]=(protect_parameter_b[6]>>j)&0x0001;					
					for(j=0;j<16;j++)					
						LCD_Ddata_buffer[i++]=(protect_parameter_b[7]>>j)&0x0001;					
					for(j=0;j<16;j++)					
						LCD_Ddata_buffer[i++]=(protect_parameter_b[8]>>j)&0x0001;//��λ����			
					LCD_Ddata_buffer[i++]=0;
					LCD_Ddata_buffer[i++]=0;
					LCD_Ddata_buffer[i++]=0;//49 50 51 ��������
					RS232_Slcd_data(0,0x1CC,58,LCD_Ddata_buffer);		

				parameter_receive_sign=0;	
			}
			else
			if(change_data!=0)//���ָ�������и���
			{
				    LCD_Ddata_buffer[0]=(protect_parameter_b[(change_data-1)/16+6]&(0x0001<<(change_data-1)%16))!=0;
				    RS232_Slcd_data(0,0x1D2+change_data,1,LCD_Ddata_buffer);//����
				    change_data=0;
			}
			else
			if(KEY_com==5)//�ٻ�
			{
				ecan1WriteTxMsgBufId(1,0x201,0,0);
				ecan1WriteTxMsgBufData(1,8,order,equipment_ID[0][1],0,0);//ϵͳ����
				C1TR01CONbits.TXREQ1=1;			
				KEY_com=0;
			}
			else
			if(KEY_com==6)//����
			{	
					ecan1WriteTxMsgBufId(1,0x202,0,0);
					ecan1WriteTxMsgBufData(1,8,hide_parameter_b[0][0],protect_parameter_b[0],protect_parameter_b[1],protect_parameter_b[2]);
					C1TR01CONbits.TXREQ1=1;
					
					ecan1WriteTxMsgBufId(2,0x203,0,0);
					ecan1WriteTxMsgBufData(2,8,hide_parameter_b[1][0],protect_parameter_b[3],protect_parameter_b[4],protect_parameter_b[5]);
					C1TR23CONbits.TXREQ2=1;
					
					ecan1WriteTxMsgBufId(3,0x204,0,0);
					ecan1WriteTxMsgBufData(3,8,hide_parameter_b[2][0],protect_parameter_b[6],protect_parameter_b[7],protect_parameter_b[8]);
					C1TR23CONbits.TXREQ3=1;					
					KEY_com=0;	
			}
			else
			if(KEY_com==7)//����
			{					
					RS232_Slcd_data(0,0x1CC,6,protect_parameter_b);		
			}	
		}	
	}	
}

void LCD_page_504(void)//ϵͳ����-��ʱ����
{
	unsigned int i,j,m,n;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
				j=RS232_Slcd_record_nolast(0,0,20);
				i=0;
				for(m=0;m<6;m++)
					for(n=0;n<7;n++)
						LCD_Ddata_buffer[i++]=P_E2PROM[n*6+m+32];
						
				RS232_Slcd_data(j,0x190,42,LCD_Ddata_buffer);
		}					
	}
	else
	{	
		
	}	
}


void LCD_page_601(unsigned int page,unsigned int order)//ϵͳ����1-ϵͳ����
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			j=RS232_Slcd_record_nolast(0,0,page); //ҳ����ת			
			for(i=0;i<61;i++) LCD_Ddata_buffer[i]=0;
			RS232_Slcd_data(j,0x1C9,61,LCD_Ddata_buffer);

			ecan1WriteTxMsgBufId(1,0x201,0,0);
			ecan1WriteTxMsgBufData(1,8,order,equipment_ID[0][1],0,0);//ϵͳ����
			C1TR01CONbits.TXREQ1=1;
		}
	}
	else
	{	
		if(LCD_Tbuffer_sign==1)
		{
			if((parameter_receive_sign+1)%3==0)//���һ�����ݷ������
			{
				for(i=0;i<9;i++)//���յ������ݸ�������
						protect_parameter_b[i]=hide_parameter_b[i/3][(i%3)+1];

					for(i=0;i<6;i++)//���յ������ݸ�������
							LCD_Ddata_buffer[i]=protect_parameter_b[i];									
					LCD_Ddata_buffer[i++]=0;
									
					for(j=0;j<16;j++)					
						LCD_Ddata_buffer[i++]=(protect_parameter_b[6]>>j)&0x0001;					
					for(j=0;j<16;j++)					
						LCD_Ddata_buffer[i++]=(protect_parameter_b[7]>>j)&0x0001;					
					for(j=0;j<16;j++)					
						LCD_Ddata_buffer[i++]=(protect_parameter_b[8]>>j)&0x0001;//��λ����			
					LCD_Ddata_buffer[i++]=0;
					LCD_Ddata_buffer[i++]=0;
					LCD_Ddata_buffer[i++]=0;//49 50 51 ��������
					RS232_Slcd_data(0,0x1CC,58,LCD_Ddata_buffer);		

				parameter_receive_sign=0;	
			}
			else
			if(change_data!=0)//���ָ�������и���
			{
				    LCD_Ddata_buffer[0]=(protect_parameter_b[(change_data-1)/16+6]&(0x0001<<(change_data-1)%16))!=0;
				    RS232_Slcd_data(0,0x1D2+change_data,1,LCD_Ddata_buffer);//����
				    change_data=0;
			}
			else
			if(KEY_com==5)//�ٻ�
			{
				ecan1WriteTxMsgBufId(1,0x201,0,0);
				ecan1WriteTxMsgBufData(1,8,order,equipment_ID[0][1],0,0);//ϵͳ����
				C1TR01CONbits.TXREQ1=1;			
				KEY_com=0;
			}
			else
			if(KEY_com==6)//����
			{	
					ecan1WriteTxMsgBufId(1,0x202,0,0);
					ecan1WriteTxMsgBufData(1,8,hide_parameter_b[0][0],protect_parameter_b[0],protect_parameter_b[1],protect_parameter_b[2]);
					C1TR01CONbits.TXREQ1=1;
					
					ecan1WriteTxMsgBufId(2,0x203,0,0);
					ecan1WriteTxMsgBufData(2,8,hide_parameter_b[1][0],protect_parameter_b[3],protect_parameter_b[4],protect_parameter_b[5]);
					C1TR23CONbits.TXREQ2=1;
					
					ecan1WriteTxMsgBufId(3,0x204,0,0);
					ecan1WriteTxMsgBufData(3,8,hide_parameter_b[2][0],protect_parameter_b[6],protect_parameter_b[7],protect_parameter_b[8]);
					C1TR23CONbits.TXREQ3=1;					
					KEY_com=0;	
			}
			else
			if(KEY_com==7)//����
			{					
					RS232_Slcd_data(0,0x1CC,6,protect_parameter_b);		
			}	
		}	
	}	
}

void LCD_page_602(unsigned int page,unsigned int order)//ϵͳ����9�����ݵĶ�������ô��
{
	unsigned int i,j;
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			j=RS232_Slcd_record_nolast(0,0,page); //ҳ����ת		
			for(i=0;i<9;i++) LCD_Ddata_buffer[i]=0;
			RS232_Slcd_data(j,0x20A,9,LCD_Ddata_buffer);
			ecan1WriteTxMsgBufId(1,0x201,0,0);
			ecan1WriteTxMsgBufData(1,8,order,equipment_ID[0][1],0,0);//�ٻ�����
			C1TR01CONbits.TXREQ1=1;
		}
	}
	else
	{	
		if(LCD_Tbuffer_sign==1)
		{
			if((parameter_receive_sign+1)%3==0)//���һ�����ݷ������
			{
				for(i=0;i<9;i++)//���յ������ݸ�������
			    protect_parameter_b[i]=hide_parameter_b[i/3][(i%3)+1];				
				RS232_Slcd_data(0,0x20A,9,protect_parameter_b);
				
				parameter_receive_sign=0;	
			}
			else if(KEY_com==5)//�ٻ�
			{
				ecan1WriteTxMsgBufId(1,0x201,0,0);
				ecan1WriteTxMsgBufData(1,8,order,equipment_ID[0][1],0,0);//�ٻ�����
				C1TR01CONbits.TXREQ1=1;			
				KEY_com=0;
			}
			else if(KEY_com==6)//����
			{	
					ecan1WriteTxMsgBufId(1,0x202,0,0);
					ecan1WriteTxMsgBufData(1,8,hide_parameter_b[0][0],protect_parameter_b[0],protect_parameter_b[1],protect_parameter_b[2]);
					C1TR01CONbits.TXREQ1=1;
					
					ecan1WriteTxMsgBufId(2,0x203,0,0);
					ecan1WriteTxMsgBufData(2,8,hide_parameter_b[1][0],protect_parameter_b[3],protect_parameter_b[4],protect_parameter_b[5]);
					C1TR23CONbits.TXREQ2=1;
					
					ecan1WriteTxMsgBufId(3,0x204,0,0);
					ecan1WriteTxMsgBufData(3,8,hide_parameter_b[2][0],protect_parameter_b[6],protect_parameter_b[7],protect_parameter_b[8]);
					C1TR23CONbits.TXREQ3=1;					
					KEY_com=0;	
			}
			else
			if(KEY_com==7)//����
			{
				RS232_Slcd_data(0,0x20A,9,protect_parameter_b);	
			}			
		}	
	}	
}


void LCD_page_606(unsigned int page,unsigned int order)//ϵͳ����-�������
{
	unsigned int i,j;
	unsigned long data_tran;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			j=RS232_Slcd_record_nolast(0,0,page); //ҳ����ת				
			for(i=0;i<3;i++) LCD_Ddata_buffer[i]=P_E2PROM[23+i];
			j=RS232_Slcd_data_nolast(j,0x23A,3,LCD_Ddata_buffer);
            data_tran=P_E2PROM[10];
            data_tran*=10000;
            data_tran+=P_E2PROM[11];
            LCD_Ddata_buffer[0]=data_tran>>16;
            LCD_Ddata_buffer[1]=data_tran&0xffff;
            LCD_Ddata_buffer[2]=P_E2PROM[12];	
            j=RS232_Slcd_data_nolast(j,0x0186,3,LCD_Ddata_buffer);//��������ʱ������ֵ		
			for(i=0;i<9;i++) LCD_Ddata_buffer[i]=0;
			RS232_Slcd_data(j,0x20A,9,LCD_Ddata_buffer);

			ecan1WriteTxMsgBufId(1,0x201,0,0);
			ecan1WriteTxMsgBufData(1,8,order,equipment_ID[0][1],0,0);//�ٻ�����
			C1TR01CONbits.TXREQ1=1;
		}
	}
	else
	{	
		if(LCD_Tbuffer_sign==1)
		{
			if((parameter_receive_sign+1)%3==0)//���һ�����ݷ������
			{
				for(i=0;i<9;i++)//���յ������ݸ�������
						protect_parameter_b[i]=hide_parameter_b[i/3][(i%3)+1];				
				RS232_Slcd_data(0,0x20A,9,protect_parameter_b);
				
				parameter_receive_sign=0;	
			}
			else if(KEY_com==5)//�ٻ�
			{
				ecan1WriteTxMsgBufId(1,0x201,0,0);
				ecan1WriteTxMsgBufData(1,8,order,equipment_ID[0][1],0,0);//�ٻ�����
				C1TR01CONbits.TXREQ1=1;			
				KEY_com=0;
			}
			else if(KEY_com==6)//����
			{	
					ecan1WriteTxMsgBufId(1,0x202,0,0);
					ecan1WriteTxMsgBufData(1,8,hide_parameter_b[0][0],protect_parameter_b[0],protect_parameter_b[1],protect_parameter_b[2]);
					C1TR01CONbits.TXREQ1=1;
					
					ecan1WriteTxMsgBufId(2,0x203,0,0);
					ecan1WriteTxMsgBufData(2,8,hide_parameter_b[1][0],protect_parameter_b[3],protect_parameter_b[4],protect_parameter_b[5]);
					C1TR23CONbits.TXREQ2=1;
					
					ecan1WriteTxMsgBufId(3,0x204,0,0);
					ecan1WriteTxMsgBufData(3,8,hide_parameter_b[2][0],protect_parameter_b[6],protect_parameter_b[7],protect_parameter_b[8]);
					C1TR23CONbits.TXREQ3=1;					
					KEY_com=0;	
			}
			else if(KEY_com==7)//����
			{
				RS232_Slcd_data(0,0x20A,9,protect_parameter_b);	
			}			
		}	
	}	
}

void LCD_page_701(void)//��ʾ����-��������1
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
		if(LCD_Tbuffer_sign==1)
		{
			i=0;
			j=RS232_Slcd_record_nolast(0,0,47);
			LCD_Ddata_buffer[i++]=P_E2PROM[14];
			LCD_Ddata_buffer[i++]=P_E2PROM[17];
			LCD_Ddata_buffer[i++]=P_E2PROM[18];
			LCD_Ddata_buffer[i++]=P_E2PROM[26];
			LCD_Ddata_buffer[i++]=P_E2PROM[15];
			LCD_Ddata_buffer[i++]=P_E2PROM[16];
			LCD_Ddata_buffer[i++]=P_E2PROM[20];
			LCD_Ddata_buffer[i++]=P_E2PROM[29];
			LCD_Ddata_buffer[i++]=P_E2PROM[27];
			LCD_Ddata_buffer[i++]=P_E2PROM[31];
			RS232_Slcd_data(j,0x24A,i,LCD_Ddata_buffer);	
		}	
	}
	else
	{	
		
	}	
}

void LCD_page_702(void)//��ʾ����-��������2
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
			if(LCD_Tbuffer_sign==1)
			{
				i=0;
				j=RS232_Slcd_record_nolast(0,0,48);				
				LCD_Ddata_buffer[i++]=P_E2PROM[21];
				LCD_Ddata_buffer[i++]=P_E2PROM[22];
				LCD_Ddata_buffer[i++]=P_E2PROM[30];
				LCD_Ddata_buffer[i++]=P_E2PROM[104];
				LCD_Ddata_buffer[i++]=P_E2PROM[105];		
				LCD_Ddata_buffer[i++]=P_E2PROM[106];
				LCD_Ddata_buffer[i++]=P_E2PROM[107];
				LCD_Ddata_buffer[i++]=P_E2PROM[109];
				LCD_Ddata_buffer[i++]=P_E2PROM[108];
				LCD_Ddata_buffer[i++]=P_E2PROM[110];
				
				RS232_Slcd_data(j,0x25A,i,LCD_Ddata_buffer);	
			}
	}
	else
	{	
		
	}	
}

void LCD_page_703(void)//��ʾ����-ģ����ʾ1
{
	unsigned int i,j;
	
	if(LCD_page_dis==0)//�����һ�ν���
	{
			if(LCD_Tbuffer_sign==1)
			{
				i=0;
				j=RS232_Slcd_record_nolast(0,0,49);
				
				LCD_Ddata_buffer[i++]=P_E2PROM[80];
				LCD_Ddata_buffer[i++]=P_E2PROM[81];
				LCD_Ddata_buffer[i++]=P_E2PROM[82];
				LCD_Ddata_buffer[i++]=P_E2PROM[83];
				LCD_Ddata_buffer[i++]=P_E2PROM[84];
				LCD_Ddata_buffer[i++]=P_E2PROM[85];
				LCD_Ddata_buffer[i++]=P_E2PROM[86];
				LCD_Ddata_buffer[i++]=P_E2PROM[87];
				LCD_Ddata_buffer[i++]=P_E2PROM[88];
				LCD_Ddata_buffer[i++]=P_E2PROM[89];
				RS232_Slcd_data(j,0x26A,i,LCD_Ddata_buffer);				
			}
	}
	else
	{	
		
	}	
}

void LCD_page_704(void)//��ʾ����-ģ����ʾ2
{
	unsigned int i,j;
	unsigned long data_tran;
	if(LCD_page_dis==0)//�����һ�ν���
	{
			if(LCD_Tbuffer_sign==1)
			{
				i=0;
				j=RS232_Slcd_record_nolast(0,0,50);	
				i=ADD_edit_inside(i,record_single[6][2],record_single[7][0]);
				j=RS232_Slcd_data_nolast(j,0x23A,i,LCD_Ddata_buffer);//���ڲ��汾����ʾ����//&&&&
				i=0;				
				LCD_Ddata_buffer[i++]=P_E2PROM[90];
				LCD_Ddata_buffer[i++]=P_E2PROM[123];
				LCD_Ddata_buffer[i++]=P_E2PROM[92];	
				LCD_Ddata_buffer[i++]=P_E2PROM[115];	
				LCD_Ddata_buffer[i++]=P_E2PROM[114];
				LCD_Ddata_buffer[i++]=P_E2PROM[116];		
				j=RS232_Slcd_data_nolast(j,0x27A,i,LCD_Ddata_buffer);
				data_tran=P_E2PROM[120];
				data_tran*=10000;
				data_tran+=P_E2PROM[121];
				LCD_Ddata_buffer[0]=data_tran>>16;
				LCD_Ddata_buffer[1]=data_tran&0xffff;
				LCD_Ddata_buffer[2]=P_E2PROM[122];
                                RS232_Slcd_data(j,0x281,3,LCD_Ddata_buffer);
			}
	}
	else
	{	
		if(DELAY_refresh_data_com>P_E2PROM[21]*10)
		{
			DELAY_refresh_data_com=0;
			i=0;
			LCD_Ddata_buffer[i++]=ask_time>>16;
			LCD_Ddata_buffer[i++]=ask_time&0xffff;
			
			LCD_Ddata_buffer[i++]=C1RERRCNT;
			LCD_Ddata_buffer[i++]=C1TERRCNT;
			RS232_Slcd_data(0,0x285,i,LCD_Ddata_buffer);
		}
	}	
}

void LCD_page_705()//������Ϣ��������
{
    unsigned int i,j,u;
    if(LCD_page_dis==0)//�����һ�ν���
    {
        if(LCD_Tbuffer_sign==1)
        {
            j=RS232_Slcd_record_nolast(0,0,94);
            i=0;
            LCD_Ddata_buffer[i++]=P_E2PROM[124];
            j=RS232_Slcd_data_nolast(j,0x28a,1,LCD_Ddata_buffer);
            i=0;
            for(u=0;u<16;u++)
                LCD_Ddata_buffer[i++]=(P_E2PROM[125]>>u)&0x0001;
	    for(u=0;u<16;u++)
		LCD_Ddata_buffer[i++]=(P_E2PROM[126]>>u)&0x0001;
	    for(u=0;u<16;u++)
	        LCD_Ddata_buffer[i++]=(P_E2PROM[127]>>u)&0x0001;
             for(u=0;u<16;u++)
                LCD_Ddata_buffer[i++]=(P_E2PROM[128]>>u)&0x0001;
	    for(u=0;u<16;u++)
		LCD_Ddata_buffer[i++]=(P_E2PROM[129]>>u)&0x0001;
	    for(u=0;u<16;u++)
	        LCD_Ddata_buffer[i++]=(P_E2PROM[130]>>u)&0x0001;
            RS232_Slcd_data(j,0x291,i,LCD_Ddata_buffer);
        }
    }
    else
    {
        if(LCD_Tbuffer_sign==1&&change_data_h!=0)
        {
            j=i=0;
            if(change_data_h==1)
            {
                LCD_Ddata_buffer[i++]=P_E2PROM[124];
                j=RS232_Slcd_data(j,0x28a,1,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==2)
            {
               for(u=0;u<16;u++)
                LCD_Ddata_buffer[i++]=(P_E2PROM[125]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x291,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==3)
            {
	       for(u=0;u<16;u++)
		LCD_Ddata_buffer[i++]=(P_E2PROM[126]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2a1,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==4)
            {
	       for(u=0;u<16;u++)
	        LCD_Ddata_buffer[i++]=(P_E2PROM[127]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2b1,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==5)
            {
              for(u=0;u<16;u++)
                LCD_Ddata_buffer[i++]=(P_E2PROM[128]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2c1,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==6)
            {
	        for(u=0;u<16;u++)
		LCD_Ddata_buffer[i++]=(P_E2PROM[129]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2d1,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==7)
            {
	        for(u=0;u<16;u++)//����
	        LCD_Ddata_buffer[i++]=(P_E2PROM[130]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2e1,i,LCD_Ddata_buffer);
            }
            change_data_h=0;
        }
    }
}
void LCD_page_706(void)//GSM��������
{
     unsigned int i,j,u;
    if(LCD_page_dis==0)//�����һ�ν���
    {
        if(LCD_Tbuffer_sign==1)
        {
            j=RS232_Slcd_record_nolast(0,0,96);
            i=0;
            LCD_Ddata_buffer[i++]=P_E2PROM[131];
            j=RS232_Slcd_data_nolast(j,0x28a,1,LCD_Ddata_buffer);
            i=0;
            for(u=0;u<16;u++)
                LCD_Ddata_buffer[i++]=(P_E2PROM[132]>>u)&0x0001;
	    for(u=0;u<16;u++)
		LCD_Ddata_buffer[i++]=(P_E2PROM[133]>>u)&0x0001;
	    for(u=0;u<16;u++)
	        LCD_Ddata_buffer[i++]=(P_E2PROM[134]>>u)&0x0001;
             for(u=0;u<16;u++)
                LCD_Ddata_buffer[i++]=(P_E2PROM[135]>>u)&0x0001;
	    for(u=0;u<16;u++)
		LCD_Ddata_buffer[i++]=(P_E2PROM[136]>>u)&0x0001;
	    for(u=0;u<16;u++)
	        LCD_Ddata_buffer[i++]=(P_E2PROM[137]>>u)&0x0001;
            RS232_Slcd_data(j,0x291,i,LCD_Ddata_buffer);
        }
    }
    else
    {
        if(LCD_Tbuffer_sign==1&&change_data_h!=0)
        {
            j=i=0;
            if(change_data_h==1)
            {
                LCD_Ddata_buffer[i++]=P_E2PROM[131];
                j=RS232_Slcd_data(j,0x28a,1,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==2)
            {
               for(u=0;u<16;u++)
                LCD_Ddata_buffer[i++]=(P_E2PROM[132]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x291,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==3)
            {
	       for(u=0;u<16;u++)
		LCD_Ddata_buffer[i++]=(P_E2PROM[133]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2a1,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==4)
            {
	       for(u=0;u<16;u++)
	        LCD_Ddata_buffer[i++]=(P_E2PROM[134]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2b1,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==5)
            {
              for(u=0;u<16;u++)
                LCD_Ddata_buffer[i++]=(P_E2PROM[135]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2c1,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==6)
            {
	        for(u=0;u<16;u++)
		LCD_Ddata_buffer[i++]=(P_E2PROM[136]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2d1,i,LCD_Ddata_buffer);
            }
            else
            if(change_data_h==7)
            {
	        for(u=0;u<16;u++)//����
	        LCD_Ddata_buffer[i++]=(P_E2PROM[137]>>u)&0x0001;
               j=RS232_Slcd_data(j,0x2e1,i,LCD_Ddata_buffer);
            }
            change_data_h=0;
        }
    }
}

void LCD_page_run(void)
{
     unsigned int i,j;
	 RS232_Rlcd_button();//������յ��İ�ť
	 pro_message();//��ʾ��Ϣ
	 LCD_habit();//��״̬����ѯ��
		 if(RUN_stop!=0)  RUN_stop_1();	
	else if(LCD_page_sign==0x101) LCD_page_101();//�������µİ�ť������ת
	else if(LCD_page_sign==0x102) LCD_page_102();
	else if(LCD_page_sign==0x103) LCD_page_103();
	else if(LCD_page_sign==0x201) LCD_page_201();
	else if(LCD_page_sign==0x202) LCD_page_202();
	else if(LCD_page_sign==0x203) LCD_page_203();
	else if(LCD_page_sign==0x204) LCD_page_204();
	else if(LCD_page_sign==0x205) LCD_page_205();
	else if(LCD_page_sign==0x206) LCD_page_206();
	else if(LCD_page_sign==0x207) LCD_page_207();
	else if(LCD_page_sign==0x301) LCD_page_301();
	else if(LCD_page_sign==0x401) LCD_page_401();
	else if(LCD_page_sign==0x501) LCD_page_501();
	else if(LCD_page_sign==0x502) LCD_page_502();
	else if(LCD_page_sign==0x503) LCD_page_503(40,0x101);
	else if(LCD_page_sign==0x504) LCD_page_504();
	else if(LCD_page_sign>=0x601&&LCD_page_sign<=0x608)
	{
		if(which_equ==2)	 //3.5 80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************80APF**************
		{		
				if(LCD_page_sign==0x601)  LCD_page_601(40,0x101);
			else if(LCD_page_sign==0x602) LCD_page_602(59,0x105);//
			else if(LCD_page_sign==0x603) LCD_page_602(60,0x106);//
			else if(LCD_page_sign==0x604) LCD_page_602(61,0x107);//
			else if(LCD_page_sign==0x605) LCD_page_602(43,0x104);//
			else if(LCD_page_sign==0x606) LCD_page_606(44,0x108);
			else if(LCD_page_sign==0x607) LCD_page_602(41,0x102);//
			else if(LCD_page_sign==0x608) LCD_page_602(42,0x103);//
		}
        else if(which_equ==3)//3.5 SVG
		{
				if(LCD_page_sign==0x601) LCD_page_602(84,0x101);
			else if(LCD_page_sign==0x602) LCD_page_602(85,0x102);//
			else if(LCD_page_sign==0x603) LCD_page_602(86,0x103);//
			else if(LCD_page_sign==0x604) LCD_page_602(87,0x104);//
			else if(LCD_page_sign==0x605) LCD_page_601(90,0x105);//
			else if(LCD_page_sign==0x606) LCD_page_602(91,0x106);//
			else if(LCD_page_sign==0x607) LCD_page_606(88,0x107);
//			else if(LCD_page_sign==0x608) LCD_page_606(44);	
		}	
	}
	else if(LCD_page_sign==0x701) LCD_page_701();
 	else if(LCD_page_sign==0x702) LCD_page_702();
	else if(LCD_page_sign==0x703) LCD_page_703();
	else if(LCD_page_sign==0x704) LCD_page_704();
	else if(LCD_page_sign==0x705) LCD_page_705();
    else if(LCD_page_sign==0x706) LCD_page_706();
    else if(LCD_page_sign>=0x801&&LCD_page_sign<=0x808)
	{
			     if(LCD_page_sign==0x801) LCD_page_602(62,0x105);//
			else if(LCD_page_sign==0x802) LCD_page_602(63,0x106);//
			else if(LCD_page_sign==0x803) LCD_page_602(64,0x107);//
    }        
	else if(LCD_page_sign==0)
	{	
			if(LCD_page_dis_b==0)//�����һ�ν���
			{
				if(LCD_Tbuffer_sign==1)
				{					
					change_data=0x5a;
					j=RS232_Slcd_record_nolast(0,0,0);					
					RS232_Slcd_record(j,11,0);//��FLASH
					LCD_page_dis_b++;	
				}
			}
			else if(LCD_page_dis_b==1)//
			{
				if(LCD_Tbuffer_sign==1)
				{
					if(P_E2PROM[103]==0)//���û������ֱ����ת
					{
						RS232_Slcd_record(0,15,0);//&&&&���������ں˰汾��
						if(DELAY_fist_page>=1)//����ʱ��ʱ����
						{
								LCD_page_sign=0x101;	//�ϵ�һ��ʱ�ָ�����
								LCD_page_dis=0;
						}
					}
					else    //���������
					{
						if(which_equ==2) i=ADD_Pnum(0,3,APF_CHAR);
						else			 i=ADD_Pnum(0,3,SVG_CHAR);
						RS232_Slcd_data(0,0x016A,i,LCD_Ddata_buffer);
						message_num=10;	
					}
				}
				
			}	
	}			
}
