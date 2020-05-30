#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphemat.h"

booleen  grapheDetruit = vrai;

// remise ‡ zÈro du tableau de marquage
static void razMarque (GrapheMat* graphe)
{

    for (int i=0; i<graphe->n; i++) graphe->marque [i] = faux;
}

GrapheMat* creerGrapheMat (int nMax, int value)
{
    // allocation de graphe
    GrapheMat* graphe  = (GrapheMat*) malloc (sizeof (GrapheMat));
    graphe->n       = 0;
    graphe->nMax    = nMax;
    graphe->value   = value; // value : vrai si le graphe est valuÈ
    graphe->nomS    = (NomSom*)  malloc (sizeof(NomSom)  *nMax);
    graphe->marque  = (booleen*) malloc (sizeof(booleen) *nMax);
    graphe->element = (int*)     malloc (sizeof(int)*nMax*nMax);
    graphe->valeur  = (int*)     malloc (sizeof(int)*nMax*nMax);
    // initialisation par dÈfaut
    for (int i=0; i<nMax; i++)
    {
        for (int j=0; j<nMax; j++)
        {
            graphe->element [i*nMax+j] = faux;
            graphe->valeur  [i*nMax+j] = INFINI;
        }
    }
    razMarque (graphe);
    grapheDetruit = faux;
    return graphe;
}


// dÈsallocation d'un graphe
void detruireGraphe (GrapheMat* graphe)
{
    free (graphe->nomS);
    free (graphe->marque);
    free (graphe->element);
    free (graphe->valeur);
    free (graphe);
    grapheDetruit = vrai;
}

static int rang (GrapheMat* graphe, NomSom nom)
{
    int      i      = 0;
    booleen  trouve = faux;
    while ( (i<graphe->n) && !trouve)
    {
        trouve = strcmp (graphe->nomS [i], nom) == 0;
        if (!trouve) i++;
    }
    return trouve ? i : -1;
}

void ajouterUnSommet (GrapheMat* graphe, NomSom nom)
{
    if (rang (graphe, nom) == -1)
    {
        if (graphe->n < graphe->nMax)
        {
            strcpy (graphe->nomS [graphe->n++], nom);
        }
        else
        {
            printf ("\nNombre de sommets > %d\n", graphe->nMax);
        }
    }
    else
    {
        printf ("\n%s dÈj‡ dÈfini\n", nom);
    }
}

void ajouterUnArc (GrapheMat* graphe, NomSom somD, NomSom somA, int cout)
{
    int nMax = graphe->nMax;
    int rd = rang (graphe, somD);
    int rg = rang (graphe, somA);
    graphe->element [rd*nMax+rg] = vrai;
    graphe->valeur  [rd*nMax+rg] = cout;
}

void ecrireGraphe (GrapheMat* graphe)
{
    if (grapheDetruit == vrai)
    {
        printf ("Le graphe a ete detruit ou n'est pas encore cree\n");
    }
    else
    {
        int nMax = graphe->nMax;
        for (int i=0; i<graphe->n; i++) printf ("%s ", graphe->nomS[i]);
        printf (";\n");
        for (int i=0; i<graphe->n; i++)
        {
            printf ("\n%s : ", graphe->nomS[i]);
            for (int j=0; j<graphe->n; j++)
            {
                if (graphe->element [i*nMax+j] == vrai)
                {
                    printf ("%s ", graphe->nomS[j]) ;
                    if (graphe->value)
                    {
                        printf (" (%3d) ", graphe->valeur [i*nMax+j] );
                    }
                }
            }
            printf (";");
        }
    }
}

static void profondeur (GrapheMat* graphe, int numSommet)
{
    int nMax = graphe->nMax;
    graphe->marque [numSommet] = vrai;
    printf ("%s\n", graphe->nomS [numSommet]);
    for (int i=0; i<graphe->n; i++)
    {
        if ( (graphe->element [numSommet*nMax+i] == vrai)
                && !graphe->marque [i] )
        {
            profondeur (graphe, i);
        }
    }
}


void parcoursProfond (GrapheMat* graphe)
{
    razMarque (graphe);
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i]) profondeur (graphe, i);
    }
}

static void ecrireEtape (Matrice a, Matrice p, int k, int ns, int nMax)
{
    printf ("Passage par le sommet numÈro %d\n", k);

    for (int i=0; i<ns; i++)
    {
        for (int j=0; j<ns; j++)
        {
            if (a [i*nMax+j]==INFINI)
            {
                printf (" %3s", "*");

            }
            else
            {
                printf (" %3d", a [i*nMax+j]);
            }
        }
        printf ("%6s", " ");
        for (int j=0; j<ns; j++)
        {
            printf ("%3d", p [i*nMax+j]);
        }
        printf ("\n");
    }
    printf ("\n");
}

// initialiser les matrices a et p ‡ partir de graphe
static void initFloyd (GrapheMat* graphe, Matrice* a, Matrice* p)
{
    int nMax = graphe->nMax;

    Matrice ta = (int*) malloc (sizeof(int)*nMax*nMax);
    Matrice tp = (int*) malloc (sizeof(int)*nMax*nMax);


    for (int i=0; i<graphe->n; i++)
    {
        for (int j=0; j<graphe->n; j++)
        {
            if(i==j){
                ta [i*nMax+j]=0;
                tp [i*nMax+j] = i;
            }
            else{
                ta [i*nMax+j] = graphe->valeur [i*nMax+j];
                tp [i*nMax+j] = i;
            }
        }
    }
    *a = ta;
    *p = tp;
}

void floyd (GrapheMat* graphe)
{
    Matrice a, p;
    int n = graphe->n;
    int nMax = graphe->nMax;

    initFloyd (graphe, &a, &p);


    for (int k=0; k<n; k++)
    {
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                if ( (a [i*nMax+k] != INFINI) &&
                        (a [k*nMax+j] != INFINI) &&
                        (a [i*nMax+k] + a [k*nMax+j] < a [i*nMax+j]) )
                {
                    a [i*nMax+j] = a [i*nMax+k] + a [k*nMax+j];
                    p [i*nMax+j] = p [k*nMax+j];
                }
            }
        }
        ecrireEtape (a, p, k, n, nMax);
    }
}
