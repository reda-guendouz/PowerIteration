#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <errno.h> 
#include <string.h>

#define MAX_VALUE_MATRIX 5

#define MATRIX_SIZE 4

#define NUM_THREADS 4


typedef struct Matrix Matrix;

struct Matrix
{
    int n;
    float *tabMatrix;
};

void init_matrix(Matrix *matrix, int parN);

void print_matrix(Matrix matrix);

void power_iteration(Matrix matrix, double errorRate);