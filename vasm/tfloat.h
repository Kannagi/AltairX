/* tfloat.h Floating point type and string conversion function. */
/* (c) 2014 Frank Wille */

#ifdef __VBCC__
typedef double tfloat;
#define strtotfloat(n,e) strtod(n,e)
#else
typedef long double tfloat;
#define strtotfloat(n,e) strtold(n,e)
#endif
