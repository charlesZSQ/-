/** ###################################################################
**     Filename  : Events.c
**     Project   : debug_test
**     Processor : MC9S12XEP100MAL
**     Component : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 30/08/2018, 10:10
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#define StartMonitor 0x01 //监测电压和温度并返回
#define PerioCheck  0x02  //周期检测
#define DisplayData 0x03  //在PC显示电压温度，即CAN传输参数到PC
#define ReadReg 0x04  //返回寄存器配置信息
#define Break 0x05    //停止检测

extern char checkFlag;
extern char convertFlag;
extern char SPIsenFlag;
extern unsigned char Rec[8];
extern char RecState;
extern word Num;
extern char intervalFlag;    //测量周期标志

extern byte CANrecFlag;
extern byte CANsenFlag;
extern byte CANsenData[8];
extern byte CANrecData[8];
extern dword CANrecID;
extern byte CANrecLen;  

#pragma CODE_SEG DEFAULT

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void)
{
  /* Write your code here ... */
  intervalFlag = 1;
}

/*
** ===================================================================
**     Event       :  SPI_1_OnError (module Events)
**
**     Component   :  SPI_1 [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SPI_1_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SPI_1_OnFullRxBuf (module Events)
**
**     Component   :  SPI_1 [SynchroMaster]
**     Description :
**         This event is called when the input buffer is full, i.e.
**         after reception of the last character that was successfully
**         placed into input buffer.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Input buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SPI_1_OnFullRxBuf(void)
{
  /* Write your code here ... */
  switch(SPIsenFlag)
  {
    case 1:
      SPI_1_ClearRxBuf();   //清除缓冲区
      SPIsenFlag = 0;
      break;
    case 2:
      RecState = SPI_1_RecvBlock(Rec,4,&Num);  //取数据
      SPI_1_ClearRxBuf();   //清除缓冲区
      SPIsenFlag = 3;
      break;
    case 3:
      RecState = SPI_1_RecvBlock(Rec+4,4,&Num);
      SPI_1_ClearRxBuf();   //清除缓冲区
      SPIsenFlag = 0;
      break;
    default:break;
  }    
}

/*
** ===================================================================
**     Event       :  SPI_1_OnFreeTxBuf (module Events)
**
**     Component   :  SPI_1 [SynchroMaster]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Output buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SPI_1_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI2_OnInterrupt(void)
{
  /* Write your code here ... */
  convertFlag = 1;  
}

/*
** ===================================================================
**     Event       :  SPI_2_OnError (module Events)
**
**     Component   :  SPI_2 [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SPI_2_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SPI_2_OnFullRxBuf (module Events)
**
**     Component   :  SPI_2 [SynchroMaster]
**     Description :
**         This event is called when the input buffer is full, i.e.
**         after reception of the last character that was successfully
**         placed into input buffer.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Input buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SPI_2_OnFullRxBuf(void)
{
  /* Write your code here ... */
  switch(SPIsenFlag)
  {
    case 1:
      SPI_2_ClearRxBuf();   //清除缓冲区
      SPIsenFlag = 0;
      break;
    case 2:
      RecState = SPI_2_RecvBlock(Rec,4,&Num);  //取数据
      SPI_2_ClearRxBuf();   //清除缓冲区
      SPIsenFlag = 3;
      break;
    case 3:
      RecState = SPI_2_RecvBlock(Rec+4,4,&Num);
      SPI_2_ClearRxBuf();   //清除缓冲区
      SPIsenFlag = 0;
      break;
    default:break;
  }
}

/*
** ===================================================================
**     Event       :  SPI_2_OnFreeTxBuf (module Events)
**
**     Component   :  SPI_2 [SynchroMaster]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Output buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SPI_2_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  CAN1_OnFreeTxBuffer (module Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called after a successful transmission of a
**         message. The event is available only if Interrupt
**         service/event is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         BufferMask      - Transmit buffer mask. The
**                           mask can be used to check what message
**                           buffer caused the transmit interrupt.
**     Returns     : Nothing
** ===================================================================
*/
void CAN1_OnFreeTxBuffer(word BufferMask)
{
  /* Write your code here ... */
  CANsenFlag ++;
}

/*
** ===================================================================
**     Event       :  CAN1_OnFullRxBuffer (module Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when the receive buffer is full
**         after a successful reception of a message. The event is
**         available only if Interrupt service/event is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CAN1_OnFullRxBuffer(void)
{
  /* Write your code here ... */
  CAN1_ReadFrame(&CANrecID,DATA_FRAME,STANDARD_FORMAT,&CANrecLen,CANrecData);//读CANbuffer
  switch(CANrecData[0]){
    case StartMonitor:
      checkFlag = StartMonitor;break;  
    case ReadReg:
      checkFlag = ReadReg;break;
    case PerioCheck:
      checkFlag = PerioCheck;break;
    case DisplayData:
      checkFlag = DisplayData;break;
    case Break:
      checkFlag = Break;break;
  }
}

/*
** ===================================================================
**     Event       :  CAN1_OnBusOff (module Events)
**
**     Component   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when the node status becomes bus-off.
**         The event is available only if Interrupt service/event is
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CAN1_OnBusOff(void)
{
  /* Write your code here ... */
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.05 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
