#ifndef UTILS_H
#define UTILS_H

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLAMP(v, a, b) MAX((a), MIN((v), (b)))

#include <stdint.h>

#define function   static
#define global     static

typedef unsigned char u1;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  f32;
typedef double f64;

typedef uint8_t  b8;
typedef uint32_t b32;

typedef uintptr_t uptr;


#define true  1
#define false 0

#define PI 3.141592653589793
#define HALF_PI 1.570796326794897
#define TAU 6.283185307179586
#define SIN_CURVE_A 0.0415896
#define SIN_CURVE_B 0.00129810625032

#define KILOBYTE(x) ((u64)(x) * (u64)1024L)
#define MEGABYTE(x) ((u64)KILOBYTE(x) * (u64)1024L)
#define GIGABYTE(x) ((u64)MEGABYTE(x) * (u64)1024L)

#endif
