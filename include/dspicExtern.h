
extern volatile unsigned int yuan1;
extern volatile unsigned int zhongyuan;//������

extern volatile unsigned int soft_APF_SVG[][2];//�汾��
//**********ѡ�����************************************
#define compilemodbus 0
#define compileGSM 1
//************** main ***************************
extern volatile int record_wave[12][64];
extern volatile int record_single[][3];

extern volatile unsigned char __attribute__((__far__))  LCD_Rbuffer[256];
extern volatile unsigned char __attribute__((__far__))  LCD_Tbuffer[1200];
extern volatile unsigned int LCD_Rbuffer_now,LCD_Rbuffer_num,LCD_Rbuffer_sign;
extern volatile unsigned int LCD_Tbuffer_now,LCD_Tbuffer_num,LCD_Tbuffer_sign;

extern volatile int __attribute__((__far__))  record_wave_b[64];//����
extern volatile int __attribute__((__far__))  record_wave_b1[64];//��������
extern volatile int __attribute__((__far__))  record_wave_b2[64];//��������

extern volatile int __attribute__((__far__))  MAX_data[4][2];//0:Ƶ�Σ�1:��Чֵ
//****************************************************************************
//****************************************************************************
extern volatile unsigned long DELAY_BLIGHT_SIGN;
extern volatile unsigned int WIRE_cost[][3];
extern volatile unsigned int LOAD_cost[][3];//����������ʾ�ڽ����ϵĵ�����

extern volatile unsigned int WIRE_cost_buffer[7][3][22];//APF ϵͳ����
extern volatile unsigned int LOAD_cost_buffer[9][3][22];//APF ��������

extern volatile unsigned int APF_state;//�豸����״̬ 0ͣ�� 1�����У�2������3���У�4���ϣ�
extern volatile unsigned int APF_error;//0���޹��ϣ�1���й���
extern volatile unsigned int APF_CAN;//0:CANͨ���նˣ�1:CANͨѶ����
extern volatile unsigned int P_E2PROM[];//��ʼ���Ĳ�����

extern volatile unsigned int system_parameter_b[45];
extern volatile unsigned int protect_parameter_b[9];
extern volatile unsigned int hide_parameter_b[3][4];
extern volatile unsigned int equipment_skate[8][4];//�豸��״̬�͹��ϴ���
extern volatile unsigned int equiment_set[];//���صĸ���
extern volatile unsigned int equipment_ID[8][4];
extern volatile unsigned int electric_receive_sign;//���������ձ�ʶ
extern volatile unsigned int parameter_receive_sign;//�������ñ�ʶ
extern volatile unsigned int parameter_receive_SMSsign;

extern volatile unsigned int GSM_NUM_fist;


//*********AUTO_run**************************
extern volatile unsigned int  AUTO_run;
extern volatile unsigned int  AUTO_stop;
extern volatile unsigned int  which_equ;
extern volatile unsigned long ask_time;
void DS1302_AUTO_RUN_STOP(void);
void page_state(void);


//**********DSPIC33F_sys*******************
#define start_GSM LATBbits.LATB10=1
#define stop_GSM  LATBbits.LATB10=0 
void stopWdog(void);
void startWdog(void);
void clearWdog(void);
void oscConfig(void);
void clearIntrflags(void);
void send_elect(void);


//*****E2PROM************************
void E2ROM_93LC86C_Config(void);
void E2ROM_93LC86C_WCommand(unsigned int data);
void E2ROM_93LC86C_Wdata(unsigned int data);
void E2ROM_93LC86C_Busy(void);
int E2ROM_93LC86C_Read(void);
void E2ROM_93LC86C_writedata(unsigned int addr,int data);
int E2ROM_93LC86C_readdata(unsigned int addr);
unsigned int E2ROM_WR(unsigned int add,unsigned int num,volatile unsigned int *character);
void E2ROM_RD(unsigned int add,unsigned int num,volatile unsigned int *character);
void E2ROM_93LC86C_init();
void WRITE_fault_one(unsigned int fault,unsigned int mode);
void WRITE_fault_shadow(unsigned int fault,unsigned int mode);
void CLEAR_fault_all(unsigned int type);

