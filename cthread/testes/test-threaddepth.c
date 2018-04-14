/*
 *	Test Program - cyield not using Semáforo
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

void * func0(void* arg);
void * func1(void* arg);
void * func2(void* arg);
void * func3(void* arg);


void * func0(void* arg){


    LGA_LOGGER_TEST("ARRIVED AT FUNCTION 0");

    int id1, id2, id3;

    id1 = ccreate(func1, (void *)arg, 0);
    id2 = ccreate(func2, (void *)&id1, 0);
    id3 = ccreate(func3, (void *)arg, 0);

    cjoin(id1);

    LGA_LOGGER_TEST("WAITING FOR FUNCTION 3");


    return 0;
}

void * func1(void* arg){
    

    LGA_LOGGER_TEST("ARRIVED AT FUNCTION 1");

    int id2, id3;
    
    id2 = ccreate(func2, (void *)arg, 0);
    id3 = ccreate(func3, (void *)arg, 0);

    csuspend(*(int *)arg);



    return 0;
}

void * func2(void* arg){


    LGA_LOGGER_TEST("ARRIVED AT FUNCTION 2");


    cresume(*(int *)arg);

    int id3;

    id3 = ccreate(func3, (void *)arg, 0);

    return 0;
}


void * func3(void* arg){

    LGA_LOGGER_TEST("ARRIVED AT FUNCTION 3");


    csuspend(*(int*)arg);

    return 0;
}

int main(int argc, char *argv[]){

    int id0, id1, id2, id3;
    int i = 0;

	LGA_LOGGER_TEST("TEST INITIATED");

	LGA_LOGGER_TEST("Creating threads");

    id0 = ccreate(func0, (void *)&i, 0);
    cjoin(id0);
    id1 = ccreate(func1, (void *)&i, 0);
    id2 = ccreate(func2, (void *)&i, 0);
    id3 = ccreate(func3, (void *)&i, 0);



    LGA_LOGGER_TEST("TEST COMPLETED");

    return 0;
}
