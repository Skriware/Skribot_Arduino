#include "Skribot.h"

#define EEPROM_EMPTY_ESP32 255
#define EEPROM_EMPTY_ARDUINO 0


Skribot::Skribot(String predef){
  #ifdef DEBUG_MODE
  Serial.begin(115200);
  #endif
  NDistSensors    = 0;
  NLEDs           = 0;
  NLineSensors    = 0;
  NLeftDCRotors   = 0;
  NRightDCRotors  = 0;
  NClaws          = 0;
  NABSDevices     = 0;
  NSPIDevices     = 0;
  NI2CDevices     = 0;
  BTmodule   = NULL;
  #ifdef ESP_H
  SPIcomm[0] = NULL;
  SPIcomm[1] = NULL;
  I2Ccomm[0] = NULL;
  I2Ccomm[1] = NULL;
  #endif
  using_BLE_Connection = false;
  program_End_Reported = false;
  connection_Break_Reported = false;
  stausLEDused = false;
  ignore_connection_break = false;
  high_power_usage = false;
  eeprom_version_loaded = false;
  claw_closed = false;
  claw_closed_time = true;
  config_mode = false;
  Remote_block_used = false;
  leftSmartRotor = NULL;
  rightSmartRotor = NULL;
  smartRotor = NULL;
  for(byte tt = 0; tt < MAX_HARDWARE; tt++){
    if(tt < MAX_LED_MATRIX)   LED_Matrixes[tt]  =NULL;
    if(tt < MAX_LEFT_ROTORS)  LeftDCRotors[tt]  =NULL;
    if(tt < MAX_RIGHT_ROTORS) RightDCRotors[tt] =NULL;
    if(tt < MAX_DIST_SENSORS) DistSensors[tt]   =NULL;
    if(tt < MAX_LINE_SENSORS) LineSensors[tt]   =NULL;
    if(tt < MAX_LEDs)         LEDs[tt]          =NULL;
    if(tt < MAX_CLAWS)        Claws[tt]         =NULL;
    if(tt < MAX_BUZZERS)      Buzzers[tt]       =NULL;
    if(tt < MAX_ABS_DIV)      Devices[tt]       =NULL;
  }

  if(predef != " ")Configure_Connections(predef);
}

  int Skribot::BaterryCheck(){
      int tmpStatus;
      if(stausLEDused && !high_power_usage){
        tmpStatus = status->CheckBateryStatus();
      }else{
        tmpStatus = 0;
      }
      return(tmpStatus);
    }

    void Skribot::EnterConfigMode(){
      config_mode = true;
    }
    void Skribot::ExitConfigMode(){
      if(config_mode){
        config_mode = false;
      }
    }

  void Skribot::Set_Line_Sensor_Logic_Border(int L1_border,int L2_border,int L3_border){
      for(int zz = 0; zz < NLineSensors ; zz++){
                    if(LineSensors[zz]->GetSensorPin() == LINE_PIN_1 && L1_border != 0){
                      LineSensors[zz]->SetLogicBorder(L1_border);
                    }else if(LineSensors[zz]->GetSensorPin() == LINE_PIN_2 && L2_border != 0){
                      LineSensors[zz]->SetLogicBorder(L2_border);
                    }else if(LineSensors[zz]->GetSensorPin() == LINE_PIN_3 && L3_border != 0){
                      LineSensors[zz]->SetLogicBorder(L3_border);
                    }
      }
    }


  void Skribot::Set_Line_Sensor_Logic_Border(String id, int line, int noline){
    int unscaledline = line/100*4095;
    int unscaledNoline = noline/100*4095;
    #ifdef ESP_H
    if(!EEPROM.begin(64)){
          #ifdef DEBUG_MODE
          Serial.println("EEPROM init fail, aborting calibration");
          #endif
          return;
       }
    #endif
                        int lb = noline +abs(unscaledline-unscaledNoline)/2;
                        if(id == "L1")Write_EEPROM_INT(EEPROM_L1_BORDER_ADDR,lb);
                        if(id == "L2")Write_EEPROM_INT(EEPROM_L2_BORDER_ADDR,lb);
                        if(id == "L3")Write_EEPROM_INT(EEPROM_L3_BORDER_ADDR,lb);
                        #ifdef ESP_H 
                        EEPROM.commit(); 
                        #endif
                        delay(100);
                        if(!user_config){
                          EEPROM.write(EEPROM_SETTINGS_OVERRIDED_ADDR,1);
                          user_config = true;
                          #ifdef ESP_H 
                          EEPROM.commit(); 
                          #endif
                    
                        }
  Serial.print(id);
  Serial.println(" calibrated!");
}
byte Skribot::ReadBattery(){
  return(status->ReadBatteryState());
}

void Skribot::Calibrate_sensors_no_Line(){
                   for(int zz = 0; zz < NLineSensors ; zz++){
                      TurnLEDOn(255,255,255);
                      LineSensors[zz]->No_Line_Readout();
                      TurnLEDOn(0,0,0);
                      delay(100);
                      if(LineSensors[zz]->GetSensorPin() == LINE_PIN_1)L1_b = LineSensors[zz]->GetLogicBorder();
                      if(LineSensors[zz]->GetSensorPin() == LINE_PIN_2)L2_b = LineSensors[zz]->GetLogicBorder();
                      if(LineSensors[zz]->GetSensorPin() == LINE_PIN_3)L3_b = LineSensors[zz]->GetLogicBorder();
                    }
    }
void Skribot::Calibrate_sensors_Line(){
                    for(int zz = 0; zz < NLineSensors ; zz++){
                      TurnLEDOn(255,255,255);
                      LineSensors[zz]->Line_Readout();
                      TurnLEDOn(0,0,0);
                      delay(100);
                      if(LineSensors[zz]->GetSensorPin() == LINE_PIN_1)L1_b = LineSensors[zz]->GetLogicBorder();
                      if(LineSensors[zz]->GetSensorPin() == LINE_PIN_2)L2_b = LineSensors[zz]->GetLogicBorder();
                      if(LineSensors[zz]->GetSensorPin() == LINE_PIN_3)L3_b = LineSensors[zz]->GetLogicBorder();
                    }
    }