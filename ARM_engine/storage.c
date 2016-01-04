#include "ARM_engine.h"
#include <stdlib.h>

unsigned int *stack = NULL;
unsigned int reg[16] = { 0 };
unsigned int *lr;
unsigned int *pc;
union CPSR cpsr;
union CPSR spsr;

/* stack참조를 위해 맨 처음 스택의 주소를 저장 */
int originalStack;

/* 현재 stack이 할당받은 총 크기 */
int stackSize;

/* 프로그램 실행전에 스택등을 준비해 놓는다. */
void init() {
	lr = reg + 14;
	pc = reg + 15;
	/* 실제 arm에서의 main실행시 cpsr 초기값 */
	cpsr.bits.z = 1;
	cpsr.bits.c = 1;
	/* 프로그램 종료를 위한 lr 초기화 */
	*lr = -4;

	stackSize = 16;
	stack = (unsigned int *) calloc(stackSize, sizeof(int));
	originalStack = reg[13] = (unsigned int) stack;
	symTable[symCount].addr = (unsigned int) stack - stackSize * sizeof(unsigned int);
}

/*
* 호출시 stack에 남아있는 공간이 항상 16개가 되도록 재할당,
* symTable에서 stack의 최소 주소값을 갱신.
*/
void stackUpdate() {
	int addstack = 17 - (stackSize - (originalStack - reg[13]) / 4);

	stack = (unsigned int *) realloc(stack, (stackSize += addstack) * sizeof(unsigned int));
	symTable[symCount].addr -= addstack * sizeof(unsigned int);
}

/*
* address : 변환할 주소값
*
* 가상의 메모리 주소값 address를 실제 주소로 바꿈.
*/
unsigned int* stackAddressConverter(unsigned int address) {
	return stack + (originalStack - address) / 4;
}