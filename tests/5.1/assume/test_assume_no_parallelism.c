//===--- test_assume_no_parallelism.c ---------------------------------------===//
//
//  OpenMP API Version 5.1 Aug 2021
//
//  Using assume no_parallelism, the compiler can be optimized as it knows that a
//  structured block does not contain any parallelism.
//
////===----------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"
#include <math.h>

#define N 1024

int errors, i;

int test_assume_no_parallelism() {
    #pragma omp assume no_parallelism
    {
        int x = omp_get_thread_num(); // OMP runtime routine; should be 0
        int arr[N];
        for(i = 0; i < N; i++){
            arr[i] = i + x;
        }
    }
    #pragma omp target parallel for map(tofrom: arr)
    for(i = 0; i < N; i++){
        arr[i] = arr[i]*2;
    }
    // Test that no issues were caused from using assume directive
    for(i = 0; i < N; i++){
        OMPVV_TEST_AND_SET(errors, arr[i] != (i+x)*2);
    }
    return errors;
}

int main() {
   errors = 0;
   OMPVV_TEST_OFFLOADING;
   OMPVV_TEST_AND_SET_VERBOSE(errors, test_assume_no_parallelism() != 0);
   OMPVV_REPORT_AND_RETURN(errors);
}
