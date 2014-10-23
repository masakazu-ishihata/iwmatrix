# iwmatrix

## 概要

[ウェーブレット配列][wm] の実装。  
[ここ][takeda25] を見ながら実装しました。  
とてもわかりやすくて涙しました。

## methods
### new / free

    iwmatrix *iwmatrix_new(ul _n, ui *_a);

整数配列 _a[0,_n) を表現するウェーブレット行列 _m[0,_n) を生成する。  
あくまでも _m は整数配列として扱う。

    void iwmatrix_free(void *_p);

ウェーブレット行列で表現された整数配列 _m[0,_n) を free する。

### access / rank / select

    ui iwmatrix_access(iwmatrix *_m, ul _i);

整数配列 _m[0,_n) の _i 番目の値 _m[_i] を返す。

    ul iwmatrix_rank(iwmatrix *_m, ui _v, ul _i);

整数配列 _m[0,_i) 中の _v の数を返す。

    ul iwmatrix_select(iwmatrix *_m, ui _v, ul _i);

整数配列 _m[0,_n) 中の (_i+1) 番目の _v の位置を返す。

[wm]: http://www.dcc.uchile.cl/~gnavarro/ps/spire12.4.pdf "The Wavelet Matrix"
[takeda25]: http://d.hatena.ne.jp/takeda25/20130303/1362301095 "中学生にもわかるウェーブレット行列"
