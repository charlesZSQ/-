#include"comAndData.h"
#include "LTC6804.h"
#include "temptureTable.h"
#include "math.h"
extern word DisTemp[2];
extern word H;
extern word L;

/****************************************
 ************ 数据处理函数 ******************
 ****************************************/
 /***** 处理电压数据 ******/
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

/***** 处理温度数据 ******/
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
 ************ 测量函数 ******************
 ****************************************/
//测量电压
void measureVol(int chipNum){
  
  convertFlag = 0;
  
  switch(chipNum){
    case 1:
      startVolConvert(1);  //开始芯片1的ADC转换
      TI2_Enable();     //开启5ms定时器
      while(convertFlag == 0);  //等待转换完成
      TI2_Disable();  //关闭定时器
      convertFlag = 0;  
      
      readReg(1,RDCVA1,ResA); //读取寄存器
      readReg(1,RDCVB1,ResB);
      readReg(1,RDCVC1,ResC);
      readReg(1,RDCVD1,ResD);
        
      getVolVal(1);   //获取转换值
      clearCellVol(1);  //清除芯片1中电池电压寄存器的值 
      break;
    
    case 2:
      startVolConvert(2);  //开始芯片1的ADC转换
      TI2_Enable();   //开启5ms定时器
      while(convertFlag == 0);  //等待转换完成
      TI2_Disable();  //关闭定时器
      convertFlag = 0;  
      
      readReg(2,RDCVA2,ResA); //读取寄存器
      readReg(2,RDCVB2,ResB);
      readReg(2,RDCVC2,ResC);
      readReg(2,RDCVD2,ResD);
        
      getVolVal(2);   //获取转换值
      clearCellVol(2);  //清除芯片1中电池电压寄存器的值 
      break;
  }
    
}

//测量温度
void measureTemp(int chipNum){
  switch(chipNum){
    case 1:
      startTempConvert(1);
      TI2_Enable();     //开启5ms定时器
      while(convertFlag == 0);  //等待转换完成
      TI2_Disable();  //关闭定时器
      convertFlag = 0;  
      
      readReg(1,RDAUXA1,ResA);  //读取寄存器
      readReg(1,RDAUXB1,ResB);

      getTempVal(1);   //获取转换值
      
      DisTemp[0] = CalTemp(cellTemp1[0],L,H);
      DisTemp[1] = CalTemp(cellTemp1[1],L,H);
      
      clearCellTemp(1);  //清除芯片1中辅助寄存器的值 
      break;
    case 2:
      startTempConvert(2);  //开始芯片1的ADC转换
      TI2_Enable();   //开启5ms定时器
      while(convertFlag == 0);  //等待转换完成
      TI2_Disable();  //关闭定时器
      convertFlag = 0;  
      
      readReg(2,RDAUXA2,ResA);  //读取寄存器
      readReg(2,RDAUXB2,ResB);
        
      getTempVal(2);   //获取转换值
      clearCellTemp(2);  //清除芯片2中辅助寄存器的值 
      break;
  }
}


/****************************************
 ************ CAN通信函数 **************
 ****************************************/
//发送电压和温度数据
void CANsendVol(){
  
  //发送芯片1的8个电压数据
  CANstate = CAN1_SendFrame(0,0x01,DATA_FRAME,8,cellVol1);
  CANstate = CAN1_SendFrame(1,0x02,DATA_FRAME,8,cellVol1+6);
  while(CANsenFlag!=2);
  CANsenFlag = 0;  
  
  //发送芯片2的8个电压数据
  CANstate = CAN1_SendFrame(0,0x11,DATA_FRAME,8,cellVol2);
  CANstate = CAN1_SendFrame(1,0x12,DATA_FRAME,8,cellVol2+6);
  while(CANsenFlag!=2);
  CANsenFlag = 0;
}
void CANsendTemp(){
  //发送芯片1的GPIO0、1测量的温度
  CAN1_SendFrame(0,0x03,DATA_FRAME,4,DisTemp);
  while(CANsenFlag!=1);
  CANsenFlag = 0;
}

//发送开路检查函数
void CANsendOpen() {
  CANstate = CAN1_SendFrame(0,0x04,DATA_FRAME,2,&openStatus);
  while(CANsenFlag!=1);
    CANsenFlag = 0;
  openStatus = 0;
} 

//发送控制寄存器数据
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