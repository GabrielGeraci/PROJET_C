//Main du Labyrinthe
//Gabriel Daurat-Geraci
/*
Le programme que j'ai effectué ne permet pas malheureusement de jouer automatiquement contre des bots...
Je me suis arrêté à l'expansion, signifiant que je ne peux que jouer manuellement contre des bots même si c'est quand
même kiffant de défoncer ce pauvre DONTMOVE en boucle parcequ'il faut bien s'acharner sur quelqu'un parfois :D
En y réflechissant cela signifie aussi que je serais le DONTMOVE du tournoi sur qui tout le monde s'acharnera.
Fin de réfléxion, début de mon code -->
*/
#include <stdio.h>
#include <stdlib.h>
#include "labyrinthAPI.h"

//Structure qui va contenir les infos des joueurs
typedef struct {
    int posXj;
    int posYj;
    int posXbot;
    int posYbot;
    int nextTresorJ;
    int nextTresorBot;
} t_infosJoueurs;

//Structure qui va contenir les infos du labyrinthe
typedef struct {
    int tuileSupN;
    int tuileSupW;
    int tuileSupE;
    int tuileSupS;
    int hintTresor;
} t_infosLaby;

//Fonction d'initialisation du labyrinthe qui prend en paramètres les données du labyrinthe qu'on modifiera, les données des joueurs et la tuile supplémentaire
//Initialise le labyrinthe et la position des joueurs en fonction de qui doit commencer
void init(t_infosJoueurs* pJoueurs, t_infosLaby* pTuile, int tile_N, int tile_E, int tile_S, int tile_W, int tile_Objet, int start, int tailleX, int tailleY)
{
    //cas ou je commence donc ma position est en haut a gauche et celle de l'adversaire en bas a droite + trésors numérotés correctement
    if (start == 0)
    {
        pJoueurs->posXj = 0;
        pJoueurs->posYj = 0;
        pJoueurs->posXbot = tailleX-1;
        pJoueurs->posYbot = tailleY-1;
        pJoueurs->nextTresorJ = 1;
        pJoueurs->nextTresorBot = 24;
    }
    //cas ou l'adversaire commence, donc inversement
    else if (start == 1)
    {
        pJoueurs->posXbot = 0;
        pJoueurs->posYbot = 0;
        pJoueurs->posXj = tailleX-1;
        pJoueurs->posYj = tailleY-1;
        pJoueurs->nextTresorBot = 1;
        pJoueurs->nextTresorJ = 24;
    }
    //init de la tuile supplémentaire
    pTuile->tuileSupN = tile_N;
    pTuile->tuileSupW = tile_W;
    pTuile->tuileSupE = tile_E;
    pTuile->tuileSupS = tile_S;
    pTuile->hintTresor = tile_Objet;
}

