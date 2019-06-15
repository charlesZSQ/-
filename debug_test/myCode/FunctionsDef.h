#include"comAndData.h"
#include "LTC6804.h"
#include "temptureTable.h"
#include "math.h"
extern word DisTemp[2];
extern word H;
extern word L;

/****************************************
 ************ ���ݴ����� ******************
 ****************************************/
 /***** �����ѹ���� ******/
void getVolVal(int chipNum){
  switch(chipNum){
    case 1:
      cellVol1[0] = (ResA[1]<<8) + ResA[0];
      cellVol1[1] = (ResA[3]<<8) + ResA[2];
      cellVol1[2] = (ResA[5]<<8) + ResA[4];
      cellVol1[3] = (ResB[1]<<8) + ResB[0];
      cellVol1[4] = (ResB[3]<<8) + ResB[2];
      cellVol1[5] = (ResB[5]<<8) + ResB[4];
      cellVol1[6] = (ResC[1]<<8) + ResC[0];
      cellVol1[7] = (ResC[3]<<8) + ResC[2];
      cellVol1[8] = (ResC[5]<<8) + ResC[4];
      cellVol1[9] = (ResD[1]<<8) + ResD[0];
      cellVol1[10] = (ResD[3]<<8) + ResD[2];
      cellVol1[11] = (ResD[5]<<8) + ResD[4];
      break;
    case 2:
      cellVol2[0] = (ResA[1]<<8) + ResA[0];
      cellVol2[1] = (ResA[3]<<8) + ResA[2];
      cellVol2[2] = (ResA[5]<<8) + ResA[4];
      cellVol2[3] = (ResB[1]<<8) + ResB[0];
      cellVol2[4] = (ResB[3]<<8) + ResB[2];
      cellVol2[5] = (ResB[5]<<8) + ResB[4];
      cellVol2[6] = (ResC[1]<<8) + ResC[0];
      cellVol2[7] = (ResC[3]<<8) + ResC[2];
      cellVol2[8] = (ResC[5]<<8) + ResC[4];
      cellVol2[9] = (ResD[1]<<8) + ResD[0];
      cellVol2[10] = (ResD[3]<<8) + ResD[2];
      cellVol2[11] = (ResD[5]<<8) + ResD[4];
      break;
  }
}

/***** �����¶����� ******/
void getTempVal(int chipNum){
  switch(chipNum){
    case 1:
      cellTemp1[0] = (ResA[1]<<8) + ResA[0];
      cellTemp1[1] = (ResA[3]<<8) + ResA[2];
      cellTemp1[2] = (ResA[5]<<8) + ResA[4];
      cellTemp1[3] = (ResB[1]<<8) + ResB[0];
      cellTemp1[4] = (ResB[3]<<8) + ResB[2];
      cellTempREF1 = (ResB[5]<<8) + ResB[4];
      break;
    case 2:
      cellTemp2[0] = (ResA[1]<<8) + ResA[0];
      cellTemp2[1] = (ResA[3]<<8) + ResA[2];
      cellTemp2[2] = (ResA[5]<<8) + ResA[4];
      cellTemp2[3] = (ResB[1]<<8) + ResB[0];
      cellTemp2[4] = (ResB[3]<<8) + ResB[2];
      cellTempREF2 = (ResB[5]<<8) + ResB[4];
      break;
  }
}
 
/****************************************
 ************ �������� ******************
 ****************************************/
