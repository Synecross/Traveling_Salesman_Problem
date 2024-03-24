#include "itineraire.h"

Trajet *Creation(Site depart){
	//Créer le premier element de la liste
	Trajet *nouveau;
	nouveau = (Trajet*)malloc(sizeof(Trajet));
	nouveau->s=depart;
	nouveau->suiv=NULL;
	return nouveau;
}

void InsererEnFin(Trajet *Voyage, Site s){
	//Insertion en fin de liste

	//Creation d'un nouveau itineraire
	Trajet *nouveau;
	nouveau = (Trajet*)malloc(sizeof(Trajet));
	nouveau->s = s;
	nouveau->suiv=NULL;
	
	if (Voyage==NULL)
		Voyage = nouveau;

	Trajet *tmp=Voyage;
	while (tmp->suiv!=NULL)
		tmp=tmp->suiv;
	tmp->suiv=nouveau;
}

void Affiche_trajet(Trajet *Voyage){
	//Affichage des sites du Voyage
	int cpt = 0; //compteur d'itineraires
	Trajet *courant = Voyage;
	while (courant){
		printf("%d) ",cpt++);
		affichageSite(courant->s);
		courant = courant->suiv;
	}
}

void Supprime(Trajet **Voyage){
	//Libère la mémoire alouée par la liste
	Trajet *courant = *Voyage;
	while (*Voyage){
		courant = courant->suiv;
		free(*Voyage);
		*Voyage=courant;
	}
}

int Pays_Deja_Vu(Trajet *Voyage, char *pays){
	//Vérifie si le pays a été visité : retourne 1 si oui, 0 sinon.
	Trajet *tmp = Voyage;
	while (tmp!=NULL){
		if (strcmp(tmp->s.pays,pays)==0) //les deux chaines sont identiques : le pays est déjà présent dans le trajet du voyage
			return 1; 
		tmp=tmp->suiv;
	}
	return 0;
}

void MAJ(Trajet *Voyage, Site S[], int i_courant, int *nb_culturals, int *nb_naturals, int *nb_mixtes, int *nb_sites_visitees, int *nb_pays, int *nb_dangers){
	//Met à jour les compteurs

	if (Pays_Deja_Vu(Voyage, S[i_courant].pays)==0)
		*nb_pays+=1;

	if (strcmp(S[i_courant].categorie,"Mixed")==0)
		*nb_mixtes+=1;
	else if (strcmp(S[i_courant].categorie,"Cultural")==0)
		*nb_culturals+=1;
	else //Le site est naturel
		*nb_naturals+=1;

	if (S[i_courant].enDanger==1)
		*nb_dangers+=1;

	*nb_sites_visitees+=1;
}

void Affiche_Bilan(Trajet *Voyage, double temps_parcouru, int nb_culturals, int nb_naturals, int nb_mixtes, int nb_sites_visitees, int nb_pays, int nb_dangers){
	//Affiche le bilan du Voyage
	printf("Challenge Unesco\n");
	printf("Temps total : %f\n",temps_parcouru);
	printf("Nombre de sites naturels = %d\n",nb_naturals);
	printf("Nombre de sites culturels = %d\n", nb_culturals);
	printf("Nombre de sites mixtes = %d\n", nb_mixtes);
	printf("ITINERAIRE DE VOYAGE :\n");

	Affiche_trajet(Voyage);

	printf("Evaluation :\n");
	printf("\t%d sites visités * 10 points = %d\n",nb_sites_visitees,nb_sites_visitees*10);
	printf("\t%d pays visités * 20 points = %d\n", nb_pays, nb_pays*20);
	printf("\t%d sites en danger visités * 30 points = %d\n",nb_dangers, nb_dangers*30);
	printf("Score total = %d\n", nb_sites_visitees*10+nb_pays*20+nb_dangers*30);
	
}

