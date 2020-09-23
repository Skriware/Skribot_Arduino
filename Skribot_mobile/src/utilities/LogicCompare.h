#ifndef LogicComp_H
#define LogicComp_H
#include "LogicBlock.h"

class LogicCompare : public LogicBlock {

public:
  LogicCompare(byte id,byte _logicOperation,byte _input_left, byte _input_right);
  ~LogicCompare();
  void get_value();
  bool get_bool_output();
protected:
 char logicOperation;
 bool val_L;
 bool val_R;

};

#endif
