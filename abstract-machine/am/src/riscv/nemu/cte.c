#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

//my 当做硬件操作 user_handler才是具体的软件操作
Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
//my
      case -1:
      // case 0xb:
        ev.event =EVENT_YIELD;
        break;
        //
      case 0x0: //SYS_exit
      case 0x1: //SYS_yield
      case 0x2: //SYS_open
      case 0x3: //SYS_read
      case 0x4: //SYS_write
      case 0x7: //SYS_close
      case 0x8: //SYS_lseek
      case 0x9: //SYS_brk
      case 19: //SYS_gettimeofday
        ev.event =EVENT_SYSCALL;
        break;
      // case 0x0:
      //   ev.event =EVENT_NULL;
      //   break;
//
      default: 
        ev.event = EVENT_ERROR; 
        break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  // int a=0,b;
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));
  
  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}
void yield() {
  asm volatile("li a7, -1; ecall");
  // asm volatile("li a7, 10; ecall");


//my
  // isa_raise_intr(EVENT_YIELD,__am_asm_trap);
//
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
