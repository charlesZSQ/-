char checkFlag;
char intervalFlag;    //测量周期标志
char convertFlag;  //转换等待标志

#define StartMonitor 0x01 //监测电压和温度并返回
#define PerioCheck  0x02  //周期检测
#define DisplayData 0x03  //在PC显示电压温度，即CAN传输参数到PC
#define ReadReg 0x04  //返回寄存器配置信息
#define Break 0x05    //停止检测

//SPI通信变量
word Num;   //存储发送字节个数
unsigned char SenState;   //存储发送状态
unsigned char RecState;    //存储接收状态
char SPIsenFlag;    //SPI发送完成标志位

//CAN通信变量
byte CANrecFlag;
byte CANsenFlag;
byte CANsenData[8];
byte CANrecData[8];
dword CANrecID;
byte CANrecLen;
byte CANstate;

unsigned char Rec[8];   //接收数据数组
unsigned char ResA[8];  //寄存器A
unsigned char ResB[8];  //...B
unsigned char ResC[8];  //...C
unsigned char ResD[8];  //...D

//电压（芯片号，电池编号)
//开路状态检查
word openStatus;
word cellPU1[12];
word cellPD1[12];

word cellPU2[12];
word cellPD2[12];

int cellDelt1[12];
int cellDelt2[12];

word cellVol1[12];  //芯片1测量单体电池电压
word cellVol2[12];  //芯片2测量单体电池电压

word test[12] = {0,1,2,3,4,5,6,7,8,9,10,11};


//GPIO_ADC 温度
word L = 0;      //二分法计算温度的坐标
word H = 50;   
word DisTemp[2];  //转换后发送的温度 

word cellTemp1[5];   //GPIO 1~5
word cellTempREF1;  //第二基准电压

word cellTemp2[5];
word cellTempREF2;  //第二基准电压

//控制寄存器参数
struct ConReg{
  byte GPIO;
  byte REFON;
  byte SWTRD;
  byte DCTO;  
  byte ADCOPT;
  word VUV;
  word VOV;
  word DCC;
}ConReg[2];


byte PECflag;
word RecPEC;
