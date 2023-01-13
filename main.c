//Main du Labyrinthe
//Gabriel Daurat-Geraci

#include <stdio.h>
#include <stdlib.h>
#include "labyrinthAPI.h"

typedef struct {
    int posXj;
    int posYj;
    int posXbot;
    int posYbot;
    int nextTresorJ;
    int nextTresorBot;
} t_infosJoueurs;

typedef struct {
    int tuileSupN;
    int tuileSupW;
    int tuileSupE;
    int tuileSupS;
    int hintTresor;
} t_infosLaby;

void init(t_infosJoueurs* pJoueurs, t_infosLaby* pTuile, int tile_N, int tile_E, int tile_S, int tile_W, int tile_Objet, int start, int tailleX, int tailleY)
{
    if (start == 0)
    {
        pJoueurs->posXj = 0;
        pJoueurs->posYj = 0;
        pJoueurs->posXbot = tailleX-1;
        pJoueurs->posYbot = tailleY-1;
        pJoueurs->nextTresorJ = 1;
        pJoueurs->nextTresorBot = 24;
    }
    else if (start == 1)
    {
        pJoueurs->posXbot = 0;
        pJoueurs->posYbot = 0;
        pJoueurs->posXj = tailleX-1;
        pJoueurs->posYj = tailleY-1;
        pJoueurs->nextTresorBot = 1;
        pJoueurs->nextTresorJ = 24;
    }
    pTuile->tuileSupN = tile_N;
    pTuile->tuileSupW = tile_W;
    pTuile->tuileSupE = tile_E;
    pTuile->tuileSupS = tile_S;
    pTuile->hintTresor = tile_Objet;
}

void majDonnees(t_infosJoueurs* pJoueurs, t_infosLaby* pTuile, t_move Mouvement, int Joueur, int tailleX, int tailleY, t_infosLaby Labyrinthe[tailleY][tailleX])
{
    if (Joueur == 0)
    {
        pJoueurs->posXbot = Mouvement.x;
        pJoueurs->posYbot = Mouvement.y;
        pJoueurs->nextTresorBot = Mouvement.nextItem;
    }
    else if (Joueur == 1)
    {
        pJoueurs->posXj = Mouvement.x;
        pJoueurs->posYj = Mouvement.y;
        pJoueurs->nextTresorJ = Mouvement.nextItem;
    }
    pTuile->tuileSupN = Mouvement.tileN;
    pTuile->tuileSupW = Mouvement.tileW;
    pTuile->tuileSupE = Mouvement.tileE;
    pTuile->tuileSupS = Mouvement.tileS;
    pTuile->hintTresor = Mouvement.tileItem;
    t_infosLaby temp;
    if (Mouvement.insert == 0)
    {
        temp=Labyrinthe[Mouvement.number][tailleX-1];
        for (int i=tailleX-1; i>0; i--)
        {
            Labyrinthe[Mouvement.number][i] = Labyrinthe[Mouvement.number][i-1];
        }
        Labyrinthe[Mouvement.number][0] = *pTuile;
    }
    if (Mouvement.insert == 1)
    {
        temp=Labyrinthe[Mouvement.number][0];
        for (int i=0; i>tailleX-1; i++)
        {
            Labyrinthe[Mouvement.number][i] = Labyrinthe[Mouvement.number][i+1];
        }
        Labyrinthe[Mouvement.number][0] = *pTuile;
    }
    if (Mouvement.insert == 2)
    {
        temp=Labyrinthe[tailleY-1][Mouvement.number];
        for (int i=tailleY-1; i>0; i--)
        {
            Labyrinthe[i][Mouvement.number] = Labyrinthe[i-1][Mouvement.number];
        }
        Labyrinthe[0][Mouvement.number] = *pTuile;
    }
    if (Mouvement.insert == 3)
    {
        temp=Labyrinthe[0][Mouvement.number];
        for (int i=0; i>tailleY-1; i++)
        {
            Labyrinthe[i][Mouvement.number] = Labyrinthe[i+1][Mouvement.number];
        }
        Labyrinthe[0][Mouvement.number] = *pTuile;
    }
}

int main()
{
    char nomLaby[20]; 
    int taille_X, taille_Y;
    int tile_N, tile_E, tile_S, tile_W, tile_Objet;
    int temp1;
    int *lab;
    t_move coupMoi;
    t_move coupAutre;
    connectToServer("172.105.76.204",1234,"Gabz");
    waitForLabyrinth("TRAINING DONTMOVE timeout=1000 display=debug seed=0x13A8D5 start=0", nomLaby, &taille_X, &taille_Y);
    lab=malloc(taille_X*taille_Y*5*sizeof(int));
    getLabyrinth(lab, &tile_N, &tile_E, &tile_S, &tile_W, &tile_Objet);
    t_infosLaby Labyrinthe[taille_Y][taille_X];
    int k=0;
    printLabyrinth();
    for (int i=0; i<taille_Y; i++)
    {
        for (int j=0; j<taille_X; j++)
        {
            Labyrinthe[i][j].tuileSupN = lab[k];
            Labyrinthe[i][j].tuileSupE = lab[k+1];
            Labyrinthe[i][j].tuileSupS = lab[k+2];
            Labyrinthe[i][j].tuileSupW = lab[k+3];
            Labyrinthe[i][j].hintTresor = lab[k+4];
            k=k+5;
            printf("%d%d%d%d%d ",Labyrinthe[i][j].tuileSupN,Labyrinthe[i][j].tuileSupE,Labyrinthe[i][j].tuileSupS,Labyrinthe[i][j].tuileSupW,Labyrinthe[i][j].hintTresor);
        }
        printf("\n");
    }
    while(1)
    {
        printLabyrinth();
        printf("A moi de jouer ! Type d'insertion : ");
        scanf("%d",&temp1);
        coupMoi.insert = temp1;
        printf("Le numéro de la ligne/ou de la colonne : ");
        scanf("%d",&coupMoi.number);
        printf("La rotation (de 0 à 3) : ");
        scanf("%d",&coupMoi.rotation);
        printf("Déplacement -> coordonée en x : ");
        scanf("%d",&coupMoi.x);
        printf("Déplacement -> coordonée en y : ");
        scanf("%d",&coupMoi.y);
        sendMove(&coupMoi);
        getMove(&coupAutre);
    }
    closeConnection();
    return(0);
}