extern volatile unsigned int __attribute__((__far__)) add_error_data[15][11];//���������0~9������Ϣ��10:��ַ��11��д��־
extern volatile unsigned int error_write,error_read;

//***********CAN1**************************************
void DMA_C1R(void);
void DMA_C1S(void);
void ecan1WriteRxAcptFilter(int n, long identifier, unsigned int exide, unsigned int bufPnt,unsigned int maskSel);
void ecan1WriteRxAcptMask(int m, long identifier, unsigned int mide, unsigned int exide);
void ecan1WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
void ecan1WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);
void ecan1DisableRXFilter(int n);
void Ecan1_Config(void);//CAN1ģ������
void CAN1_examp(void);


//*****************fft*************************
extern volatile unsigned int coordinate_now;
extern volatile int zero_wave[3][64];

void find_max(volatile int *data);
void find_max_plus(volatile int *data);
void fft_calculation (volatile int *data);
void fft_effective_all(void);
void fft_effective_one(void);
unsigned int effective_c(volatile int *data);
void ACCOUNT_data(void);
void ACCOUNT_Sdata(void);
void ACCOUNT_buffer_one(volatile unsigned int *data_buffer,volatile unsigned int *data);
void ACCOUNT_buffer(void);

void LCD_wave_calculation(volatile unsigned int coor,volatile int *data_dir,volatile int *data,volatile int *data_b,volatile int *data1,volatile int *data_b1,volatile int *data2,volatile int *data_b2,volatile int *data_n,volatile int *data_bn);
void LCD_histogram_calculation(volatile unsigned int coor,volatile int *data,volatile int *data_b,volatile int *data1,volatile int *data_b1,volatile int *data2,volatile int *data_b2);


//*********init******************************
void Init_data_buffer(void);
void Init_array(void);


//*************LED******************************
void DELAY(unsigned long time);
void LED_Config(void);
void LED_ray(void);//��ˮ�ƣ���ʱ�ã�ɾ��
void LED_light(unsigned char data);
void LED_light_ray(unsigned data);
unsigned char switch_num(void);


//**test**********************
extern volatile unsigned int TEST_SIGN;

void TEST_ALL(void);


//*******time1***************
#define start_time1() _TON=1    //������ʱ��	
#define stop_time1()  _TON=0;	  //������ʱ��	
extern volatile unsigned long DELAY_TIME;
extern volatile unsigned long DELAY_SLEEP;
extern volatile unsigned long DELAY_Blight;
extern volatile unsigned long DELAY_ASKTIME;
extern volatile unsigned long DELAY_DSIskate;//״̬��ʾ
extern volatile unsigned long DELAY_CAN[8];//CANͨѶ��ʱ
extern volatile unsigned long DELAY_run_time;//�����豸����ʱ��
extern volatile unsigned long DELAY_refresh_data;//����ˢ��ʱ��
extern volatile unsigned long DELAY_GSM;//   ��ʱ����******
extern volatile unsigned long DELAY_send_message;//������Ϣ��
extern volatile unsigned long DELAY_message;     //��ʾ��Ϣ��ʱ
extern volatile unsigned long DELAY_fist_page;//�ϵ����
extern volatile unsigned long DELAY_GSM_error;
extern volatile unsigned long DELAY_elect_synch;
extern volatile unsigned int  MODBUS_TIME;
extern volatile unsigned long DELAY_GSM_NUM;

extern volatile unsigned long DELAY_refresh_data_com;
extern volatile unsigned int run_time_sign;//����ʱ��ı��ʶ

//extern char R232_send_data[20];         //��ʱ����*****
extern volatile unsigned int R232_send_data_num; //��ʱ����*****
extern volatile unsigned int R232_send_data_now; //��ʱ����*****

void time1_config(unsigned int time);
void time2_config(unsigned int time);


//**********RS232***************************
#define RS232_Sfifo   U1STAbits.UTXBF //1�����ͻ��������������ٰ����ݷ��뷢�ͻ�����
#define RS232_Rfifo   U1STAbits.URXDA //1�����ջ����������ݣ��Ѿ��յ����ݣ�����ȥȡ���ջ�����
#define RS232_Sover	  U1STAbits.TRMT  //1�����ͼĴ���Ϊ�գ�һ�η������

