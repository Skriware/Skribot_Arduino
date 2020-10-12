#ifndef Block_Handler
#define Block_Handler
#include "Block.h"
#include "Loop.h"
#include "Conditional.h"
#include "LogicBlock.h"
#include "ConstBlock.h"
#include "LogicCompare.h"
#include "AritmeticBlock.h"
#include "InterruptHandler.h"

#define INVALID_MSG_ERROR_CODE 255
#define TIMEOUT_ERROR_CODE 254
#define CONN_BRK_CODE 253
#define MESSAGE_TIMEOUT 2000
#define ACK_RESEND_TIME 5
#define AFTER_TIMOUT_DELAY_INCREASE 5
#define NO_MSG_CODE '0'
#define CODE_COMPLETE 2
#define CODE_PASSED 1
#define MAX_ACK_RESENT_TIME 500

//Line Tags
#define BAPTISED 'b'
#define RENAME 'R'
#define VERSION 'V'
#define RESET 'r'
#define END 'E'
#define BEGIN 'B'
#define BATTERY 'L'
#define REMOTE 'M'
#define GRIPPER 'G'
#define PIANO 'P'
#define HARDWARE_SET 'H'
#define CALIBRATE 'C' 

#define CODE_VALID_AND_COMPLETE_RUN_O 1
#define CODE_VALID_AND_COMPLETE_RUN_C 0
#define CODE_NOT_VALID 3
#define CODE_TOO_LONG 4
#define CODE_NOT_COMPLETE 2

class BlockHandler {


public:
	BlockHandler();

	bool addLoop(int id,	int startBlockID,	int endBlockID,		int count);
	bool addBlock(int id,	int _nextBlockID,int _actionID,byte* _usedBlocksIDs= NULL,byte _NusedBlocks = 0);
	bool addIf(int id,int _next_true, int _next_false, int _logic_block);
	bool addLogic(int id,int logicOperation,int _input_left, int _input_right);
	bool addLogicCompare(int id,int _logicOperation,int _input_left, int _input_right);
	bool addInterrupt(byte type,byte input,byte trigger,byte _priority,byte _starting_block,int value = 0);
	bool addAritmeticBlock(int id,int _operation,int _left,int _right);
	bool addConst(int id, int32_t value);
	bool addConst(int id, int32_t *value,byte N);
	bool addConst(int id,String value);	
	bool checkForInterrupts();																		//Adding Blocks types
	
	bool active_wait(uint32_t ms,int interval,bool interruppted = false,bool *int_info = NULL);

	bool MakeConections();
	bool doBlock(bool loopmode = false);
	
	byte readMessageLine();
	byte readCodeLine();
	void processMessageLine(byte LineCode);
	bool AddToMessage(char x);
	void CheckLongCodes(char *x);
	bool CheckForTimeout();																							//Making conectons(assigning pointers from IDs) 
																												// Starting code
	

	int freeRam();
	int cti(char c);
	int32_t readInt();
	int32_t readIntDirect(byte max_d = 10);
	char BLE_readwithTIMEOUT();
	byte* readMultipleInts(byte *n);
	int32_t* readMultipleInts32(byte *N);																							//BT message parsing help functions
	int Handle_Msg();																							// Handle one line of the message
	void ReadfromBT();	
	void clear();	
	void init();
	int portUID(byte portID);																					// BT message hendler												
	bool Check_CON_BREAK(char A);
	bool runCode,transfereBlocks,O_got;
	int ack_resend_time;

	 #define blockList_MAX 			300
	 #define IfblockList_MAX 		50
	 #define LoopblockList_MAX 		50
	 #define LogicblockList_MAX 	50
	 #define AritmeticblockList_MAX	50
	 #define MAX_INTERRUPTS 8
	 #define MAX_MSG_L 2500

	Block *blockList[blockList_MAX];
	Conditional *IfblockList[IfblockList_MAX];
	Loop *LoopblockList[LoopblockList_MAX];
	AritmeticBlock *AritmeticblockList[AritmeticblockList_MAX];
	LogicBlock *LogicblockList[LogicblockList_MAX];
	Block *current;
	Block *StartBlock;
	InterruptHandler *Interrupts[MAX_INTERRUPTS];
	byte interrupt_running;
	byte interrupts_N;
	uint32_t millis_left_from_interrupt;
	byte interrupted_step;
	bool interruped_precesed;
	int blockList_N;
	int IfblockList_N;
	int LoopblockList_N;
	int LogicblockList_N;
	int AritmeticblockList_N;					//Actual number of blocks in lists
	int Mcursor;								// variable for message parsing
	char AllMessage[MAX_MSG_L];
	int messageLength;

};

#endif