char checkFlag;
char intervalFlag;    //�������ڱ�־
char convertFlag;  //ת���ȴ���־

#define StartMonitor 0x01 //����ѹ���¶Ȳ�����
#define PerioCheck  0x02  //���ڼ��
#define DisplayData 0x03  //��PC��ʾ��ѹ�¶ȣ���CAN���������PC
#define ReadReg 0x04  //���ؼĴ���������Ϣ
#define Break 0x05    //ֹͣ���

//SPIͨ�ű���
word Num;   //�洢�����ֽڸ���
unsigned char SenState;   //�洢����״̬
unsigned char RecState;    //�洢����״̬
char SPIsenFlag;    //SPI������ɱ�־λ

//CANͨ�ű���
byte CANrecFlag;
byte CANsenFlag;
byte CANsenData[8];
byte CANrecData[8];
dword CANrecID;
byte CANrecLen;
byte CANstate;

unsigned char Rec[8];   //������������
unsigned char ResA[8];  //�Ĵ���A
unsigned char ResB[8];  //...B
unsigned char ResC[8];  //...C
unsigned char ResD[8];  //...D

//��ѹ��оƬ�ţ���ر��)
//��·״̬���
word openStatus;
word cellPU1[12];
word cellPD1[12];

word cellPU2[12];
word cellPD2[12];

int cellDelt1[12];
int cellDelt2[12];

word cellVol1[12];  //оƬ1���������ص�ѹ
word cellVol2[12];  //оƬ2���������ص�ѹ

word test[12] = {0,1,2,3,4,5,6,7,8,9,10,11};


//GPIO_ADC �¶�
word L = 0;      //���ַ������¶ȵ�����
word H = 50;   
word DisTemp[2];  //ת�����͵��¶� 

word cellTemp1[5];   //GPIO 1~5
word cellTempREF1;  //�ڶ���׼��ѹ

word cellTemp2[5];
word cellTempREF2;  //�ڶ���׼��ѹ

//���ƼĴ�������
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