#define RS232_Rbusy     U1STAbits.RIDLE //1:���������У�0��æ
#define RS232_PERR		U1STAbits.PERR  //1:��żУ�����
#define RS232_FERR		U1STAbits.FERR  //1:֡����
#define RS232_OERR		U1STAbits.OERR  //1:�����������
//	U1TXREG;	//���ͼĴ���
//	U1RXREG		//���ռĴ��� �ĸ�������
void RS232_Config(void);
void RS232dis_Config(void);
void RS232_Sdata(void);
void RS232_Rdata(void);


//**********RS232dis***************************
#define RS232dis_Sfifo   U2STAbits.UTXBF //1�����ͻ��������������ٰ����ݷ��뷢�ͻ�����
#define RS232dis_Rfifo   U2STAbits.URXDA //1�����ջ����������ݣ��Ѿ��յ����ݣ�����ȥȡ���ջ�����
#define RS232dis_Sover	  U2STAbits.TRMT  //1�����ͼĴ���Ϊ�գ�һ�η������

#define RS232dis_Rbusy     U2STAbits.RIDLE //1:���������У�0��æ
#define RS232dis_PERR		U2STAbits.PERR  //1:��żУ�����
#define RS232dis_FERR		U2STAbits.FERR  //1:֡����
#define RS232dis_OERR		U2STAbits.OERR  //1:�����������
//	U2TXREG;	//���ͼĴ���
//	U2RXREG		//���ռĴ��� �ĸ�������

void RS232dis_Config(void);
void RS232dis_Sdata(void);
void RS232dis_Rdata(void);

//************������*******************************
extern volatile unsigned long DELAY_refresh_data_com;

extern volatile unsigned int run_time_sign;//����ʱ��ı��ʶ

extern volatile unsigned int R232_send_data_num;
extern volatile unsigned int R232_send_data_now;

void time1_config(unsigned int time);

#if compileGSM
//********GSM***********
	extern volatile unsigned char __attribute__((__far__)) SOURCE[370];
	extern volatile unsigned char __attribute__((__far__)) SOURCE_data[200];
	extern volatile unsigned char  SEND_CMGS;
	
	extern volatile unsigned char __attribute__((__far__)) SOURCE_R_num[40];//���ն������ĺ���
	extern volatile unsigned int  R232_receive_Ndata_now;//���ն������ĺ���
	
	extern volatile unsigned char __attribute__((__far__)) SOURCE_R[370];//�������ݼ�غ���
	extern volatile unsigned char __attribute__((__far__)) SOURCE_data_R[160];//����
	extern volatile unsigned char  SEND_CMGS_R;
	
	extern volatile unsigned int GSM_send_num;//���ڿ���GSM��������
	extern volatile unsigned int GSM_record_sign;//�����жϷ��͵���������
	
	extern volatile unsigned char __attribute__((__far__)) R232_send_data[1000];
	extern volatile unsigned int R232_send_data_num;
	extern volatile unsigned int R232_send_data_now;
	extern volatile unsigned int R232_Send_sign;//�Ƿ����ڷ�������
	extern volatile unsigned int R232_receive_data_num;
	extern volatile unsigned int R232_receive_data_now;
	extern volatile unsigned int R232_Rbuffer_sign;
	
	extern volatile unsigned int  send_message_time;
	
