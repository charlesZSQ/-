/*数据和命令*/          
const unsigned char empty[4] = {0x00,0x00,0x00,0x00};
/*chip1 0011; chip2 1100*/
//写命令
const unsigned char WRCFG1[4] = {0x98,0x01,0xd2,0x4a};
const unsigned char WRCFG2[4] = {0xe0,0x01,0xac,0xec};
//写控制寄存器
const unsigned char CFGR1[8] = { 0xfc,0x00,0x00,0x00,0x00,0x00,0x4f,0x82 };
const unsigned char CFGR2[8] = { 0xfc,0x00,0x00,0x00,0x00,0x00,0x4f,0x82 };
//清除电池电压寄存器数据
const unsigned char CLRCELL1[4] = {0x9f,0x11,0x26,0xe4};    //清除电压寄存器
const unsigned char CLRCELL2[4] = {0xe7,0x11,0x58,0x42};
//清除辅助寄存器内容
const unsigned char CLRAUX1[4] = {0x9F,0x12,0x30,0x80};
const unsigned char CLRAUX2[4] = {0xE7,0x12,0x4e,0x26};
//电池电压ADC转换
const unsigned char ADCV1[4] = {0x9B,0x60,0x1b,0x48};  //7kHz模式，放电未得到允许，全部电池
const unsigned char ADCV2[4] = {0xe3,0x60,0x65,0xee};  //7kHz模式，放电未得到允许，全部电池
//GPIO ADC转换
const unsigned char ADAX1[4] = {0x9d,0x60,0x3c,0x84};  //7kHz模式，放电未得到允许，全部电池
const unsigned char ADAX2[4] = {0xe5,0x60,0x42,0x22};  //7kHz模式，放电未得到允许，全部电池
//开路检查
const unsigned char ADOWPU1[4] = {0x9b,0x68,0xf3,0x46};
const unsigned char ADOWPU2[4] = {0xe3,0x68,0x8d,0xe0};
const unsigned char ADOWPD1[4] = {0x9b,0x28,0x14,0xcc};
const unsigned char ADOWPD2[4] = {0xe3,0x28,0x6a,0x6a};
//读取状态寄存器命令
const unsigned char RDSTATA1[4] = {0x98,0x10,0x02,0x56};
const unsigned char RDSTATA2[4] = {0xe0,0x10,0x7c,0xf0};
const unsigned char RDSTATB1[4] = {0x98,0x12,0x9f,0x00};
const unsigned char RDSTATB2[4] = {0xe0,0x12,0xe1,0xa6};
//读取控制寄存器内容
const unsigned char RDCFG1[4] = {0x98,0x02,0xc4,0x2e};
const unsigned char RDCFG2[4] = {0xe0,0x02,0xba,0x88};
//读取电池电压寄存器组A，B，C，D
const unsigned char RDCVA1[4] = {0x98,0x04,0xe8,0xe6}; 
const unsigned char RDCVA2[4] = {0xe0,0x04,0x96,0x40};
const unsigned char RDCVB1[4] = {0x98,0x06,0x75,0xb0};
const unsigned char RDCVB2[4] = {0xe0,0x06,0x0b,0x16};
const unsigned char RDCVC1[4] = {0x98,0x08,0xb1,0x76};
const unsigned char RDCVC2[4] = {0xe0,0x08,0xcf,0xd0};
const unsigned char RDCVD1[4] = {0x98,0x0a,0x2c,0x20};
const unsigned char RDCVD2[4] = {0xe0,0x0a,0x52,0x86};
//读取辅助寄存器组A.B
const unsigned char RDAUXA1[4] = {0x98,0x0c,0x00,0xe8};
const unsigned char RDAUXA2[4] = {0xe0,0x0c,0x7e,0x4e};
const unsigned char RDAUXB1[4] = {0x98,0x0e,0x9d,0xbe};
const unsigned char RDAUXB2[4] = {0xe0,0x0e,0xe3,0x18};