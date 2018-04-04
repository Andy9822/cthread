#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ucontext.h"

#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/LGA_logger.h"
#include "../include/LGA_support.h"

FILA2 apt, exec, bloq;
int init = 1;
ucontext_t *final_context = NULL, *thread_release = NULL;
TCB_t *main_tcb = NULL;

int LGA_init();
/*
  Functions to avoid reewriting code and ease the process of developing
  this program
 */
void LGA_next_thread();
void* LGA_find_element(int tid);
int LGA_dispose_exec_thread();
int LGA_move_queues(int tid, PFILA2 removeQueue, PFILA2 insert, int state);

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
  TCB_t*tcb = (TCB_t *) malloc(sizeof(TCB_t));

  if (init) {
    if(LGA_init() == SUCCEEDED) {
      LGA_LOGGER_LOG("Init completed");
    } else {
      LGA_LOGGER_ERROR("Init couldnt be completed");
      return FAILED;
    }
  }

  if (getcontext(&(tcb->context)) != SUCCEEDED) {
    LGA_LOGGER_ERROR("The context couldnt be cloned");
    return FAILED;
  }

  LGA_LOGGER_LOG("Creating New Context");
  tcb->tid = (int) Random2();
  tcb->state = PROCST_APTO;
  tcb->context.uc_link = final_context;
  tcb->context.uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  tcb->context.uc_stack.ss_size = STACK_SIZE;

  LGA_LOGGER_LOG("Changing the New Thread context");
  makecontext(&(tcb->context), (void (*) (void)) start, 1, (void *)arg);

  if (AppendFila2(&apt, (void *)tcb) == SUCCEEDED) {
    LGA_LOGGER_LOG("Insert Succesfully");
    /* Update the main thread context */
    getcontext(&(main_tcb->context));
    return tcb->tid;
  } else {
    LGA_LOGGER_ERROR("The new thread couldn't be inserted in the apt queue");
    return FAILED;
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
  TCB_t *tcb_resumed = NULL, *tcb_blocked = NULL;

  if(FirstFila2(&apt) == SUCCEEDED) {
    LGA_LOGGER_LOG("[cjoin] Apt is not empty");
  } else {
    LGA_LOGGER_WARNING("[cjoin] Apt is empty");
    return FAILED;
  }

  if(FirstFila2(&exec) == SUCCEEDED) {
    LGA_LOGGER_LOG("[cjoin] Exec is not empty");
  } else {
    LGA_LOGGER_WARNING("[cjoin] Exec is empty");
    return FAILED;
  }

  tcb_blocked = (TCB_t *) GetAtIteratorFila2(&exec);
  if (tcb_blocked == NULL) {
    LGA_LOGGER_WARNING("[cjoin] Exec is empty");
  }
  if (DeleteAtIteratorFila2(&exec) == SUCCEEDED) {
    LGA_LOGGER_LOG("[cjoin] Removed the first element from exec");
    if(AppendFila2(&bloq, (void *) tcb_blocked) == SUCCEEDED) {
      tcb_blocked->state = PROCST_BLOQ;
      LGA_LOGGER_LOG("[cjoin] Inserted the first element from exec to bloq");
    } else {
      LGA_LOGGER_ERROR("[cjoin] The first element from exec couldnt be inserted into bloq");
      return FAILED;
    }
  } else {
    LGA_LOGGER_LOG("[cjoin] The first element couldnt be removed from exec");
  }

  if (LGA_tidInsideFila(&apt, tid) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[cjoin] Thread isnt in the apt queue");
    return FAILED;
  }

  tcb_resumed = (TCB_t *) LGA_tidGetFila(&apt, tid);

  if (tcb_resumed == NULL) {
    LGA_LOGGER_WARNING("[cjoin] TCB released is NULL");
    return END_CONTEXT;
  }

  if (DeleteAtIteratorFila2(&apt) == SUCCEEDED) {
    LGA_LOGGER_LOG("[cjoin] Removed the first element from apt");
    if(AppendFila2(&exec, (void *) tcb_resumed) == SUCCEEDED) {
      tcb_resumed->state = PROCST_EXEC;
      makecontext(tcb_resumed->context.uc_link, (void (*) (void)) LGA_thread_release, 1, (void *)&(tcb_blocked->tid)),
      LGA_LOGGER_LOG("[cjoin] Inserted the first element from apt to exec");
    } else {
      LGA_LOGGER_ERROR("[cjoin] The first element from apt couldnt be inserted into exec");
      return FAILED;
    }
  } else {
    LGA_LOGGER_LOG("[cjoin] The first element couldnt be removed from apt");
    return FAILED;
  }
  setcontext(&(tcb_resumed->context));
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

/*
  Initialize the used structures
  Return 0 - SUCCEEDED
  Return -1 - FAILED
  */
int LGA_init() {
  init = 0;
  final_context = (ucontext_t *) malloc(sizeof(ucontext_t));
  thread_release = (ucontext_t *) malloc(sizeof(ucontext_t));
  main_tcb = (TCB_t *) malloc(sizeof(TCB_t));

  if(CreateFila2(&apt) == SUCCEEDED) {
    LGA_LOGGER_LOG("Created the Apt Queue");
  } else {
    LGA_LOGGER_ERROR("Apt Queue couldnt be created");
    return FAILED;
  }
  if(CreateFila2(&exec) == SUCCEEDED) {
    LGA_LOGGER_LOG("Created the Exec Queue");
  } else {
    LGA_LOGGER_ERROR("Exec Queue couldnt be created");
    return FAILED;
  }
  if(CreateFila2(&bloq) == SUCCEEDED) {
    LGA_LOGGER_LOG("Created the Bloq Queue");
  } else {
    LGA_LOGGER_ERROR("Bloq Queue couldnt be created");
    return FAILED;
  }

  if (getcontext(&(main_tcb->context)) == SUCCEEDED) {
    main_tcb->tid = 0;
    main_tcb->state = PROCST_EXEC;
    main_tcb->context.uc_stack.ss_sp = (char*) malloc(163840);
    main_tcb->context.uc_stack.ss_size = 163840;
    main_tcb->context.uc_link = NULL;
    LGA_LOGGER_LOG("Main Thread Created");
  } else {
    LGA_LOGGER_ERROR("The Main thread couldnt be created");
    return FAILED;
  }
  if (AppendFila2(&exec, main_tcb) == SUCCEEDED) {
    LGA_LOGGER_LOG("Main Thread inserted in exec");
  } else {
    LGA_LOGGER_ERROR("The Main thread couldnt be inserted in the exec queue");
    return FAILED;
  }

  if (getcontext(final_context) != SUCCEEDED) {
    LGA_LOGGER_ERROR("Couldnt get the final context");
    return FAILED;
  }
  LGA_LOGGER_LOG("Creating Final Context");
  final_context->uc_link = NULL;
  final_context->uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  final_context->uc_stack.ss_size = STACK_SIZE;
  makecontext(final_context, (void (*) (void)) LGA_final, 0);

  if (getcontext(thread_release) != SUCCEEDED) {
    LGA_LOGGER_ERROR("Couldnt get the final context");
    return FAILED;
  }
  LGA_LOGGER_LOG("Creating Thread Release Context");
  thread_release->uc_link = NULL;
  thread_release->uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  thread_release->uc_stack.ss_size = STACK_SIZE;
  makecontext(thread_release, (void (*) (void)) LGA_thread_release, 0);

  return SUCCEEDED;
}

/*
  When a context finalizes its function this function should be called
  because this function should be at context.uc_link
  This function move a thread from Apt to Exec and Free the context structure
 */
void* LGA_final(void *arg) {
  TCB_t *tcb_finalized = NULL, *tcb_resumed = NULL;

  if(FirstFila2(&apt) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_final] Apt is not empty");
  } else {
    LGA_LOGGER_WARNING("[LGA_final] Apt is empty");
    return END_CONTEXT;
  }

  if(FirstFila2(&exec) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_final] Exec is not empty");
  } else {
    LGA_LOGGER_WARNING("[LGA_final] Exec is empty");
    return END_CONTEXT;
  }

  tcb_finalized = (TCB_t *) GetAtIteratorFila2(&exec);

  if (DeleteAtIteratorFila2(&exec) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_final] Freeing the first element from exec");
    free(tcb_finalized->context.uc_stack.ss_sp);
    free(tcb_finalized);
  } else {
    LGA_LOGGER_LOG("[LGA_final] The first element couldnt be removed from exec");
  }

  tcb_resumed = (TCB_t *) GetAtIteratorFila2(&apt);
