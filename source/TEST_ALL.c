#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

volatile unsigned int TEST_SIGN;

void Ecan1_Config_test(void)//CAN1ģ������
{
	C1CTRL1bits.REQOP=4;		 //��������ģʽ
	while(C1CTRL1bits.OPMODE!=4);//�ȴ���������ģʽ
	
//����������
	C1CTRL1bits.CANCKS = 0x1;  //CANʱ��ΪFCY
	C1CFG1bits.SJW = 0x3;	   //ͬ����ת���
	C1CFG1bits.BRP = 19;       //50K ������
	C1CFG2bits.SEG1PH=0x7; //��λ�����1��8 TQ
	C1CFG2bits.SEG2PHTS = 0x1;//��λ�����2�������ɱ��
	C1CFG2bits.SEG2PH = 0x5;//��λ�����2��6 TQ
	C1CFG2bits.PRSEG = 0x4; //����ʱ���λ��5 TQ
	C1CFG2bits.SAM = 0x1;   //���߱���������
	
//���仺����
	C1FCTRLbits.FSA=0b01000;	//FIFO����ӵ�8����������ʼ
	C1FCTRLbits.DMABS=0b110;	//ʹ��DMA�е�32��������
	
//���ý��ջ�����
	ecan1WriteRxAcptFilter(0,0x206,1,15,0);//���������ã���ַ0x206����fifo������0
	
	ecan1WriteRxAcptMask(0,0x00,1,0);//����������
	
	C1CTRL1bits.REQOP=0;		  //������������ģʽ
	while(C1CTRL1bits.OPMODE!=0); //�ȴ�������������ģʽ
	
//**********************���շ�����Ϣ�Ŀ���******************
	C1RXFUL1=C1RXFUL2=C1RXOVF1=C1RXOVF2=0x0000;//���ջ����������Ĵ�����־λ������Ĵ�����־λ ����
	
	//TR0~TR3Ϊ���ͻ���������Ҫ�������������
	C1TR01CONbits.TXEN0=1;			/* ECAN1, Buffer 0 is a Transmit Buffer */
	
	//���ȼ���Ϊ���
	C1TR01CONbits.TX0PRI=0b11; 		/* Message Buffer 0 Priority Level */

	
//******�ж�����***********************************************
	IEC2bits.C1IE = 1;//ECAN1�¼��ж�ʹ��λ
	C1INTEbits.TBIE = 1;//���ͻ���������λ
	C1INTEbits.RBIE = 1;//���ջ���������λ
	
}

void UART232_receive(void)//�������ݣ�������յ�Ϊ1����ת����ȥ
{
	unsigned char receive;	

	while(1)
	{
		if(RS232_Rfifo==1)
		{
		   receive=U1RXREG;
		   if(receive==1)
		   {
                U1TXREG=1;//�ط�01  232����
				break;
		   }
		}
	}
}

void CAN_TEST(void)
{
	TEST_SIGN=0;
	ecan1WriteTxMsgBufId(0,0x0555AAA0,1,0);
	ecan1WriteTxMsgBufData(0,8,0xaaaa,0xaaaa,0xaaaa,0xaaaa);
	C1TR01CONbits.TXREQ0=1;
	
	DELAY_TIME=0;
	while(1)
	{
		if(TEST_SIGN==1)   break;
		else
		if(DELAY_TIME>=5000)//���500ms
		{
			U1TXREG=7;//can�ղ�������
			break;
		}
	}	
}	

void E2PROM_test(void)
{
	E2ROM_93LC86C_writedata(1023,0xaaaa);
	if(E2ROM_93LC86C_readdata(1023)==0xaaaa) 
	{
		U1TXREG=14;
		LED_light(2);
	}	
	else                                     
	{
		U1TXREG=15;	
		LED_light_ray(2);
	}	
}	

void SCI485_receive(void)//�������ݣ��������һ������ת����ȥ
{
	unsigned char receive,num=0,SCI_erro=0;
	unsigned char xu[8],xu_num=0,k;
	

	while(num<=6)//����������
	{	
		while(SPI1STATbits.SPITBF==1);
		SS1=0;
		SPI1BUF=CN_THR|0xaa;
		while(SPI1STATbits.SPIRBF==0);
		k=SPI1BUF;
		SS1=1;
					
		num++;
 	}   

	num=0;
	DELAY_TIME=0;//��ʱ����
	while(1)
	{
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
		
		if(RS485_receive==1)
		{
				while(SPI1STATbits.SPITBF==1);
				SS1=0;
				SPI1BUF=0;
				while(SPI1STATbits.SPIRBF==0);
				receive=SPI1BUF;
				SS1=1;			
				xu[xu_num++]=receive;
				
				if(receive==0x55)
				{
					num++;		
					if(num>=5)
					{
						U1TXREG=2;//485��·����			
						break;
					}
				}
				else
				{
							SCI_erro++;
							if(SCI_erro>=3)
							{
								U1TXREG=3;//485ͨѶ����
								break;
							}
				}
			
		}
	    
	      	if(DELAY_TIME>=10000)//���1000ms
			{
					U1TXREG=4;//485 ͨѶ ������ ���黻����
					break;
			}
	}
}

void TEST_ALL(void)//���еĲ���
{	
 	unsigned char name[]=
 	{
	  0xCC,0xDD,0x04,0x80,0x03,0x00,0x02,0xCC,0xDD,0x11,0x82,0x1A,0xF4,0xB4,0xAE,0xBF,0xDA,0xC6,0xC1,0xCD,0xA8,0xD0,0xC5,0xD5,0xFD,0xB3,0xA3 	 	
	};
	unsigned int i=0;
 	
 	Ecan1_Config_test();//����CAN����
	      
   while(1)
   {
		    UART232_receive();//232
		    CAN_TEST();       //CAN	
		    SCI485_receive();//485		
	
		E2PROM_test();
		
		while(i<27)
		{
			if(RS232dis_Sfifo==0)
					U2TXREG=name[i++];	
		}
		
		U1TXREG=8;
	
		i=0;
		U1TXREG=255;//������־				
	}
}


