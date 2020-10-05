#include "Block.h"
#include "BlockHandler.h"
#define STOP 0
#define MOVE_FORWARD 1
#define MOVE_BACK 2
#define TURN_LEFT 3
#define TURN_RIGHT 4


Skribot* Block::robot = NULL;
BlockHandler* Block::BH = NULL;

Block::Block() {
  next = NULL;
  interrupted = false;
  action_with_no_interrupt = true;
  used_blocks_N = 0;
};

Block::~Block() {
  //TO DO - check if it gives memory licks
  //delete used_blocks;
  //delete used_blocksIDs;
}

Block::Block(Block* b) {
  next = b;
  next = NULL;
  actionID = 1000;
  interrupted = false;
  action_with_no_interrupt = true;
  used_blocks_N = 0;
}

Block::Block(byte id,byte _nextBlockID,int _actionID,byte *_usedblockIDs,byte Nused_blocks){
  blockID = id;
  actionID = _actionID;
  nextblockID = _nextBlockID;
  used_blocksIDs = _usedblockIDs;
  used_blocks_N = Nused_blocks;
  next = NULL;
  interrupted = false;
  action_with_no_interrupt = true;
}

byte Block::getActionID(){
  return(actionID);
}

byte Block::getInputID(){
  return(inputblockID);
}

byte Block::getOutputID(){
  return(outputblockID);
}

byte Block::getID(){
  return(blockID);
}

