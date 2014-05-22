#include <stdio.h>
#include <stdlib.h>
#include "ARM_engine.h"

/** �Է��� �ް�, ����, ���Ḧ ���. */

/* ����, �Լ��� ������ ���еǴ� �ּҸ� ���� */
addr *symTable;

/* �Է��� ���� */
List *lines;

int startPoint;
int symCount = 1, lineCount = 0;
int symTableMax = 10, linesMax = 10;

/* �Է¹޴� �κ� */
int main(int argc, char **args) {
	char buf[100] = "";
	int prevLine;

	symTable = (addr *)calloc(10, sizeof(addr));
	lines = (List *)calloc(10, sizeof(List));

	fgets(buf, 100, stdin);
	sscanf_s(buf, "%x", &startPoint);
	symTable[0].addr = prevLine = startPoint;
	prevLine -= 4;

	/*
	 * ������ ��(Ctrl + Z)�� �����Ҷ����� ���پ� �о����
	 *
	 * ���α��� :
	 * �غ��� �迭�� ������ 10�������� ũ�� ����.
	 * �ٷ� �� �Է� �ּҿ��� ���̰� 4���� Ŭ��� (������ �������ų�, �ٸ� �Լ� �ΰ��)
	 * ���� �ּҿ� �ٷξձ����� �� ���� symTable�� ����.
	 */
	for (; fgets(buf, 100, stdin); lineCount++) {
		if (buf[0] == '\n') {
			lineCount--;
			continue;
		}

		if (linesMax <= lineCount) {
			lines = (List *)realloc(lines, (linesMax += 10)*sizeof(List));
		}

		sscanf_s(buf, "%x:%x", &lines[lineCount].addr, &lines[lineCount].oper);

		if (prevLine + 4 != lines[lineCount].addr.INT) {
			if (symTableMax < symCount) {
				symTable = (addr *)realloc(symTable, (symTableMax += 10)*sizeof(addr));
			}
			symCount++;
			symTable[symCount - 1].count = (prevLine - symTable[symCount - 2].addr) / 4 + symTable[symCount - 2].count + 1;
			symTable[symCount - 1].addr = lines[lineCount].addr.INT;
		}
		prevLine = lines[lineCount].addr.INT;
	}

	execute();
}

/*
 * address : �˻��� �ּ�
 *
 * address�� �ش��ϴ� �����ڵ� ��ȯ
 */
unsigned int getCodeAt(unsigned int address) {
	int labelJumpN = symCount;

	while (address < symTable[--labelJumpN].addr);
	return lines[((address - symTable[labelJumpN].addr) >> 2) + symTable[labelJumpN].count].oper.INT;
}

void printCode() {
	int i = 0;

	for (; i < lineCount; i++) {
		printf("%08x: %08x\n", lines[i].addr, lines[i].oper);
	}
}

/* �Է¹��� �ڵ带 ���� */
void execute() {
	cpsr.bits.z = 1;
	cpsr.bits.c = 1;
	*pc = startPoint + 8;
	*lr = -4;

	while (*pc != 0) {
		address oper;
		oper.INT = getCodeAt(*pc - 8);

		if (checkCond(oper.dataproc.cond)) {
			/* �����ڵ��� 26:27 ��Ʈ�� ��ɹ����� ���� */
			switch (oper.dataproc._zero) {
			case 0: DataProcess(oper.INT); break;
			case 1:
			case 2: Branch_BDT(oper.INT); break;
			case 3: Coproc(oper.INT); break;
			}
		}
		//printf("%8x %8x %8x %8x\n", reg[0], reg[1], reg[2], cpsr.cpsr);

		*pc += 4;
	}
}