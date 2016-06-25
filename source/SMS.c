#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"
#if compileGSM
//******���͵Ļ�����********************************
//	char SOURCE[176]="0891683108200505F011000D9168 51 99 46 26 99 F3 00000048";//48  +8613800250500
//	char SOURCE[176]="0891683108200505F011000D91685152712704F500000048";//48��غ���
	volatile unsigned char __attribute__((__far__)) SOURCE[370]="0891683108200505F011000D91685152712704F500020055";//48��غ���
	volatile unsigned char __attribute__((__far__)) SOURCE_data[200];//����
	volatile unsigned char  SEND_CMGS;//һ���������� CMGF
	
	volatile unsigned char __attribute__((__far__)) SOURCE_R_num[40];//���ն������ĺ���
	volatile unsigned int  R232_receive_Ndata_now=0;//���ն������ĺ���
	
	volatile unsigned char __attribute__((__far__)) SOURCE_R[370];//�������ݼ�غ���
	volatile unsigned char __attribute__((__far__)) SOURCE_data_R[160];//����
	volatile unsigned char  SEND_CMGS_R;
	
	volatile unsigned int GSM_send_num=0;//���ڿ���GSM��������
	volatile unsigned int GSM_record_sign=0;//�����жϷ��͵���������
	
	volatile unsigned char __attribute__((__far__)) R232_send_data[1000];//���ͻ�����
	volatile unsigned int R232_send_data_num=0;
	volatile unsigned int R232_send_data_now=0;
	volatile unsigned int R232_Send_sign=0;//0:���Է������ݣ�1�������ݷ��ͣ�2:
	
	volatile unsigned int R232_receive_data_num=0;
	volatile unsigned int R232_receive_data_now=0;
	volatile unsigned int R232_Rbuffer_sign=0;  //0:û���յ����ݣ�1�������˽����������2���������
	
	volatile unsigned int send_message_time;
	volatile unsigned int return_run_sign=0;
	
int char_decimal(volatile unsigned int point,volatile unsigned char *char_data)
{
	unsigned int all_num,fist_num,i,j,k,fu,dian; 
	unsigned int data[8];
	int return_data=0;
	
	k=0;
	while(point!=1)
	{
		point/=10;
		k++;	
	}	
	point=k;//��ȷ��0��λ 1ʮλ
	
	i=0;j=0;fu=0;dian=0;fist_num=0;
	while(char_data[i]=='-'||char_data[i]=='.'||(char_data[i]>='0'&&char_data[i]<='9'))
	{
		data[j++]=char_data[i];

		if(char_data[i]=='-') 
		{
			if(i!=0)  
			{
				return_data=0x7fff;
				break;
			}
			else
			{
				fu++;
				j--;
			}	
		}
		else
		if(char_data[i]=='.') 
		{
			dian++;			
			fist_num=j-1;
			j--;
			if(dian>1)
			{
				return_data=0x7fff;
				break;
			}	
		}
		
		i++;
	}
	
	if(return_data!=0x7fff)
	{
		if(fist_num==0) fist_num=j;

		fist_num=j-fist_num;
		
		if(point>fist_num)
		{
			for(k=0;k<point-fist_num;k++)
			{
				data[j++]='0';	
			}	
		}
		else
		if(point<fist_num)
		{
			for(k=0;k<fist_num-point;k++)
				j--;	
		}	
		
		all_num=1;
		return_data=0;
		for(k=0;k<j;k++)
		{
			return_data+=((data[j-k-1]&0xff)-'0')*all_num;			
			all_num*=10;	
		}	
		
		if(fu!=0)
		{
			return_data*=-1;			
		}
	}	

	return return_data;
}	
	
unsigned int LCD6448_edit_mode(unsigned int num,unsigned int fist_data,unsigned int second_data)
{
	unsigned int i,j,k=0;
		
	for(i=0;i<4;i++)//ǰ��С����
	{	
		j=fist_data>>12;
		fist_data<<=4;		
		if(j!=0||k!=0)
		{
			if(j>9) SOURCE_data[num++]=j+0x37;
			else    SOURCE_data[num++]=j+0x30;
			k=1;//�´ν���
		}		
		
		if(i==1)//С����
		{
			if(k==0)//���С������ǰΪ�㡢
			{
				if(j>9) SOURCE_data[num++]=j+0x37;
			    else    SOURCE_data[num++]=j+0x30;
				k=1;//�´ν���
			}
						
			SOURCE_data[num++]='.';
		}		
	}
	
	k=0;
	for(i=0;i<4;i++)//ĩβ�ж��ٸ���
	{
		if(((second_data>>(i<<2))&0x000f)!=0) break;
		else k++;
	}	
	
	for(i=0;i<4-k;i++)//���λ
	{	
		j=second_data>>12;
		second_data<<=4;		
		if(j>9) SOURCE_data[num++]=j+0x37;
		else    SOURCE_data[num++]=j+0x30;		
	}
	
	return	num;
}

