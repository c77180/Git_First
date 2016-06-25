#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

#include <dsp.h>
#include <math.h>

#define BLOCK_LENGTH   64        //һ�����ٸ���
#define LOG2_BLOCK_LENGTH    6  


volatile fractional FFTMagnitude[BLOCK_LENGTH/2+1];  	//�������
volatile float THD[BLOCK_LENGTH/2];		//����г���İٷֱ� 

volatile unsigned int coordinate_now=0;//��ǰ����,���ã�ÿ����ʾ�µ����궼��Ҫ����
volatile int zero_wave[3][64];

const fractcomplex twiddleFactors[] __attribute__ ((space(auto_psv), aligned(BLOCK_LENGTH*2)))=  //32*2
      {
        0x7FFF, 0x0000, 0x7F62, 0xF374, 0x7D8A, 0xE707, 0x7A7D, 0xDAD8,//�������ӣ����ڼ���fft
        0x7642, 0xCF04, 0x70E3, 0xC3A9, 0x6A6E, 0xB8E3, 0x62F2, 0xAECC,
        0x5A82, 0xA57E, 0x5134, 0x9D0E, 0x471D, 0x9592, 0x3C57, 0x8F1D,
        0x30FC, 0x89BE, 0x2528, 0x8583, 0x18F9, 0x8276, 0x0C8C, 0x809E,
        0x0000, 0x8000, 0xF374, 0x809E, 0xE707, 0x8276, 0xDAD8, 0x8583,
        0xCF04, 0x89BE, 0xC3A9, 0x8F1D, 0xB8E3, 0x9592, 0xAECC, 0x9D0E,
        0xA57D, 0xA57D, 0x9D0E, 0xAECC, 0x9592, 0xB8E3, 0x8F1D, 0xC3A9,
        0x89BE, 0xCF04, 0x8583, 0xDAD8, 0x8276, 0xE707, 0x809E, 0xF374,
      };
 
volatile int FFTPreProcessCnt=0;      
volatile fractcomplex FFTData[BLOCK_LENGTH] _YBSS(BLOCK_LENGTH*4);
	 volatile int temp_fft_data_max, temp_fft_data_min;//�ŵ����淽����ʾ����

unsigned int Optimization_up(unsigned int data,unsigned int range,unsigned int scale,unsigned int limit)
{//����  ��Χ������  ���ֵ   �����ݿ��ư��� range ��limit ֮��
	unsigned long return_data;
	return_data=data;
	return_data=return_data*scale/10;

		while(return_data<range)
			return_data+=10;
			
		if(return_data>limit) return_data=limit;
		
	return 	return_data;	
}

unsigned int Optimization_down(unsigned int data,unsigned int range,unsigned int scale,unsigned int limit)
{//����  ��Χ������ up_down:   �����ݿ�����limit�� range֮��
	unsigned long return_data;
	return_data=data;
	return_data=return_data*scale/10;

		while(return_data>range)
			return_data-=10;
			
		if(return_data<limit) return_data=limit;
	
	return 	return_data;
}


void find_max(volatile int *data)//�ҳ����ֵ
{
		unsigned char k;
		
		temp_fft_data_max = data[0];
		temp_fft_data_min = data[0];

		//�����ֵ��Сֵ
 		for (k=0; k<BLOCK_LENGTH; k++)
 		{   
		   	if(data[k] > temp_fft_data_max)
				temp_fft_data_max = data[k]; 
		   	else if(data[k] < temp_fft_data_min)
				temp_fft_data_min = data[k]; 
		} 
		//��������ֵ
		temp_fft_data_min = -temp_fft_data_min;
		if (temp_fft_data_max < temp_fft_data_min)	
			temp_fft_data_max = temp_fft_data_min;
		
		temp_fft_data_min=temp_fft_data_max;//���һ����
}	

void find_max_plus(volatile int *data)//32���������ҵ����ֵ
{
	unsigned char k;
		temp_fft_data_min = data[0];
	
		for (k=0; k<32; k++)
 		{   
		   	if(data[k]>temp_fft_data_min)
		   			temp_fft_data_min=data[k];
		} 	
}	


