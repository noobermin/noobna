#define NOOBNA_IMPL
#include "../noobna.h"
#include <stdio.h>

const char usage[] = "\
usage: nbna_stats <input>";

#define FLT_FMT "%.10e"

main(int argc, char *argv[]){
  FILE *fp;
  noobna_t in;
  int n = 0, I=0;
  int i,j,k;
  _nbna_float max, min, avg;
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

  max = nbna_max(&in), min = nbna_min(&in), avg = nbna_avg(&in);
  
  printf(
    "min == " FLT_FMT
    ",max == " FLT_FMT
    ",avg == " FLT_FMT "\n",
    min,max,avg);
  nbna_clean(&in);
  return 0;
}
