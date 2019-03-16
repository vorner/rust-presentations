#include "morse.h"

#include <stdio.h>

int main(int argc, const char *argv[]) {
	int ch;
	while ((ch = getchar()) != EOF) {
		demorse(ch);
	}
	return 0;
}
