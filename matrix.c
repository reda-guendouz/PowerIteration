#include "matrix.h"

/**
 * Initialise une matrice carre avec une taille donne
 * 
 * @param matrix matrice a initialiser
 * @param parN taille de la matrice
 */
void init_matrix(Matrix *matrix, int parN)
{
    int i, j;

    /* Initialisation de la matrice */
    matrix->n = parN;
    matrix->tabMatrix = NULL;
    matrix->tabMatrix = malloc((parN * parN) * sizeof(float));

    /* Remplir la matrice avec des valeurs aleatoires */
    for (i = 0; i < parN; i++)
        for (j = 0; j < parN; j++)
            matrix->tabMatrix[i * parN + j] = 1 + rand() % MAX_VALUE_MATRIX;

    
    matrix->tabMatrix[0] = -6;
    matrix->tabMatrix[1] = 4;
    matrix->tabMatrix[2] = 45;
    matrix->tabMatrix[3] = -14;
    matrix->tabMatrix[4] = -14;
    matrix->tabMatrix[5] = 51;
    matrix->tabMatrix[6] = -25;
    matrix->tabMatrix[7] = 524;
    matrix->tabMatrix[8] = 144;
    matrix->tabMatrix[9] = -24;
    matrix->tabMatrix[10] = 24;
    matrix->tabMatrix[11] = 142;
    matrix->tabMatrix[12] = -214;
    matrix->tabMatrix[13] = 452;
    matrix->tabMatrix[14] = 1421;
    matrix->tabMatrix[15] = -45;

}

/**
 * Affiche la matrice donne en parametre
 * 
 * @param matrix matrice a affichee
 */
void display_matrix(Matrix matrix)
{
    printf("\nDISPLAY MATRIX\n");
    for (int i = 0; i < matrix.n; i++)
    {
        printf("[");
        for (int j = 0; j < matrix.n; j++)
        {
            printf(" %.3f ", matrix.tabMatrix[(i - 1) * matrix.n + j]);
        }
        printf("]\n");
    }
}

/**
 * Algorithme de calcul de valeur propre parrallelise
 * 
 * Code de base utilise : https://www.codewithc.com/c-program-for-power-method/
 * 
 * @param A 
 * @param convergence 
 */
void power_iteration(Matrix A, double convergence)
{
    int i, j, k;
    int finish = 0;
    double start, end;
    double ak = 1, mem = 0, old_ak = 0;
    double v[MATRIX_SIZE], vk[MATRIX_SIZE];

    //Initialisation du vecteur v
    v[0] = 1;
    for (int i = 1; i < MATRIX_SIZE; i++)
        v[i] = 0;

    //Debut du timer
    printf("\n\nDebut du chronometre\n");
    start = omp_get_wtime();

    do
    {
        //Creation de vk (parallelisable)
        #pragma omp parallel for schedule(static, MATRIX_SIZE / NUM_THREADS) num_threads(NUM_THREADS)
        for (j = 0; j < MATRIX_SIZE; j++)
        {
            vk[j] = 0;
            //(parallelisable)
            #pragma omp parallel for schedule(static, MATRIX_SIZE / NUM_THREADS) num_threads(NUM_THREADS)
            for (k = 0; k < MATRIX_SIZE; k++)
            {
                vk[j] = vk[j] + A.tabMatrix[j * MATRIX_SIZE + k] * v[k];
            }
        }

        if (old_ak == 0)
        {
            ak = vk[0];
            for (j = 1; j < MATRIX_SIZE; j++)
            {
                if (vk[j] > ak)
                    ak = vk[j];
            }
        }

        #pragma omp parallel for schedule(static, MATRIX_SIZE / NUM_THREADS) num_threads(NUM_THREADS)
        for (j = 0; j < MATRIX_SIZE; j++)
        {
            vk[j] = vk[j] * (1 / ak);
            v[j] = vk[j];
        }

        //ARGMAX (parallelisable)
        ak = fabs(vk[0]);
        #pragma omp parallel for schedule(static, MATRIX_SIZE / NUM_THREADS) num_threads(NUM_THREADS)
        for (j = 1; j < MATRIX_SIZE; j++)
        {
            #pragma omp critical
            {
                if ((fabs(vk[j])) > ak)
                    ak = fabs(vk[j]);
            }
        }

        if (fabs(ak - old_ak) < convergence)
            finish = 1;
        else
            old_ak = ak;

    } while (!finish);

    //Factoriser le vector
    for (i = 0; i < MATRIX_SIZE; i++)
    {
        if (mem < vk[i])
            mem = vk[i];
    }
    if (mem)
    {
        for (i = 0; i < MATRIX_SIZE; i++)
            vk[i] = vk[i] / mem;
    }

    //Fin du timer
    printf("Fin du chronometre \n\n");
    end = omp_get_wtime();

    //Affichage de la solution
    printf("\nValeur propre max : %f\n", ak);
    

    //Affichage du temps d'execution
    printf("Work took %f seconds\n\n", end - start);
    printf("####################################\n\n");
}