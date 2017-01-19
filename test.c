#include <stdio.h>

int main(void) {

	volatile int pikachy;
	char buf[32];

	printf("%p\n", &pikachy);
	printf("%p\n", &buf);

}
