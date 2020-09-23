	#include "BlockHandler.h"

	BlockHandler::BlockHandler(){
		init();
    ack_resend_time = ACK_RESEND_TIME;
	}

	void BlockHandler::init(){
		runCode = false;
		for(int tt = 0; tt < blockList_MAX; tt++){
			if(tt < IfblockList_MAX)IfblockList[tt] = NULL;
			if(tt < LoopblockList_MAX)LoopblockList[tt] = NULL;
			if(tt < LogicblockList_MAX)LogicblockList[tt] = NULL;
			if(tt < AritmeticblockList_MAX)AritmeticblockList[tt] = NULL;
      if(tt < MAX_INTERRUPTS)Interrupts[tt] = NULL;	
			blockList[tt] = NULL;
		}
	current = NULL;
   StartBlock = NULL;
	 blockList_N 		= 0;
	 IfblockList_N 	 = 0;
	 LoopblockList_N 	= 0;
	 LogicblockList_N 	= 0;
	 AritmeticblockList_N 	= 0;
	 Mcursor = 0;
	 messageLength = 0;
   interrupt_running = MAX_INTERRUPTS;
   interrupts_N = 0;
   interruped_precesed = false;
   millis_left_from_interrupt = 0;
   interrupted_step = 0;
   transfereBlocks = false;
	}

  void BlockHandler::clear(){
    runCode = false;
    for(int tt = 0; tt < blockList_MAX; tt++){
      delete blockList[tt];
    }
    for(byte yy = 0; yy <MAX_INTERRUPTS;yy++)delete Interrupts[yy];
   init();
  }
	bool BlockHandler::addLoop(int id,	int startBlockID,	int endBlockID,		int count){
		 Loop *l = new Loop(id,startBlockID,endBlockID,count);
      	if(blockList_N < blockList_MAX && LogicblockList_N < LogicblockList_MAX){
         blockList[blockList_N] = l;
      	 blockList_N++;
      	 LoopblockList[LoopblockList_N] = l;
      	 LoopblockList_N++;
         return(true);
       }
       return(false);
	}

	bool BlockHandler::addBlock(int id,  int _nextBlockID,int _actionID,byte* _usedBlocksIDs,byte _NusedBlocks){
		Block *block = new Block(id,_nextBlockID,_actionID,_usedBlocksIDs,_NusedBlocks);
      	if(blockList_N < blockList_MAX){
          blockList[blockList_N] = block;
      	 blockList_N++;
         return(true);
        }
          return(false);
	}

	bool BlockHandler::addIf(int id,int _next_true, int _next_false, int _logic_block){
		Conditional *condition = new Conditional(id,_next_true,_next_false, _logic_block);
      	if(blockList_N < blockList_MAX && IfblockList_N < IfblockList_MAX){
          blockList[blockList_N] = condition;
        	blockList_N++;
        	IfblockList[IfblockList_N] = condition;
        	IfblockList_N++;
          return(true);
        }
        return(false);
      	
	}

	bool BlockHandler::addLogic(int id,int logicOperation,int _input_left, int _input_right){
		LogicBlock *lblock = new LogicBlock(id,logicOperation,_input_left,_input_right);
	if(blockList_N < blockList_MAX && LogicblockList_N < LogicblockList_MAX){
    blockList[blockList_N] = lblock;
    blockList_N++;
		LogicblockList[LogicblockList_N] =  lblock;
		LogicblockList_N++;
    return(true);
  }
    return(false);
  
		
	}

	bool BlockHandler::addLogicCompare(int id,int logicOperation,int _input_left, int _input_right){
		LogicCompare *lblock = new LogicCompare(id,logicOperation,_input_left,_input_right);
	if(blockList_N < blockList_MAX && LogicblockList_N < LogicblockList_MAX){
    blockList[blockList_N] = lblock;
    blockList_N++;
		LogicblockList[LogicblockList_N] =  lblock;
		LogicblockList_N++;
    return(true);
  }
    return(false);
  
	
	}

	bool BlockHandler::addConst(int id, int32_t value){
		ConstBlock *cblock = new ConstBlock(id,value);
		if(blockList_N < blockList_MAX){
      blockList[blockList_N] = cblock;
      blockList_N++;
      return(true);
    }

    return(false);

      
	}

  bool BlockHandler::addConst(int id, int32_t *value,byte N){
    ConstBlock *cblock = new ConstBlock(id,value,N);
    if(blockList_N < blockList_MAX){
      blockList[blockList_N] = cblock;
      blockList_N++;
      return(true);
    }
  return(false);

  }


	bool BlockHandler::addAritmeticBlock(int id,int _operation,int _left,int _right){
		AritmeticBlock *ablock = new AritmeticBlock(id,_operation,_left,_right);
		  if(blockList_N < blockList_MAX && AritmeticblockList_N < AritmeticblockList_MAX){
        blockList[blockList_N] = ablock;
    		AritmeticblockList[AritmeticblockList_N] = ablock;
    		blockList_N++;
    		AritmeticblockList_N++;
        return(true);
      }
        return(false);
	}

  bool BlockHandler::addInterrupt(byte type,byte input,byte trigger,byte _priority,byte _starting_block_id,int value){
    if(interrupts_N < MAX_INTERRUPTS){
      Interrupts[interrupts_N]= new InterruptHandler(type,input,trigger,_priority,_starting_block_id);
      Interrupts[interrupts_N]->setValue(value);
      interrupts_N++;
      return(true);
    }
    return(false);
  }

  bool BlockHandler::checkForInterrupts(){
    if(interrupt_running == MAX_INTERRUPTS && !interruped_precesed){
    byte interrupts_triggered = 0;
    byte triggered_interrupt_ids[MAX_INTERRUPTS];
    for(byte ii = 0; ii < interrupts_N; ii++){
      if(Interrupts[ii]->Check_for_interrupt()){
        triggered_interrupt_ids[interrupts_triggered] = ii;
        interrupts_triggered++;
      }
    }
    if(interrupts_triggered == 1){
      interrupt_running = triggered_interrupt_ids[0];
      return(true);
    }else if(interrupts_triggered ==0){
      interrupt_running = MAX_INTERRUPTS;
      return(false);
    }else if(interrupts_triggered >1){
      byte highest_priority_id = triggered_interrupt_ids[0];
      for(byte rr = 1; rr < interrupts_triggered;rr++){
        if(Interrupts[highest_priority_id]->get_priority() < Interrupts[triggered_interrupt_ids[rr]]->get_priority()){
          highest_priority_id = triggered_interrupt_ids[rr];
        }
      }
      interrupt_running = highest_priority_id;
      return(true);
    }
  }
  return(false);
}


	bool BlockHandler::MakeConections(){
		#if ENABLED(DEBUG_MODE)
      Serial.println("Making connections!");          
    #endif
		for(int ii = 0 ; ii <  blockList_N ; ii++){
			 if(!blockList[ii]->set_next(blockList,blockList_N))return(false);         
		  }
      #ifdef DEBUG_MODE
      Serial.println("Next OK");
      #endif

			for(int kk = 0 ; kk <  blockList_N ; kk++){
        Serial.println(blockList[kk]->getID());
			 if(!blockList[kk]->set_used_block(blockList,blockList_N))return(false);  
			}

			#ifdef DEBUG_MODE
        Serial.println("Used blocks OK");
        #endif

  		for(int kk = 0 ; kk < LogicblockList_N ; kk++){
  			if(!LogicblockList[kk]->set_logics(blockList,blockList_N))return(false);
  		}
#ifdef DEBUG_MODE
      Serial.println("Logic OK");
      #endif
  		for(int kk = 0 ; kk < IfblockList_N ; kk++){
  			if(!IfblockList[kk]->set_logics(blockList,blockList_N,LogicblockList,LogicblockList_N))return(false);
  		}
    #ifdef DEBUG_MODE
      Serial.println("IF OK");
    #endif
  		for(int jj = 0 ; jj < LoopblockList_N ; jj++){
  			if(!LoopblockList[jj]->set_connections(blockList,blockList_N))return(false);
  		}
      #ifdef DEBUG_MODE
      Serial.println("LOOP OK");
      #endif
  		for(int ll = 0; ll < AritmeticblockList_N ; ll++){
  			if(!AritmeticblockList[ll]->set_connections(blockList,blockList_N))return(false);
  		}
       #ifdef DEBUG_MODE
      Serial.println("Aritmetic OK");
      #endif
  		 for(int jj = 0 ; jj <  blockList_N ; jj++){
          if(1 == blockList[jj]->getID()){
            StartBlock = blockList[jj];
            break;
          }
		  }
      if(StartBlock == NULL)return(false);
		  current = StartBlock;

    #ifdef DEBUG_MODE
      Serial.println("Start OK");
    #endif
      Serial.println("interrupts OK");
      for(byte tt = 0; tt < interrupts_N;tt++){
        if(!Interrupts[tt]->set_start_block(blockList,blockList_N))return(false);
      }
      
    #if ENABLED(DEBUG_MODE)
      Serial.println("Connections done!");          
    #endif
      return(true);
	}
	bool BlockHandler::doBlock(bool loopmode){
    #ifdef DEBUG_MODE
          Serial.print("current:");
		   		Serial.println(current->getID());
    #endif

    Block::robot->wait_And_Check_BLE_Connection(5,2);      
    current->do_action();

    #ifdef DEBUG_MODE
       Serial.print("next:");
			 Serial.println(current->getNextID());
    #endif

    checkForInterrupts();  //do not check interrupts if you are in one alerady
    if(interrupt_running !=MAX_INTERRUPTS && !interruped_precesed){
      current->interrupted = true;
      Interrupts[interrupt_running]->set_interrupted_block(current); 
      current = Interrupts[interrupt_running]->get_starting_Block();
      interruped_precesed = true;
    }else{
      current = current->get_next(); 
    }
        if (current == NULL){
          if(interrupt_running !=MAX_INTERRUPTS){
            current = Interrupts[interrupt_running]->get_interrupted_block();
            interrupt_running = MAX_INTERRUPTS;
            interruped_precesed = false;
          }else if(loopmode){
            current = StartBlock;
            return(true);
          }else{
            return(false);
          }
        }
        return(true);
    }


