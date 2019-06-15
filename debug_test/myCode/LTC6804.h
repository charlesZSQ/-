
/********************************
 ******** Sending Functiong ********
********************************/

//��������,data���������飬chipNum��оƬ��
void send(const unsigned char *data,int chipNum){
  SPIsenFlag = 1;
  Num = 0;
  switch(chipNum){
    case 1:
        SenState = SPI_1_ClearTxBuf();   //������ͻ�����
        SenState = SPI_1_ClearRxBuf();   //������ջ�����
        SenState = SPI_1_SendBlock(data,4,&Num);
        while(SPIsenFlag == 1);
        break; 
    case 2:
        SenState = SPI_2_ClearTxBuf();   //������ͻ�����
        SenState = SPI_2_ClearRxBuf();   //������ջ�����
        SenState = SPI_2_SendBlock(data,4,&Num);
        while(SPIsenFlag == 1);
        break; 
  }
}

/********************************
 ******** receiving Functiong ********
********************************/
//������
void readData(char size,int chipNum){
 
  SPIsenFlag = 2;
  Num = 0;
  
  switch(chipNum) {
  
  case 1:
    SenState = SPI_1_ClearTxBuf();   //������ͻ�����
    SenState = SPI_1_ClearRxBuf();   //������ջ�����
    if(size == 4) {
      SenState = SPI_1_SendBlock(empty,4,&Num);
      while(SPIsenFlag == 2);
    }
    
    if(size == 8) {
      SenState = SPI_1_SendBlock(empty,4,&Num);
      while(SPIsenFlag == 2);
      if(SPIsenFlag == 3)
        SenState = SPI_1_SendBlock(empty,4,&Num);  //���պ�4�ֽ�
      while(SPIsenFlag == 3);
    }break;
    
  case 2:
    SenState = SPI_2_ClearTxBuf();   //������ͻ�����
    SenState = SPI_2_ClearRxBuf();   //������ջ�����
    if(size == 4) {
      SenState = SPI_2_SendBlock(empty,4,&Num);
      while(SPIsenFlag == 2);
    }
    
    if(size == 8) {
      SenState = SPI_2_SendBlock(empty,4,&Num);
      while(SPIsenFlag == 2);
      if(SPIsenFlag == 3)
        SenState = SPI_2_SendBlock(empty,4,&Num);  //���պ�4�ֽ�
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
      send(WRCFG1,1);//д1��6804�Ĵ�������
      send(CFGR1,1);
      send(CFGR1+4,1);
      break;
    case 2:
      send(WRCFG2,2);//д2��6804�Ĵ�������
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
      send(empty,1);//����   
      Cpu_Delay100US(10); //��ʱ1ms
      break;
      
    case 2:
      send(empty,2);//����   
      Cpu_Delay100US(10); //��ʱ1ms
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
      CS1_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
      writeConReg(1);   //д��Ĵ���1
      CS1_PutVal(1);   //����CSC��д���
      
      break;
      
    case 2:
    
      wakeup(2);  
      CS2_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
      writeConReg(2);   //д��Ĵ���2
      CS2_PutVal(1);   //����CSC��д���
      
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
      wakeup(1);         //����
      CS1_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
      send(ADCV1,1);    //ת������
      CS1_PutVal(1);   //����CSC
      break;
    case 2:
      wakeup(2);         //����
      CS2_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
      send(ADCV2,2);    //ת������
      CS2_PutVal(1);   //����CSC
      break;
  }
}

//start temperature converting
void startTempConvert(int chipNum) {
  switch(chipNum){
    case 1:
      wakeup(1);         //����
      CS1_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
      send(ADAX1,1);    //ת������
      CS1_PutVal(1);   //����CSC
      break;
    case 2:
      wakeup(2);         //����
      CS2_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
      send(ADAX2,2);    //ת������
      CS2_PutVal(1);   //����CSC
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
      wakeup(1);         //����
      CS1_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(comName,1);    //���Ͷ�����
      readData(8,1);  
      CS1_PutVal(1);   //����CSC
      for(;i<8;i++)
        RecRegName[i] = Rec[i];
      break;
    
    case 2:
      wakeup(2);         //����
      CS2_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(comName,2);    //���Ͷ�����
      readData(8,2);  
      CS2_PutVal(1);   //����CSC
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
      wakeup(1);         //����
      CS1_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(CLRCELL1,1);    //���Ͷ�����
      CS1_PutVal(1);   //����CSC
      break;
      
    case 2:
      wakeup(2);         //����
      CS2_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(CLRCELL2,2);    //���Ͷ�����
      CS2_PutVal(1);   //����CSC
      break;
  }
 }
 
 /****************************************
 **** Clear Values in AX Registers *****
 ****************************************/
  void clearCellTemp(int chipNum){
  
  switch(chipNum) {
    case 1:
      wakeup(1);         //����
      CS1_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(CLRAUX1,1);    //���Ͷ�����
      CS1_PutVal(1);   //����CSC
      break;
      
    case 2:
      wakeup(2);         //����
      CS2_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(CLRAUX2,2);    //���Ͷ�����
      CS2_PutVal(1);   //����CSC
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
 ************ ��·��麯�� **************
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

/******** ���߿�·��� ********/
void getOpenCheckVal(int chipNum){
  int i = 0;
  int j = 0;
  switch(chipNum) {
  
    case 1:
    
    for(;j<3;j++){      //����ADOW3��
      wakeup(1);         //����
      CS1_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(ADOWPU1,1);    //���Ͷ�����  
      CS1_PutVal(1);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
    }

      readReg(1,RDCVA1,ResA); //��ȡ�Ĵ���
      readReg(1,RDCVB1,ResB);
      readReg(1,RDCVC1,ResC);
      readReg(1,RDCVD1,ResD);
      
      getCellPU(1);
      
    for(;j<3;j++){      //����ADOW3��
      wakeup(1);         //����
      CS1_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(ADOWPD1,1);    //���Ͷ�����  
      CS1_PutVal(1);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
    }
      
      readReg(1,RDCVA1,ResA); //��ȡ�Ĵ���
      readReg(1,RDCVB1,ResB);
      readReg(1,RDCVC1,ResC);
      readReg(1,RDCVD1,ResD);
      getCellPD(1);
      break;
    
    case 2:
    
    for(;j<3;j++){      //����ADOW3��
      wakeup(1);         //����
      CS2_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(ADOWPU2,2);    //���Ͷ�����  
      CS2_PutVal(1);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
    }

      readReg(2,RDCVA2,ResA); //��ȡ�Ĵ���
      readReg(2,RDCVB2,ResB);
      readReg(2,RDCVC2,ResC);
      readReg(2,RDCVD2,ResD);
      getCellPU(2);
      
    for(;j<3;j++){      //����ADOW3��
      wakeup(1);         //����
      CS2_PutVal(0);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ�� 
      send(ADOWPD2,2);    //���Ͷ�����  
      CS2_PutVal(1);   //����CSC
      Cpu_Delay100US(5);  //�ȶ�һ��ʱ��
    }
      
      readReg(2,RDCVA2,ResA); //��ȡ�Ĵ���
      readReg(2,RDCVB2,ResB);
      readReg(2,RDCVC2,ResC);
      readReg(2,RDCVD2,ResD);
      getCellPD(2);
      break;
  }
}


 //��cellDelt(n+1)=0,��cell(n)��· 
 //��cellPD(12)=0,��cell(12)��·
 //��cellPU(1)=0,��cell(0)��·
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
        temp |= 0x0001;    //��1��λΪ��·
     
      if(cellPD1[11] == 0)
        temp |= 0x0800;
      
      for(i=1;i<11;i++) {
        if(cellDelt1[i]<-4000)
          temp |= (0x0001<<i);
      }
   
       value |= ((temp<<8) & 0x0f00);   //1��Ϊ��λ
       value |= ((temp<<6) & 0xf000);
       break;
      
    case 2:
      getOpenCheckVal(2);
      
      for(;i<12;i++)
        cellDelt2[i] = cellPU2[i]-cellPD2[i];
     
      if(cellPU2[0] == 0)
        temp |= 0x0001;    //��1��λΪ��·
      
      if(cellPD2[11] == 0)
        temp |= 0x0800;
      
      for(i=1;i<11;i++) {
        if(cellDelt2[i]<-4000)
          temp |= (0x0001<<i);
      }
      
      value |= (temp & 0x000f);     //2��Ϊ��λ
      value |= ((temp>>2) & 0x00f0); 
      break;  
       
  }
  
    return value;
}



