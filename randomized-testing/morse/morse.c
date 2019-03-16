#include "morse.h"

#include <stdio.h>
#include <stdlib.h>

static size_t acc = 1;

static const char *tbl = " ETIANMSURWDKGOHVF?L?PJBXCYZQ??54?3???2???????16???????7???8?90";

void demorse(char c) {
	switch (c) {
		case '.':
			acc *= 2;
			break;
		case '-':
			acc = acc * 2 + 1;
			break;
		default:
			putchar(tbl[acc - 1]);
			acc = 1;
	}
}
