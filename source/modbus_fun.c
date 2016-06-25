#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"
#if compilemodbus
 volatile unsigned int MODBUS_SIGN=4; 	//���ͱ�ʶ
								//0����δ�յ�����
                    			//1:�յ�����
	                   			//2��һ��֡�����������
 volatile unsigned int Modbus_mortize=0;     //���ڷ��ͻ���

 volatile unsigned char __attribute__((__far__))   MODBUS_RECEIVE[64];//�������ݻ�����
 volatile unsigned int MODBUS_RECEIVE_LENGTH=0;     //�������ݻ������е����ݵĸ���

 volatile unsigned char __attribute__((__far__))   MODBUS_SEND[256];//�������ݻ�����
 volatile unsigned int MODBUS_SEND_LENGTH=0;     //�������ݻ������е����ݵĸ���


volatile int  __attribute__((__far__))  Modbus_electric[ELECTRIC_N];	   //  ������  30001~~39999
 volatile int  __attribute__((__far__))  Modbus_parameter[PARAMETER_N][2];  // ����    40001~~49999
 		  
 volatile unsigned int Modbus_write_sign=0;//�Ƿ��п��������߲�����д��


void modbus_init()//��ʼ�������ݱ�������******
{
   int i,j;
   for(i=0;i<2;i++)
   {		
		for(j=0;j<PARAMETER_N;j++)
			Modbus_parameter[j][i]=0;	
   }
	for(i=0;i<ELECTRIC_N;i++)	
   			Modbus_electric[i]=0; 
}

