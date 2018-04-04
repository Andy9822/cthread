#include "support.h"
#include "cdata.h"

#define EQUALS 0
#define SUCCEEDED 0
#define FAILED -1
#define END_CONTEXT 0
#define STACK_SIZE 163840

int LGA_tidInsideFila(PFILA2 pFila, int tid);

int LGA_tidRemoveFila (PFILA2 pFila, int tid);

void* LGA_tidGetFila (PFILA2 pFila, int tid);
