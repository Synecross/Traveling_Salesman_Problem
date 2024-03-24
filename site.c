#include "site.h"

Site construireSite(char *nom, float LAT, float LONG, char *categorie, char *pays, int enDanger){
	
	Site s;
	s.nom = strdup(nom); // alloue de la mémoire et copie la chaine de caractères
	//s.nom = (char *)malloc((strlen(nom)+1)*sizeof(char));
	//strcpy(s.nom,nom);

	s.LAT = LAT;
	s.LONG = LONG;

	s.categorie = strdup(categorie);
	//s.categorie = (char *)malloc((strlen(categorie)+1)*sizeof(char));
	//strcpy(s.categorie,categorie);

	s.pays = strdup(pays);
	//s.pays = (char*)malloc((strlen(pays)+1)*sizeof(char));
	//strcpy(s.pays,pays);

	s.enDanger = enDanger;

	s.visite = 0;

	return s;
}

void affichageSite(Site s){
	//Affiche l'un des sites
	printf("%s %.2f %.2f %s %s %d\n",s.nom,s.LAT,s.LONG,s.categorie,s.pays, s.enDanger);
}

void init_Sites(FILE *fid, Site S[], int len){
	//Initialise le tableau contenant les sites du fichier fid
	char nom[250], categorie[250], pays[250], continent[250]; //variables de stockage := tableau assez grand : taille 250 arbitraire
	float LAT, LONG;
	int enDanger;

	for (int i=1; i<len; i++){ // on commence à 1, car le premier site c'est le point de départ

		GetChaine(fid,250,nom); //lit toutes les caractères avant une virgule

		Skip(fid); //skip la virgule séparatrice
		fscanf(fid, "%f", &LAT); //lit un réel

		Skip(fid);
		fscanf(fid, "%f", &LONG); 

		GetChaine(fid,250,categorie);

		GetChaine(fid,250,pays);

		GetChaine(fid,250,continent);
		
		Skip(fid); //skip la virgule séparatrice
		fscanf(fid, "%d", &enDanger);

		//Initialisation du tableau :
		S[i]=construireSite(nom,LAT,LONG,categorie, pays, enDanger);
	}
}

void affichageSites(Site S[], int len){
	//Affiche tous les sites du tableau
	for (int i=0; i<len; i++)
		affichageSite(S[i]);
}

void init_mat_dist(double **matriceDistance, int len, Site S[]){
	//Remplit la matrice de distance
	for (int i=0; i<len; i++)
		for (int j=0; j<len; j++)
			matriceDistance[i][j] = calculDistance(S[i].LAT, S[i].LONG, S[j].LAT, S[j].LONG);
}

void afficheMat(double **matriceDistance, int len){
	//Affiche la matrice
	for (int i=0; i<len; i++){
		for (int j=0; j<len; j++)
			printf("%.2f\t",matriceDistance[i][j]);
		printf("\n");
	}
}

void libere(Site s){
	//Libère la mémoire allouée pour le nom, la catégorie et le pays d'un site
	free(s.nom);
	free(s.categorie);
	free(s.pays);
}

void libere_sites(Site S[], int len){
	//Libère la mémoire allouée pour chaque site du tableau
	for (int i=0; i<len; i++)
		libere(S[i]);
}

void libere_matrice(double **matriceDistance, int len){
	//Libère la mémoire de la matrice des distances
	for (int j=0; j<len; j++)
		free(matriceDistance[j]);
	free(matriceDistance);
}
