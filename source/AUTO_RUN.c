#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

volatile unsigned int  AUTO_run=0;//�ϵ��ʼ��
volatile unsigned int  AUTO_stop=0;//�ϵ��ʼ��
volatile unsigned int  Off_line=2;//���߱�־//1�����ߣ�0�������ߣ�2��������
volatile unsigned int  which_equ=2;//�ϵ�Ĭ��APF80

volatile unsigned long ask_time=0;//�鿴һ��������ѯ��ʱ��
//*********����������ʼ��******************************	

void DS1302_AUTO_RUN_STOP(void)//
{	
		if(TIME_NOW[3]==0) TIME_NOW[3]=7;
		
		if(P_E2PROM[35+(TIME_NOW[3]-1)*6]==1&&TIME_NOW[4]==P_E2PROM[36+(TIME_NOW[3]-1)*6]&&TIME_NOW[5]==P_E2PROM[37+(TIME_NOW[3]-1)*6])//�����ʱֹͣʹ��
		{
			if(AUTO_stop==0)
			{
					if(P_E2PROM[105]==0)
					{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0001,0,0,0);//ֹͣ����
							C1TR01CONbits.TXREQ1=1;//ֹͣ�ź�
							AUTO_stop=1;
							WRITE_fault_one(40,0);
#if compilemodbus
                                                        Modbus_electric[0]=40;Modbus_electric[1]=1;
#endif
					}
					else
					{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0006,0,0,0);//��������
							C1TR01CONbits.TXREQ1=1;//�����ź�
							AUTO_stop=1;	
							WRITE_fault_one(52,0);
#if compilemodbus
                                                        Modbus_electric[0]=52;Modbus_electric[1]=1;
#endif
					}	
			}
			
		}
                else if(AUTO_stop!=0&&TIME_NOW[5]!=P_E2PROM[37+(TIME_NOW[3]-1)*6])
		{
			AUTO_stop=0;
		}
                else if(P_E2PROM[32+(TIME_NOW[3]-1)*6]==1&&TIME_NOW[4]==P_E2PROM[33+(TIME_NOW[3]-1)*6]&&TIME_NOW[5]==P_E2PROM[34+(TIME_NOW[3]-1)*6])//�����ʱ����ʹ��
		{
			if(AUTO_run==0)
			{
							ecan1WriteTxMsgBufId(1,0x201,0,0);
							ecan1WriteTxMsgBufData(1,8,0x0002,0,0,0);//��������
							C1TR01CONbits.TXREQ1=1;//�����ź�
							AUTO_run=1;
							WRITE_fault_one(39,0);
#if compilemodbus                 
                                                        Modbus_electric[0]=39;Modbus_electric[1]=1;
#endif
			}

		}
                else if(AUTO_run!=0&&TIME_NOW[5]!=P_E2PROM[34+(TIME_NOW[3]-1)*6])
		{
			AUTO_run=0;
		}
}

	
void page_state(void)//�豸״̬
{	
	unsigned int n=0,m=0,l=1,p=0,i,z;
	
//***************����������**********	

//***�ж��Ƿ�����***********	
	for(i=0;i<P_E2PROM[13];i++)
	{
		if(DELAY_CAN[i]>20000) 
		{
			equipment_skate[i][2]=3;//�ж��Ƿ�����
		}
			
		if(equipment_skate[0][2]==3&&Off_line==0)//���ߺ󣬲���֮ǰ��ͨѶ����д������ʱ�����
		{
			WRITE_fault_one(24,1);
#if compilemodbus
                        Modbus_electric[0]=24;Modbus_electric[1]=1;//�豸����
#endif
			Off_line=1;//ͨѶ�ж���
		}
		else	
		if(equipment_skate[0][2]!=3) Off_line=0;//ͨѶ����
		
		if(record_single[0][2]!=0)//��ʾ��Ϣ//&&&&&
			{
					 if(record_single[0][2]==1) message_num=12;//
				else  if(record_single[0][2]==2) message_num=16;//
				else  if(record_single[0][2]==3) message_num=17;//
				else  if(record_single[0][2]==4) message_num=18;//
				else  if(record_single[0][2]==5) message_num=19;//
				else  if(record_single[0][2]==6) message_num=20;//
				else  if(record_single[0][2]==7) message_num=21;//
							
				record_single[0][2]=0;
			}			
			
		if(equipment_skate[i][2]==1)//�ж��豸�Ƿ�����
			n=1;//ֻҪ��һ���豸�����еģ���ô��Ϊ����
		
		if(equipment_skate[i][2]!=2)//�ж��Ƿ��й���
		   l=0;
		
		if(equipment_skate[i][2]==4)//�ж��Ƿ��м�ͣ
		   p=1;
		 
		if(equipment_skate[i][2]!=3)//��̨�豸ͨѶ����
			m++;//�м����豸����
#if compilemodbus
                if(equipment_skate[i][2] != 2)//����Modbus_electric[0]�ĸ�ֵֻ���ڷ�������ʱ���������Իָ�������Modbus_electric[0]ֵ������һ������ֵ������ҳ265�У������Դ˴���������������modbus����
                {
                    Modbus_electric[0] = 0;
                    Modbus_electric[1] = 0;
                    Modbus_electric[84] = 0;
                    Modbus_electric[85] = 0;
                    Modbus_electric[86] = 0;
                    Modbus_electric[87] = 0;
                    Modbus_electric[88] = 0;
                }
#endif
	}

	
//**�ж��������صĸ���************
		equiment_set[0]=m;
		
//*******�жϹ���״̬************û��
		APF_error=l;

//******��¼����ʱ��******�޸İ�ť״̬******
	if(n==1) 
	{
		APF_state=1;

		if(run_time_sign>=2000)//����豸һֱ��ͣ��ÿһǧ���Ӵ�һ��
		{
			E2ROM_WR(3,2,P_E2PROM+3);//д��E2
			E2ROM_WR(962,2,P_E2PROM+111);//д��E2
			run_time_sign=0;
		}	
	}
	else   	 
	{
		if(p==1) APF_state=4;//��ͣ   ȷ���豸��ǰ����״̬����ֻҪ��һ���豸�����еģ���ô��Ϊ����
		else    APF_state=0;//ֹͣ		
		
		if(run_time_sign!=0)//�����ֹͣ״̬����������ʱ���Ѿ��ı䣬�޸�����ʱ��
		{
			E2ROM_WR(3,2,P_E2PROM+3);//д��E2
			E2ROM_WR(962,2,P_E2PROM+111);//д��E2
			run_time_sign=0;
		}
	}

//****�ж��豸�Ƿ��޸ģ���д��E2************
		m=(equipment_ID[0][2]>>4)&0xff;
		n=equipment_ID[0][2]&0x0f;
		l=equipment_ID[0][3]&0xfff;
		p=equipment_ID[0][3]>>12;//�豸����

		if(p!=3&&p!=4) p=2;//����SVG,����APF80
		else  		   p=3;

		if(m!=P_E2PROM[23]||n!=P_E2PROM[24]||l!=P_E2PROM[25]||p!=which_equ)
		{
			 P_E2PROM[23]=m;
			 P_E2PROM[24]=n;
			 P_E2PROM[25]=l;
			 
			 E2ROM_WR(23,3,P_E2PROM+23);
			 
			 which_equ=p;
			 equipment_skate_sign=0xaaaa;
		}
	
//***********д���ϴ���	*****************************
             
		for(i=0;i<P_E2PROM[13];i++)//д���ϴ���
		{    
			if(equipment_skate[i][1]!=0)//500ms�������ι��ϼ�¼����һ��
			{				
				if((equipment_skate[i][1]&0x8000)==0)//����˫ģ��
				{
					if((equipment_skate[i][1]&0xff00)==0x1000)
					{
						WRITE_fault_shadow(equipment_skate[i][1]&0x00ff,i+1);//���صĲ˵�	
					}
					else
					if((equipment_skate[i][1]&0xff00)==0x2000)
					{
							m=equipment_skate[i][1]&0x00ff;								
								if(m==1) message_num=12;//
							else  if(m==2) message_num=16;//
							else  if(m==3) message_num=17;//
							else  if(m==4) message_num=18;//
							else  if(m==5) message_num=19;//
							else  if(m==6) message_num=20;//
							else  if(m==7) message_num=21;//
					}	
					else
					if(equipment_skate[i][1]>0&&equipment_skate[i][1]<=100)//�Ժ����ӹ��ϴ�����Ҫ���������ӷ�Χ
					{
						WRITE_fault_one(equipment_skate[i][1],i+1);
#if compilemodbus
                                                Modbus_electric[0]=equipment_skate[i][1];Modbus_electric[1]=i+1;
#endif
						message_num=equipment_skate[i][1]|0x8000;//����ʾ������ʾ
					}
					else
					{
						WRITE_fault_one(0,i+1);//���������Χ��д��0
					}
				}
				else
				if((equipment_skate[i][1]&0x8000)==0x8000)
				{
					equipment_skate[i][1]&=0x0fff;
					if(equipment_skate[i][1]>0&&equipment_skate[i][1]<=100)//�Ժ����ӹ��ϴ�����Ҫ���������ӷ�Χ
					{
						WRITE_fault_one(equipment_skate[i][1],i+2);
#if compilemodbus
                                                Modbus_electric[0]=equipment_skate[i][1];Modbus_electric[1]=i+1;
#endif
						message_num=equipment_skate[i][1]|0x8000;//����ʾ������ʾ
					}
					else
					{
						WRITE_fault_one(0,i+2);//���������Χ��д��0
					}
				}			
				
				equipment_skate[i][1]&=0x00ff;
#if compileGSM
				if(P_E2PROM[131]==1)
                                {
                                    z=0;
                                    if(equipment_skate[i][1]>=1&&equipment_skate[i][1]<=16)
                                    {
                                       if((P_E2PROM[132]&(0x0001<<(equipment_skate[i][1]-1)))!=0) z=1;
                                    }
                                    else
                                    if(equipment_skate[i][1]>=17&&equipment_skate[i][1]<=32)
                                    {
                                        if((P_E2PROM[133]&(0x0001<<(equipment_skate[i][1]-17)))!=0)z=1;
                                    }
                                    else
                                    if(equipment_skate[i][1]>=33&&equipment_skate[i][1]<=48)
                                    {
                                       if((P_E2PROM[134]&(0x0001<<(equipment_skate[i][1]-33)))!=0)z=1;
                                    }
                                    else
                                    if(equipment_skate[i][1]>=49&&equipment_skate[i][1]<=64)
                                    {
                                        if((P_E2PROM[135]&(0x0001<<(equipment_skate[i][1]-49)))!=0)z=1;
                                    }
                                    else
                                    if(equipment_skate[i][1]>=65&&equipment_skate[i][1]<=80)
                                    {
                                        if((P_E2PROM[136]&(0x0001<<(equipment_skate[i][1]-65)))!=0)z=1;
                                    }
                                    else
                                    if(equipment_skate[i][1]>=81&&equipment_skate[i][1]<=96)
                                    {
                                        if((P_E2PROM[137]&(0x0001<<(equipment_skate[i][1]-81)))!=0)z=1;
                                    }
                                    if(z==1)send_message();
                                }
                                else
                                {
                                    send_message();
                                }
#endif
				equipment_skate[i][1]=0;
			}
		}
	
            }
