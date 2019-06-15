#ifndef CAN_INIT_H
#define CAN_INIT_H

#include "ControlCAN.h"
#include "QString"

//命令
VCI_INIT_CONFIG CANinitData = {0,0xffffffff,0,0,0x01,0x1c,0};    //CAN初始化命令 100kbit/s
VCI_CAN_OBJ StartMonitor = {0,0,0,0,0,0,1,{1},{0}};   //1   //开始
VCI_CAN_OBJ PeriodCheck = {0,0,0,0,0,0,1,{2},{0}};    //2
VCI_CAN_OBJ DisplayData = {0,0,0,0,0,0,1,{3},{0}};      //3   读取当前数据
VCI_CAN_OBJ ReadReg = {0,0,0,0,0,0,1,{4},{0}};        //5
VCI_CAN_OBJ Break = {0,0,0,0,0,0,1,{5},{0}};          //6

VCI_CAN_OBJ RecData[6];     //电压和温度一共5帧，控制寄存器4帧
VCI_CAN_STATUS CANregStatus;
VCI_CAN_OBJ RecVol[4];     //接收电压帧
VCI_CAN_OBJ RecTemp;    //接收温度帧
VCI_CAN_OBJ openCheck;  //开路检查帧

VCI_ERR_INFO CANerr;
int Display_Vol[16];    //存储待显示电压数据
int Display_Temp[8];     //待显示温度数据
int DisplayFlag = 0;     //显示类型标志
QString Display_Open[2];       //待显示开路状态

#endif // CAN_INIT_H
