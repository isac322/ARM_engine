#include "ARM_engine.h"

#ifndef REGISTER
#define REGISTER

unsigned int reg[16] = { 0 };
unsigned int *lr = reg + 14;
unsigned int *pc = reg + 15;
union CPSR cpsr;
union CPSR spsr;

#endif

