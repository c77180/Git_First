#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

//�����ʼ���
#define FCAN  	40000000 
#define BITRATE 500000  
#define NTQ 	20		// 20 Time Quanta in a Bit Time
#define BRP_VAL		((FCAN/(2*NTQ*BITRATE))-1)

//#define FCAN  	36850000
//#define BITRATE 500000  
//#define NTQ 	20		// 20 Time Quanta in a Bit Time
//#define BRP_VAL		((FCAN/(2*NTQ*BITRATE))-1)

//DMA CAN�ռ�����
#define  ECAN1_MSG_BUF_LENGTH 	32
typedef unsigned int ECAN1MSGBUF [ECAN1_MSG_BUF_LENGTH][8];	//��RAM�ռ� ����һ��������
extern ECAN1MSGBUF  ecan1msgBuf __attribute__((space(dma)));
ECAN1MSGBUF ecan1msgBuf __attribute__((space(dma),aligned(ECAN1_MSG_BUF_LENGTH*16))); //ָ����������С���뷽ʽ


//
#define ReceiveData   _RBIF  //�����ջ������յ�����ʱ����1�������жϣ��ֶ�����
#define ReceiveBuff0  _RXFUL0//���ջ�����0,���ձ�־λ
#define ReceiveBuff1  _RXFUL1//���ջ�����1,���ձ�־λ
#define ReceiveBuff2  _RXFUL2//���ջ�����2,���ձ�־λ
#define ReceiveBuff3  _RXFUL3//���ջ�����3,���ձ�־λ
#define ReceiveBuff4  _RXFUL4//���ջ�����4,���ձ�־λ

void DMA_C1S(void)//DMA���ã�CAN1�����ͣ�ͨ��0
{	
	 DMACS0=0;      //����ͨ���ĳ�ͻ��־λ��λ
     DMA0CON=0x2020;//�֣���DMAд�����裬������������жϣ���������
     				//������Ѱַģʽ����ֹ��������ƹ��ģʽ
	 DMA0PAD=0x0442;//CAN1�����͵��������ݼĴ�����ַ��C1TXD
 	 DMA0CNT=0x0007;//DMA ��������Ĵ���λ��  �������Ҫ��ʼ���أ���8��
	 DMA0REQ=0x0046;//CAN1���� �жϵ�ַ
	 DMA0STA=  __builtin_dmaoffset(ecan1msgBuf);//DMA RAM��ʼ��ַλ����֪�����������
	 DMA0CONbits.CHEN=1;//ʹ�� ͨ��0
}


void DMA_C1R(void)//DMA���ã�CAN1�����գ�ͨ��0
{
	 DMACS0=0;
     DMA2CON=0x0020;
	 DMA2PAD=0x0440;	/* ECAN 1 (C1RXD) */
 	 DMA2CNT=0x0007;
	 DMA2REQ=0x0022;	/* ECAN 1 Receive */
	 DMA2STA= __builtin_dmaoffset(ecan1msgBuf);	
	 DMA2CONbits.CHEN=1;
}

