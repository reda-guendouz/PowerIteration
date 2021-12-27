#include "matrix.h"

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