unsigned char char_hex(unsigned char D_high,unsigned char D_low);
unsigned int char_int(unsigned int num,volatile unsigned char *character);
unsigned int add_data_string(unsigned int fist_num,unsigned int record,unsigned int point);
unsigned int add_num_string(unsigned int fist_num,unsigned int record,unsigned int num);
void add_num(void);
void send_message(void);
int GSMEncode7bit(volatile unsigned char *pDst,volatile unsigned char *pSrc, int nSrcLength);
int GSMDecode7bit(volatile unsigned char *pDst,volatile unsigned char *pSrc);
void GSM_handle(void);
#endif
//*****����������*******************
extern volatile unsigned int LCD_page_sign;//��ʼ��ʱ��ʲô������ʾ
extern volatile unsigned int LCD_page_dis;//�Ƿ��һ�ν���
extern volatile unsigned int LCD_page_dis_b;//��������
extern volatile unsigned int LCD_page_dis_bb;//��������
extern volatile unsigned int LCD_page_dis_bbb;//��������
extern volatile unsigned int LCD_page_dis_bbbb;//��������
extern volatile int *LCD_page_array;             //��ַ
extern volatile unsigned int LCD_page101;
extern volatile unsigned int LCD_page102;
extern volatile unsigned int LCD_page103;
extern volatile unsigned int LCD_page201;
extern volatile unsigned int LCD_page202;
extern volatile unsigned int LCD_page203;
extern volatile unsigned int LCD_page204;
extern volatile unsigned int LCD_page205;
extern volatile unsigned int LCD_page206;
extern volatile unsigned int LCD_page301;
extern volatile unsigned int LCD_page401;
extern volatile unsigned int LCD_page501;
extern volatile unsigned int LCD_page502;
extern volatile unsigned int LCD_page503;
extern volatile unsigned int LCD_page504;
extern volatile unsigned int LCD_page505;
extern volatile unsigned int LCD_page601;
extern volatile unsigned int LCD_page602;
extern volatile unsigned int LCD_page603;
extern volatile unsigned int LCD_page604;
extern volatile unsigned int LCD_page605;
extern volatile unsigned int LCD_page606;
extern volatile unsigned int LCD_page701;
extern volatile unsigned int LCD_page702;
extern volatile unsigned int LCD_page703;
extern volatile unsigned int LCD_Ddata_buffer[255];//���ڲ�����ʾ����ʹ��
extern volatile unsigned char __attribute__((__far__))  LCD_Ddata_tran[20];//���ڹ���ʹ��
extern volatile unsigned int KEY_com;//���ܰ�ť
extern volatile unsigned int ERROR_num;//���ϵ���Ŀ���ڹ�����Ŀ�ı��ʱ����ʾҪ��֮�ı� ������
extern volatile unsigned int change_data;//���ݸı�ʱ���±���
extern volatile unsigned int change_data_only;
extern volatile unsigned int message_num;//��ʾ��Ϣ�������е�λ��
extern volatile unsigned int LED_state;//���⵱ǰ��״̬���ϵ�Ĭ��Ϊ�ǿ���;
extern volatile unsigned int equipment_skate_sign;//�ϵ���ʾ����  �������߸���
extern volatile unsigned int RUN_stop;//����ֹͣ���е�ҳ��
extern volatile unsigned int TIME_NOW[];//��ǰʱ�� 0:�꣬1���£�2�գ�3����  4ʱ 5��
extern volatile unsigned int fault_data_num;//��ǰ��ʾ�Ĺ��ϴ���ĵ�ַ,
extern volatile unsigned long password_one;//��һ������
extern volatile unsigned long password_two;//�ڶ�������
extern volatile unsigned char K_num;//���뿪��״̬
extern volatile unsigned int  equipment_activate;
extern volatile unsigned int  ALL_cap;

//*****�ײ㷢�ͽ���**********************************************
void RS232_Slcd(void);
void RS232_Rlcd(void);
//*****************���մ���**************************************************************************

void RS232_Rlcd_button(void);
//*******���ݴ���**************************************************************************

void LCD_page_101(void);
void LCD_page_102(void);
void LCD_page_103(void);
void LCD_page_201(void);
void LCD_page_202(void);
void LCD_page_203(void);
void LCD_page_204(void);
void LCD_page_205(void);
void LCD_page_206(void);
void LCD_page_301(void);
void LCD_page_401(void);
void LCD_page_501(void);
void LCD_page_502(void);
void LCD_page_503(unsigned int page,unsigned int order);
void LCD_page_504(void);
void LCD_page_505(void);
void LCD_page_601(unsigned int page,unsigned int order);
void LCD_page_602(unsigned int page,unsigned int order);
void LCD_page_606(unsigned int page,unsigned int order);
void LCD_page_701(void);
void LCD_page_702(void);
void LCD_page_703(void);
void LCD_page_run(void);


