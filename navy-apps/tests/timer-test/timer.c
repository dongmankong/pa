#include <stdio.h>
#include <sys/time.h>
// #include <time.h>
int main(void) {
  struct timeval tv;
  // struct timezone tz;
  gettimeofday(&tv, NULL);
  __uint64_t ms = 5000;
  while (1) {
    while ((tv.tv_sec * 1000 + tv.tv_usec / 1000) < ms) {
      gettimeofday(&tv, NULL);
    }
    ms += 5000;
    printf("过了5s\n");
  }
  // printf("ms = %d\n", ms);
  return 0;
}