void modbus_change()//���������������ı仯������ʵ�ʵ������޸�
{
	Modbus_electric[2] =WIRE_cost[0][0];
	Modbus_electric[3] =WIRE_cost[0][1];
	Modbus_electric[4] =WIRE_cost[0][2];//
	Modbus_electric[5] =WIRE_cost[1][0];
	Modbus_electric[6] =WIRE_cost[1][1];
	Modbus_electric[7] =WIRE_cost[1][2];//
	Modbus_electric[8] =WIRE_cost[2][0];
	Modbus_electric[9] =WIRE_cost[2][1];
	Modbus_electric[10]=WIRE_cost[2][2];//
	Modbus_electric[11]=WIRE_cost[3][0];
	Modbus_electric[12]=WIRE_cost[3][1];
	Modbus_electric[13]=WIRE_cost[3][2];
	Modbus_electric[14]=WIRE_cost[4][0];
	Modbus_electric[15]=WIRE_cost[4][1];
	Modbus_electric[16]=WIRE_cost[4][2];
	Modbus_electric[17]=WIRE_cost[5][0];
	Modbus_electric[18]=WIRE_cost[5][1];	
	Modbus_electric[19]=WIRE_cost[5][2];//
	
	Modbus_electric[20]=LOAD_cost[0][0];
	Modbus_electric[21]=LOAD_cost[0][0];
	Modbus_electric[22]=LOAD_cost[0][0];
	
	Modbus_electric[23]=LOAD_cost[1][0];
	Modbus_electric[24]=LOAD_cost[1][1];
	Modbus_electric[25]=LOAD_cost[1][2];
	Modbus_electric[26]=LOAD_cost[2][0];
	Modbus_electric[27]=LOAD_cost[2][1];
	Modbus_electric[28]=LOAD_cost[2][2];
	Modbus_electric[29]=LOAD_cost[3][0];
	Modbus_electric[30]=LOAD_cost[3][1];
	Modbus_electric[31]=LOAD_cost[3][2];
	Modbus_electric[32]=LOAD_cost[4][0];
	Modbus_electric[33]=LOAD_cost[4][1];
	Modbus_electric[34]=LOAD_cost[4][2];
	Modbus_electric[35]=LOAD_cost[5][0];
	Modbus_electric[36]=LOAD_cost[5][1];
	Modbus_electric[37]=LOAD_cost[5][2];
	Modbus_electric[38]=WIRE_cost[6][0];
	Modbus_electric[39]=WIRE_cost[6][1];
	Modbus_electric[40]=WIRE_cost[6][2];
	
	Modbus_electric[41]=equipment_skate[0][2];
	Modbus_electric[42]=0;
	Modbus_electric[43]=0;
	Modbus_electric[44]=0;
	Modbus_electric[45]=0;
	Modbus_electric[46]=0;
	Modbus_electric[47]=0;
	Modbus_electric[48]=0;
	
	Modbus_electric[49]=(equipment_skate[0][3]>>2)&0x01;
	Modbus_electric[50]=0;
	Modbus_electric[51]=0;
	Modbus_electric[52]=0;
	Modbus_electric[53]=0;
	Modbus_electric[54]=0;
	Modbus_electric[55]=0;
	Modbus_electric[56]=0;
	
	Modbus_electric[57]=record_single[1][0];
	Modbus_electric[58]=0;
	Modbus_electric[59]=0;
	Modbus_electric[60]=0;
	Modbus_electric[61]=0;
	Modbus_electric[62]=0;
	Modbus_electric[63]=0;
	Modbus_electric[64]=0;

	Modbus_electric[65]=record_single[1][1];
	Modbus_electric[66]=0;
	Modbus_electric[67]=0;
	Modbus_electric[68]=0;
	Modbus_electric[69]=0;
	Modbus_electric[70]=0;
	Modbus_electric[71]=0;
	Modbus_electric[72]=0;
	Modbus_electric[73]=WIRE_cost[7][0];
	Modbus_electric[74]=WIRE_cost[7][1];
	Modbus_electric[75]=WIRE_cost[7][2];

        Modbus_electric[76]=1<<equipment_skate[0][2];
	Modbus_electric[77]=1<<equipment_skate[1][2];
	Modbus_electric[78]=1<<equipment_skate[2][2];
	Modbus_electric[79]=1<<equipment_skate[3][2];
	Modbus_electric[80]=1<<equipment_skate[4][2];
	Modbus_electric[81]=1<<equipment_skate[5][2];
	Modbus_electric[82]=1<<equipment_skate[6][2];
	Modbus_electric[83]=1<<equipment_skate[7][2];

	if(Modbus_electric[0] == 0)
	{
		Modbus_electric[84] = 0;
		Modbus_electric[85] = 0;
		Modbus_electric[86] = 0;
		Modbus_electric[87] = 0;
                Modbus_electric[88] = 0;
	}
	else
	if(Modbus_electric[0]<=16)
	{
		Modbus_electric[84] = 1<<(Modbus_electric[0]-1);
		Modbus_electric[85] = 0;
		Modbus_electric[86] = 0;
		Modbus_electric[87] = 0;
                Modbus_electric[88] = 0;
	}
	else
	if(Modbus_electric[0]<=32)
	{
		Modbus_electric[84] = 0;
		Modbus_electric[85] = 1<<(Modbus_electric[0]-17);
		Modbus_electric[86] = 0;
		Modbus_electric[87] = 0;
                Modbus_electric[88] = 0;

	}
	else
	if(Modbus_electric[0]<=48)
	{
		Modbus_electric[84] = 0;
		Modbus_electric[85] = 0;
		Modbus_electric[86] = 1<<(Modbus_electric[0]-33);
		Modbus_electric[87] = 0;
                Modbus_electric[88] = 0;

	}
	else
	if(Modbus_electric[0]<=64)
	{
		Modbus_electric[84] = 0;
		Modbus_electric[85] = 0;
		Modbus_electric[86] = 0;
		Modbus_electric[87] = 1<<(Modbus_electric[0]-49);
                Modbus_electric[88] = 0;
	}
        else
	if(Modbus_electric[0]<=80)
	{
		Modbus_electric[84] = 0;
		Modbus_electric[85] = 0;
		Modbus_electric[86] = 0;
		Modbus_electric[87] = 0;
                Modbus_electric[88] = 1<<(Modbus_electric[0]-65);
	}
	else
	{
		Modbus_electric[84] = 0;
		Modbus_electric[85] = 0;
		Modbus_electric[86] = 0;
		Modbus_electric[87] = 0;
                Modbus_electric[88] = 0;
	}
	
	Modbus_parameter[0][0]=0xffff;
	Modbus_parameter[1][0]=equipment_skate[0][2];
	Modbus_parameter[2][0]=0;
	Modbus_parameter[3][0]=0;
	Modbus_parameter[4][0]=0;
	Modbus_parameter[5][0]=0;
	Modbus_parameter[6][0]=0;
	Modbus_parameter[7][0]=0;
	Modbus_parameter[8][0]=0;
}

