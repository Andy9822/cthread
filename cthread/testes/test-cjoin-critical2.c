/*
 *	Test Program - cjoin-critical2
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

int id0, id1, id2, id3;

void* func1(void *arg) {
  LGA_LOGGER_TEST("[07] Entering in the second thread");

	if (*(int *) arg == 10) {
		LGA_LOGGER_TEST("[08] SECOND TEST SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("[??] SECOND TEST FAILED");
	}

	return 0;
}

void* func0(void *arg) {
	LGA_LOGGER_TEST("[04] Entering in the first thread");
	int i2 = 10;
	id1 = ccreate(func1, (void *)&i2, 0);

	if(csuspend(id1) != 0) {
		LGA_LOGGER_ERROR("ERROR SUSPENDING SECOND THREAD");
		LGA_LOGGER_TEST("[??] SUSPENDING SECOND THREAD");
	}

	LGA_LOGGER_TEST("[05] CYIELDING without any thread in apt");

	cyield();

	if (cresume(id1) == 0)
		LGA_LOGGER_TEST("[06] RESUMING SECOND THREAD");

	cyield();

	LGA_LOGGER_TEST("[09] BACKING TO FIRST THREAD");

	if (*(int *) arg == 5) {
		LGA_LOGGER_TEST("[10] FIRST TEST SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("[??] FIRST TEST FAILED");
	}
	return 0;
}


int main(int argc, char *argv[]) {

	int i = 5;

	LGA_LOGGER_TEST("[01] TEST INITIATED");

	LGA_LOGGER_TEST("[02] Creating both threads");

	id0 = ccreate(func0, (void *)&i, 0);

	LGA_LOGGER_TEST("[03] MAIN AFTER CREATING THE THREAD");

	cjoin(id0);

	LGA_LOGGER_TEST("[11] MAIN AFTER CYIELD");


  LGA_LOGGER_TEST("[12] TEST COMPLETED");

	return 0;
}
