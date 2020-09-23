#ifndef Loop_H
#define Loop_H

#include "Block.h"

class Loop : public Block {
public:
  Loop();
  ~Loop();
  Loop(byte,byte,byte,byte);
  Loop(byte);

  void do_action();
  Block* get_next();

  void set_begin_block(Block*);
  void set_end_block(Block*);
  void set_limit_block(Block*);
  void set_limit();
  bool set_connections(Block *blockList[],int);

  byte get_end_block_id();
  byte get_begin_block_id();
  byte get_limit_block_id();

protected:
  Block* begin_block;
  Block* end_block;
  Block* limit_block;

  byte begin_block_id;
  byte end_block_id;
  byte limit_block_id;
  int counter = 0;
  int limit;
};

#endif
