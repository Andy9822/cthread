/*
 *	Test Program - csuspend
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void* func0(void *arg) {
  LGA_LOGGER_TEST("Entering First thread");

	if (*(int *)arg == 5) {
		LGA_LOGGER_TEST("Thread Creation Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("Thread Creation Test FAILED");
    }

	cyield();
    

	LGA_LOGGER_TEST("First Thread ended - Shouldn't happen");

	return 0;
}

void* func1(void *arg) {
  	LGA_LOGGER_TEST("Entering Second thread");

	LGA_LOGGER_TEST("Suspending First thread");
	csuspend(*(int *)arg);

	LGA_LOGGER_TEST("Second Thread ended");

	return 0;
}

int main(int argc, char *argv[]) {

	int	id0, id1;
    int i = 5;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating threads");

	id0 = ccreate(func0, (void *)&i, 0);
	id1 = ccreate(func1, (void *)&id0, 0);

	LGA_LOGGER_TEST("Main after creating threads");

	cjoin(id1);

	LGA_LOGGER_TEST("YIELDING MAIN THREAD");
	cyield();
	LGA_LOGGER_TEST("IF IT HASN'T RETURNED TO THE FIRST THREAD IT HAS SUCCEEDED");


	LGA_LOGGER_TEST("TEST COMPLETED");

	return 0;
}
