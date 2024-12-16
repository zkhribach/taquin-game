#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int random_(int n);
void tableau_vide(int chaine[100][100], int n);
void tableau_rand(int chaine[100][100], int n);

int main(){
    srand( time( 0 ) );
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
    int g[100][100];
    tableau_vide(g,n);
    tableau_rand(g,n);
    printf("Generated grid:\n");
    for (int i = 0; i < n ; i++) {
        for (int j = 0; j < n ; j++){
            printf("(%d) ", g[i][j]);
        }
        printf("\n");
    }
    return 0;
}
int random_(int n) {
    int searchedValue = (rand() % n - 1) + 1;
    return searchedValue;
}

void tableau_vide(int chaine[100][100], int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            chaine[i][j] = 0;
        }
    }
}

void tableau_rand(int chaine[100][100], int n){
    int taille = n * n;
    int a;
    int used[taille];
    for (int i = 0; i< taille; i++){
        used[i] = 0;
    }
    used[0]= 1;
    for (int i = 0; i< n  ; i++){
        for (int j = 0; j < n ; j++){
            if (i == n - 1 && j == n - 1){
                continue;
            }
            do {
                a = random_(taille);
            } while (used[a] == 1);
            chaine[i][j] = a;
            used[a] = 1;
        }
    }
}










