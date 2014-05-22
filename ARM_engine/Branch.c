#include "ARM_engine.h"
#include "InstructionsSetBitField.h"

/** 26, 27��Ʈ�� 10�� ��ɵ� ���� */

union {
	int oper;
	BlockDataTransfer btransf;
	Brach branch;
}oper;

void branch() {
	if (oper.branch.l == 1) *lr = reg[15] - 4;
	*pc = oper.branch.offset << 2;
}

void block_data_transfer() {

}

/* 26, 27��Ʈ�� 10�� ��ɵ� ���� */
void Branch_BDT(unsigned int Instruction) {
	oper.oper = Instruction;

	if (((oper.oper >> 25) & 1) == 1) {
		branch();
	} else {
		block_data_transfer();
	}
}