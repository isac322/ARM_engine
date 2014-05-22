#include "ARM_engine.h"

/** 비트열의 수정, 가공하는 기능의 함수들 모음 */

/*
 * src : 원본 비트열
 * start : 시작 비트 위치
 * len : 가져올 비트 개수
 *
 * 비트는 오른쪽으로 가져온다.
 */
unsigned int getBins(int src, int start, int len) {
	int tmp = 0, mask = 1;

	mask = ((1 << len) - 1) << (start - len);
	tmp = (mask & src) >> (start - len);

	return tmp;
	/*
	int tmpAddress = 0;
	int i = start - 1;

	for (; i > start - len - 1; i--) {
		tmpAddress <<= 1;
		if ((src >> i) & 1) tmpAddress++;
	}

	return tmpAddress;
	*/
}

unsigned int lsl(unsigned int bits, unsigned int count) {
	if (count > 0) {
		spsr.bits.c = (bits >> (32 - count)) & 1 ? 1 : 0;
	}
	return bits <<= count;
}

unsigned int lsr(unsigned int bits, unsigned int count) {
	if (count > 0) {
		spsr.bits.c = (bits >> (count - 1)) & 1 ? 1 : 0;
		bits >>= 1;
		if (((bits >> 31) & 1) == 1) bits -= 1 << 31;
		if (count > 1) bits >>= count - 1;
	}

	return bits;
}

unsigned int asr(unsigned int bits, unsigned int count) {
	if (count > 0) {
		spsr.bits.c = (bits >> (count - 1)) & 1 ? 1 : 0;
	}
	return bits >>= count;
}

unsigned int ror(unsigned int bits, unsigned int count) {
	if (count > 0) {
		unsigned int tmp = getBins(bits, count, count);

		bits = lsr(bits, count);
		bits += tmp << (32 - count);
		spsr.bits.c = (tmp >> (count - 1)) & 1 ? 1 : 0;
	}

	return bits;
}

/*
 * operand : 12비트의 operand2
 * imme : 1이면 상수 0이면 레지스터
 *
 * operand 분석, 쉬프트 처리 후 알맞은 값 반환.
 */
unsigned int shift(unsigned int operand, unsigned int imme) {
	unsigned int shiftCount = 0;
	unsigned int shiftedVal = 0;

	if (imme == 1) {
		shiftCount = getBins(operand, 12, 4) << 1;
		shiftedVal = ror(getBins(operand, 8, 8), shiftCount);
	} else {
		unsigned int shiftOper = getBins(operand, 7, 2);
		unsigned int rn = reg[getBins(operand, 4, 4)];
		shiftCount = getBins(operand, 12, 5);

		if (((operand >> 4) & 1) == 1) {
			shiftCount = reg[lsr(shiftCount, 1)];
		}

		switch (shiftOper) {
		case 0:shiftedVal = lsl(rn, shiftCount); break;
		case 1:shiftedVal = lsr(rn, shiftCount); break;
		case 2:shiftedVal = asr(rn, shiftCount); break;
		case 3:shiftedVal = ror(rn, shiftCount); break;
		}
	}

	return shiftedVal;
}

/*
 * num1 : 연산전 값
 * num2 : 연산 할 값
 * result : 연산후 값
 *
 * n -- 31번째 비트가 1이면 1
 * z -- 결과 값이 0이면 1
 * c -- 32번째 비트가 1이면 1
 * v -- 연산할 두 수의 부호가같고, 연산후와는 다르면 1
 */
void cpsrUpdate(unsigned int num1, unsigned int num2, long long result) {
	cpsr.bits.n = ((result >> 31) & 1ll) == 1 ? 1 : 0;
	cpsr.bits.z = result == 0 ? 1 : 0;
	cpsr.bits.c = result > 0xffffffff ? 1 : 0;
	cpsr.bits.v = (((num1 ^ num2) >> 31) & 1) == 0 && (((num1 ^ result) >> 31) & 1ll) == 1 ? 1 : 0;
}

/* cpsr을 참조해서 bool값 반환 */
int checkCond(int cond) {
	switch (cond) {
	case 0: return cpsr.bits.z;
	case 1: return !cpsr.bits.z;
	case 2: return cpsr.bits.c;
	case 3: return !cpsr.bits.c;
	case 4: return cpsr.bits.n;
	case 5: return !cpsr.bits.n;
	case 6: return cpsr.bits.v;
	case 7: return !cpsr.bits.v;
	case 8: return cpsr.bits.c & !cpsr.bits.z;
	case 9: return !cpsr.bits.c | cpsr.bits.z;
	case 10: return !(cpsr.bits.n ^ cpsr.bits.v);
	case 11: return cpsr.bits.n ^ cpsr.bits.v;
	case 12: return !cpsr.bits.z & !(cpsr.bits.n ^ cpsr.bits.v);
	case 13: return cpsr.bits.z | (cpsr.bits.n ^ cpsr.bits.v);
	case 14: return 1;
	case 15: return 0;
	}
}