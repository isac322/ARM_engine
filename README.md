# ARM v7 processor emulator

_**This is just fork repository. [Original repo](https://bitbucket.org/isac322/arm_engine/)**_

- interpret ARMv7 Assembly code

## Environment
- C99 support c compiler
  
## Usage
	cat <Assembly code> | ./a.out
	
## example code
### Assembly code
``` assembly
00008354 <main>:
	8354:	e92d4000 	push	{lr}
	8358:	e3a01000 	mov	r1, #0
	835c:	e3a02001 	mov	r2, #1
	8360:	e3a03001 	mov	r3, #1
	8364:	e3a04002 	mov	r4, #2
	8368:	e1530004 	cmp	r3, r4
	836c:	0b000004 	bleq	8384 <test>
	8370:	e1540003 	cmp	r4, r3
	8374:	0b000002 	bleq	8384 <test>
	8378:	e1520003 	cmp	r2, r3
	837c:	0b000000 	bleq	8384 <test>
	8380:	e8bd8000 	pop	{pc}

00008384 <test>:
	8384:	e2811001 	add	r1, r1, #1
	8388:	e12fff1e 	bx	lr
```

### C code
``` C
int a, b, c, d;

void test() {
	b++;
	return;
}

void main() {
	a = 0;
	b = 1;
	c = 1;
	d = 2;
	
	if (c == d) {
		test();
	} else if (d == c) {
		test();
	} else if (b == c) {
		test();
	}
	
	return;
}
```
	
## Issue
- swi() (Software Inturrupt) does not implement. **swi() just print all register and kill program.**
