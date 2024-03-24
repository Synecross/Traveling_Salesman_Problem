#ifndef SITE_H
#define SITE_H
#define NB_SITES 1052

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lectureFichiers.h"
#include "haversine.h"

typedef struct site{
	char *nom;
	float LAT;
	float LONG;
	char *categorie; //Cultural, Natural, Mixed
	char *pays;
	int enDanger; //0 si le site n'est pas en danger, 1 sinon
	int visite; //0 si le site n'a pas encore été visité, 1 sinon
}Site;

//Permet d'affecter à une structure du type Site les arguments en paramètres
Site construireSite(char *nom, float LAT, float LONG, char *categorie, char *pays, int enDanger);

//Affiche l'un des sites
void affichageSite(Site s);

//Initialise le tableau contenant les sites du fichier fid
void init_Sites(FILE *fid, Site S[], int len);

//Affiche tous les sites du tableau
void affichageSites(Site s[], int len);

//Remplit la matrice de distance
void init_mat_dist(double **matriceDistance, int len, Site sites[]);

//Affiche la matrice
void afficheMat(double **matriceDistance, int len);

//Libère la mémoire allouée pour le nom, la catégorie et le pays d'un site
void libere(Site s);

//Libère la mémoire allouée pour chaque site du tableau
void libere_sites(Site S[], int len);

//Libère la mémoire de la matrice des distances
void libere_matrice(double **matriceDistance, int len);

#endif