void ecan1WriteRxAcptFilter(int n, long identifier, unsigned int exide, unsigned int bufPnt,unsigned int maskSel) 
{//�������ý��չ�������n:�������ţ�identifier:SID;exide:0:��׼֡��1:��չ֡��bufPnt:������ָ��0~14��15fifo��maskSel��11�������Σ�else����Ӧ�������ĺ���
 //
		unsigned long sid10_0=0, eid15_0=0, eid17_16=0;
		unsigned int *sidRegAddr,*bufPntRegAddr,*maskSelRegAddr, *fltEnRegAddr;

		C1CTRL1bits.WIN=1;
		// Obtain the Address of CiRXFnSID, CiBUFPNTn, CiFMSKSELn and CiFEN register for a given filter number "n"
		sidRegAddr = (unsigned int *)(&C1RXF0SID + (n << 1));
		bufPntRegAddr = (unsigned int *)(&C1BUFPNT1 + (n >> 2));
		maskSelRegAddr = (unsigned int *)(&C1FMSKSEL1 + (n >> 3));
		fltEnRegAddr = (unsigned int *)(&C1FEN1);

	// Bit-filed manupulation to write to Filter identifier register
	if(exide==1) 
	{ 	// Filter Extended Identifier
		eid15_0 = (identifier & 0xFFFF);
		eid17_16= (identifier>>16) & 0x3;
		sid10_0 = (identifier>>18) & 0x7FF;

		*sidRegAddr=(((sid10_0)<<5) + 0x8) + eid17_16;	// Write to CiRXFnSID Register
	    *(sidRegAddr+1)= eid15_0;					// Write to CiRXFnEID Register

	}
	else
	{			// Filter Standard Identifier
		sid10_0 = (identifier & 0x7FF);			
		*sidRegAddr=(sid10_0)<<5;					// Write to CiRXFnSID Register
		*(sidRegAddr+1)=0;							// Write to CiRXFnEID Register
	}

   *bufPntRegAddr = (*bufPntRegAddr) & (0xFFFF - (0xF << (4 *(n & 3)))); // clear nibble
   *bufPntRegAddr = ((bufPnt << (4 *(n & 3))) | (*bufPntRegAddr));       // Write to C1BUFPNTn Register

   *maskSelRegAddr = (*maskSelRegAddr) & (0xFFFF - (0x3 << ((n & 7) * 2))); // clear 2 bits
   *maskSelRegAddr = ((maskSel << (2 * (n & 7))) | (*maskSelRegAddr));      // Write to C1FMSKSELn Register

   *fltEnRegAddr = ((0x1 << n) | (*fltEnRegAddr)); // Write to C1FEN1 Register

   C1CTRL1bits.WIN=0;
}


void ecan1WriteRxAcptMask(int m, long identifier, unsigned int mide, unsigned int exide)
{//�������μĴ�����m:���μĴ����ţ�identifier:SID;mide:1,exide:0:��׼֡����չ֡

		unsigned long sid10_0=0, eid15_0=0, eid17_16=0;
		unsigned int *maskRegAddr;

	C1CTRL1bits.WIN=1;

	// Obtain the Address of CiRXMmSID register for given Mask number "m"
	maskRegAddr = (unsigned int *)(&C1RXM0SID + (m << 1));

	// Bit-filed manupulation to write to Filter Mask register
	if(exide==1) 
	{ 	// Filter Extended Identifier
		eid15_0 = (identifier & 0xFFFF);
		eid17_16= (identifier>>16) & 0x3;
		sid10_0 = (identifier>>18) & 0x7FF;

		if(mide==1)
			*maskRegAddr=((sid10_0)<<5) + 0x0008 + eid17_16;	// Write to CiRXMnSID Register
		else
			*maskRegAddr=((sid10_0)<<5) + eid17_16;	// Write to CiRXMnSID Register
	    *(maskRegAddr+1)= eid15_0;					// Write to CiRXMnEID Register
	}
	else
	{			// Filter Standard Identifier
		sid10_0 = (identifier & 0x7FF);			
		if(mide==1)
			*maskRegAddr=((sid10_0)<<5) + 0x0008;					// Write to CiRXMnSID Register
		else
			*maskRegAddr=(sid10_0)<<5;					// Write to CiRXMnSID Register	
		
		*(maskRegAddr+1)=0;							// Write to CiRXMnEID Register
	}

	C1CTRL1bits.WIN=0;	
}


void ecan1WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit)
{//���÷��Ͳ���:buf���������ţ�txIdentifier:ID;ide��0����׼��1����չ��remoteTransmit��0��������Ϣ��1��Զ����Ϣ��
	
			unsigned long word0=0, word1=0, word2=0;
			unsigned long sid10_0=0, eid5_0=0, eid17_6=0;

		if(ide)
		{
			eid5_0  = (txIdentifier & 0x3F);
			eid17_6 = (txIdentifier>>6) & 0xFFF;
			sid10_0 = (txIdentifier>>18) & 0x7FF;
			word1 = eid17_6;
		}
		else
		{
			sid10_0 = (txIdentifier & 0x7FF);
		}
	
		if(remoteTransmit==1) 
		{ 	// Transmit Remote Frame
			word0 = ((sid10_0 << 2) | ide | 0x2);
			word2 = ((eid5_0 << 10)| 0x0200);
		}
		else 
		{	
			word0 = ((sid10_0 << 2) | ide);
			word2 = (eid5_0 << 10);
		}		
// Obtain the Address of Transmit Buffer in DMA RAM for a given Transmit Buffer number
		if(ide)
			ecan1msgBuf[buf][0] = (word0 | 0x0002);
		else
			ecan1msgBuf[buf][0] = word0;

		ecan1msgBuf[buf][1] = word1;
		ecan1msgBuf[buf][2] = word2;
}

