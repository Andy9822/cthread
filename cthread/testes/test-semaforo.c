/*
*   Test Program - Semáforo
*
*   Creating three threads, we stablish an order for critical section access.
*   Thread 1 asks for the 'SEMAFORO' first, followed by the second thread. While the first thread asks first, 
*   it will be the second who has the final say on the critical section value, for we are using a 'FIFO' structure.
*   After the third thread signals the 'SEMAFORO' and ends, the first in the line (thread 1) wakes up and changes the critical
*   variable's value. When this thread signals the 'SEMAFORO' and ends, it will be the second thread's turn to set the variable's value.
*   We return to the main thread to execute the final test. 
*/


#include <stdio.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/LGA_logger.h"

csem_t sem;

void * func0(void* arg){


    LGA_LOGGER_TEST("ENTERED FIRST THREAD");

    if(*(int *)arg == 5){
        LGA_LOGGER_TEST("THREAD CREATION SUCCEEDED");
    }else{
        LGA_LOGGER_TEST("THREAD CREATION FAILED");
    }
    

    LGA_LOGGER_TEST("FIRST THREAD STARTS TO SLEEP");
    cwait(&sem);
    LGA_LOGGER_TEST("FIRST THREAD WAKES UP");

    *(int *) arg = 3;

    LGA_LOGGER_TEST("FIRST THREAD SIGNALS SECOND THREAD");
    csignal(&sem);

    LGA_LOGGER_TEST("FIRST THREAD ENDS");
    return 0;

}

void * func1(void* arg){


    LGA_LOGGER_TEST("ENTERED SECOND THREAD");

    if(*(int *)arg == 5){
        LGA_LOGGER_TEST("THREAD CREATION SUCCEEDED");
    }else{
        LGA_LOGGER_TEST("THREAD CREATION FAILED");
    }
    

    LGA_LOGGER_TEST("SECOND THREAD STARTS TO SLEEP");
    cwait(&sem);
    LGA_LOGGER_TEST("SECOND THREAD WAKES UP");

    *(int *) arg = 7;

    LGA_LOGGER_TEST("SECOND THREAD ENDS");
    return 0;

}

void * func2(void* arg){

    LGA_LOGGER_TEST("ENTERED THIRD THREAD");

    if(*(int *)arg == 5){
        LGA_LOGGER_TEST("THREAD CREATION SUCCEEDED");
    }else{
        LGA_LOGGER_TEST("THREAD CREATION FAILED");
    }
    
    *(int *) arg = 70;




    LGA_LOGGER_TEST("THIRD THREAD SIGNALS FIRST THREAD");
    csignal(&sem);
    LGA_LOGGER_TEST("THIRD THREAD ENDS");

    return 0;

}




int main(int argc, char * argv[]){

    int id1;
    int i = 5;


    csem_init(&sem, 0);

    LGA_LOGGER_TEST("Creating both threads");

    ccreate(func0, (void *)&i, 0);
    id1 = ccreate(func1, (void *)&i, 0);
    ccreate(func2, (void *)&i, 0);

    cjoin(id1);

    if(i == 7){        
        LGA_LOGGER_TEST("TEST SUCCEEDED");
    }else{
        LGA_LOGGER_TEST("TEST FAILED");
    }

    LGA_LOGGER_TEST("TEST COMPLETED");

    return 0;
}