unsigned int hex_char(unsigned int fist_num,unsigned char hex_data)
{//���ֱ�ʮ��λ�ַ�
	unsigned int i;
	
	i=hex_data>>4;
	if(i<10) SOURCE[fist_num++]=i+'0';
	else     SOURCE[fist_num++]=i-10+'A';
	
	i=hex_data&0x0f;
	if(i<10) SOURCE[fist_num++]=i+'0';
	else     SOURCE[fist_num++]=i-10+'A';

	return fist_num;
}

unsigned char char_hex(unsigned char D_high,unsigned char D_low)
{//�ַ�������
	if(D_high<58) D_high-='0';
	else          D_high-='A'-10;	
	if(D_low<58) D_low-='0';
	else          D_low-='A'-10;
	return (D_high<<4)|D_low;	
}	

unsigned int char_int(unsigned int num,volatile unsigned char *character)
{
	unsigned int i,j=0;
	
	for(i=0;i<num;i++)
	{
		j*=10;
		j+=character[i]-'0';	
	}
	return j;
}	
	
unsigned int add_data_string(unsigned int fist_num,unsigned int record,unsigned int point)
{//record����Ҫ��������ݣ�point��1:0λ��10��һλ��
	unsigned int h;
	unsigned int i=0,m=10000,k=0,n;
	
	if(fist_num<160&&point!=0)//�жϷ�Χ
	{	
		SOURCE_data[fist_num++]='=';
		h=fist_num;	
		while(m!=0)
		{
				if(i==0) i=point/m; //�Ƿ���Ҫ��ǰ����
				n=record/m;  //���λ
				record=record%m;  //	
				
				if((n!=0)||(k!=0)||(i!=0))		//������λ��Ϊ��
				{ 
					if(k==0)  k=1;
					SOURCE_data[fist_num++]=n+'0';       //�����ݼ�������
					if((m==point)&&(point!=1)) SOURCE_data[fist_num++]='.';//����С����
				}
				m/=10;  //
		}
		if(h==fist_num)  SOURCE_data[fist_num++]='0';
	}
	return fist_num;
}

unsigned int add_INTdata_string(unsigned int fist_num,int record,unsigned int point)
{//record����Ҫ��������ݣ�point��1:0λ��10��һλ��
	unsigned int h;
	unsigned int i=0,m=10000,k=0,n;

	if(fist_num<160&&point!=0)//�жϷ�Χ
	{	
		SOURCE_data[fist_num++]='=';
		
		if(record<0){SOURCE_data[fist_num++]='-';record=-record;}//����и���

		h=fist_num;	
		while(m!=0)
		{
				if(i==0) i=point/m; //�Ƿ���Ҫ��ǰ����
				n=record/m;  //���λ
				record=record%m;  //	
				
				if((n!=0)||(k!=0)||(i!=0))		//������λ��Ϊ��
				{ 
					if(k==0)  k=1;
					SOURCE_data[fist_num++]=n+'0';       //�����ݼ�������
					if((m==point)&&(point!=1)) SOURCE_data[fist_num++]='.';//����С����
				}
				m/=10;  //
		}
		if(h==fist_num)  SOURCE_data[fist_num++]='0';
	}
	return fist_num;
}


unsigned int add_num_string(unsigned int fist_num,unsigned int record,unsigned int num)
{//record����Ҫ��������ݣ�point��1:0λ��10��һλ��
	unsigned int i=0,m=10000,k=0,n;
		
	if(fist_num<160)//�жϷ�Χ
	{	
		while(m!=0)
		{
				if(i==0) i=num/m; //�Ƿ���Ҫ��ǰ����
				n=record/m;  //���λ
				record=record%m;  //	
				
				if((n!=0)||(k!=0)||(i!=0))		//������λ��Ϊ��
				{ 
					if(k==0)  k=1;
					SOURCE_data[fist_num++]=n+'0';       //�����ݼ�������
				}
				m/=10;  //
		}
	}
	return fist_num;
}

void add_num(void)//��E2�еĺ��룬���ص���Ϣè��
{
	unsigned int i,h1,h2,h3,h4;
	unsigned char __attribute__((__far__)) SOURCE_buffer[]="0891683108200505F011000D91685152712704F500020055";
			
	for(i=0;i<48;i++) SOURCE[i]=SOURCE_buffer[i];//��ֹ���

	h1=P_E2PROM[10]/1000;
	h2=(P_E2PROM[10]/100)%10;
	h3=(P_E2PROM[10]/10)%10;
	h4=P_E2PROM[10]%10;
	SOURCE[28]='0'+h2;
	SOURCE[29]='0'+h1;
	SOURCE[30]='0'+h4;
	SOURCE[31]='0'+h3;
	
	h1=P_E2PROM[11]/1000;
	h2=(P_E2PROM[11]/100)%10;
	h3=(P_E2PROM[11]/10)%10;
	h4=P_E2PROM[11]%10;
	SOURCE[32]='0'+h2;
	SOURCE[33]='0'+h1;
	SOURCE[34]='0'+h4;
	SOURCE[35]='0'+h3;
	
	h1=(P_E2PROM[12]/100)%10;
	h2=(P_E2PROM[12]/10)%10;
	h3=P_E2PROM[12]%10;
	SOURCE[36]='0'+h2;
	SOURCE[37]='0'+h1;
	SOURCE[39]='0'+h3;
	
	//************************************		
	h1=P_E2PROM[120]/1000;
	h2=(P_E2PROM[120]/100)%10;
	h3=(P_E2PROM[120]/10)%10;
	h4=P_E2PROM[120]%10;
	SOURCE[6]='0'+h2;
	SOURCE[7]='0'+h1;
	SOURCE[8]='0'+h4;
	SOURCE[9]='0'+h3;
	
	h1=P_E2PROM[121]/1000;
	h2=(P_E2PROM[121]/100)%10;
	h3=(P_E2PROM[121]/10)%10;
	h4=P_E2PROM[121]%10;
	SOURCE[10]='0'+h2;
	SOURCE[11]='0'+h1;
	SOURCE[12]='0'+h4;
	SOURCE[13]='0'+h3;
	
	h1=(P_E2PROM[122]/100)%10;
	h2=(P_E2PROM[122]/10)%10;
	h3=P_E2PROM[122]%10;
	SOURCE[14]='0'+h2;
	SOURCE[15]='0'+h1;
	SOURCE[17]='0'+h3;
}	


