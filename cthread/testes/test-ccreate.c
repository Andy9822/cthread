/*
 *	Test Program - ccreate
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void* func0(void *arg) {
	if (*(int *)arg == 5) {
		LGA_LOGGER_TEST("First Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("First Test FAILED");
	}
	return 0;
}

void* func1(void *arg) {
	if (*(int *)arg == 2) {
		LGA_LOGGER_TEST("Second Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("Second Test FAILED");
	}
	return 0;
}

int main(int argc, char *argv[]) {

	int	id0, id1;
	int i = 5;

	LGA_LOGGER_TEST("Testing ccreate and cjoin");

	LGA_LOGGER_TEST("Creating both threads");

	id0 = ccreate(func0, (void *)&i, 0);

	id1 = ccreate(func1, (void *)&i, 0);

	LGA_LOGGER_TEST("Main after creating both threads");

	printf("%d\n", id0);
	printf("%d\n", id1);
	cjoin(id0);

	LGA_LOGGER_TEST("Main after thread 1");

	i = 2;

	cjoin(id1);

	LGA_LOGGER_TEST("Main after thread 2");

	return 0;
}
