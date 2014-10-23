#ifndef _IWMATRIX_H_
#define _IWMATRIX_H_

/*------------------------------------*/
/* include */
/*------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ibary.h>

/*------------------------------------*/
/* typedef */
/*------------------------------------*/
typedef unsigned char uc; /* 8  bits */
typedef unsigned int  ui; /* 16 bits */
typedef unsigned long ul; /* 32 bits */

/*------------------------------------*/
/* iwmatrix */
/*------------------------------------*/
typedef struct IWMATRIX
{
  ul  n;     /* length of array */
  ui  s;     /* # alphabets */
  ui  h;     /* hight of matrix */
  ui *a;     /* array */
  ibary **B; /* bit matrix */
  ui *Z;     /* # 0s in each column */
  ul *S;     /* starting point of each charactor */
} iwmatrix;

/* new / free / show */
iwmatrix *iwmatrix_new(ul _n, ui *_a);
void iwmatrix_free(void *_p);

/* access / rank / select */
ui iwmatrix_access(iwmatrix *_m, ul _i);
ul iwmatrix_rank(iwmatrix *_m, ui _v, ul _i);
ul iwmatrix_select(iwmatrix *_m, ui _v, ul _i);

/* show */
void iwmatrix_show(FILE *_fp, iwmatrix *_p);

#endif