void send_message(void)
{
//	unsigned int error_buf=0;
	unsigned int num=0;
	
	add_num();//�ı�绰����
	
	  num=add_data_string(num,101,1);//101
	  
		num=add_data_string(num,5,1);   //�������

			SOURCE_data[num++]='=';
			SOURCE_data[num++]='6';
			SOURCE_data[num++]='6';
			SOURCE_data[num++]='6';
			SOURCE_data[num++]='6';
			SOURCE_data[num++]='6';
			SOURCE_data[num++]='6';
			SOURCE_data[num++]='6';
			SOURCE_data[num++]=equipment_skate[0][2]+'0';//�豸״̬	
	
	
	num=add_data_string(num,equipment_skate[0][3]&0x0f,1);//����״̬				
	num=add_data_string(num,equipment_skate[0][1],1);//���ϴ���
	equipment_skate[0][1]=0;//���ϴ��룻
	
	num=add_data_string(num,WIRE_cost[0][0],1);
	num=add_data_string(num,WIRE_cost[0][1],1);
	num=add_data_string(num,WIRE_cost[0][2],1);//������ѹ
	
	num=add_data_string(num,WIRE_cost[1][0],1);
	num=add_data_string(num,WIRE_cost[1][1],1);
	num=add_data_string(num,WIRE_cost[1][2],1);//��������
	
	num=add_data_string(num,WIRE_cost[2][0],1);
	num=add_data_string(num,WIRE_cost[2][1],1);
	num=add_data_string(num,WIRE_cost[2][2],1);//���ص���
	
	num=add_data_string(num,WIRE_cost[3][0],1);
	num=add_data_string(num,WIRE_cost[3][1],1);
	num=add_data_string(num,WIRE_cost[3][2],1);//��������
	
	num=add_data_string(num,LOAD_cost[0][0],10);//Ƶ��
	
	num=add_data_string(num,LOAD_cost[1][0],100);
	num=add_data_string(num,LOAD_cost[1][1],100);
	num=add_data_string(num,LOAD_cost[1][2],100);//������������
	
	num=add_data_string(num,LOAD_cost[2][0],100);
	num=add_data_string(num,LOAD_cost[2][1],100);
	num=add_data_string(num,LOAD_cost[2][2],100);//���ع�������
	
	num=add_data_string(num,LOAD_cost[4][0],10);
	num=add_data_string(num,LOAD_cost[4][1],10);
	num=add_data_string(num,LOAD_cost[4][2],10);//��������������
	
	num=add_data_string(num,LOAD_cost[5][0],10);
	num=add_data_string(num,LOAD_cost[5][1],10);
	num=add_data_string(num,LOAD_cost[5][2],10);//���ص���������
	
	num=add_data_string(num,LOAD_cost[6][0],1);//ֱ����ѹ
	num=add_data_string(num,LOAD_cost[6][1],1);//����ģ���¶�
	num=add_data_string(num,WIRE_cost[6][0],1);//�����������
	num=add_data_string(num,WIRE_cost[6][1],1);//�����������
	
	if(num>=160) num=160;//��󲻳���160���ַ�
	
	hex_char(46,num);//Ҫ��ʾ�ַ��ĸ���

	SEND_CMGS=GSMEncode7bit(SOURCE+48,SOURCE_data,num);
	num=SEND_CMGS*2+48;//һ����ӵĶ�������
	SEND_CMGS+=15;//һ������������
	
	SOURCE[num++]='\0';
	SOURCE[num++]='\0';

	GSM_record_sign=0x0100;//����һ����Ϣ״̬
}

