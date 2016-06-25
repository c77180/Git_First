#include "..\include\p33FJ128GP708A.h"
#include "..\include\dspicExtern.h"
#include "..\include\character_h.h"

void stopWdog(void)//��ֹ���Ź�
{
	_SWDTEN=0;
}

void startWdog(void)//ʹ�ܽ�ֹ���Ź�
{
	_SWDTEN=1;
}

void clearWdog(void) // ι��
{
	ClrWdt();	
}

void oscConfig(void) //��������
{
  
//  OSCTUN=0;//����Ƶ��7.37M
//  PLLFBD=38;  //M=40
//  PLLFBD=41;  //M=43

	PLLFBD=30;  //M=32
  
//CLKDIV
  _DOZEN=0;    //������ʱ�ӱ�1:1
  _PLLPOST=0; //N2=2
  _PLLPRE=0;  //N1=2
  
  
 //OSCCON
//   _COSC=3;//��ǰ������PLL�������� (ֻ��)
//   _NOSC=3;//��
   _CLKLOCK=0;//δ����
   _LPOSCEN=0;//��ֹ��������
   
   	__builtin_write_OSCCONH(0x03);		/*	Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)*/
	__builtin_write_OSCCONL(0x01);
	while (OSCCONbits.COSC != 0b011);	/*	Wait for Clock switch to occur	*/
	while(!OSCCONbits.LOCK);

}

void clearIntrflags(void)
{/* Clear Interrupt Flags */

	IFS0=0;
	IFS1=0;
	IFS2=0;
	IFS3=0;
	IFS4=0;
}





