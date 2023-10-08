#include <common.h>
#include "syscall.h"

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
//my
  c->GPRx=0;
  // a[2]=c->GPR3;
  // a[3]=c->GPR4;
  Log("%x\n",a[0]);
//
  switch (a[0]) {
//my
    case SYS_yield:
      yield();
      break;
//
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
