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

void init_matrix(Matrix *matrix, int parN)
{
    matrix->n = parN;

    matrix->tabMatrix = NULL;

    matrix->tabMatrix = malloc((parN * parN) * sizeof(float));

    for (int i = 1; i <= parN; i++)
    {
        for (int j = 1; j <= parN; j++)
        {
            printf("A[%d][%d]=", i, j);
            scanf("%f", &matrix->tabMatrix[(i - 1) * parN + j]);
        }
    }
}

void print_matrix(Matrix matrix)
{
    printf("\nDISPLAY MATRIX\n");
    for (int i = 1; i <= matrix.n; i++)
    {
        printf("[");
        for (int j = 1; j <= matrix.n; j++)
        {
            printf(" %.3f ", matrix.tabMatrix[(i - 1) * matrix.n + j]);
        }
        printf("]\n");
    }
}

int main()
{
    int i, j, k, n = 3, m = 1000000;
    float v[n + 1], vk[n + 1], ak = 1, mem;
    double start, end;
    Matrix A;
    init_matrix(&A, n);
    print_matrix(A);

    v[1] = 1;
    v[2] = 0;
    v[3] = 0;

    start = omp_get_wtime();

    for (i = 0; i < m; i++)
    { // JUSQU'A CONVERGENCE

        //Creation de vk (parallelisable)
        for (j = 1; j <= n; j++)
        {
            vk[j] = 0;
            //(parallelisable)
            for (k = 1; k <= n; k++)
            {
                //Reduction
                vk[j] = vk[j] + A.tabMatrix[(j - 1) * n + k] * v[k];
            }
        }

        if (i == 0)
        {
            ak = vk[1];
            for (j = 2; j <= n; j++)
            {
                if (vk[j] > ak)
                    ak = vk[j];
            }
        }

        //(parallelisable)
        for (j = 1; j <= n; j++)
        {
            vk[j] = vk[j] / ak;
        }

        //(parallelisable)
        for (j = 1; j <= n; j++)
        {
            v[j] = vk[j];
        }

        //ARGMAX (parallelisable)
        ak = fabs(vk[1]);
        for (j = 2; j <= n; j++)
        {
            if ((fabs(vk[j])) > ak)
                ak = fabs(vk[j]);
        }
    }
    mem = 0;
    for (i = 1; i <= n; i++)
    {
        if (mem < vk[i])
            mem = vk[i];
    }
    if (mem)
    {
        for (i = 1; i <= n; i++)
        {
            vk[i] = vk[i] / mem;
        }
    }
    end = omp_get_wtime();

    printf("\nValeur propre max : %f", ak);
    printf("\n\nVecteur propre max :\n");
    for (i = 1; i <= n; i++)
    {
        printf("%f\t", vk[i]);
    }
    printf("\n");

    printf("Work took %f seconds\n", end - start);
    return 0;
}