void modbus_Shandle(void)//
{
	unsigned int i,j;
	
	if(Modbus_write_sign!=0)
	{
		for(i=0;i<2;i++)
		{
			if(Modbus_parameter[i][1]==1)
			{
				if(Modbus_parameter[i][0]==0) {j=1;WRITE_fault_one(43,0);Modbus_electric[0]=43;Modbus_electric[1]=1;}
				else
				if(Modbus_parameter[i][0]==1) {j=2;WRITE_fault_one(42,0);Modbus_electric[0]=42;Modbus_electric[1]=1;}
				else
				if(Modbus_parameter[i][0]==2) {j=5;WRITE_fault_one(54,0);Modbus_electric[0]=54;Modbus_electric[1]=1;}
				else
				if(Modbus_parameter[i][0]==5) {j=6;WRITE_fault_one(53,0);Modbus_electric[0]=53;Modbus_electric[1]=1;}//����
				else                           j=9;
				
				ecan1WriteTxMsgBufId(1,0x201,0,0);
				ecan1WriteTxMsgBufData(1,8,j,0,0,0);//��������
				C1TR01CONbits.TXREQ1=1;
				
				Modbus_parameter[i][1]=0;
			}	
		}
		
		Modbus_write_sign=0;
	}
}

/*********************************************************************/
//������: modbus_get_crc16()
//����:  ����modbus��CRC�ļ���(�ǲ��)
//����:2010.12.10
//���ܣ���������ͱ��ĵ�CRCУ�飬�����ڱ��ĵĺ��� ���ֽ���ǰ�����ֽ��ں�
//����:ȫ�ֱ�����MODBUS_SEND:���ĵ��׵�ַ
//				MODBUS_SEND_LENGTH:���ĵĳ���
//���:         ��������ĵ�CRCУ�飬�����ڱ��ĵĺ��� ���ֽ���ǰ�����ֽ��ں�
/*********************************************************************/
 void modbus_get_crc16() 
{
    unsigned int i,j;
    unsigned int c, crc = 0xFFFF;
    for (i = 0; i < MODBUS_SEND_LENGTH; i++)
    {
		c=MODBUS_SEND[i] & 0x00ff;

   		crc^=c;
        	for(j=0;j<8;j++){
             	if (crc & 0x0001)
                 	{crc>>=1;crc^=0xA001;}
             	else crc>>=1;
        	}
    }
	MODBUS_SEND[MODBUS_SEND_LENGTH++]=crc & 0xff;
	MODBUS_SEND[MODBUS_SEND_LENGTH++]=(crc >> 8) & 0xff;
} 

/*********************************************************************/
//������: modbus_if_crc16() 
//����:  ����modbus��CRC�ļ���(�ǲ��)
//����:2010.12.10
//���ܣ��ж��յ����ݵı����Ƿ���ȷ
//����:ȫ�ֱ�����MODBUS_RECEIVE:���ĵ��׵�ַ
//				MODBUS_RECEIVE_LENGTH:���ĵĳ���
//���:         �ж��յ�����CRCУ���Ƿ���ȷ�������ȷ����1��������󷵻�0
/*********************************************************************/
int modbus_if_crc16()
{
    unsigned int i,j;
    unsigned int c, crc = 0xFFFF;
    for (i = 0; i < MODBUS_RECEIVE_LENGTH-2; i++)
    {
		c=MODBUS_RECEIVE[i] & 0x00ff;

   		crc^=c;
        	for(j=0;j<8;j++){
             	if (crc & 0x0001)
                 	{crc>>=1;crc^=0xA001;}
             	else crc>>=1;
        	}
    }

	if((MODBUS_RECEIVE[MODBUS_RECEIVE_LENGTH-2]==(crc & 0xff))&&(MODBUS_RECEIVE[MODBUS_RECEIVE_LENGTH-1]==((crc >> 8) & 0xff)))
	     return 1;
	else return 0;
}  


