#ifndef UFUNC
#define UFUNC
#include <Arduino.h>

#define Robot Block::robot

void UserFunctions_Setup();

void UserFunction_1();

void UserFunction_2(int input = 0);

int UserFunction_3(int input = 0);

int UserFunction_4();

void UserFunction_5(int input_1 = 0, int input_2 = 0);
#endif