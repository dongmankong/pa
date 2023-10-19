#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//my
#include <sys/time.h>

// 
static int evtdev = 3;
static int fbdev = 4;
static int screen_w = 0, screen_h = 0;

//my
struct timeval tv;
int startTime;
// 以毫秒为单位返回
uint32_t NDL_GetTicks() {
//my
  // int ms = startTime+1;
  // while (1) {
    // while ((tv.tv_sec * 1000 + tv.tv_usec / 1000) < ms) {
      gettimeofday(&tv, NULL);
    // }
    // printf("当前时间为%d ms\n",tv.tv_sec * 1000 + tv.tv_usec/1000-startTime);
  //   ms += 1;
  // }
  return tv.tv_sec * 1000 + tv.tv_usec/1000-startTime;
  // while (1) {
  //   gettimeofday(&tv, NULL);

  //   printf("当前时间为%d s\n",tv.tv_sec-startTime);
  // }
// 
  // return 0;
}

int NDL_PollEvent(char *buf, int len) {
  return read(evtdev, buf, len);
  // return 0;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
  // FILE *fp = fopen("/proc/dispinfo", "r");
  void *buf[2];
  read(5,buf,2);
  *w=(int)buf[0];
  *h=(int)buf[1];

  // printf("%d  %d",*w,*h);
  
}
void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
//my
  uint32_t *color_buf = malloc(w*sizeof(uint32_t));
  // fseek(fbdev, 0, SEEK_SET);
  printf("sssssssssssssssssssssss\n");
  for(int i=0;i<h;++i){
    for(int j=0;j<w;++j){
      color_buf[j]=pixels[i*w+j];
    }
    write(fbdev,(void *)color_buf,w);
  }
//
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
//my
  gettimeofday(&tv, NULL);
  startTime=tv.tv_sec*1000+tv.tv_usec/1000; //ms
  // printf("开始毫秒为%d\n",startTime);
// 
  return 0;
}

void NDL_Quit() {
}
