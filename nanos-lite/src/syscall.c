#include <common.h>
#include "syscall.h"
//my  a[0] type; a[1] a0;a[2] a1;a[3] a2
//
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
//my
  // c->GPRx=0;
  a[1]=c->GPR2;
  a[2]=c->GPR3;
  a[3]=c->GPR4;
  // Log("a[0]: %x,a[1]:%x ,a[2]:%x,a[3]:%x   \n",a[0],a[1],a[2],a[3]);
//
  switch (a[0]) {
//my
    case SYS_yield:
      c->GPRx=0;
      // yield();
      break;
    case SYS_exit:
      halt(a[1]);
      // c->GPRx=0;
      // yield();
      break;
    //
    case SYS_write:  //_syscall_(SYS_write, fd, buf, count);
      // Log("a[0]: %x,a[1]:%x ,a[2]:%x,a[3]:%x   \n",a[0],a[1],a[2],a[3]);
      assert(a[1]==1 || a[1]==2);
      char *p=(char *)a[2];
      size_t count=a[3];
      // int count=13;
      for(int i=0;i<count;++i){
        putch(p[i]);
        // Log("%c\n",p[i]);
        // p++;
      }
      c->GPRx=count;
      break;
    case SYS_brk: //_syscall_(SYS_brk, (intptr_t)program_break, increment, 0)==0
      // a[1]=(uint32_t)(char*)a[1]+(int)a[2];
      c->GPRx=0;
      break;

    // case SYS_open:

//
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
