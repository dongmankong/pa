#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

//定义的是具体的文件写操作（对于串口来说 也就是stdout标准流这一类
size_t serial_write(const void *buf, size_t offset, size_t len) {
//my
    char *p=(char *)buf;
    for(int i=0;i<len;++i){
      putch(p[i]);
      // Log("%c\n",p[i]);
      // p++;
    }
    return len;
//
  // return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
//my
  printf("events_read\n");

  bool keydown=io_read(AM_INPUT_KEYBRD).keydown;
  int keycode=io_read(AM_INPUT_KEYBRD).keycode;
  printf("%d\n", keycode);

  if(keycode==AM_KEY_NONE){
    *(char*)buf = '\0';
    return 0;
  }
  int lenMy=0;
  if(keydown){
    lenMy=sprintf(buf,"kd %s\n",keyname[keycode]);
  }else{
    lenMy=sprintf(buf,"ku %s\n",keyname[keycode]);
  }
  printf("%s\n", buf);
  return lenMy;
//
  // return 0;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
