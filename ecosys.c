#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir=0.01;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;

/* PARTIE 1*/

/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  //faut verifier que le pointeur retourne par malloc n’est pas NULL
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  // Vérification que les coordonnées sont valides
  assert(x >= 0 && x < SIZE_X);
  assert(y >= 0 && y < SIZE_Y);
 
  Animal *nouvel_animal = creer_animal(x, y, energie);
  *liste_animal = ajouter_en_tete_animal(*liste_animal, nouvel_animal);
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
    // Vérifie si la liste est vide ou si l'animal à supprimer est NULL
    if (*liste == NULL || animal == NULL) return;

    Animal *courant = *liste; // Pointeur pour parcourir la liste
    Animal *precedent = NULL; // Pointeur pour garder une trace de l'élément précédent

    // Cherche l'animal à supprimer
    while (courant != NULL) {
        if (courant == animal) {
            // Si l'animal à supprimer est le premier élément
            if (precedent == NULL) {
                *liste = courant->suivant; // Met à jour la tête de la liste
            } else {
                precedent->suivant = courant->suivant; // Retire l'animal de la liste
            }
            free(courant); // Libération de la mémoire allouée à l'animal
            return; // Quitte la fonction après avoir supprimé l'animal
        }
        precedent = courant; // Met à jour le pointeur précédent
        courant = courant->suivant; // Passe à l'animal suivant
    }
}

/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
   Animal *tmp;
   while(liste){
   tmp=liste;
    liste=liste->suivant;
    free(tmp);
   }

  return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */

void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; i++) {
    for (j = 0; j < SIZE_Y; j++) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }
  
  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}

void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}



/* PARTIE 2*/
void ecrire_ecosys(const char* nom_fichier, Animal* liste_predateur, Animal* liste_proie){
	FILE* f= fopen(nom_fichier,"w"); //mode écriture "w"
	if(f == NULL){
		printf("Erreur ouverture %s\n",nom_fichier);
		return;
	}
	Animal* ani;
	fprintf(f,"<proies>\n");
	ani = liste_proie;
	while(ani){
		fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",ani->x,ani->y,ani->dir[0],ani->dir[1],ani->energie);
		ani = ani->suivant;
	}
	fprintf(f,"</proies>\n");

	fprintf(f,"<predateurs>\n");
	ani = liste_predateur;
	while(ani){
		fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",ani->x,ani->y,ani->dir[0],ani->dir[1],ani->energie);
		ani = ani->suivant;
	}
	fprintf(f,"</predateurs>");
	fclose(f);
}



void lire_ecosys(const char* nom_fichier, Animal** liste_proie, Animal** liste_predateur){
	FILE* f = fopen(nom_fichier,"r"); //lecture
	if(f==NULL){
		printf("Erreur d'ouverture du fichier : %s\n",nom_fichier);
		return;
	}
	Animal* ani;
	char buffer[256];
	int x,y;
	int dir[2];
	float e;
	//premier fgets pour passer apres la ligne "<proies>"
	fgets(buffer,256,f);  // fgets(char* buffer, int taille, FILE* f) permet de recuperer la ligne du fichier, et a chaque fgets ca passe a la ligne suivante
	assert(strncmp(buffer,"<proies>",8)==0); // compare les 8 premiers caracteres
	fgets(buffer,256,f); // on ecrase le contenu du buffer et maintenant on est a la ligne qui nous interesse
	while(strncmp(buffer,"</proies>",9)){ // tant que strncmp ne renvoi pas 0 (tant que on a pas atteint la fin des proies )
		sscanf(buffer,"x=%d y=%d dir[%d %d] e=%f\n", &x,&y,&dir[0],&dir[1],&e);
		ani = creer_animal(x,y,e); // On crée l'animal
		ani->dir[0] = dir[0];
		ani->dir[1] = dir[1];
		*liste_proie = ajouter_en_tete_animal(*liste_proie,ani); // on ajoute l'animal ayant les données lue dans le fichier
		fgets(buffer,256,f); // on passe a la ligne suivante
	}
	fgets(buffer,256,f);
	assert(strncmp(buffer,"<predateurs>",12)==0); // pour verifier qu'on a bien fini de lire toutes le proies et que maintenant on est aux predateurs
	fgets(buffer,256,f);
	while(strncmp(buffer,"</predateurs>",13)){
		sscanf(buffer,"x=%d y=%d dir[%d %d] e=%f\n", &x,&y,&dir[0],&dir[1],&e);
		ani = creer_animal(x,y,e); // On crée l'animal
		ani->dir[0] = dir[0];
		ani->dir[1] = dir[1];
		*liste_predateur = ajouter_en_tete_animal(*liste_predateur,ani); // on ajoute l'animal ayant les données lue dans le fichier
		fgets(buffer,256,f); // on passe a la ligne suivante
	}
  fclose(f);
}




