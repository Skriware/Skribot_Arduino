#ifndef ConstBlock_H
#define ConstBlock_H
#include "Block.h"
class ConstBlock : public Block {

public:
  ConstBlock(int id,int32_t const);
  ConstBlock(int id, int32_t *value,byte N);
  ~ConstBlock(); 
  int32_t* get_table_output();
  byte* get_table_output_8();
  int16_t* get_table_output_16();
  void setTableOutput(int32_t *tmp, byte N);
  byte get_output_N();
  int32_t *t_output;
  byte N_output;

};

#endif