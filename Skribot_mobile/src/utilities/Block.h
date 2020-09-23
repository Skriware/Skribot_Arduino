#ifndef Block_H
#define Block_H
#include "Skribot.h"
//#include "UserFunctions.h"

class Block;
class BlockHandler;

class Block {
public:
	Block();
	Block(Block*);
	Block(byte id,byte _nextBlockID,int _actionID ,byte* _usedblockIDs = NULL,byte NusedBlocks = 0);
	virtual ~Block();
	void do_action();
	virtual	Block* get_next();
	virtual bool get_bool_output();
  	virtual int32_t get_output();
  	virtual int32_t* get_table_output();
  	virtual byte* get_table_output_8();
  	virtual int16_t* get_table_output_16();
  	virtual byte get_output_N();
  	void set_output(int x);
  	byte getID();
  	bool set_next(Block* blockList[],int blockList_N);
  	bool set_used_block(Block* blockList[],int blockList_N);
  	byte getNextID();
  	byte getInputID();
  	byte getOutputID();
  	byte getActionID();
  	bool interrupted,action_with_no_interrupt;
  	virtual String get_string_output();
  	static void setRobot(Skribot *rob);
  	static void setBlockHandler(BlockHandler *bh);
  	static Skribot *robot;
  	static BlockHandler *BH;



protected:
	byte blockID;
	int32_t output;
	Block** used_blocks;
	Block *input_block;
	Block *output_block;
	Block *next; 
	byte  *used_blocksIDs;
	byte used_blocks_N;
	byte inputblockID;
	byte outputblockID;
	byte nextblockID;
	byte actionID;
	};

#endif
