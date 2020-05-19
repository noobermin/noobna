/**
 * to compile:
 * cc -g testnoobna.c -o testnoobna
 */
#include <stdio.h>
#define NOOBNA_IMPL
#define NOOBNA_DEBUG
#define NOOBNA_STATIC
#include "../noobna.h"

void
pnoobna(noobna_t*);

int
main(int argc, char*argv[]) {
  FILE *fp;
  noobna_t in;
  if (argc != 2) {
    printf("usage: testnoobna <file>\n");
    return 0;
  }
  if ((fp=fopen(argv[1],"r"))==NULL) {
    perror("failed to open file\n");
    return 1;
  } else {
    int ret = nbna_loadfile(fp, &in);
    if (ret == -1) {
      perror("i/o error with file\n");
      return 2;
    }
    fclose(fp);
  }
  pnoobna(&in);
  nbna_clean(&in);
  return 0;
}




void ptab(int tab) {
  if (tab > 0) while(tab--) putchar(' '); }

void
parr(const _nbna_float *arr,
     int len,
     int tab,
     int maxlen){
  const char fmt[]="%1.3e";
  int online = 0, i;
  if(!len) return;  
  ptab(tab);
  for (i=0; i < len; ++i) {
    printf(fmt, arr[i]);
    if (++online > maxlen) {
      putchar('\n'); ptab(tab);
      online=0;
    } else {
      putchar(' ');
    }
  }
  printf(fmt, arr[0]);
  putchar('\n');
}
#define MAXLEN 8
#define TAB    2
void
pnoobna(noobna_t* in) {
  int i, acc=0;
  /* dimensions */
  for(i=0; i<in->n; ++i) {
    int n=in->ns[i];
    printf("n%d: %d\n", i, n);
    parr(nbna_getndim(in,i),
         n, TAB, MAXLEN);
  }
  /* data */
  printf("data: %d\n", in->sz);
  parr(nbna_getq(in),in->sz,TAB,MAXLEN);
}
