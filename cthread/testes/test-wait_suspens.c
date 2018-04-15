/*
 *	Test Program - cresume
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "../include/cthread.h"
#include "../include/LGA_logger.h"

csem_t sem;

void* func0(void *arg) {
  LGA_LOGGER_TEST("func0");
  cwait(&sem);
  LGA_LOGGER_IMPORTANT("func0 locked SC");
  LGA_LOGGER_TEST("cyield func0");
  cyield();
  LGA_LOGGER_TEST("func0 with locking SC and suspending func1")
  csuspend(*(int *)arg);
  cyield();
  LGA_LOGGER_TEST("func0 unloking SC");
  csignal(&sem);
  cyield()
  LGA_LOGGER_TEST("func0 resuming func1");
  cyield();


	return ;
}

void* func1(void *arg) {
  LGA_LOGGER_TEST("func1");
  LGA_LOGGER_TEST("cwait func1 WITH LOCKED SC SO WILL SLEEP");
  cwait(&sem);
  LGA_LOGGER_IMPORTANT("func1 running with unlocked SC");


	return ;
}

void* func2(void *arg) {
  LGA_LOGGER_TEST("func2  after func0 lock SC and func1 sleeps for being locked SC");
  cyield();
  LGA_LOGGER_TEST("func2 after func0 suspend blocked func1 while still locking SC");
  cyield();
  LGA_LOGGER_TEST("func2 after SC unlocked but func1 now has to be on suspended apt");
	return ;
}


int main(int argc, char *argv[]) {
  int id1, id2;
	LGA_LOGGER_TEST("TEST INITIATED");
  csem_init(&sem,1);

	id1 = ccreate(func0, , 0);
  id2 = ccreate(func1, , 0);
  ccreate(func2, , 0);

  LGA_LOGGER_TEST("Main cjoin func1");
  cjoin(id1);
  LGA_LOGGER_TEST("COMPLETED TEST");


	return 0;
}
