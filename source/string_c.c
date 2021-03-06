
//串口屏**********
const unsigned int ERR_code_com[][10]=    //串口屏故障代码
{
 {0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},   //0空着
 {0xB9A6,0xC2CA,0xC4A3,0xBFE9,0xB9CA,0xD5CF,0x2020,0x2020,0x2020,0x2020},	//0x0001:1功率模块故障
 {0xD7B0,0xD6C3,0xB9FD,0xC1F7,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},   //0x0002:2装置过流
 {0xCFB5,0xCDB3,0xBDBB,0xC1F7,0xB9FD,0xD1B9,0x2020,0x2020,0x2020,0x2020},	//0x0004:3系统交流过压
 {0xD6B1,0xC1F7,0xB2E0,0xB9FD,0xD1B9,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x0008:4直流侧过压
 {0xB5E7,0xCDF8,0xCDAC,0xB2BD,0xB4ED,0xCEF3,0x2020,0x2020,0x2020,0x2020},	//0x0010:5电网同步错误
 {0xD4A4,0xB3E4,0xB5E7,0xB3AC,0xCAB1,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x0020:6预充电超时
 {0xCFB5,0xCDB3,0xBDBB,0xC1F7,0xC7B7,0xD1B9,0x2020,0x2020,0x2020,0x2020},	//0x0040:7系统交流欠压
 {0xBFB4,0xC3C5,0xB9B7,0xB3AC,0xCAB1,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x0080:8看门狗超时
 {0xC4E6,0xB1E4,0xCAE4,0xB3F6,0xB3F6,0xB4ED,0x2020,0x2020,0x2020,0x2020},	//0x0100:9逆变输出出错
 {0xC8B1,0xCFE0,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x8000:10缺相
 {0xCFE0,0xD0F2,0xB4ED,0xCEF3,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x4000:11相序错误
 {0xC7C5,0xB1DB,0xB2BB,0xC6BD,0xBAE2,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x2000:12桥臂不平衡
 {0xB9FD,0xCEC2,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x1000:13过温
 {0xBCB1,0xCDA3,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x1001:14急停 
 {0xC9A2,0xC8C8,0xC6F7,0xB9FD,0xCEC2,0x2020,0x2020,0x2020,0x2020,0x2020},	//0x1002:15散热器过温
 {0xBBA5,0xB8D0,0xC6F7,0xB5E7,0xC1F7,0xB3AC,0xCFDE,0x2020,0x2020,0x2020},   //16 互感器电流超限
 {0xC4A3,0xBFE9,0xC8B1,0xCAA7,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},   //17模块缺失
};

const unsigned int MODE_num_com[][4]=//设备运行状态
{
	{0x2020,0x2020,0x2020,0x2020},//空的
	{0xC4A3,0xBFE9,0xD2BB,0x3a20},//模块一
	{0xC4A3,0xBFE9,0xB6FE,0x3a20},//模块二
	{0xC4A3,0xBFE9,0xC8FD,0x3a20},//模块三
	{0xC4A3,0xBFE9,0xCBC4,0x3a20},//模块四
	{0xC4A3,0xBFE9,0xCEE5,0x3a20},//模块五
	{0xC4A3,0xBFE9,0xC1F9,0x3a20},//模块六
	{0xC4A3,0xBFE9,0xC6DF,0x3a20},//模块七
	{0xC4A3,0xBFE9,0xB0CB,0x3a20},//模块八
};



const unsigned int MESSAGE_com[][12]=//提示显示
{
	{0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},//空
	{0xC3DC,0xC2EB,0xCAE4,0xC8EB,0xB4ED,0xCEF3,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020},//1 密码输入错误
	{0xB9CA,0xD5CF,0xBCC7,0xC2BC,0xD2D1,0xBEAD,0xC7E5,0xBFD5,0x2020,0x2020,0x2020,0x2020},//2 故障记录已经清空
	{0xCFD4,0xCABE,0xBDE7,0xC3E6,0xD2D1,0xBEAD,0xBBD6,0xB8B4,0xB3F6,0xB3A7,0xC9E8,0xD6C3},//3 显示界面已经恢复出厂设置
	{0xC4FA,0xCAE4,0xC8EB,0xB5C4,0xB2CE,0xCAFD,0xB3AC,0xB3F6,0xC9E8,0xD6C3,0xB7B6,0xCEA7},//4 您输入的参数超出设置范围
	{0xC4FA,0xCAE4,0xC8EB,0xB5C4,0xB2CE,0xCAFD,0xB5CD,0xD3DA,0xC9E8,0xD6C3,0xB7B6,0xCEA7},//5 您输入的参数低于设置范围
	{0xC8E7,0xD0E8,0xCDA3,0xD6B9,0xC9E8,0xB1B8,0x2c20,0xC7EB,0xB0B4,0xCFC2,0xBCB1,0xCDA3},//6 如需停止设备,请按下急停

};

const unsigned int PARAMETER1_range[][2]=//范围
{
	{0,1},   //是否是三相四线
	{0,9999},//负载侧互感器规格
	{0,9999},//系统侧互感器规格
	{0,9999},//额定容量
	{0,9999},//补偿无功值
	{0,1},   //是否指定补偿
	{0,1},   //备用
	{0,1},   //
	{0,1},	 //
};

const unsigned int PARAMETER2_range[][2]=//范围
{
	{0,200},//电流比例系数
	{0,10000},//电流积分常数
	{0,200},//电压比例系数
	{0,200},//电压积分常数
	{0,200},//电容中心比例系数
	{0,200},//电容中心积分常数
	{600,800},//直流侧设定值
	{0,200},//电流外环比例系数
	{0,10000},//电流外环积分常数	
};	

const unsigned int PARAMETER3_range[][2]=//范围
{
	{0,32767},//直流电压检测比
	{0,32767},//电网电压检测比
	{0,20000},//负载电流检测比
	{0,9999},//反馈电流检测比
	{0,500},//电感值
	{0,16},//输出补偿比例
	{0,9999},//显示容量
	{0,16},//中线限容倍数
	{20,50},//互感器电流上限	
};

const unsigned int PARAMETER4_range[][2]=//范围
{
	{130,220},//电网欠压值
	{220,270},//电网过压值
	{0,9999},//电流过流保护
	{600,850},//直流侧预过压
	{650,900},//直流侧实过压
	{350,450},//电容过压保护
	{30,100},//电容欠压保护
	{0,1},//是否断电自启
	{0,1},//备用	
};