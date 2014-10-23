#include "iwmatrix.h"

#define mask(i)   ( 0x01 << (i) )
#define bit(a, i) ( ((a) & mask(i)) ? 1 : 0 )

/*----------------------------------------------------------------------------*/
/* new / free */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* new wavlet matrix _m[0,_n) = _a[0,_n) */
/*------------------------------------*/
iwmatrix *iwmatrix_new(ul _n, ui *_a)
{
  ul i;
  iwmatrix *_m = (iwmatrix *)malloc(sizeof(iwmatrix));

  /*------------------------------------*/
  /* clone a */
  /*------------------------------------*/
  _m->n = _n;
  _m->a = (ui *)calloc(_m->n, sizeof(ui));
  memmove(_m->a, _a, _n * sizeof(ui));

  /*------------------------------------*/
  /* alphabet size */
  /*------------------------------------*/
  _m->s = 0;
  for(i=0; i<_m->n; i++)
    if(_m->s < _a[i])
      _m->s = _a[i];
  _m->s++;

  /*------------------------------------*/
  /* Wavelet Matrix B */
  /*------------------------------------*/
  /* initialize */
  _m->h = ceil( log2(_m->s) );                      /* # digits (bits) */
  _m->B = (ibary **)calloc(_m->h, sizeof(ibary *)); /* Wavelet Matrix */
  _m->Z = (ui *)calloc(_m->h, sizeof(ui));          /* # 0s in each digit */
  _m->S = (ul *)calloc(_m->s, sizeof(ul));          /* starting point of each number */

  /* for radix sort */
  ui *s = (ui *)calloc(_m->n, sizeof(ui)); /* sorted a */
  ui *t = (ui *)calloc(_m->n, sizeof(ui)); /* temporal space */
  memmove(s, _a, _n * sizeof(ui));         /* a -> s */

  /* construct Wavelet Matrix B */
  ui j, k, n;
  uc b;
  for(j=0; j<_m->h; j++){
    /* construct B[j] */
    _m->B[j] = ibary_new(_m->n);
    for(i=0; i<_m->n; i++){
      b = bit(s[i], _m->h - j - 1); /* j-th bit of s[i] */
      ibary_set(_m->B[j], i, b);    /* set B[j][i] = b */
      if(b == 0) _m->Z[j]++;        /* count # 0s in B[j] */
    }

    /* radix sort */
    for(n=0, i=0; i<_m->n; i++){
      b = bit(s[i], _m->h - j - 1); /* j-th bit of s[i] */
      k = b ? _m->Z[j] + n : i - n; /* get a position in bucket "b" */
      t[k] = s[i];                  /* drop into the bucket */
      n += b;                       /* # items in bucket "1" */
    }
    memmove(s, t, _m->n * sizeof(ui)); /* t -> s */
  }

  /* S */
  for(i=0; i<_m->s; i++) _m->S[i] = _m->n;
  for(i=0; i<_m->n; i++) _m->S[ s[_m->n - i - 1] ] = _m->n - i - 1;

  /* free */
  free(s);
  free(t);

  return _m;
}
/*------------------------------------*/
/* free */
/*------------------------------------*/
void iwmatrix_free(void *_p)
{
  int i;
  iwmatrix *_m = (iwmatrix *)_p;

  if(_m != NULL){
    for(i=0; i<_m->h; i++){
      ibary_free(_m->B[i]);
    }
    free(_m->B);
    free(_m->Z);
    free(_m->S);
    free(_m->a);
    free(_m);
  }
}


/*----------------------------------------------------------------------------*/
/* access / rank / select */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* access(_m, _i) = _m[_i] */
/*------------------------------------*/
ui iwmatrix_access(iwmatrix *_m, ul _i)
{
  return _m->a[_i];
}
/*------------------------------------*/
/* rank(_m, _v, _i) = # _v in _m[0,_i)  */
/*------------------------------------*/
ul iwmatrix_rank(iwmatrix *_m, ui _v, ul _i)
{
  if(_i == 0) return 0;
  if(_m->S[_v] == _m->n) return 0;

  int j;
  uc b;
  ul k = _i;

  for(j=0; j<_m->h; j++){
    b = bit(_v, _m->h - j - 1);     /* b = j-th bit of _v */
    k = ibary_rank(_m->B[j], b, k); /* k = # items in B[j] s.t. their j-th bit = b */
    k = b * _m->Z[j] + k;           /* next k */
  }

  return k - _m->S[_v];
}
/*------------------------------------*/
/* select(_m, _v, _i) = index of (_i+1)-th v in _m */
/*------------------------------------*/
ul iwmatrix_select(iwmatrix *_m, ui _v, ul _i)
{
  /* skip if no such index */
  if(_i+1 > iwmatrix_rank(_m, _v, _m->n)) return _m->n;

  /* binary search */
  ul r, v;
  ul s = 0, t = _m->n, m;
  do{
    m = (s + t) / 2;                /* mean */
    r = iwmatrix_rank(_m, _v, m+1); /* # _vs in _m[s, t) */
    v = iwmatrix_access(_m, m);     /* _m[m-1] */

    if(r == _i + 1 && v == _v) return m;
    else if(r < _i + 1) s = m + 1;
    else                t = m - 1;

    if(s > t){
      printf("error : %lu < %lu\n", s, t);
      exit(1);
    }
  }while(1);
}

/*----------------------------------------------------------------------------*/
/* show */
/*----------------------------------------------------------------------------*/
void iwmatrix_show(FILE *_fp, iwmatrix *_m)
{
  int i, j, b;

  fprintf(_fp, "iwmatrix\n");
  fprintf(_fp, "n = %lu\n", _m->n);
  fprintf(_fp, "s = %u\n", _m->s);
  fprintf(_fp, "h = %u\n", _m->h);
  fprintf(_fp, "a =\n");

  /* i */
  fprintf(_fp, "index : ");
  for(i=0; i<_m->n; i++)
    fprintf(_fp, "%d", i % 10);
  fprintf(_fp, "\n");

  /* 10 */
  fprintf(_fp, "   10 : ");
  for(i=0; i<_m->n; i++)
    fprintf(_fp, "%d", _m->a[i] / 10);
  fprintf(_fp, "\n");

  /* 1 */
  fprintf(_fp, "    1 : ");
  for(i=0; i<_m->n; i++)
    fprintf(_fp, "%d", _m->a[i] % 10);
  fprintf(_fp, "\n");

  /* bits */
  for(j=0; j<_m->h; j++){
    fprintf(_fp, "bit %d : ", j);
    for(i=0; i<_m->n; i++){
      b = bit(_m->a[i], j);
      fprintf(_fp, "%d", b);
    }
    fprintf(_fp, " [%3d]\n", _m->Z[j]);
  }

  /* show B */
  for(i=0; i<_m->h; i++)
    ibary_show(stdout, _m->B[i]);
}
