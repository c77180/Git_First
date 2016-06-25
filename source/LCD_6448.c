#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"
#include "..\include\string_h.h"


void LCD6448_Config(void)//LCD���ã���ʼ��
{
	_TRISC3=0;
	_TRISC4=0;//ת��оƬ����������ţ����

	TRISDbits.TRISD0_7=0x00;//���ݷ��� ���
	_TRISC14=0;
	_TRISD13=0;
	_TRISD12=0;
	_TRISG2=0;
	_TRISG3=0;//���Ʒ������
	
	_LATC3=1;
	_LATC4=1;//ת��оƬ����A-B
	
	LCD6448_RD=1;
	LCD6448_WR=1;
}	


void LCD6448_X(unsigned char ADD)//�е�ַ�Ĵ��� X(0-639)
{
	LCD6448_CS=0;
	LCD6448_A1=0;
	LCD6448_A0=0;
	LCD6448_data=ADD;
	LCD6448_WR=0;
	DELAY(1);//>=40ns
	LCD6448_WR=1;
	LCD6448_CS=1;
}	

void LCD6448_Y(unsigned char ADD)//�е�ַ�Ĵ��� Y(0-479)
{
	LCD6448_CS=0;
	LCD6448_A1=0;
	LCD6448_A0=1;
	LCD6448_data=ADD;
	LCD6448_WR=0;
	DELAY(1);//>=40ns
	LCD6448_WR=1;
	LCD6448_CS=1;
}	

void LCD6448_Wcmd(unsigned char CMD)//д���ƼĴ��� CMD
{
	LCD6448_CS=0;
	LCD6448_A1=1;
	LCD6448_A0=0;
	LCD6448_data=CMD;
	LCD6448_WR=0;
	DELAY(1);//>=40ns
	LCD6448_WR=1;
	LCD6448_CS=1;
}

void LCD6448_Wdata(unsigned char DAT)//д���ݼĴ���DAT
{
	LCD6448_CS=0;
	LCD6448_A1=1;
	LCD6448_A0=1;
	LCD6448_data=DAT;
	LCD6448_WR=0;
	DELAY(1);//>=40ns
	LCD6448_WR=1;
	LCD6448_CS=1;
}

void LCD6448_light(unsigned int light)
{
	LCD6448_Wcmd(0x06);
	LCD6448_Wdata(light);	
}	

void LCD6448_Waddr(unsigned int x,unsigned int y)//д��ַ639*479
{
	LCD6448_X(x%256);
	LCD6448_X(x/256);
	LCD6448_Y(y%256);
	LCD6448_Y(y/256);	
}	

void LCD6448_clear(unsigned char data) //����  data������ɫ
{     
	 LCD6448_Wcmd(0x04);//���ñ���ɫ
	 LCD6448_Wdata(data);
	 LCD6448_Wcmd(0x08);//����
	 DELAY(65000);//����ʱ16.6 ����  30M//��ʱ***************************************************************
}

void LCD6448_Bcolour(unsigned char colour) //���ñ���ɫ
{
	LCD6448_Wcmd(0x04);
	LCD6448_Wdata(colour);
}

void LCD6448_Fcolour(unsigned char colour) //����ǰ��ɫ
{
	LCD6448_Wcmd(0x02);
	LCD6448_Wdata(colour);
}

void LCD6448_Scolour(unsigned char font,unsigned char back) //������ɫ,ǰ��ɫ��ǰ������ɫ�ں�
{
	LCD6448_Fcolour(font);
	LCD6448_Bcolour(back);
}

void LCD6448_Wbit(unsigned int x,unsigned int y,unsigned char colour)//��colourΪ��ɫ��X,Y��дһ����
{  
     LCD6448_Wcmd(0x00);//����ģʽ
	 LCD6448_Waddr(x,y);
	 LCD6448_Wdata(colour);
}
void LCD6448_line_Wbit(unsigned int x,unsigned int y,unsigned int num,unsigned char colour,unsigned int way)//��colourΪ��ɫ��X,Y��дһ����
{  
    unsigned int i;   	 
	 if(way==0)  LCD6448_Wcmd(0x00);//ˮƽ
	 else        LCD6448_Wcmd(0x10);//��ֱ

	 LCD6448_Waddr(x,y);
	 for(i=0;i<num;i++)     //�߳�
			LCD6448_Wdata(colour);   
}

void LCD6448_Wpicture(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,const unsigned char *character)//��x,y����ʾrow*tier��ͼƬ
{                                   //��ʾͼƬ
     unsigned int i,j;
	 LCD6448_Wcmd(0x00);//����ģʽ
	 for(i=0;i<tier;i++)
	 {
		LCD6448_Waddr(x,y++);
		for(j=0;j<row;j++)
			LCD6448_Wdata(*(character++));//д�ַ���Ҫ  350ns		 
     }
}


void LCD6448_Wword(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,const unsigned char *character)//��x,y����ʾrow*tier���ַ�
{                        //��ʾ�ַ�
     unsigned int i,j;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 row=row/8;//дһ�����˸���ַ

	 for(i=0;i<tier;i++)
	 {       
			 LCD6448_Waddr(x,y++);
			 for(j=0;j<row;j++)
					 LCD6448_Wdata(*(character++));//д�ַ���Ҫ  350ns
     }
}

