/* Type of thermistor: MF514C01 */
static const word TempTable[51] =
{21736,21476,21212,20945,20676,20403,20128,19851,19572,19291, //0~9
 19008,18724,18439,18153,17866,17578,17290,17002,16714,16427, //10~19
 16140,15854,15569,15286,15000,14722,14443,14167,13891,13619, //20~29
 13349,13082,12816,12555,12296,12040,11788,11538,11292,11049, //30~39
 10811,10574,10343,10115,9890, 9669, 9453, 9240, 9029, 8825,  //40~49
 8623                                                        //50
};

//二分法找温度值

word CalTemp(word value,word down,word up) {
 
  if(value >= TempTable[(down+up)/2]){
  
   up = (down+up)/2;    //减小范围
   
   if((abs(up-down) == 1)) {
     if((value-TempTable[up]) < (value-TempTable[down]))
     return up;   
     else return down;
   }
   return CalTemp(value,down,up);
  }
 
  if(value <= TempTable[(down+up)/2]){
   
    down = (down+up)/2;  //减小范围
   
    if((abs(up-down) == 1)) { 
      if((value-TempTable[up]) < (value-TempTable[down]))
      return up;   
      else return down;
    }
   return CalTemp(value,down,up);
  }  
}