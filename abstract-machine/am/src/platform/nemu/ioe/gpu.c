#include <am.h>
#include <nemu.h>
#include <klib.h>
#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
//my
  // uint32_t i;
  // uint32_t w = inw(VGACTL_ADDR);  // TODO: get the correct width
  // uint32_t h = inw(VGACTL_ADDR+2);  // TODO: get the correct height
  // // int i;
  // // int w = 0;  // TODO: get the correct width
  // // int h = 0;  // TODO: get the correct height
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);
//
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
//my
  uint32_t screen_size = inl(VGACTL_ADDR);
  uint32_t w = screen_size>>16;  
  uint32_t h = screen_size & 0xffff;  
//
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  // if (ctl->sync) {
  //   outl(SYNC_ADDR, 1);
  // }
//my
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  if (!ctl->sync && (w == 0 || h == 0)) return;
  // if(ctl->sync==false){
    // printf("%d  %d\n",x,y);
    // if (w == 0 || h == 0) return;
    // int block_size = w * h;
  uint32_t screen_w = (inl(VGACTL_ADDR))>>16;  
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  fb+=screen_w*y+x;
  uint32_t *p=ctl->pixels;
  for(int i=0;i<h;++i){
    uint32_t *fb_first=fb;
    for(int j=0;j<w;++j){
      *fb_first=*p;
      p++;
      fb_first++;
    }
    fb+=screen_w;
  }
  // }  
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
//
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
