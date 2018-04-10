/*
 *	Test Program - cjoin
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void* func0(void *arg) {
	LGA_LOGGER_TEST("Entering in the first thread");
	if (*(int *)arg == 5) {
		LGA_LOGGER_TEST("First Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("First Test FAILED");
	}
	return 0;
}

void* func1(void *arg) {
  LGA_LOGGER_TEST("Entering in the second thread");

	if (*(int *)arg == 5) {
		LGA_LOGGER_TEST("Second Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("Second Test FAILED");
	}
	return 0;
}

int main(int argc, char *argv[]) {

	int	id0, id1;
	int i = 5;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating both threads");

	id0 = ccreate(func0, (void *)&i, 0);

	id1 = ccreate(func1, (void *)&i, 0);

	LGA_LOGGER_TEST("Main after creating both threads");

	cjoin(id0);

	i = 2;

	cjoin(id1);

  LGA_LOGGER_TEST("TEST COMPLETED");

	return 0;
}
