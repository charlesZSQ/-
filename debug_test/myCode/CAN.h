/***************************
***********CAN ����*********
***************************/
//�������ȼ���Buffer1-��ߣBBuffer3-���
void initCAN(void){
  CANstate = CAN1_SetTxBufPriority(0,1);
  CANstate = CAN1_SetTxBufPriority(1,2); 
  CANstate = CAN1_SetTxBufPriority(2,3); 
}