/* Part 2. Exercice 4, question 1 */

void bouger_animaux(Animal *la) {

	Animal* animal = la;
	while(animal){
		if(animal->energie < 1){ // si l'animal n'a plus d'energie il ne peux plus bouger
			return;
		}
		//changement de direction
		float d = (float)(rand())/RAND_MAX;
		if(d <= p_ch_dir){ 
			animal->dir[0] = rand()%3-1;
			animal->dir[1] = rand()%3-1; // génère un nombre dans la plage [-1, 0, 1], ce qui correspond à un choix entre -1, 0 et 1 pour les coordonnées x et y.
		}
		//changement de position
		animal->x += animal->dir[0];
		animal->y += animal->dir[1];
		//gestion de la toricité
		if(animal->x == SIZE_X) animal->x = 0; //x revient à 0 (le bord gauche)
		if(animal->x < 0) animal->x = SIZE_X-1; //x revient à SIZE_X - 1 (le bord droit).

		if(animal->y == SIZE_Y) animal->y = 0;
		if(animal->y < 0) animal->y = SIZE_Y-1;
	
		//perte d'energie
		animal->energie--;
		animal = animal->suivant;
		}
}



/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
	Animal* ani = *liste_animal;  // Animal courant
	float d;
	
	// Parcours de la liste
	while(ani){
		d = (float)(rand())/RAND_MAX;
		// Cas de reproduction
		if(d <= p_reproduce){
			ani->energie = ani->energie/2;
			ajouter_animal(ani->x,ani->y,ani->energie,liste_animal);
		}
		ani = ani->suivant;
	}

}



/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
	assert(liste_proie);
	
	//Déplacement des animaux
	bouger_animaux(*liste_proie);

	// Suppression des proies dont l'energie est a 0
	Animal* ani = *liste_proie;

	while(ani){
		ani->energie--; //perte d'énergie représente le coût de survie pour chaque proie.
		if(ani->energie <= 0){
			enlever_animal(liste_proie,ani);
			assert(ani);
		}
		else if(monde[ani->x][ani->y]>0){	 // Si la case contient de l'herbe
			ani->energie += monde[ani->x][ani->y]; //l'énergie de la proie est augmentée de la quantité d'herbe présente
			monde[ani->x][ani->y] = temps_repousse_herbe; //ce qui correspond au délai avant que l'herbe repousse
		}
		ani = ani->suivant;
	}

	//Reproduction
	reproduce(liste_proie,p_reproduce_proie); 
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
	Animal* proie = l;
	while(proie){
		if(proie->x == x && proie->y ==y) return proie;
		proie = proie->suivant;
	}
  return NULL;
}

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
	assert(liste_predateur);
	Animal* ani = *liste_predateur;
	Animal* proie = NULL;

	//Déplacement des animaux
	bouger_animaux(*liste_predateur);

	// Suppression des proies dont l'energie est a 0
	while(ani){
		ani->energie--;
		if(ani->energie <= 0){
			enlever_animal(liste_predateur,ani);
			assert(ani);
		}
		else{
			proie = animal_en_XY(*liste_proie,ani->x,ani->y); 	// Si la case contient une proie
			if(proie != NULL){
				ani->energie += proie->energie;			// le predateur prend l'energie de la proie
				enlever_animal(liste_proie,proie);
			}
		}
		ani = ani->suivant;
	}

	//Reproduction
	reproduce(liste_predateur,p_reproduce_predateur); 

}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){	
	for(int i=0; i<SIZE_X; i++){
		for(int j=0; j<SIZE_Y; j++){
			monde[i][j]++;
		}
	}

}


