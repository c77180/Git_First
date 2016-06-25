#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"
#if compileGSM
//*********���ϴ���*********************************************
void READ_fault_one_else(unsigned int num,unsigned int add)//��һ�����ϴ���
{
	unsigned int i,k;
	
//	 R232_send_data[R232_send_data_num++]=0x0a;	
//	 R232_send_data[R232_send_data_num++]=0x0d;
	 

	  R232_send_data[R232_send_data_num++]=0x32;
	  R232_send_data[R232_send_data_num++]=0x30;
	  i=E2ROM_93LC86C_readdata(add+1);
	  R232_send_data[R232_send_data_num++]=i/10+0x30;
	  R232_send_data[R232_send_data_num++]=i%10+0x30;//��
	  R232_send_data[R232_send_data_num++]=0x5f;
	  
	  i=E2ROM_93LC86C_readdata(add+2);
	  R232_send_data[R232_send_data_num++]=i/10+0x30;
	  R232_send_data[R232_send_data_num++]=i%10+0x30;//��
	  R232_send_data[R232_send_data_num++]=0x5f;	

	  i=E2ROM_93LC86C_readdata(add+3);
	  R232_send_data[R232_send_data_num++]=i/10+0x30;
	  R232_send_data[R232_send_data_num++]=i%10+0x30;//��
	  R232_send_data[R232_send_data_num++]=0x20;
	  
	  i=E2ROM_93LC86C_readdata(add+4);
	  R232_send_data[R232_send_data_num++]=i/10+0x30;
	  R232_send_data[R232_send_data_num++]=i%10+0x30;//ʱ
	  R232_send_data[R232_send_data_num++]=0x3a;
	  
	  i=E2ROM_93LC86C_readdata(add+5);
	  R232_send_data[R232_send_data_num++]=i/10+0x30;
	  R232_send_data[R232_send_data_num++]=i%10+0x30;//��
	  
	  R232_send_data[R232_send_data_num++]=0x20;
	  R232_send_data[R232_send_data_num++]=0x20;
	  
	  	i=E2ROM_93LC86C_readdata(add+7);//ģ���
		for(k=0;k<4;k++)
		{
			R232_send_data[R232_send_data_num++]=MODE_num_com[i][k]>>8;
			R232_send_data[R232_send_data_num++]=MODE_num_com[i][k]&0xff;
		}
		
		i=E2ROM_93LC86C_readdata(add+6);//���ϴ���	
		for(k=0;k<10;k++)
		{
			R232_send_data[R232_send_data_num++]=ERR_code_com[i][k]>>8;
	  		R232_send_data[R232_send_data_num++]=ERR_code_com[i][k]&0xff;
	    } 
}
#endif
void RS232_Config(void)//rs232���� 9600
{
	_U1EIE=0;	//U1,�����ж���������������жϸ�λ
	_U1TXIE=0;	//U1�������ж�����
	_U1RXIE=0;	//U1�������ж�����
	
	U1MODE=0x8000;//���������������ţ�8���ݣ�1ֹͣ����У��
//	U1MODE=0x8002;//���������������ţ�8���ݣ�1ֹͣ��żУ��
	U1BRG=0x103;   //9600    40M/16/9600-1
	
//	U1BRG=0x81;   //19200    40M/16/19200-1
	
//	U1STA=0x0400; //�����жϱ�־������ʹ��
	U1STA=0x0400; //�����жϱ�־������ʹ��
}
#if compileGSM
void RS232_Sdata_debug(void)//���� һ���ַ���  ��ʱ����*************
{	
	while(RS232_Sfifo==0&&R232_send_data_now<R232_send_data_num) //��������������������
		U1TXREG=R232_send_data[R232_send_data_now++];
}	

void RS232_Rdata_debug(void)//��������ݾͶ����� ��ʱ����*************
{
	unsigned int i=0,j=0,data[10];
	
	while(1)
	{
			if(RS232_Rfifo==1) 
			{
				data[j++]=U1RXREG;
				if(j==10) 
				i++;//��ϵ㣬���յ��������Ƿ���ȷ
			}	
	}
}	

void RS232_Sdata(void)//���� һ���ַ���
{	
	if(R232_Send_sign==0)
	{
		while(RS232_Sfifo==0&&R232_send_data_now<R232_send_data_num) //��������������������
			U1TXREG=R232_send_data[R232_send_data_now++];
			
		if(R232_send_data_now>=R232_send_data_num)//����������
		{	
			R232_Send_sign=1;
			DELAY_GSM=0;
		}
	}
}	