void LCD6448_blank(unsigned int x,unsigned int y,unsigned int row,unsigned int tier)//��� ��x,y����ʾrow*tier���ַ� 
{                        //��ʾ�ַ�
     unsigned int i,j;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 row=row>>3;//дһ�����˸���ַ
	 for(i=0;i<tier;i++)
	 {       
			 LCD6448_Waddr(x,y++);
			 for(j=0;j<row;j++)
					 LCD6448_Wdata(0);//д�ַ���Ҫ  350ns
     }
}



void LCD6448_Wstring(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,unsigned int num,const unsigned int *character)//��x,y����ʾrow*tier���ַ�����������
{                        //дһ���ַ�����flashΪ�ֿ⣬character�б����ַ���λ�ã����һ���������֣�num:����
     unsigned int i,j,k,row1,m,n=0;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 row1=row>>3;//дһ�����˸���ַ

	 for(k=0;k<num;k++)
	 {
			n=0;
			m=y;	
			if(row==32&&tier==32)
				K9F1208_Rpage(character[k] & 0x0fff,(character[k]&0xf000)>>7,128,flash_buffer);//
			else	
			if(row==16&&tier==32)
				K9F1208_Rpage(character[k] & 0x0fff,(character[k]&0xf000)>>7,64,flash_buffer);//
			else
			if(row==48&&tier==32)
				K9F1208_Rpage(character[k] & 0x0fff,(character[k]&0xf000)>>7,192,flash_buffer);//
				
				 for(i=0;i<tier;i++)
				 {       
						 LCD6448_Waddr(x+row*k,m++);
						 for(j=0;j<row1;j++)							 
									LCD6448_Wdata(flash_buffer[n++]);
			     }
	}
}



void LCD6448_num(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,int record,unsigned int point,unsigned int num,unsigned int unit,const unsigned char *character)//��x,y����ʾrow*tier���ַ�
{                        //��X,Y,����ʾrecord,����С����point��1:һλ��10����λ��
						//unit����0���ޣ�1��A;2��V��3��-��4::5:H,6:z,7:W; num:һ���м�λ����1��һλ��10����λ   //���Զ�����ʣ�µ�λ��7λ
     unsigned int i=0,j,k=0,row1,l,n,m=10000;
     unsigned char data[8],data_num=0;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 row1=row>>3;//дһ�����˸���ַ
	 
	 if(record<0) 
	 {
		 data[data_num++]=13;//���recordΪ������ǰ��Ӹ���
		 record*=-1;         //�������		 
	 }
	 
	 while(m!=0)
	 {
	//		i=num/m;
			
//			i=point/m;
			
			if(i==0) i=num/m+point/m; //�Ƿ���Ҫ��ǰ����
			n=record/m;  //���λ
			record=record%m;  //	
			
			if((n!=0)||(k!=0)||(i!=0))		//������λ��Ϊ��
			{ 
				if(k==0)  k=1;
				data[data_num++]=n;           //�����ݼ�������
				if((m==point)&&(point!=1)) data[data_num++]=10;//����С����
			}

			m/=10;  //
	 }	 
	 
	 l=row*tier>>3;	  
	 
	 if((unit & 0x0f)!=0) data[data_num++]=10+(unit & 0x0f);//��ӷ��� 
	 if(((unit >> 4)&0x0f)!=0)   data[data_num++]=10+(((unit >> 4)&0x0f));//��ӷ��� 
	 if((unit >> 8)!=0)   data[data_num++]=10+(unit >> 8);//��ӷ���    //�����������
	 
	 	
	 for(k=0;k<8;k++)
	 {
			n=0;
			m=y;		
				 for(i=0;i<tier;i++)
				 {       
						 LCD6448_Waddr(x+row*k,m++);
						 for(j=0;j<row1;j++)
						 {
							if(k<data_num)
								 LCD6448_Wdata(character[data[k]*l+n++]);//д�ַ���Ҫ  350ns
							else
								 LCD6448_Wdata(0x00);
						 }
			     }
	}
}

void LCD6448_Wshadow(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,unsigned int shadow,unsigned int colour)
{										//��ʾһ������Ӱ�ķ��飬ǰ��ɫ��ĳ�colour,Ҳ������ʾһ��ֱ��
	 unsigned int i,j,row1,tier1;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 row1=row>>3;//дһ�����˸���ַ
     tier1=tier>>3;
	 for(i=0;i<tier;i++)//��ʾһ������
	 {       
			 LCD6448_Waddr(x,y+i);
			 for(j=0;j<row1;j++)
					 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns 
     }
     
     if(shadow!=0)
     {
     
			     LCD6448_Fcolour(colour); //��Ӱ����ɫ��01����
			     LCD6448_Wcmd(0x01);//�ַ�ģʽ
				  for(i=0;i<shadow;i++)//����ĺ��� ��Ӱ
				  {
					  LCD6448_Waddr(x+i,y+tier+i-1);
						 for(j=0;j<row1;j++)
								 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns 
				  } 
				  
				  LCD6448_Wcmd(0x11);//y++,8λ  
				  for(i=0;i<shadow;i++)//�����������Ӱ
				  {
					  LCD6448_Waddr(x+row+i-1,y+i);
						 for(j=0;j<tier1;j++)
								 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns
				  }	
	  }
}	