Trajet *PlusProcheVoisin(Site S[], double **matriceDistance, int len, int duree){
	//Algorithme du plus proche voisin

	Trajet *Voyage;
	Voyage=Creation(S[0]); // Point de départ

	int i_voisin = 0; //Indice du plus proche site voisin dans le tableau de sites S
	double d_voisin =0; //Distance (matriceDistance) correspondant par rapport au i_courant

	int i_courant=0; // Indice du site où il se trouve en ce moment, on commence au point départ
	char *categorie = S[0].categorie; //Commence par "Unknown", puis prend la dernière catégorie différente de mixed pour vérifier l'alternance entre naturel et culturel

	int fin = 0; // 1 si le voyage est fini et qu'on est bien revenu au point de départ 
	double temps_parcouru = 0;

	//Compteurs
	int nb_culturals=0;
	int nb_naturals=0;
	int nb_mixtes=0;
	int nb_sites_visitees = 0;
	int nb_pays=0;
	int nb_dangers=0;

	while (fin==0){
		//Recherche du plus proche voisin avec une catégorie différente de la catégorie courante

		d_voisin = 20000; //nombre très grand pour permettre ensuite de trouver la distance minimale

		for (int i=1; i<len; i++){ //on commence à 1, car la distance par rapport au point de départ nous intéresse pas
			if (i!=i_courant && S[i].visite==0 && (strcmp(categorie,S[i].categorie)!=0)){
				//Si le site n'a pas encore été visité et qu'il est de catégorie différente par rapport au dernier culturel/naturel, alors :
				if(matriceDistance[i_courant][i]<d_voisin){
					//Si le site est plus proche :
					d_voisin = matriceDistance[i_courant][i];
					i_voisin = i;
				}
			}
		}

		//L'avion se déplace à 85 km/h
		//On suppose qu'on passe 7 heures par site
		if (i_courant!=i_voisin && (duree - temps_parcouru - (matriceDistance[i_voisin][0]/85 + 7 + matriceDistance[i_voisin][i_courant]/85)) >= 0){
			//Il y a assez de temps pour visiter un autre site et retourner au point de départ à partir de ce site
			temps_parcouru += (matriceDistance[i_courant][i_voisin]/85 + 7);
			i_courant = i_voisin;

			//Mise à jour des compteurs
			MAJ(Voyage, S, i_courant, &nb_culturals, &nb_naturals, &nb_mixtes, &nb_sites_visitees, &nb_pays, &nb_dangers);

			//Insertion du site à visiter dans l'itinéraire du Voyage
			InsererEnFin(Voyage, S[i_courant]);
			S[i_courant].visite = 1;

			if (strcmp(S[i_courant].categorie,"Mixed")!=0)
				//On conserve la dernière catégorie différente de mixte, on change la categorie de comparaison pour garder le bon décompte de sites naturels/culturels visités
				categorie = S[i_courant].categorie;

		}else{ 
			//Pas assez de temps pour faire une nouvelle visite donc on retourne directement au point de départ
			InsererEnFin(Voyage, S[0]); // Insertion du point d'arrivé := départ
			temps_parcouru += (matriceDistance[i_courant][0]/85); //Ajout du temps parcouru pour aller du site courant au point dé départ
			fin=1;
		}
	}

	//Affiche le bilan du voyage :
	Affiche_Bilan(Voyage, temps_parcouru, nb_culturals, nb_naturals, nb_mixtes, nb_sites_visitees, nb_pays, nb_dangers);
	return Voyage;
}