void fft_calculation (volatile int *data)
{ 
	 unsigned char k;

		find_max(data);
		FFTPreProcessCnt=0;	

		if(temp_fft_data_max!=0)
		{
			//���������������С
			if(temp_fft_data_max > 16383)	//if the maximum absolute value great than 0.5
				FFTPreProcessCnt = -1;		//�������0.5
			
			else if(temp_fft_data_max < 8192)//���С��0.25
			{
					while(temp_fft_data_max < 8192)
					{
						temp_fft_data_max = temp_fft_data_max << 1;//������0.25��0.5֮��
						FFTPreProcessCnt++;
					}
			}
		}
		
		  for (k=0; k<BLOCK_LENGTH; k++)//��λ����ʼ��
		  {    
			if(FFTPreProcessCnt < 0)
		    	FFTData[k].real = data[k] >> 1;
			else
		    	FFTData[k].real = data[k] << FFTPreProcessCnt;
		    	FFTData[k].imag = 0; 
	 	  }   
 
 	   
	  FFTComplexIP (LOG2_BLOCK_LENGTH, &FFTData[0], (fractcomplex *) __builtin_psvoffset(&twiddleFactors[0]), (int)__builtin_psvpage(&twiddleFactors[0]));   
	  BitReverseComplex (LOG2_BLOCK_LENGTH, &FFTData[0]);
	  ComputeMagnitude (&FFTData[0], &FFTMagnitude[0], 32);//��ֵ����
}

void fft_effective_all(void)//���������Чֵ�ͻ�����
{//FFTMagnitude �Ÿ�����Чֵ 32������Чֵ  THD�Ÿ��λ����ʣ�1���ܻ�����
	unsigned int i,j,k,m,error_data;
	unsigned long all_data=0;
	
	m=FFTMagnitude[1];		  //����һ��г��
	all_data=0;//ȥ��ֱ������
	FFTMagnitude[0]=0; //ȥ��ֱ������
		
	if(FFTPreProcessCnt<6) {k=0;j=6-FFTPreProcessCnt;error_data=0;}
	else				   {k=1;j=FFTPreProcessCnt-6;error_data=(2<<j)<<j;}//����Ǳ�С
 
	for(i=0;i<32;i++)
	{
              if(i>=9)
              {
                  if(FFTMagnitude[i] < error_data)  FFTMagnitude[i] = 0;
                  else                              FFTMagnitude[i] -= error_data; // ����һ����Χ
               }
		if(i>=2) all_data+=FFTMagnitude[i];	
		FFTMagnitude[i]=sqrt(FFTMagnitude[i]);
		if(k==0) FFTMagnitude[i]<<=j;
		else    FFTMagnitude[i]>>=j;		//������Чֵ
		if(i>2) THD[i]=FFTMagnitude[i]*100.0/FFTMagnitude[1];//���λ�����
	}	
		THD[1] = all_data*10000.0/m;
		THD[1] = sqrt(THD[1]);                   
		if(THD[1]>100) THD[1]=100;//�ܻ�����
		
		all_data+=m;
		FFTMagnitude[32]=sqrt(all_data);
		if(k==0) FFTMagnitude[32]<<=j;
		else    FFTMagnitude[32]>>=j;		//����Чֵ
}	

void fft_effective_one(void)//�����ܻ����ʣ�����Чֵ
{
	unsigned long data_all,i,j;
	unsigned int error_data;
        if(FFTPreProcessCnt<6) {j=6-FFTPreProcessCnt;error_data=0;}
        else               {j=FFTPreProcessCnt-6;error_data=(2<<j)*(2<<j);}//����Ǳ�С
	data_all=0;//ȥ��ֱ������
	FFTMagnitude[0]=0;//ȥ��ֱ������	
	for(i=2;i<32;i++)
	{
                if(i>=9)
                {
                    if(FFTMagnitude[i] < error_data)  FFTMagnitude[i] = 0;
                    else                              FFTMagnitude[i] -= error_data; // ����һ����Χ
                }
		data_all+=FFTMagnitude[i];
	}
	
	THD[1] = data_all*10000.0/FFTMagnitude[1];
	THD[1] = sqrt(THD[1]);                   
	if(THD[1]>100) THD[1]=100;				  //�ܻ�����
	
	FFTMagnitude[32]=sqrt(data_all+FFTMagnitude[1]);
	if(FFTPreProcessCnt<6) FFTMagnitude[32]<<=j;
	else				   FFTMagnitude[32]>>=j;//����Чֵ
}

	
unsigned int effective_c(volatile int *data)//����ʮ�ĸ����ƽ��ֵ
{
	unsigned int i;
	unsigned long all_data=0,data_one;	
	
	for(i=0;i<64;i++)
	{
		data_one=data[i];
		all_data+=data_one*data_one;	
	}
	
	return 	(unsigned int)sqrt(all_data>>6);
}	

void fft_effective_xiebo(void)//г����Чֵ
{
	unsigned long data_all,i;
	
	data_all=0;	
	for(i=2;i<32;i++)
	{	
		data_all+=FFTMagnitude[i];
	}
	FFTMagnitude[32]=sqrt(data_all);
	if(FFTPreProcessCnt<6) FFTMagnitude[32]<<=6-FFTPreProcessCnt;
	else				   FFTMagnitude[32]>>=FFTPreProcessCnt-6;//����Чֵ	
}
	
