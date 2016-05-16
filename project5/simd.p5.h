#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>

#include <omp.h>

#ifndef SIMD_H
#define SIMD_H

// SSE stands for Streaming SIMD Extensions
 #define SSE_WIDTH4
 #define ALIGNED__attribute__((aligned(16)))

 voidSimdMul(    float *, float *,  float *, int );
 floatSimdMulSum( float *, float *, int );


#endif// SIMD_H