/*
  if (DeleteAtIteratorFila2(&apt) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_final] Removed the first element from apt");
    if(AppendFila2(&exec, (void *) tcb_resumed) == SUCCEEDED) {
      tcb_resumed->state = PROCST_EXEC;
      LGA_LOGGER_LOG("[LGA_final] Inserted the first element from apt to exec");
    } else {
      LGA_LOGGER_ERROR("[LGA_final] The first element from apt couldnt be inserted into exec");
      return END_CONTEXT;
    }
  } else {
    LGA_LOGGER_LOG("[LGA_final] The first element couldnt be removed from apt");
    return END_CONTEXT;
  }*/
  LGA_move_queues(tcb_resumed->tid, &apt, &exec, PROCST_EXEC);

  LGA_LOGGER_LOG("[LGA_final] Swapping the context");
  setcontext(&(tcb_resumed->context));
  return END_CONTEXT;
}

void* LGA_thread_release(void *tid) {
  TCB_t *tcb_released = NULL;
/*
  if (LGA_tidInsideFila(&bloq, *(int*)tid) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[LGA_thread_release] Thread isnt in the bloq queue");
    return END_CONTEXT;
  }*/
/*
  tcb_released = (TCB_t *) LGA_tidGetFila(&bloq, *(int*)tid);
  if (tcb_released == NULL) {
    LGA_LOGGER_WARNING("[LGA_thread_release] TCB released is NULL");
    return END_CONTEXT;
  }

  if (LGA_tidRemoveFila(&bloq, *(int*)tid) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_thread_release] Remove the element from bloq");
    if(AppendFila2(&apt, (void *) tcb_released) == SUCCEEDED) {
      tcb_released->state = PROCST_APTO;
      LGA_LOGGER_LOG("[LGA_thread_release] Insert the element from bloq to apt");
    } else {
      LGA_LOGGER_ERROR("[LGA_thread_release] The element from bloq couldnt be inserted into apt");
      return END_CONTEXT;
    }
  } else {
    LGA_LOGGER_LOG("[LGA_thread_release] The element couldnt be removed from bloq");
    return END_CONTEXT;
  }*/
  if (LGA_move_queues(*(int *) tid, &bloq, &apt, PROCST_APTO) != SUCCEEDED) {
    LGA_LOGGER_ERROR("Couldnt move from bloq to apt");
    return END_CONTEXT;
  }
  makecontext(final_context, (void (*) (void)) LGA_final, 0);
  setcontext(final_context);
  return END_CONTEXT;
}