//}
void send_elect(void)
{
	unsigned int j;
	//********�����������������****************  
    if(equipment_ID[4][0]==0&&(electric_receive_sign==2||electric_receive_sign==0)&&DELAY_elect_synch>=P_E2PROM[115]*10)//����ͬ���ռ����� 500ms
    {    
	    if(which_equ==2) j=record_single[2][2];
	    else			 j=0;
	    
	    ecan1WriteTxMsgBufId(0,0x000,0,0);
		ecan1WriteTxMsgBufData(0,8,0xaaaa,equipment_skate[0][2],0,j);//ϵͳ����
		C1TR01CONbits.TXREQ0=1;	
		
		equipment_ID[4][0]++;
		DELAY_elect_synch=0;
	}
	else
	if(equipment_ID[4][0]==1&&DELAY_elect_synch>200)//����ͬ���ռ������10ms ����һ�� ��ѹ������
	{	
		ecan1WriteTxMsgBufId(0,0x000,0,0);
		ecan1WriteTxMsgBufData(0,8,0xcccc,LOAD_cost[3][0],LOAD_cost[3][1],LOAD_cost[3][2]);//ϵͳ����
		C1TR01CONbits.TXREQ0=1;	
		
		equipment_ID[4][0]++;
	}
	else
	if(equipment_ID[4][0]==2&&DELAY_elect_synch>400)//����ͬ���ռ������40ms ����һ�� ������
	{
		if(which_equ==3) j=record_single[2][2];
	    else			 j=0;
	    
					ecan1WriteTxMsgBufId(0,0x000,0,0);
					ecan1WriteTxMsgBufData(0,8,0xbbbb,equipment_ID[0][1],1,j);//����������ָ�� ȫ������
					C1TR01CONbits.TXREQ0=1;
					
					equipment_ID[4][0]=0;
	}	
}	









