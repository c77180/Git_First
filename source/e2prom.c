#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

#define E_DO   LATDbits.LATD0   //����  ����
#define E_DI   PORTDbits.RD11
#define E_SCK  LATCbits.LATC13
#define E_CS   LATCbits.LATC14

#define EWEN_CMD    0x1300       // ��дʹ��
#define EWDS_CMD    0x1000       // ��д��ֹ
#define WRITE_CMD   0x1400       // д       ��Ҫ��ַ
#define READ_CMD    0x1800       // ��       ��Ҫ��ַ
#define ERASE_CMD   0x1c00      // ����      ��Ҫ��ַ
#define ERAL_CMD    0x1200      // ȫ������ 
#define WRAL_CMD    0x1100      // ȫ��д 

#define E2_SCK_DELAY DELAY(1);  //  SCK 2.5V ��VCC< 4.5V   200~250ns    ƽ��5������
#define E2_CS_DELAY  DELAY(1);  //  CS����ʱ�� 2.5V ��VCC< 4.5V   100ns

volatile unsigned int __attribute__((__far__)) add_error_data[15][11];//���������0~9������Ϣ��10:��ַ��11��д��־
volatile unsigned int error_write=0,error_read=0;

void E2ROM_93LC86C_Config(void)//E2PROM����
{
	_TRISD0=0;
	_TRISD11=1;
	_TRISC13=0;
	_TRISC14=0;
}

void E2ROM_93LC86C_WCommand(unsigned int data)//д13λ������
{
  unsigned char i=0;

  E_CS=1;
  E2_CS_DELAY;
  for(i=0;i<13;i++)
  {
     if (data & 0x1000)    E_DO = 1;    //�ӵ�13λ��ʼ          
     else                E_DO = 0;                       
     data=data << 1;
                  
    E_SCK = 0;                       
    E2_SCK_DELAY;                  
    E_SCK = 1; 
	E2_SCK_DELAY;//д���� ������д��
  }
} 

void E2ROM_93LC86C_Wdata(unsigned int data)//д16λ������
{
  unsigned char i=0;

  for(i=0;i<16;i++)
  {
     if (data & 0x8000)    E_DO = 1;    //�ӵ�16λ��ʼ          
     else                E_DO = 0;                       
     data=data << 1;
    
    E_SCK = 0;                       
    E2_SCK_DELAY;                  
    E_SCK = 1; 
	E2_SCK_DELAY;            //������д����
  }
} 

void E2ROM_93LC86C_Busy(void)//�ж϶�д�Ƿ��Ѿ�����
{
  E_CS=0;
  E2_SCK_DELAY;//��Ҫ250
  E_CS=1;
  E2_SCK_DELAY;//��Ҫ300 
  while(E_DI==0){};
  E2_CS_DELAY;
  E_CS=0;
  E2_CS_DELAY;  //��Ҫ���ٺ���������DOλ
}

int E2ROM_93LC86C_Read(void)//������   �������ô����, ֱ�Ӷ�ȡ
{
  unsigned char i=0;
  int data=0;

  for(i=0;i<16;i++)
  {
    E_SCK = 0;                       
    E2_SCK_DELAY;
	E_SCK = 1;
    E2_SCK_DELAY;

    data = data << 1; 
    if (E_DI == 1) data=data | 0x0001;
  }
  E_CS=0;
  E2_CS_DELAY;  //��Ҫ���ٺ���������DOλ
  return data;
}

void E2ROM_93LC86C_writedata(unsigned int addr,int data)//������λdata��д���ַΪaddr�ĵ�ַ  
{
  E2ROM_93LC86C_WCommand(EWEN_CMD);//��дʹ��
  E_CS=0;
  E2_CS_DELAY;  //��ʱ


  E2ROM_93LC86C_WCommand(addr| WRITE_CMD);
  E2ROM_93LC86C_Wdata(data);
  E2ROM_93LC86C_Busy();

  E2ROM_93LC86C_WCommand(EWDS_CMD);//��д��ֹ
  E_CS=0;
  E2_CS_DELAY;  //��ʱ
}

int E2ROM_93LC86C_readdata(unsigned int addr)//����ַΪaddr  ������
{
  int data;
  addr=addr | READ_CMD;  //12,11,10:  110
  E2ROM_93LC86C_WCommand(addr);//д��ָ��� ֱ�Ӷ�ȡ������
  data=E2ROM_93LC86C_Read();

  return data;
}

