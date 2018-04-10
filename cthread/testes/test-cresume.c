/*
 *	Test Program - cresume
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void* func0(void *arg) {
  LGA_LOGGER_TEST("Entering in thread");

	if (*(int *)arg == 5) {
		LGA_LOGGER_TEST("Thread Creation Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("Thread Creation Test FAILED");
    }

	LGA_LOGGER_TEST("Finished Thread");

	return 0;
}

int main(int argc, char *argv[]) {

	int	id0;
    int i = 5;
    int result;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating threads");

	id0 = ccreate(func0, (void *)&i, 0);

	LGA_LOGGER_TEST("Main after creating threads");

    csuspend(id0);
    LGA_LOGGER_TEST("Thread Suspended");
    result = cresume(id0);
    cjoin(id0);

    if(result < 0){
        LGA_LOGGER_TEST("TEST FAILED");
    } else {
        LGA_LOGGER_TEST("TEST COMPLETED");
    }
	return 0;
}