/*7-bit����  pSrc:Դ�ַ���ָ��  pDst:Ŀ����봮ָ��  nSrcLength:Դ�ַ�������  ����:Ŀ����봮����*/ 
int GSMEncode7bit(volatile unsigned char *pDst,volatile unsigned char *pSrc,int nSrcLength)
{//���룬pStr:Դ�ַ�����pDst:Ŀ���ַ���������Ŀ���ַ����ĳ���;
	int nSrc;
	int nDst; 
	int nChar; 
	unsigned char nLeft;
	unsigned char tran_data,i;

    nSrc=0;
	nDst=0;
	
	while(nSrc<nSrcLength)
	{
		nChar=nSrc & 7;

		if(nChar==0)
		{
			nLeft=*pSrc;
		}
		else
		{
			tran_data=(*pSrc<<(8-nChar))|nLeft;			
				i=tran_data>>4;	
				if(i<10) *(pDst++)=i+'0';
				else     *(pDst++)=i-10+'a';
				i=tran_data&0x0f;
				if(i<10) *(pDst++)=i+'0';
				else     *(pDst++)=i-10+'a';				
			nLeft=*pSrc>>nChar;
			nDst++;
		}	   
		pSrc++;
		nSrc++;
	}
	if((nSrc&7)!=0)
	{	
				i=nLeft>>4;	
				if(i<10) *(pDst++)=i+'0';
				else     *(pDst++)=i-10+'a';
				i=nLeft&0x0f;
				if(i<10) *(pDst++)=i+'0';
				else     *(pDst++)=i-10+'a';	
				nDst++;
	}
		return nDst;	
}

/* 7-bit���� pSrc:Դ���봮ָ�� pDst:Ŀ���ַ���ָ��  nSrcLength:Դ���봮����  ����:Ŀ���ַ�������*/ 
int GSMDecode7bit(volatile unsigned char* pSrc,volatile unsigned char* pDst)
{  
    int nDst;        
    int nByte;       
    unsigned char nLeft;
    unsigned char tran_data;
    nDst = 0;   
    nByte = 0; 
    nLeft = 0;     
    while(*pSrc!=0x0d)
    { 
        tran_data=char_hex(*pSrc,*(pSrc+1));
        *pDst = ((tran_data << nByte) | nLeft) & 0x7f;  
        nLeft = tran_data >> (7-nByte);    
        pDst++; 
        nDst++;   
        nByte++; 
        if(nByte == 7) 
        { 
            *pDst = nLeft;
            pDst++;
            nDst++;
            nByte = 0;
            nLeft = 0;
        }     
        pSrc+=2;
    }    
    *pDst = 0;  
    return nDst; 
}

void GSM_add_record(unsigned int num,const unsigned char *character)
{//�������ݣ�׼������
	unsigned int i;
	R232_send_data_num=0;
	R232_send_data_now=0;
	for(i=0;i<num;i++)
		R232_send_data[R232_send_data_num++]=character[i];
	R232_send_data[R232_send_data_num]='\0';
	
	
}

