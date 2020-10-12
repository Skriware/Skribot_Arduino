	#include "BlockHandler.h"
  #include "../Skribot_mobile.h"


  bool BlockHandler::Check_CON_BREAK(char A){
    if(O_got && A == 'K'){
      O_got = false;
      return(true);
    }
    if(A == 'O'){
      O_got = true;
    }else{
      O_got = false;
    }
    return(false);
  }
  int BlockHandler::portUID(byte portid){
        int primes[] = {2,3,5,7,11,13,17,19,23};
        return(primes[portid]);
  }

  bool BlockHandler::AddToMessage(char x){
    if(messageLength < MAX_MSG_L){
          AllMessage[messageLength] = x;
          messageLength++;
          #ifdef DEBUG_MODE
          Serial.print(x);
          #endif
          return (true);
    }
          return(false);
  }

  void BlockHandler::CheckLongCodes(char *asciTmp){
    if(*asciTmp == 'B'){                                                  //Baptised case 
          while(true){
            if(Block::robot->BLE_dataAvailable()){
              char tmp2 = Block::robot->BLE_read();
              if(tmp2 == 'A'){
                *asciTmp = 'b';
                break; 
              }else if(tmp2 == 'E'){
                break;
              }else{
                *asciTmp = INVALID_MSG_ERROR_CODE;
              }
            }
          }
        }else if(*asciTmp == 'R'){
          while(true){
            if(Block::robot->BLE_dataAvailable()){
              char tmp2 = Block::robot->BLE_read();
              while(Block::robot->BLE_dataAvailable() == 0);
                tmp2 = Block::robot->BLE_read();
              if(tmp2 == 'S'){
                *asciTmp = 'r';
                break; 
              }else if(tmp2 == 'N'){
                break;
              }else{
                *asciTmp = INVALID_MSG_ERROR_CODE;
              }
            }
          }
        }
  }

   bool BlockHandler::CheckForTimeout(){
            sei();
            bool tmp = false;
            bool ack_send = false;
            long time_passed = 0;
            while((Block::robot->BLE_dataAvailable() == 0)){
              time_passed++;
                //Serial.println(time_passed);
              if(time_passed > 160000){
                tmp = true;
                //if(ack_resend_time < MAX_ACK_RESENT_TIME)ack_resend_time += AFTER_TIMOUT_DELAY_INCREASE;
                break;
              }
              if(!ack_send && (time_passed > 10000)){
                Block::robot->BLE_write("ack\n");
                ack_send = true;
              }
            }
            return(tmp);
  }

  byte BlockHandler::readMessageLine(){
      char MainAsci,asciTmp;
      asciTmp = '0';
      if(Block::robot->BLE_dataAvailable()){
        MainAsci = Block::robot->BLE_read();                                 //Reading first character of the message 255-error Code
        byte rubbish = 0;
        if(MainAsci == 'O' || MainAsci == 'S'){
          while((rubbish < 6 || MainAsci == 'S') && (MainAsci == 'O' || rubbish < 18) ){
            if(Block::robot->BLE_dataAvailable()){
              Block::robot->BLE_read();
              rubbish++;
            }
            }
        return(NO_MSG_CODE);
        }
        CheckLongCodes(&MainAsci);
        Check_CON_BREAK(MainAsci);
        #ifdef DEBUG_MODE
        Serial.print("Got_Main:");
        Serial.println(MainAsci);
        #endif
        if(MainAsci == INVALID_MSG_ERROR_CODE)return(INVALID_MSG_ERROR_CODE);
        while(asciTmp != '\n' && MainAsci != 'C' && MainAsci != 'G'){
          if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
            #ifdef DEBUG_MODE
            Serial.print(asciTmp);
            #endif
            if(Check_CON_BREAK(asciTmp))return(CONN_BRK_CODE);
          }else{
           if(CheckForTimeout())return(TIMEOUT_ERROR_CODE);
          }
        }

        return(MainAsci);
      }else{
        return(NO_MSG_CODE);
      }
  }
  byte BlockHandler::readCodeLine(){
    char MainAsci,asciTmp;
    asciTmp = '0';
    if(Block::robot->BLE_dataAvailable()){
      MainAsci = Block::robot->BLE_read();                                 //Reading first character of the message 255-error Code
      if(MainAsci == 'O'){
        Block::robot->BLE_Flush();
        return(TIMEOUT_ERROR_CODE);
      }
      AddToMessage(MainAsci);
      Check_CON_BREAK(MainAsci);  
      asciTmp = MainAsci;
    while(asciTmp != '\n'){
          if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
            AddToMessage(asciTmp); 
             if(Check_CON_BREAK(asciTmp))return(CONN_BRK_CODE);
          }else{
            if(CheckForTimeout()){
              transfereBlocks = false;  
              return(TIMEOUT_ERROR_CODE);
            }
          }
        }
    }else{
      CheckForTimeout();
      return(NO_MSG_CODE);
    }
    if(MainAsci == 'R'){
        transfereBlocks = false;
        return(CODE_COMPLETE);
    }
    if(!(messageLength < MAX_MSG_L))return(CODE_TOO_LONG);
    return(CODE_PASSED);
  }
  void BlockHandler::processMessageLine(byte LineCode){
        char tmp = 'A';
        char tmpNameArray[32] = {' '};
        int tmpCounter;
        char tmp_tag[10] = {' '};
        byte hardware_types[15][6] = {{' '},{' '}};
        int n_hardware = 0;
        bool vaildcommand = true;
        int tmp_checksum = 0;
        byte n_pins;
        switch(LineCode){
          case BAPTISED:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("Named");
                #endif
                  Block::robot->sendNameInfo();
                  clear();
          break;
          case RENAME:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("NewName");
                #endif
                while(Block::robot->BLE_dataAvailable() == 0);
                tmp = Block::robot->BLE_read();
                tmpNameArray[32] = {' '};
                tmpCounter = 0;
                while(tmp != '\n'){
                  if(Block::robot->BLE_dataAvailable()){
                    tmpNameArray[tmpCounter] = tmp;
                    tmpCounter++;
                    tmp = Block::robot->BLE_read();
                  }
                }
                #if ENABLED(DEBUG_MODE)
                  Serial.println(tmpNameArray);
                #endif  
                Block::robot->BLE_changeName(tmpNameArray);
                clear();
                
          break;
          case VERSION:
                #if ENABLED(DEBUG_MODE)
                  Serial.println("Version Request");
                #endif
                sprintf(tmp_tag,"VERSION:2.1",FIRMWARE_VERSION);
                Block::robot->BLE_write(tmp_tag);
                clear();
          break;
          case RESET:
              #if ENABLED(DEBUG_MODE)
                Serial.println("RESET BLE");
              #endif
                Block::robot->BLE_reset();
                clear();
          break;
          case END:
                clear();
                Block::robot->Stop();
                #ifndef _VARIANT_BBC_MICROBIT_
                Block::robot->OpenClaw();
                Block::robot->Put_Down();
                #endif
          break;
          case BEGIN:
                transfereBlocks = true;
          break;
          case NO_MSG_CODE:
          
          break;
          case REMOTE:
          if(Block::robot->NLeftDCRotors ==0)Block::robot->AddDCRotor(EDU_ROTOR_SPEED_PIN_L,EDU_ROTOR_DIR_PIN_L,"Left"); ;
          if(Block::robot->NRightDCRotors ==0)Block::robot->AddDCRotor(EDU_ROTOR_SPEED_PIN_R,EDU_ROTOR_DIR_PIN_R,"Right");
          Block::robot->RawRotorMove(readIntDirect(),readIntDirect());
          break;
          case GRIPPER:
          if(Block::robot->NClaws == 0)Block::robot->AddClaw();
            tmp = BLE_readwithTIMEOUT();
            tmp = BLE_readwithTIMEOUT();
          switch(tmp){
              case  '1':
             Block::robot->CloseClaw();
            break;
               case '2':
             Block::robot->OpenClaw();
            break;
               case '3':
             Block::robot->Pick_Up();
            break;
               case '4':
             Block::robot->Put_Down();
            break;
          }
            tmp = BLE_readwithTIMEOUT();
          break;
          case BATTERY:
          Serial.println("Battery");
          sprintf(tmp_tag,"%d",100);
          Block::robot->BLE_write(tmp_tag);
          break;
          case PIANO:
              if(Block::robot->Buzzers[SERVO_2] == NULL)Block::robot->AddBuzzer(EDU_CLAW_PIN2,SERVO_2);
              switch(readIntDirect()){
                      case 0:
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->StopNote();
                        break;
                      case 1:   
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C4");
                        break;
                      case 2:
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C#4");
                        break;
                      case 3: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("D4");
                        break;
                      case 4: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("D#4");
                        break;
                      case 5: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("E4");
                        break;
                      case 6: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("F4");
                        break;
                      case 7: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("F#4");
                        break;
                      case 8: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("G4");
                        break;
                      case 9: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("G#4");
                        break;
                      case 10: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("A4");
                        break;
                      case 11: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("A#4");
                        break;
                      case 12: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("B4");
                        break;
                      case 13: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C5");
                        break;
                      default:
                        break;
              }
          break;
          case HARDWARE_SET:
            #ifdef ESP_H
                tmp = BLE_readwithTIMEOUT();
                while(true){
                tmp = BLE_readwithTIMEOUT();

                if(tmp == '\n' || n_hardware == 15)break;
                if(tmp == ' ')tmp = BLE_readwithTIMEOUT();
                hardware_types[n_hardware][0] = (byte)tmp;
                n_pins = Block::robot->getPinN_for_hardware(hardware_types[n_hardware][0]);
                hardware_types[n_hardware][1] = (byte)readIntDirect();
                Serial.print("pins to get:");
                Serial.println(n_pins);
                for(byte yy = 2; yy < n_pins+2;yy++){
                  Serial.print("pin:");
                  hardware_types[n_hardware][yy] = readIntDirect();
                  Serial.println(hardware_types[n_hardware][yy]);
                }
                tmp_checksum += hardware_types[n_hardware][0]*portUID(n_hardware);
                n_hardware++;
                }
              if(tmp_checksum != Block::robot->hardware_checksum){
                Block::robot->ClearHardware();
                for(byte tt = 0; tt< n_hardware+1;tt++)Block::robot->AddHardware(hardware_types[tt]);
                Block::robot->hardware_checksum = tmp_checksum;  
                Serial.println("HARDWARE SET");
              }else{
                Serial.println("Now Hardware changes.");
              }
          #endif

              /*
               while(tmp != '\n' && n_hardware < 10){
                hardware_types[n_hardware][0] = BLE_readwithTIMEOUT();
                hardware_types[n_hardware][1] = BLE_readwithTIMEOUT();
                n_hardware++;
                tmp = BLE_readwithTIMEOUT();
                tmp_checksum += cti(hardware_types[n_hardware][1])*portUID(hardware_types[n_hardware][0]);
              }
              if(tmp_checksum != Block::robot->hardware_checksum){
                Block::robot->ClearHardware();
                for(byte tt = 0; tt< n_hardware;tt++)Block::robot->AddHardware(hardware_types[tt]);
                Block::robot->hardware_checksum = tmp_checksum;  
                Serial.println("HARDWARE SET");
              }else{
                Serial.println("Now Hardware changes.");
              }
              */

          break;
          case CALIBRATE:
              tmp = BLE_readwithTIMEOUT();
              tmp_tag[0] = BLE_readwithTIMEOUT();
              tmp = BLE_readwithTIMEOUT();
              if(tmp_tag[0] == 'M'){
                Block::robot->left_scale= readIntDirect();
                Block::robot->right_scale= readIntDirect();
                Block::robot->left_invert= readIntDirect();                
                Block::robot->right_invert= readIntDirect();
                      #ifdef DEBUG_MODE
                        Serial.println("User Corrections:");
                        Serial.print("LS: ");
                        Serial.println(Block::robot->left_scale);
                        Serial.print("RS: ");
                        Serial.println(Block::robot->right_scale);
                        Serial.print("LI: ");
                        Serial.println(Block::robot->left_invert);
                        Serial.print("RI: ");
                        Serial.println(Block::robot->right_invert);
                      #endif
              Block::robot->Save_Calibration_Data(CALIB_MOTORS);
              Block::robot->Invert_Left_Rotors(Block::robot->left_invert);
              Block::robot->Scale_Left_Rotors(Block::robot->left_scale);
              Block::robot->Invert_Right_Rotors(Block::robot->right_invert);
              Block::robot->Scale_Right_Rotors(Block::robot->right_scale);
              }else if(tmp_tag[0] == 'W'){
                if(Block::robot->NLineSensors == 0){
                            Block::robot->AddLineSensor(LINE_PIN_1, 1);
                            Block::robot->AddLineSensor(LINE_PIN_2, 2);
                            Block::robot->AddLineSensor(LINE_PIN_3, 3);
                }
              Serial.println("Calibrating No Line!");
              Block::robot->Calibrate_sensors_no_Line();
              }else if(tmp_tag[0] == 'B'){
                 Serial.println("Calibrating Line!");
              Block::robot->Calibrate_sensors_Line();
              Block::robot->Save_Calibration_Data(CALIB_LINE_SENSORS);
              }
               Block::robot->BLE_Flush();
          break;
          default:
              Block::robot->BLE_Flush();
              clear();
              vaildcommand = false;
          break;
        }

        if(LineCode == TIMEOUT_ERROR_CODE){
          Serial.println("Timeout Error");
        
          //Block::robot->BLE_write("ERROR:TIMEOUT_ERROR\n");
          return;
        }
        if(LineCode == CONN_BRK_CODE){
            Serial.println("CONN_BRK_CODE");
            Block::robot->BLE_Flush();
            return;
        }
        if(LineCode != NO_MSG_CODE && !transfereBlocks && LineCode != HARDWARE_SET){
        if(vaildcommand){
          sprintf(tmp_tag,"%cOK\n",LineCode);
          Block::robot->BLE_write(tmp_tag);
        }else{
          Serial.println("UNKNOWN_COMMAND");
          Block::robot->BLE_Flush();
            return;
          //sprintf(tmp_tag,"ERROR:STOP:UNKNOWN_COMMAND:%c\n",LineCode);
          //Block::robot->BLE_write(tmp_tag);
        }
      }
  } 

  char BlockHandler::BLE_readwithTIMEOUT(){
    char asciTmp;
    if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
          }else{
            if(CheckForTimeout()){
              return(TIMEOUT_ERROR_CODE);
            }else{
              asciTmp = Block::robot->BLE_read();
            }
          }
      return(asciTmp);
}

  int32_t BlockHandler::readIntDirect(byte max_d){
  int nDigits = 0;
  int sign = 1;
  byte cursor = 0;
  char tmp;
  char msg[10];
  tmp = BLE_readwithTIMEOUT();
  while(tmp != ' ' && tmp != '\n' && tmp < 250){
    msg[cursor+nDigits] = tmp;
    tmp = BLE_readwithTIMEOUT();
    nDigits++;
  }
  
  if(msg[cursor] == '-')sign = -1;
  if(sign==-1){
    cursor++;
    nDigits--;
  }
  int32_t out = 0;
  int32_t power = 1;
    for(int ii = nDigits-1; ii > -1; ii--){
      if(ii != nDigits-1) power *=10;
      int32_t add = cti(msg[cursor + ii])*power;
      out += add;
    }
    return(out*sign);
}



          