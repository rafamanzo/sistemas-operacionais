#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "record.h"
#include "proc.h"

#define NULL 0

int startrecording(){
  if( proc->recording == 0 ){
    proc->recording = 1;
    return 0;
  }
  return -1;
}

int stoprecording(){
  if( proc->recording == 1 ){
    proc->recording = 0;
    return 0;
  }
  return -1;
}

/* Tranforma o numero da System Call no respectivo nome */
char* translatesyscall(int sc){
  switch(sc){
    case 1:
      return "SYS_fork";
    case 2:
      return "SYS_exit";
    case 3:
      return "SYS_wait";
    case 4:
      return "SYS_pipe";
    case 5:
      return "SYS_write";
    case 6:
      return "SYS_read";
    case 7:
      return "SYS_close";
    case 8:
      return "SYS_kill";
    case 9:
      return "SYS_exec";
    case 10:
      return "SYS_open";
    case 11:
      return "SYS_mknod";
    case 12:
      return "SYS_unlink";
    case 13:
      return "SYS_fstat";
    case 14:
      return "SYS_link";
    case 15:
      return "SYS_mkdir";
    case 16:
      return "SYS_chdir";
    case 17:
      return "SYS_dup";
    case 18:
      return "SYS_getpid";
    case 19:
      return "SYS_sbrk";
    case 20:
      return "SYS_sleep";
    case 21:
      return "SYS_uptime";
    case 22:
      return "SYS_startrecording";
    case 23:
      return "SYS_stoprecording";
    case 24:
      return "SYS_fetchrecords";
    default:
      return "Doesn't exist";
  }
}

void printrecord(struct record rec){  
  switch(rec.type){
    case SYSCALL_NO:
        cprintf("SYSCALL_NO: %s\n",translatesyscall(rec.value.intval));
        break;
    case ARG_INTEGER:
        cprintf("ARG_INTEGER: %d\n",rec.value.intval);
        break;
    case ARG_POINTER:
        cprintf("ARG_POINTER: %p\n",rec.value.ptrval);
        break;
    case ARG_STRING:
        cprintf("ARG_STRING: %s\n",rec.value.strval);
        break;
    case RET_VALUE:
        cprintf("RET_VALUE: %d\n",rec.value.intval);
        break;
    }
}

int fetchrecords(struct record *records, int num_records){
  int i;
   
  if( records == NULL)
    return fetchrecordslist(proc->recl);
  else{
    for( i = 0; (i < num_records); i++)
      printrecord(records[i]);
    return i;
  }
}

int addrecordtolist(reclist *list, struct record rec){
  reclist aux;

  if( *list == NULL){
    if( (*list = (reclist) kalloc()) == 0)
       return 0;
    (*list)->rec = rec;
    (*list)->next = NULL;
   }
  for( aux = *list; aux -> next != NULL; aux =  aux -> next);

  if( (aux -> next = (reclist) kalloc()) == 0 )
    return 0;
  aux = aux -> next;
  aux -> rec = rec;
  aux -> next = NULL;  

  return 1;
}

reclist copyrecordslist(reclist list){
  reclist new, aux, ant;
  new = aux = ant = NULL;

  /* Como a lista não possui cabeça tratamos o primeiro caso separadamente */
  if(list != NULL){
      if( (aux = (reclist) kalloc()) == 0 )
        return NULL;
      aux -> rec = list -> rec;
      aux -> next = NULL;
      ant = aux;
      list = list -> next;
      new = aux;
      aux = NULL;
  } 
  while( list != NULL ){
      if( (aux = (reclist) kalloc()) == 0 )
        return NULL;
      aux -> rec = list -> rec;
      ant -> next = aux;
      ant = aux;
      aux = NULL;
      list = list -> next;
  }
  return new;
}

int fetchrecordslist(reclist list){
  reclist runner;
  int cnt;  

  for(cnt = 0, runner = list; runner != NULL; runner = runner->next, cnt++)
    printrecord(runner->rec);
    
  return cnt;
}

int releaserecordslist(reclist list){
  reclist runner, next;
  
  runner = list; 
  
  while(runner != NULL){
    next = runner->next;
    kfree( (char *) runner );
    runner = next; 
  }
  
  return 1;
  
  return 1;
}


