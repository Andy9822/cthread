/*
 *	Test Program - cyield not using Semáforo
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void * func0(void* arg){

    LGA_LOGGER_TEST("Entering FIRST thread");

    LGA_LOGGER_TEST("ENTERED LOOP");
    while(1){
	};

}

void * func1(void* arg){

    int result = csuspend(*(int*)arg);

    if(result < 0){
        LGA_LOGGER_TEST("TEST SUCCEEDED");
    } else {
        LGA_LOGGER_TEST("TEST FAILED");
    }


    return 0;
}

int main(int argc, char *argv[]){

    int id0, id1;
    int i = 5;
    int falseTid = 924;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating threads");

    id0 = ccreate(func0, (void *)&i, 0);
    id1 = ccreate(func1, (void *)&id0, 0);

    csuspend(id0);
    LGA_LOGGER_TEST("FIRST THREAD SUSPENDED");

    cjoin(id1);


    int result = csuspend(falseTid);

    if(result < 0){
      LGA_LOGGER_TEST("NO THREAD WITH THIS TID TEST SUCCEEDED");
    } else {
      LGA_LOGGER_TEST("NO THREAD WITH THIS TID TEST FAILED");
    }


    LGA_LOGGER_TEST("TEST COMPLETED");

    return 0;
}
