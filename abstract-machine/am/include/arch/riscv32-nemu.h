#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  // TODO: fix the order of these members to match trap.S
  // uintptr_t mepc, mcause, gpr[32], mstatus;
  // void *pdir;

//my
  uintptr_t gpr[32];
  uintptr_t mcause;
  uintptr_t mstatus;
  uintptr_t mepc;
  void *pdir;
//
};

#define GPR1 gpr[17] // a7
//my  "a0", "a1", "a2", "a0"
#define GPR2 gpr[10]
#define GPR3 gpr[11]
#define GPR4 gpr[12]
#define GPRx gpr[10]
// 
#endif
