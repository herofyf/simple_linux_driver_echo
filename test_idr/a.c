#include <stdio.h>

int main() {
	
	int a = 3;
	int b = (a & (a -1));
	printf("a = %d, and b = %d.\n", a, b);

	a = 4;
	b = (a & (a - 1));
	printf("a = %d, and b = %d.\n", a, b);
	return 0;

}
