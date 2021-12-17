#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void calcul_valeur_propre() {
    int n = 100000;
    int tab[n];
    #pragma omp parallel for num_threads(12)
    for (int i = 0; i < n; i++)
    {
        #pragma omp parallel for num_threads(12)
        for (int j = 0; j < i; j++)
        {
            tab[i] += j;
        }
    }
    

    printf("result  : %d\n", tab[99999]);
}



int main(int argc, char const *argv[])
{
    double start, end;
    start = omp_get_wtime();
    calcul_valeur_propre();
    //Fin du timer
    end = omp_get_wtime();
    //Affichage du temps d'execution
    printf("Work took %f seconds\n", end - start);
    return 0;
}