int BlockHandler::freeRam() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

int32_t BlockHandler::readInt(){
  int nDigits = 0;
  int sign = 1;
  while((AllMessage[Mcursor + nDigits] != ' ') && (AllMessage[Mcursor + nDigits] != '\n')){
      if(AllMessage[Mcursor + nDigits] == '-')sign = -1;
      nDigits++;
  }
  if(sign==-1){
    Mcursor++;
    nDigits--;
  }
  int32_t out = 0;
  int32_t power = 1;
    for(int ii = nDigits-1; ii > -1; ii--){
      if(ii != nDigits-1) power *=10;
      int32_t add = cti(AllMessage[Mcursor + ii])*power;
      out += add;
    }
    #ifdef DEBUG_MODE
    //Serial.println(out*sign);
    #endif
    Mcursor += nDigits+1;
    return(out*sign);
}


int32_t* BlockHandler::readMultipleInts32(byte *N){
  int start_cursor_pos = Mcursor;
  byte intsToRead = 0;
  while(AllMessage[Mcursor-1] != '\n'){
    readInt();
    intsToRead++;
  }
if(intsToRead !=0){
  int32_t *tmp = new int32_t[intsToRead];
  Mcursor = start_cursor_pos;
  for(byte yy = 0; yy < intsToRead; yy++)tmp[yy] = readInt();
    *N = intsToRead;
    return(tmp);
}else{
  *N = 0;
  return(NULL);
}
  *N = 0;
  return(NULL);
}

