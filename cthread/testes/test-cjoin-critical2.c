/*
 *	Test Program - cjoin-critical2
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

int id0, id1, id2, id3;

void* func1(void *arg) {
  LGA_LOGGER_TEST("Entering in the second thread");

	if (*(int *) arg == 10) {
		LGA_LOGGER_TEST("SECOND TEST SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("SECOND TEST FAILED");
	}

	return 0;
}

void* func0(void *arg) {
	LGA_LOGGER_TEST("Entering in the first thread");
	int i2 = 10;
	id1 = ccreate(func1, (void *)&i2, 0);

	if(csuspend(id1) != 0) {
		LGA_LOGGER_ERROR("SUSPENDING SECOND THREAD");
	}

	LGA_LOGGER_TEST("CYIELDING without any thread in apt");

	cyield();

	if (cresume(id1) == 0)
		LGA_LOGGER_TEST("RESUMING SECOND THREAD");

	cyield();

	LGA_LOGGER_TEST("BACKING TO FIRST THREAD");

	if (*(int *) arg == 5) {
		LGA_LOGGER_TEST("FIRST TEST SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("FIRST TEST FAILED");
	}
	return 0;
}


int main(int argc, char *argv[]) {

	int i = 5;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating both threads");

	id0 = ccreate(func0, (void *)&i, 0);

	LGA_LOGGER_TEST("MAIN AFTER CREATING THE THREAD");

	cjoin(id0);

	LGA_LOGGER_TEST("MAIN AFTER CYIELD");


  LGA_LOGGER_TEST("TEST COMPLETED");

	return 0;
}
