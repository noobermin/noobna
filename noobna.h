#ifndef NOOBNA_H_
#define NOOBNA_H_
/*------------------------------------------------------------------
noobna.h, simple nd array file reader, for small n and non-resizable
(although you can resize it if you want, just have to copy everything
since it's contiguous, and you need to do that your own self buddy)

Copyright: Gregory K. Ngirmang, 2020


Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation files
(the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Usage:
#include in a project. Define NOOBNA_IMPL to implement functions. Define
NOOBNA_STATIC if you want the implementation to be static. Otherwise,
just define the type. Please don't use NOOBNA_STATIC without IMPL
and then implement static. You know such will be the source of linker
errors.
---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef int    _nbna_int;
typedef double _nbna_float;

typedef struct nbna_ {
  _nbna_int n,sz;
  _nbna_int _off;
  _nbna_int *ns;
  _nbna_float *x;
}noobna_t;

#if !defined(NOOBNA_IMPL) && !defined(NOOBNA_STATIC)
  void nbna_clean(noobna_t*);
  int  nbna_loadfile(FILE*, noobna_t*);
  _nbna_float* nbna_getndim(const noobna_t*,_nbna_int);
  _nbna_float* nbna_getq(const noobna_t*);

#endif

#ifdef NOOBNA_IMPL

#ifdef NOOBNA_STATIC
 #undef NOOBNA_STATIC
 #define NOOBNA_STATIC static
#else 
 #define NOOBNA_STATIC
#endif /* NOOBNA_STATIC */

NOOBNA_STATIC _nbna_float* nbna_getndim(const noobna_t* in, _nbna_int n) {
  size_t offset=0;
  if (n > in->n) return NULL;
  while ((--n)>=0)
    offset += in->ns[n];
  return in->x + offset;
}

NOOBNA_STATIC _nbna_float* nbna_getq(const noobna_t* in) {
  return in->x + in->_off;
}

NOOBNA_STATIC _nbna_int nbna_getI(const noobna_t* in, ...) {
  va_list args;
  _nbna_int
    len = in->n, I = 0, fac = 1, n = 0;
  for(n=0; n < len; ++n) fac *= in->ns[n]; /* allow div of in->ns[0] in following */
  va_start(args, len);
  for(n=0; n < len; ++n) {
    _nbna_int curi=0;
    fac /= in->ns[n];
    curi = va_arg(args, _nbna_int);
    I += curi*fac;
  }
  va_end(args);
  return I;
}

NOOBNA_STATIC _nbna_int nbna_getIa(const noobna_t* in, const _nbna_int* Is) {
  _nbna_int
    len = in->n, I = 0, fac = 1, n = 0;
  for(n=0; n < len; ++n) fac *= in->ns[n]; /* allow div of in->ns[0] in following */
  for(n=0; n < len; ++n) {
    fac /= in->ns[n];
    I += Is[n]*fac;
  }
  return I;
}




NOOBNA_STATIC void nbna_clean(noobna_t* out) {
  free(out->ns); free(out->x); out->n=0;
}

NOOBNA_STATIC int nbna_loadfile(FILE* f, noobna_t *out){
#define _fr(a,b,l) if(fread(a,b,l,f)!=l) goto _noobna_loadfile_fail; 0
#define _mk(dest,type,sz) if ( (dest=(type*)malloc(sizeof(type)*(sz))) == NULL) \
    goto _noobna_loadfile_fail;
  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
  int sz=1,acc=0, n;
  
  n = out->n  = 0;
  out->ns = NULL, out->x = NULL;
  _fr(&out->n,sizeof(_nbna_int),1);
  n = out->n;
  /* ns */
  _mk(out->ns,_nbna_int, out->n);
  _fr(out->ns,sizeof(_nbna_int),out->n);
  for (_nbna_int i=0; i<n; ++i){
    _nbna_int cursz = out->ns[i];
    sz*= cursz;
    acc+=cursz;
  }
  /* dims */
  _mk(out->x, _nbna_float, acc + sz);
  out->_off = acc;
  out->sz   = sz;
  acc=0;
  for (_nbna_int i=0; i<n; ++i){
    _nbna_int cursz = out->ns[i];
    _fr(out->x + acc, sizeof(_nbna_float), cursz);
    acc+=cursz;
  }
  /* quantity */
  _fr(out->x + out->_off, sizeof(_nbna_float), sz);
  return 0;
  /*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
#undef  _fr
#undef  _mk
 _noobna_loadfile_fail:
  nbna_clean(out);
  return -1;
}

#ifdef NOOBNA_DEBUG
#ifdef _fr
#error "_fr defined!"
#endif
#ifdef _mk
#error "_mk defined!"
#endif
#endif /*NOOBNA_DEBUG*/

#endif /*NOOBNA_IMPL*/

#endif /*NOOBNA_H_*/