unsigned int E2ROM_WR(unsigned int add,unsigned int num,volatile unsigned int *character)
{//��character ��num����д��add��ʼ�ĵ�ַ��
		unsigned i,j=1;	
			for(i=0;i<num;i++)//���ϴ����ʼ��
			{
				E2ROM_93LC86C_writedata(i+add,character[i]);
				if(E2ROM_93LC86C_readdata(i+add)!=character[i]) {j=0;break;}
			}  	
	  return j;
}	

void E2ROM_RD(unsigned int add,unsigned int num,volatile unsigned int *character)
{//�ѵ�ַΪadd�����ݶ�num����character�У�
	unsigned int i;
			for(i=0;i<num;i++)//
				character[i]=E2ROM_93LC86C_readdata(i+add);//e2prom����	
}	

void E2ROM_93LC86C_init()//�����ĳ�ʼ������������ӵĲ������������****************
{
	unsigned int i,j=0;

	  j=E2ROM_93LC86C_readdata(0);
	  i=0;
	  if(j!=0x9997)//���û�г�ʼ���������ʼ��
	  {			 
			 j=E2ROM_WR(1,98,P_E2PROM+1);//ϵͳ���ó�ʼ��
			 j=E2ROM_WR(950,39,P_E2PROM+99);//ϵͳ���ó�ʼ��

			if(j==1)//д��
		   {
				E2ROM_93LC86C_writedata(0,P_E2PROM[0]);//д���־λ
				if(E2ROM_93LC86C_readdata(0)!=P_E2PROM[0]) j=0;
		   }
	  }
	  else
	  {			
			E2ROM_RD(1,98,P_E2PROM+1);//��ȡE2�еĲ���
			E2ROM_RD(950,39,P_E2PROM+99);//��ȡE2�еĲ���
	  }
	  
	DELAY_BLIGHT_SIGN=P_E2PROM[7];DELAY_BLIGHT_SIGN*=600000;//����ʱ��
}

void WRITE_fault_one(unsigned int fault,unsigned int mode)//д���ϴ���
{
	if(P_E2PROM[1]!=0)
	{
	 if(P_E2PROM[2]>=499) P_E2PROM[2]=0;
	 else                   P_E2PROM[2]++;
	}
	else P_E2PROM[2]=0;

	add_error_data[error_write][1]=TIME_NOW[0];
	add_error_data[error_write][2]=TIME_NOW[1];
	add_error_data[error_write][3]=TIME_NOW[2];
	add_error_data[error_write][4]=TIME_NOW[4];
	add_error_data[error_write][5]=TIME_NOW[5];	
	add_error_data[error_write][6]=fault;
	add_error_data[error_write][7]=mode;//ģ�����
		
	add_error_data[error_write][10]=P_E2PROM[2];
	
	if(error_write==14) error_write=0;
	else				error_write++;
	
	if(P_E2PROM[1]<500) P_E2PROM[1]++;
	E2ROM_WR(1,2,P_E2PROM+1);//д��E2

	change_data_only=0x5a;//׼����ȡread
	DELAY_TIME=0;	
}	

void WRITE_fault_shadow(unsigned int fault,unsigned int mode)//д���ϴ���
{
	 if(P_E2PROM[8]>=892) P_E2PROM[8]=740;
	 else
	 if(P_E2PROM[8]<740)  P_E2PROM[8]=740;
	 else                 P_E2PROM[8]+=8;
	 
	E2ROM_93LC86C_writedata(P_E2PROM[8],0xaaaa);
	E2ROM_93LC86C_writedata(P_E2PROM[8]+1,TIME_NOW[0]);
	E2ROM_93LC86C_writedata(P_E2PROM[8]+2,TIME_NOW[1]);
	E2ROM_93LC86C_writedata(P_E2PROM[8]+3,TIME_NOW[2]);
	E2ROM_93LC86C_writedata(P_E2PROM[8]+4,TIME_NOW[4]);
	E2ROM_93LC86C_writedata(P_E2PROM[8]+5,TIME_NOW[5]);
	E2ROM_93LC86C_writedata(P_E2PROM[8]+6,fault);
	E2ROM_93LC86C_writedata(P_E2PROM[8]+7,mode);//ģ�����

	E2ROM_WR(8,1,P_E2PROM+8);//д��E2
}