byte* BlockHandler::readMultipleInts(byte *N){
  int start_cursor_pos = Mcursor;
  byte intsToRead = 0;
  while(AllMessage[Mcursor-1] != '\n'){
    readInt();
    intsToRead++;
  }
if(intsToRead !=0){
  byte *tmp = new byte[intsToRead];
  Mcursor = start_cursor_pos;
  for(byte yy = 0; yy < intsToRead; yy++)tmp[yy] = readInt();
    *N = intsToRead;
    return(tmp);
}else{
  *N = 0;
  return(NULL);
}
  *N = 0;
  return(NULL);
}

int BlockHandler::cti(char x){
  int y = x - '0';
  return(y);
}

int BlockHandler::Handle_Msg(){
  int id;
  bool codeOk,memoryOk;
  memoryOk = true;
  if(AllMessage[Mcursor] == 'R'){
             codeOk = MakeConections();
              if(AllMessage[Mcursor+4] == 'C'){
              	if(codeOk){
                  return(CODE_VALID_AND_COMPLETE_RUN_C);
                }else{
                  return(CODE_NOT_VALID);
                }
              }else{
                if(codeOk){
              	 return(CODE_VALID_AND_COMPLETE_RUN_O);
                }else{
                  return(CODE_NOT_VALID);
                }
               }
  }
          id = readInt();
          byte startBlockID;
          byte endBlockID; 
          int countID; 
          int32_t value;
          byte actionID;
          byte input;
          byte next;
          byte nextTrue,nextFalse,logicBlock;
          byte input_left,input_right, compareOperation;
          byte type,trigger,priority, starting_block;
          byte *tmp_b;
          int32_t *tmp_32;
  switch(AllMessage[Mcursor]){
    case 'L':
          Mcursor += 2;
          countID = readInt();
          startBlockID  = readInt();
          endBlockID = readInt();
          memoryOk = addLoop(id,startBlockID,endBlockID,countID);
          break;
     case 'C':
          Mcursor += 2;
          if(AllMessage[Mcursor] == 'I'){
            value = -1;
            Mcursor += 2;
          }else if(AllMessage[Mcursor] == 'T'){
            value = 1;
            Mcursor += 2;
          }else if(AllMessage[Mcursor] == 'F'){
            value = 0;
            Mcursor += 2;
          }else{
            tmp_32 = readMultipleInts32(&input);
            if(input == 1){
            memoryOk = addConst(id,tmp_32[0]);
            }else if(input >1){
            memoryOk = addConst(id,tmp_32,input);
            }
            break;
          }
          memoryOk = addConst(id,value);
          break;
      case 'I':
          Mcursor += 2;
          logicBlock = readInt();
          nextTrue = readInt();
          nextFalse = readInt();
          memoryOk = addIf(id,nextTrue,nextFalse,logicBlock);
      break;

      case 'O':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	memoryOk = addAritmeticBlock(id,compareOperation,input_left,input_right);
      break;
      case 'K':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	memoryOk = addLogic(id,compareOperation,input_left,input_right);
      break;
      case 'U':
      	Mcursor += 2;
      	compareOperation = readInt();
      	input_left = readInt();
      	input_right = readInt();
      	memoryOk = addLogicCompare(id,compareOperation,input_left,input_right);
      break;
      case 'J':
          Mcursor += 2;
          next = readInt();
          memoryOk = addBlock(id,next,69);
       break;
      case 'A':
          Mcursor +=2;
          actionID = readInt();
          next  = readInt();
          tmp_b = readMultipleInts(&input);
          memoryOk = addBlock(id,next,actionID,tmp_b,input);
          break;
      case 'V':
          Serial.println("Interrupt");
          Mcursor +=2;
          type = readInt();
          input = readInt();
          trigger = readInt();
          value = readInt();
          priority = readInt();
          starting_block = readInt();
          memoryOk =  addInterrupt(type,input,trigger,priority,starting_block,value);
          break;
      default:
            #if ENABLED(DEBUG_MODE)
              Serial.print("Got sth stragne:");
              Serial.println(AllMessage[Mcursor]);          
            #endif
        break;
  }
  if(!memoryOk)return(CODE_TOO_LONG);
  return(CODE_NOT_COMPLETE);    
}

