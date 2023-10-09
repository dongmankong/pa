#include <common.h>
#include "syscall.h"

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
//my
  // c->GPRx=0;
  a[1]=c->GPR2;
  a[2]=c->GPR3;
  a[3]=c->GPR4;
  Log("a[0]: %x,a[1]:%x ,a[2]:%x,a[3]:%x   \n",a[0],a[1],a[2],a[3]);
//
  switch (a[0]) {
//my
    case SYS_yield:
      c->GPRx=0;
      // yield();
      break;
    case SYS_exit:
      halt(a[0]);
      // c->GPRx=0;
      // yield();
      break;
//
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
