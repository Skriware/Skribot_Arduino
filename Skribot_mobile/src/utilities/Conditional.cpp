#include "Conditional.h"

Conditional::~Conditional() {}


Conditional::Conditional(byte id,byte _next_true, byte _next_false, byte _logic_block){
  next_True_ID  = _next_true;
  next_False_ID = _next_false;
  first_LogicblockID = _logic_block;
  blockID = id;
  actionID = 255;
  input_block = NULL;
  next = NULL;
  output_block = NULL;
  next_true = NULL;
  next_false = NULL;
  logic_block = NULL;
  inputblockID = 0;
  outputblockID = 0;
  nextblockID = 0;

}

bool Conditional::set_logics(Block* blockList[], int blockList_N,LogicBlock* logicblockList[], int logicblockList_N){
  for(int ii = 0; ii < blockList_N; ii++){
      if(blockList[ii]->getID() == next_True_ID){
            next_true = blockList[ii];
      }else if(blockList[ii]->getID() == next_False_ID){
            next_false = blockList[ii];
      }else if(blockList[ii]->getID() == first_LogicblockID){
            logic_block = (LogicBlock*)blockList[ii];
      }
   
  }

  if(logic_block != NULL && (next_false != NULL || next_False_ID == 0) && next_true != NULL){
    return(true);
  }else{
    return(false);
  }

}




void Conditional::do_action() {

}

Block* Conditional::get_next() {
  if(logic_block->get_bool_output())
    return next_true;
  else
    return next_false;
}