void ACCOUNT_data(void)//�������е�����
{
	long tran_data;			
	float Va_angle,Vb_angle,Vc_angle;
	float Ia_angle,Ib_angle,Ic_angle;
	int i;
//*****������ѡ��*******************************	
	if(P_E2PROM[14]==0)//���û�е���������
	{
		for(i=0;i<64;i++)
		{
			record_wave[3][i]=record_wave[6][i]-record_wave[9][i];
			record_wave[4][i]=record_wave[7][i]-record_wave[10][i];
			record_wave[5][i]=record_wave[8][i]-record_wave[11][i];//�����������
		}	
	}
	else//���û�и��ػ�����
	if(P_E2PROM[14]==1)
	{
		for(i=0;i<64;i++)
		{	
			record_wave[6][i]=record_wave[3][i]+record_wave[9][i];
			record_wave[7][i]=record_wave[4][i]+record_wave[10][i];
			record_wave[8][i]=record_wave[5][i]+record_wave[11][i];//���㸺�ص���	
		}	
	}	
//****�����������************************	
	if(P_E2PROM[20]!=0)//������������
	{
		for(i=0;i<64;i++)
		{
			zero_wave[0][i]=record_wave[3][i]+record_wave[4][i]+record_wave[5][i];//
			zero_wave[1][i]=record_wave[6][i]+record_wave[7][i]+record_wave[8][i];//
		}	
		
		if(P_E2PROM[104]==0)//�������
		{
				WIRE_cost[6][0]=effective_c(zero_wave[0]);//�����������
				WIRE_cost[6][1]=effective_c(zero_wave[1]);//�����������		
		}
		else
		{
				fft_calculation(zero_wave[0]);
				fft_effective_xiebo();
				WIRE_cost[6][0]=FFTMagnitude[32];//�����������		
				
				fft_calculation(zero_wave[1]);
				fft_effective_xiebo();
				WIRE_cost[6][1]=FFTMagnitude[32];//�����������	
		}	
			
		WIRE_cost[6][0]=Optimization_down(WIRE_cost[6][0],30000,P_E2PROM[90],1);//�Ż�
		WIRE_cost[6][1]=Optimization_down(WIRE_cost[6][1],30000,P_E2PROM[91],1);//�Ż�
	}
	
	if(P_E2PROM[29]!=0)//��������������
	{
		for(i=0;i<64;i++)
		{
			zero_wave[2][i]=record_wave[9][i]+record_wave[10][i]+record_wave[11][i];
		}			

		WIRE_cost[6][2]=effective_c(zero_wave[2]);//�����������
	}
	
//*******���������ѹ********************************************
		fft_calculation(record_wave[0]);
		fft_effective_one();
		WIRE_cost[0][0]=FFTMagnitude[32];
		LOAD_cost[3][0]=THD[1]*10;
		Va_angle=atan2f(FFTData[1].imag,FFTData[1].real);
		LOAD_cost[3][0]=Optimization_down(LOAD_cost[3][0],P_E2PROM[84]*10,P_E2PROM[85],1);
		
		fft_calculation(record_wave[1]);
		fft_effective_one();
		WIRE_cost[0][1]=FFTMagnitude[32];
		LOAD_cost[3][1]=THD[1]*10;
		Vb_angle=atan2f(FFTData[1].imag,FFTData[1].real);
		LOAD_cost[3][1]=Optimization_down(LOAD_cost[3][1],P_E2PROM[84]*10,P_E2PROM[85],1);
		
		fft_calculation(record_wave[2]);
		fft_effective_one();
		WIRE_cost[0][2]=FFTMagnitude[32];
		LOAD_cost[3][2]=THD[1]*10;
		Vc_angle=atan2f(FFTData[1].imag,FFTData[1].real);//���������ѹ
		LOAD_cost[3][2]=Optimization_down(LOAD_cost[3][2],P_E2PROM[84]*10,P_E2PROM[85],1);
		
//*******�����������********************************************		
		fft_calculation(record_wave[3]);
		fft_effective_all();//����г��ֵ
		for(i=1;i<32;i++)
#if compilemodbus
                Modbus_electric[99+i]=FFTMagnitude[i];//modbus
#endif
		WIRE_cost[1][0]=FFTMagnitude[32];
		LOAD_cost[4][0]=THD[1]*10;
		Ia_angle=atan2f(FFTData[1].imag,FFTData[1].real);
		LOAD_cost[4][0]=Optimization_down(LOAD_cost[4][0],P_E2PROM[86]*10,P_E2PROM[87],1);
		
		fft_calculation(record_wave[4]);
		fft_effective_all();//����г��ֵ
		for(i=1;i<32;i++)
#if compilemodbus
		Modbus_electric[149+i]=FFTMagnitude[i];//modbus
#endif
		WIRE_cost[1][1]=FFTMagnitude[32];
		LOAD_cost[4][1]=THD[1]*10;
		Ib_angle=atan2f(FFTData[1].imag,FFTData[1].real);
		LOAD_cost[4][1]=Optimization_down(LOAD_cost[4][1],P_E2PROM[86]*10,P_E2PROM[87],1);
		
		fft_calculation(record_wave[5]);
		fft_effective_all();//����г��ֵ
		for(i=1;i<32;i++)
#if compilemodbus
		Modbus_electric[199+i]=FFTMagnitude[i];//modbus
#endif
		WIRE_cost[1][2]=FFTMagnitude[32];
		LOAD_cost[4][2]=THD[1]*10;	
		Ic_angle=atan2f(FFTData[1].imag,FFTData[1].real);//�����������
		LOAD_cost[4][2]=Optimization_down(LOAD_cost[4][2],P_E2PROM[86]*10,P_E2PROM[87],1);
		
//********������������************************************************		
	   if(P_E2PROM[123]==0)
	   {
	    if(Va_angle>Ia_angle) Ia_angle=Va_angle-Ia_angle;
		else                  Ia_angle=Ia_angle-Va_angle;
		LOAD_cost[1][0]=(unsigned int)(cos(Ia_angle)*100);
		//LOAD_cost[1][0]=Optimization_up(LOAD_cost[1][0],P_E2PROM[80]*10,P_E2PROM[81],100);
		
		if(Vb_angle>Ib_angle) Ib_angle=Vb_angle-Ib_angle;
		else                  Ib_angle=Ib_angle-Vb_angle;
		LOAD_cost[1][1]=(unsigned int)(cos(Ib_angle)*100);
		//LOAD_cost[1][1]=Optimization_up(LOAD_cost[1][1],P_E2PROM[80]*10,P_E2PROM[81],100);
		
		if(Vc_angle>Ic_angle) Ic_angle=Vc_angle-Ic_angle;
		else                  Ic_angle=Ic_angle-Vc_angle;
		LOAD_cost[1][2]=(unsigned int)(cos(Ic_angle)*100);//���������
		//LOAD_cost[1][2]=Optimization_up(LOAD_cost[1][2],P_E2PROM[80]*10,P_E2PROM[81],100);
	   
	   }
	   else
	   if(P_E2PROM[123]==1)
	   {
	     if(Va_angle>=Ia_angle) Ia_angle=Va_angle-Ia_angle;
		else                   Ia_angle=6.2832+Va_angle-Ia_angle;//�����������λ��		
		if(Ia_angle>3.1415)    Ia_angle -= 3.1415;   //����Ǹ���λ
		LOAD_cost[1][0]=(int)(cos(Ia_angle)*100);
		if(LOAD_cost[1][0]==0xff9c) LOAD_cost[1][0] =100;
		else
		if(LOAD_cost[1][0]==0xff9d) LOAD_cost[1][0] = 99;
//		LOAD_cost[1][0]=Optimization_up(LOAD_cost[1][0],P_E2PROM[80]*10,P_E2PROM[81],100);
		
		if(Vb_angle>=Ib_angle) Ib_angle=Vb_angle-Ib_angle;
		else                   Ib_angle=6.2832+Vb_angle-Ib_angle;//�����������λ��		
		if(Ib_angle>3.1415)    Ib_angle -= 3.1415;   //����Ǹ���λ
		LOAD_cost[1][1]=(int)(cos(Ib_angle)*100);
		if(LOAD_cost[1][1]==0xff9c) LOAD_cost[1][1] = 100;
		else
		if(LOAD_cost[1][1]==0xff9d) LOAD_cost[1][1] = 99;
//		LOAD_cost[1][1]=Optimization_up(LOAD_cost[1][1],P_E2PROM[80]*10,P_E2PROM[81],100);
		
		if(Vc_angle>Ic_angle) Ic_angle=Vc_angle-Ic_angle;
		else                   Ic_angle=6.2832+Vc_angle-Ic_angle;//�����������λ��		
		if(Ic_angle>3.1415)    Ic_angle -= 3.1415;   //����Ǹ���λ
		LOAD_cost[1][2]=(int)(cos(Ic_angle)*100);//���������
		if(LOAD_cost[1][2]==0xff9c) LOAD_cost[1][2] = 100;
		else
		if(LOAD_cost[1][2]==0xff9d) LOAD_cost[1][2] = 99;
//	        LOAD_cost[1][2]=Optimization_up(LOAD_cost[1][2],P_E2PROM[80]*10,P_E2PROM[81],100);
	
	   }

	
//********���ฺ�ص���************************************************
		fft_calculation(record_wave[6]);
		fft_effective_one();
		WIRE_cost[2][0]=FFTMagnitude[32];
		LOAD_cost[5][0]=THD[1]*10;
		Ia_angle=atan2f(FFTData[1].imag,FFTData[1].real);
		LOAD_cost[5][0]=Optimization_down(LOAD_cost[5][0],P_E2PROM[88]*10,P_E2PROM[89],1);
		
		fft_calculation(record_wave[7]);
		fft_effective_one();
		WIRE_cost[2][1]=FFTMagnitude[32];
		LOAD_cost[5][1]=THD[1]*10;
		Ib_angle=atan2f(FFTData[1].imag,FFTData[1].real);
		LOAD_cost[5][1]=Optimization_down(LOAD_cost[5][1],P_E2PROM[88]*10,P_E2PROM[89],1);

		fft_calculation(record_wave[8]);
		fft_effective_one();
		WIRE_cost[2][2]=FFTMagnitude[32];
		LOAD_cost[5][2]=THD[1]*10;
		Ic_angle=atan2f(FFTData[1].imag,FFTData[1].real);//����ص���	
		LOAD_cost[5][2]=Optimization_down(LOAD_cost[5][2],P_E2PROM[88]*10,P_E2PROM[89],1);

//********���ع�������************************************************		
		if(P_E2PROM[123]==0)
		{
		  if(Va_angle>Ia_angle) Ia_angle=Va_angle-Ia_angle;
		else                  Ia_angle=Ia_angle-Va_angle;
		LOAD_cost[2][0]=(unsigned int)(cos(Ia_angle)*100);
		LOAD_cost[2][0]=Optimization_up(LOAD_cost[2][0],P_E2PROM[82]*10,P_E2PROM[83],100);		
		
		if(Vb_angle>Ib_angle) Ib_angle=Vb_angle-Ib_angle;
		else                  Ib_angle=Ib_angle-Vb_angle;
		LOAD_cost[2][1]=(unsigned int)(cos(Ib_angle)*100);
		LOAD_cost[2][1]=Optimization_up(LOAD_cost[2][1],P_E2PROM[82]*10,P_E2PROM[83],100);
		
		if(Vc_angle>Ic_angle) Ic_angle=Vc_angle-Ic_angle;
		else                  Ic_angle=Ic_angle-Vc_angle;
		LOAD_cost[2][2]=(unsigned int)(cos(Ic_angle)*100);//���������
		LOAD_cost[2][2]=Optimization_up(LOAD_cost[2][2],P_E2PROM[82]*10,P_E2PROM[83],100);
		}
		else
		if(P_E2PROM[123]==1)
		{
		  if(Va_angle>Ia_angle) Ia_angle=Va_angle-Ia_angle;
		else                   Ia_angle=6.2832+Va_angle-Ia_angle;//�����������λ��		
		if(Ia_angle>3.1415)    Ia_angle -= 3.1415;   //����Ǹ���λ
		LOAD_cost[2][0]=(int)(cos(Ia_angle)*100);
		if(LOAD_cost[2][0]==0xff9c) LOAD_cost[2][0] = 100;
		else
		if(LOAD_cost[2][0]==0xff9d) LOAD_cost[2][0] = 99;
//		LOAD_cost[2][0]=Optimization_up(LOAD_cost[2][0],P_E2PROM[82]*10,P_E2PROM[83],100);		
		
		if(Vb_angle>Ib_angle) Ib_angle=Vb_angle-Ib_angle;
		else                   Ib_angle=6.2832+Vb_angle-Ib_angle;//�����������λ��		
		if(Ib_angle>3.1415)    Ib_angle -= 3.1415;   //����Ǹ���λ
		LOAD_cost[2][1]=(int)(cos(Ib_angle)*100);
		if(LOAD_cost[2][1]==0xff9c) LOAD_cost[2][1] = 100;
		else
		if(LOAD_cost[2][1]==0xff9d) LOAD_cost[2][1] = 99;
//		LOAD_cost[2][1]=Optimization_up(LOAD_cost[2][1],P_E2PROM[82]*10,P_E2PROM[83],100);
		
		if(Vc_angle>Ic_angle) Ic_angle=Vc_angle-Ic_angle;
		else                   Ic_angle=6.2832+Vc_angle-Ic_angle;//�����������λ��		
		if(Ic_angle>3.1415)    Ic_angle -= 3.1415;   //����Ǹ���λ
		LOAD_cost[2][2]=(int)(cos(Ic_angle)*100);//���������
		if(LOAD_cost[2][2]==0xff9c) LOAD_cost[2][2] = 100;
		else
		if(LOAD_cost[2][2]==0xff9d) LOAD_cost[2][2] = 99;
//		LOAD_cost[2][2]=Optimization_up(LOAD_cost[2][2],P_E2PROM[82]*10,P_E2PROM[83],100);
		}

		

//********��������***********************************************************
		WIRE_cost[3][0]=effective_c(record_wave[9]);
		WIRE_cost[3][1]=effective_c(record_wave[10]);
		WIRE_cost[3][2]=effective_c(record_wave[11]);//���ಹ������
	
	

//********���๦��***********************************************************
		tran_data=WIRE_cost[1][0];tran_data*=WIRE_cost[0][0];WIRE_cost[4][0]=tran_data/1000;//��������A
		tran_data=WIRE_cost[1][1];tran_data*=WIRE_cost[0][1];WIRE_cost[4][1]=tran_data/1000;//��������B
		tran_data=WIRE_cost[1][2];tran_data*=WIRE_cost[0][2];WIRE_cost[4][2]=tran_data/1000;//��������C
		
		tran_data=WIRE_cost[2][0];tran_data*=WIRE_cost[0][0];WIRE_cost[5][0]=tran_data/1000;//���ع���A
		tran_data=WIRE_cost[2][1];tran_data*=WIRE_cost[0][1];WIRE_cost[5][1]=tran_data/1000;//���ع���B
		tran_data=WIRE_cost[2][2];tran_data*=WIRE_cost[0][2];WIRE_cost[5][2]=tran_data/1000;//���ع���C	


//*********����Ƶ��**********************************************************
		LOAD_cost[0][0]=record_single[0][0];//����Ƶ��
		LOAD_cost[0][1]=record_single[0][1];//����汾��
		LOAD_cost[0][2]=record_single[0][2];//��ʾ��Ϣ
		
		if(record_single[0][2]!=0)
		{
			if(record_single[0][2]==1) message_num=12;
						
			record_single[0][2]=0;	
		}

		LOAD_cost[6][0]=record_single[1][0];//ֱ�����ѹ
		LOAD_cost[6][1]=record_single[1][1];//����ģ���¶�һ
		LOAD_cost[6][2]=record_single[1][2];//����汾һ
		
		LOAD_cost[7][0]=record_single[2][0];//�������
		LOAD_cost[7][1]=record_single[2][1];//�豸�¶�2
		LOAD_cost[7][2]=record_single[2][2];//��ʾ����
		
		LOAD_cost[8][0]=record_single[3][0];//����Ͷ��״̬1 1~15
		LOAD_cost[8][1]=record_single[3][1];//SRC��������
		LOAD_cost[8][2]=record_single[3][2];//Ͷ��״̬2     16~31
		
		LOAD_cost[9][0]=record_single[4][0];//�ֲ�Ͷ��״̬    A0~4,B5~9,C10~14
		LOAD_cost[9][1]=record_single[4][1];//�ֲ�����
		LOAD_cost[9][2]=record_single[4][2];//�ֲ���Ͷ��״̬2 A5~9,B10~14,C15~19

		if(which_equ==3) ALL_cap=record_single[2][2]*15;
		else		     ALL_cap=record_single[2][2]*10;
		
		
		tran_data=WIRE_cost[3][0];
		tran_data*=10000;
		WIRE_cost[7][0]=tran_data/ALL_cap;		
		
		tran_data=WIRE_cost[3][1];
		tran_data*=10000;
		WIRE_cost[7][1]=tran_data/ALL_cap;	
		
		tran_data=WIRE_cost[3][2];
		tran_data*=10000;
		WIRE_cost[7][2]=tran_data/ALL_cap;	
}

