#include "ARM_engine.h"
#include "InstructionsSetBitField.h"

/** 26, 27비트가 01인 명령들 묶음 */

union
{
	int oper;
	SingleDataTransfer SDT;
	Undefined unDifened;
}oper;

void str()
{
	unsigned int savereg = reg[oper.SDT.rd];
	unsigned int excutereg = reg[oper.SDT.rn];
	unsigned int temp = oper.SDT.offset;

	if ( oper.SDT.i == 1 ) temp = shift( temp, 0 );
	if ( oper.SDT.u == 0 ) temp *= -1;
	if ( oper.SDT.p == 1 ) excutereg += temp;

	if ( oper.SDT.b == 1 ) savereg = getBins( savereg, 8, 8 );
	setCodeAt(excutereg, savereg);

	if ( oper.SDT.w == 1 || oper.SDT.p == 0 ) reg[oper.SDT.rn] += temp;
}

void ldr()
{
	unsigned int loadreg;
	unsigned int excutereg = reg[oper.SDT.rn];
	unsigned int temp = oper.SDT.offset;

	if ( oper.SDT.i == 1 ) temp = shift( temp, 0 );
	if ( oper.SDT.u == 0 ) temp *= -1;
	if ( oper.SDT.p == 1 ) excutereg += temp;
	loadreg = getCodeAt( excutereg );

	if ( oper.SDT.b == 1 ) loadreg = getBins( loadreg, 8, 8 );
	reg[oper.SDT.rd] = loadreg;

	if ( oper.SDT.w == 1 || oper.SDT.p == 0 ) reg[oper.SDT.rn] += temp;
}

void SDTransfer( unsigned int inscruction )
{
	oper.oper = inscruction;

	if ( oper.SDT.l == 0 ) str();
	else ldr();
}