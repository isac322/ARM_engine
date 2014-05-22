#include "ARM_engine.h"
#include "InstructionsSetBitField.h"

/** 26, 27비트가 11인 명령들 묶음 */

union {
	int oper;
	CoprocDataTransfer CDTransfer;
	CoprocDataOperation CDOper;
	CoprocRegisterTransfer CRTransfer;
	S_W_Interrup swi;
}oper;

void swi() {
	int i = 0;

	for (; i < 16; i++) printf("r%2d: \t%08x\n", i, reg[i]);
}

/* 26, 27비트가 11인 명령들 묶음 */
void Coproc(unsigned int Instruction) {
	oper.oper = Instruction;

	if (oper.CDTransfer._six == 6) {

	} else if (oper.CDOper._foreTeen == 14) {
		if (oper.CDOper._zero == 0) {

		} else {

		}
	}
	else {
		swi();
	}
}