void LCD6448_line(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,unsigned int way)
{				//0:����ֱ�ߣ�1������ֱ�ߣ�������ʾ����
	 unsigned int i,j,row1,tier1;

     if(way==0)
     {
	     LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 	 row1=row>>3;//дһ�����˸���ַ
	 	 
	 	 for(i=0;i<tier;i++)//��ʾһ������
	 	 {       
			 LCD6448_Waddr(x,y+i);
			 for(j=0;j<row1;j++)
					 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns 
     	 } 
	 }
	 else
	 {
		 LCD6448_Wcmd(0x11);
		 tier1=tier>>3;
		 
		 for(i=0;i<row;i++)//��ʾһ������
	 	 {       
			 LCD6448_Waddr(x+i,y);
			 for(j=0;j<tier1;j++)
					 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns 
     	 } 
	  } 
}	

void LCD6448_button(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,unsigned int shadow,unsigned int F_colour,unsigned int button)
{										//��ʾ��ť����Ӱ��button=0��û�а��£�    colour: ����ť����ɫ;  �ı���ɫ
	 unsigned int i,j,row1,tier1,k,l;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 row1=row>>3;//дһ�����˸���ַ
	 tier1=tier>>3;

     //�·�			        			     
			     

    for(l=0;l<2;l++)
    {
			if(l==0)
			{
				if(button==0) k=1;
				else          k=0;	
			}
			else
			{
				if(button==0) k=0;
				else          k=1;	
			}	

	        
	        if(k==0)
			{     
			     if(button==0) LCD6448_Fcolour(64); //��Ӱ����ɫ �·�
			     else          LCD6448_Fcolour(F_colour); 
			     
					  LCD6448_Wcmd(0x01);//�ַ�ģʽ
					  for(i=0;i<shadow;i++)//����ĺ��� ��Ӱ
					  {
						     if(i==shadow-1 && button!=0) {LCD6448_Fcolour(255);LCD6448_Wcmd(0x01);}//���һ���߸ĳɰ�ɫ����	     
						     LCD6448_Waddr(x+i,y+tier+i-1);
							 for(j=0;j<row1;j++)
									 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns 
					  } 		  
					  
				if(button==0) LCD6448_Fcolour(64); //��Ӱ����ɫ  �Ҳ�
			     else          LCD6448_Fcolour(F_colour); 
					  
					  LCD6448_Wcmd(0x11);//y++,8λ  
					  for(i=0;i<shadow;i++)//�����������Ӱ
					  {
						     if(i==shadow-1 && button!=0) {LCD6448_Fcolour(255);LCD6448_Wcmd(0x011);}//���һ���߸ĳɰ�ɫ����
						     LCD6448_Waddr(x+row+i-1,y+i);
							 for(j=0;j<tier1;j++)
									 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns
					  }   		
			}	
							  	
//�Ϸ�				
			if(k==1)
			{		  

				 if(button==0) LCD6448_Fcolour(F_colour); //��Ӱ����ɫ�ϲ�
			     else          LCD6448_Fcolour(64); 
			     
			     LCD6448_Wcmd(0x01);//�ַ�ģʽ
				  for(i=shadow;i>0;i--)//����ĺ��� ��Ӱ
				  {
					     if(i==1 && button==0) {LCD6448_Fcolour(255);LCD6448_Wcmd(0x01);}//���һ���߸ĳɰ�ɫ����
					     LCD6448_Waddr(x+i-1,y+i-1);
						 for(j=0;j<row1;j++)
								 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns 
				  } 
				  
				  
				 if(button==0) LCD6448_Fcolour(F_colour); //��Ӱ����ɫ���
			     else          LCD6448_Fcolour(64); 				 
				  LCD6448_Wcmd(0x11);//y++,8λ  
				  for(i=shadow;i>0;i--)//�����������Ӱ
				  {					     
					     if(i==1 && button==0) {LCD6448_Fcolour(255);LCD6448_Wcmd(0x011);}//���һ���߸ĳɰ�ɫ����
					     LCD6448_Waddr(x+i-1,y+i-1);
						 for(j=0;j<tier1;j++)
								 LCD6448_Wdata(0xff);//д�ַ���Ҫ  350ns
				  }	  
			}
			
		  }
}


