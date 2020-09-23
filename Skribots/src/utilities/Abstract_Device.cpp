#include <Arduino.h>
#include "Abstract_Device.h"

	bool Abstract_Device::verifyGPIO(byte *table){
		bool tmp = false;
		for(byte rr = 1; rr< table[0]+1;rr++){
			if(channel_pin == table[rr]){
				tmp = true;
				break;
			}
		}
		return(tmp);
	}

	bool Abstract_Device::channel_Init(){
		bool tmp = true; 
		#ifdef ESP_H
			byte ADC_PINS[]= {6,36,39,34,35,32,33};
			byte DAC_PINS[]= {2,26,25};
			byte TOUCH_PINS[]= {10,4,0,2,15,13,12,14,27,33,32}; 
		#else
			byte ADC_PINS[]= {7,A1,A2,A3,A4,A5,A6,A7};
		#endif
		switch(channel_id){
			case D_READ:
    			pinMode(channel_pin,INPUT);
    		break;	
			case D_WRITE:
				pinMode(channel_pin,OUTPUT);
			break;	
			case A_READ:
				tmp = verifyGPIO(ADC_PINS);
			break;	
			case PWM_WRITE:
				 if(SetNewPWMChannel(channel_pin) == -1)tmp=false;
			break;	
			case DAC_WRITE:
				#ifdef ESP_H
				tmp = verifyGPIO(DAC_PINS);
				#endif
			break;	
			case PULSE_IN:
				 pinMode(channel_pin,INPUT);
			break;
			case TOUCH:
				#ifdef ESP_H
				 tmp =verifyGPIO(TOUCH_PINS);
				#endif
			break;	
		}
		return(tmp);

	}

	Abstract_Device::Abstract_Device(byte _channel_pin, byte _channel_id, byte _id){
		channel_pin = _channel_pin;
		channel_id =  _channel_id;
		id = _id;
	}
	byte Abstract_Device::GetID(){
		return(id);
	} 
    uint32_t 	Abstract_Device::performChannelAction(byte input){
    	uint32_t output = 0;
    	switch(channel_id){
    		case D_READ:
    			output = digitalRead(channel_pin);
    		break;	
			case D_WRITE:
				digitalWrite(channel_pin,input);
			break;	
			case A_READ:
				output = analogRead(channel_pin);
			break;	
			case PWM_WRITE:
				PWM_Write(channel_pin,input);
			break;	
			case DAC_WRITE:
				#ifdef ESP_H
				dacWrite(channel_pin, input);
				#endif
			break;	
			case PULSE_IN:
				output = pulseIn(channel_pin,input,15000);
			break;
			case TOUCH:
				#ifdef ESP_H
				output = touchRead(channel_pin);
				#endif
			break;	
    	}
    	return(output);
    }
  	
