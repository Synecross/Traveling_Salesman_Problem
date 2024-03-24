#include "site.h"
#include "itineraire.h"

int main(int argc, char *argv[]){

	//Coordonnées de départ
	float dep_LAT = atof(argv[1]), dep_LONG = atof(argv[2]);
	//atof permet la conversion d'une chaine de caractères en un réel, l'instruction au-dessus est équivalente à celle en dessous :
	//float dep_LAT, dep_LONG;
	//sscanf(argv[1],"%f",&dep_LAT); sscanf(argv[2],"%f",&dep_LONG); 
	//printf("Coordonnées du points de départ : (%f,%f)\n", dep_LAT, dep_LONG);

	//Lecture du fichier
	FILE *fid = fopen("sites.csv","r");
	SkipLine(fid);

	//Tableau de sites
	Site S[NB_SITES+1];
	S[0]=construireSite("Départ", dep_LAT, dep_LONG, "Unknown", "Unknown", 0);
	init_Sites(fid,S,NB_SITES+1);
	//affichageSites(S,NB_SITES+1);
	
	//Sauvegarde des distances dans une matrice
	double **matriceDistance;
	matriceDistance = (double **)malloc((NB_SITES+1)*sizeof(double *));
	for (int i=0; i<NB_SITES+1; i++)
		matriceDistance[i]=(double *)malloc((NB_SITES+1)*sizeof(double));
	init_mat_dist(matriceDistance, NB_SITES+1, S);
	//afficheMat(matriceDistance, NB_SITES+1);

	//Liste des itinéraires
	Trajet *Voyage;
	//Voyage=PlusProcheVoisin(S, matriceDistance, NB_SITES+1, DUREE);
	//Affiche_trajet(Voyage);
	Voyage=The_best(S, matriceDistance, NB_SITES+1, DUREE);
	
	//Création du fichier Tour.txt
	FILE *Tour = fopen("Tour.txt","w");
	fprintf(Tour, "%f, %f\n", dep_LAT, dep_LONG);
	Trajet *tmp = Voyage->suiv;
	while (tmp->suiv!=NULL){
		fprintf(Tour, "%f, %f, %s\n",tmp->s.LAT, tmp->s.LONG, tmp->s.categorie);
		tmp=tmp->suiv;
	}
	fprintf(Tour, "%f, %f", dep_LAT, dep_LONG);
	fclose(Tour);
	
	//Visualisation de l'itineraire via JAVA
	const char* commandLine="java -jar UnescoMap.jar";
	system(commandLine);

	//Libération de la mémoire allouée par le tableau
	libere_sites(S,NB_SITES+1);

	//Libération de la mémoire allouée pour la matrice des distances
	libere_matrice(matriceDistance,NB_SITES+1);

	//Libération de la mémoire allouée par la liste
	Supprime(&Voyage);

	//Fermeture du fichier
	fclose(fid);

	return 0;
}