void ACCOUNT_buffer_one(volatile unsigned int *data_buffer,volatile unsigned int *data)//����һ�λ��幦��
{
	unsigned int i;
	unsigned long tran_data=0;
	
	data_buffer[data_buffer[21]++]=*data;
	if(data_buffer[20]==1)//���������
	{
		for(i=0;i<P_E2PROM[22];i++)   tran_data+=data_buffer[i];//����ƽ��ֵ	
		*data=tran_data/P_E2PROM[22];		                    //��ֵ���µ�����
		 if(*data<=2){data_buffer[20]=0;data_buffer[21]=0;}		//������ݱ���㣬��ջ����
	  else if(data_buffer[21]>=P_E2PROM[22]) data_buffer[21]=0;   //ָ������
	}
	else 			//��������δ��
	{
		for(i=0;i<data_buffer[21];i++) tran_data+=data_buffer[i]; //����ƽ��ֵ	
		*data=tran_data/data_buffer[21];	
		if(data_buffer[21]>=P_E2PROM[22]) {data_buffer[21]=0;data_buffer[20]=1;}
	}
}

void ACCOUNT_buffer(void)//�������еĻ�������
{
	unsigned int i,j;
	
	for(i=0;i<7;i++)
	{
		for(j=0;j<3;j++)
			ACCOUNT_buffer_one(WIRE_cost_buffer[i][j],&WIRE_cost[i][j]);			
	}
	for(i=0;i<8;i++)
	{
		for(j=0;j<3;j++)
			ACCOUNT_buffer_one(LOAD_cost_buffer[i][j],&LOAD_cost[i][j]);			
	}
}	


