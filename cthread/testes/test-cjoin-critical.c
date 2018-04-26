/*
 *	Test Program - cjoin-critical
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void* func0(void *arg) {
	LGA_LOGGER_TEST("[04] Entering in the first thread and cyielding");
	cyield();
	if (*(int *)arg == 5) {
		LGA_LOGGER_TEST("[07] First Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("[??] First Test FAILED");
	}
	return 0;
}

void* func1(void *arg) {
  LGA_LOGGER_TEST("[05] Entering in the second thread");
	cjoin(*(int *) arg);
	LGA_LOGGER_TEST("[06] IF THIS LOG IS BEFORE THE First Thread SUCCEEDS, it worked");

	return 0;
}

int main(int argc, char *argv[]) {

	int	id0, id1;
	int i = 5;

	LGA_LOGGER_TEST("[01] TEST INITIATED");

	LGA_LOGGER_TEST("[02] Creating both threads");

	id0 = ccreate(func0, (void *)&i, 0);

	id1 = ccreate(func1, (void *)&id0, 0);

	LGA_LOGGER_TEST("[03] Main after creating both threads");

	cjoin(id0);

	i = 2;

	cjoin(id1);

  LGA_LOGGER_TEST("[08] TEST COMPLETED");

	return 0;
}
