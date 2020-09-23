#ifndef DEFINE_INTERRUPT_H
#define DEFINE_INTERRUPT_H
#include "Arduino.h"
#include "Block.h"

#define BUTTON_INTERRUPT 2			//interrupt types
#define DISTANCE_INTERRUPT 1
#define LINE_INTERRUPT 0
#define TIME_INTERRUPT 3

#define DIST_D1 	0					//interrupt inputs
#define DIST_D2 	1
#define LINE_L1 	1
#define LINE_L2 	2
#define LINE_L3 	3
#define TIME    	5
#define BUTTON_1	1
#define BUTTON_2	2
#define BUTTON_3	3					

										//interrupt triggers
#define DISTANCE_GRATER_THEN 0
#define DISTANCE_LESS_THEN 1
#define LINE_DETECTED 2
#define NO_LINE_DETECTED 3
#define BUTTON_PRESSED 4
#define BUTTON_HOLD 5
#define TIME_PASSED 6

class InterruptHandler {
public:
	InterruptHandler(byte int_type,
					  byte int_input,
					  byte trigger,
					  byte _priority,
					  byte _starting_block_id);
	~InterruptHandler();
	bool Check_for_interrupt();
	bool Condition_saniti_check();
	byte get_start_block_id();
	Block* get_starting_Block();
	bool set_start_block(Block* blockList[],int blockList_N);
	byte get_priority();
	Block* get_interrupted_block();
	void set_interrupted_block(Block *tmp);
	void setValue(int v);

protected:
	byte interrupt_type;
	byte starting_block_id;
	byte priority;
	byte trigger_type;
	byte input;
	int value;
	bool sanity_check;
	Block *starting_block;
	Block *last_interrupted_Block;
	Skribot *robot;
	long last_interrupt_time;
	};

#endif
