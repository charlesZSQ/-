
/********************************
 ******** Sending Functiong ********
********************************/

//发送命令,data是命令数组，chipNum是芯片号
void send(const unsigned char *data,int chipNum){
  SPIsenFlag = 1;
  Num = 0;
  switch(chipNum){
    case 1:
        SenState = SPI_1_ClearTxBuf();   //清除发送缓冲区
        SenState = SPI_1_ClearRxBuf();   //清除接收缓冲区
        SenState = SPI_1_SendBlock(data,4,&Num);
        while(SPIsenFlag == 1);
        break; 
    case 2:
        SenState = SPI_2_ClearTxBuf();   //清除发送缓冲区
        SenState = SPI_2_ClearRxBuf();   //清除接收缓冲区
        SenState = SPI_2_SendBlock(data,4,&Num);
        while(SPIsenFlag == 1);
        break; 
  }
}

/********************************
 ******** receiving Functiong ********
********************************/
//读数据
void readData(char size,int chipNum){
 
  SPIsenFlag = 2;
  Num = 0;
  
  switch(chipNum) {
  
  case 1:
    SenState = SPI_1_ClearTxBuf();   //清除发送缓冲区
    SenState = SPI_1_ClearRxBuf();   //清除接收缓冲区
    if(size == 4) {
      SenState = SPI_1_SendBlock(empty,4,&Num);
      while(SPIsenFlag == 2);
    }
    
    if(size == 8) {
      SenState = SPI_1_SendBlock(empty,4,&Num);
      while(SPIsenFlag == 2);
      if(SPIsenFlag == 3)
        SenState = SPI_1_SendBlock(empty,4,&Num);  //接收后4字节
      while(SPIsenFlag == 3);
    }break;
    
  case 2:
    SenState = SPI_2_ClearTxBuf();   //清除发送缓冲区
    SenState = SPI_2_ClearRxBuf();   //清除接收缓冲区
    if(size == 4) {
      SenState = SPI_2_SendBlock(empty,4,&Num);
      while(SPIsenFlag == 2);
    }
    
    if(size == 8) {
      SenState = SPI_2_SendBlock(empty,4,&Num);
      while(SPIsenFlag == 2);
      if(SPIsenFlag == 3)
        SenState = SPI_2_SendBlock(empty,4,&Num);  //接收后4字节
      while(SPIsenFlag == 3);
    }break;
  }
  
}


/********************************
 ******** write Register ********
********************************/
//write control register
void writeConReg(int chipNum) {
  switch(chipNum){
    case 1:
      send(WRCFG1,1);//写1号6804寄存器命令
      send(CFGR1,1);
      send(CFGR1+4,1);
      break;
    case 2:
      send(WRCFG2,2);//写2号6804寄存器命令
      send(CFGR2,2);
      send(CFGR2+4,2);
      break;
  }
}

/********************************
 ******** Wake up LTC6804 ********
********************************/
void wakeup(int chipNum){
  
  switch(chipNum) {
    case 1:
      send(empty,1);//唤醒   
      Cpu_Delay100US(10); //延时1ms
      break;
      
    case 2:
      send(empty,2);//唤醒   
      Cpu_Delay100US(10); //延时1ms
      break;
  }
}

void readConReg(int chipNum);
/****************************************
 ********** initiating LTC6804 **********
 ****************************************/
void init6804(int chipNum) {

  switch(chipNum) {
    
    case 1:
      
      wakeup(1);  
      CS1_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间
      writeConReg(1);   //写入寄存器1
      CS1_PutVal(1);   //拉高CSC，写完毕
      
      break;
      
    case 2:
    
      wakeup(2);  
      CS2_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间
      writeConReg(2);   //写入寄存器2
      CS2_PutVal(1);   //拉高CSC，写完毕
      
      break;
  }
}

/********************************
 ***** start ADC converting *****
 *********************************/
//start voltage converting
void startVolConvert(int chipNum) {
  switch(chipNum){
    case 1:
      wakeup(1);         //唤醒
      CS1_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间
      send(ADCV1,1);    //转换命令
      CS1_PutVal(1);   //拉高CSC
      break;
    case 2:
      wakeup(2);         //唤醒
      CS2_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间
      send(ADCV2,2);    //转换命令
      CS2_PutVal(1);   //拉高CSC
      break;
  }
}

