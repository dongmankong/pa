#include <proc.h>
#include <elf.h>
//my
#include "ramdisk.h"
//
#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
//my
  Elf_Ehdr ehdr;
  ramdisk_read(&ehdr,0,sizeof(Elf_Ehdr));
  // Log("%x",*(uint32_t *)ehdr.e_ident);
  assert(*(uint32_t *)ehdr.e_ident == 0x464c457f);
  assert(ehdr.e_machine==EM_RISCV);
  
  Elf_Phdr phdrs[ehdr.e_phnum];
  Log("%d\n",ehdr.e_phentsize);
  
  Log("%d\n",sizeof(Elf_Phdr)*ehdr.e_phnum);

  ramdisk_read(phdrs,ehdr.e_phoff,ehdr.e_phentsize);
  for(int i=0;i<ehdr.e_phnum;++i){
    if(phdrs[i].p_type==PT_LOAD){
      ramdisk_write((void *)phdrs[i].p_vaddr,phdrs[i].p_offset,phdrs[i].p_memsz);
      memset((void*)(phdrs[i].p_vaddr+phdrs[i].p_filesz),0,phdrs[i].p_memsz-phdrs[i].p_filesz);
    }
  }

  return ehdr.e_entry;
//
  // return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