Trajet *OptimumRetour(Site S[], double **matriceDistance, int len, int duree, float c1,float c2,float c3, int pivot){
	//Créer l'itinéraire du voyage via les paramètres passés en argument

	Trajet *Voyage;
	Voyage=Creation(S[0]); // Point de départ

	int i_voisin = 0; // 10 pts : plus proche voisin
	int i_pays = 0; // 30 pts : plus proche site voisin d'un pays voisin différent
	int i_voisin_ed = 0; // 40 pts : plus proche site en danger voisin
	int i_pays_ed = 0; // 60 pts : plus proche site en danger d'un pays voisin différent

	//Distances correspondants
	double d_voisin;
	double d_pays;
	double d_voisin_ed;
	double d_pays_ed;

	int i_courant=0; //Indice du site où il se trouve en ce moment, on commence au point départ
	char *categorie = S[0].categorie; //Commence par "Unknown", puis prend la dernière catégorie différente de mixed pour vérifier l'alternance entre naturel et culturel

	int i_new; //Indice du prochain site à visiter

	int fin = 0; // 1 si le voyage est fini et qu'on est bien revenu au point de départ 
	double temps_parcouru = 0;

	//Compteurs
	int nb_culturals=0;
	int nb_naturals=0;
	int nb_mixtes=0;
	int nb_sites_visitees = 0;
	int nb_pays=0;
	int nb_dangers=0;

	while (fin==0){
		d_voisin = 20000;
		d_pays = 20000;
		d_voisin_ed = 20000;
		d_pays_ed = 20000;

		for (int i=1; i<len; i++){ //on commence à 1, car la distance par rapport au point de départ nous intéresse pas
			if (i!=i_courant && S[i].visite==0 && (strcmp(categorie,S[i].categorie)!=0)){
				//Site non visite et catégorie différent
				if(matriceDistance[i_courant][i]<d_voisin ){
					//Plus proche site voisin
					d_voisin = matriceDistance[i_courant][i];
					i_voisin = i;
				}
				if(matriceDistance[i_courant][i]<d_voisin_ed && S[i].enDanger==1){
					//Plus proche site en danger voisin
					d_voisin_ed = matriceDistance[i_courant][i];
					i_voisin_ed = i;
				}

				if (Pays_Deja_Vu(Voyage,S[i].pays)==0){
					if(matriceDistance[i_courant][i]<d_pays){
						//Plus proche site voisin d'un pays voisin différent
						d_pays = matriceDistance[i_courant][i];
						i_pays = i;
					}
					if(matriceDistance[i_courant][i]<d_pays_ed && S[i].enDanger==1){
						//Plus proche site en danger d'un pays voisin différent
						d_pays_ed = matriceDistance[i_courant][i];
						i_pays_ed = i;
					}
				}
			}
		}

		//A partir d'un certain temps (pivot) du parcours, on ajoute des restrictions sur la recherche, i.e. on fait de sorte qu'on revient petit à petit vers le point de départ
		if (temps_parcouru > pivot){
			d_voisin = 20000;
			d_pays = 20000;
			d_voisin_ed = 20000;
			d_pays_ed = 20000;

			for (int i=1; i<len; i++){ //on commence à 1, car la distance par rapport au point de départ nous intéresse pas
				if (matriceDistance[i][0]<matriceDistance[i_courant][0]){ 
				//Le site appartient à un périmètre du départ défini par la distance entre le point de départ et le site courant
					if ((duree - temps_parcouru - (matriceDistance[i][0]/85 + 7 + matriceDistance[i][i_courant]/85)) >= 0){
					//On vérifie qu'on a bien le temps d'aller visiter le site et retourner au départ
						if (i!=i_courant && S[i].visite==0 && (strcmp(categorie,S[i].categorie)!=0)){
							//Site non visite et catégorie différent
							if(matriceDistance[i_courant][i]<d_voisin ){
								//Plus proche site voisin
								d_voisin = matriceDistance[i_courant][i];
								i_voisin = i;
							}
							if(matriceDistance[i_courant][i]<d_voisin_ed && S[i].enDanger==1){
								//Plus proche site en danger voisin
								d_voisin_ed = matriceDistance[i_courant][i];
								i_voisin_ed = i;
							}

							if (Pays_Deja_Vu(Voyage,S[i].pays)==0){
								if(matriceDistance[i_courant][i]<d_pays){
									//Plus proche site voisin d'un pays voisin différent
									d_pays = matriceDistance[i_courant][i];
									i_pays = i;
								}
								if(matriceDistance[i_courant][i]<d_pays_ed && S[i].enDanger==1){
									//Plus proche site en danger d'un pays voisin différent
									d_pays_ed = matriceDistance[i_courant][i];
									i_pays_ed = i;
								}
							}
						}
					}
				}
			}
		}

		//Tableau contenant les 4 cas possibles de site à visiter
		int ind[4]={i_voisin, i_pays, i_voisin_ed, i_pays_ed};

		//Tableaux de points gagnés pour chacun des cas
		int Scores[4] = {10, 30, 40, 60};

		//L'indice du site à visiter qui apporte le plus de points et dont le parcours est rentable
		int i_opt = 0; //i_opt prend par défaut le plus proche site voisin
		double t_opt = matriceDistance[ind[i_opt]][i_courant]/85+7;

		//Coefficients pour paramétrer la priorité des cas à visiter
		double coef[4]={1, c1, c2, c3};

		for (int i=1; i<4; i++){
			if (i_courant!=ind[i] && (duree - temps_parcouru - (matriceDistance[ind[i]][0]/85 + 7 + matriceDistance[ind[i]][i_courant]/85)) >= 0){
			//On vérifie que les autres cas sont envisageables à visiter
				double t_i = matriceDistance[ind[i]][i_courant]/85+7;
				//Comparaison avec le meilleur site à visiter gardé dans i_opt
				if (coef[i]*t_i < Scores[i]/Scores[i_opt] * t_opt){
					//Prend l'indice du site à visiter ayant le plus grand score par rapport au coefficient de priorité
					t_opt = t_i;
					i_opt = i;
				}
			}
		}

		//Affectation de l'indice du site à visiter après avoir tout testé 
		i_new = ind[i_opt];

		//L'avion se déplace à 85 km/h
		//On suppose qu'on passe 7 heures par site
		if (i_courant!=i_new && (duree - temps_parcouru - (matriceDistance[i_new][0]/85 + 7 + matriceDistance[i_new][i_courant]/85)) >= 0){
			//Il y a assez de temps pour visiter un autre site et retourner au point de départ à partir de ce site
			temps_parcouru += (matriceDistance[i_courant][i_new]/85 + 7);
			i_courant = i_new;

			//Mise à jour des compteurs
			MAJ(Voyage, S, i_courant, &nb_culturals, &nb_naturals, &nb_mixtes, &nb_sites_visitees, &nb_pays, &nb_dangers);

			//Insertion du site à visiter dans l'itinéraire du Voyage
			InsererEnFin(Voyage, S[i_courant]);
			S[i_courant].visite = 1;

			if (strcmp(S[i_courant].categorie,"Mixed")!=0)
				//On conserve la dernière catégorie différente de mixte, on change la categorie de comparaison pour garder le bon décompte de sites naturels/culturels visités
				categorie = S[i_courant].categorie;

		}else{ 
			//Pas assez de temps pour faire une nouvelle visite donc on retourne directement au point de départ
			InsererEnFin(Voyage, S[0]); // Insertion du point d'arrivé := départ
			temps_parcouru += (matriceDistance[i_courant][0]/85); //Ajout du temps parcouru pour aller du site courant au point dé départ
			fin=1;
		}
	}

	//Affiche le bilan du voyage :
	Affiche_Bilan(Voyage, temps_parcouru, nb_culturals, nb_naturals, nb_mixtes, nb_sites_visitees, nb_pays, nb_dangers);
	return Voyage;
}

