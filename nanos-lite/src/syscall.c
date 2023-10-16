#include <common.h>
#include <sys/time.h>
// #include <time.h>
#include "syscall.h"
//my  a[0] type; a[1] a0;a[2] a1;a[3] a2
//my
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
//
//
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
//my
  // c->GPRx=0;
  a[1]=c->GPR2;
  a[2]=c->GPR3;
  a[3]=c->GPR4;
  // Log("异常类型为: %x  ,a[1]:%x ,a[2]:%x,a[3]:%x   \n",a[0],a[1],a[2],a[3]);
//
  // int x;
  Log("\033[34m异常类型为: %x\033[34m   ,a[1]:%x ,a[2]:%x,a[3]:%x   \n",a[0],a[1],a[2],a[3]);
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
      // Log("write\n");
      // Log("a[0]: %x,a[1]:%x ,a[2]:%x,a[3]:%x   \n",a[0],a[1],a[2],a[3]);
      // assert(a[1]==1 || a[1]==2);
      //////////////////////////////////////////////////////////
      // if(a[1]==1 || a[1]==2){ //stdout 标准流
      //   char *p=(char *)a[2];
      //   size_t count=a[3];
      //   // int count=13;
      //   for(int i=0;i<count;++i){
      //     putch(p[i]);
      //     // Log("%c\n",p[i]);
      //     // p++;
      //   }
      //   c->GPRx=count;
      // }else{
      //   c->GPRx=fs_write(a[1], (void *)a[2], a[3]);
      // }
      //////////////////////////////////////////////////////////
      c->GPRx=fs_write(a[1], (void *)a[2], a[3]);

      break;
    case SYS_brk: //_syscall_(SYS_brk, (intptr_t)program_break, increment, 0)==0
      // a[1]=(uint32_t)(char*)a[1]+(int)a[2];
      c->GPRx=0;
      break;

    case SYS_open: //_syscall_(SYS_open, path, flags, mode);
      // x=fs_open((char *)a[1], a[2], a[3]);
      // c->GPRx=x;
      c->GPRx=fs_open((char *)a[1], a[2], a[3]);
      // Log("open over    x:%d\n",x);
      break;
    case SYS_read: //_read(int fd, void *buf, size_t count)
      c->GPRx=fs_read(a[1], (void *)a[2], a[3]);
      break;
    case SYS_close: //_syscall_(SYS_close, fd, 0, 0)
      c->GPRx=fs_close(a[1]);
      break;
    case SYS_lseek: //_lseek(int fd, off_t offset, int whence)
      c->GPRx=fs_lseek(a[1], a[2], a[3]);
      break;

    case SYS_gettimeofday:{ //_syscall_(SYS_gettimeofday, (intptr_t)&tv, (intptr_t)&tz, 0)
      struct timeval* tv = (struct timeval*)a[1];
      uint64_t now= io_read(AM_TIMER_UPTIME).us;
      Log("%d\n",now);
      if(tv!=NULL){
        tv->tv_sec=now/(1000*1000);
        Log("%d\n",tv->tv_sec);
        tv->tv_usec=now%(1000*1000);
      }

      c->GPRx=0;
      break;
    }
//
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