void ecan1WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4)
{//buf���������ţ�dataLength:����;
	ecan1msgBuf[buf][2] = ((ecan1msgBuf[buf][2] & 0xFFF0) + dataLength) ;
	
	ecan1msgBuf[buf][3] = data1;
	ecan1msgBuf[buf][4] = data2;
	ecan1msgBuf[buf][5] = data3;
	ecan1msgBuf[buf][6] = data4;
}

void ecan1DisableRXFilter(int n)//��ֹ���չ���ʹ�ܼĴ���
{
	unsigned int *fltEnRegAddr;
   C1CTRL1bits.WIN=1;
   fltEnRegAddr = (unsigned int *)(&C1FEN1);
   *fltEnRegAddr = (*fltEnRegAddr) & (0xFFFF - (0x1 << n));
   C1CTRL1bits.WIN=0;
}

void Ecan1_Config(void)//CAN1ģ������
{
	C1CTRL1bits.REQOP=4;		 //��������ģʽ
	while(C1CTRL1bits.OPMODE!=4);//�ȴ���������ģʽ
	
//����������
	C1CTRL1bits.CANCKS = 0x1;  //CANʱ��ΪFCY
	C1CFG1bits.SJW = 0x3;	   //ͬ����ת���
	C1CFG1bits.BRP = BRP_VAL;
	C1CFG2bits.SEG1PH=0x7; //��λ�����1��8 TQ
	C1CFG2bits.SEG2PHTS = 0x1;//��λ�����2�������ɱ��
	C1CFG2bits.SEG2PH = 0x5;//��λ�����2��6 TQ
	C1CFG2bits.PRSEG = 0x4; //����ʱ���λ��5 TQ
	C1CFG2bits.SAM = 0x1;   //���߱���������
	
//���仺����
	C1FCTRLbits.FSA=0b01000;	//FIFO����ӵ�8����������ʼ
//	C1FCTRLbits.FSA=0b10000;	//FIFO����ӵ�16����������ʼ
	C1FCTRLbits.DMABS=0b110;	//ʹ��DMA�е�32��������
	
//���ý��ջ�����

//	ecan1WriteRxAcptFilter(1,0x200,0,4,3); //���������ã���ַ0x201����׼֡��������4��������
//	ecan1WriteRxAcptFilter(1,0x201,0,5,3); //
//	ecan1WriteRxAcptFilter(1,0x202,0,6,3); //
//	ecan1WriteRxAcptFilter(1,0x203,0,7,3);//
//	ecan1WriteRxAcptFilter(1,0x204,0,8,3);//
//	ecan1WriteRxAcptFilter(1,0x205,0,9,3);//
//	ecan1WriteRxAcptFilter(1,0x206,0,10,3);//
//	ecan1WriteRxAcptFilter(1,0x207,0,11,3);//
//	ecan1WriteRxAcptFilter(1,0x208,0,12,3);//

	ecan1WriteRxAcptFilter(0,0x206,0,15,0);//���������ã���ַ0x206����׼֡��fifo������0
	
	ecan1WriteRxAcptMask(0,0x00,1,0);//����������
	
	C1CTRL1bits.REQOP=0;		  //������������ģʽ
	while(C1CTRL1bits.OPMODE!=0); //�ȴ�������������ģʽ
	
//**********************���շ�����Ϣ�Ŀ���******************
	C1RXFUL1=C1RXFUL2=C1RXOVF1=C1RXOVF2=0x0000;//���ջ����������Ĵ�����־λ������Ĵ�����־λ ����
	
	//TR0~TR3Ϊ���ͻ���������Ҫ�������������
	C1TR01CONbits.TXEN0=1;			/* ECAN1, Buffer 0 is a Transmit Buffer */
	C1TR01CONbits.TXEN1=1;			/* ECAN1, Buffer 0 is a Transmit Buffer */
	C1TR23CONbits.TXEN2=1;			/* ECAN1, Buffer 0 is a Transmit Buffer */
	C1TR23CONbits.TXEN3=1;			/* ECAN1, Buffer 0 is a Transmit Buffer */
//	C1TR45CONbits.TXEN4=0;			/* ECAN1, Buffer 0 is a Transmit Buffer */
//	C1TR45CONbits.TXEN5=0;			/* ECAN1, Buffer 0 is a Transmit Buffer */
//	C1TR67CONbits.TXEN6=0;			/* ECAN1, Buffer 0 is a Transmit Buffer */
//	C1TR67CONbits.TXEN7=0;			/* ECAN1, Buffer 0 is a Transmit Buffer */

	//���ȼ���Ϊ���
	C1TR01CONbits.TX0PRI=0b11; 		/* Message Buffer 0 Priority Level */
	C1TR01CONbits.TX1PRI=0b11; 		/* Message Buffer 0 Priority Level */
	C1TR23CONbits.TX2PRI=0b11; 		/* Message Buffer 0 Priority Level */
	C1TR23CONbits.TX3PRI=0b11; 		/* Message Buffer 0 Priority Level */
//	C1TR45CONbits.TX4PRI=0b11; 		/* Message Buffer 0 Priority Level */
//	C1TR45CONbits.TX5PRI=0b11; 		/* Message Buffer 0 Priority Level */
//	C1TR67CONbits.TX6PRI=0b11; 		/* Message Buffer 0 Priority Level */
//	C1TR67CONbits.TX7PRI=0b11; 		/* Message Buffer 0 Priority Level */
	
	
//******�ж�ʹ������***********************************************
	IEC2bits.C1IE = 1;//ECAN1�¼��ж�ʹ��λ
	C1INTEbits.TBIE = 1;//���ͻ���������λ
	C1INTEbits.RBIE = 1;//���ջ���������λ
	
}