//***********����************
void LCD_wave_calculation(volatile unsigned int coor,volatile int *data_dir,volatile int *data,volatile int *data_b,volatile int *data1,volatile int *data_b1,volatile int *data2,volatile int *data_b2,volatile int *data_n,volatile int *data_bn)//����data�Ĳ��Σ�������ʾ��λ�÷���data_b�а�data_b�е����ݷŵ�data_bb��
{//��*data�е�����ת���� *data_b
	unsigned int i=0,j,coor_data,max_data;
	float angle;
	long data_tran;

		fft_calculation(data_dir);
		angle=atan2f(FFTData[1].imag,FFTData[1].real);
		if(angle<0) angle=-angle;		   //����Ǹ�������2*PI
		else        angle=6.2831853-angle;
		j=64.0*angle/6.2831853;	
//		max_data=temp_fft_data_min;
		
	if(coor==0)//�Զ�����
	{		
		find_max(data);
		max_data=temp_fft_data_min;
		
		find_max(data1);
		if(temp_fft_data_min>max_data) max_data=temp_fft_data_min;
		
		find_max(data2);
		if(temp_fft_data_min>max_data) max_data=temp_fft_data_min;

		find_max(data_n);
		if(temp_fft_data_min<max_data) temp_fft_data_min=max_data;
		
			 if(temp_fft_data_min<4)   coor_data=4;
		else if(temp_fft_data_min<8)   coor_data=8;
		else if(temp_fft_data_min<20)  coor_data=20;
		else if(temp_fft_data_min<40)  coor_data=40;
		else if(temp_fft_data_min<80)  coor_data=80;
		else if(temp_fft_data_min<200) coor_data=200;
		else if(temp_fft_data_min<400) coor_data=400;
		else if(temp_fft_data_min<800) coor_data=800;
		else if(temp_fft_data_min<1600) coor_data=1600;
		else if(temp_fft_data_min<3200) coor_data=3200;
		else if(temp_fft_data_min<6400) coor_data=6400;//�жϷ�Χ
		else 							coor_data=6400;//�жϷ�Χ
		
		if(coor_data>coordinate_now) i=1;
		else
		if(coor_data==coordinate_now) i=0;
		else 
		if(coor_data==8)
		{
			if(temp_fft_data_min<=6) i=1;	
		}
		else
		if(coor_data==80)
		{
			if(temp_fft_data_min<65) i=1;
		}
		else		
		if((temp_fft_data_min<<1)+(temp_fft_data_min>>2)<coordinate_now)i=1;//����ֵ
				
		if(i==1)
		{
			coordinate_now=coor_data;			
		}
	}
	else
	{
		coordinate_now=coor;
	}				
	
			for(i=0;i<64;i++)
			{	
				data_tran=data[j];
				data_b[i]=data_tran*50/coordinate_now+50;		
					 if(data_b[i]>100) data_b[i]=100;
				else if(data_b[i]<0) data_b[i]=0;
													
				data_tran=data1[j];
				data_b1[i]=data_tran*50/coordinate_now+50;
					 if(data_b1[i]>100) data_b1[i]=100;
				else if(data_b1[i]<0) data_b1[i]=0;
															
				data_tran=data2[j];
				data_b2[i]=data_tran*50/coordinate_now+50;
					 if(data_b2[i]>100) data_b2[i]=100;
				else if(data_b2[i]<0) data_b2[i]=0;
							
				data_tran=data_n[j++];
				data_bn[i]=data_tran*50/coordinate_now+50;
					 if(data_bn[i]>100) data_bn[i]=100;
				else if(data_bn[i]<0) data_bn[i]=0;//����
							
				if(j>=64)  j=0;	
			}		
}

