#include "InterruptHandler.h"

	InterruptHandler::InterruptHandler(byte int_type,byte int_input,byte trigger,byte _priority,byte _starting_block_id){
	interrupt_type = int_type;
	starting_block_id = _starting_block_id;
	priority = _priority;
	trigger_type = trigger;
	input = int_input;
	if(int_type == BUTTON_INTERRUPT){			//mapping for SKRIBRAIN
		switch(input){
			#ifdef ESP_H
			case BUTTON_1:
				input = SKRIBRAIN_ANALOG_PIN_1;
			break;
			case BUTTON_2:
				input = SKRIBRAIN_ANALOG_PIN_2;
			break;
			case BUTTON_3:
				input = SKRIBRAIN_ANALOG_PIN_3;
			break;
			#endif
			default:
				input;
			break;
		}
	}
	sanity_check = Condition_saniti_check();
	last_interrupted_Block = NULL;
	value = 0;
	}

	InterruptHandler::~InterruptHandler(){
		if(interrupt_type == BUTTON_INTERRUPT)buttonDisable(input);
	}

	bool InterruptHandler::Check_for_interrupt(){
		if(!sanity_check)return(false);
		byte distance;
		bool line_trig;
		switch(interrupt_type){
			case BUTTON_INTERRUPT:
				if(trigger_type == BUTTON_PRESSED){
					if(buttonEventPending(input)){
					 if(buttonPressed(input)){
					 	buttonClearEvent(input);
					 	return(true);
					 }else{
					 	buttonClearEvent(input);
					 	return(false);
					 }		
				}
				}else if(trigger_type == BUTTON_HOLD){
					 if(buttonHeld(input,value)){
					 	buttonClearEvent(input);
					 	return(true);
					 }else{
					 	buttonClearEvent(input);
					 	return(false);
					 }
				}
 			break;
 			case DISTANCE_INTERRUPT:
 				distance = robot->ReadDistSensor(input);
 				switch(trigger_type){
 					case DISTANCE_GRATER_THEN:
 						return(distance > value);
 					break;
 					case DISTANCE_LESS_THEN:
 						return(distance < value);
 					break;
 					default:
 						return(false);
 					break;
 				}

 			break; 
 			case LINE_INTERRUPT:
 				if(trigger_type == LINE_DETECTED){
 					line_trig = true;
 				}else if(trigger_type == NO_LINE_DETECTED){
 					line_trig = false;
 				}else{
 					return(false);
 				}
 				return(robot->ReadLineSensor(input) == line_trig);
 			break; 
 			case TIME_INTERRUPT :
 				if(millis()-last_interrupt_time > value){
 					last_interrupt_time = millis();
 					return(true);
 				}else{
 					return(false);
 				}
 			break;
 			default:
 				return(false);
 			break;

		}
			return(false);
	}
	bool InterruptHandler::Condition_saniti_check(){
	/*	if(interrupt_type == BUTTON_INTERRUPT){
			if(input == SKRIBRAIN_ANALOG_PIN_1 || input == SKRIBRAIN_ANALOG_PIN_2 || input == SKRIBRAIN_ANALOG_PIN_3){
				if(trigger_type == BUTTON_PRESSED || trigger_type == BUTTON_HOLD){
					buttonEnable(input);
					return(true);
				}
			}
		}else if(interrupt_type == DISTANCE_INTERRUPT){
			if(input == DIST_D1 || input == DIST_D2){
				if(trigger_type == DISTANCE_GRATER_THEN || trigger_type == DISTANCE_LESS_THEN){
					return(true);
				}
			}
		}else if(interrupt_type == LINE_INTERRUPT){
			if(input == LINE_L1 || input == LINE_L2 || input == LINE_L3){
				if(trigger_type == NO_LINE_DETECTED || trigger_type == LINE_DETECTED){
					return(true);
				}
			}
		}else if(interrupt_type == TIME_INTERRUPT){
			if(input == 0 && trigger_type == 0){
				return(true);
			}
		}
		#ifdef DEBUG_MODE
			Serial.println("Interrupt condition invalid");
		#endif
		*/
		return(true);
	}
	byte InterruptHandler::get_start_block_id(){
		return(starting_block_id);
	}
	Block* InterruptHandler::get_starting_Block(){
		return(starting_block);
	}

	bool InterruptHandler::set_start_block(Block* blockList[],int blockList_N){
		robot = Block::robot;
		if(starting_block_id != 0){
            for(int jj = 0 ; jj <  blockList_N ; jj++){
                if(starting_block_id == blockList[jj]->getID()){
                   starting_block = blockList[jj];
                  return(true);
                }
            }
            return(false);
          }else{
            return(true);
          }
	}

	byte InterruptHandler::get_priority(){
		return(priority);
	}

	Block* InterruptHandler::get_interrupted_block(){
		return(last_interrupted_Block);
	}

	void InterruptHandler::set_interrupted_block(Block *tmp){
		last_interrupted_Block = tmp;
	}
	void InterruptHandler::setValue(int val){
		value = val;
	}

	