void LCD6448_button_state(unsigned char colour)//����KEY_list ��ʾ��ť״̬ �ı���ɫ
{ 
	 unsigned int i;
	 
	 LCD6448_Fcolour(colour);//ǰ��ɫ	�Ϻ�ɫ
	 LCD6448_Wshadow(534,50,96,64,0,64);//Ӱ�ӷ���
	 LCD6448_button(534,50,96,64,4,colour,KEY_list[0]==0);//���
	 
	 LCD6448_Fcolour(colour);//ǰ��ɫ	�Ϻ�ɫ
	 LCD6448_Wshadow(534,128,96,64,0,64);//Ӱ�ӷ���
	 LCD6448_button(534,128,96,64,4,colour,KEY_list[1]==0);//���
	 
	 LCD6448_Fcolour(colour);//ǰ��ɫ	�Ϻ�ɫ
	 LCD6448_Wshadow(534,206,96,64,0,64);//Ӱ�ӷ���
	 LCD6448_button(534,206,96,64,4,colour,KEY_list[2]==0);//���
	 
	 LCD6448_Fcolour(colour);//ǰ��ɫ	�Ϻ�ɫ
	 LCD6448_Wshadow(534,284,96,64,0,64);//Ӱ�ӷ���
	 LCD6448_button(534,284,96,64,4,colour,KEY_list[3]==0);//���
	 
	 LCD6448_Fcolour(colour);//ǰ��ɫ	�Ϻ�ɫ
	 LCD6448_Wshadow(534,362,96,64,0,64);//Ӱ�ӷ���
	 LCD6448_button(534,362,96,64,4,colour,KEY_list[4]==0);//���
	 
	 LCD6448_Scolour(255,colour);//���֣��ӱ���
	 for(i=0;i<5;i++)
	 {	 
		 if(KEY_list[i]!=0)
		 LCD6448_Wstring(550,66+i*78,32,32,2,KEY_LIST[KEY_list[i]-1]);//��ʾ�ַ�
	 } 
}	

void LCD6448_button_down(unsigned int x,unsigned int y,unsigned int num)//��ť����ʱ��ʾ �ı���ɫ
{	
	LCD6448_button(x,y,96,64,4,colour_button,1);//���	
	LCD6448_Scolour(255,colour_button);//���֣���ɫ����
	LCD6448_blank(x+16,y+16,64,32);//���
	LCD6448_Wstring(x+20,y+20,32,32,2,KEY_LIST[KEY_list[num]-1]);//��ʾ	 ��ť��1��ʼ��0����Ϊδʹ��
}	


void LCD6448_button_up(unsigned int x,unsigned int y,unsigned int num)//��ť�ɿ�ʱ��ʾ �ı���ɫ
{	
	LCD6448_button(x,y,96,64,4,colour_button,0);//���	
	LCD6448_Scolour(255,colour_button);//���֣���ɫ�ӱ���
	LCD6448_blank(x+20,y+20,64,32);//���
	LCD6448_Wstring(x+16,y+16,32,32,2,KEY_LIST[KEY_list[num]-1]);//��ʾ	
}


void LCD6448_list(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,unsigned int begin,unsigned int end,unsigned int interval,unsigned int Align,unsigned int unit,unsigned int num,const unsigned int character[][num])//��ʾ�˵�
{  //x,y:��ʼ���꣬row��tierһ���ַ��ĳ���begin,end:��ʼ��������һ��0��interval:���¼��,
   //Align��0����룬1���Ҷ��룬2���м����;num:�����ַ������飬*character�������׵�ַ,
   unsigned int i;
   unsigned int x_align=0;
   
   for(i=begin;i<=end;i++)
   {
	   if(Align==1)  x_align=(num-character[i][0]-1)*row;
	   else
	   if(Align==2)	 x_align=((num-character[i][0]-1)>>1)*row;
	   	   
	   LCD6448_Wstring(x+x_align,y+i*interval,row,tier,character[i][0],character[i]+1);//��ʾ�ַ�,��һ���ַ�������ʾ������
	   
	   if(unit!=0)  LCD6448_Wword(x+x_align+character[i][0]*row,y+i*interval,16,32,Num16_32+896);//ð��
	   
	} 
	
}

void LCD6448_num_list(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,unsigned num,unsigned int interval,unsigned int record[][3],const unsigned char *character)//��x,y����ʾrow*tier���ַ�
{  //num��һ�����У�intervalÿ�м����٣� record����ʾ������ָ�룬һά���ݣ���άС���㣬��ά����,character:ʹ�õ���������
	unsigned int i;
	for(i=0;i<num;i++)
	{
		LCD6448_num(x,y+i*interval,row,tier,record[i][0],record[i][1],0,record[i][2],character);//��x,y����ʾrow*tier���ַ�
	}
}

	
void LCD6448_clear_menu(unsigned int num,const unsigned int *character)
{//ɾ�����������е����ݣ����ı�˵���������
	unsigned int i;
	
	LCD6448_Scolour(colour_num,colour_window);
	LCD6448_blank(26,60,502,365);//�������
	i=num<<5;
	LCD6448_Scolour(colour_button,colour_window);//   �����ڵ�һ��ֱ��
	LCD6448_Wshadow(30,100,i,8,0,64);//Ӱ�ӷ���
	LCD6448_button(30,100,i,8,2,colour_button,0);

	LCD6448_Scolour(colour_num,colour_window);
	LCD6448_Wstring(30,62,32,32,num,character);		
}

