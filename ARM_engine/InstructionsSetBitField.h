#ifndef INSTRUCTION_SET_BIT_FIELD_H
#define INSTRUCTION_SET_BIT_FIELD_H

/** 실행코드 묶음별 비트필드 정의 */

typedef struct _DataProcessing {
	unsigned opR2 : 12;
	unsigned rd : 4;
	unsigned rn : 4;
	unsigned s : 1;
	unsigned opCode : 4;
	unsigned i : 1;
	unsigned _zero : 2;
	unsigned cond : 4;
}DataProcessing;

typedef struct _Multiply {
	unsigned rm : 4;
	unsigned _nine : 4;
	unsigned rs : 4;
	unsigned rn : 4;
	unsigned rd : 4;
	unsigned s : 1;
	unsigned a : 1;
	unsigned _zero : 6;
	unsigned cond : 4;
}Multiply;

typedef struct _DataSwap {
	unsigned rm : 4;
	unsigned _nine : 8;
	unsigned rd : 4;
	unsigned rn : 4;
	unsigned _zero : 2;
	unsigned b : 1;
	unsigned _two : 5;
	unsigned cond : 4;
}DataSwap;

typedef struct _SingleDataTransfer {
	unsigned offset : 12;
	unsigned rd : 4;
	unsigned rn : 4;
	unsigned l : 1;
	unsigned w : 1;
	unsigned b : 1;
	unsigned u : 1;
	unsigned p : 1;
	unsigned i : 1;
	unsigned _one : 2;
	unsigned cond : 4;
}SingleDataTransfer;

typedef struct _Undefined {
	unsigned __empty2 : 4;
	unsigned _one : 1;
	unsigned __empty : 20;
	unsigned _three : 3;
	unsigned cond : 4;
}Undefined;

typedef struct _BlockDataTransfer {
	unsigned registerList : 16;
	unsigned rn : 4;
	unsigned l : 1;
	unsigned w : 1;
	unsigned s : 1;
	unsigned u : 1;
	unsigned p : 1;
	unsigned _fore : 3;
	unsigned cond : 4;
}BlockDataTransfer;

typedef struct _Branch {
	unsigned offset : 24;
	unsigned l : 1;
	unsigned _five : 3;
	unsigned cond : 4;
}Brach;

typedef struct _CoprocDataTransfer {
	unsigned offset : 8;
	unsigned cpSHAP : 4;
	unsigned crd : 4;
	unsigned rn : 4;
	unsigned l : 1;
	unsigned w : 1;
	unsigned n : 1;
	unsigned u : 1;
	unsigned p : 1;
	unsigned _six : 3;
	unsigned cond : 4;
}CoprocDataTransfer;

typedef struct _CoprocDataOperation {
	unsigned crm : 4;
	unsigned _zero : 1;
	unsigned cp : 3;
	unsigned cpSHAP : 4;
	unsigned crd : 4;
	unsigned crn : 4;
	unsigned cpOpc : 4;
	unsigned _foreTeen : 4;
	unsigned cond : 4;
}CoprocDataOperation;

typedef struct _CoprocRegisterTransfer {
	unsigned crm : 4;
	unsigned _one : 1;
	unsigned cp : 3;
	unsigned cpSHAP : 4;
	unsigned crd : 4;
	unsigned crn : 4;
	unsigned l : 1;
	unsigned cpOpc : 3;
	unsigned _foreTeen : 4;
	unsigned cond : 4;
}CoprocRegisterTransfer;

typedef struct _S_W_Interrup {
	unsigned __empty : 24;
	unsigned _fifteen : 4;
	unsigned cond : 4;
}S_W_Interrup;

typedef struct _Mull {
	unsigned rm : 4;
	unsigned _nine : 4;
	unsigned rs : 4;
	unsigned RdLo : 4;
	unsigned RdHi : 4;
	unsigned s : 1;
	unsigned u : 2;
	unsigned _one : 5;
	unsigned cond : 4;
}Mull;

#endif