#define NOOBNA_IMPL
#include "../noobna.h"
#include <stdio.h>

const char usage[] = "\
usage: nbna_getval <input> <i>...";

#define FLT_FMT "%.10e"

main(int argc, char *argv[]){
  FILE *fp;
  noobna_t in;
  int n = 0, *Is=NULL, I=0;
  if (argc < 2) {
    puts(usage); return 0;
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
  n = in.n;
  if (argc < 2 + n) {
    fprintf(
      stderr,
      "not enough indices (got %d, expected %d for %s)\n",
      argc - 2, n, argv[1]);
    puts(usage); return 0;
  }
  Is = (int*) malloc(sizeof(int)*n);
  if (Is == NULL) { perror("memory error\n"); return 3; }

  for (int i=2; i < n+2; ++i) {
    Is[i-2] = atoi(argv[i]);
    if (Is[i-2] < 0 || Is[i-2] >= in.ns[i-2]) {
      fprintf(stderr,
              "index %d exceeds corresponding axis (%d out of %d)\n",
              i-2, Is[i-2], in.ns[i-2]);
      return 4;
    }
  }
  
  I = nbna_getIa(&in, Is);
  printf("q[%d] == " FLT_FMT "\n", I, nbna_getq(&in)[I]);
  nbna_clean(&in);
  return 0;
}

