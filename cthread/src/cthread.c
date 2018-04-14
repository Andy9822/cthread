#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ucontext.h"

#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/LGA_logger.h"
#include "../include/LGA_support.h"

FILA2 apt, exec, bloq, apt_sus, bloq_sus, releasers;
int init = 1;
ucontext_t *final_context = NULL;
TCB_t *main_tcb = NULL;

int LGA_init();
int LGA_queues_init();
/*
  Only use as callback to change the final context of a thread
 */
void* CB_end_thread(void *arg);
void* CB_cjoin_release(void *block_releaser_in);

/*
  Functions to avoid reewriting code and ease the process of developing
  this program
 */
void LGA_next_thread();
void LGA_next_thread_swap(TCB_t *tcb);
void* LGA_find_element(int tid);
int LGA_dispose_exec_thread();
int LGA_move_queues(int tid, PFILA2 removeQueue, PFILA2 insert, int state);
void* LGA_get_first_queue(FILA2 * queue);
int LGA_block_exec_thread(TCB_t *tcb_actual);
int LGA_remove_exec(TCB_t *tcb);

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
  TCB_t *tcb = (TCB_t *) malloc(sizeof(TCB_t));
  ucontext_t *context_callback = (ucontext_t *) malloc(sizeof(ucontext_t));

  LGA_LOGGER_IMPORTANT("[ccreate] Begun");

  if (init) {
    if(LGA_init() == SUCCEEDED) {
      LGA_LOGGER_DEBUG("[ccreate] Init completed");
    } else {
      LGA_LOGGER_ERROR("[ccreate] Init couldnt be completed");
      return FAILED;
    }
  }

  if (getcontext(context_callback) != SUCCEEDED) {
    LGA_LOGGER_ERROR("The context_callback couldnt be created");
    return FAILED;
  }

  LGA_LOGGER_LOG("[ccreate] Creating New Context CALLBACK");
  context_callback->uc_stack.ss_sp = (char*) malloc(STACK_SIZE * sizeof(char));
  context_callback->uc_stack.ss_size = STACK_SIZE;
  context_callback->uc_link = NULL;
  // Here is where the magic happens. The context_callback will go to CB_end_thread
  // when its called
  makecontext(context_callback, (void (*) (void)) CB_end_thread, 0);

  if (getcontext(&(tcb->context)) != SUCCEEDED) {
    LGA_LOGGER_ERROR("The context couldnt be created");
    return FAILED;
  }

  LGA_LOGGER_LOG("[ccreate] Creating New Context");
  tcb->tid = (int) Random2();
  tcb->state = PROCST_APTO;
  tcb->context.uc_stack.ss_sp = (char*) malloc(STACK_SIZE * sizeof(char));
  tcb->context.uc_stack.ss_size = STACK_SIZE;
  // When this context is done its call the context_callback and finish it
  // in the right way
  tcb->context.uc_link = context_callback;

  LGA_LOGGER_DEBUG("[ccreate] Changing the New Thread context");
  makecontext(&(tcb->context), (void (*) (void)) start, 1, (void *)arg);

  if (AppendFila2(&apt, (void *)tcb) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[ccreate] Insert Succesfully");
    return tcb->tid;
  } else {
    LGA_LOGGER_ERROR("[ccreate] The new thread couldn't be inserted in the apt queue");
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
int cyield(void) {
  LGA_LOGGER_IMPORTANT("[cyield] Begun");

  TCB_t *tcb;
  tcb = (TCB_t *) LGA_get_first_queue(&exec);
  if(tcb == NULL){ // Caso nao tenha conseguido recuperar tcb atual
    return FAILED;
  }
  LGA_LOGGER_DEBUG("[cyield] Got the element of exec");

  if(LGA_move_queues(tcb->tid, &exec, &apt, PROCST_APTO) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[cyield]Couldnt remove from exec and insert into apt");
    return FAILED;
  }

  LGA_LOGGER_LOG("[cyield] Moved the element from exec to apt");

  LGA_next_thread_swap(tcb);

  return SUCCEEDED;
}

/******************************************************************************
Par�metros:
	tid:	identificador da thread cujo t�rmino est� sendo aguardado.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cjoin(int tid) {
  TCB_t *tcb_blocked = NULL, *tcb_releaser = NULL;
  BLOCK_RELEASER *block_releaser = NULL; // Use to ease the identification

  LGA_LOGGER_IMPORTANT("[cjoin] Begun");

  if (LGA_tid_inside_of_fila(&releasers, tid) == SUCCEEDED) {
    LGA_LOGGER_WARNING("[cjoin] This thread is already releasing another thread, \
    so you can't use it to block");
    return FAILED;
  }

  tcb_blocked = (TCB_t *) LGA_get_first_queue(&exec);
  if(tcb_blocked == NULL){ // Caso nao tenha conseguido recuperar tcb atual
    return FAILED;
  }

  tcb_releaser = (TCB_t *) LGA_find_element(tid);

  if(tcb_releaser == NULL) {
    LGA_LOGGER_WARNING("[cjoin] Theres none threads with this tid, so i wont block");
    return FAILED;
  }
  LGA_LOGGER_LOG("[cjoin] Found the releaser thread");

  if(AppendFila2(&releasers, (void *) tcb_releaser) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[cjoin] Couldnt insert the releaser into releasers queue");
    return FAILED;
  }
  LGA_LOGGER_DEBUG("[cjoin] Inserted the releaser into releasers queue");

  if (LGA_move_queues(tcb_blocked->tid, &exec, &bloq, PROCST_BLOQ) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[cjoin] Couldnt move the thread from exec to bloq");
    return FAILED;
  }
  LGA_LOGGER_DEBUG("[cjoin] Moved the thread from exec to bloq");

  // Make the callback of the TCB_TRIGGER_RELEASE be the CB_cjoin_release with
  // the tcb_blocked->tid to release it when the TCB__TRIGGER_RELEASE is done
  block_releaser = (BLOCK_RELEASER *) calloc(1, sizeof(BLOCK_RELEASER));
  if (block_releaser == NULL) {
    LGA_LOGGER_ERROR("[cjoin] Block_releaser struct is NULL");
    return FAILED;
  }
  block_releaser->tid_block = tcb_blocked->tid;
  block_releaser->tid_releaser = tcb_releaser->tid;


  makecontext(tcb_releaser->context.uc_link, (void (*) (void)) CB_cjoin_release, 1, \
    (void *)block_releaser);

  LGA_LOGGER_LOG("[cjoin] Change the UC_LINK of tcb_releaser");
  // Get the next thread in EXEC and Updated the actual context
  LGA_next_thread_swap(tcb_blocked);
  return SUCCEEDED;
};

/******************************************************************************
Par�metros:
	tid:	identificador da thread a ser suspensa.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int csuspend(int tid) {
  TCB_t *tcb_suspend;

  LGA_LOGGER_IMPORTANT("[csuspend] Begun");
  tcb_suspend = (TCB_t *) LGA_find_element(tid);
  if (tcb_suspend == NULL) {
    LGA_LOGGER_WARNING("[csuspend] Theres none threads with this tid in bloq nor apt queues");
    return FAILED;
  }
  LGA_LOGGER_LOG("[csuspend] Find the thread that will be suspended");

  if (tcb_suspend->state == PROCST_APTO) {
    if (LGA_move_queues(tcb_suspend->tid, &apt, &apt_sus, PROCST_APTO_SUS) != SUCCEEDED) {
      LGA_LOGGER_ERROR("[csuspend] Couldnt move the thread from apt to Apt Suspenso");
      return FAILED;
    }
    LGA_LOGGER_LOG("[csuspend] Moved the thread from apt to Apt Suspenso");
  }

  else if (tcb_suspend->state == PROCST_BLOQ) {
    if (LGA_move_queues(tcb_suspend->tid, &bloq, &bloq_sus, PROCST_BLOQ_SUS) != SUCCEEDED) {
      LGA_LOGGER_ERROR("[csuspend] Couldnt move the thread from bloq to Bloqueado Suspenso");
      return FAILED;
    }
    LGA_LOGGER_LOG("[csuspend] Moved the thread from bloq to Bloqueado Suspenso");
  }

  else if(tcb_suspend->state == PROCST_APTO_SUS || tcb_suspend->state == PROCST_BLOQ_SUS){
      LGA_LOGGER_WARNING("[csuspend] Cannot suspend an already suspended thread");
      return FAILED;
  }

  else if (tcb_suspend->state == PROCST_EXEC) {
    LGA_LOGGER_WARNING("[csuspend] Cannot resume a thread that is executing");
    return FAILED;
  }
  return SUCCEEDED;
}

/******************************************************************************
Par�metros:
	tid:	identificador da thread que ter� sua execu��o retomada.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cresume(int tid) {
  TCB_t *tcb_resume;

  LGA_LOGGER_IMPORTANT("[cresume] Begun");

  tcb_resume = (TCB_t *) LGA_find_element(tid);
  if (tcb_resume == NULL) {
    LGA_LOGGER_WARNING("[cresume] Theres none threads with this tid in Bloqueado Suspenso nor Apt Suspenso queues");
    return FAILED;
  }
  LGA_LOGGER_LOG("[cresume] Find the thread that will be resumed");

  if (tcb_resume->state == PROCST_APTO_SUS) {
    if (LGA_move_queues(tcb_resume->tid, &apt_sus, &apt, PROCST_APTO) != SUCCEEDED) {
      LGA_LOGGER_ERROR("[cresume] Couldnt move the thread from Apt Suspenso to apt");
      return FAILED;
    }
    LGA_LOGGER_LOG("[cresume] Moved the thread from Apt Suspenso to apt");
  }

  else if (tcb_resume->state == PROCST_BLOQ_SUS) {
    if (LGA_move_queues(tcb_resume->tid, &bloq_sus, &bloq, PROCST_BLOQ) != SUCCEEDED) {
      LGA_LOGGER_ERROR("[cresume] Couldnt move the thread from Bloqueado Suspenso to bloq");
      return FAILED;
    }
    LGA_LOGGER_LOG("[cresume] Moved the thread from Bloqueado Suspenso to bloq");
  }

  else if(tcb_resume->state == PROCST_APTO || tcb_resume->state == PROCST_BLOQ) {
      LGA_LOGGER_WARNING("[csuspend] Cannot resume a not suspended thread");
      return FAILED;
  }

  else if (tcb_resume->state == PROCST_EXEC) {
    LGA_LOGGER_WARNING("[csuspend] Cannot resume a thread that is executing");
    return FAILED;
  }

  return SUCCEEDED;
}

/******************************************************************************
Par�metros:
	sem:	ponteiro para uma vari�vel do tipo csem_t. Aponta para uma estrutura de dados que representa a vari�vel sem�foro.
	count: valor a ser usado na inicializa��o do sem�foro. Representa a quantidade de recursos controlados pelo sem�foro.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int csem_init(csem_t *sem, int count){
  LGA_LOGGER_IMPORTANT("[csem_init] begun");
  int result;
  sem->fila = (PFILA2 ) malloc(sizeof(PFILA2));
  result = CreateFila2(sem->fila);
  if (result == FAILED) {
      LGA_LOGGER_ERROR("[csem_init] Queue couldn't be created");
      return FAILED;
  }
  LGA_LOGGER_LOG("[csem_init] Created the csem_t Queue");
  sem->count = count;
  return SUCCEEDED;
}

/******************************************************************************
Par�metros:
	sem:	ponteiro para uma vari�vel do tipo sem�foro.
Retorno:
	Se correto => 0 (zero)
	Se erro	   => Valor negativo.
******************************************************************************/
int cwait(csem_t *sem){
  LGA_LOGGER_IMPORTANT("[cwait] begun");
  TCB_t *tcb_actual;
  int result;
  //P(S)
  sem->count -= 1;
  if(sem->count < 0){ //Caso não esteja disponivel a SC

    // Get tcb of current executing thread
    tcb_actual = (TCB_t *) LGA_get_first_queue(&exec);
    if(tcb_actual == NULL){ // Caso nao tenha conseguido recuperar tcb atual
      return FAILED;
    }
    LGA_LOGGER_DEBUG("[cwait] Got the element of exec");

    //Add tcb of actual executing thread to csem_t queue
    if(AppendFila2(sem->fila,(void*)tcb_actual->tid) == FAILED){
      LGA_LOGGER_ERROR("[cwait] Couldn't append executing tcb to csem_t queue");
    }
    LGA_LOGGER_IMPORTANT("[cwait] Critical Section locked, appended actual executing tcb to csem_t queues");

    //Sleep actual thread (block itself ) and change contexto to next thread
    LGA_block_exec_thread(tcb_actual);
  }

  return SUCCEEDED;
}

int csignal(csem_t *sem);

/*
  Initialize the used structures
  Return 0 - SUCCEEDED
  Return -1 - FAILED
  */
int LGA_init() {
  init = 0;
  final_context = (ucontext_t *) malloc(sizeof(ucontext_t));
  main_tcb = (TCB_t *) malloc(sizeof(TCB_t));

  LGA_LOGGER_IMPORTANT("[LGA_init] Begun");

  if(LGA_queues_init() != SUCCEEDED) {
    LGA_LOGGER_ERROR("The queues couldnt be created");
    return FAILED;
  }

  if (getcontext(&(main_tcb->context)) == SUCCEEDED) {
    main_tcb->tid = 0;
    main_tcb->state = PROCST_EXEC;
    main_tcb->context.uc_stack.ss_sp = (char*) malloc(163840);
    main_tcb->context.uc_stack.ss_size = 163840;
    main_tcb->context.uc_link = NULL;
    LGA_LOGGER_DEBUG("[LGA_init] Main Thread Created");
  } else {
    LGA_LOGGER_ERROR("[LGA_init] The Main thread couldnt be created");
    return FAILED;
  }
  if (AppendFila2(&exec, main_tcb) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[LGA_init] Main Thread inserted in exec");
  } else {
    LGA_LOGGER_ERROR("[LGA_init] The Main thread couldnt be inserted in the exec queue");
    return FAILED;
  }

  if (getcontext(final_context) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[LGA_init] Couldnt get the final context");
    return FAILED;
  }
  LGA_LOGGER_DEBUG("[LGA_init] Creating Final Context");

  final_context->uc_link = NULL;
  final_context->uc_stack.ss_sp = (char*) malloc(STACK_SIZE);
  final_context->uc_stack.ss_size = STACK_SIZE;
  makecontext(final_context, (void (*) (void)) CB_end_thread, 0);

  LGA_LOGGER_LOG("[LGA_init] All structures have been initialized");
  return SUCCEEDED;
}

/*
  Initialize the used queues
  Return 0 - SUCCEEDED
  Return -1 - FAILED
 */
int LGA_queues_init() {
  LGA_LOGGER_IMPORTANT("[LGA_queues_init] Begun");

  if(CreateFila2(&apt) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[LGA_queues_init] Created the Apt Queue");
  } else {
    LGA_LOGGER_ERROR("[LGA_queues_init] Apt Queue couldnt be created");
    return FAILED;
  }

  if(CreateFila2(&apt_sus) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[LGA_queues_init] Created the Apt Suspenso Queue");
  } else {
    LGA_LOGGER_ERROR("[LGA_queues_init] Apt Suspenso Queue couldnt be created");
    return FAILED;
  }

  if(CreateFila2(&exec) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[LGA_queues_init] Created the Exec Queue");
  } else {
    LGA_LOGGER_ERROR("[LGA_queues_init] Exec Queue couldnt be created");
    return FAILED;
  }

  if(CreateFila2(&bloq) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[LGA_queues_init] Created the Bloq Queue");
  } else {
    LGA_LOGGER_ERROR("[LGA_queues_init] Bloq Queue couldnt be created");
    return FAILED;
  }

  if(CreateFila2(&bloq_sus) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[LGA_queues_init] Created the Bloqueado Suspenso Queue");
  } else {
    LGA_LOGGER_ERROR("[LGA_queues_init] Bloqueado Suspenso Queue couldnt be created");
    return FAILED;
  }

  if(CreateFila2(&releasers) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[LGA_queues_init] Created the Releasers Queue");
  } else {
    LGA_LOGGER_ERROR("[LGA_queues_init] Releasers Queue couldnt be created");
    return FAILED;
  }

  LGA_LOGGER_LOG("[LGA_queues_init] All queues has been created");
  return SUCCEEDED;
}

/*
  When a context finalizes its function this function should be called
  because this function should be at context.uc_link
  This function move a thread from Apt to Exec and Free the context structure
 */
void* CB_end_thread(void *arg) {
  // Dispose the element from EXEC
  LGA_LOGGER_IMPORTANT("[CB_end_thread] Begun");

  if (LGA_dispose_exec_thread() != SUCCEEDED) {
    LGA_LOGGER_ERROR("[CB_end_thread] Couldnt dispose the element from exec");
    return END_CONTEXT;
  }
  LGA_LOGGER_LOG("[CB_end_thread] Ending a thread");
  // Get the next thread from APT and exec it
  LGA_next_thread();

  return END_CONTEXT;
}

/*
  Release the thread that has the given tid from Bloq Queue to Apt Queue
 */
void* CB_cjoin_release(void *block_releaser_in) {
  BLOCK_RELEASER *block_releaser = (BLOCK_RELEASER *) block_releaser_in;
  int tid_block, tid_releaser;
  TCB_t * tcb_block;

  LGA_LOGGER_IMPORTANT("[CB_cjoin_release] Begun");

  tid_block = block_releaser->tid_block;
  tid_releaser = block_releaser->tid_releaser;

  tcb_block = (TCB_t *) LGA_find_element(tid_block);

  if(tcb_block == NULL){
    LGA_LOGGER_ERROR("[CB_cjoin_release] Tid not found");
    return END_CONTEXT;
  }

  if(tcb_block->state == PROCST_BLOQ){
    LGA_LOGGER_IMPORTANT("[CB_cjoin_release] TCP found on blocked queue");
    if (LGA_move_queues(tid_block, &bloq, &apt, PROCST_APTO) != SUCCEEDED) {
      LGA_LOGGER_ERROR("[CB_cjoin_release] Couldnt move from bloq to apt");
      return END_CONTEXT;
    }
  } else if(tcb_block->state == PROCST_BLOQ_SUS){
    LGA_LOGGER_IMPORTANT("[CB_cjoin_release] TCP found on blocked_sus queue");
    if (LGA_move_queues(tid_block, &bloq_sus, &apt_sus, PROCST_APTO) != SUCCEEDED) {
      LGA_LOGGER_ERROR("[CB_cjoin_release] Couldnt move from bloq_sus to apt_sus");
      return END_CONTEXT;
    }
  } else {
    LGA_LOGGER_ERROR("[CB_cjoin_release] Couldnt find element in both block and block_sus");
    return END_CONTEXT;
  }
  LGA_LOGGER_DEBUG("[CB_cjoin_release] Releasing thread");

  if (LGA_tid_remove_from_fila(&releasers, tid_releaser) != SUCCEEDED){
    LGA_LOGGER_ERROR("[CB_cjoin_release] Couldnt remove from Releasers");
    return END_CONTEXT;
  }
  LGA_LOGGER_DEBUG("[CB_cjoin_release] Releaser removed from releasers queue");

  if(block_releaser_in != NULL) {
    free(block_releaser_in);
    LGA_LOGGER_DEBUG("[CB_cjoin_release] Freeing the block_releaser_in");
  }
  // Change the context to CB_end_thread, cuz when a cjoin triggers it means that
  // the actual thread is done doing its job, so we need to end it too
  LGA_LOGGER_LOG("[CB_cjoin_release] Updating the final context and changing to it");

  getcontext(final_context);
  makecontext(final_context, (void (*) (void)) CB_end_thread, 0);
  setcontext(final_context);

  return END_CONTEXT;
}

/*
  Move a Tcb_t that has the TID from removeQueue to insertQueue and change
  its state to new state
  Return 0 - SUCCEEDED
  Return -1 - FAILED
 */                                    //apt               exec
int LGA_move_queues(int tid, PFILA2 removeQueue, PFILA2 insertQueue, int state) {
  TCB_t *tcb = NULL;

  LGA_LOGGER_IMPORTANT("[LGA_move_queues] Begun");

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
    LGA_LOGGER_DEBUG("[LGA_move_queues] Inserted the element");
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
  LGA_LOGGER_IMPORTANT("[LGA_next_thread] Begun");

  // Get tcb of executing thread
  TCB_t *tcb;
  tcb = (TCB_t *) LGA_get_first_queue(&apt);
  if(tcb == NULL){ // Caso nao tenha conseguido recuperar tcb atual
    return;
  }

  // Moves next thread from apt queue to exec queue
  if(LGA_move_queues(tcb->tid, &apt, &exec, PROCST_EXEC) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[LGA_next_thread] Couldnt move the element from apt to exec");
    return;
  }

  LGA_LOGGER_LOG("[LGA_next_thread] Setting the thread in exec to execute");
  setcontext(&(tcb->context));
}

/*
  Moves the first element from Apt Queue to Exec queue swapping the context with
  given tcb
 */
 // ####
void LGA_next_thread_swap(TCB_t *tcb) {
  LGA_LOGGER_IMPORTANT("[LGA_next_thread_swap] Begun");

  TCB_t *tcb_resumed;
  tcb_resumed = (TCB_t *) LGA_get_first_queue(&apt);
  if(tcb_resumed == NULL){ // Caso nao tenha conseguido recuperar tcb atual
    return;
  }

  //Moves next thread from apt queue to execute queue
  if(LGA_move_queues(tcb_resumed->tid, &apt, &exec, PROCST_EXEC) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[LGA_next_thread] Couldnt move the element from apt to exec");
    return;
  }

  LGA_LOGGER_LOG("[LGA_next_thread] Setting the thread in exec to execute");
  swapcontext(&(tcb->context),&(tcb_resumed->context));
}

/*
  Move actual tcb being executed to block Queue
  Return SUCCEEDED if all goes succesfully
  Return FAILED if something went wrong
 */
int LGA_block_exec_thread(TCB_t *tcb_actual) {
  LGA_LOGGER_IMPORTANT("[LGA_block_exec_thread] Begun");
  //Move actual thread tcb to bloq queue
  if(LGA_move_queues(tcb_actual->tid, &exec, &bloq, PROCST_BLOQ) != SUCCEEDED) {
    LGA_LOGGER_ERROR("[LGA_block_exec_thread] Couldnt remove from exec and insert into bloq");
    return FAILED;
  }
  LGA_LOGGER_DEBUG("[LGA_block_exec_thread] Actual executing tcb moved to bloq queue");
  //Swap context to next thread
  LGA_next_thread_swap(tcb_actual);

  return SUCCEEDED;
}


/*
  Get the tcb being executed
  Return Valid Pointer - SUCCEEDED
  Return NULL - FAILED
 */
void* LGA_get_first_queue(FILA2 * queue) {
  LGA_LOGGER_IMPORTANT("[LGA_get_first_queue] Begun");

  TCB_t *tcb_disposed;
  if(FirstFila2(queue) == SUCCEEDED) {
    LGA_LOGGER_DEBUG("[LGA_get_first_queue] Queue is not empty");
  }
  else {
    LGA_LOGGER_WARNING("[LGA_get_first_queue] Queue is empty");
    return NULL;
  }
  tcb_disposed = (TCB_t *) GetAtIteratorFila2(queue);
  if(tcb_disposed == NULL) {
    LGA_LOGGER_WARNING("[LGA_get_first_queue] none tcb was found in exec queue");
  }
  return (void *) tcb_disposed;
}

/*
  Remove element pointed by iterator of EXEC
  Return 0 - SUCCEEDED
  Return -1 - FAILED
 */
 // ##### Suspeita dos free poderem dar leak por ter trocado do LGA_dispose_exec_thread
 //para cá o free
 int LGA_remove_exec(TCB_t *tcb){
   if (DeleteAtIteratorFila2(&exec) == SUCCEEDED) {
     free(tcb->context.uc_stack.ss_sp);
     free(tcb);
     LGA_LOGGER_LOG("[LGA_remove_exec] Disposed the first element from exec");
     return SUCCEEDED;
   } else {
     LGA_LOGGER_ERROR("[LGA_remove_exec] The element of exec couldnt be disposed");
     return FAILED;
   }
 }


/*
  Free the element of EXEC
  Return 0 - SUCCEEDED
  Return -1 - FAILED
 */
int LGA_dispose_exec_thread() {
  TCB_t *tcb_disposed;
  int result;
  tcb_disposed = (TCB_t *) LGA_get_first_queue(&exec);
  if(tcb_disposed == NULL){ // Caso nao tenha conseguido recuperar tcb atual
    return FAILED;
  }
  result = LGA_remove_exec(tcb_disposed);
  return result;
}

/*
  Get a tcb that has the given TID and is inside of Apt, AptSus, Bloq or BloqSus
  Return Valid Pointer - SUCCEEDED
  Return NULL - FAILED
 */
void* LGA_find_element(int tid) {

  LGA_LOGGER_IMPORTANT("[LGA_find_element] Begun");

  if(LGA_tid_inside_of_fila(&apt, tid) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_find_queue] Found the tid inside the Apt Queue");
    return (void *)LGA_tid_get_from_fila(&apt, tid);
  }

  if(LGA_tid_inside_of_fila(&bloq, tid) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_find_queue] Found the tid inside the Bloq Queue");
    return (void *)LGA_tid_get_from_fila(&bloq, tid);
  }
  if(LGA_tid_inside_of_fila(&apt_sus, tid) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_find_queue] Found the tid inside the Apt Queue");
    return (void *)LGA_tid_get_from_fila(&apt_sus, tid);
  }

  if(LGA_tid_inside_of_fila(&bloq_sus, tid) == SUCCEEDED) {
    LGA_LOGGER_LOG("[LGA_find_queue] Found the tid inside the Bloq Queue");
    return (void *)LGA_tid_get_from_fila(&bloq_sus, tid);
  }

  LGA_LOGGER_WARNING("[LGA_find_queue] The element isnt in the Apt nor Bloq queues");
  return NULL;
}