int OptimumScore(Site S[], double **matriceDistance, int len, int duree, float c1,float c2,float c3, int pivot){
	//Renvoie le score de l'itineraire d'un voyage

	Trajet *Voyage;
	Voyage=Creation(S[0]); // Point de départ

	int i_voisin = 0; // 10 pts : plus proche voisin
	int i_pays = 0; // 30 pts : plus proche site voisin d'un pays voisin différent
	int i_voisin_ed = 0; // 40 pts : plus proche site en danger voisin
	int i_pays_ed = 0; // 60 pts : plus proche site en danger d'un pays voisin différent

	//Distances correspondants
	double d_voisin;
	double d_pays;
	double d_voisin_ed;
	double d_pays_ed;

	int i_courant=0; //Indice du site où il se trouve en ce moment, on commence au point départ
	char *categorie = S[0].categorie; //Commence par "Unknown", puis prend la dernière catégorie différente de mixed pour vérifier l'alternance entre naturel et culturel

	int i_new; //Indice du prochain site à visiter

	int fin = 0; // 1 si le voyage est fini et qu'on est bien revenu au point de départ 
	double temps_parcouru = 0;

	//Compteurs
	int nb_culturals=0;
	int nb_naturals=0;
	int nb_mixtes=0;
	int nb_sites_visitees = 0;
	int nb_pays=0;
	int nb_dangers=0;

	while (fin==0){
		d_voisin = 20000;
		d_pays = 20000;
		d_voisin_ed = 20000;
		d_pays_ed = 20000;

		for (int i=1; i<len; i++){ //on commence à 1, car la distance par rapport au point de départ nous intéresse pas
			if (i!=i_courant && S[i].visite==0 && (strcmp(categorie,S[i].categorie)!=0)){
				//Site non visite et catégorie différent
				if(matriceDistance[i_courant][i]<d_voisin ){
					//Plus proche site voisin
					d_voisin = matriceDistance[i_courant][i];
					i_voisin = i;
				}
				if(matriceDistance[i_courant][i]<d_voisin_ed && S[i].enDanger==1){
					//Plus proche site en danger voisin
					d_voisin_ed = matriceDistance[i_courant][i];
					i_voisin_ed = i;
				}

				if (Pays_Deja_Vu(Voyage,S[i].pays)==0){
					if(matriceDistance[i_courant][i]<d_pays){
						//Plus proche site voisin d'un pays voisin différent
						d_pays = matriceDistance[i_courant][i];
						i_pays = i;
					}
					if(matriceDistance[i_courant][i]<d_pays_ed && S[i].enDanger==1){
						//Plus proche site en danger d'un pays voisin différent
						d_pays_ed = matriceDistance[i_courant][i];
						i_pays_ed = i;
					}
				}
			}
		}

		//A partir d'un certain temps (pivot) du parcours, on ajoute des restrictions sur la recherche, i.e. on fait de sorte qu'on revient petit à petit vers le point de départ
		if (temps_parcouru > pivot){
			d_voisin = 20000;
			d_pays = 20000;
			d_voisin_ed = 20000;
			d_pays_ed = 20000;

			for (int i=1; i<len; i++){ //on commence à 1, car la distance par rapport au point de départ nous intéresse pas
				if (matriceDistance[i][0]<matriceDistance[i_courant][0]){ 
				//Le site appartient à un périmètre du départ défini par la distance entre le point de départ et le site courant
					if ((duree - temps_parcouru - (matriceDistance[i][0]/85 + 7 + matriceDistance[i][i_courant]/85)) >= 0){
					//On vérifie qu'on a bien le temps d'aller visiter le site et retourner au départ
						if (i!=i_courant && S[i].visite==0 && (strcmp(categorie,S[i].categorie)!=0)){
							//Site non visite et catégorie différent
							if(matriceDistance[i_courant][i]<d_voisin ){
								//Plus proche site voisin
								d_voisin = matriceDistance[i_courant][i];
								i_voisin = i;
							}
							if(matriceDistance[i_courant][i]<d_voisin_ed && S[i].enDanger==1){
								//Plus proche site en danger voisin
								d_voisin_ed = matriceDistance[i_courant][i];
								i_voisin_ed = i;
							}

							if (Pays_Deja_Vu(Voyage,S[i].pays)==0){
								if(matriceDistance[i_courant][i]<d_pays){
									//Plus proche site voisin d'un pays voisin différent
									d_pays = matriceDistance[i_courant][i];
									i_pays = i;
								}
								if(matriceDistance[i_courant][i]<d_pays_ed && S[i].enDanger==1){
									//Plus proche site en danger d'un pays voisin différent
									d_pays_ed = matriceDistance[i_courant][i];
									i_pays_ed = i;
								}
							}
						}
					}
				}
			}
		}

		//Tableau contenant les 4 cas possibles de site à visiter
		int ind[4]={i_voisin, i_pays, i_voisin_ed, i_pays_ed};

		//Tableaux de points gagnés pour chacun des cas
		int Scores[4] = {10, 30, 40, 60};

		//L'indice du site à visiter qui apporte le plus de points et dont le parcours est rentable
		int i_opt = 0; //i_opt prend par défaut le plus proche site voisin
		double t_opt = matriceDistance[ind[i_opt]][i_courant]/85+7;

		//Coefficients pour paramétrer la priorité des cas à visiter
		double coef[4]={1, c1, c2, c3};

		for (int i=1; i<4; i++){
			if (i_courant!=ind[i] && (duree - temps_parcouru - (matriceDistance[ind[i]][0]/85 + 7 + matriceDistance[ind[i]][i_courant]/85)) >= 0){
			//On vérifie que les autres cas sont envisageables à visiter
				double t_i = matriceDistance[ind[i]][i_courant]/85+7;
				//Comparaison avec le meilleur site à visiter gardé dans i_opt
				if (coef[i]*t_i < Scores[i]/Scores[i_opt] * t_opt){
					//Prend l'indice du site à visiter ayant le plus grand score par rapport au coefficient de priorité
					t_opt = t_i;
					i_opt = i;
				}
			}
		}

		//Affectation de l'indice du site à visiter après avoir tout testé 
		i_new = ind[i_opt];

		//L'avion se déplace à 85 km/h
		//On suppose qu'on passe 7 heures par site
		if (i_courant!=i_new && (duree - temps_parcouru - (matriceDistance[i_new][0]/85 + 7 + matriceDistance[i_new][i_courant]/85)) >= 0){
			//Il y a assez de temps pour visiter un autre site et retourner au point de départ à partir de ce site
			temps_parcouru += (matriceDistance[i_courant][i_new]/85 + 7);
			i_courant = i_new;

			//Mise à jour des compteurs
			MAJ(Voyage, S, i_courant, &nb_culturals, &nb_naturals, &nb_mixtes, &nb_sites_visitees, &nb_pays, &nb_dangers);

			//Insertion du site à visiter dans l'itinéraire du Voyage
			InsererEnFin(Voyage, S[i_courant]);
			S[i_courant].visite = 1;

			if (strcmp(S[i_courant].categorie,"Mixed")!=0)
				//On conserve la dernière catégorie différente de mixte, on change la categorie de comparaison pour garder le bon décompte de sites naturels/culturels visités
				categorie = S[i_courant].categorie;

		}else{ 
			//Pas assez de temps pour faire une nouvelle visite donc on retourne directement au point de départ
			InsererEnFin(Voyage, S[0]); // Insertion du point d'arrivé := départ
			temps_parcouru += (matriceDistance[i_courant][0]/85); //Ajout du temps parcouru pour aller du site courant au point dé départ
			fin=1;
		}
	}

	int new_score = nb_sites_visitees*10+nb_pays*20+nb_dangers*30;
	Supprime(&Voyage);
	return new_score;
}

