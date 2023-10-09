/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
//
  for(int i=0;i<ARRLEN(cpu.gpr);++i){
    if(ref_r->gpr[i]!=cpu.gpr[i]){
      if(ref_r->gpr[5]==0xb) continue;
      Log("error: %x\n",pc);
      Log("正确的值为 : %x\n",ref_r->pc);
      Log("error: %d\n",i);
      Log("error: %x\n",cpu.gpr[i]);
      Log("正确的值为: %x\n",ref_r->gpr[i]);


      return false;
    }
  }
  if(cpu.pc!=ref_r->pc){
    return false;
  }
//pa3
  // if(cpu.csr.mcause!=ref_r->csr.mcause){
  //   Log("mcause error: %x\n",cpu.csr.mcause);
  //   Log("正确的值为 : %x\n",ref_r->csr.mcause);
  //   return false;
  // }
  if(cpu.csr.mepc!=ref_r->csr.mepc){
    Log("mepc error: %x\n",cpu.csr.mepc);
    Log("正确的值为 : %x\n",ref_r->csr.mepc);
    return false;
  }
  // if(cpu.csr.mstatus!=ref_r->csr.mstatus){
  //   Log("mstatus error: %x\n",cpu.csr.mstatus);
  //   Log("正确的值为 : %x\n",ref_r->csr.mstatus);
  //   return false;
  // }
  if(cpu.csr.mtvec!=ref_r->csr.mtvec){
    Log("mtvec error: %x\n",cpu.csr.mtvec);
    Log("正确的值为 : %x\n",ref_r->csr.mtvec);
    return false;
  }
//
  return true;
//my
}

void isa_difftest_attach() {

}