byte Block::getNextID(){
  return(nextblockID);
}

 void Block::do_action() {
 /*
    0 up regular actions

    100 up reading from sensors actions

    100 down custom actions(for pilot control)
 */ 
action_with_no_interrupt = true;
        byte font[][8] = {{ 0x7e, 0x7e, 0x66, 0x66, 0x7e, 0x7e, 0x66, 0x66 }, // A
                          { 0x78, 0x7c, 0x6c, 0x7c, 0x7e, 0x66, 0x7e, 0x7c }, // B
                          { 0x7e, 0x7e, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x7e }, // C
                          { 0x7c, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x7e, 0x7c }, // D
                          { 0x7e, 0x7e, 0x60, 0x78, 0x78, 0x60, 0x7e, 0x7e }, // E
                          { 0x7e, 0x7e, 0x60, 0x78, 0x78, 0x60, 0x60, 0x60 }}; // F
byte *tmp;
char *tmp_c;
SmartRotorSystem::Which which;
size_t tmp_n;
  switch(actionID){
     case STOP:
      if (Block::robot->smartRotor != nullptr)
      {
        Block::robot->smartRotor->stop();
      }
      else
      {
        Block::robot->Stop();
      }
      break;
    case MOVE_FORWARD:
          Block::robot->SetSpeed(used_blocks[1]->get_output());
          Block::robot->MoveForward();
          if(used_blocks[0]->get_output() != -1){
            Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
            Block::robot->Stop();
          }
        break;
    case MOVE_BACK:
          Block::robot->SetSpeed(used_blocks[1]->get_output());
          Block::robot->MoveBack();
          if(used_blocks[0]->get_output() != -1){
            Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
            Block::robot->Stop();
          }         
        break;
    case TURN_LEFT:
          Block::robot->SetSpeed(used_blocks[1]->get_output());
          Block::robot->FaceLeft();
          if(used_blocks[0]->get_output() != -1){
            Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
            Block::robot->Stop();
          }
        break;
    case TURN_RIGHT:
          Block::robot->SetSpeed(used_blocks[1]->get_output());
          Block::robot->FaceRight();
          if(used_blocks[0]->get_output() != -1){
            Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
            Block::robot->Stop();
          }
        break;
    case 5:
        Block::BH->active_wait(used_blocks[0]->get_output(),10,interrupted,&action_with_no_interrupt);
        break;
    case 6:
         Block::robot->Stop();
         Block::BH->active_wait(10,10,interrupted,&action_with_no_interrupt);
        break;
    case 8:
        Block::robot->CloseClaw();
        Block::BH->active_wait(100,10,interrupted,&action_with_no_interrupt);
        break;
    case 9:
          Block::robot->OpenClaw();
          Block::BH->active_wait(100,10,interrupted,&action_with_no_interrupt);
        break;
    case 10:
          Block::robot->Pick_Up();
          Block::BH->active_wait(100,10,interrupted,&action_with_no_interrupt);
        break;
    case 11:
        Block::robot->Put_Down();
        Block::BH->active_wait(100,10,interrupted,&action_with_no_interrupt);
        break;
    case 12:
         if(used_blocks[1]->get_output_N() == 1){
         switch(used_blocks[1]->get_output()){
            case 0: 
                Block::robot->TurnLEDOn(255,0,0,used_blocks[0]->get_output());
            break;
             case 1: 
                  Block::robot->TurnLEDOn(0,0,255,used_blocks[0]->get_output());
            break;
             case 2: 
                Block::robot->TurnLEDOn(0,255,0,used_blocks[0]->get_output());
            break;
             case 4: 
                Block::robot->TurnLEDOn(255,0,255,used_blocks[0]->get_output());
            break;
             case 5: 
                  Block::robot->TurnLEDOn(255,255,255,used_blocks[0]->get_output());
            break;
             case 3: 
                Block::robot->TurnLEDOn(184, 255, 3,used_blocks[0]->get_output());
            break;
            default:
            break;
          }
        }else if(used_blocks[1]->get_output_N() == 3){
                tmp = used_blocks[1]->get_table_output_8();
                Block::robot->TurnLEDOn(tmp[0],tmp[1],tmp[2],used_blocks[0]->get_output());
        }
           Block::BH->active_wait(10,10,interrupted,&action_with_no_interrupt);
        break;
    case 13:
        Block::robot->TurnLEDOff();
        break;
    case 14:
      {
          tmp = used_blocks[1]->get_table_output_8();
          
          if(used_blocks[0]->get_output() != 2){
          tmp_n = robot->LED_Matrixes[used_blocks[0]->get_output()]->SetAnimation(0,tmp,(size_t)used_blocks[1]->get_output_N());  
          robot->LED_Matrixes[used_blocks[0]->get_output()]->PlayAnimation(0);
          
          }else{
            tmp_n = robot->LED_Matrixes[0]->SetAnimation(0,tmp,(size_t)used_blocks[1]->get_output_N());
            tmp_n = robot->LED_Matrixes[1]->SetAnimation(0,tmp,(size_t)used_blocks[1]->get_output_N());
            robot->LED_Matrixes[0]->PlayAnimation(0);
            robot->LED_Matrixes[1]->PlayAnimation(0);
          }
          if(tmp_n > 1){
          for(byte pp = 0; pp< tmp_n;pp++){
           if(used_blocks[0]->get_output() !=2){
            robot->LED_Matrixes[used_blocks[0]->get_output()]->Update();
           }else{
            robot->LED_Matrixes[0]->Update();
            robot->LED_Matrixes[1]->Update();
           }
            if(Block::BH->active_wait(500,5,interrupted,&action_with_no_interrupt))break;
          }
        }else{
            if(used_blocks[0]->get_output() !=2){
            robot->LED_Matrixes[used_blocks[0]->get_output()]->Update();
           }else{
            robot->LED_Matrixes[0]->Update();
            robot->LED_Matrixes[1]->Update();
           }
            Block::BH->active_wait(50,5,interrupted,&action_with_no_interrupt);
        }
      }
        break;
    case 15:
    {
      uint8_t charCount = used_blocks[1]->get_output_N();

      if (charCount < 2)
      {
        char c = (char) used_blocks[1]->get_output();
        if (used_blocks[0]->get_output() != 2)
        {
          auto m = robot->LED_Matrixes[used_blocks[0]->get_output()];
          m->SetChar(0, c);
          m->Update();
        }
        else
        {
          auto m0 = robot->LED_Matrixes[0];
          auto m1 = robot->LED_Matrixes[1];

          m0->SetChar(0, c);
          m0->Update();
          m1->SetChar(0, c);
          m1->Update();
        }
      }
      else
      {
        tmp_c = (char*)used_blocks[1]->get_table_output_8();
        size_t frames = (charCount - 1) * 7; 
        if (used_blocks[0]->get_output() != 2)
        {
          robot->LED_Matrixes[used_blocks[0]->get_output()]->StartMarquee(tmp_c);
        }
        else
        {
          robot->LED_Matrixes[0]->StartMarquee(tmp_c);
          robot->LED_Matrixes[1]->StartMarquee(tmp_c);
        }
      
        for (int yy = 0; yy < frames; yy++)
        {
          if(used_blocks[0]->get_output() !=2)
          {
            robot->LED_Matrixes[used_blocks[0]->get_output()]->Update();
          }
          else
          {
            robot->LED_Matrixes[0]->Update();
            robot->LED_Matrixes[1]->Update();
          }

          if(Block::BH->active_wait(100,5,interrupted,&action_with_no_interrupt))break;
        }

        if (used_blocks[0]->get_output() != 2)
        {
          robot->LED_Matrixes[used_blocks[0]->get_output()]->StopMarquee();
        }
        else
        {
          robot->LED_Matrixes[0]->StopMarquee();
          robot->LED_Matrixes[1]->StopMarquee();
        }

      //ShowText
      }
    }
    break;
    case 16:
         
          //showVariable
        break;
    case 17:
          robot->LED_Matrixes[used_blocks[0]->get_output()]->ClearDisplay(0);
          robot->LED_Matrixes[used_blocks[0]->get_output()]->StopAnimation(0);
          robot->LED_Matrixes[used_blocks[0]->get_output()]->Update();
          Block::BH->active_wait(10,5,interrupted,&action_with_no_interrupt);
        break;
    case 18:
    {
      auto *buzzer = Block::robot->Buzzers[SERVO_2];
      if (buzzer == nullptr)
        break;

      switch (used_blocks[0]->get_output()) {
        case 0: buzzer->StopNote(); break;
        case 1: buzzer->PlayNote("C4"); break;
        case 2: buzzer->PlayNote("C#4"); break;
        case 3: buzzer->PlayNote("D4"); break;
        case 4: buzzer->PlayNote("D#4"); break;
        case 5: buzzer->PlayNote("E4"); break;
        case 6: buzzer->PlayNote("F4"); break;
        case 7: buzzer->PlayNote("F#4"); break;
        case 8: buzzer->PlayNote("G4"); break;
        case 9: buzzer->PlayNote("G#4"); break;
        case 10: buzzer->PlayNote("A4"); break;
        case 11: buzzer->PlayNote("A#4"); break;
        case 12: buzzer->PlayNote("B4"); break;
        case 13: buzzer->PlayNote("C5"); break;
        default:
          break;
      }
      Block::BH->active_wait(used_blocks[1]->get_output(),5,interrupted,&action_with_no_interrupt);
      buzzer->StopNote();
      break;
    }
    case 19:
        {
          //playMusic
          uint16_t *freqs = (uint16_t *) used_blocks[0]->get_table_output_16();
          uint16_t *delays = (uint16_t *) used_blocks[1]->get_table_output_16();
          size_t melodySize = (size_t) used_blocks[0]->get_output_N();
          int repeatCount = used_blocks[2]->get_output();
          auto *buzzer = Block::robot->Buzzers[SERVO_2];

          if (buzzer != nullptr)
          {
            buzzer->SetMelody(freqs, delays, melodySize);

            for(byte yy = 0; yy < repeatCount; yy++)
            {
              for (int i = 0; i < melodySize; i++)
              {
                uint16_t duration = buzzer->PlayNextNote();
                if (Block::BH->active_wait(duration, 5, interrupted, &action_with_no_interrupt))
                {
                  buzzer->StopNote();                 

                  /*
                  This `goto` is reasonable because we want to break out of a
                  nested loop and creating separate function just for this
                  purpose would require:

                    * many arguments to be passed,
                    * including Buzzer.h from another project
                      just to declare such a function.
                     
                  Moreover, the destination of this jump is not so far away,
                  just a few lines.
                  */
                  goto out_of_the_melody_loop;
                }
              }
            }

            out_of_the_melody_loop:
              Block::BH->active_wait(10, 5, interrupted, &action_with_no_interrupt);
          }
        }
        break;
    case 20:
      if (Block::robot->smartRotor != nullptr)
      {
          int speed = map(used_blocks[2]->get_output(), 0, 100, 60, 255);
          Block::robot->smartRotor->setSpeed(speed);
          which = (SmartRotorSystem::Which)used_blocks[0]->get_output();
          Block::robot->smartRotor->setDirection(1);
          Block::robot->smartRotor->moveByRevolutions((float)used_blocks[1]->get_output()/1000.0,which);
          while (Block::robot->smartRotor->isMoving())
            Block::BH->active_wait(10, 10,interrupted,&action_with_no_interrupt);
      }
      
        else
        {
            if(used_blocks[0]->get_output() == 1){
                  for(int kk = 0; kk < Block::robot->NLeftDCRotors ; kk++){
                    Block::robot->LeftDCRotors[kk]->SetDirection(1);
                    Block::robot->SetSpeed(used_blocks[2]->get_output());
                    Block::robot->LeftDCRotors[kk]->Move();
                  }
            }else if(used_blocks[0]->get_output() == 2){
                  for(int kk = 0; kk < Block::robot->NRightDCRotors ; kk++){
                    Block::robot->RightDCRotors[kk]->SetDirection(1);
                    Block::robot->SetSpeed(used_blocks[2]->get_output());
                    Block::robot->RightDCRotors[kk]->Move();
                  }
            }
          if(used_blocks[1]->get_output() != -1){
            Block::BH->active_wait(used_blocks[1]->get_output(),10,interrupted,&action_with_no_interrupt);
            Block::robot->Stop();
          }
        }
    
      break;
    case 21:
      if (Block::robot->smartRotor != nullptr)
      {
          int speed = map(used_blocks[2]->get_output(), 0, 100, 60, 255);
          which = (SmartRotorSystem::Which)used_blocks[0]->get_output();
          Block::robot->smartRotor->setDirection(0);
          Block::robot->smartRotor->setSpeed(speed);
          Block::robot->smartRotor->moveByRevolutions((float)used_blocks[1]->get_output()/1000.0,which);
          while (Block::robot->smartRotor->isMoving())
            Block::BH->active_wait(10, 10,interrupted,&action_with_no_interrupt);
      }
     
        else
        {
            if(used_blocks[0]->get_output() == 1){
                  for(int kk = 0; kk < Block::robot->NLeftDCRotors ; kk++){
                    Block::robot->LeftDCRotors[kk]->SetDirection(0);
                    Block::robot->SetSpeed(used_blocks[2]->get_output());
                    Block::robot->LeftDCRotors[kk]->Move();
                  }
            }else if(used_blocks[0]->get_output() == 2){
                  for(int kk = 0; kk < Block::robot->NRightDCRotors ; kk++){
                    Block::robot->RightDCRotors[kk]->SetDirection(0);
                    Block::robot->SetSpeed(used_blocks[2]->get_output());
                    Block::robot->RightDCRotors[kk]->Move();
                  }
            }
          if(used_blocks[1]->get_output() != -1){
            Block::BH->active_wait(used_blocks[1]->get_output(),10,interrupted,&action_with_no_interrupt);
            Block::robot->Stop();
          }
        }
      break;
    case 22:
        if (Block::robot->smartRotor != nullptr)
        {
          Block::robot->smartRotor->setDirection(1);
          Block::robot->smartRotor->setSpeed(map(used_blocks[1]->get_output(), 0, 100, 60, 255));
          Block::robot->smartRotor->moveByMeters((float)used_blocks[0]->get_output()/100.0);
          while (Block::robot->smartRotor->isMoving())
            Block::BH->active_wait(10, 10,interrupted,&action_with_no_interrupt);
        }
      break;
    case 23:
        if (Block::robot->smartRotor != nullptr)
          {
            Block::robot->smartRotor->setDirection(0);
            Block::robot->smartRotor->setSpeed(map(used_blocks[1]->get_output(), 0, 100, 60, 255));
            Block::robot->smartRotor->moveByMeters((float)used_blocks[0]->get_output()/100.0);
            while (Block::robot->smartRotor->isMoving())
              Block::BH->active_wait(10,10,interrupted,&action_with_no_interrupt);
          }
      break;
    case 24:
          if (Block::robot->smartRotor != nullptr)
          {
            Block::robot->smartRotor->setSpeed(map(used_blocks[1]->get_output(), 0, 100, 60, 255));
            Block::robot->smartRotor->turnByAngle(-used_blocks[0]->get_output());
            while (Block::robot->smartRotor->isMoving())
              Block::BH->active_wait(10, 10,interrupted,&action_with_no_interrupt);
          }
      break;
    case 25:
           if (Block::robot->smartRotor != nullptr)
          {
            Block::robot->smartRotor->setSpeed(map(used_blocks[1]->get_output(), 0, 100, 60, 255));
            Block::robot->smartRotor->turnByAngle(used_blocks[0]->get_output());
            while (Block::robot->smartRotor->isMoving())
              Block::BH->active_wait(10, 10,interrupted,&action_with_no_interrupt);
          }
      break;
      /**
    case 94:
        UserFunction_5(used_blocks[0]->get_output(),used_blocks[1]->get_output());
        break;
    case 95:
        used_blocks[1]->set_output(UserFunction_4());
        break;
    case 96:
        used_blocks[1]->set_output(UserFunction_3(used_blocks[0]->get_output()));
        break;
    case 97:
        UserFunction_2(used_blocks[0]->get_output());
        break;
    case 98:
        UserFunction_1();
        break;
        **/ 
   #ifdef ESP_H
    case 94:
        Block::robot->I2Ccomm[used_blocks[0]->get_output()]->SetFreq(used_blocks[1]->get_output());
        break;
    case 95:
        Block::robot->SPIcomm[used_blocks[0]->get_output()]->set_SPI_Settings(used_blocks[1]->get_output(),used_blocks[2]->get_output(),used_blocks[3]->get_output());
        break;
    case 96:
        Block::robot->I2Ccomm[used_blocks[0]->get_output()]->I2CTransfer(used_blocks[1]->get_table_output_8());
        if(used_blocks[3] != NULL)used_blocks[3]->set_output(Block::robot->I2Ccomm[used_blocks[0]->get_output()]->output_buffer[used_blocks[2]->get_output()]);
        break;
    case 97:
        Block::robot->SPIcomm[used_blocks[0]->get_output()]->SPITransfer(used_blocks[1]->get_table_output_8());
        if(used_blocks[3] != NULL)used_blocks[3]->set_output(Block::robot->SPIcomm[used_blocks[0]->get_output()]->output[used_blocks[2]->get_output()]);
        break;
    #endif
    case 98:
        used_blocks[2]->set_output(Block::robot->Devices[used_blocks[0]->get_output()]->performChannelAction(used_blocks[1]->get_output()));
        break;
    case 99:
        Block::robot->RawRotorMove(used_blocks[0]->get_output(),used_blocks[1]->get_output());
        Block::robot->Remote_block_used = true;
        break;
    case 101:
        used_blocks[1]->set_output(Block::robot->ReadDistSensor(used_blocks[0]->get_output()));
        break;
    case 102:
        used_blocks[1]->set_output((int) !Block::robot->ReadLineSensor(used_blocks[0]->get_output()));
        break;
    case 255:
      //Saved for loops and ifs
      break;
    default :
        break;

  }
  if(interrupted && action_with_no_interrupt)interrupted = false;
}

