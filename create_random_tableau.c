#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int random_(int n);
char* tableau_vide(int n);
char* tableau_rand(char* chaine, int n);


int main(){
    int n;
    printf("Chose the difficulty : 3 for 3x3 ,4 for 4x4 ,5 for 5x5 : \n");
    scanf("%d", &n);
    switch (n) {
        case 3 : printf("3x3 chosen \n") ;
        break ;
        case 4 : printf("4x4 chosen \n") ;
        break ;
        case 5 : printf("5x5 chosen \n") ;
        break ;
        default : printf("Invalid choice\n");
        return 1; //add loop here later
    }
    char* g = tableau_vide(n);
    char* grid = tableau_rand(g,n);
    printf("Generated grid:\n");
    for (int i = 0; i < n * n; i++) {
        printf("(%d) ", grid[i]);
        if ((i + 1) % n == 0) {
            printf("\n");
        }
    }
    return 0;
}

int random_(int n) {
    return (rand() % n - 1) + 1;
}
char* tableau_vide(int n){
    static char chaine[100];
    for (int i = 0; i<n * n; i++){
        chaine[i] = 0;
    }
    return chaine; // creer une chaine vide
}

char* tableau_rand(char* chaine, int n){
    int taille = n * n;
    int a;
    int used[taille];
    for (int i = 0; i< taille; i++){
        used[i] = 0;
    }
    for (int i = 0; i< taille - 1; i++){
        do {
            a = random_(taille);
        } while (used[a] == 1);

        chaine[i] = a;
        used[a] = 1;
    }
    return chaine;
}



