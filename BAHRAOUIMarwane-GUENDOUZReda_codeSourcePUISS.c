#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <errno.h> 
#include <string.h>

#define MAX_VALUE_MATRIX 2048

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
 * @param matrix matrice a afficher
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
    int i, j;
    int finish = 0;
    double start, end;
    double ak = 1, mem = 0, old_ak = 0;
    double v[MATRIX_SIZE], vk[MATRIX_SIZE];

    //Initialisation du vecteur v
    v[0] = 1;
    for (i = 1; i < MATRIX_SIZE; i++)
        v[i] = 0;

    //Debut du timer
    printf("\n\nDebut du chronometre\n");
    start = omp_get_wtime();

    do
    {
        //Premier partie de l'affectation de vk 
        #pragma omp parallel for schedule(static, MATRIX_SIZE / NUM_THREADS) num_threads(NUM_THREADS)
        for (i = 0; i < MATRIX_SIZE; i++)
        {
            vk[i] = 0;
            //Avk-1
            #pragma omp parallel for schedule(static, MATRIX_SIZE / NUM_THREADS) num_threads(NUM_THREADS)
            for (j = 0; j < MATRIX_SIZE; j++)
            {
                vk[i] = vk[i] + A.tabMatrix[i * MATRIX_SIZE + j] * v[j];
            }
        }

        //
        if (old_ak == 0)
        {
            ak = vk[0];
            for (i = 1; i < MATRIX_SIZE; i++)
            {
                if (vk[i] > ak)
                    ak = vk[i];
            }
        }

        //Deuxieme partie de l'affectation de vk 
        //1/ak
        #pragma omp parallel for schedule(static, MATRIX_SIZE / NUM_THREADS) num_threads(NUM_THREADS)
        for (i = 0; i < MATRIX_SIZE; i++)
        {
            vk[i] = vk[i] * (1 / ak);
            v[i] = vk[i];
        }

        //ARGMAX de la valeur propre
        ak = vk[0];
        #pragma omp parallel for schedule(static, MATRIX_SIZE / NUM_THREADS) num_threads(NUM_THREADS)
        for (i = 1; i < MATRIX_SIZE; i++)
        {
            #pragma omp critical
            {
                if (fabs(vk[i]) > ak)
                    ak = vk[i];
            }
        }

        //Si l'ecart entre la valeur propre de la boucle n-1 et la valeur propre de la boucle n est inferieur a la convergence
        if (fabs(ak - old_ak) < convergence)
            finish = 1;
        else
            old_ak = ak;

    } while (!finish);

    //Factoriser le vector en fonction de la valeur propre
    for (i = 0; i < MATRIX_SIZE; i++)
    {
        if (fabs(mem) < fabs(vk[i]))
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

    //Afficher vecteur
    if (MATRIX_SIZE < 6)
    {
        for(i = 0; i < MATRIX_SIZE; i++){
            printf("%f\t", vk[i]);
        }
        printf("\n");
    }

    //Affichage du temps d'execution
    printf("Work took %f seconds\n\n", end - start);
    printf("####################################\n\n");
}

int main()
{
    double convergence;
    char errorRate[15];
    char* endErrorRate = NULL;
    
    /* Initialisation de la generation de nombre aleatoire */
    srand(time(NULL));

    Matrix A;
    init_matrix(&A, MATRIX_SIZE);
    
    printf("/*************\\ \n");
    printf("POWER ITERATION\n");
    printf("\\*************/ \n\n");

    do
    {   
        //Choix du taux d'erreur
        printf("Veuillez entrez un taux d'erreur superieur a 0 \n(Exemple : 0.001)\nCtrl+C pour quitter\n");
        scanf("%s",errorRate);
        
        //Conversion d'une chaine de caractere en double
        convergence = strtod(errorRate,&endErrorRate);

        if (convergence > 0)
            power_iteration(A,convergence);
        
    }while(1);

    return 0;
}