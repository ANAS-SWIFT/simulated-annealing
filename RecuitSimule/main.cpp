//
//  main.cpp
//  RecuitSimule
//
//  Created by ANAS on 5/14/20.
//  Copyright © 2020 ANAS. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graphemat.h"
#include "liregraphe.h"
#include <math.h>

typedef GrapheMat Graphe;
NomSom path[]= {"A", "B", "C", "D" ,"E"};
int parcours[5];
int hasard()
{
    int randnb = rand() % 5;
    return randnb;
}

NomSom* parcours_hasard(Graphe* graphe)
{
    NomSom* trajet=(NomSom*)malloc(5*sizeof(NomSom));
    int pos = hasard();
    for(int j=0; j<graphe->n; j++)
    {
        if(pos+j<5)
        {
            printf("path[%d]=%s \t",pos+j, path[pos+j]);
            strcpy((char*)trajet[j],(char*)path[pos+j]);
            printf("trajet[%d]=%s \n",j,trajet[j]);
        }
        else
        {
            printf("path[%d]=%s \t",5-pos-j, path[5-pos-j]);
            strcpy((char*)trajet[j],(char*)path[5-pos-j]);
            printf("trajet[%d]=%s \n",j,trajet[j]);

        }
    }
    return trajet;
}

int nombreVilles(Graphe* graphe)
{
    return graphe->n;
}

int cout_parcours(NomSom* trajet,Graphe* graphe)
{
    int pos1,pos2,i,j,k;
    int nMax=5;
    int d=0;
    for(j=0; j<5; j++)
    {
        for(i=0; i<5; i++)
        {

            if(strcasecmp(graphe->nomS[i],trajet[j])==0)
            {
                pos1=i;
                break;
            }
        }
        for(k=0; k<5; k++)
        {
            if(strcasecmp(graphe->nomS[k],trajet[j+1])==0)
            {
                pos2=k;
                break;
            }
        }
        if(pos1==i && pos2==k)
        {
            if(graphe->element [pos1*20+pos2]==vrai)
            {
                d+=graphe->valeur[pos1*20+pos2];
            }
        }
    }
    printf("\n cout %d", d);
    return d;
}



NomSom* parcours_2opt(NomSom* trajet,int i , int k)
{
    int dec=0;
    NomSom* parcours2=(NomSom*)malloc(5*sizeof(NomSom));
    for(int j=0; j<=i-1 ; j++)
    {
        strcpy((char*)parcours2[j],(char*)trajet[j]);
        printf(" %s ->", parcours2[j] );
    }
    for(int j=i; j<=k ; j++)
    {
        strcpy(parcours2[j],trajet[k-dec]);
        dec++;
        printf("%s -> ", parcours2[j] );
    }
    for(int j=k+1; j<5 ; j++)
    {
        strcpy(parcours2[j],trajet[j]);
        printf("%s ->", parcours2[j] );
    }
    return parcours2;
}

void escalade(Graphe* graphe)
{
    booleen trouve=false;
    NomSom* courant=parcours_hasard(graphe);
    while(!trouve)
    {
        int i=1;
        int k=3;
        if(i<k)
        {
            NomSom* voisin=parcours_2opt(courant,i ,k);
            if(cout_parcours(voisin,graphe)<cout_parcours(courant,graphe))
                courant=voisin;
            trouve=true;
        }
        else
        {
            NomSom* voisin=parcours_2opt(courant,k ,i);
            if(cout_parcours(voisin,graphe)<cout_parcours(courant,graphe))
                courant=voisin;
            trouve=true;
        }

    }
    printf("\n Chemin moins couteux -> ");
    for (int i=0; i<5; i++)
    {
        printf(" %s ->", courant[i]);
    }
}
void recuit_simule(Graphe* graphe)
{
    int tmax=1000;
    int tinitial=0;
    double fact=0.93;
    NomSom* T1=parcours_hasard(graphe);
    while(tmax>tinitial)
    {
        int i=1;
        int k=3;
        if(i<k)
        {
            NomSom* T2=parcours_2opt(T1,i ,k);
            if(cout_parcours(T2,graphe)<cout_parcours(T1,graphe))
                T1=T2;
            else
            {
                double nombre ;
                nombre= ( rand()/(double)RAND_MAX ) * 2;
                printf("\n %f", nombre);
                if(nombre<=exp(-(cout_parcours(T2,graphe)-cout_parcours(T1,graphe))/tmax))
                    T1=T2;
            }

        }
        else
        {
            NomSom* T2=parcours_2opt(T1,k ,i);
            if(cout_parcours(T2,graphe)<cout_parcours(T1,graphe))
                T1=T2;
            else
            {
                double nombre ;
                nombre= ( rand()/(double)RAND_MAX ) * 2;
                printf("%f", nombre);
                if(nombre<=exp(-(cout_parcours(T2,graphe)-cout_parcours(T1,graphe))/tmax))
                    T1=T2;
            }

        }
        tmax=tmax*fact;


    }
    printf("\n Chemin moins couteux -> ");
    for (int i=0; i<5; i++)
    {
        printf(" %s ->", T1[i]);
    }
}



int menu ()
{

    printf ("\n\nGRAPHES avec matrices\n\n");

    printf ("0 - Fin du programme\n");
    printf ("1 - Liste des sommets et des arcs\n");
    printf ("2 - Escalade\n");
    printf ("3 - Recuit simule\n");
    printf ("\n");
    printf ("Votre choix ? ");
    int cod;
    scanf ("%d", &cod);
    getchar();
    printf ("\n");
    return cod;
}
int main ()
{
    srand(time(NULL));
    Graphe* graphe;
    booleen fini = faux;
    char nomFe [50];
    //scanf  ("%s", nomFe);
    strcpy (nomFe, "/Users/mac/Desktop/RecuitSimule/RecuitSimule/graphe2.txt");
    printf ("Nom du fichier contenant le graphe <%s> ", nomFe);

    FILE* fe = fopen (nomFe, "r");
    if (fe == NULL)
    {
        perror (nomFe);
    }
    else
    {
        graphe = lireGraphe (fe, 20);
        fclose (fe);
    }

    while (!fini)
    {
        switch ( menu() )
        {
        case 0:
            fini = vrai;
            detruireGraphe (graphe);
            break;
        case 1:
        {
            ecrireGraphe (graphe);
            break;
        }
        break;
        case 2:
        {
            if(graphe->value)
                escalade(graphe);
        }
        break;
        case 3:
        {
            if(graphe->value)
                recuit_simule(graphe);
            else
                printf ("Graphe non valuÈ\n");
        }
        break;

        }   // switch
        if (!fini)
        {
            printf ("\n\nTaper Return pour continuer\n");
            getchar();
        }
    }

    return 0;
}