//������ѹ
void measureVol(int chipNum){
  
  convertFlag = 0;
  
  switch(chipNum){
    case 1:
      startVolConvert(1);  //��ʼоƬ1��ADCת��
      TI2_Enable();     //����5ms��ʱ��
      while(convertFlag == 0);  //�ȴ�ת�����
      TI2_Disable();  //�رն�ʱ��
      convertFlag = 0;  
      
      readReg(1,RDCVA1,ResA); //��ȡ�Ĵ���
      readReg(1,RDCVB1,ResB);
      readReg(1,RDCVC1,ResC);
      readReg(1,RDCVD1,ResD);
        
      getVolVal(1);   //��ȡת��ֵ
      clearCellVol(1);  //���оƬ1�е�ص�ѹ�Ĵ�����ֵ 
      break;
    
    case 2:
      startVolConvert(2);  //��ʼоƬ1��ADCת��
      TI2_Enable();   //����5ms��ʱ��
      while(convertFlag == 0);  //�ȴ�ת�����
      TI2_Disable();  //�رն�ʱ��
      convertFlag = 0;  
      
      readReg(2,RDCVA2,ResA); //��ȡ�Ĵ���
      readReg(2,RDCVB2,ResB);
      readReg(2,RDCVC2,ResC);
      readReg(2,RDCVD2,ResD);
        
      getVolVal(2);   //��ȡת��ֵ
      clearCellVol(2);  //���оƬ1�е�ص�ѹ�Ĵ�����ֵ 
      break;
  }
    
}

//�����¶�
void measureTemp(int chipNum){
  switch(chipNum){
    case 1:
      startTempConvert(1);
      TI2_Enable();     //����5ms��ʱ��
      while(convertFlag == 0);  //�ȴ�ת�����
      TI2_Disable();  //�رն�ʱ��
      convertFlag = 0;  
      
      readReg(1,RDAUXA1,ResA);  //��ȡ�Ĵ���
      readReg(1,RDAUXB1,ResB);

      getTempVal(1);   //��ȡת��ֵ
      
      DisTemp[0] = CalTemp(cellTemp1[0],L,H);
      DisTemp[1] = CalTemp(cellTemp1[1],L,H);
      
      clearCellTemp(1);  //���оƬ1�и����Ĵ�����ֵ 
      break;
    case 2:
      startTempConvert(2);  //��ʼоƬ1��ADCת��
      TI2_Enable();   //����5ms��ʱ��
      while(convertFlag == 0);  //�ȴ�ת�����
      TI2_Disable();  //�رն�ʱ��
      convertFlag = 0;  
      
      readReg(2,RDAUXA2,ResA);  //��ȡ�Ĵ���
      readReg(2,RDAUXB2,ResB);
        
      getTempVal(2);   //��ȡת��ֵ
      clearCellTemp(2);  //���оƬ2�и����Ĵ�����ֵ 
      break;
  }
}


/****************************************
 ************ CANͨ�ź��� **************
 ****************************************/
//���͵�ѹ���¶�����
void CANsendVol(){
  
  //����оƬ1��8����ѹ����
  CANstate = CAN1_SendFrame(0,0x01,DATA_FRAME,8,cellVol1);
  CANstate = CAN1_SendFrame(1,0x02,DATA_FRAME,8,cellVol1+6);
  while(CANsenFlag!=2);
  CANsenFlag = 0;  
  
  //����оƬ2��8����ѹ����
  CANstate = CAN1_SendFrame(0,0x11,DATA_FRAME,8,cellVol2);
  CANstate = CAN1_SendFrame(1,0x12,DATA_FRAME,8,cellVol2+6);
  while(CANsenFlag!=2);
  CANsenFlag = 0;
}
void CANsendTemp(){
  //����оƬ1��GPIO0��1�������¶�
  CAN1_SendFrame(0,0x03,DATA_FRAME,4,DisTemp);
  while(CANsenFlag!=1);
  CANsenFlag = 0;
}

//���Ϳ�·��麯��
void CANsendOpen() {
  CANstate = CAN1_SendFrame(0,0x04,DATA_FRAME,2,&openStatus);
  while(CANsenFlag!=1);
    CANsenFlag = 0;
  openStatus = 0;
} 

//���Ϳ��ƼĴ�������
void CANsendConReg(){
  CANstate = CAN1_SendFrame(0,0x05,DATA_FRAME,5,&ConReg[0]);
  CANstate = CAN1_SendFrame(1,0x06,DATA_FRAME,6,&ConReg[0].VUV);
  while(CANsenFlag!=2);
    CANsenFlag = 0;  
  CANstate = CAN1_SendFrame(0,0x15,DATA_FRAME,5,&ConReg[1]);
  CANstate = CAN1_SendFrame(1,0x16,DATA_FRAME,6,&ConReg[1].VUV);
  while(CANsenFlag!=2);
    CANsenFlag = 0;
}