bool Block::set_next(Block* blockList[],int blockList_N) {
          if(getNextID() != 0){
            for(int jj = 0 ; jj <  blockList_N ; jj++){
                if(getNextID() == blockList[jj]->getID()){
                  next = blockList[jj];
                  return(true);
                }
            }
            return(false);
          }else{
            return(true);
          }
}



bool Block::set_used_block(Block* blockList[],int blockList_N){
          if(used_blocks_N == 0)return(true);
          byte usedBlockAttached = 0;
          byte Idswithout0s[used_blocks_N] = {0};
          byte used_nonzero_blocks_N =0;
           for(byte jj = 0; jj < used_blocks_N; jj++){
              if(used_blocksIDs[jj] != 0){
                    Idswithout0s[used_nonzero_blocks_N] = used_blocksIDs[jj];
                    used_nonzero_blocks_N++;
              }
            }
            if(used_nonzero_blocks_N == 0)return(true);
          used_blocks = new Block*[used_nonzero_blocks_N];
          for(byte uu = 0;uu<used_nonzero_blocks_N;uu++) used_blocks[uu] = NULL;
              for(byte tt = 0; tt < used_nonzero_blocks_N; tt++){
                for(int jj = 0 ; jj <  blockList_N ; jj++){
                  if(Idswithout0s[tt] == blockList[jj]->getID()){
                    used_blocks[tt] = blockList[jj];
                    usedBlockAttached++;
                    break;
                  }
              }
            }  
          if(usedBlockAttached == used_nonzero_blocks_N){
            return(true);
          }else{
            return(false);
          }
}

Block* Block::get_next() {
  return(next);
}

int32_t Block::get_output() {
  return output;
}

String Block::get_string_output(){
  return("");
}

void Block::setRobot(Skribot *_robot){
  Block::robot = _robot;
}

void Block::setBlockHandler(BlockHandler *bh){
  Block::BH = bh;
}


void Block::set_output(int x){
  output = x;
}

bool Block::get_bool_output(){
  return(false);
}

int32_t* Block::get_table_output(){return(NULL);}

byte* Block::get_table_output_8(){return(NULL);}

int16_t* Block::get_table_output_16(){return(NULL);}

byte Block::get_output_N(){return(0);}
