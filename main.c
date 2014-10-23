#include "main.h"

#define rand_r ( rand() / (double)RAND_MAX )
#define dice(n) ( (int)(n * rand_r) )

int main(void)
{
  ui n = 32;
  ui a[32] = {11, 0, 15, 6, 5, 2, 7, 12, 11, 0, 12, 12, 13, 4, 6, 13, 1, 11, 6, 1, 7, 10, 2, 7, 14, 11, 1, 7, 5, 4, 14, 6};
  iwmatrix *m = iwmatrix_new(n, a);

  ui i, v;
  ul r, s;

  /*------------------------------------*/
  /* access */
  /*------------------------------------*/
  printf("<<<< access >>>>\n");
  printf("    i:");
  for(i=0; i<n; i++) printf("%3u", i);
  printf("\n");
  printf(" a[i]:");
  for(i=0; i<n; i++) printf("%3u", iwmatrix_access(m, i));
  printf("\n");

  /*------------------------------------*/
  /* rank */
  /*------------------------------------*/
  printf("<<<< rank >>>>\n");
  printf("    i:");
  for(i=0; i<n; i++) printf("%3u", i);
  printf("\n");

  for(v=0; v<m->s; v++){
    printf("%5u:", v);
    for(i=0; i<n; i++){
      r = iwmatrix_rank(m, v, i);
      printf("%3lu", r);
    }
    printf("\n");
  }

  /*------------------------------------*/
  /* select */
  /*------------------------------------*/
  printf("<<<< select >>>>\n");
  printf("    i:");
  for(i=0; i<n; i++) printf("%3u", i);
  printf("\n");

  for(v=0; v<m->s; v++){
    printf("%5u:", v);
    for(i=0; i<n; i++){
      s = iwmatrix_select(m, v, i);
      if(s == n) break;
      printf("%3lu", s);
    }
    printf("\n");
  }

  iwmatrix_free(m);
  return 0;
}