//start temperature converting
void startTempConvert(int chipNum) {
  switch(chipNum){
    case 1:
      wakeup(1);         //唤醒
      CS1_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间
      send(ADAX1,1);    //转换命令
      CS1_PutVal(1);   //拉高CSC
      break;
    case 2:
      wakeup(2);         //唤醒
      CS2_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间
      send(ADAX2,2);    //转换命令
      CS2_PutVal(1);   //拉高CSC
      break;
  }
}



/****************************************
 *********Read Register*********
 ****************************************/
 
 void readReg(int chipNum,const unsigned char *comName,unsigned char *RecRegName){
  int i = 0;
  
  switch(chipNum) {
    case 1:
      wakeup(1);         //唤醒
      CS1_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(comName,1);    //发送读命令
      readData(8,1);  
      CS1_PutVal(1);   //拉高CSC
      for(;i<8;i++)
        RecRegName[i] = Rec[i];
      break;
    
    case 2:
      wakeup(2);         //唤醒
      CS2_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(comName,2);    //发送读命令
      readData(8,2);  
      CS2_PutVal(1);   //拉高CSC
      for(;i<8;i++)
        RecRegName[i] = Rec[i];
      break;
  }
}

/****************************************
 **** Clear Values in VOL Registers *****
 ****************************************/
 void clearCellVol(int chipNum){
  
  switch(chipNum) {
    case 1:
      wakeup(1);         //唤醒
      CS1_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(CLRCELL1,1);    //发送读命令
      CS1_PutVal(1);   //拉高CSC
      break;
      
    case 2:
      wakeup(2);         //唤醒
      CS2_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(CLRCELL2,2);    //发送读命令
      CS2_PutVal(1);   //拉高CSC
      break;
  }
 }
 
 /****************************************
 **** Clear Values in AX Registers *****
 ****************************************/
  void clearCellTemp(int chipNum){
  
  switch(chipNum) {
    case 1:
      wakeup(1);         //唤醒
      CS1_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(CLRAUX1,1);    //发送读命令
      CS1_PutVal(1);   //拉高CSC
      break;
      
    case 2:
      wakeup(2);         //唤醒
      CS2_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(CLRAUX2,2);    //发送读命令
      CS2_PutVal(1);   //拉高CSC
      break;
  }
 }
 
unsigned char ConRegData1[8];
unsigned char ConRegData2[8];

/********************************
********** read Registers *******
*********************************/
//deal with Reg data
void GetRegData(int chipNum) {
  switch(chipNum) {
    case 1:
      ConReg[0].GPIO = (ConRegData1[0] & 0xf8)>>3;
      ConReg[0].REFON = (ConRegData1[0] & 0x04)>>2;
      ConReg[0].SWTRD = (ConRegData1[0] & 0x02)>>1;
      ConReg[0].ADCOPT = ConRegData1[0] & 0x01;
      ConReg[0].VUV = ConRegData1[1]+(ConRegData1[2] & 0x000f)<<8;
      ConReg[0].VOV = ((ConRegData1[3]&0x00ff)<<4)+(ConRegData1[2] & 0xf0)>>4;
      ConReg[0].DCC = ConRegData1[4]+(ConRegData1[5] & 0x000f)<<8; 
      ConReg[0].DCTO = (ConRegData1[5] & 0xf0)>>4; 
    break;
    case 2:
      ConReg[1].GPIO = (ConRegData2[0] & 0xf8)>>3;
      ConReg[1].REFON = (ConRegData2[0] & 0x04)>>2;
      ConReg[1].SWTRD = (ConRegData2[0] & 0x02)>>1;
      ConReg[1].ADCOPT = ConRegData2[0] & 0x01;
      ConReg[1].VUV = ConRegData2[1]+(ConRegData2[2] & 0x000f)<<8;
      ConReg[1].VOV = ((ConRegData2[3]&0x00ff)<<4)+(ConRegData2[2] & 0xf0)>>4;
      ConReg[1].DCC = ConRegData2[4]+(ConRegData2[5] & 0x000f)<<8; 
      ConReg[1].DCTO = (ConRegData2[5] & 0xf0)>>4; 
    break;
  }
}


