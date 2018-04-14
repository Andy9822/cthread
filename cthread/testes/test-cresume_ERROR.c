/*
 *	Test Program - cyield not using Semáforo
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void * func0(void* arg){

    LGA_LOGGER_TEST("Entering FIRST thread");

    int result = cresume(*(int *)arg);


    if(result < 0){
        LGA_LOGGER_TEST("RESUMING IN EXECUTION THREAD TEST SUCCEEDED");
    } else {
        LGA_LOGGER_TEST("RESUMING IN EXECUTION THREAD TEST FAILED");
    }


    cyield();

    return 0;

}

void * func1(void* arg){

    int result = cresume(*(int*)arg);

    if(result < 0){
        LGA_LOGGER_TEST("TEST SUCCEEDED");
    } else {
        LGA_LOGGER_TEST("TEST FAILED");
    }


    return 0;
}

int main(int argc, char *argv[]){

    int id0, id1;
    int falseTid = 919;

	  LGA_LOGGER_TEST("TEST INITIATED");

	  LGA_LOGGER_TEST("Creating threads");

    id0 = ccreate(func0, (void *)&id0, 0);
    id1 = ccreate(func1, (void *)&id0, 0);

    cjoin(id0);


    int result = cresume(falseTid);

    if(result < 0){
      LGA_LOGGER_TEST("NO THREAD WITH THIS TID TEST SUCCEEDED");
    } else {
      LGA_LOGGER_TEST("NO THREAD WITH THIS TID TEST FAILED");
    }


    LGA_LOGGER_TEST("TEST COMPLETED");

    return 0;
}
