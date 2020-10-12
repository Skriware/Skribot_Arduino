//#define DEBUG_MODE
#include "Arduino.h"
#include "Skribot_mobile.h"
BlockHandler BH;
Skribot *robot;
bool transmision_recieved = false;
bool BT_state =false;
bool Connection_Break = false;
char ascitmp;
char softVersion[] = "1.0.0";
long last_BT_check = 0;
bool LED_STATE = true;

void ENTER_TO_IDLE(){
  BH.clear();
  robot->Stop();
  //robot->TurnLEDOff(); 
  //robot->OpenClaw();
  //robot->Put_Down();
  //robot->TurnLEDOn(255,255,255);

  /*for (int i = 0; i < 5; i++)
  {
    auto *buzzer = robot->Buzzers[i];
    if (buzzer != nullptr)
      buzzer->StopNote();
  }*/

}

void Blink(){
  if (millis()-last_BT_check > 500){
    if(LED_STATE){
      robot->TurnLEDOn(255,255,255);
    }else{
      robot->TurnLEDOff();
    }
      last_BT_check = millis();
      LED_STATE = !LED_STATE;
  }
}

int freeRam() 
{
  #ifndef ESP_H
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
  #else
  return(500);
  #endif
}


int CompileCode(){
     int flag; 
  while(freeRam() > 190){
      flag = BH.Handle_Msg(); 
      if(flag != CODE_NOT_COMPLETE)return(flag);
    }
}

void idle_connectioncheck(){
   if(!robot->BLE_checkConnection()){
     Blink();
     if(BT_state){
      robot->Stop();
      BT_state = !BT_state;
      Serial.println("DISCONNECTED");
      ENTER_TO_IDLE();
      #ifdef ESP_H
      robot->status->TurnOn(YELLOW,2);
      #endif
     }
  }else if(robot->BLE_checkConnection() && !BT_state){ 
     BT_state = !BT_state;
      Serial.println("CONNECTED");
     BH.clear();
     #ifdef ESP_H
      robot->status->TurnOn(BLUE,2);
     #endif
  }
} 

void SendCodeEndMEssage(){
        BH.clear();
        robot->TurnLEDOn(255,255,255);
        if(!Connection_Break){
          robot->ProgramENDRepotred();
          if(!robot->Remote_block_used){
            //robot->wait_And_Check_BLE_Connection(500,20);
            robot->BLE_write("DONE\n");
          }else{
            robot->Remote_block_used = false;
          }
        }
        robot->connection_Break_Reported = false;
        #if ENABLED(DEBUG_MODE)
          Serial.println("CONFIRMING END OF CODE");
        #endif
        ENTER_TO_IDLE();
}
void ExecuteCode(){
  Serial.print("FREE RAM:");
  Serial.println(freeRam());
        #if ENABLED(DEBUG_MODE)
          Serial.println("CONFIRMING START OF CODE");
        #endif
        //robot->status->ReadBatteryState();
while(BH.doBlock()){
           if(robot->ProgramENDRepotred()){
              #if ENABLED(DEBUG_MODE)
              Serial.println("Stopping the robot!");
              #endif
                BH.clear();
                robot->Stop();
              break;  
            }else if(!robot->BLE_checkConnection()){
                ENTER_TO_IDLE();
                Connection_Break = true;
                break;
            }
        }
}

void SendErrorMsg(char *msg){
            ENTER_TO_IDLE();
            #ifdef ESP_H        
            robot->status->TurnOn(RED,2);
            #endif
            #if ENABLED(DEBUG_MODE)
            Serial.println(msg);
            #endif
            Block::robot->BLE_Flush();
}

void setup() {
  #ifdef ESP_H
     robot = new Skribot("RAW_SKRIBRAIN");
  #else
    robot = new Skribot("EDU_SHIELD");
  #endif
  bool tmp = robot->Check_Board_Version();
  if(tmp)robot->ConfigureBoardEEPROM();
  robot->BaterryCheck();
  robot->BLE_Setup();
  Block::setRobot(robot); 
  Block::setBlockHandler(&BH);
  BT_state = false; 
  #if ENABLED(DEBUG_MODE)
    Serial.println(freeRam());
  #endif 
    //UserFunctions_Setup();
    ENTER_TO_IDLE();
    robot->status->BLINK_OK();
    #ifdef ESP_H
      robot->status->TurnOn(YELLOW,2);
    #endif
    robot->status->ReadBatteryState();
    Block::robot->ClearHardware();
    Serial.println("setup end");
}

void loop() {
    byte tmp = BH.readMessageLine();
    BH.processMessageLine(tmp);
    
    if(BH.transfereBlocks){
      byte codeinfo;
      while(true){                  //read code
        codeinfo = BH.readCodeLine();
        if(codeinfo == TIMEOUT_ERROR_CODE)break;
        if(codeinfo == CODE_COMPLETE)break;
        if(codeinfo == CODE_TOO_LONG)break;
        if(codeinfo == CONN_BRK_CODE)break;
      }
      if(codeinfo == CODE_COMPLETE){
        Serial.println("BEGIN COMPILATION!");
        byte succes = CompileCode();        //make blocks connections
        Serial.println("COMPILATION ENDED");
        if(succes == CODE_VALID_AND_COMPLETE_RUN_O){                    //chceck compiler errors
            Connection_Break = false;
            if(!robot->Remote_block_used)robot->BLE_write("OK");
            ExecuteCode();                  //Here robot runs the code
            SendCodeEndMEssage();

        }else if(succes == CODE_NOT_VALID){
          //robot->BLE_write("ERROR:CODE_NOT_VALID\n");
          SendErrorMsg("CODE NOT VALID");
        }else if(succes == CODE_TOO_LONG){
          //robot->BLE_write("ERROR:STOP:CODE_TOO_LONG\n");
          SendErrorMsg("CODE TOO LONG");
        }

      }else if(codeinfo == TIMEOUT_ERROR_CODE){
          SendErrorMsg("TIMEOUT ERROR");
          //robot->BLE_write("ERROR:TIMEOUT_ERROR\n");
      }else if(codeinfo == CODE_TOO_LONG){
          //robot->BLE_write("ERROR:STOP:CODE_TOO_LONG\n");
          SendErrorMsg("CODE TOO LONG");
      }else if(codeinfo == CONN_BRK_CODE){
          SendErrorMsg("CONNECTION BREAK");
      }

  }
  idle_connectioncheck();
}