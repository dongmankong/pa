#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

int main() {
  NDL_Init(0);
  int w, h;
  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  printf("%d   %d\n",w,h);
  assert(bmp);

  NDL_OpenCanvas(&w, &h);
  int wScreen=0,hScreen=0;
  NDL_OpenCanvas(&wScreen, &hScreen);
  printf("%d   %d\n",wScreen,hScreen);
  NDL_DrawRect(bmp, (wScreen-w)/2, (hScreen-h)/2, w, h);
  free(bmp);
  NDL_Quit();
  printf("Test ends! Spinning...\n");
  while (1);
  return 0;
}
