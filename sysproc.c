#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

extern int curPolicy;                  // shows that current scheduling policy (Added)
extern int multilayer;
extern int q;

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{ 
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{ 
 int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{ 
  return myproc()->pid;
}

int
sys_sbrk(void)
{ 
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{ 
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}



// set or modify priority of current process (Added)
int
sys_setPriority(void)
{
  int priority;

  if(argint(0, &priority) < 0)
    return -1;

  return setPriority(priority);
}

// return priority of current process (Added)
int
sys_getPriority(void)
{
  return getPriority();
}

// change scheduling policy (Added)
int
sys_changePolicy(void)
{
  int policy;
  
  if(argint(0, &policy) < 0)
    return -1;
  curPolicy = policy; 
  return 1;
}

// return scheduling policy (Added)
int
sys_getPolicy(void)
{
  return curPolicy;
}

// this wait syscall returns the related times for specifed process (Added)
int 
sys_wait2(void)
{
  Times *times;
  if(argptr(0, (void*)&times, sizeof(*times)) < 0)
    return -1;
  return wait2(times);
}

// this function add current process to given queue (Added)
int
sys_fork2(void)
{
  int queue;
  if(argint(0, &queue) < 0)
    return -1;
  return fork2(queue);
}