void CAN1_examp(void)
{
	ecan1WriteTxMsgBufId(0,0x101,0,0);
	ecan1WriteTxMsgBufData(0,8,0x1111,0x1111,0x1111,0x1111);

	ecan1WriteTxMsgBufId(1,0x102,0,0);
	ecan1WriteTxMsgBufData(1,8,0x2222,0x2222,0x2222,0x2222);

	ecan1WriteTxMsgBufId(2,0x103,0,0);
	ecan1WriteTxMsgBufData(2,8,0x3333,0x3333,0x3333,0x3333);
}	

void __attribute__((interrupt, no_auto_psv))_C1Interrupt(void)  
{    	
	int k,xu[12];//debug
    unsigned int id,num,sign;
	
	IFS2bits.C1IF = 0;        // clear interrupt flag
	if(C1INTFbits.TBIF)//�����ж�
    { 
    	C1INTFbits.TBIF = 0;	
    	//���ͳɹ���������Ҫһ�����ͻ��壬���ڷ���������
    } 
 
    if(C1INTFbits.RBIF)//�����ж�
    {      
		while(C1FIFObits.FNRB!=C1FIFObits.FBP)
		{
			id=(ecan1msgBuf[C1FIFObits.FNRB][0]>>2) & 0x7FF;	
			xu[0]=ecan1msgBuf[C1FIFObits.FNRB][3];
			xu[1]=ecan1msgBuf[C1FIFObits.FNRB][4];
			xu[2]=ecan1msgBuf[C1FIFObits.FNRB][5];
			xu[3]=ecan1msgBuf[C1FIFObits.FNRB][6];

			if(id==0x155)//���Գ���
			{
               TEST_SIGN=1;
               if(xu[0]==0xaaaa&&xu[1]==0xaaaa&&xu[2]==0xaaaa&&xu[3]==0xaaaa)  U1TXREG=5;//CAN ����
               else  U1TXREG=6;   //CANͨѶ����   
			}
			else
			if((id & 0xff0)==0x100)//��������
			{
				sign=ecan1msgBuf[C1FIFObits.FNRB][3] & 0xff;//��ʶ
				num=ecan1msgBuf[C1FIFObits.FNRB][3]>>8;//����
				
				if(sign<=11)  //���Ͳ�������
				{
					if(num<63)//
					{
						record_wave[sign][num]=ecan1msgBuf[C1FIFObits.FNRB][4];
						record_wave[sign][num+1]=ecan1msgBuf[C1FIFObits.FNRB][5];
						record_wave[sign][num+2]=ecan1msgBuf[C1FIFObits.FNRB][6];
					}
					else//���һ����
					{
						record_wave[sign][63]=ecan1msgBuf[C1FIFObits.FNRB][4];	
					}		
				}	
				else
				if(sign>=12)//���ͷǲ������� ��ʱ��15
				{
					record_single[sign-12][0]=ecan1msgBuf[C1FIFObits.FNRB][4];
					record_single[sign-12][1]=ecan1msgBuf[C1FIFObits.FNRB][5];
					record_single[sign-12][2]=ecan1msgBuf[C1FIFObits.FNRB][6];
					
					if(sign==15) 
					{
						electric_receive_sign=1;//֪ͨ�������ݽ�����Ͽ���
						ask_time=DELAY_elect_synch;
	//					DELAY_TIME=0;
					}
				}
			}
			else
			if(id==0x301)//�����ٻ��Ĳ�������Ҫһ����ʶλ
			{			
				sign=(ecan1msgBuf[C1FIFObits.FNRB][3]&0xff)%3;//�ڼ�������
				
				hide_parameter_b[sign][0]=ecan1msgBuf[C1FIFObits.FNRB][3];
				hide_parameter_b[sign][1]=ecan1msgBuf[C1FIFObits.FNRB][4];
				hide_parameter_b[sign][2]=ecan1msgBuf[C1FIFObits.FNRB][5];
				hide_parameter_b[sign][3]=ecan1msgBuf[C1FIFObits.FNRB][6];
				
				parameter_receive_sign=ecan1msgBuf[C1FIFObits.FNRB][3]&0xff;//һ���������������	
				parameter_receive_SMSsign=parameter_receive_sign;//ͬ���ĸ�ֵ
				//if(sign==2) ��ʶλ��λ	
			}
			else
			if(id==0x001)//0x001,���ϴ����Լ�״̬
			{
				sign=ecan1msgBuf[C1FIFObits.FNRB][3];

					DELAY_CAN[0]=0;//���㣬�����ж��豸�Ƿ�����**
					
					equipment_skate[0][0]=sign;
					if(ecan1msgBuf[C1FIFObits.FNRB][4]!=0)//����й��ϴ���Ÿ�ֵ
				            equipment_skate[0][1]=ecan1msgBuf[C1FIFObits.FNRB][4];//���ϴ���
					equipment_skate[0][2]=ecan1msgBuf[C1FIFObits.FNRB][5];//����״̬
					equipment_skate[0][3]=ecan1msgBuf[C1FIFObits.FNRB][6];//�豸������
	
					k++;
			}
			else
			if(id==0x400)//�������
			{
				sign=ecan1msgBuf[C1FIFObits.FNRB][4];//ID �豸ID
				equipment_ID[0][1]=sign;
				equipment_ID[0][2]=xu[2];
				equipment_ID[0][3]=xu[3];	
			}
			
	//��ȡ���������ʶλ		
			if(C1FIFObits.FNRB<16)//
				C1RXFUL1=~(0x01<<C1FIFObits.FNRB);
			else
				C1RXFUL2=~(0x01<<(C1FIFObits.FNRB-16));
			C1INTFbits.RBIF = 0;
			
			k++;
		
		}
		
	}
	
	//	IFS2bits.C1IF=0;//�������㣿��
	
}


void __attribute__((interrupt, no_auto_psv))_C2Interrupt(void)  
{
	IFS3bits.C2IF = 0;        // clear interrupt flag
	if(C2INTFbits.TBIF)
    { 
		C2INTFbits.TBIF = 0;
    } 
    
    if(C2INTFbits.RBIF)
     {      
		C2INTFbits.RBIF = 0;
     }
}
 

//------------------------------------------------------------------------------
//    DMA interrupt handlers
//------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
   IFS0bits.DMA0IF = 0;          // Clear the DMA0 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void)
{
   IFS0bits.DMA1IF = 0;          // Clear the DMA1 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{
   IFS1bits.DMA2IF = 0;          // Clear the DMA2 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA3Interrupt(void)
{
   IFS2bits.DMA3IF = 0;          // Clear the DMA3 Interrupt Flag;
}