/****************************************/
//������: unsigned int modbus_read_data()
//����:  ����DSP2812 SCIB �� modbus ���� ����
//����:2010.12.10
//�������MODBUS_T1_5����Ҫ����ʱ���жϵĴ���		
//ȫ�ֱ�����SCIB_NUM:ʵ�ʽ���ʱ���жϵĴ���
//			MODBUS_RECEIVE:���ĵ��׵�ַ
//			MODBUS_RECEIVE_LENGTH:���ĵĳ���
//����:RECEIVE_SIGN  0����δ�յ�����
//                    1:�յ�����
//                    2��һ��֡�����������
//���:����RECEIVE_SIGN �����ж�һ��֡�Ƿ�������
/****************************************/
void modbus_read_data()
{   
	unsigned int k;
	
//*****����һ������****************************		
			while(SPI1STATbits.SPITBF==1);
			SS1=0;
			SPI1BUF=CN_LSR;
			while(SPI1STATbits.SPIRBF==0);
			k=SPI1BUF;
			SS1=1;
			
			CH432T_reset=(k>>7)&0x01;
			RS485_receive=k&0x01;// DATARDY Ϊ1 ��˵�������ݿ��Զ���  0���յ�����
			RS485_send_fifo=(k>>5)&0x01;// THRE 1���Լ�����fifo�������ݣ�0,fifo���� 16��
			RS485_send_over=(k>>6)&0x01;// TEMT 1����ȫ������
			
		if(RS485_receive==0)
		{
			 if(MODBUS_SIGN==1)  //�Ѿ��յ������ݣ�
			 {     
					if(MODBUS_TIME>=MODBUS_T1_5)
					{
							MODBUS_SIGN=2;  //��ʾ���Դ���������
							MODBUS_TIME=0;     //��λʱ�䣬Ҫ��T3.5 �󷢳�		
					}
			 }
		}
		else //����յ������ݣ�	
	   {				
			MODBUS_SIGN=1;     //�Ѿ��յ��˵�һ������
			MODBUS_TIME=0;			//��λʱ��

				while(SPI1STATbits.SPITBF==1);
				SS1=0;
				SPI1BUF=0;
				while(SPI1STATbits.SPIRBF==0);
				MODBUS_RECEIVE[MODBUS_RECEIVE_LENGTH++]=SPI1BUF;
				SS1=1;
	   }
}


/****************************************/
//������:unsigned int modbus_analysis()
//����: �����յ�����ʲô������,�����ûط�����
//����:2010.12.10