void LCD6448_time_num(unsigned int x,unsigned int y,unsigned char record)//��x,y����ʾrow*tier���ַ�
{                        //�޸�ʱ�������ʮ������ ��ʾ
	unsigned int i;
	  i=record;
	 LCD6448_Wword(x,y,16,32,Num16_32+((i&0xf0)<<2));
	 LCD6448_Wword(x+16,y,16,32,Num16_32+((i&0x0f)<<6));
}

void LCD6448_num_set(unsigned int x,unsigned int y,int record,unsigned num)//��x,y����ʾrow*tier���ַ�����С��ʾ��λ
{                        //��X,Y, num:�������Ӽ�λ�հ�,��С��ʾ2λ�������numλ�հ�
     int i,j,k,n,m; 
     unsigned char data[8],data_num=0;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 
	 while(record!=0)
	 {
			n=record%10; 
			data[data_num++]=n;
			record/=10;
	 }
	
	if(data_num==0) {data[0]=0;data[1]=0;data_num=2;}
	else
	if(data_num==1) {data[1]=0;data_num=2;}
	
	while(data_num<num)//
	{
		data[data_num++]=0;
	}	
	
	 for(k=0;k<data_num;k++)
	 {
		   n=0;
		   m=y;
				 for(i=0;i<32;i++)
				 {       
						 LCD6448_Waddr(x+16*k,m++);
						 for(j=0;j<2;j++)
						 {
							if(k<data_num)
								 LCD6448_Wdata(Num16_32[(data[data_num-k-1]<<6)+n++]);//д�ַ���Ҫ  350ns
							else
								 LCD6448_Wdata(0x00);
						 }
			     }
	}
}

void LCD6448_num_bit(unsigned int x,unsigned int y,unsigned int record,unsigned int num,unsigned int point)//��x,y����ʾrow*tier���ַ�,��ʾ1λ
{                        //num:�ڼ�λ����ʾ
						 //point��������λ��Ч����,�Ǽ�λ
     unsigned int i=0,j,k=0,row1,n,m=10000;
     unsigned char data[8],data_num=0;
     unsigned int num_byte,num_bit,point_bit=1,num_Bbit=1;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 row1=2;//дһ�����˸���ַ
	 num_bit=num>>8;       //���ڼ�λ��
	 num_byte=num & 0xff;  //һ������λ
	 
	 for(i=0;i<point;i++)    point_bit*=10;
	 for(i=1;i<num_byte;i++) num_Bbit*=10;
	 
	 i=0;
	 while(m!=0)
	 {		
			if(i==0) i=num_Bbit/m+point_bit/m; //�Ƿ���Ҫ��ǰ����
			n=record/m;  //���λ
			record=record%m;  //	
			
			if((n!=0)||(k!=0)||(i!=0))		//������λ��Ϊ��
			{ 
				if(k==0)  k=1;
				data[data_num++]=n;           //�����ݼ�������
				if((m==point_bit)&&(point_bit!=1)) data[data_num++]=10;//����С����
			}
			m/=10;  //
	 }
	 	 	
	 for(k=0;k<data_num;k++)
	 {
			n=0;
			m=y;		
				if(num==0)//����
				{
					 
					 LCD6448_Scolour(colour_window,colour_num);//����
				}
				else
				if(num==0xff)//����
				{
					 LCD6448_Scolour(colour_num,colour_window);//����
				}
				else	
				 if(num_byte-num_bit>=point)//��С����֮ǰ
				{
						if(k==num_bit-1) LCD6448_Scolour(colour_window,colour_num);//����	
						else             LCD6448_Scolour(colour_num,colour_window);//����
				} 
				else
				{
						if(k==num_bit) LCD6448_Scolour(colour_window,colour_num);//����	
						else           LCD6448_Scolour(colour_num,colour_window);//����	
				}
				
				LCD6448_Wcmd(0x01);//�ַ�ģʽ

				 for(i=0;i<32;i++)
				 {       
						 LCD6448_Waddr(x+16*k,m++);
						 for(j=0;j<row1;j++)
						 {
							LCD6448_Wdata(Num16_32[(data[k]<<6)+n++]);//д�ַ���Ҫ  350ns
						 }
			     }
	}
}

