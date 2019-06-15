#include "variables.h"
/***************************
 ******** PEC 计算  ********
 ***************************/
/*PEC calculator*/
int pec15Table[256];
int CRC15_POLY = 0x4599;
int remainder;
void init_PEC15_Table()
{
	int i, bit;
	for (i = 0; i < 256; i++)
	{
		remainder = i << 7;
		for (bit = 8; bit > 0; --bit)
		{
			if (remainder & 0x4000)
			{
				remainder = ((remainder << 1));
				remainder = (remainder ^ CRC15_POLY);
			}
			else
			{
				remainder = ((remainder << 1));
			}
		}
		pec15Table[i] = remainder & 0xFFFF;
	}
}

unsigned int pec15(unsigned char *data, int len)
{
	int remainder, address, i;
	remainder = 16;//PEC seed
	for (i = 0; i < len; i++)
	{
		address = ((remainder >> 7) ^ data[i]) & 0xff;  //calculate PEC table address
		remainder = (remainder << 8) ^ pec15Table[address];
	}
	return (remainder * 2);//The CRC15 has a 0 in the LSB so the final value must be multiplied by 2
}

/********************************
 ******** PEC 校验  ********
********************************/

//PEC校验
byte PECcheck(int size){ 
  RecPEC = 0;
  RecPEC = (Rec[size-2]<<8)+Rec[size-1];  //计算接收到PEC码
  if(RecPEC == pec15(Rec,size-2))   //与数据理论PEC码比较
    return 0;
  else return 1;
}