#include "ARM_engine.h"
#include "InstructionsSetBitField.h"

/** 26, 27비트가 00인 명령들 묶음 */

union {
	int oper;
	DataProcessing dataproc;
	Multiply multi;
	DataSwap swap;
}oper;

void mul() {
	long long tmp = reg[oper.multi.rm];
	tmp *= reg[oper.multi.rs];

	if (oper.multi.a == 1) tmp += reg[oper.multi.rn];
	reg[oper.multi.rd] = (unsigned int)tmp;
}

void and() {
	unsigned int result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result &= tmp;

	if (oper.dataproc.s == 1) {
		spsr.bits.v = cpsr.bits.v;
		cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
		cpsr.bits.v = spsr.bits.v;
		cpsr.bits.c = spsr.bits.c;
	}

	reg[oper.dataproc.rd] = result;
}

void eor() {
	unsigned int result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result ^= tmp;

	if (oper.dataproc.s == 1) {
		spsr.bits.v = cpsr.bits.v;
		cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
		cpsr.bits.v = spsr.bits.v;
		cpsr.bits.c = spsr.bits.c;
	}

	reg[oper.dataproc.rd] = result;
}

void sub() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = ~shift(oper.dataproc.opR2, oper.dataproc.i) + 1;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int)result;
}

void rsb() {
	long long result = shift(oper.dataproc.opR2, oper.dataproc.i);
	unsigned int tmp = ~reg[oper.dataproc.rn] + 1;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int)result;
}

void add() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int)result;
}

void adc() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i) + cpsr.bits.c;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int)result;
}

void sbc() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = ~shift(oper.dataproc.opR2, oper.dataproc.i) + 1 + cpsr.bits.c;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int)result;
}

void rsc() {
	long long result = shift(oper.dataproc.opR2, oper.dataproc.i);
	unsigned int tmp = ~reg[oper.dataproc.rn] + 1 + cpsr.bits.c;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int)result;
}

void tst() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result &= tmp;

	cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
}

void movw() {
	reg[oper.dataproc.rd] = 0;
	reg[oper.dataproc.rd] = oper.dataproc.opR2 + (oper.dataproc.rn << 12);
}

void bx() {
	*pc = reg[getBins(oper.dataproc.opR2, 4, 4)];
}

void teq() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result ^= tmp;

	cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
}

void movt()	{
	reg[oper.dataproc.rd] &= 0xffff;
	reg[oper.dataproc.rd] += ((oper.dataproc.opR2 + (oper.dataproc.rn << 12)) << 16);
}

void cmp() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = ~shift(oper.dataproc.opR2, oper.dataproc.i) + 1;
	result += tmp;

	cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
}

void cmn() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result += tmp;

	cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
}

void orr() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result |= tmp;

	if (oper.dataproc.s == 1) {
		spsr.bits.v = cpsr.bits.v;
		cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
		cpsr.bits.v = spsr.bits.v;
		cpsr.bits.c = spsr.bits.c;
	}

	reg[oper.dataproc.rd] = (unsigned int)result;
}

void mov() {
	long long result = shift(oper.dataproc.opR2, oper.dataproc.i);

	if (oper.dataproc.s == 1) {
		int c = cpsr.bits.c;
		int v = cpsr.bits.v;
		cpsrUpdate(reg[oper.dataproc.rd], reg[oper.dataproc.rd], result);
		if (oper.dataproc.i == 1 || (oper.dataproc.i == 0 && getBins(oper.dataproc.opR2, 12, 8) == 0)) cpsr.bits.c = c;
		else cpsr.bits.c = spsr.bits.c;
		cpsr.bits.v = v;
	}

	reg[oper.dataproc.rd] = (unsigned int)result;
}

void bic() {
	unsigned int result = reg[oper.dataproc.rn];
	unsigned int tmp = ~shift(oper.dataproc.opR2, oper.dataproc.i);
	result &= tmp;

	if (oper.dataproc.s == 1) {
		spsr.bits.v = cpsr.bits.v;
		cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
		cpsr.bits.v = spsr.bits.v;
		cpsr.bits.c = spsr.bits.c;
	}

	reg[oper.dataproc.rd] = result;
}

void mvn() {
	long long result = ~shift(oper.dataproc.opR2, oper.dataproc.i);

	if (oper.dataproc.s == 1) {
		int c = cpsr.bits.c;
		int v = cpsr.bits.v;
		cpsrUpdate(reg[oper.dataproc.rd], reg[oper.dataproc.rd], result);
		if (oper.dataproc.i == 0 && getBins(oper.dataproc.opR2, 12, 8) == 0) cpsr.bits.c = c;
		cpsr.bits.v = v;
	}

	reg[oper.dataproc.rd] = (unsigned int)result;
}

/* 26, 27비트가 00인 명령들 묶음 */
void DataProcess(unsigned int Instruction) {
	oper.oper = Instruction;

	if (oper.multi._nine == 9 && oper.multi._zero == 0) {
		mul();
	} else if (oper.swap._two == 2 && oper.swap._zero == 0 && oper.swap._nine == 9) {

	} else {
		switch (oper.dataproc.opCode) {
		case 0: and(); break;
		case 1:	eor(); break;
		case 2: sub(); break;
		case 3: rsb(); break;
		case 4: add(); break;
		case 5: adc(); break;
		case 6:	sbc(); break;
		case 7: rsc(); break;
		case 8:
			if (oper.dataproc.s == 1) tst();
			else movw();
			break;
		case 9:
			if (oper.dataproc.s == 1) teq();
			else if (((oper.oper >> 4) & 1) == 1) bx();
			else /*msr()*/;
			break;
		case 10:
			if (oper.dataproc.s == 1) cmp();
			else movt();
			break;
		case 11: cmn(); break;
		case 12: orr(); break;
		case 13: mov(); break;
		case 14: bic(); break;
		case 15: mvn(); break;
		}
	}
}