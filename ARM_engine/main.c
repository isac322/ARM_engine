#include <stdio.h>
#include <stdlib.h>
#include "ARM_engine.h"

/** 입력을 받고, 실행, 종료를 담당. */

/* 섹션, 함수들 사이의 구분되는 주소를 저장 */
addr *symTable;

/* 입력을 저장 */
List *lines;

int startPoint;
int symCount = 1, lineCount = 0;
int symTableMax = 10, linesMax = 10;

/* 입력받는 부분 */
int main(int argc, char **args) {
	char buf[100] = "";
	int prevLine;

	symTable = (addr *) calloc(10, sizeof(addr));
	lines = (List *) calloc(10, sizeof(List));

	fgets(buf, 100, stdin);
	sscanf(buf, "%x", &startPoint);
	symTable[0].addr = prevLine = startPoint;
	prevLine -= 4;

	/*
	* 파일의 끝(Ctrl + Z)에 도달할때까지 한줄씩 읽어들임
	*
	* 세부구현 :
	* 준비한 배열이 꽉차면 10개단위로 크기 증가.
	* 바로 전 입력 주소와의 차이가 4보다 클경우 (섹션이 나뉘었거나, 다른 함수 인경우)
	* 지금 주소와 바로앞까지의 줄 수를 symTable에 저장.
	*/
	for (; fgets(buf, 100, stdin); lineCount++) {
		if (buf[0] == '\n') {
			lineCount--;
			continue;
		}

		if (linesMax <= lineCount) {
			lines = (List *) realloc(lines, (linesMax += 10)*sizeof(List));
		}

		sscanf(buf, "%x:%x", &lines[lineCount].addr, &lines[lineCount].oper);

		if (prevLine + 4 != lines[lineCount].addr) {
			if (symTableMax < symCount) {
				symTable = (addr *) realloc(symTable, (symTableMax += 10)*sizeof(addr));
			}
			symTable[symCount].count = lineCount;
			symTable[symCount].addr = lines[lineCount].addr;
			symCount++;
		}
		prevLine = lines[lineCount].addr;
	}
	init();

	execute();
}

/* 입력받은 코드를 실행 */
void execute() {
	address oper;
	*pc = startPoint + 8;

	while (*pc != 0) {
		oper.INT = getCodeAt(*pc - 8);

		if (checkCond(oper.dataproc.cond)) {
			/* 실행코드의 26:27 비트로 명령묶음을 나눔 */
			switch (oper.dataproc._zero) {
				case 0: DataProcess(oper.INT); break;
				case 1: SDTransfer(oper.INT); break;
				case 2: Branch_BDT(oper.INT); break;
				case 3: Coproc(oper.INT); break;
			}
		}

		//printf( "%8x %8x %8x %8x %8x %8x %8x %8x\n", oper.INT, reg[15], reg[14], reg[13], reg[3], reg[2], reg[1], reg[0] );

		*pc += 4;
	}
}

/*
* address : 검색할 주소
*
* address에 해당하는 실행코드 반환
*/
unsigned int getCodeAt(unsigned int address) {
	int labelJumpN = symCount;

	for (; address < symTable[labelJumpN].addr; labelJumpN--);

	if (labelJumpN == symCount) {
		return *stackAddressConverter(address);
	}
	else {
		return lines[(address - symTable[labelJumpN].addr) / 4 + symTable[labelJumpN].count].oper.INT;
	}
}

/*
* address : 검색할 주소
* value : 저장할 값
*
* address에 해당하는 주소에 value를 저장
*/
void setCodeAt(unsigned int address, int value) {
	int labelJumpN = symCount;

	for (; address < symTable[labelJumpN].addr; labelJumpN--);

	if (labelJumpN == symCount) {
		*stackAddressConverter(address) = value;
	}
	else {
		lines[(address - symTable[labelJumpN].addr) / 4 + symTable[labelJumpN].count].oper.INT = value;
	}
}