//read control register chipNum
void readConReg(int chipNum){
  switch(chipNum){
    case 1:
      readReg(1,RDCFG1,ConRegData1);
      GetRegData(1);
      break;
    case 2:
      readReg(2,RDCFG2,ConRegData2);
      GetRegData(2);
      break;
  }
}
 
 
 /****************************************
 ************ 开路检查函数 **************
 ****************************************/
void getCellPU(int chipNum){
  switch(chipNum){
    case 1:
      cellPU1[0] = (ResA[1]<<8) + ResA[0];
      cellPU1[1] = (ResA[3]<<8) + ResA[2];
      cellPU1[2] = (ResA[5]<<8) + ResA[4];
      cellPU1[3] = (ResB[1]<<8) + ResB[0];
      cellPU1[4] = (ResB[3]<<8) + ResB[2];
      cellPU1[5] = (ResB[5]<<8) + ResB[4];
      cellPU1[6] = (ResC[1]<<8) + ResC[0];
      cellPU1[7] = (ResC[3]<<8) + ResC[2];
      cellPU1[8] = (ResC[5]<<8) + ResC[4];
      cellPU1[9] = (ResD[1]<<8) + ResD[0];
      cellPU1[10] = (ResD[3]<<8) + ResD[2];
      cellPU1[11] = (ResD[5]<<8) + ResD[4];
      break;
    case 2:
      cellPU2[0] = (ResA[1]<<8) + ResA[0];
      cellPU2[1] = (ResA[3]<<8) + ResA[2];
      cellPU2[2] = (ResA[5]<<8) + ResA[4];
      cellPU2[3] = (ResB[1]<<8) + ResB[0];
      cellPU2[4] = (ResB[3]<<8) + ResB[2];
      cellPU2[5] = (ResB[5]<<8) + ResB[4];
      cellPU2[6] = (ResC[1]<<8) + ResC[0];
      cellPU2[7] = (ResC[3]<<8) + ResC[2];
      cellPU2[8] = (ResC[5]<<8) + ResC[4];
      cellPU2[9] = (ResD[1]<<8) + ResD[0];
      cellPU2[10] = (ResD[3]<<8) + ResD[2];
      cellPU2[11] = (ResD[5]<<8) + ResD[4];
      break;
  }
}

void getCellPD(int chipNum){
  switch(chipNum){
    case 1:
      cellPD1[0] = (ResA[1]<<8) + ResA[0];
      cellPD1[1] = (ResA[3]<<8) + ResA[2];
      cellPD1[2] = (ResA[5]<<8) + ResA[4];
      cellPD1[3] = (ResB[1]<<8) + ResB[0];
      cellPD1[4] = (ResB[3]<<8) + ResB[2];
      cellPD1[5] = (ResB[5]<<8) + ResB[4];
      cellPD1[6] = (ResC[1]<<8) + ResC[0];
      cellPD1[7] = (ResC[3]<<8) + ResC[2];
      cellPD1[8] = (ResC[5]<<8) + ResC[4];
      cellPD1[9] = (ResD[1]<<8) + ResD[0];
      cellPD1[10] = (ResD[3]<<8) + ResD[2];
      cellPD1[11] = (ResD[5]<<8) + ResD[4];
      break;
    case 2:
      cellPD2[0] = (ResA[1]<<8) + ResA[0];
      cellPD2[1] = (ResA[3]<<8) + ResA[2];
      cellPD2[2] = (ResA[5]<<8) + ResA[4];
      cellPD2[3] = (ResB[1]<<8) + ResB[0];
      cellPD2[4] = (ResB[3]<<8) + ResB[2];
      cellPD2[5] = (ResB[5]<<8) + ResB[4];
      cellPD2[6] = (ResC[1]<<8) + ResC[0];
      cellPD2[7] = (ResC[3]<<8) + ResC[2];
      cellPD2[8] = (ResC[5]<<8) + ResC[4];
      cellPD2[9] = (ResD[1]<<8) + ResD[0];
      cellPD2[10] = (ResD[3]<<8) + ResD[2];
      cellPD2[11] = (ResD[5]<<8) + ResD[4];
      break;
  }
}

