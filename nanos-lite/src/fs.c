#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t start_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

//my
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

//


size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}
//my
/* This is the information about all files in disk. */
// static Finfo file_table[] __attribute__((used)) = {
//   [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
//   [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
//   [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
// #include "files.h"
// };

static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, 0,invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, 0,invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, 0,invalid_read, invalid_write},
#include "files.h"
};

//my
int fs_open(const char *pathname, int flags, int mode){
  Log("%s\n",pathname);
  for(int i=0;i<sizeof(file_table)/sizeof(Finfo);++i){
      if(strcmp(file_table[i].name,pathname)==0){
        Log("文件下标为%d   开始位置为%d\n   文件大小为%d",i,file_table[i].disk_offset,file_table[i].size);
        file_table[i].start_offset=file_table[i].disk_offset;
        return i;  //文件记录表的下标作为相应文件的文件描述符返回给用户程序
      }
  }
  assert(0);
}

size_t ramdisk_read(void *buf, size_t offset, size_t len);

size_t ramdisk_write(const void *buf, size_t offset, size_t len);

size_t fs_read(int fd, void *buf, size_t len){
  // Log("进入read\n");
  // if(len==0) return 0;
  // size_t shengyu=file_table[fd].size-(file_table[fd].disk_offset-file_table[fd].start_offset);
  // // Log("%d\n",len);
  // if(shengyu<len){
  //   Log("文件越界\n");
  //   assert(0);
  // }
  ramdisk_read(buf,file_table[fd].disk_offset,len);
  file_table[fd].disk_offset+=len;
  // Log("%x",*(uint32_t *)ehdr.e_ident);
  // Log("离开read\n");

  return len;
}

size_t fs_write(int fd, const void *buf, size_t len){
  // Log("write\n");
  if(len==0) return 0;
  // size_t shengyu=file_table[fd].size-(file_table[fd].disk_offset-file_table[fd].start_offset);
  // // Log("%d\n",len);
  // if(shengyu<len){
  //   Log("文件越界\n");
  //   assert(0);
  // }
  ramdisk_write(buf,file_table[fd].disk_offset,len);
  file_table[fd].disk_offset+=len;
  // Log("%x",*(uint32_t *)ehdr.e_ident);
  // Log("离开write\n");
  return len;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  //enum {SEEK_SET, SEEK_CUR, SEEK_END};
  // Log("%d      %d\n",offset,whence);
  switch (whence)
  {
  case SEEK_SET:
    file_table[fd].disk_offset=file_table[fd].start_offset+offset;
    // Log("seek : %x\n",file_table[fd].disk_offset);
    return file_table[fd].disk_offset;
    break;
  case SEEK_CUR:
    file_table[fd].disk_offset=file_table[fd].disk_offset+offset;
    // Log("seek : %x\n",file_table[fd].disk_offset);
    return file_table[fd].disk_offset;
    break;
  case SEEK_END:
    file_table[fd].disk_offset=file_table[fd].start_offset+file_table[fd].size+offset;
    // Log("seek : %x\n",file_table[fd].disk_offset);
    return file_table[fd].disk_offset;
    break;
  default:
    assert(0);
    break;
  }
  return -1;
}
int fs_close(int fd){
  return 0;
}
//
void init_fs() {
  // TODO: initialize the size of /dev/fb
}
