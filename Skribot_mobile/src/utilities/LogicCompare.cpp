#include "LogicCompare.h"

LogicCompare::LogicCompare(byte id, byte _logics,byte _input_left, byte _input_right){
	input_Left_ID 	= _input_left;
	input_Right_ID 	= _input_right;
	blockID = id;
  logicOperation = _logics;
  input_left = NULL;
  input_right = NULL;
  inputblockID = 0;
  outputblockID = 0;
  nextblockID = 0;
}

LogicCompare::~LogicCompare(){
}

  void LogicCompare::get_value(){
	  if(input_left   != NULL)val_L   = input_left->get_bool_output();
	  if(input_right  != NULL)val_R   = input_right->get_bool_output();
	  switch (logicOperation) {
    case 0:
      bool_output = val_L && val_R;
      break;
    case 1:
      bool_output = val_L || val_R;
      break;
    default:
      bool_output = false;
  	}
  }

  bool LogicCompare::get_bool_output(){
    get_value();
    return(bool_output);
  }
