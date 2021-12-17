#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

typedef struct Matrix Matrix;

struct Matrix
{
    int n;
    float *tabMatrix;
};

void init_matrix(Matrix *matrix, int parN);

void print_matrix(Matrix matrix);