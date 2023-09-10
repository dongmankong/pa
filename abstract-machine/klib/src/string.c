#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if(s==NULL) return 0;
  size_t num=0;
  while(s[num]!='\0'){
    num++;
  }
  return num;
  // panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  assert(dst != NULL);
	assert(src != NULL);

  char* target=dst;
  do{
    *target=*src;
    src++;
    target++;
  }while(*src!='\0');
  return dst;
  // panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  assert(dst != NULL);
	assert(src != NULL);
  char *res=dst;
  while(n && *src!='\0'){
    *dst=*src;
    dst++;
    src++;
    n--;
  }
  while(n){
    *dst='\0';
    dst++;
    n--;
  }
  return res;
  // panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  assert(dst != NULL);
	assert(src != NULL);

  char *res=dst;
  while(*dst!='\0'){
    dst++;
  }
  do{
    *dst=*src;
    src++;
    dst++;
  }while(*src!='\0');
  return res;
  // panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  assert(s1 != NULL);
	assert(s2 != NULL);

  while(*s1!='\0' && *s2!='\0' && *s1==*s2){
    s1++;
    s2++;
  }
  if((unsigned char)*s1>(unsigned char)*s2){
    return 1;
  }else if((unsigned char)*s1<(unsigned char)*s2){
    return -1;
  }
  return 0;
  // panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  assert(s1 != NULL);
	assert(s2 != NULL);

  while(*s1!='\0' && *s2!='\0' && *s1==*s2 && n){
    s1++;
    s2++;
    n--;
  }
  if(n==0) return 0;
  if((unsigned char)*s1>(unsigned char)*s2){
    return 1;
  }else if((unsigned char)*s1<(unsigned char)*s2){
    return -1;
  }
  return 0;
  // panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  assert(s!=NULL);

  unsigned char *p=s;
  while(n){
    *p=c;
    p++;
    n--;
  }
  return s;
  // panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  assert(dst!=NULL);
  assert(src!=NULL);
  if(n==0 || dst==src) return dst;
  unsigned char * res=dst;
  const unsigned char * source=src;
  if(res<source){
    while(n--){
      *res=*source;
      res++;
      source++;
    }
  }else{
    res+=n;
    source+=n;
    while(n--){
      res--;
      source--;
      *res=*source;
    }
  }
  return dst;

  // panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  assert(out!=NULL);
  assert(in!=NULL);
  if(n==0 || out==in) return out;
  unsigned char *dst=out;
  const unsigned char *src=in;
  while(n--){
    *dst=*src;
    dst++;
    src++;
  }
  return out;
  // panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  assert(s1!=NULL);
  assert(s2!=NULL);
  const unsigned char* src1=s1;
  const unsigned char* src2=s2;
  while(n && *src1==*src2){
    src1++;
    src2++;
    n--;
  }
  if(n==0 || *src1==*src2) return 0;
  if(*src1>*src2){
    return 1;
  }else{
    return -1;
  }
  // panic("Not implemented");
}

#endif
