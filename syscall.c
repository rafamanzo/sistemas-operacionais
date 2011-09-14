#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "record.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"

// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from process p.
int
fetchint(struct proc *p, uint addr, int *ip)
{  
  if(addr >= p->sz || addr+4 > p->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from process p.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int
fetchstr(struct proc *p, uint addr, char **pp)
{
  char *s, *ep;

  if(addr >= p->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)p->sz;
  for(s = *pp; s < ep; s++)
    if(*s == 0)
      return s - *pp;
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  int ret;
  struct record rec;
  
  ret = fetchint(proc, proc->tf->esp + 4 + 4*n, ip);
  rec.type = ARG_INTEGER;
  rec.value.intval = *ip;
  addrecordtolist(proc->recl, rec);
  
  return ret;
}

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size n bytes.  Check that the pointer
// lies within the process address space.
int
argptr(int n, char **pp, int size)
{
  int i;
  struct record rec;
  
  if(argint(n, &i) < 0)
    return -1;
  if((uint)i >= proc->sz || (uint)i+size > proc->sz)
    return -1;
    
  *pp = (char*)i;
  
  rec.type = ARG_PTR;
  rec.value.ptrval = *pp;
  addrecordtolist(proc->recl, rec);
  
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  int addr;
  int ret;
  int i;
  struct record rec;
  
  if(argint(n, &addr) < 0)
    return -1;
    
  ret = fetchstr(proc, addr, pp);
  
  rec.type = ARG_STRING;
  
  for(i = 0; i < 20; i++){
    if(i < ret){
      rec.value.strval[i] = *pp[i];
    }else{
      rec.value.strval[i] = ' ';
    }
  }
  addrecordtolist(proc->recl, rec);
  
  return ret;
}

extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_startrecording(void);
extern int sys_stoprecording(void);
extern int sys_fetchrecords(void);

static int (*syscalls[])(void) = {
[SYS_chdir]           sys_chdir,
[SYS_close]           sys_close,
[SYS_dup]             sys_dup,
[SYS_exec]            sys_exec,
[SYS_exit]            sys_exit,
[SYS_fork]            sys_fork,
[SYS_fstat]           sys_fstat,
[SYS_getpid]          sys_getpid,
[SYS_kill]            sys_kill,
[SYS_link]            sys_link,
[SYS_mkdir]           sys_mkdir,
[SYS_mknod]           sys_mknod,
[SYS_open]            sys_open,
[SYS_pipe]            sys_pipe,
[SYS_read]            sys_read,
[SYS_sbrk]            sys_sbrk,
[SYS_sleep]           sys_sleep,
[SYS_unlink]          sys_unlink,
[SYS_wait]            sys_wait,
[SYS_write]           sys_write,
[SYS_uptime]          sys_uptime,
[SYS_startrecording]  sys_startrecording,
[SYS_stoprecording]   sys_stoprecording,
[SYS_fetchrecords]    sys_fetchrecords,
};

void
syscall(void)
{
  int num;
  int ret;
  struct record rec_no, rec_ret;
  
  num = proc->tf->eax;
  
  rec_no.type = SYSCALL_NO;
  rec_no.value.intval = num;
  addrecordtolist(proc->recl, rec_no);
  
  if(num >= 0 && num < NELEM(syscalls) && syscalls[num])
    ret = syscalls[num]();
    proc->tf->eax = ret;
  else {
    cprintf("%d %s: unknown sys call %d\n",
            proc->pid, proc->name, num);
    proc->tf->eax = -1;
  }
  
  rec_ret.type = RET_VALUE;
  rec_ret.value.intval = ret;
  addrecordtolist(proc->recl, rec_ret);
}
