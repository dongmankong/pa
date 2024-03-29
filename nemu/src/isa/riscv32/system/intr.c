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
//my NO对应异常种类，epc对应触发异常的指令地址，最后返回异常入口地址
word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
//my
  // Log("异常类型： %u\n",NO);
  cpu.csr.mcause=NO;
  cpu.csr.mepc=epc;
  // if(NO==0xb){
  //   cpu.csr.mepc=epc+4;
  // }else{
  //   cpu.csr.mepc=epc;
  // }

  // Log("%x\n",cpu.csr.mtvec);

  return cpu.csr.mtvec;
//
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