//**RS485***********************
#define CN_RBR        0x0000   //�� ���ջ�������ַ
#define CN_THR        0x0200   //д ���ͻ������ĵ�ַ


#define CN_IER_reset  0x0680  //д ��λ����  ��ֹ�����ж�
#define CN_IER_read   0x0400  //�� ���ж��Ƿ�ʹ�ܻ��ֹ   ����

#define CN_IIR_read   0x0800  //ֻ�� �ж�״̬��־��ַ�������λC˵��fifi�Ѿ�����
							  //0x1���жϣ�0x6���յ���·״̬ ,0x4�������ݴﵽFIFO�趨��ֵ��
							  //0xC 4������ʱ��δ�յ�����  2���ͼĴ����� 
							  
#define CN_FCR_write  0x0a07  //ֻд  ���2λ:�յ�1,4,8,14�����ݴ����ж�  	     
							  //����һ�ֽڴ����жϣ���շ��ͽ���fifo����λfifo������FIFO
							  
#define CN_MCR_write  0x1200  //д 
#define CN_MCR_read   0x1000  //�� 5:1����CTS,RTS;4:1����ģʽ��3:1���������ж������2:1�Զ��壬1:1RTS���������Ч��0:1DTR���������Ч

#define CN_LCR_write_hz 0x0E80  //д ����д������ ��ʼʱ��      8���� żУ�飬1ֹͣ
#define CN_LCR_write    0x0E00  //д ��ֹд�����ʣ����Զ�ȡ���� 8���� żУ�飬1ֹͣ
#define CN_LCR_read     0x0c00	//�� ������  ����

#define CN_LSR        0x1400    //�� ��·״̬��0��1����fifo�������ݣ���ȡ����
                                          // 1��1����fifo���
                                          // 2��1fifoУ�����
                                          // 3��1fifo֡����ȱ��ֹͣλ
                                          // 4��1��⵽��·���
                                          // 5��1���ͱ���THR��
                                          // 6��1THR,TSRȫ���գ�����ȫ������
                                          // 7��1��·��������һ���Ĵ���
  
//�ڲ�1.8432                                             
#define CN_DLL        0X0200   //д ������ 9600
#define CN_DLM        0X0600   //д ������ 9600

#define SS1			  		_LATB2
#define SPI_send_over 		SPI1STATbits.SPITBF   //���ͻ�������״̬1
#define SPI_receive_over 	SPI1STATbits.SPIRBF  //���ջ�������״̬1

extern volatile unsigned int CH432T_reset;// Ϊ1 �򴮿ڸ�λ
extern volatile unsigned int RS485_receive;// DATARDY Ϊ1 ��˵�������ݿ��Զ���  0���յ�����
extern volatile unsigned int RS485_send_fifo;// THRE 1���Լ�����fifo�������ݣ�0,fifo���� 16��
extern volatile unsigned int RS485_send_over;// TEMT 1����ȫ�����꣬ 0
extern volatile unsigned int BSP_reset;
extern volatile unsigned int  MODBUS_T1_5;  
extern volatile unsigned int  MODBUS_T3_5; 

void RS485_Config(void);
void CH432T_config(void);



//**********modbus************************************
//#define   MODBUS_T1_5  20    //50*100um=2ms����ʱ    ʵ��Ӧ���� 1000/(9600/11)*1.5=1.718ms;
//#define   MODBUS_T3_5  40    //90*100um=3.51ms����ʱ ʵ��Ӧ���� 1000/(9600/11)*3.5=4.01ms;
#if compilemodbus
#define ELECTRIC_N  250   //������
#define PARAMETER_N 9   //����

//extern char MODBUS_ID;
extern volatile int  __attribute__((__far__))  Modbus_electric[ELECTRIC_N];			//  ������  30001~~39999
extern volatile int  __attribute__((__far__))  Modbus_parameter[PARAMETER_N][2];		// ����    40001~~49999
extern volatile unsigned int Modbus_write_sign;//�Ƿ��п��������߲�����д��


void modbus_init();
void modbus_change();
void modbus_in();
void modbus_Shandle(void);

#endif
