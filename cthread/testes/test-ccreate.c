/*
 *	Test Program - ccreate
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void* func0(void *arg) {
	LGA_LOGGER_TEST("[03] Entering in the thread");
	if (*(int *)arg == 5) {
		LGA_LOGGER_TEST("[04] First Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("[??] First Test FAILED");
	}
	return 0;
}

int main(int argc, char *argv[]) {

	int	id0;
	int i = 5;

	LGA_LOGGER_TEST("[01] TEST INITIATED");

	id0 = ccreate(func0, (void *)&i, 0);

	LGA_LOGGER_TEST("[02] Main after creating thread");

	cjoin(id0);

	LGA_LOGGER_TEST("[05] TEST COMPLETED");

	return 0;
}
