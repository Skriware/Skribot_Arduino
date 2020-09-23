#include "LogicBlock.h"


LogicBlock::LogicBlock(byte id,byte _logics,byte _input_left, byte _input_right){
	input_Left_ID 	= _input_left;
	input_Right_ID 	= _input_right;
	blockID = id;
  operation = _logics;
  input_right = NULL;
  input_left = NULL;
  inputblockID = 0;
  outputblockID = 0;
  nextblockID = 0;
}

LogicBlock::LogicBlock(){
}

LogicBlock::~LogicBlock(){
}

  void LogicBlock::get_value(){
    Serial.println("Getting logic value!");
	  if(input_left   != NULL)val_left    = input_left->get_output();
	  if(input_right  != NULL)val_right   = input_right->get_output();

	  switch (operation) {
    case 0:
      bool_output = val_left == val_right;
      break;
    case 3:
      bool_output = val_left > val_right;
      break;
    case 2:
      bool_output = val_left < val_right;
      break;
    case 1:
      bool_output = val_left != val_right;
      break;
    default:
      bool_output = false;
  	}
  }
  bool LogicBlock::set_logics(Block *blockList[], int blockList_N){
  		for(int ii = 0; ii < blockList_N; ii++){
			     if(blockList[ii]->getID() == input_Left_ID){
			            input_left = blockList[ii];
			      }else if(blockList[ii]->getID() == input_Right_ID){
			            input_right = blockList[ii];
			      }
  		}
      if(input_right != NULL && input_left != NULL){
        return(true);
      }else{
        return(false);
      }
  }

  bool LogicBlock::get_bool_output(){
    get_value();
    Serial.println(blockID);
    Serial.print(" ");
    Serial.println(bool_output);
    return(bool_output);
  }
