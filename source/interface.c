#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"
#include "..\include\string_h.h"


unsigned int page_sign=1;
unsigned int page1_sign=0;//
unsigned int page2_sign=0;//
unsigned int page3_sign=0;//
unsigned int page4_sign=0;//
unsigned int page5_sign=0;//
unsigned int page6_sign=0;//	
unsigned int page7_sign=0;//
unsigned int page8_sign=0;//
unsigned int page9_sign=0;//
unsigned int page10_sign=0;//
unsigned int page11_sign=0;//
unsigned int page12_sign=0;//
unsigned int page13_sign=0;//
unsigned int page14_sign=0;//
unsigned int page15_sign=0;//
unsigned int page15_1_sign=0;//
unsigned int page15_2_sign=0;//
unsigned int page15_3_sign=0;//
unsigned int page15_4_sign=0;//
unsigned int page15_5_sign=0;//
unsigned int page15_6_sign=0;//
unsigned int page15_7_sign=0;//
unsigned int page16_sign=0;//
unsigned int page17_sign=0;//
unsigned int page18_sign=0;//

unsigned int LAST_PAGE=0;

unsigned int command_state;//�Ͱ�ť����һ�£����°�ťִ�е�״̬��1:����;0:ֹͣ
unsigned int command_send;//�������͵���ʲô����1�����Ϳ����źţ�2��Ƿѹ��ѹֵ��������ֵ��
                          //3��У׼����ֵ��4��ϵͳ����ֵ��5��ͨ�Ų���ֵ��6�����صĲ���
                          //7,��չ��ϣ�8����յ��·�������9���ָ���������
unsigned int fault_data_num;//��ǰ��ʾ�Ĺ��ϴ���ĵ�ַ,

unsigned int tell_stop=0xffff;//�������1��˵����ʾ����

unsigned int tran_data=0;//������
