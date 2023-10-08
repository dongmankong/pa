#include <common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
//my    
    case EVENT_YIELD:
      Log("%s\n","进入自陷异常");
      c->mepc+=4;
      break;
    //
    case EVENT_SYSCALL:
      Log("EVENT_SYSCALL\n");
      // c->mepc+=4;
      break;  
//
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
