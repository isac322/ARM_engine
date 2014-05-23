#include "ARM_engine.h"

#ifndef REGISTER
#define REGISTER

unsigned int *stack = (void *)0;
unsigned int reg[16] = { 0 };
reg[13] = &stack;
unsigned int *lr = reg + 14;
unsigned int *pc = reg + 15;
union CPSR cpsr;
union CPSR spsr;

#endif