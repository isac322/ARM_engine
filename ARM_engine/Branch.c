#include "ARM_engine.h"
#include "InstructionsSetBitField.h"

/** 26, 27비트가 10인 명령들 묶음 */

union {
	int oper;
	BlockDataTransfer btransf;
	Brach branch;
}oper;

void branch() {
	int tmp;

	if (oper.branch.l == 1) *lr = reg[15];
	tmp = oper.branch.offset << 8;
	*pc += (tmp >> 6) + 4;
}

void ldm() {
	unsigned int base = reg[oper.btransf.rn];
	int offset = oper.btransf.u == 1 ? 4 : -4;
	int tmp = 0, i;

	for (; tmp < 16; tmp++) {
		/* Up 이거나 Down 일때를 대비한 코드 */
		i = oper.btransf.u == 1 ? tmp : 15 - tmp;

		if (((oper.btransf.registerList >> i) & 1) == 1) {
			if (oper.btransf.p == 1) base += offset;
			reg[i] = getCodeAt(base);
			if (oper.btransf.p == 0) base += offset;
		}
	}
	if (oper.btransf.w == 1) reg[oper.btransf.rn] = base;

	stackUpdate();
}

void stm() {
	unsigned int base = reg[oper.btransf.rn];
	int offset = oper.btransf.u == 1 ? 4 : -4;
	int tmp = 0, i;

	for (; tmp < 16; tmp++) {
		/* Up 이거나 Down 일때를 대비한 코드 */
		i = oper.btransf.u == 1 ? tmp : 15 - tmp;

		if (((oper.btransf.registerList >> i) & 1) == 1) {
			if (oper.btransf.p == 1) base += offset;
			setCodeAt(base, reg[i]);
			if (oper.btransf.p == 0) base += offset;
		}
	}
	if (oper.btransf.w == 1) reg[oper.btransf.rn] = base;

	stackUpdate();
}

void block_data_transfer() {
	if (oper.btransf.l == 1) ldm();
	else stm();
}

/* 26, 27비트가 10인 명령들 묶음 */
void Branch_BDT(unsigned int Instruction) {
	oper.oper = Instruction;

	if (((oper.oper >> 25) & 1) == 1) {
		branch();
	}
	else {
		block_data_transfer();
	}
}