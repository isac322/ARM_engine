#include "BitStruct.h"

#ifndef ARM_ENGIEN_H
#define ARM_ENGIEN_H

/* main.c */
unsigned int getCodeAt(unsigned int address);
unsigned int getBins(int src, int start, int len);
void execute();
void printCode();

/* bitHandler.c */
unsigned int lsl(unsigned int bits, unsigned int count);
unsigned int lsr(unsigned int bits, unsigned int count);
unsigned int asr(unsigned int bits, unsigned int count);
unsigned int ror(unsigned int bits, unsigned int count);
unsigned int shift(unsigned int operand, unsigned int imme);
void cpsrUpdate(unsigned int num1, unsigned int num2, long long result);
int checkCond(int cond);

void DataProcess(unsigned int Instruction);
void Branch_BDT(unsigned int Instruction);
void Coproc(unsigned int Instruction);

extern unsigned int reg[16];
extern unsigned int *lr;
extern unsigned int *pc;
extern union CPSR cpsr;
extern union CPSR spsr;

#endif