#include "ARM_engine.h"
#include "InstructionsSetBitField.h"

/** 26, 27비트가 00인 명령들 묶음
* 맨 밑의 DataProcess에서 명령어를 골라 실행
*/

union {
	int oper;
	DataProcessing dataproc;
	Multiply multi;
	DataSwap swap;
	Mull mull;
}oper;

/* a비트가 1일때는 mla로 동작한다. */
void mul() {
	long long tmp = reg[oper.multi.rm];
	tmp *= reg[oper.multi.rs];

	if (oper.multi.a == 1) tmp += reg[oper.multi.rn];
	reg[oper.multi.rd] = (unsigned int) tmp;
}

void swp() {
	unsigned int rm = reg[oper.swap.rm];
	unsigned int rn = reg[oper.swap.rn];
	unsigned int temp;

	if (oper.swap.b == 1) {
		reg[oper.swap.rd] = 0;
		reg[oper.swap.rd] = getBins(getCodeAt(rn), 8, 8);
		temp = getCodeAt(rn) & 0xFFFFFF00;
		rm = getBins(rm, 8, 8);
		setCodeAt(rn, temp + rm);
	}
	else if (oper.swap.b == 0) {
		reg[oper.swap.rd] = getCodeAt(rn);
		setCodeAt(rn, rm);
	}
}

void umull() {
	unsigned long long rm = reg[oper.mull.rm];
	unsigned long long rs = reg[oper.mull.rs];
	unsigned long long result;

	result = rm * rs;

	reg[oper.mull.RdLo] = (unsigned int) result;
	reg[oper.mull.RdHi] = (unsigned int) (result >> 32);
}

void umlal() {
	unsigned long long rm = reg[oper.mull.rm];
	unsigned long long rs = reg[oper.mull.rs];
	unsigned long long rdhi = reg[oper.mull.RdHi];
	long long tmp1, tmp2, result;

	tmp1 = rm * rs;

	tmp2 = (rdhi << 32ll) + reg[oper.mull.RdLo];
	result = tmp1 + tmp2;

	reg[oper.mull.RdLo] = (unsigned int) result;
	reg[oper.mull.RdHi] = (unsigned int) (result >> 32);

}

void smull() {
	long rm = reg[oper.mull.rm];
	long rs = reg[oper.mull.rs];
	long long result;

	result = rm * rs;

	reg[oper.mull.RdLo] = (unsigned int) result;
	reg[oper.mull.RdHi] = (unsigned int) (result >> 32);
}

void smlal() {
	long rm = reg[oper.mull.rm];
	long rs = reg[oper.mull.rs];
	long long rdhi = reg[oper.mull.RdHi];
	long long tmp1, tmp2, result;

	tmp1 = rm * rs;

	tmp2 = (rdhi << 32) + reg[oper.mull.RdLo];
	result = tmp1 + tmp2;

	reg[oper.mull.RdLo] = (unsigned int) result;
	reg[oper.mull.RdHi] = (unsigned int) (result >> 32);
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

	reg[oper.dataproc.rd] = (unsigned int) result;
}

void rsb() {
	long long result = shift(oper.dataproc.opR2, oper.dataproc.i);
	unsigned int tmp = ~reg[oper.dataproc.rn] + 1;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int) result;
}

void add() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int) result;
}

void adc() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i) + cpsr.bits.c;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int) result;
}

void sbc() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = ~shift(oper.dataproc.opR2, oper.dataproc.i) + cpsr.bits.c;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int) result;
}

void rsc() {
	long long result = shift(oper.dataproc.opR2, oper.dataproc.i);
	unsigned int tmp = ~reg[oper.dataproc.rn] + cpsr.bits.c;
	result += tmp;

	if (oper.dataproc.s == 1) cpsrUpdate(reg[oper.dataproc.rd], tmp, result);

	reg[oper.dataproc.rd] = (unsigned int) result;
}

void tst() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result &= tmp;

	cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
}

void mrs() {
	reg[oper.dataproc.rd] = (((oper.oper >> 22) & 1) == 1 ? spsr : cpsr).cpsr;
}

void movw() {
	reg[oper.dataproc.rd] = 0;
	reg[oper.dataproc.rd] = oper.dataproc.opR2 + (oper.dataproc.rn << 12);
}

void bx() {
	*pc = reg[getBins(oper.dataproc.opR2, 4, 4)];
}

void msr() {
	union CPSR target;
	unsigned int rm = reg[getBins(oper.dataproc.opR2, 4, 4)];

	target = ((oper.oper >> 22) & 1) == 1 ? spsr : cpsr;

	if (oper.dataproc.rn == 8) {
		target.cpsr &= 0x07ffffff;
		target.cpsr += rm & 0xf8000000;
	}

	if (((oper.oper >> 22) & 1) == 1) {
		spsr = target;
	}
	else {
		cpsr = target;
	}
}

void teq() {
	long long result = reg[oper.dataproc.rn];
	unsigned int tmp = shift(oper.dataproc.opR2, oper.dataproc.i);
	result ^= tmp;

	cpsrUpdate(reg[oper.dataproc.rd], tmp, result);
}

void movt() {
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

	reg[oper.dataproc.rd] = (unsigned int) result;
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

	reg[oper.dataproc.rd] = (unsigned int) result;
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

	reg[oper.dataproc.rd] = (unsigned int) result;
}

/* 26, 27비트가 00인 명령들 묶음 */
void DataProcess(unsigned int Instruction) {
	oper.oper = Instruction;

	if (oper.multi._nine == 9 && oper.multi._zero == 0) {
		mul();
	}
	else if (oper.swap._two == 2 && oper.swap._zero == 0 && oper.swap._nine == 9) {
		swp();
	}
	else if (oper.mull._one == 1 && oper.mull._nine == 9) {
		switch (oper.mull.u) {
			case 0: umull(); break;
			case 1: umlal(); break;
			case 2: smull(); break;
			case 3: smlal(); break;
		}
	}
	else {
		switch (oper.dataproc.opCode) {
			case 0: and (); break;
			case 1:	eor(); break;
			case 2: sub(); break;
			case 3: rsb(); break;
			case 4: add(); break;
			case 5: adc(); break;
			case 6:	sbc(); break;
			case 7: rsc(); break;
			case 8:
				if (oper.dataproc.s == 1) tst();
				else if (oper.dataproc.rn == 0xf && oper.dataproc.opR2 == 0) mrs();
				else movw();
				break;
			case 9:
				if (oper.dataproc.s == 1) teq();
				else if (((oper.oper >> 4) & 1) == 1) bx();
				else msr();
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