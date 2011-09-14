#include "record.h"
#include "proc.h"

int startrecording();
int stoprecording();
int fetchrecords(struct record *records, int num_records);


int addrecordtolist(reclist list, struct record rec){
  reclist aux;

  if( list == NULL){
    if( (list = kalloc()) == 0)
       return 0;
    list -> rec = rec;
    list -> next = NULL;
   }
  for( aux = list; aux -> next != NULL; aux =  aux -> next);

  if( (aux -> next = kalloc()) == 0 )
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
      if( (aux = kalloc()) == 0 )
        return NULL;
      aux -> rec = list -> rec;
      aux -> next = NULL;
      ant = aux;
      list = list -> next;
      new = aux;
      aux = NULL;
  } 
  while( list != NULL ){
      if( (aux = kalloc()) == 0 )
        return NULL;
      aux -> rec = list -> rec;
      ant -> next = aux;
      ant = aux;
      aux = NULL;
      list = list -> next;
  }
  return new;
}

void printrecord(struct record rec){
  
  switch(rec.type){
    case SYSCALL_NO:
        cprintf("SYSCALL_NO: %s\n",translatesyscall(rec.value));
        break;
    case ARG_INTEGER:
        cprintf("ARG_INTEGER: %d\n",rec.value):
        break;
    case ARG_POINTER:
        cprintf("ARG_POINTER: %p\n",rec.value):
        break;
    case ARG_STRING:
        cprintf("ARG_STRING: %s\n",rec.value):
        break;
    case RET_VALUE:
        cprintf("RET_VALUE: %d\n",rec.value):
        break;
    }
}

char* translatesyscall(int sc){

  switch(sc)
    case 1:
      return "SYS_fork";
    case 2:
      return 
    case 3:
      return 
    case 4:
      return 
    case 5:
      return 
    case 6:
      return 
    case 7:
      return 
    case 8:
      return 
    case 9:
      return 
    case 10:
      return 
    case 11:
      return 
    case 12:
      return 
    case 13:
      return 
    case 14:
      return 
    case 15:
      return 
    case 16:
      return 
    case 17:
      return 
    case 18:
      return 
    case 19:
      return 
    case 20:
      return 
    case 21:
      return 
    case 22:
      return 
    case 23:
      return 
    case 24:
      return 
 

#define             1
#define SYS_exit            2
#define SYS_wait            3
#define SYS_pipe            4
#define SYS_write           5
#define SYS_read            6
#define SYS_close           7
#define SYS_kill            8
#define SYS_exec            9
#define SYS_open           10
#define SYS_mknod          11
#define SYS_unlink         12
#define SYS_fstat          13
#define SYS_link           14
#define SYS_mkdir          15
#define SYS_chdir          16
#define SYS_dup            17
#define SYS_getpid         18
#define SYS_sbrk           19
#define SYS_sleep          20
#define SYS_uptime         21
#define SYS_startrecording 22
#define SYS_stoprecording  23
#define SYS_fetchrecords   24





int fetchrecordslist(reclist list);
int releaserecordslist(reclist list);


