#include "ConstBlock.h"

ConstBlock::ConstBlock(int id, int32_t value){
	blockID = id;
	output = value;
	  input_block 	= NULL;
  	next 			= NULL;
  	output_block 	= NULL;
  	inputblockID 	= 0;
  	outputblockID 	= 0;
  	actionID 		= 0;
  	nextblockID 	= 0;
    t_output = NULL;
    N_output = 1;
}

ConstBlock::ConstBlock(int id, int32_t *value,byte N){
    blockID = id;
    output = 0;
    input_block   = NULL;
    next      = NULL;
    output_block  = NULL;
    inputblockID  = 0;
    outputblockID   = 0;
    actionID    = 0;
    nextblockID   = 0;
    t_output = value;
    N_output = N;
}

ConstBlock::~ConstBlock(){}

void ConstBlock::setTableOutput(int32_t *tmp, byte N){
  t_output = tmp;
  N_output = N;
}

int32_t* ConstBlock::get_table_output(){return(t_output);}

byte ConstBlock::get_output_N(){return(N_output);}

byte* ConstBlock::get_table_output_8(){
  if(N_output > 0){
  uint8_t *tmp = new uint8_t[N_output]; 
  for(byte rr = 0; rr<N_output;rr++){
    tmp[rr] = (uint8_t)t_output[rr];

  }
  return(tmp);
}else{
  return(NULL);
}
  
}

int16_t* ConstBlock::get_table_output_16() {
  if(N_output > 0) {
    int16_t *tmp = new int16_t[N_output]; 
    for(byte rr = 0; rr<N_output;rr++) {
      tmp[rr] = (int16_t) t_output[rr];
  }
  return(tmp);
}else{
  return(NULL);
}
  
}

/*ConstBlock::ConstBlock(int id , String value){
	blockID = id;
	string_output = value;
	input_block = NULL;
  next = NULL;
  output_block = NULL;
}

String ConstBlock::get_string_output(){
  return(string_output);
}*/
