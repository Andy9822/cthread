/*
 *	Test Program - cresume
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "../include/cthread.h"
#include "../include/LGA_logger.h"

csem_t sem;
int id1,id2;

void* func0(void *arg) {
  LGA_LOGGER_TEST("[1] func0");
  cwait(&sem);
  LGA_LOGGER_IMPORTANT("func0 locked SC");
  LGA_LOGGER_TEST("[2] cyield func0");
  cyield();


  LGA_LOGGER_TEST("[6] func0 with locking SC and suspending func1");
  csuspend(id2);
  cyield();


  LGA_LOGGER_TEST("[7] func0 unloking SC");
  csignal(&sem);
  cyield();


  LGA_LOGGER_TEST("[9] func0 resuming func1");
  cresume(id2);
  cyield();


	return 0;
}

void* func1(void *arg) {
  LGA_LOGGER_TEST("[3]func1");
  LGA_LOGGER_TEST("[4]cwait func1 WITH LOCKED SC SO WILL SLEEP");
  cwait(&sem);


  LGA_LOGGER_IMPORTANT("[10] func1 running with unlocked SC");


	return 0;
}

void* func2(void *arg) {
  LGA_LOGGER_TEST("[5] func2  after func0 lock SC and func1 sleeps for being locked SC");
  cyield();


  LGA_LOGGER_TEST("[6] func2 after func0 suspend blocked func1 while still locking SC");
  cyield();

  
  LGA_LOGGER_TEST("[8] func2 after SC unlocked but func1 now has to be on suspended apt");
	return 0;
}


int main(int argc, char *argv[]) {
	LGA_LOGGER_TEST("TEST INITIATED");
  csem_init(&sem,1);

	id1 = ccreate(func0,NULL , 0);
  id2 = ccreate(func1, NULL, 0);
  ccreate(func2,NULL, 0);

  LGA_LOGGER_TEST("Main cjoin func1");
  cjoin(id1);
  LGA_LOGGER_TEST("COMPLETED TEST");


	return 0;
}