//Fonction qui met à jour les données du labyrinthe
void majDonnees(t_infosJoueurs* pJoueurs, t_infosLaby* pTuile, t_move Mouvement, int Joueur, int tailleX, int tailleY, t_infosLaby Labyrinthe[tailleY][tailleX])
{
    //Si c'est à l'adversaire de jouer, ca change les positions de l'adversaire 
    if (Joueur == 0)
    {
        pJoueurs->posXbot = Mouvement.x;
        pJoueurs->posYbot = Mouvement.y;
        pJoueurs->nextTresorBot = Mouvement.nextItem;
    }
    //Sinon c'est à moi et ça change mes positions
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
    //Différents cas pour le type d'insertion (gauche droite haut bas)
    //insertion gauche ca décale vers la droite
    if (Mouvement.insert == 0)
    {
        temp=Labyrinthe[Mouvement.number][tailleX-1];
        for (int i=tailleX-1; i>0; i--)
        {
            Labyrinthe[Mouvement.number][i] = Labyrinthe[Mouvement.number][i-1];
        }
        Labyrinthe[Mouvement.number][0] = *pTuile;
    }
    //insertion droite ca décale vers la gauche
    if (Mouvement.insert == 1)
    {
        temp=Labyrinthe[Mouvement.number][0];
        for (int i=0; i>tailleX-1; i++)
        {
            Labyrinthe[Mouvement.number][i] = Labyrinthe[Mouvement.number][i+1];
        }
        Labyrinthe[Mouvement.number][0] = *pTuile;
    }
    //insertion haut ca décale vers le bas
    if (Mouvement.insert == 2)
    {
        temp=Labyrinthe[tailleY-1][Mouvement.number];
        for (int i=tailleY-1; i>0; i--)
        {
            Labyrinthe[i][Mouvement.number] = Labyrinthe[i-1][Mouvement.number];
        }
        Labyrinthe[0][Mouvement.number] = *pTuile;
    }
    //insertion bas ca décale vers le haut
    if (Mouvement.insert == 3)
    {
        temp=Labyrinthe[0][Mouvement.number];
        for (int i=0; i>tailleY-1; i++)
        {
            Labyrinthe[i][Mouvement.number] = Labyrinthe[i+1][Mouvement.number];
        }
        Labyrinthe[0][Mouvement.number] = *pTuile;
    }
    //Cas pour la rotation de la tuile sup
    //pas de rotation
    if (Mouvement.rotation == 0)
    {
        pTuile->tuileSupN = pTuile->tuileSupN;
        pTuile->tuileSupW = pTuile->tuileSupW;
        pTuile->tuileSupE = pTuile->tuileSupE;
        pTuile->tuileSupS = pTuile->tuileSupS;
    }
    //une rotation
    if (Mouvement.rotation == 1)
    {
        pTuile->tuileSupN = pTuile->tuileSupW;
        pTuile->tuileSupW = pTuile->tuileSupE;
        pTuile->tuileSupE = pTuile->tuileSupS;
        pTuile->tuileSupS = pTuile->tuileSupN;
    }
    //deux rotations
    if (Mouvement.rotation == 2)
    {
        pTuile->tuileSupN = pTuile->tuileSupE;
        pTuile->tuileSupW = pTuile->tuileSupS;
        pTuile->tuileSupE = pTuile->tuileSupN;
        pTuile->tuileSupS = pTuile->tuileSupW;
    }
    //trois rotations
    if (Mouvement.rotation == 3)
    {
        pTuile->tuileSupN = pTuile->tuileSupS;
        pTuile->tuileSupW = pTuile->tuileSupN;
        pTuile->tuileSupE = pTuile->tuileSupW;
        pTuile->tuileSupS = pTuile->tuileSupE;
    }
}

//fonction d'expansion, mon but était de trouver un chemin, pas forcément le plus proche pour être honnête.
//Au final ça ne marche pas, c'est incomplet.
//J'optais pour l'option "Je place une tuile au début aléatoire --> Vérifie si ya un chemin"
//"Si ya un chemin j'y vais, sinon je ne bouge pas de position" comme ça pas de risque pris.
//Je sais que mon raisonnement est surement pas bête mais je ne sais pas comment le faire..
/*int expansion (int tailleX, int tailleY, t_infosLaby Labyrinthe[tailleY][tailleX], int depart[2],int arrivee[2])
{
    int chemin; //ya pas de chemin si = 0 et inversement
    while(1)
    {
        //poser la tuile aléatoirement
        if (chemin == 0)
        {

        }
        //
    }
}
*/
//foncton main permettant de jouer manuellement contre le bot training dontmove et gagner logiquement si on est pas completement débile
int main()
{
    char nomLaby[20]; 
    int taille_X, taille_Y;
    int tile_N, tile_E, tile_S, tile_W, tile_Objet;
    int temp1;
    int *lab;
    t_move coupMoi;
    t_move coupAutre;
    connectToServer("172.105.76.204",1234,"GabGG");
    waitForLabyrinth("TRAINING DONTMOVE timeout=1000 seed=0x13A8D5 start=0", nomLaby, &taille_X, &taille_Y);
    lab=malloc(taille_X*taille_Y*5*sizeof(int));
    getLabyrinth(lab, &tile_N, &tile_E, &tile_S, &tile_W, &tile_Objet);
    t_infosLaby Labyrinthe[taille_Y][taille_X];
    int k=0;
    //printLabyrinth();
    printf("\n");

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
        printf("\n");
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
