#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
// #include <time.h>
// int main(void) {
//   struct timeval tv;
//   // struct timezone tz;
//   gettimeofday(&tv, NULL);
//   int ms = 5000;
//   // printf("%d\n",(tv.tv_sec * 1000 + tv.tv_usec / 1000));
//   while (1) {
//     // printf("%d\n",(tv.tv_sec * 1000 + tv.tv_usec / 1000));
//     while ((tv.tv_sec * 1000 + tv.tv_usec / 1000) < ms) {
//     // printf("过了5s\n");

//       gettimeofday(&tv, NULL);
//     }
//     ms += 5000;
//     // printf("ms = %d\n", ms);

//     printf("过了5s\n");
//   }
//   // printf("ms = %d\n", ms);
//   return 0;
// }
uint32_t NDL_GetTicks();
int NDL_Init(uint32_t flags);
int main(void) {
  NDL_Init(0);
  while (1) {
    NDL_GetTicks();
  }
  return 0;
}