void LCD6448_SET_focus(unsigned int num,unsigned int num_up,unsigned int character[],const unsigned int record[][4])
{//num���任���˵ڼ�����record��Ϣ��ŵĵط�
 //���record[num][2]�����λΪ1����num_upΪ�ϴ���ʾ��λ��,�߰�λΪ������������ʾλ��λ�ã��ս�����Ч
 //���record[num][2]�����λλ0����num_upΪ�ϴ���ʾ��λ��
//	unsigned int place_bit;
	
	if((record[num_up&0xff][2]&0x8000)==0x8000)//������λΪ1
	{
			LCD6448_num_bit(record[num_up&0xff][0],record[num_up&0xff][1],character[num_up&0xff],(record[num_up&0xff][2]&0xff)|0xff00,record[num_up&0xff][3]);
	}
	else
	{
		LCD6448_Scolour(colour_num,colour_window);//�����һ�ε�		
		if((record[num_up&0xff][3]&0x8000)==0x8000&&(record[num_up&0xff][3]&0x7fff)==character[num_up&0xff])//����ﵽ��ֵ
		{
			LCD6448_blank(record[num_up&0xff][0],record[num_up&0xff][1],64,32);//�������
			LCD6448_Wstring(record[num_up&0xff][0],record[num_up&0xff][1],32,32,2,always_char);//����
		}
		else
		{		
			if((record[num_up&0xff][0] & 0xc000)==0)
				LCD6448_num_set(record[num_up&0xff][0],record[num_up&0xff][1],character[num_up&0xff],0);
			else
				LCD6448_num_bit(record[num_up&0xff][0]&0x3fff,record[num_up&0xff][1],character[num_up&0xff],0xff,record[num_up&0xff][0]>>14);		
		}	
	}	
	

	if((record[num][2]&0x8000)==0x8000)//������λΪ1
	{
		LCD6448_num_bit(record[num][0],record[num][1],character[num],(num_up&0xff00)|(record[num][2]&0xff),record[num][3]);
	}
	else
	{		
		if((record[num&0xff][3]&0x8000)==0x8000&&(record[num&0xff][3]&0x7fff)==character[num&0xff])//����ﵽ��ֵ
		{
			LCD6448_Scolour(colour_num,colour_window);//���
			LCD6448_blank(record[num&0xff][0],record[num&0xff][1],64,32);//�������
			LCD6448_Scolour(colour_window,colour_num);//����
			LCD6448_Wstring(record[num&0xff][0],record[num&0xff][1],32,32,2,always_char);//����
		}
		else
		{
			LCD6448_Scolour(colour_window,colour_num);//������һ�ε�
			if((record[num&0xff][0] & 0xc000)==0)
				LCD6448_num_set(record[num][0],record[num][1],character[num],0);
			else
				LCD6448_num_bit(record[num&0xff][0]&0x3fff,record[num&0xff][1],character[num&0xff],0,record[num&0xff][0]>>14);
		}	
	}	
};	
void LCD6448_SET_ADD(unsigned int num,unsigned int ADD_SUB,unsigned int character[],const unsigned int record[][4])
{//num���任���˵ڼ�����record��Ϣ��ŵĵط� ADD_SUB:0:����1����
	unsigned int i=0,j;
	unsigned int num_byte,num_bit;
	
	num_byte=num & 0xff;
	num_bit =num & 0xff00;
	
	if((record[num_byte][2]&0x8000)==0x8000)//������λΪ1
	{
		LCD6448_Scolour(colour_window,colour_num);//����
		
		j=1;
		for(i=0;i<(record[num_byte][2]&0x7fff)-(num_bit>>8);i++) j*=10;	
		
		if(ADD_SUB==0)//��
		{	
			if(character[num_byte]%(j*10)/j==0) character[num_byte]+=j*9;
			else  								character[num_byte]-=j;	
		}
		else
		{		
			if(character[num_byte]%(j*10)/j==9) character[num_byte]-=j*9;
			else                                character[num_byte]+=j;		
		}
				
		LCD6448_num_bit(record[num_byte][0],record[num_byte][1],character[num_byte],num_bit|(record[num_byte][2]&0xff),record[num_byte][3]);		
	}
	else
	{		
		j=record[num][3] & 0x7fff;
		if(ADD_SUB==0)//��
		{
			if(character[num]<=record[num][2])  character[num]=j;
			else 
			{    
				character[num]--;
			   if(character[num]==99||character[num]==999||character[num]==9999) i=1;
			}
		}
		else
		{
			if(character[num]>=j) 
			{	
				character[num]=record[num][2];
				while(j>0)
				{
					j/=10;
					i++;
				}	
				i-=2;
			}
			else  character[num]++;	
		}	
		
			j=record[num][3] & 0x8000;
		if(j==0x8000&&(record[num][3]&0x7fff)==character[num])//��ʾ����
		{
			i=(i+1)/2;	
			LCD6448_Scolour(colour_num,colour_window);//
			LCD6448_blank(record[num][0],record[num][1],32+i*32,32);//�������
			LCD6448_Scolour(colour_window,colour_num);//����
			LCD6448_Wstring(record[num][0],record[num][1],32,32,2,always_char);//����
		}
		else
		{	
			if(j==0x8000&&(character[num]==record[num][2]||character[num]==(record[num][3]&0x7fff)-1)) i++;//���Ǻ� Ҫ���һ���ֽڵ�����
			i=(i+1)/2;

			if((record[num][0] & 0xc000)==0)
			{
				LCD6448_Scolour(colour_num,colour_window);//
				LCD6448_blank(record[num][0],record[num][1],32+i*32,32);//�������
				LCD6448_Scolour(colour_window,colour_num);//����
				LCD6448_num_set(record[num][0],record[num][1],character[num],0);
			}
			else
			{
				LCD6448_Scolour(colour_num,colour_window);//
				LCD6448_blank(record[num][0],record[num][1],64+i*32,32);//�������
				LCD6448_Scolour(colour_window,colour_num);//����
				LCD6448_num_bit(record[num][0]&0x3fff,record[num][1],character[num],0,record[num][0]>>14);	
			}	
		}
	}
}