void CLEAR_fault_all(unsigned int type)//������й��� �������ó�ʼ��
{//0���������ã�1���������;2:������ʱ��
	unsigned int i;
unsigned int P_E2PROM_b[]=//��������
{
	0x9997, //0 E2ʹ��
	0,	    //1 ��������
	0,		//2 ���¹��ϵ�ַ
	0,	    //3 �豸������ʱ�� - ��
	0,	    //4 �豸������ʱ�� - ��
	64,	    //5 ��������
	0,	    //6 �ر�����
	10,	    //7 ����ʱ��
	740,	//8 ���¹��ϵ�ַ
	21,	    //9 modbusվ��ַ
	0000,	//10 �����ֻ�����1~4
	0000,	//11 �����ֻ�����5~8
	000,	//12 �����ֻ�����9~11
	1,      //13 һ�����ٸ������豸
	0,		//14 0���أ�1������2ȫ��
	2,		//15 �Ƿ���ʾ����THDֵ
	3,		//16 �Ƿ���ʾ����THDֵ
	5,		//17 �Ƿ���ʾ������������
	6,      //18 �Ƿ���ʾ���ع�������
	0,		//19 �Ƿ��Ƿ��Ż���ʾ���� 0���Ż�,1ȫ�Ż���2���Ż� ��ʱ����********
	1,      //20 �Ƿ���ʾ������� ��Ӧ29*
	800,    //21 ����ˢ��ʱ��
	0,      //22 ���ݻ����С
	11,      //23 ��Ʒ��ţ���
	1,      //24 ��Ʒ��ţ���
	1, 		//25 ��Ʒ��ţ���
	4,      //26 �Ƿ���ʾ������ѹ������***
	0,      //27 �Ƿ���ʾװ�����
	1,      //28 �Ƿ���ʾ���ص������� 1����ʾ��0������ʾ ��ʱû����
	1,      //29 �Ƿ���ʾ�����������*
	0,		//30 �Ƿ�ͨѶ�ж���ʾ��ͣ1����ʾ��0������ʾ     ��ʱ����****
	0,      //31 �Ƿ���˫ģ�飬�����¶ȣ��������ϴ���
	0,		//32 ��һ�Ƿ�ʱ�����豸
	8,		//33 ��һ�����豸-ʱ
	30,		//34 ��һ�����豸-��
	0,		//35 ��һ�Ƿ�ʱֹͣ�豸
	17,		//36 ��һֹͣ�豸-ʱ
	30,		//37 ��һֹͣ�豸-��

	0,		//38 �ܶ��Ƿ�ʱ�����豸
	8,		//39 �ܶ������豸-ʱ
	30,		//40 �ܶ������豸-��
	0,		//41 �ܶ��Ƿ�ʱֹͣ�豸
	17,		//42 �ܶ�ֹͣ�豸-ʱ
	30,		//43 �ܶ�ֹͣ�豸-��

	0,		//44 �����Ƿ�ʱ�����豸
	8,		//45 ���������豸-ʱ
	30,		//46 ���������豸-��
	0,		//47 �����Ƿ�ʱֹͣ�豸
	17,		//48 ����ֹͣ�豸-ʱ
	30,		//49 ����ֹͣ�豸-��

	0,		//50 �����Ƿ�ʱ�����豸
	8,		//51 ���������豸-ʱ
	30,		//52 ���������豸-��
	0,		//53 �����Ƿ�ʱֹͣ�豸
	17,		//54 ����ֹͣ�豸-ʱ
	30,		//55 ����ֹͣ�豸-��

	0,		//56 �����Ƿ�ʱ�����豸
	8,		//57 ���������豸-ʱ
	30,		//58 ���������豸-��
	0,		//59 �����Ƿ�ʱֹͣ�豸
	17,		//60 ����ֹͣ�豸-ʱ
	30,		//61 ����ֹͣ�豸-��

	0,		//62 �����Ƿ�ʱ�����豸
	8,		//63 ���������豸-ʱ
	30,		//64 ���������豸-��
	0,		//65 �����Ƿ�ʱֹͣ�豸
	17,		//66 ����ֹͣ�豸-ʱ
	30,		//67 ����ֹͣ�豸-��

	0,		//68 �����Ƿ�ʱ�����豸
	8,		//69 ���������豸-ʱ
	30,		//70 ���������豸-��
	0,		//71 �����Ƿ�ʱֹͣ�豸
	17,		//72 ����ֹͣ�豸-ʱ
	30,		//73 ����ֹͣ�豸-��
	0,      //74 ������ѹ���� �������
	0,      //75 ������������ �������
	0,      //76 ���ص������� �������
	0,      //77 ������ѹƵ�� �������
	0,      //78 ��������Ƶ�� �������
	0,      //79 ���ص���Ƶ�� �������
	0,      //80 ��������������Χ
	10,     //81 ����������������
	0,      //82 ���ع���������Χ
	10,     //83 ���ع�����������
	100,    //84 ������ѹTHD��Χ
	10,     //85 ������ѹTHD����
	100,    //86 ��������THD��Χ
	10,     //87 ��������THD����
	100,    //88 ���ص���THD��Χ
	10,     //89 ���ص���THD����
	10,     //90 ���������������
	10,     //91 ���������������
	5,      //92 ģ����ʾ����С����
	10,     //93 �����ܲ�������
	100,     //94 �����ܲ�����Χ
	10,      //95 ���β�������
	10,      //96 ��β�������
	10,      //97 �ߴβ�������
	10,      //98 �Ŵβ�������

	10,      //99  ʮһ�β�������  950
	10,      //100 ʮ���β�������  951
	10,      //101 ʮ��β�������  952
	10,      //102 ʮ�ߴβ�������  953
	0,       //103  ����״̬0��������㲻���� 954
	0,       //104 �Ƿ� ����г������    955
	0,       //105 �Ƿ� ��ʱ����        956
	1,       //106 �Ƿ� ��ʾ Ƶ��    957
	1,       //107 �Ƿ���ʾֱ�����ѹ   958
	2,       //108 �Ƿ���ʾһ���¶�     959
	0,       //109 �Ƿ���ʾ˫ֱ����ѹ   960
	0,       //110 �Ƿ���ʾ�������     961
	0,	     //111 ���������ʱ�� - ��  962
	0,	     //112 ���������ʱ�� - ��  963
	0,	     //113 ������������ �������964
	0,	     //114 �ֶ�����ֹͣ�����Ƿ�ʹ�� 965
	400,	 //115  ͬ���ռ�ָ������         966
	0,		 //116 �Ƿ���ʾ�����Ĳ�������    967
	0,	 	 //117 modbus ������ h			 968
	9600,	 //118 modbus ������ l			 969
	2,		 //119 modbus У��			     970
	000,    //120 �������ĺ���1~4              971
	 000,    //121 �������ĺ���5~8              972
	000,     //122 �������ĺ���9~11             973
	0,	 //123 �Ƿ���ʾ����������            974
	1,      //124 ����������Ϣʹ��      975
        0xf7f5, //125 1��16������Ϣ����     976
	0x7fff, //126 17��32������Ϣ����    977
	0xfff7, //127 33��48������Ϣ����    978
        0xffff, //128 49��64������Ϣ����    979
        0xfffd, //129 65��80������Ϣ����    980
        0xffff, //130 81��96������Ϣ����    981
        //GSM��������
        1,      //131 ����������Ϣʹ��      982
        0xf7f5, //132 1��16������Ϣ����     983
	0x7fdf, //133 17��32������Ϣ����    984
	0x17f7, //134 33��48������Ϣ����    985
        0xffff, //135 49��64������Ϣ����    986
        0xffff, //136 65��80������Ϣ����    987
        0xffff, //137 81��96������Ϣ����    988
};
	
	if(type==0)//��������
	{	
		for(i=0;i<=130;i++) P_E2PROM[i]=P_E2PROM_b[i];
		
		E2ROM_WR(1,98,P_E2PROM+1);//ϵͳ���ó�ʼ��
		E2ROM_WR(950,39,P_E2PROM+99);//ϵͳ���ó�ʼ��
	}
	else
	if(type==1)//�������
	{
		for(i=1;i<=2;i++)
			P_E2PROM[i]=P_E2PROM_b[i];

		E2ROM_WR(1,2,P_E2PROM+1);//д��E2
		
		message_num=2;
	}
}	
