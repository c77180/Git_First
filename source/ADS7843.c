#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"
#include "..\include\string_h.h"

#define ADS_DCLK LATDbits.LATD11
#define ADS_CS   LATDbits.LATD10
#define ADS_DIN  LATDbits.LATD9
#define ADS_DOUT PORTDbits.RD8 

unsigned int KEY_sign=0;//����Ѿ�����ȥ�ˣ�1, ����0����KEY_sign�ֲ�Ϊ0��ʱ����Ҫ��ԭ��ť���������λ
unsigned int KEY_list[5]={1,2,3,5,7};//��Ҫ��ʼ��  ��ť��˳��
unsigned int KEY_cipher[10];//������1,2223322222��
unsigned int KEY_cipher_num=0;//����λ����

unsigned long button_Xall=0;//����
unsigned long button_Yall=0;//����
unsigned int  button_num=0;//����ĸ���

unsigned int  button_sign=0;//��ť��ʶ

void ADS7843_Config(void)//ADS7843����
{
	_TRISD11=0;
	_TRISD10=0;
	_TRISD9=0;
	_TRISD8=1;
	_TRISA3=0;
}	

void ADS7843_init(void)  //��ʼ�� �����ź���
{
	ADS_CS=1;
	ADS_DIN=0;
	ADS_DCLK=0;
	ADS_CS=0;
}


void ADS7843_Wcom(unsigned char DATA)  //д������
{
	unsigned char TEMP=0X80,count;
	for(count=0;count<8;count++)
	{
	   if(DATA&TEMP){ADS_DIN=1;}	   	
	   else	{ADS_DIN=0;}	
	   ADS_DCLK=1;
       DELAY(0); 
       ADS_DCLK=0;
	   DELAY(0);
	   TEMP>>=1;
	}
}

unsigned int ADS7843_Rdata(void)  //����������
{
	unsigned char count=0;
    unsigned int DAT=0;
	for(count=0;count<12;count++)
	{
	   DAT<<=1;
	   ADS_DCLK=1; 
	   DELAY(0); //
	   ADS_DCLK=0; 
	   if(ADS_DOUT) 
       DAT++; 
	}
	return DAT;
}

unsigned int ADS7843_Rx1(void) //����X��Ĳ���ֵ һ��
{

		ADS7843_init();
		ADS7843_Wcom(0XD0);
		return ADS7843_Rdata();
} 
unsigned int ADS7843_Ry1(void)  //����Y��Ĳ���ֵ һ��
{
   unsigned int i;
		ADS7843_init();
		ADS7843_Wcom(0X90);
		i=ADS7843_Rdata();
	return (~(i-2047)+1);
} 

unsigned int ADS7843_Rxn(unsigned int time) //����X��Ĳ���ֵ
{
	unsigned int i,j=0;	
	for(i=0;i<time;i++) j+=ADS7843_Rx1();	
	return j/time;
} 
unsigned int ADS7843_Ryn(unsigned int time)  //����Y��Ĳ���ֵ
{
 	unsigned int i,j=0;	
	for(i=0;i<time;i++) j+=ADS7843_Ry1();	
	return j/time;
} 

unsigned int ADS7843_KEY(void)//���жϴ���Ҳ������
{
	unsigned int key=0,sign=0,datax1=0,datax2=0,datay1=0;

	if(KEY_sign==0)
	{
			if (ADS7843_Rx1()>50)
			{
					DELAY_Blight=0;
					DELAY_SLEEP=0;//�����ʱ�临λ
					light_b=1;//����ƿ��Ʊ���
					
					if(button_sign==0) 
					{
						DELAY_AD=0;
						button_sign=1;
					}
					else
					if(button_sign==1&&DELAY_AD>100) //��ʱ
					{
						button_sign=2;
						button_Xall=0;
						button_Yall=0;
						button_num=0;
						DELAY_AD=0;
						
					}
					else
					if(button_sign==2)//����
					{
						datax1=ADS7843_Rx1();
						datay1=ADS7843_Ry1();
						
						while(datax1!=0&&datay1!=0&&DELAY_AD<1000)
						{
							datax1=ADS7843_Rx1();
							datay1=ADS7843_Ry1();
							button_Xall+=datax1;
							button_Yall+=datay1;
							button_num++;
						}
						
						button_sign=0;
						
						datax1=button_Xall/button_num;
						datay1=button_Yall/button_num;
																		
						//if(datax1>=1620 && datax1<=1900)
						if(datax1>=1350 && datax1<=1900)
						{
							//if(datay1>=330 && datay1<=570 && KEY_list[0]!=0)
							if(datay1>=330 && datay1<=680 && KEY_list[0]!=0)
							{
								LCD6448_button_down(534,50,0);//��ť״̬�ı�
								KEY_sign=1;
								KEY_cipher_num=0;
							}
							else
							if(datay1>=700 && datay1<=860 && KEY_list[1]!=0)
							//if(datay1>=720 && datay1<=900 && KEY_list[1]!=0)
							{
								LCD6448_button_down(534,128,1);
								KEY_sign=2;
								
								if(KEY_cipher_num<10)
								            KEY_cipher[KEY_cipher_num++]=2;
								else		KEY_cipher_num=0;
							}
							else
							//if(datay1>=910 && datay1<=1150 && KEY_list[2]!=0)
							if(datay1>=950 && datay1<=1100 && KEY_list[2]!=0)
							{
								LCD6448_button_down(534,206,2);
								KEY_sign=3;
								
								if(KEY_cipher_num<10)
								            KEY_cipher[KEY_cipher_num++]=3;
								else		KEY_cipher_num=0;
							}
							else
							//if(datay1>=1200 && datay1<=1440 && KEY_list[3]!=0)
							if(datay1>=1120 && datay1<=1300 && KEY_list[3]!=0)
							{
								LCD6448_button_down(534,284,3);
								KEY_sign=4;
							}
							else
							if(datay1>=1350 && datay1<=1740 && KEY_list[4]!=0)
							//if(datay1>=1600 && datay1<=1740 && KEY_list[4]!=0)
							{
								LCD6448_button_down(534,362,4);
								KEY_sign=5;
								
								if(KEY_cipher_num==10)
								{
									if(KEY_cipher[0]==2&&KEY_cipher[1]==2&&KEY_cipher[2]==2&&KEY_cipher[3]==3&&KEY_cipher[4]==3&&KEY_cipher[5]==2&&KEY_cipher[6]==2&&KEY_cipher[7]==2&&KEY_cipher[8]==2&&KEY_cipher[9]==2)
									{
										page_sign=15;
										page15_sign=0;
										KEY_cipher_num=0;
										KEY_sign=0; 
									}	
								}	
							}
						}				
					  }
		 }//if (ADS7843_Rx1()>50)
	}
	else
	{
			if (ADS7843_Rx1()<50)
			{		
					
					if(KEY_sign==1)
					{
						LCD6448_button_up(534,50,0);
					}
					else
					if(KEY_sign==2)
					{
						LCD6448_button_up(534,128,1);
					}
					else
					if(KEY_sign==3)
					{
						LCD6448_button_up(534,206,2);
					}
					else
					if(KEY_sign==4)
					{
						LCD6448_button_up(534,284,3);
					}
					else
					if(KEY_sign==5)
					{
						LCD6448_button_up(534,362,4);
					}
					
					key=KEY_sign;				
					KEY_sign=0;
			}
	}	
	
	return key;	
}	
	


