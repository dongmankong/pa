#include <proc.h>
#include <elf.h>
// //my
// #include "ramdisk.h"
// //


#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
size_t ramdisk_read(void *buf, size_t offset, size_t len);

size_t ramdisk_write(const void *buf, size_t offset, size_t len);
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
//my
  // Elf_Ehdr ehdr;
  // ramdisk_read(&ehdr,0,sizeof(Elf_Ehdr));
  // // Log("%x",*(uint32_t *)ehdr.e_ident);
  // assert(*(uint32_t *)ehdr.e_ident == 0x464c457f);
  // assert(ehdr.e_machine==EM_RISCV);
  
  // Elf_Phdr phdrs[ehdr.e_phnum];
  // ramdisk_read(phdrs,ehdr.e_phoff,sizeof(Elf_Phdr)*ehdr.e_phnum);
  // for(int i=0;i<ehdr.e_phnum;++i){
  //   if(phdrs[i].p_type==PT_LOAD){
  //     ramdisk_read((void *)phdrs[i].p_vaddr,phdrs[i].p_offset,phdrs[i].p_memsz);
  //     memset((void*)(phdrs[i].p_vaddr+phdrs[i].p_filesz),0,phdrs[i].p_memsz-phdrs[i].p_filesz);
  //   }
  // }

  // return ehdr.e_entry;

//////////////////////////////////////////////////////////////////////////////////////////////
    Elf_Ehdr ehdr;
    int fd=fs_open(filename, 0, 0);
    fs_read(fd, &ehdr, sizeof(Elf_Ehdr));
    assert(*(uint32_t *)ehdr.e_ident == 0x464c457f);
    assert(ehdr.e_machine==EM_RISCV);
    Log("%x\n",ehdr.e_phoff);
    Elf_Phdr phdrs[ehdr.e_phnum];
    ramdisk_read(phdrs,ehdr.e_phoff+0x400143,sizeof(Elf_Phdr)*ehdr.e_phnum);
    for(int i=0;i<ehdr.e_phnum;++i){
      if(phdrs[i].p_type==PT_LOAD){
        ramdisk_read((void *)phdrs[i].p_vaddr,phdrs[i].p_offset+0x400143,phdrs[i].p_memsz);
        memset((void*)(phdrs[i].p_vaddr+phdrs[i].p_filesz)+0x400143,0,phdrs[i].p_memsz-phdrs[i].p_filesz);
      }
    }
    return ehdr.e_entry+0x400143;
/////////////////////////////////////////////////////////

//
  // return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  Log("进入loader\n");
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