/****************************************/
void modbus_analysis()
{   
//	 char MODBUS_RECEIVE[128]={0};//�������ݻ�����
// 	unsigned int MODBUS_RECEIVE_LENGTH=0;     //�������ݻ������е����ݵĸ���
  unsigned int addr_begin,data_num,data_value;
  unsigned int i,j;//,k,l; 

		if(MODBUS_RECEIVE[1]==4) //��ȡ������@@***********************************************************
	    {
			addr_begin=MODBUS_RECEIVE[2];
			addr_begin=(addr_begin<<8) | MODBUS_RECEIVE[3];//��ʼ��ַ

			data_num=MODBUS_RECEIVE[4];
			data_num=(data_num<<8) | MODBUS_RECEIVE[5];//����

			MODBUS_SEND[MODBUS_SEND_LENGTH++]=data_num<<1;//�ֽ���=�Ĵ�����������2

			j=addr_begin-30001;//��������ȥ30000  ��ֹ���������

			if((data_num<1)||(data_num>0x007d))     //�쳣��3
			{
				MODBUS_SEND[MODBUS_SEND_LENGTH-2]=MODBUS_RECEIVE[1]|0x80;//������ĳɲ����
				MODBUS_SEND[MODBUS_SEND_LENGTH-1]=03;   //�쳣��3
			}
			else if((j+data_num>ELECTRIC_N)||(j+data_num<=0))//�쳣��2 
			{
				MODBUS_SEND[MODBUS_SEND_LENGTH-2]=MODBUS_RECEIVE[1]|0x80;//������ĳɲ����
				MODBUS_SEND[MODBUS_SEND_LENGTH-1]=02;   //�쳣��2
			}
			else 
			{ 
				for(i=0;i<data_num;i++)
				{
					MODBUS_SEND[MODBUS_SEND_LENGTH++]=(Modbus_electric[j+i]>>8) & 0xff;//�ȸ��ֽ�
					MODBUS_SEND[MODBUS_SEND_LENGTH++]= Modbus_electric[j+i] & 0xff;//����ֽ�
				}
				
				/*if(j==0)
				{
					Modbus_electric[0]=0;
					Modbus_electric[1]=0;
				}*/
			}	
				modbus_get_crc16();//����CRCУ��
				MODBUS_SIGN=3;//��ʾ���Է�������
	    		
		}
		else if(MODBUS_RECEIVE[1]==3)//��ȡ����**************************************************************
	    {
			addr_begin=MODBUS_RECEIVE[2];
			addr_begin=(addr_begin<<8) | MODBUS_RECEIVE[3];//��ʼ��ַ

			data_num=MODBUS_RECEIVE[4];
			data_num=(data_num<<8) | MODBUS_RECEIVE[5];//����

			MODBUS_SEND[MODBUS_SEND_LENGTH++]=data_num<<1;//�ֽ���=�Ĵ�����������2

			j=addr_begin-40001;//��������ȥ40000

			if((data_num<1)||(data_num>0x007d))     //�쳣��3
			{
				MODBUS_SEND[MODBUS_SEND_LENGTH-2]=MODBUS_RECEIVE[1]|0x80;//������ĳɲ����
				MODBUS_SEND[MODBUS_SEND_LENGTH-1]=03;   //�쳣��3
			}
			else if((j+data_num>PARAMETER_N)||(j+data_num<=0))//�쳣��2
			{
				MODBUS_SEND[MODBUS_SEND_LENGTH-2]=MODBUS_RECEIVE[1]|0x80;//������ĳɲ����
				MODBUS_SEND[MODBUS_SEND_LENGTH-1]=02;   //�쳣��2
			}
			else 
			{
				for(i=0;i<data_num;i++)
				{
					MODBUS_SEND[MODBUS_SEND_LENGTH++]=(Modbus_parameter[j+i][0]>>8) & 0xff;//�ȸ��ֽ�
					MODBUS_SEND[MODBUS_SEND_LENGTH++]= Modbus_parameter[j+i][0] & 0xff;//����ֽ�
				}
			}
				modbus_get_crc16();//����CRCУ��
				MODBUS_SIGN=3;//��ʾ���Է�������
		}
		else if(MODBUS_RECEIVE[1]==6)//д��������
	    {	
			addr_begin=MODBUS_RECEIVE[2];
			addr_begin=(addr_begin<<8) | MODBUS_RECEIVE[3];//��ʼ��ַ

			data_value=MODBUS_RECEIVE[4];
			data_value=(data_value<<8) | MODBUS_RECEIVE[5];//ֵ

			j=addr_begin-40001;//��������ȥ40000

			 if((j>=PARAMETER_N)||(j<0))//�쳣��2
			{
				MODBUS_SEND[MODBUS_SEND_LENGTH-2]=MODBUS_RECEIVE[1]|0x80;//������ĳɲ����
				MODBUS_SEND[MODBUS_SEND_LENGTH-1]=02;   //�쳣��2
			}
			else 
			{
				Modbus_parameter[j][0]=data_value;
				Modbus_parameter[j][1]=1;	
				Modbus_write_sign=1;//�Ķ���
				for(i=2;i<6;i++)
					MODBUS_SEND[MODBUS_SEND_LENGTH++]=MODBUS_RECEIVE[i];
			}
			
			modbus_get_crc16();//����CRCУ��	
			MODBUS_SIGN=3;//��ʾ���Է�������

		}
		else if(MODBUS_RECEIVE[1]==16)//д�������
	    {

			addr_begin=MODBUS_RECEIVE[2];
			addr_begin=(addr_begin<<8) | MODBUS_RECEIVE[3];//��ʼ��ַ

			data_num=MODBUS_RECEIVE[4];
			data_num=(data_num<<8) | MODBUS_RECEIVE[5];//����

			//l=MODBUS_RECEIVE[6]>>1;//�ֽ���=�Ĵ�����������2

			j=addr_begin-40001;//��������ȥ40000


			if((data_num<1)||(data_num>0x007b)||(MODBUS_RECEIVE[6]!=data_num<<1)) //�쳣��3
			{
				MODBUS_SEND[MODBUS_SEND_LENGTH-2]=MODBUS_RECEIVE[1]|0x80;//������ĳɲ����
				MODBUS_SEND[MODBUS_SEND_LENGTH-1]=03;   //�쳣��3
			}
			else if((j+data_num>PARAMETER_N)||(j+data_num<=0))//�쳣��2
			{
				MODBUS_SEND[MODBUS_SEND_LENGTH-2]=MODBUS_RECEIVE[1]|0x80;//������ĳɲ����
				MODBUS_SEND[MODBUS_SEND_LENGTH-1]=02;   //�쳣��2
			}
			else 
			{
				for(i=0;i<data_num;i++)
				{
					Modbus_parameter[j+i][0]=MODBUS_RECEIVE[2*i+7];
					Modbus_parameter[j+i][0]=(Modbus_parameter[j+i][0]<<8)|MODBUS_RECEIVE[2*i+8];
					Modbus_parameter[j+i][1]=1;
				}
					Modbus_write_sign=1;
					for(i=2;i<6;i++)
						MODBUS_SEND[MODBUS_SEND_LENGTH++]=MODBUS_RECEIVE[i];//����Ӧ��
			}
			
			modbus_get_crc16();//����CRCУ��
			MODBUS_SIGN=3;//��ʾ���Է�������
		}
		else
		{
			MODBUS_SEND[MODBUS_SEND_LENGTH-1]=MODBUS_RECEIVE[1]|0x80;//������ĳɲ����
			MODBUS_SEND[MODBUS_SEND_LENGTH++]=01;   //�޹�������
			modbus_get_crc16();//����CRCУ��
			MODBUS_SIGN=3;//��ʾ���Է�������
		}
} 