/******** 导线开路检查 ********/
void getOpenCheckVal(int chipNum){
  int i = 0;
  int j = 0;
  switch(chipNum) {
  
    case 1:
    
    for(;j<3;j++){      //运行ADOW3次
      wakeup(1);         //唤醒
      CS1_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(ADOWPU1,1);    //发送读命令  
      CS1_PutVal(1);   //拉高CSC
      Cpu_Delay100US(5);  //稳定一段时间
    }

      readReg(1,RDCVA1,ResA); //读取寄存器
      readReg(1,RDCVB1,ResB);
      readReg(1,RDCVC1,ResC);
      readReg(1,RDCVD1,ResD);
      
      getCellPU(1);
      
    for(;j<3;j++){      //运行ADOW3次
      wakeup(1);         //唤醒
      CS1_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(ADOWPD1,1);    //发送读命令  
      CS1_PutVal(1);   //拉高CSC
      Cpu_Delay100US(5);  //稳定一段时间
    }
      
      readReg(1,RDCVA1,ResA); //读取寄存器
      readReg(1,RDCVB1,ResB);
      readReg(1,RDCVC1,ResC);
      readReg(1,RDCVD1,ResD);
      getCellPD(1);
      break;
    
    case 2:
    
    for(;j<3;j++){      //运行ADOW3次
      wakeup(1);         //唤醒
      CS2_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(ADOWPU2,2);    //发送读命令  
      CS2_PutVal(1);   //拉高CSC
      Cpu_Delay100US(5);  //稳定一段时间
    }

      readReg(2,RDCVA2,ResA); //读取寄存器
      readReg(2,RDCVB2,ResB);
      readReg(2,RDCVC2,ResC);
      readReg(2,RDCVD2,ResD);
      getCellPU(2);
      
    for(;j<3;j++){      //运行ADOW3次
      wakeup(1);         //唤醒
      CS2_PutVal(0);   //拉低CSC
      Cpu_Delay100US(5);  //稳定一段时间 
      send(ADOWPD2,2);    //发送读命令  
      CS2_PutVal(1);   //拉高CSC
      Cpu_Delay100US(5);  //稳定一段时间
    }
      
      readReg(2,RDCVA2,ResA); //读取寄存器
      readReg(2,RDCVB2,ResB);
      readReg(2,RDCVC2,ResC);
      readReg(2,RDCVD2,ResD);
      getCellPD(2);
      break;
  }
}


 //若cellDelt(n+1)=0,则cell(n)开路 
 //若cellPD(12)=0,则cell(12)开路
 //若cellPU(1)=0,则cell(0)开路
word openCheck(int chipNum){
  unsigned char i = 0;
  word temp = 0;
  word value = 0;
  switch(chipNum){
    case 1:
      getOpenCheckVal(1);
     
      for(;i<12;i++)
        cellDelt1[i] = cellPU1[i]-cellPD1[i];
      
      if(cellPU1[0] == 0)
        temp |= 0x0001;    //置1的位为开路
     
      if(cellPD1[11] == 0)
        temp |= 0x0800;
      
      for(i=1;i<11;i++) {
        if(cellDelt1[i]<-4000)
          temp |= (0x0001<<i);
      }
   
       value |= ((temp<<8) & 0x0f00);   //1组为高位
       value |= ((temp<<6) & 0xf000);
       break;
      
    case 2:
      getOpenCheckVal(2);
      
      for(;i<12;i++)
        cellDelt2[i] = cellPU2[i]-cellPD2[i];
     
      if(cellPU2[0] == 0)
        temp |= 0x0001;    //置1的位为开路
      
      if(cellPD2[11] == 0)
        temp |= 0x0800;
      
      for(i=1;i<11;i++) {
        if(cellDelt2[i]<-4000)
          temp |= (0x0001<<i);
      }
      
      value |= (temp & 0x000f);     //2组为低位
      value |= ((temp>>2) & 0x00f0); 
      break;  
       
  }
  
    return value;
}