void GSM_handle(void)//GSM�������
{
	unsigned int i,j,k,m,n,num,equal,num_data,x,z,temp1,temp2;
	int parameter_sms[9];
	
	if(GSM_record_sign==0&&R232_Send_sign==1&&R232_Rbuffer_sign==0)//������û�з����ݣ�û�н������ݵ�ʱ��û��׼���������ݵ�ʱ��
	{	
		DELAY_GSM_error=0;//�������
		
		if((TIME_NOW[4]==0x0008||TIME_NOW[4]==0x000E||TIME_NOW[4]==0x0014)&&P_E2PROM[25]%60==TIME_NOW[5]&&send_message_time==0)
		{
			send_message();
			send_message_time++;
		}
		else
		if(return_run_sign==0xaaaa)
		 {
			 send_message();//����ֹͣ����Ļظ�
			 
			 return_run_sign=0;
                 }
                if((TIME_NOW[4]==0x0008||TIME_NOW[4]==0x000E||TIME_NOW[4]==0x0014)&&(P_E2PROM[25]+1)%60==TIME_NOW[5])
                {
                    send_message_time=0;
                }
		
	 }
	 else
	 if(GSM_record_sign==0&&R232_Rbuffer_sign==0)
	 {
	 	if(R232_Send_sign>=102&&R232_Send_sign<=111)
		{
			if((parameter_receive_SMSsign+1)%3==0)//���һ�����ݷ������
			{			
					 for(i=0;i<9;i++)//���յ������ݸ�������
					 protect_parameter_b[i]=hide_parameter_b[i/3][(i%3)+1];	

					 if(which_equ==2)	 i=0;//3.5 80APF
					else if(which_equ==1) i=1;//3.5 ��80
					else if(which_equ==3) i=2;//3.5 SVG
					else if(which_equ==4) i=3;//3.5 SVG-S-A
	
					 num=0;
					 add_num();//�ı�绰����	
					 
					 if(R232_Send_sign>=106) n=R232_Send_sign+2;
					 else 					 n=R232_Send_sign;

					 num=add_data_string(num,n,1);//1	
					 	 
					 for(j=0;j<9;j++)
					 {
						 num=add_INTdata_string(num,protect_parameter_b[j],PARAMETER[i][R232_Send_sign-102][j]);//	 
					 } 
					 
					 num=add_data_string(num,1,1);
					 num=add_data_string(num,which_equ,1);
								
					  hex_char(46,num);//Ҫ��ʾ�ַ��ĸ���
					  SEND_CMGS=GSMEncode7bit(SOURCE+48,SOURCE_data,num);
					  num=SEND_CMGS*2+48;//һ����ӵĶ�������
					  SEND_CMGS+=15;//һ������������	
					  SOURCE[num++]='\0';
					  SOURCE[num++]='\0';
							
					  GSM_record_sign=0x0100;//����һ����Ϣ״̬
					  parameter_receive_SMSsign=0;
					  
					  R232_Send_sign=1;
			  }		
		 }
	 }
	 else
	 if(DELAY_GSM_error>=300000)//���30S  ��λ����è����
	 {	 
		 GSM_record_sign=0;
		 R232_Send_sign=1;
		 R232_Rbuffer_sign=0;
		 DELAY_GSM=0;
		 R232_send_data_now=0;
		 R232_send_data_num=0;
		 R232_receive_data_now=0;	 
	 }


//	if(R232_Rbuffer_sign==0&&(GSM_record_sign>>8)==0x01)//���û���յ�����,���ҵ�ǰû�����з�������
	if(R232_Rbuffer_sign==0)//û�����ݽ��մ���
	{		
		if(DELAY_GSM>=40000||((GSM_record_sign&0xff)!=0x00&&DELAY_GSM>=12000))//1.2S
		 {
			DELAY_GSM=0;//������
			
			if((GSM_record_sign&0xff)==0x00)
			{
				GSM_add_record(3,AT);//����AT
				GSM_record_sign++;
				R232_Send_sign=0;//��ʼ����
			}
			else
			if((GSM_record_sign&0xff)==0x01)
			{
				if(GSM_record_sign>>8==0x01)//��������
				{
					GSM_add_record(10,AT_CMGF);//����AT_CMGF
					GSM_record_sign++;
				}
				else
				if(GSM_record_sign>>8==0x02)//ɾ������
				{
					GSM_add_record(10,AT_CMGD);//ɾ������
					GSM_record_sign=0;	//��ʾ���ͽ���
					R232_Rbuffer_sign=2;//ɾ��֮�����֡����
				}
				else
				if(GSM_record_sign>>8==0x03)//ɾ������
				{
					GSM_add_record(10,AT_CMGD);//ɾ������
					GSM_record_sign=0;	//��ʾ���ͽ���
					R232_Rbuffer_sign=1;//ɾ��֮�������������
				}
				else//Ҫ����
				{
					if(DELAY_GSM_NUM<200000)//20s��ѯ��
					{
						GSM_add_record(10,AT_CMGR);//������
						GSM_record_sign=0;	//��ʾ���ͽ���
						R232_Rbuffer_sign=1;//��ʾ���Խ���������
                                         }
					else
                                        if(DELAY_GSM_NUM<300000)//30s��ɾ��      //����Ԫ-����***********************************
					{
						GSM_add_record(10,AT_CMGD);//ɾ������
						GSM_record_sign=0;	//��ʾ���ͽ���
						R232_Rbuffer_sign=1;//ɾ��֮�������������

						DELAY_GSM_NUM = 400000;//��ʼ�ʶ�������
					}
					else
					{
						GSM_add_record(9,AT_CSCA);//����AT+CSCA
						GSM_record_sign=0;	//��ʾ���ͽ���
						R232_Rbuffer_sign=1;//��ʾ���Խ���������
							
						DELAY_GSM_NUM=0;
						GSM_NUM_fist=1;  //��գ���Ҳ��ѯ��						
					}	
				}
				R232_Send_sign=0;//��ʼ����
			}
			else
			if((GSM_record_sign&0xff)==0x02)
			{
				GSM_add_record(8,AT_CMGS);//����AT_CMGS
				R232_send_data[R232_send_data_num++]=SEND_CMGS/100+'0';
				R232_send_data[R232_send_data_num++]=(SEND_CMGS/10%10)+'0';
				R232_send_data[R232_send_data_num++]=(SEND_CMGS%10)+'0';
				R232_send_data[R232_send_data_num++]='\r';
				GSM_record_sign++;
				R232_Send_sign=0;//��ʼ����
			}
			else
			if((GSM_record_sign&0xff)==0x03)//����������
			{
					R232_send_data_num=0;
					R232_send_data_now=0;
				while(SOURCE[R232_send_data_num]!='\0')
					R232_send_data[R232_send_data_num++]=SOURCE[R232_send_data_num];
				GSM_record_sign++;
				R232_Send_sign=0;//��ʼ����
			}
			else
			if((GSM_record_sign&0xff)==0x04)
			{
				U1TXREG=0X1A;
				R232_Send_sign=0;//һ�η��ͽ���
				R232_Send_sign=0;//��ʼ����
                                GSM_record_sign=0x0300;//ɾ��������Ҫ�Ķ���    //����Ԫ-����***********************************
			}			
		 }//if(DELAY_GSM>=14000)//1.2S
	}
	else
	if(R232_Rbuffer_sign==1)//�����յ�������
	{
		if(DELAY_GSM>=50000)//5s�����û�лḴ�������½���
				R232_Rbuffer_sign=0;		
	}	
	else
	if(R232_Rbuffer_sign==2)//�����յ�������
	{
		if(R232_Send_sign!=0xaaaa)//�������ظ�
		{
			num_data=GSMDecode7bit(SOURCE_R+58,SOURCE_data_R);
			R232_Send_sign=0;
		}
		else
		{
			num_data=7;
		}	
		
		equal=1; n=0;j=0;
		for(i=0;i<num_data;i++)
		{
			if(SOURCE_data_R[i]=='=')
			{
				j++;
				if(j==1)//��һ���Ⱥ�
				{
					equal=equipment_ID[0][1];

					if(SOURCE_data_R[i+1]=='3')//�ٻ�
					{
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='1')//�ٻ�������
						{
							send_message();//���͵�����
						}
						else
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='2')
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0101,equal,0,0);//
							C1TR01CONbits.TXREQ1=1;
							R232_Send_sign=102;
						}
						else
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='3')
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0102,equal,0,0);//
							C1TR01CONbits.TXREQ1=1;
							R232_Send_sign=103;
						}
						else
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='4')
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0103,equal,0,0);//
							C1TR01CONbits.TXREQ1=1;
							R232_Send_sign=104;
						}
						else
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='5')
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0104,equal,0,0);//
							C1TR01CONbits.TXREQ1=1;
							R232_Send_sign=105;
						}
						else
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='8')
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0105,equal,0,0);//
							C1TR01CONbits.TXREQ1=1;
							R232_Send_sign=106;
						}
						else
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='9')
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0106,equal,0,0);//
							C1TR01CONbits.TXREQ1=1;
							R232_Send_sign=107;
						}
						else
						if(SOURCE_data_R[i+2]=='1'&&SOURCE_data_R[i+3]=='0')
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0107,equal,0,0);//
							C1TR01CONbits.TXREQ1=1;
							R232_Send_sign=108;
						}
						else
						if(SOURCE_data_R[i+2]=='1'&&SOURCE_data_R[i+3]=='1')//�ڰ���
						{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0108,equal,0,0);//
							C1TR01CONbits.TXREQ1=1;
							R232_Send_sign=109;
						}
						else
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='6')//�ٻ�������ţ��豸�汾��
						{
								num=0;
								add_num();//�ı�绰����	
								
								num=add_data_string(num,106,1);//1								
								SOURCE_data[num++]='=';
								
								num=add_num_string(num,P_E2PROM[23],10);//
								
								if(which_equ==1||which_equ==2) //0 APF
								{
									SOURCE_data[num++]='A';
									SOURCE_data[num++]='P';
									SOURCE_data[num++]='F';	
								    n=0;	
								}
								else
								if(which_equ==3) //2 SVG
								{
									SOURCE_data[num++]='S';
									SOURCE_data[num++]='V';
									SOURCE_data[num++]='G';
									n=1;
								}
								else
								if(which_equ==4) //3 SVG-S-A
								{
									SOURCE_data[num++]='S';
									SOURCE_data[num++]='V';
									SOURCE_data[num++]='G';
									SOURCE_data[num++]='-';
									SOURCE_data[num++]='S';
									SOURCE_data[num++]='-';
									SOURCE_data[num++]='A';
									n=2;
								}	
								num=add_num_string(num,P_E2PROM[24],10);//
								num=add_num_string(num,P_E2PROM[25],100);////�������
								
								SOURCE_data[num++]='=';
									
								num=LCD6448_edit_mode(num,soft_APF_SVG[0][0],soft_APF_SVG[0][1]);
								
								SOURCE_data[num++]='=';
								
								num=LCD6448_edit_mode(num,record_single[1][2],record_single[0][1]);
								
								num=add_data_string(num,1,1);  //Ĭ��IDΪ1
								
								hex_char(46,num);//Ҫ��ʾ�ַ��ĸ���
								SEND_CMGS=GSMEncode7bit(SOURCE+48,SOURCE_data,num);
								num=SEND_CMGS*2+48;//һ����ӵĶ�������
								SEND_CMGS+=15;//һ������������
								
								SOURCE[num++]='\0';
								SOURCE[num++]='\0';
							
								GSM_record_sign=0x0100;//����һ����Ϣ״̬
						}
						else if(SOURCE_data_R[i+2]=='1'&&SOURCE_data_R[i+3]=='3')
                                                {
                                                    num=0;
                                                    add_num();//�ı�绰����
                                                    num=add_data_string(num,113,1);//Ͷ�п�������
                                                    SOURCE_data[num++]='=';
                                                    for(x=0;x<6;x++)
                                                    {
                                                        for(z=0;z<16;z++)
                                                        {
                                                            SOURCE_data[num++]=((P_E2PROM[125+x]>>z)&0x0001)+'0';

                                                        }
                                                        SOURCE_data[num++]=',';
                                                    }
                                                    hex_char(46,num);//Ҫ��ʾ�ַ��ĸ���
                                                    SEND_CMGS=GSMEncode7bit(SOURCE+48,SOURCE_data,num);
                                                    num=SEND_CMGS*2+48;//һ����ӵĶ�������
                                                    SEND_CMGS+=15;//һ������������
                                                    SOURCE[num++]='\0';
                                                    SOURCE[num++]='\0';
                                                    GSM_record_sign=0x0100;//����һ����Ϣ״̬
                                                }
                                                else if(SOURCE_data_R[i+2]=='1'&&SOURCE_data_R[i+3]=='4')
                                                {
                                                    num=0;
                                                    add_num();//�ı�绰����
                                                    num=add_data_string(num,114,1);
                                                    SOURCE_data[num++]='=';
                                                    for(x=0;x<6;x++)
                                                    {
                                                        for(z=0;z<16;z++)
                                                        {
                                                            SOURCE_data[num++]=((P_E2PROM[132+x]>>z)&0x0001)+'0';
                                                        }
                                                        SOURCE_data[num++]=',';
                                                    }
                                                    hex_char(46,num);//Ҫ��ʾ�ַ��ĸ���
                                                    SEND_CMGS=GSMEncode7bit(SOURCE+48,SOURCE_data,num);
                                                    num=SEND_CMGS*2+48;//һ����ӵĶ�������
                                                    SEND_CMGS+=15;//һ������������
                                                    SOURCE[num++]='\0';
                                                    SOURCE[num++]='\0';
                                                    GSM_record_sign=0x0100;//����һ����Ϣ״̬
                                                }
						parameter_receive_SMSsign=0;//���½��ձ�־						
						break;
					}
					else
					if(SOURCE_data_R[i+1]=='4')//�յ����޸ĵ绰��������
					{
						if(SOURCE_data_R[i+2]=='0'&&SOURCE_data_R[i+3]=='7')//�޸ĵ绰����
						{
								num=0;
								add_num();//�ı�绰����
								SOURCE_data[num++]='=';
								SOURCE_data[num++]='2';
								SOURCE_data[num++]='0';
								SOURCE_data[num++]='7';
								SOURCE_data[num++]='=';
								for(n=0;n<11;n++)
									SOURCE_data[num++]=SOURCE_data_R[i+n+5];
								
								hex_char(46,num);//Ҫ��ʾ�ַ��ĸ���
								SEND_CMGS=GSMEncode7bit(SOURCE+48,SOURCE_data,num);
								num=SEND_CMGS*2+48;//һ����ӵĶ�������
								SEND_CMGS+=15;//һ������������
								SOURCE[num++]='\0';
								SOURCE[num++]='\0';	
								GSM_record_sign=0x0100;//����һ����Ϣ״̬
								
								P_E2PROM[10]=char_int(4,SOURCE_data_R+i+5);
								P_E2PROM[11]=char_int(4,SOURCE_data_R+i+9);
								P_E2PROM[12]=char_int(3,SOURCE_data_R+i+13);//�绰����
								
								E2ROM_WR(10,3,P_E2PROM+10);//дE2
										
								break;
						}
                                                else if(SOURCE_data_R[i+2]=='1'&&SOURCE_data_R[i+3]=='3')
                                                {
                                                    if(SOURCE_data_R[i+106]==',')
                                                    {
                                                        num=0;
                                                        add_num();//�ı�绰����
                                                        for(x=0;x<6;x++)
                                                        {
                                                            temp2=0;
                                                            for(z=0;z<16;z++)
                                                            {
                                                                temp1=SOURCE_data_R[5+z+x*17]-'0';
                                                                temp2=temp2|((temp1&0x0001)<<z);
                                                            }
                                                            P_E2PROM[125+x]=temp2;
                                                            E2ROM_WR(976+x,1,P_E2PROM+125+x);
                                                        }
                                                        SOURCE_data[num++]='=';
                                                        SOURCE_data[num++]='1';
                                                        SOURCE_data[num++]='1';
                                                        SOURCE_data[num++]='3';
                                                        SOURCE_data[num++]='=';
                                                        for(x=0;x<102;x++)
                                                        {
                                                            SOURCE_data[num++]=SOURCE_data_R[i+5+x];
                                                        }
                                                        hex_char(46,num);//Ҫ��ʾ�ַ��ĸ���
                                                        SEND_CMGS=GSMEncode7bit(SOURCE+48,SOURCE_data,num);
                                                        num=SEND_CMGS*2+48;//һ����ӵĶ�������
                                                        SEND_CMGS+=15;//һ������������
                                                        SOURCE[num++]='\0';
                                                        SOURCE[num++]='\0';
                                                        GSM_record_sign=0x0100;//����һ����Ϣ״̬
                                                        LCD_page_dis=0;
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        break;
                                                    }
                                                }
                                                else if(SOURCE_data_R[i+2]=='1'&&SOURCE_data_R[i+3]=='4')
                                                {
                                                    if(SOURCE_data_R[i+106]==',')
                                                    {
                                                        num=0;
                                                        add_num();//�ı�绰����
                                                        for(x=0;x<6;x++)
                                                        {
                                                            temp2=0;
                                                            for(z=0;z<16;z++)
                                                            {
                                                                temp1=SOURCE_data_R[5+z+x*17]-'0';
                                                                temp2=temp2|((temp1&0x0001)<<z);
                                                            }
                                                            P_E2PROM[132+x]=temp2;
                                                            E2ROM_WR(983+x,1,P_E2PROM+132+x);
                                                        }
                                                        SOURCE_data[num++]='=';
                                                        SOURCE_data[num++]='1';
                                                        SOURCE_data[num++]='1';
                                                        SOURCE_data[num++]='4';
                                                        SOURCE_data[num++]='=';
                                                        for(x=0;x<102;x++)
                                                        {
                                                            SOURCE_data[num++]=SOURCE_data_R[i+5+x];
                                                        }
                                                        hex_char(46,num);//Ҫ��ʾ�ַ��ĸ���
                                                        SEND_CMGS=GSMEncode7bit(SOURCE+48,SOURCE_data,num);
                                                        num=SEND_CMGS*2+48;//һ����ӵĶ�������
                                                        SEND_CMGS+=15;//һ������������
                                                        SOURCE[num++]='\0';
                                                        SOURCE[num++]='\0';
                                                        GSM_record_sign=0x0100;//����һ����Ϣ״̬
                                                        LCD_page_dis=0;
                                                        break;
                                                    }
                                                }
						else
						{
								k=SOURCE_data_R[i+2]-'0';
								k*=10;
								k=k+SOURCE_data_R[i+3]-'0'+400;
								
								if(k>=408) k-=2;
						}
					}
					else
					{
						break;					
					}
				}
				else
				if(j==2)//�ڶ����Ⱥ�
				{		
					 if(k==401)
					 {
								n=SOURCE_data_R[i+8]-'0';
							
								if(n==0)
                                                                {
                                                                    n=1;WRITE_fault_one(56,0);
#if compilemodbus
                                                                    Modbus_electric[0]=56;Modbus_electric[1]=1;
#endif
                                                                }
								else
								if(n==1)
                                                                {
                                                                    n=2;WRITE_fault_one(55,0);
#if compilemodbus
                                                                    Modbus_electric[0]=55;Modbus_electric[1]=1;
#endif
                                                                }
								else
								if(n==2)
                                                                {
                                                                    n=5;WRITE_fault_one(58,0);
#if compilemodbus
                                                                    Modbus_electric[0]=58;Modbus_electric[1]=1;
#endif
                                                                }
								else
								if(n==5) 
                                                                {
                                                                    n=6;WRITE_fault_one(57,0);
#if compilemodbus
                                                                    Modbus_electric[0]=57;Modbus_electric[1]=1;
#endif
                                                                }//����
								else     n=9;//��Ч����
							
								ecan1WriteTxMsgBufId(1,0x201,0,0);
								ecan1WriteTxMsgBufData(1,8,n,0,0,0);//��������
								C1TR01CONbits.TXREQ1=1;//�����ź�

								return_run_sign=0xaaaa;

						   	    break;
				 	}
				 	else
				 	{
				 			 if(which_equ==1)   		n=1;
							 else if(which_equ==2) n=0;
							 else if(which_equ==3) n=2;
							 else if(which_equ==4) n=3;	 //������豸
							 else								n=0;	
				 			
				 	}
				}
				else
				if(j<=11)
				{									
					parameter_sms[j-3]=char_decimal(PARAMETER[n][k-402][j-3],SOURCE_data_R+i+1);//�Ÿ�����

					m=(k-402)*3;
					
					if(j==11)
					{
						ecan1WriteTxMsgBufId(1,0x202,0,0);
						ecan1WriteTxMsgBufData(1,8,m|(equal<<8),parameter_sms[0],parameter_sms[1],parameter_sms[2]);
						C1TR01CONbits.TXREQ1=1;
						
						ecan1WriteTxMsgBufId(2,0x203,0,0);
						ecan1WriteTxMsgBufData(2,8,(m+1)|(equal<<8),parameter_sms[3],parameter_sms[4],parameter_sms[5]);
						C1TR23CONbits.TXREQ2=1;
						
						ecan1WriteTxMsgBufId(3,0x204,0,0);
						ecan1WriteTxMsgBufData(3,8,(m+2)|(equal<<8),parameter_sms[6],parameter_sms[7],parameter_sms[8]);
						C1TR23CONbits.TXREQ3=1;						
						
						 SOURCE_data_R[0]='=';
						 SOURCE_data_R[1]='3';
						 SOURCE_data_R[2]=SOURCE_data_R[2];
						 SOURCE_data_R[3]=SOURCE_data_R[3];
						 SOURCE_data_R[4]='=';
						 SOURCE_data_R[5]=equal+'0';	
						 SOURCE_data_R[6]='=';

						 R232_Send_sign=0xaaaa;//���ͷ����޸ĵ�����

						break;	
					}	
				}	
				
			}//if(SOURCE_data_R[i]='=')	
			
		}//for	
		
		
		R232_receive_data_now=0;//����
		R232_receive_data_num=0;//�������ݱ�ʶ����
		
		if(R232_Send_sign==0xaaaa) R232_Rbuffer_sign=2;	
		else                       R232_Rbuffer_sign=0;//���Լ���������
	
	}	
}
#endif

