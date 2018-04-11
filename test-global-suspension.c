/*
 *	Test Program - global suspension
 *
 *	Uma main com 1 thread e uma variavel global i
 *  Essa thread cria 2 threads e as suspende
 *  Resumeia só uma dessas threads e da cjoin, e esta que deverá resumar a outra
 *  Disclaimer do multicore sepa dar merda cjoin
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"


int global_i = 10, global_id;

void* func1(void *arg);
void* func2(void *arg);

void* func0(void *arg) {
	int id1,id2,result1,result2, i = 8;

	LGA_LOGGER_TEST("Entering in the first thread");

	if (*(int *)arg == 5 && global_i == 10) {
		LGA_LOGGER_TEST("First Thread Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("First Thread Test FAILED");
	}

	LGA_LOGGER_TEST("Creating two threads in First Thread");

	id1 = ccreate(func1, (void *)&i, 0);
	id2 = ccreate(func2, (void *)&i, 0);

	LGA_LOGGER_TEST("First Thread after creating two threads");
	LGA_LOGGER_TEST("Suspending both created threads in First Thread");

  result1 = csuspend(id1);
	result2 = csuspend(id2);

	if(result1 < 0){
			LGA_LOGGER_TEST("First Thread first suspend failed");
	} else {
			LGA_LOGGER_TEST("First Thread first suspend passed");
	}

	if(result2 < 0){
			LGA_LOGGER_TEST("First Thread second suspend failed");
	} else {
			LGA_LOGGER_TEST("First Thread second suspend passed");
	}

	result2 = cresume(id2);

	if(result2 < 0){
			LGA_LOGGER_TEST("First Thread first resume failed");
	} else {
			LGA_LOGGER_TEST("First Thread first resume passed");
	}


	global_id = id1;
	LGA_LOGGER_TEST("First Thread will cjoin third thread");
	cjoin(id2);
	LGA_LOGGER_TEST("First Thread will cjoin second thread");
	cjoin(id1);

	return 0;
}

void* func1(void *arg) {
  LGA_LOGGER_TEST("Entering in the second thread");

	if (*(int *)arg == 5 && global_i == 10) {
		LGA_LOGGER_TEST("Second thread test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("Second thred test FAILED");
	}
	return 0;
}

void* func2(void *arg) {
	int result;
  LGA_LOGGER_TEST("Entering in the third thread");

	if (*(int *)arg == 8 && global_i == 10) {
		LGA_LOGGER_TEST("Third Test SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("Third Test FAILED");
	}

	result = cresume(global_id);

	if(result < 0){
			LGA_LOGGER_TEST("Third Thread resume failed");
	} else {
			LGA_LOGGER_TEST("Third Thread resume passed");
	}
	return 0;
}

int main(int argc, char *argv[]) {

	int	id0, i = 5;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating one thread in main");

	id0 = ccreate(func0, (void *)&i, 0);

	LGA_LOGGER_TEST("Main after creating one thread");

	cjoin(id0);

  LGA_LOGGER_TEST("TEST COMPLETED");

	return 0;
}