bool BlockHandler::active_wait(uint32_t ms, int interval,bool interrupted,bool *interrupt_info){
    if(ms < 0) return(false);
    if(interrupted)ms = millis_left_from_interrupt;
    int loop_iterator = ms/interval;
    int ms_left_befor_loop = ms%interval;
    bool got_interrupt = false;
    if(millis_left_from_interrupt !=0 && interrupt_running == MAX_INTERRUPTS){
      loop_iterator = millis_left_from_interrupt/interval;
      ms_left_befor_loop = millis_left_from_interrupt%interval;
      millis_left_from_interrupt = 0;
    }
    delay(ms_left_befor_loop);
    for(int yy = 1; yy < loop_iterator; yy++){
          if(Block::robot->using_BLE_Connection && !Block::robot->ignore_connection_break && !Block::robot->connection_Break_Reported && Block::robot->BLE_checkConnection() == false){
          Block::robot->connection_Break_Reported = true;
          #ifdef DEBUG_MODE
          if(Block::robot->connection_Break_Reported)Serial.println("Connection LOST!");
          #endif
        }else if(Block::robot->using_BLE_Connection && !Block::robot->program_End_Reported && Block::robot->BLE_dataAvailable() > 0){
            char tmp;
            while(Block::robot->BLE_dataAvailable() > 0){
              tmp = Block::robot->BLE_read();
              delay(5);                     // to be sure that next char will be recieved
              if((tmp == 'E' && Block::robot->BLE_read() == 'N' && Block::robot->BLE_read() == 'D') || (tmp == 'B' && Block::robot->BLE_read() == 'E' && Block::robot->BLE_read() == 'G')){
                Block::robot->program_End_Reported = true;
              }
            }
      }
      if(Block::robot->program_End_Reported || Block::robot->connection_Break_Reported)break;
      if(checkForInterrupts()){
       Serial.println("Interrupts!");
        millis_left_from_interrupt = (loop_iterator - yy)*interval;
        if(interrupt_info != NULL) *interrupt_info = false;
        Serial.print("millis left:");
        Serial.println(millis_left_from_interrupt);
        got_interrupt = true;
        break;
      }
      if(Block::robot->stausLEDused)Block::robot->status->ReadBatteryState();
      delay(interval);
    } 
    return(got_interrupt);
}


  