void LCD_histogram_calculation(volatile unsigned int coor,volatile int *data,volatile int *data_b,volatile int *data1,volatile int *data_b1,volatile int *data2,volatile int *data_b2)	//��������״ͼ����
{
	unsigned int i=0,coor_data;
	long data_tran;
	int  max_tran;
	
	fft_calculation(data);//fft
	fft_effective_all();//����г��ֵ
	for(i=0;i<32;i++)
		data_b[i]=FFTMagnitude[i];		
		
	fft_calculation(data1);//fft
	fft_effective_all();//����г��ֵ
	for(i=0;i<32;i++)
		data_b1[i]=FFTMagnitude[i];
		
	fft_calculation(data2);//fft
	fft_effective_all();//����г��ֵ
	for(i=0;i<32;i++)
		data_b2[i]=FFTMagnitude[i];//
		
	if(coor==0)//�Զ�����
	{
		find_max_plus(data_b);   
		max_tran=temp_fft_data_min;		   //���ֵ
		find_max_plus(data_b1);            //���ֵ
		if(max_tran<temp_fft_data_min) max_tran=temp_fft_data_min;
		find_max_plus(data_b2);   	   	   //���ֵ
		if(max_tran>temp_fft_data_min) temp_fft_data_min=max_tran;//
	
			if(temp_fft_data_min<4)         coor_data=4;
			else if(temp_fft_data_min<8)    coor_data=8;
			else if(temp_fft_data_min<20)   coor_data=20;
			else if(temp_fft_data_min<40)   coor_data=40;
			else if(temp_fft_data_min<80)   coor_data=80;
			else if(temp_fft_data_min<200)  coor_data=200;
			else if(temp_fft_data_min<400)  coor_data=400;
			else if(temp_fft_data_min<800)  coor_data=800;
			else if(temp_fft_data_min<1600) coor_data=1600;
			else if(temp_fft_data_min<3200) coor_data=3200;
			else if(temp_fft_data_min<6400) coor_data=6400;//�жϷ�Χ
			else 							coor_data=6400;//�жϷ�Χ
			
			if(coor_data>coordinate_now) i=1;
			else
			if(coor_data==coordinate_now) i=0;
			else 
			if(coor_data==8)
			{
				if(temp_fft_data_min<=6) i=1;
			}
			else
			if(coor_data==80)
			{
				if(temp_fft_data_min<65) i=1;
			}
			else		
			if((temp_fft_data_min<<1)+(temp_fft_data_min>>2)<coordinate_now)i=1;//����ֵ
			
			if(i==1)
			{	
				coordinate_now=coor_data;		
			}
	}
	else
	{
				coordinate_now=coor;
	}			
			
			MAX_data[0][0]=0;//ʡʱ��
			MAX_data[0][1]=0;
			MAX_data[1][0]=0;
			MAX_data[1][1]=0;
			MAX_data[2][0]=0;
			MAX_data[2][1]=0;
			MAX_data[3][0]=0;
			MAX_data[3][1]=0;
			
	
	for(i=0;i<32;i++)
	{
		//****�ҳ������ĸ�����**********************ʡʱ��
		if(data_b[i]>data_b1[i]) max_tran=data_b[i];
		else                     max_tran=data_b1[i];
		if(max_tran<data_b2[i])  max_tran=data_b2[i];//��������������

		if(max_tran>MAX_data[0][1])
		{
			MAX_data[3][0]=MAX_data[2][0];		
			MAX_data[3][1]=MAX_data[2][1];
			MAX_data[2][0]=MAX_data[1][0];		
			MAX_data[2][1]=MAX_data[1][1];
			MAX_data[1][0]=MAX_data[0][0];
			MAX_data[1][1]=MAX_data[0][1];
			MAX_data[0][0]=i;			
			MAX_data[0][1]=max_tran;
		}
		else		
		if(max_tran>MAX_data[1][1])
		{
			MAX_data[3][0]=MAX_data[2][0];		
			MAX_data[3][1]=MAX_data[2][1];
			MAX_data[2][0]=MAX_data[1][0];		
			MAX_data[2][1]=MAX_data[1][1];
			MAX_data[1][0]=i;			
			MAX_data[1][1]=max_tran;	
		}
		else		
		if(max_tran>MAX_data[2][1])
		{
			MAX_data[3][0]=MAX_data[2][0];			
			MAX_data[3][1]=MAX_data[2][1];
			MAX_data[2][0]=i;			
			MAX_data[2][1]=max_tran;	
		}
		else		
		if(max_tran>MAX_data[3][1])
		{
			MAX_data[3][0]=i;			
			MAX_data[3][1]=max_tran;
		}
		
		data_tran=data_b[i];
		data_b[i]=199-data_tran*100/coordinate_now;		
			 if(data_b[i]>199) data_b[i]=199;
		else if(data_b[i]<100) data_b[i]=100;
		
		data_tran=data_b1[i];
		data_b1[i]=199-data_tran*100/coordinate_now;		
			 if(data_b1[i]>199) data_b1[i]=199;
		else if(data_b1[i]<100) data_b1[i]=100;
		
		data_tran=data_b2[i];
		data_b2[i]=199-data_tran*100/coordinate_now;		
			 if(data_b2[i]>199) data_b2[i]=199;
		else if(data_b2[i]<100) data_b2[i]=100;
	}
}

//********SVG�����******	