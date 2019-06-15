/***************************
***********CAN 配置*********
***************************/
//发送优先级，Buffer1-最高Buffer3-最低
void initCAN(void){
  CANstate = CAN1_SetTxBufPriority(0,1);
  CANstate = CAN1_SetTxBufPriority(1,2); 
  CANstate = CAN1_SetTxBufPriority(2,3); 
}