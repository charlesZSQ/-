/** ###################################################################
**     Filename  : debug_test.c
**     Project   : debug_test
**     Processor : MC9S12XEP100MAL
**     Version   : Driver 01.14
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 30/08/2018, 10:10
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE debug_test */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "SPI_1.h"
#include "TI1.h"
#include "TI2.h"
#include "CS1.h"
#include "SPI_2.h"
#include "CS2.h"
#include "CAN1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "PEC.h"
#include "FunctionsDef.h"
#include "CAN.h"

void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  
  init6804(1);     //initiate ltc6804
  init6804(2);     //initiate ltc6804
  
  initCAN();      //initiate CAN module
  
  while(1){
    
  switch(checkFlag){
    case StartMonitor:  //start check
        TI1_Enable();   //enable 100ms timer
        checkFlag = PerioCheck;
        break;
    case PerioCheck:    //peridical check
        if(intervalFlag == 1) //测量标志位
        {
          intervalFlag = 0;   //测量标志位清零
          measureVol(1);    //measure voltage of cells connected to chip1
          measureVol(2);    //measure voltage of cells connected to chip2
          measureTemp(1);    //measure temp of cells connected to chip1
          openStatus = openCheck(1)|openCheck(2);          
      };break;
    case DisplayData:         
      CANsendVol();
      CANsendTemp();
      CANsendOpen();
      checkFlag = PerioCheck;
      break;
    case ReadReg: 
      readConReg(1);
      readConReg(2);
      CANsendConReg();
      checkFlag = PerioCheck;
      break;
    case Break:
      TI1_Disable();    //disable 100ms timer
      intervalFlag = 0;
      checkFlag = PerioCheck;
      break;
  }
}  
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END debug_test */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.05 [04.46]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
