#ifndef Abstract_D_H
#define Abstract_D_H
#include "Arduino.h"
#include "PWM_Handler.h"
#define D_READ 		1
#define D_WRITE 	2
#define A_READ 		3
#define PWM_WRITE 	4
#define DAC_WRITE 	5
#define PULSE_IN	6
#define TOUCH 7

class Abstract_Device{
  public:
    Abstract_Device(byte pins, byte action_id, byte _id);
    uint32_t 	performChannelAction(byte input = 0);
  	bool 		channel_Init();
    bool    verifyGPIO(byte *table);
    byte 	GetID();
  private:
  byte channel_pin;
  byte channel_id;
  byte output;
  byte id;
 };


#endif