#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int random_(int n);
void tableau_vide(int chaine[100][100], int n);
void tableau_rand(int chaine[100][100], int n);
void swap(int g[][100] , int x , int y, int n);

typedef struct {
    int i;
    int j;
} Index;

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
    int y[100][100];
    int c = 1;
    for (int i = 0 ; i < n ; i++)
    {
        for (int j = 0 ; j < n ; j++)
        {
            y[i][j] = c;
            c++;
        }
    }
    y[n-1][n-1]=0;
    int x;
    while (g != y)
    {
        printf("Ecrit ce que tu veut swapper :");
        scanf("%d", &x);
        swap(g,0,x,n);
        for (int i = 0; i < n ; i++) {
            for (int j = 0; j < n ; j++){
                printf("(%d) ", g[i][j]);
            }
            printf("\n");
        }
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



Index detect_index(int x, int t[][100], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (t[i][j] == x) {
                Index index = {i, j};
                return index;
            }
        }
    }
    Index invalid = {-1, -1}; // Return an invalid index if not found
    return invalid;
}

void permutation(int *x , int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void swap(int g[][100] , int x , int y, int n){
    Index a = detect_index(x, g, n);
    Index b = detect_index(y, g, n);
    if ( ( ( b.i == a.i - 1 && b.j == a.j ) || (b.i == a.i + 1 && b.j == a.j) ) || ( ( b.i == a.i  && b.j == a.j - 1 ) || (b.i == a.i && b.j == a.j + 1 ) ) )
    {
        permutation(&g[a.i][a.j], &g[b.i][b.j]);
    }
    else{
        printf("permutation non permise\n");
    }
}








