#include "workthread.h"
#include "ControlCAN.h"
#include  <QtDebug>
#include "ltc6804.h"

extern VCI_CAN_OBJ openCheck;       //开路检查帧
extern VCI_CAN_OBJ DisplayData;     //显示命令
extern VCI_CAN_OBJ ReadReg;
extern VCI_CAN_OBJ RecData[6];     //电压和温度一共5帧
extern VCI_CAN_OBJ RecVol[4];     //接收电压帧
extern VCI_CAN_OBJ RecTemp;    //接收温度帧
extern int Display_Vol[16];    //存储待显示电压数据
extern int Display_Temp[2];     //待显示温度数据
extern QString Display_Open[2];
extern VCI_ERR_INFO CANerr;
extern int DisplayFlag;
extern bool Measure_OK;
extern bool ConReg_check;

extern QString GPIO[2];
extern QString REFON[2];
extern QString SWTRD[2];
extern QString DCTO[2];
extern QString ADCOPT[2];
extern QString VUV[2];
extern QString VOV[2];
extern QString DCC[2];

CAN_rec_thread::CAN_rec_thread(){}
//CAN_sen_thread::CAN_sen_thread(){}
//display_thread::display_thread(){}

//接收线run程函数定义
void CAN_rec_thread::run()
{
    unsigned char i = 0,count = 0;;
    unsigned int j = 0, k = 0;
    DWORD flag = 0;

    if(Measure_OK)
    {
        while(Measure_OK)
        {
            i = 0; j = 0; k = 0; flag = 0;
            flag = VCI_Transmit(VCI_USBCAN1,0,0,&DisplayData,1);        //发送显示请求命令
            if(flag != 1)
            {
                DisplayFlag = 1;                                        //报错
            }


            flag = VCI_Receive(VCI_USBCAN1,0,0,RecData,10,50);          //接收数据
            if(flag != 6)
            {
                DisplayFlag = 2;
                flag = VCI_ReadErrInfo(VCI_USBCAN1,0,0,&CANerr);        //报错
            }

            if(flag == 6)                                           //收到正确帧数，处理数据
            {
                for(i = 0; i < 6; i ++)       //分开电压（4帧）和温度（2帧）
                {
                    if((RecData[i].ID & 0x10) == 0)       //芯片1电压数据
                    {
                        RecVol[j] = RecData[i];
                        j++;
                    }

                    if((RecData[i].ID & 0x10) == 0x10)    //芯片2电压数据
                    {
                        RecVol[j] = RecData[i];
                        j++;
                    }

                    if(RecData[i].ID == 0x03)       //温度数据
                    {
                        RecTemp = RecData[i];
                    }

                    if(RecData[i].ID == 0x04)
                    {
                        openCheck = RecData[i];
                    }
                }

                //合成待显示格式数据
               i = 0; j = 0;        //电压部分

                for(; j<4 ; j++)
                {
                    for( i=0 ; i<8 ; i=i+2)
                    {
                        Display_Vol[k] = (RecVol[j].Data[i]<<8) + RecVol[j].Data[i+1];
                        k++;
                    }
                }

                k = 0;          //温度部分

                for( i=0 ; i<4 ; i=i+2)
                {
                    Display_Temp[k] = (RecTemp.Data[i]<<8) + RecTemp.Data[i+1];
                    k++;
                }

               //开路状态部分
               Display_Open[0] = QString("%1").arg(openCheck.Data[0],8,2,QChar('0'));
               Display_Open[1] = QString("%1").arg(openCheck.Data[1],8,2,QChar('0'));

               DisplayFlag = 3;
            }
            msleep(100);
            emit display_signal(DisplayFlag);       //发射信号
        }
    }

    if(ConReg_check)
    {
        while(ConReg_check)
        {
            i = 0; flag = 0;
            flag = VCI_Transmit(VCI_USBCAN1,0,0,&ReadReg,1);        //发送显示请求命令
            if(flag != 1)
            {
                DisplayFlag = 1;                                        //报错
            }


            flag = VCI_Receive(VCI_USBCAN1,0,0,RecData,10,100);          //接收数据
            if(flag != 4)
            {
                DisplayFlag = 2;
                flag = VCI_ReadErrInfo(VCI_USBCAN1,0,0,&CANerr);        //报错
            }

            if(flag == 4)
            {
              ConReg[0].GPIO = RecData[0].Data[0];
              ConReg[0].REFON = RecData[0].Data[1];
              ConReg[0].SWTRD = RecData[0].Data[2];
              ConReg[0].ADCOPT = RecData[0].Data[3];
              ConReg[0].DCTO = RecData[0].Data[4];
              ConReg[0].VUV = ((RecData[1].Data[0]<<8)+RecData[1].Data[1])&0xfff;
              ConReg[0].VOV = ((RecData[1].Data[2]<<8)+RecData[1].Data[3])&0xfff;
              ConReg[0].DCC = ((RecData[1].Data[4]<<8)+RecData[1].Data[5])&0xfff;

              GPIO[0] = QString("%1").arg(ConReg[0].GPIO,2,16,QChar('0'));
              REFON[0] = QString("%1").arg(ConReg[0].REFON,1,16,QChar('0'));
              SWTRD[0] = QString("%1").arg(ConReg[0].SWTRD,1,16,QChar('0'));
              ADCOPT[0] = QString("%1").arg(ConReg[0].ADCOPT,1,16,QChar('0'));
              DCTO[0] = QString("%1").arg(ConReg[0].DCTO,1,16,QChar('0'));
              VUV[0] = QString("%1").arg(ConReg[0].VUV,3,16,QChar('0'));
              VOV[0] = QString("%1").arg(ConReg[0].VOV,3,16,QChar('0'));
              DCC[0] = QString("%1").arg(ConReg[0].DCC,3,16,QChar('0'));

              ConReg[1].GPIO = RecData[2].Data[0];
              ConReg[1].REFON = RecData[2].Data[1];
              ConReg[1].SWTRD = RecData[2].Data[2];
              ConReg[1].ADCOPT = RecData[2].Data[3];
              ConReg[1].DCTO = RecData[2].Data[4];
              ConReg[1].VUV = ((RecData[3].Data[0]<<8)+RecData[1].Data[1])&0xfff;
              ConReg[1].VOV = ((RecData[3].Data[2]<<8)+RecData[1].Data[3])&0xfff;
              ConReg[1].DCC = ((RecData[3].Data[4]<<8)+RecData[1].Data[5])&0xfff;

              GPIO[1] = QString("%1").arg(ConReg[1].GPIO,2,16,QChar('0'));
              REFON[1] = QString("%1").arg(ConReg[1].REFON,1,16,QChar('0'));
              SWTRD[1] = QString("%1").arg(ConReg[1].SWTRD,1,16,QChar('0'));
              ADCOPT[1] = QString("%1").arg(ConReg[1].ADCOPT,1,16,QChar('0'));
              DCTO[1] = QString("%1").arg(ConReg[1].DCTO,1,16,QChar('0'));
              VUV[1] = QString("%1").arg(ConReg[1].VUV,3,16,QChar('0'));
              VOV[1] = QString("%1").arg(ConReg[1].VOV,3,16,QChar('0'));
              DCC[1] = QString("%1").arg(ConReg[1].DCC,3,16,QChar('0'));

              DisplayFlag = 4;
              ConReg_check = FALSE;
            }
            msleep(50);
       }
        emit display_signal(DisplayFlag);       //发射信号
    }

}