/*
  Move a Tcb_t that has the TID from removeQueue to insertQueue and change
  its state to new state
  Return 0 - SUCCEEDED
  Return -1 - FAILED
 */
int LGA_move_queues(int tid, PFILA2 removeQueue, PFILA2 insertQueue, int state) {
  TCB_t *tcb = NULL;

  tcb = (TCB_t *) LGA_tid_get_from_fila(removeQueue, tid);

  if (tcb == NULL) {
    LGA_LOGGER_ERROR("[LGA_move_queues] TCB is null");
    return FAILED;
  }

  if (LGA_tid_remove_from_fila (removeQueue, tid) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[LGA_move_queues] Couldnt remove from the queue");
    return FAILED;
  }

  if(AppendFila2(insertQueue, (void *) tcb) == SUCCEEDED) {
    tcb->state = state;
    LGA_LOGGER_LOG("[LGA_move_queues] Inserted first element");
  } else {
    LGA_LOGGER_ERROR("[LGA_move_queues] The element from apt couldnt be insertQueueed");
    return FAILED;
  }
  return SUCCEEDED;
}

/*
  Move the first element from Apt Queue to Exec Queue
 */
void LGA_next_thread() {
  TCB_t *tcb;

  if (FirstFila2(&apt) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[LGA_next_thread] Couldnt set the iterator to the first element of apt");
    return;
  }

  tcb = (TCB_t *) GetAtIteratorFila2(&apt);

  if(tcb == NULL) {
    LGA_LOGGER_ERROR("[LGA_next_thread] TCB is null");
    return;
  }

  if(LGA_move_queues(tcb->tid, &apt, &exec, PROCST_EXEC) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[LGA_next_thread] Couldnt move the element from apt to exec");
    return;
  }

  LGA_LOGGER_LOG("Setting the thread in exec to execute");
  setcontext(&(tcb->context));
}

/*
  Free the element of EXEC
  Return 0 - SUCCEEDED
  Return -1 - FAILED
 */
int LGA_dispose_exec_thread() {
  TCB_t *tcb_disposed;

  if(FirstFila2(&exec) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_dispose_exec_thread] Exec is not empty");
  } else {
    LGA_LOGGER_WARNING("[LGA_dispose_exec_thread] Exec is empty");
    return FAILED;
  }

  tcb_disposed = (TCB_t *) GetAtIteratorFila2(&exec);

  if (DeleteAtIteratorFila2(&exec) == SUCCEEDED) {
    free(tcb_disposed->context.uc_stack.ss_sp);
    free(tcb_disposed);
    LGA_LOGGER_LOG("[LGA_dispose_exec_thread] Disposed the first element from exec");
    return SUCCEEDED;
  } else {
    LGA_LOGGER_ERROR("[LGA_dispose_exec_thread] The element of exec couldnt be disposed");
    return FAILED;
  }
}

void* LGA_find_element(int tid) {
  if(LGA_tid_inside_of_fila(&apt, tid) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_find_queue] Found the tid inside the Apt Queue");
    return (void *)LGA_tid_get_from_fila(&apt, tid);
  }

  if(LGA_tid_inside_of_fila(&bloq, tid) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_find_queue] Found the tid inside the Bloq Queue");
    return (void *)LGA_tid_get_from_fila(&bloq, tid);
  }
  LGA_LOGGER_WARNING("[LGA_find_queue] The element isnt in the Apt nor Bloq queues");
  return NULL;
}
