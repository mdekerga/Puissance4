#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>

// les constantes
#define NB_LIGNES 6
#define NB_COLONNES 7
#define MILIEU NB_COLONNES / 2
#define PION_A 'X'
#define PION_B 'O'
#define VIDE ' '
#define INCONNU ' '
#define INF 1000000000
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

// les types
typedef int Grille[NB_LIGNES][NB_COLONNES];


char adversaire;

// prototypes des fonctions
void initGrille(Grille laGrille);
void afficher(Grille laGrille, char pion, int colonne);
bool grillePleine(Grille laGrille);
void jouer(Grille laGrille, char pion, int * ligne, int * colonne);
int choisirColonne(Grille laGrille, char pion,int colonne);
int chercherLigne(Grille laGrille, int col);
int alignement(Grille tab, int l, int c, int dl, int dc, char pion);
int minmax(Grille tab,char pion, int profondeur, int alpha, int beta,int lig, int col, bool max);
int maStrategie(Grille tab,char pion,int colonne, int ligne);
void JouerB(Grille tab,int *ligne,int *colonne);
bool estVainqueur(Grille laGrille, int lig, int col);
void finDePartie(char vainqueur);


// Programme principal. C'est le pion A qui commence Ã  jouer
int main()
{
    Grille laGrille;
    char vainqueur=INCONNU;
    int ligne, colonne;
    
    initGrille(laGrille);
    afficher(laGrille, PION_A, MILIEU);

    while (vainqueur==INCONNU && !grillePleine(laGrille)){
        jouer(laGrille,PION_A, &ligne, &colonne);
        afficher(laGrille, PION_B, MILIEU);
        if (estVainqueur(laGrille, ligne, colonne) ){
            vainqueur = PION_A;
        } else if (!grillePleine(laGrille)){
            jouer(laGrille,PION_B,&ligne, &colonne);
            afficher(laGrille, PION_A, MILIEU);
            if (estVainqueur(laGrille, ligne, colonne) ){
                vainqueur = PION_B;
            }
        }
    }
    finDePartie(vainqueur);
    system(EXIT_SUCCESS);
}


void initGrille(Grille laGrille){
    int l, c;
    for (l=0 ; l<NB_LIGNES ; l++){
        for (c=0 ; c<NB_COLONNES ; c++){
            laGrille[l][c] = VIDE;
        }
    }
}

void afficher(Grille laGrille, char pion, int colonne){
    int l, c, i =0;
    system("cls");
    printf("\t");

    while(i < colonne){
        printf("    ");
        i++;
    }

    printf("  %c\n", pion);
    printf("\t");
    for (c=0; c<NB_COLONNES ; c++){
        printf("----");
    }

    
    printf("-\n");
    for (l=0 ; l<NB_LIGNES ; l++){
        printf("\t");
        for (c=0; c<NB_COLONNES ; c++){
            printf("| %c ", laGrille[l][c]);
        }
        printf("|\n");
        printf("\t");
        for (c=0; c<NB_COLONNES ; c++){
            printf("----");
        }
        printf("-\n");
    }
    printf("\t");
    for (c=0; c<NB_COLONNES ; c++){
            printf("  %d ",c);
    }
    printf("\n\n");
    printf("Commandes :\nDeplacer a droite : d , Deplacer a gauche : q , Valider la colonne : Espace \n");

}

bool grillePleine(Grille laGrille){
    bool pleine = true;
    int c = 0;
    while (pleine && c<NB_COLONNES){
        if (laGrille[0][c] == VIDE){
            pleine = false;
        }
        c++;
    }
    return pleine;
}

void jouer(Grille laGrille, char pion, int * ligne, int * colonne ){
   *ligne = -1;
    do {
        *colonne = choisirColonne(laGrille, pion, MILIEU);
        *ligne = chercherLigne(laGrille, *colonne);
    }while (*ligne==-1);
    laGrille[*ligne][*colonne] = pion;
}

int choisirColonne(Grille g,char pion,int colonne)
{
    int poscol;
    char appuie;
    poscol = MILIEU;
        while(appuie != ' ')
        {
            scanf("%c",&appuie);

            if(appuie == 'd')
            {
                poscol++;
                if(poscol > NB_LIGNES)
                {
                    poscol = 0;

                }           
            }
            else if(appuie == 'q')
            {
                poscol--;
                if(poscol < 0)
                {
                    poscol = NB_COLONNES;
                } 

            }
        afficher(g,pion,poscol);
        }
    return poscol;
}

int chercherLigne(Grille laGrille, int col){
    int ligne = -1;

    while (ligne<NB_LIGNES-1 && laGrille[ligne+1][col]==VIDE){
        ligne++;
    }
    return ligne;
}

bool estVainqueur(Grille laGrille, int lig, int col){
    // consiste Ã  regarder si une ligne de 4 pions s'est formÃ©e autour du pion qui vient de tomber en (lig, col)
    char lePion = laGrille[lig][col];
    int cpt,i,j;

    // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
    i = lig;
    cpt = 0;
    while (i<NB_LIGNES && laGrille[i][col]==lePion){
        cpt++;
        i++;
    }
    if (cpt>=4){
        return true;
    }
    // regarder l'horizontale, en comptant le nombre de pions Ã  l'Est et Ã  l'Ouest
    j = col;
    cpt = 0;
    // on regarde Ã  l'est
    while (j>=0 && laGrille[lig][j]==lePion){
        cpt++;
        j--;
    }
    j = col+1;
    // on regarde Ã  l'ouest
    while (j<NB_COLONNES && laGrille[lig][j]==lePion){
        cpt++;
        j++;
    }
    if (cpt>=4 ){
        return true;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Ouest
    while (j>=0 && i>=0 && laGrille[i][j]==lePion){
        cpt++;
        i--;
        j--;
    }
    i = lig+1;
    j = col+1;
    // on regarde au Sud Est
    while (i<NB_LIGNES && j<NB_COLONNES && laGrille[i][j]==lePion){
        cpt++;
        i++;
        j++;
    }
    if (cpt>=4 ){
        return true;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Est
    while (j<NB_COLONNES && i>=0 && laGrille[i][j]==lePion){
        cpt++;
        i--;
        j++;
    }
    i = lig+1;
    j = col-1;
    // on regarde au Sud Ouest
    while (i<NB_LIGNES && j>=0 && laGrille[i][j]==lePion){
        cpt++;
        i++;
        j--;
    }
    if (cpt>=4 ){
        return true;
    }
    return false;
}

void finDePartie(char vainqueur){
    if (vainqueur != INCONNU){
        printf("Joueur %c vainqueur\n", vainqueur);
    } else {
        printf("MATCH NUL");
    }
}


int alignement(Grille tab, int l, int c, int dl, int dc,char pion){
    if(l+dl < 0 || dl >= NB_LIGNES || c + dc < 0 || c + dc >= NB_COLONNES || tab[l+dl][c+dc] != pion){
        return 0;
    }
        
    return 1 + alignement(tab, l + dl, c + dc, dl ,dc ,pion); 
}

    