void LCD6448_num_32(unsigned int x,unsigned int y,unsigned int data_high,unsigned int data_low,unsigned int unit)//��ʾ32λ������
{	//unit����0���ޣ�1��A;2��V��3��-��4::5:H,6:z,7:W;
	 int i,j,k,n,m; 
     unsigned char data[10],data_num=0;
     unsigned char data_unit[3],data_unit_num=0;
     unsigned long record;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 
	 record=data_high;
	 
	 record=(record<<16)+data_low;
	 
	 while(record!=0)
	 {
			n=record%10; 
			data[data_num++]=n;
			record/=10;
	 }
	
	if(data_num==0) {data[0]=0;data_num=1;}
	
	 if((unit & 0x0f)!=0) 		data_unit[data_unit_num++]=10+(unit & 0x0f);//��ӷ��� 
	 if(((unit >> 4)&0x0f)!=0)  data_unit[data_unit_num++]=10+(((unit >> 4)&0x0f));//��ӷ��� 
	 if((unit >> 8)!=0)   		data_unit[data_unit_num++]=10+(unit >> 8);//��ӷ���    //�����������
	 

//	 for(k=0;k<data_num+data_unit_num;k++)
	 for(k=0;k<10;k++)
	 {
		   n=0;
		   m=y;
				 for(i=0;i<32;i++)
				 {       
						 LCD6448_Waddr(x+16*k,m++);
						 for(j=0;j<2;j++)
						 {
							if(k<data_num)
								 LCD6448_Wdata(Num16_32[(data[data_num-k-1]<<6)+n++]);//д�ַ���Ҫ  350ns
							else
							if(k<data_num+data_unit_num)
								 LCD6448_Wdata(Num16_32[(data_unit[k-data_num]<<6)+n++]);
							else 
								 LCD6448_Wdata(0x00);
							
						 }
			     }
	}
}
	
	
void LCD6448_num_e(unsigned int x,unsigned int y,unsigned int row,unsigned int tier,unsigned int record,unsigned int point,unsigned int num,const unsigned char *character)//��x,y����ʾrow*tier���ַ�
{                        //��ʾһ���ַ�,���治���ո��
						//�̶���λ
     unsigned int i=0,j,k=0,row1,l,n,m=10000;
     unsigned char data[8],data_num=0;
	 LCD6448_Wcmd(0x01);//�ַ�ģʽ
	 row1=row/8;//дһ�����˸���ַ

	 while(m!=0)
	 {			
			if(i==0) i=point/m; //�Ƿ���Ҫ��ǰ����
			n=record/m;  //���λ
			record=record%m;  //	
			
			if((n!=0)||(k!=0)||(i!=0))		//������λ��Ϊ��
			{ 
				if(k==0)  k=1;
				data[data_num++]=n;           //�����ݼ�������
				if((m==point)&&(point!=1)) data[data_num++]=10;//����С����
			}
			m/=10;
	 }	 
	 
	 l=row*tier/8;
	 
	 if(num<data_num) num=data_num;
	 	
	 for(k=0;k<num;k++)
	 {
			n=0;
			m=y;		
				 for(i=0;i<tier;i++)
				 {       
						 LCD6448_Waddr(x+row*k,m++);
						 for(j=0;j<row1;j++)
						 {
							if(k<data_num)
								 LCD6448_Wdata(character[data[k]*l+n++]);//д�ַ���Ҫ  350ns
							else
								 LCD6448_Wdata(0x00);
						 }
			     }
	}
}	

void LCD6448_SET_focus_char(unsigned int num,unsigned int num_up,unsigned int character[],const unsigned int record[][4],const unsigned int char_data[][2])
{//num���任���˵ڼ�����record��Ϣ��ŵĵط�
//	unsigned int place_bit;
	
	LCD6448_Scolour(colour_num,colour_window);//�����һ�ε�
	LCD6448_Wstring(record[num_up][0],record[num_up][1],32,32,2,char_data[character[num_up]]);//
	
	LCD6448_Scolour(colour_window,colour_num);//������һ�ε�
	LCD6448_Wstring(record[num][0],record[num][1],32,32,2,char_data[character[num]]);//
};

void LCD6448_Pnum_list(unsigned int row,unsigned int tier,unsigned num,unsigned int *record,const unsigned int place[][4],const unsigned char *character)//��x,y����ʾrow*tier���ַ�
{  //num��һ�����У�intervalÿ�м����٣� record����ʾ�����ݣ�place:��ʾ�ĵ�ַ,character:ʹ�õ���������
	unsigned int i,j,m,n;
	for(i=0;i<num;i++)
	{
		m=1,n=1;
		if((place[i][2]&0x8000)==0x8000)//
		{
		   for(j=1;j<(place[i][2]&0x0fff);j++) m*=10;//point
		   for(j=0;j<place[i][3];j++) 		   n*=10;
			
		}
		else
		if((place[i][0]& 0xc000)!=0)
		{
			m=place[i][0]>>14;
			for(j=0;j<m;j++) n*=10;//point
			m=1;
		}				
		LCD6448_num(place[i][0],place[i][1],row,tier,record[i],n,m,0,character);//��x,y����ʾrow*tier���ַ�
	}
}

