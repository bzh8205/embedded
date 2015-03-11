#include <stdlib.h>
#include <stdio.h>
#include "ThreadRunner.h"

int main(int argc, char *argv[]) {
	printf("Welcome to the QNX Momentics IDE\n");

	startThreads();
	endThreads();

	return EXIT_SUCCESS;
}
