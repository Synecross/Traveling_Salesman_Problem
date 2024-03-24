#ifndef ITINERAIRE_H
#define ITINERAIRE_H

#define DUREE 504 //Durée du trajet : 3 semaines = 504 heures
#include "site.h"

typedef struct trajet{
	Site s;
	struct trajet *suiv;
}Trajet;

//Créer le premier element de la liste
Trajet *Creation(Site depart);

//Insertion en fin de liste
void InsererEnFin(Trajet *Voyage, Site s);

//Affichage des sites du Voyage
void Affiche_trajet(Trajet *Voyage);

//Libère la mémoire alouée par la liste
void Supprime(Trajet **Voyage);

//Vérifie si le pays a été visité : retourne 1 si oui, 0 sinon.
int Pays_Deja_Vu(Trajet *Voyage, char *pays);

//Met à jour les compteurs
void MAJ(Trajet *Voyage, Site S[], int i_courant, int *nb_culturals, int *nb_naturals, int *nb_mixtes, int *nb_sites_visitees, int *nb_pays, int *nb_dangers);

//Affiche le bilan du voyage
void Affiche_Bilan(Trajet *Voyage, double temps_parcouru, int nb_culturals, int nb_naturals, int nb_mixtes, int nb_sites_visitees, int nb_pays, int nb_dangers);

//Algorithme du plus proche voisin
Trajet *PlusProcheVoisin(Site S[], double **matriceDistance, int len, int duree);

//Créer l'itinéraire du voyage via les paramètres passés en argument
Trajet *OptimumRetour(Site S[], double **matriceDistance, int len, int duree,float c1,float c2,float c3, int pivot);

//Renvoie le score de l'itineraire d'un voyage
int OptimumScore(Site S[], double **matriceDistance, int len, int duree,float c1,float c2,float c3, int pivot);

//Renvoie le meilleur itineraire parmi tous les voyages testés
Trajet *The_best(Site S[], double **matriceDistance, int len, int duree);

//Remet tous les sites.visite à 0
void reset_visite(Site S[], int len);

#endif