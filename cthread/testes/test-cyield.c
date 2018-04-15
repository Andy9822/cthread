/*
 *	Test Program - cyield
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

	LGA_LOGGER_TEST("Second thread before cyield to modify the number");

	cyield();

	LGA_LOGGER_TEST("Second thread after cyield");

	if (*(int *)arg == 2) {
		LGA_LOGGER_TEST("Second Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("Second Test FAILED");
	}
	return 0;
}

int main(int argc, char *argv[]) {

	int i = 5;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating both threads");

	ccreate(func0, (void *)&i, 0);

	ccreate(func1, (void *)&i, 0);

	LGA_LOGGER_TEST("Main before first CYIELD");

	cyield();

	LGA_LOGGER_TEST("Main after first CYIELD");

	i = 2;

	LGA_LOGGER_TEST("Main before second CYIELD");

	cyield();

	LGA_LOGGER_TEST("Main after second CYIELD");

  LGA_LOGGER_TEST("TEST COMPLETED");

	return 0;
}
