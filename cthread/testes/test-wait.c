/*
 *	Test Program - ccreate
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

int i = 5;
csem_t s1;
void* func0(void *arg) {
	LGA_LOGGER_IMPORTANT("[func0] trying cwait");
	cwait(&s1);
	return 0;
}



int main(int argc, char *argv[]) {

	LGA_LOGGER_TEST("TEST INITIATED");
	csem_init(&s1,1);

	LGA_LOGGER_TEST("Creating thread");

	ccreate(func0, (void *)&i, 0);

	LGA_LOGGER_TEST("Main before wait");

	cwait(&s1);

	printf("Usei semaforo e lockei\n" );

	LGA_LOGGER_TEST("Main after wait");

	LGA_LOGGER_TEST("Main before first CYIELD");

	cyield();

	LGA_LOGGER_TEST("Returned to main");

	LGA_LOGGER_TEST("TEST COMPLETED");
	

	s1.count = 1;

	return 0;
}