//****ָʾ��**********************************************************************************
void LCD6448_Blight(unsigned int x,unsigned int y,unsigned int colour)//��ʾһ��32*32��ָʾ�ƣ�
{//colour��0����ɫ224��0x80ǳ��ɫ236��1����ɫ24��0x81��ǳ��ɫ28��2����ɫ252��0x82ǳ��ɫ254��3:��ɫ219��
  	unsigned int i,j,n=0;
			 if(colour==0) 	  colour=224;
		else if(colour==0x80) colour=236;//��ɫ  			 
		else if(colour==1)	  colour=24;
		else if(colour==0x81) colour=28;//��ɫ
		else if(colour==2)    colour=252;
		else if(colour==0x82) colour=254;//��ɫ
		else if(colour==3)	  colour=219;//��ɫ
		else if(colour==0x83) colour=219;//
		else if(colour==4)    colour=224;//��ɫ
		else if(colour==0x84) colour=236;//
		
		LCD6448_Fcolour(colour);
		LCD6448_Wcmd(0x01);//�ַ�ģʽ
		for(i=0;i<32;i++)
		{       
			LCD6448_Waddr(x,y++);
			for(j=0;j<4;j++)
				LCD6448_Wdata(LIGHT32_32[n++]);//��������ط���
		}
};

void LCD6448_Blight_three(unsigned int place,unsigned int colour)//�������̶���λ����ʾ��ָʾ��
{//0:ͨ��ָʾ�ƣ�1:״ָ̬ʾ�ƣ�2������ָʾ��
	unsigned int x,y;
		 if(place==0){x=20;y=440;}
	else if(place==1){x=190;y=440;}
	else if(place==2){x=360;y=440;}
	LCD6448_Blight(x,y,colour);
}	

void LCD6448_Slight(unsigned int place,unsigned int colour)//�ڰ˸��̶���λ����ʾСָʾ��
{//0:ͨ��ָʾ�ƣ�1:״ָ̬ʾ�ƣ�2������ָʾ��
	unsigned int x=2,y,i,j,n=0;
	
	  		 if(colour==0) 	  colour=128;
		else if(colour==0x80) colour=228;//��ɫ  			 
		else if(colour==1)	  colour=48;
		else if(colour==0x81) colour=28;//��ɫ
		else if(colour==2)    colour=208;
		else if(colour==0x82) colour=252;//��ɫ
		else if(colour==3)	  colour=141;//��ɫ
		else if(colour==0x83) colour=219;//��ɫ
		else if(colour==4) 	  colour=43;//��ɫ
		LCD6448_Fcolour(colour);
		LCD6448_Wcmd(0x01);//�ַ�ģʽ
		y=50+(place<<5);
		for(i=0;i<16;i++)
		{       
			LCD6448_Waddr(x,y++);
			for(j=0;j<2;j++)
				LCD6448_Wdata(LIGHT16_16[n++]);//��������ط���
		}
}
	
void LCD6448_design_time(unsigned int num)
{
	unsigned int i;
	if(num==0)//��һ��
	{
		LCD6448_Scolour(colour_window,colour_num);//����
		i=num>>3;
		LCD6448_num_e(40+(num-(i<<3))*60,340+i*20,8,16,num*2+1,1,2,Num8_16);
	}
	else
	if(num==24)//���һ��
	{
		LCD6448_Scolour(colour_num,colour_window);//�����һ�ε�
		i=(num-1)>>3;
		LCD6448_num_e(40+((num-1)-(i<<3))*60,340+i*20,8,16,(num-1)*2+1,1,2,Num8_16);
	}
	else//�м�
	{
		LCD6448_Scolour(colour_num,colour_window);//�����һ�ε�
		i=(num-1)>>3;
		LCD6448_num_e(40+((num-1)-(i<<3))*60,340+i*20,8,16,(num-1)*2+1,1,2,Num8_16);
		
		LCD6448_Scolour(colour_window,colour_num);//����
		i=num>>3;
		LCD6448_num_e(40+(num-(i<<3))*60,340+i*20,8,16,num*2+1,1,2,Num8_16);	
	}	
}

void LCD6448_design_time_five(unsigned int num)
{
	if(num==0)//��һ��
	{
		LCD6448_Scolour(colour_window,colour_num);//����
		LCD6448_num_e(40+num*60,380,8,16,num+1,1,2,Num8_16);
	}
	else
	if(num==record_single[3][1])//���һ��
	{
		LCD6448_Scolour(colour_num,colour_window);//�����һ�ε�
		LCD6448_num_e(40+(num-1)*60,380,8,16,num,1,2,Num8_16);
	}
	else//�м�
	{
		LCD6448_Scolour(colour_num,colour_window);//�����һ�ε�
		LCD6448_num_e(40+(num-1)*60,380,8,16,num,1,2,Num8_16);
		
		LCD6448_Scolour(colour_window,colour_num);//����
		LCD6448_num_e(40+num*60,380,8,16,num+1,1,2,Num8_16);	
	}	
}
     
         
                
                