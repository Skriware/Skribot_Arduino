#include "AritmeticBlock.h"

AritmeticBlock::~AritmeticBlock(){}

AritmeticBlock::AritmeticBlock(byte _id, byte _operation,byte _left_id,byte _rigth_id){
	blockID = _id;
	leftID 	= _left_id;
	rightID = _rigth_id;
	operation = _operation;
	input_left 	= NULL;
	input_right = NULL;
  inputblockID = 0;
  outputblockID = 0;
  nextblockID = 0;
}
  bool AritmeticBlock::set_connections(Block *blockList[],int blockList_N){
  		for(int ii = 0; ii < blockList_N; ii++){
			     if(blockList[ii]->getID() == leftID){
			            input_left = blockList[ii];
			      }else if(blockList[ii]->getID() == rightID){
			            input_right = blockList[ii];
			      }
  		}
      if(input_left != NULL && input_right != NULL){
        return(true);
      }else{
        return(false);
      }
  }
  int32_t AritmeticBlock::get_output(){
  	int32_t val_right,val_left;
  	if(input_left   != NULL)val_left    = input_left->get_output();
	  if(input_right  != NULL)val_right   = input_right->get_output();

	  switch (operation) {
    case 1:
      output = val_left + val_right;
      break;
    case 2:
      output = val_left - val_right;
      break;
    case 3:
      output = val_left * val_right;
      break;
    case 4:
      output = val_left / val_right;
      break;
    default:
      output = 0;
  	}
    #ifdef DEBUG_MODE
  	Serial.print("Aritmetic output:");
  	Serial.println(output);
    #endif

  	return(output);
  

  }