/****************************************/
//������: modbus_in()
//����:  �ж��Ƿ� �������յ���һ������,�豸��ַ�Ƿ�һ��,RCR�Ƿ���ȷ
//����:2010.12.10
//
// 
//
/****************************************/
 void modbus_in()
{   
  	unsigned int k;
  	
  	if (MODBUS_SIGN==2)//һ�����ݽ������
	{
		if ((MODBUS_RECEIVE[0]==P_E2PROM[9])&&(modbus_if_crc16()==1))
//		if (MODBUS_RECEIVE[0]==P_E2PROM[40])//������
		{
			MODBUS_SEND[MODBUS_SEND_LENGTH++]=MODBUS_RECEIVE[0];//���ͻ����� ID
			MODBUS_SEND[MODBUS_SEND_LENGTH++]=MODBUS_RECEIVE[1];//���ͻ���������
			modbus_analysis();
			MODBUS_RECEIVE_LENGTH=0;//������ջ�����			
		}
		else 
		{
			MODBUS_RECEIVE_LENGTH=0;//��ս��ջ�����
			MODBUS_SIGN=4;			// ������ձ�ʶ
		}
	}
 	else  if(MODBUS_SIGN==3)//���Է�������
	{		
//		 RS485_send;
		 if(MODBUS_TIME>=MODBUS_T3_5)//���ݷ���
		 {	
			//*****����״̬****************************				
			while(1)
			{	
				while(SPI1STATbits.SPITBF==1);
				SS1=0;
				SPI1BUF=CN_LSR;
				while(SPI1STATbits.SPIRBF==0);
				k=SPI1BUF;
				SS1=1;
				
				CH432T_reset=(k>>7)&0x01;
				RS485_receive=k&0x01;// DATARDY Ϊ1 ��˵�������ݿ��Զ���  0���յ�����
				RS485_send_fifo=(k>>5)&0x01;// THRE 1���Լ�����fifo�������ݣ�0,fifo���� 16��
				RS485_send_over=(k>>6)&0x01;// TEMT 1����ȫ������			
				
			
				if((RS485_send_fifo==1)&&(Modbus_mortize<MODBUS_SEND_LENGTH))
				{
					while(SPI1STATbits.SPITBF==1);
					SS1=0;
					SPI1BUF=CN_THR|MODBUS_SEND[Modbus_mortize++];
					while(SPI1STATbits.SPIRBF==0);
					k=SPI1BUF;
					SS1=1;
				}
				else
				if((Modbus_mortize>=MODBUS_SEND_LENGTH)&&(RS485_send_over==1))
				{
						MODBUS_SIGN=4;//�����ȴ����� ����
						Modbus_mortize=0;
						MODBUS_SEND_LENGTH=0;//��շ��ͻ�����
						MODBUS_TIME=0;//׼����ʱ
						break;	
				}	
				else
				{
					break;		
				}
			}
		 }		
	}
 	else if(MODBUS_SIGN==4)//���ͽ�������һ�ν���ǰ����ʱ��
    {		
			if(MODBUS_TIME>MODBUS_T1_5)//����ķ�ʽ�ý��
			{
				MODBUS_SIGN=0;//�����ȴ����� ����
			}
	}
 	else
 	{                  //�ȴ�����
			modbus_read_data();
			if(CH432T_reset==1||BSP_reset==1)//��λ
			{
				CH432T_config();
				CH432T_reset=0;
				BSP_reset=0;
				MODBUS_SIGN=0;
			}
	 }
}  
#endif