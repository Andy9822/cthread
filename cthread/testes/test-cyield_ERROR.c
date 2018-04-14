/*
 *	Test Program - cyield not using Semáforo
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void * func0(void* arg){


    LGA_LOGGER_TEST("ENTERED FIRST THREAD");

    *(int *)arg = 5;

    LGA_LOGGER_TEST("APPLIED THE RIGHT VALUE");

    cyield();

    if(*(int *)arg == 5){
        LGA_LOGGER_TEST("NO SEMAFORO TEST FAILED");
    } else {
        LGA_LOGGER_TEST("NO SEMAFORO TEST SUCCEEDED");
    }

    return 0;
}

void * func1(void* arg){

    *(int *)arg = 3;
    LGA_LOGGER_TEST("APPLIED THE INTRUSION VALUE");

    return 0;
}

int main(int argc, char *argv[]){

    int id0, id1;
    int i = 0;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating threads");

    id0 = ccreate(func0, (void *)&i, 0);
    id1 = ccreate(func1, (void *)&i, 0);

    cjoin(id0);


    LGA_LOGGER_TEST("TEST COMPLETED");

    return 0;
}