void RS232_Rdata(void)//��������ݾͶ�����
{
	unsigned char i;
	unsigned int j,k,l,m;
	
	if(R232_Rbuffer_sign==1)
	{
			while(RS232_Rfifo==1) 
			{
				SOURCE_R[R232_receive_data_now++]=U1RXREG;				
				SOURCE_R_num[R232_receive_Ndata_now++]=SOURCE_R[R232_receive_data_now-1];
				
		//*********�������ĺ������**************
				if(R232_receive_Ndata_now==1){if(SOURCE_R_num[0]!='"') R232_receive_Ndata_now=0;}
			else if(R232_receive_Ndata_now==2){if(SOURCE_R_num[1]!='+')R232_receive_Ndata_now=0;}
			else if(R232_receive_Ndata_now==3){if(SOURCE_R_num[2]!='8')R232_receive_Ndata_now=0;}
			else if(R232_receive_Ndata_now==4){if(SOURCE_R_num[3]!='6')R232_receive_Ndata_now=0;}
			else if(R232_receive_Ndata_now==16&&SOURCE_R_num[15]=='"')
			{
				j=SOURCE_R_num[4]-'0';k=j*1000;		
				j=SOURCE_R_num[5]-'0';k+=j*100;	
				j=SOURCE_R_num[6]-'0';k+=j*10;	
				j=SOURCE_R_num[7]-'0';k+=j;				
				
				j=SOURCE_R_num[8]-'0';l=j*1000;		
				j=SOURCE_R_num[9]-'0';l+=j*100;	
				j=SOURCE_R_num[10]-'0';l+=j*10;	
				j=SOURCE_R_num[11]-'0';l+=j;					
				
				j=SOURCE_R_num[12]-'0';m=j*100;
				j=SOURCE_R_num[13]-'0';m+=j*10;
				j=SOURCE_R_num[14]-'0';m+=j;
				
				if(k!=P_E2PROM[120]||l!=P_E2PROM[121]||m!=P_E2PROM[122])
				{
					P_E2PROM[120]=k;	
					P_E2PROM[121]=l;	
					P_E2PROM[122]=m;	
					GSM_NUM_fist=2;//�޸ı�ʶ
				}
				R232_receive_Ndata_now=0;	
			}
				
	//**************���Ž���********************
				 if(R232_receive_data_now==1){if(SOURCE_R[0]!='0'&&SOURCE_R[0]!=190)R232_receive_data_now=0;}
			else if(R232_receive_data_now==2){if(SOURCE_R[1]!='8'&&SOURCE_R[1]!=191)R232_receive_data_now=0;}
                       // else if(R232_receive_data_now==2){if((SOURCE_R[1]!='7'&&SOURCE_R[1]!='8') &&SOURCE_R[1]!=191)R232_receive_data_now=0;} ////����Ԫ-����***********************************
			else if(R232_receive_data_now==3){if(SOURCE_R[2]!='9'&&SOURCE_R[2]!=192)R232_receive_data_now=0;}
			else if(R232_receive_data_now==4)
			{
				if(SOURCE_R[3]!='1'&&SOURCE_R[3]!=193)R232_receive_data_now=0;
				else
				if(SOURCE_R[0]==190&&SOURCE_R[1]==191&&SOURCE_R[2]==192&&SOURCE_R[3]==193)//�������ع��ϴ���
				{
					R232_Rbuffer_sign=0;
					R232_receive_data_now=0;
										
					j=1;
					for(i=0;i<20;i++)
					{
						if(E2ROM_93LC86C_readdata(740+(i<<3))==0xaaaa)
						{
							READ_fault_one_else(j++,740+(i<<3));
						}
					}
					
					R232_send_data_now=0;
					R232_Send_sign=0;
				}	
			}	
			else if(R232_receive_data_now==27){if(SOURCE_R[26]!=P_E2PROM[10]/100%10+'0'){R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==28){if(SOURCE_R[27]!=P_E2PROM[10]/1000+'0')  {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==29){if(SOURCE_R[28]!=P_E2PROM[10]%10+'0')    {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==30){if(SOURCE_R[29]!=P_E2PROM[10]/10%10+'0') {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==31){if(SOURCE_R[30]!=P_E2PROM[11]/100%10+'0'){R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==32){if(SOURCE_R[31]!=P_E2PROM[11]/1000+'0')  {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==33){if(SOURCE_R[32]!=P_E2PROM[11]%10+'0')    {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==34){if(SOURCE_R[33]!=P_E2PROM[11]/10%10+'0') {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==35){if(SOURCE_R[34]!=P_E2PROM[12]/10%10+'0') {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==36){if(SOURCE_R[35]!=P_E2PROM[12]/100+'0')   {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==38){if(SOURCE_R[37]!=P_E2PROM[12]%10+'0')    {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;}}
			else if(R232_receive_data_now==58){R232_receive_data_num=char_hex(SOURCE_R[56],SOURCE_R[57]);R232_receive_data_num*=2;}
			else if(R232_receive_data_now>=256) {R232_Rbuffer_sign=0;R232_receive_data_now=0;GSM_record_sign=0x0300;R232_receive_Ndata_now=0;}
			else
			{
				if(R232_receive_data_now!=0&&SOURCE_R[R232_receive_data_now-1]==0x0d)
				{
					R232_Rbuffer_sign=0;//������յ���һ֡����,ȥ��������Ϣɾ��
					GSM_record_sign=0x0200;//
				}
			}
		   }//while
	}
	else
	{
		i=U1RXREG;//��һЩ�޹����� ����Ҫ���꣬�����㹻��	
	}		
}
#endif
















