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
    tmp_8 = NULL;
    tmp_16 = NULL;

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
    tmp_8 = NULL;
    tmp_16 = NULL;

}

ConstBlock::~ConstBlock(){
  if(tmp_8    != NULL)delete tmp_8;
  if(tmp_16   != NULL)delete tmp_16;
  //if(t_output != NULL)delete t_output;
}

void ConstBlock::setTableOutput(int32_t *tmp, byte N){
  t_output = tmp;
  N_output = N;
}

int32_t* ConstBlock::get_table_output(){return(t_output);}

byte ConstBlock::get_output_N(){return(N_output);}

byte* ConstBlock::get_table_output_8(){
  if(N_output > 0){
  tmp_8 = new uint8_t[N_output]; 
  for(byte rr = 0; rr<N_output;rr++){
    tmp_8[rr] = (uint8_t)t_output[rr];

  }
  return(tmp_8);
}else{
  return(NULL);
}
  
}

int16_t* ConstBlock::get_table_output_16() {
  if(N_output > 0) {
    tmp_16 = new int16_t[N_output]; 
    for(byte rr = 0; rr<N_output;rr++) {
      tmp_16[rr] = (int16_t) t_output[rr];
  }
  return(tmp_16);
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