void reset_visite(Site S[], int len){
	//Remet tous les sites.visite à 0
	for(int i = 0; i< len; i++){
		if(S[i].visite == 1){
			S[i].visite = 0;
		}
	}
}

Trajet *The_best(Site S[], double **matriceDistance, int len, int duree){
	//Renvoie le meilleur itineraire parmi tous les voyages testés
	Trajet *Voyager; 
	int best_score = 0; //meilleur score obtenu
	int score;

	float i_best, j_best, k_best;
	int pivot_best;
	
	//Comparaison des voyages en faisant varier les paramètres (pivot / coefficients)
	for (int pivot = 300; pivot <= 400; pivot+=50){
		for(float i = 0.1; i<=2; i+=0.1){
			for(float j = 0.1; j<=2; j+=0.1){
				for(float k = 0.1; k<=2; k+=0.1){
					reset_visite(S,len);
					score = OptimumScore(S,matriceDistance,len, duree,i,j,k, pivot);
					if(score > best_score){
						i_best=i;
						j_best=j;
						k_best=k;
						pivot_best=pivot;
						best_score = score;
					}
				}
			}
		}
	}
	reset_visite(S,len);
	Voyager =  OptimumRetour(S,matriceDistance,len, duree,i_best,j_best,k_best, pivot_best);
	printf("Coefficients utilisés : i=%.2f, j= %.2f, k= %.2f, pivot=%d\n", i_best,j_best,k_best,pivot_best);
	return Voyager; 
}