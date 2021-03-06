#define NOOBNA_IMPL
#include "../noobna.h"
#include <stdio.h>

const char usage[] = "\
usage: test_nbna_getI3 <input> <i> <j> <k>";

#define FLT_FMT "%.10e"
#define TESTDATA_N 3

main(int argc, char *argv[]){
  FILE *fp;
  noobna_t in;
  int n = 0, I=0;
  int i,j,k;
  if (argc < 5) {
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
  if (n != TESTDATA_N) {
    fprintf(
      stderr,
      "wrong number of indices (got %d, expected %d for %s)\n",
      n, TESTDATA_N, argv[1]);
    puts(usage); return 0;
  }
  i = atoi(argv[2]), j = atoi(argv[3]), k = atoi(argv[4]);
  if ( i < 0 || i >= in.ns[0] ) {
    fprintf(stderr,
            "i exceeds size of 1st axis (%d out of %d)\n",
            i, in.ns[0]);
    return 3;
  }
  if ( j < 0 || j >= in.ns[1] ) {
    fprintf(stderr,
            "j exceeds size of 2nd axis (%d out of %d)\n",
            j, in.ns[1]);
    return 3;
  }
  if ( k < 0 || k >= in.ns[2] ) {
    fprintf(stderr,
            "k exceeds size of 3rd axis (%d out of %d)\n",
            k, in.ns[2]);
    return 3;
  }

  I = nbna_getI(&in, i,j,k);
  printf("q[%d] == " FLT_FMT "\n", I, nbna_getq(&in)[I]);
  nbna_clean(&in);
  return 0;
}
