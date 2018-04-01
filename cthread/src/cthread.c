#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/mysupport.h"
#include "../include/LGA_logger.h"

#define EQUALS 0
#define SUCCEEDED 0
#define FAILED -1

FILA2 apt, exec, bloq;
TCB_t mainThread;
int firstThread = 1;

/******************************************************************************
Par�metros:
	name:	ponteiro para uma �rea de mem�ria onde deve ser escrito um string que cont�m os nomes dos componentes do grupo e seus n�meros de cart�o.
		Deve ser uma linha por componente.
	size:	quantidade m�xima de caracteres que podem ser copiados para o string de identifica��o dos componentes do grupo.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cidentify (char *name, int size) {
  int i = 0;
	char identity[73] = {0};

	strcpy(identity, "Leonardo 00274721\nAndy Garramones 00274XXX\nGuilherme Haetinger 00274XXX\n");
	for(i=0;i<size && i < 73;i++) {
		name[i] = identity[i];
	}
  if(strcmp(identity,name) == EQUALS) {
    return SUCCEEDED;
  } else {
    return FAILED;
  }
};

/******************************************************************************
Par�metros:
	start:	ponteiro para a fun��o que a thread executar�.
	arg:	um par�metro que pode ser passado para a thread na sua cria��o.
	prio:	N�O utilizado neste semestre, deve ser sempre zero.
Retorno:
	Se correto => Valor positivo, que representa o identificador da thread criada
	Se erro	   => Valor negativo.
******************************************************************************/
int ccreate (void* (*start)(void*), void *arg, int prio) {
  TCB_t *tcb = malloc(sizeof(TCB_t));
  char func_stack[16384] = {0};

  if(firstThread == 1) {
    firstThread = 0;
    LGA_LOGGER_LOG("Creating Main Thread");
    mainThread.tid = 0;
    mainThread.state = PROCST_APTO;
    if (getcontext(&(mainThread.context)) != 0) {
      puts("[ERROR] Cannot create main thread context");
      free(tcb);
      return FAILED;
    }
  }
  if (tcb == NULL) {
    LGA_LOGGER_ERROR("TCB Couldnt be created");
    return FAILED;
  } else {
    if (&apt == NULL) {
      CreateFila2(&apt);
    }
    if (getcontext(&(tcb->context)) == -1) {
      free(tcb);
      LGA_LOGGER_ERROR("Getting the Context");
      return FAILED;
    } else {
      getcontext(&(tcb->context));
      LGA_LOGGER_LOG("Creating the Context");
      tcb->tid = 2;
      tcb->state = PROCST_APTO;
      tcb->context.uc_stack.ss_sp = func_stack;
      tcb->context.uc_stack.ss_size = sizeof(func_stack);
      tcb->context.uc_link = &(mainThread.context);
      LGA_LOGGER_LOG("Changing the Context");
      makecontext(&(tcb->context), (void (*) (void)) start, 1, (void *)arg);
      if (InsertAfterIteratorFila2(&apt, tcb) == 0) {
        LGA_LOGGER_LOG("Inserted Succesfully");
        return SUCCEEDED;
      } else {
        free(tcb);
        LGA_LOGGER_ERROR("Inserted Failed");
        return FAILED;
      }
    }
  }
};

/******************************************************************************
Par�metros:
	Sem par�metros
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cyield(void);

/******************************************************************************
Par�metros:
	tid:	identificador da thread cujo t�rmino est� sendo aguardado.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cjoin(int tid) {
  PNODE2 node = GetAtIteratorFila2(&apt);
  swapcontext(&(mainThread.context),&((TCB_t *)node->node)->context);
  NextFila2(&apt);
  return SUCCEEDED;
};

/******************************************************************************
Par�metros:
	tid:	identificador da thread a ser suspensa.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int csuspend(int tid);

/******************************************************************************
Par�metros:
	tid:	identificador da thread que ter� sua execu��o retomada.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cresume(int tid);

/******************************************************************************
Par�metros:
	sem:	ponteiro para uma vari�vel do tipo csem_t. Aponta para uma estrutura de dados que representa a vari�vel sem�foro.
	count: valor a ser usado na inicializa��o do sem�foro. Representa a quantidade de recursos controlados pelo sem�foro.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int csem_init(csem_t *sem, int count);

/******************************************************************************
Par�metros:
	sem:	ponteiro para uma vari�vel do tipo sem�foro.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cwait(csem_t *sem);

/******************************************************************************
Par�metros:
	sem:	ponteiro para uma vari�vel do tipo sem�foro.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int csignal(csem_t *sem);
