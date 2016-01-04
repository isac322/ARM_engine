#include "InstructionsSetBitField.h"

#ifndef BIT_STRUCT_H
#define BIT_STRUCT_H

union CPSR {
	unsigned int cpsr;
	struct {
		unsigned mode : 5;
		unsigned t : 1;
		unsigned f : 1;
		unsigned i : 1;
		unsigned __empty2 : 16;
		unsigned j : 1;
		unsigned __empty : 2;
		unsigned q : 1;
		unsigned v : 1;
		unsigned c : 1;
		unsigned z : 1;
		unsigned n : 1;
	} bits;
};

typedef union ADRESS {
	unsigned int INT;
	DataProcessing dataproc;
}address;

typedef struct LIST {
	unsigned int addr;
	address oper;
}List;

typedef struct SECTOK {
	unsigned int addr;
	unsigned int count;
}addr;

#endif