#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ecosys.h"
#define N_PROIES 20
#define N_PREDATEURS 20

int main() {
    srand(time(NULL));

    Animal *liste_proies = NULL;
    Animal *liste_predateurs = NULL;

    // Ajout de proies et de prédateurs
    for (int i = 0; i < 20; i++) {
        ajouter_animal((rand() % 19)+1, (rand() % 49)+1, 10.0, &liste_proies); 
        ajouter_animal((rand() % 19)+1, (rand() % 49)+1, 10.0, &liste_predateurs); 
    }
    
    // Comptage des animaux avant suppression
    int nombre_proies_avant = compte_animal_rec(liste_proies);
    printf("Nombre de proies avant suppression : %d\n", nombre_proies_avant);
    int nombre_predateurs_avant = compte_animal_rec(liste_predateurs);
    printf("Nombre de prédateurs avant suppression : %d\n", nombre_predateurs_avant);

    // Suppression d'une proie
    if (liste_proies != NULL) {
        Animal *animal_a_enlever = liste_proies;  // Enlève le premier animal pour le test
        enlever_animal(&liste_proies, animal_a_enlever);
    }

    // Suppression d'un prédateur
    if (liste_predateurs != NULL) {
        Animal *animal_a_enlever_predateur = liste_predateurs;  // Enlève le premier prédateur pour le test
        enlever_animal(&liste_predateurs, animal_a_enlever_predateur);
    }

    // Comptage des animaux après suppression
    int nombre_proies_apres = compte_animal_rec(liste_proies);
    printf("Nombre de proies après suppression : %d\n", nombre_proies_apres);
    int nombre_predateurs_apres = compte_animal_rec(liste_predateurs);
    printf("Nombre de prédateurs après suppression : %d\n", nombre_predateurs_apres);
	    
	// Ecriture dans fich.txt
    ecrire_ecosys("ecosysteme.txt",liste_predateurs,liste_proies);
    afficher_ecosys(liste_proies, liste_predateurs);
    printf(" LISTE PROIES \n");
	Animal* ani = liste_proies;
	while (ani) {
	
    	printf("x = %d -- y = %d -- energie = %.2f -- dir=[%d %d] \n", 
           ani->x, ani->y, ani->energie, ani->dir[0], ani->dir[1]);
    	ani = ani->suivant; // Passage au prochain animal
}
	Animal* cur = liste_predateurs; // On parcourt la liste des prédateurs

	printf(" LISTE PREDATEURS \n");
	while (cur) {
	 
    	printf("x = %d -- y = %d -- energie = %.2f -- dir=[%d %d] \n", 
      cur->x, cur->y, cur->energie, cur->dir[0], cur->dir[1]);
    	cur = cur->suivant; // Passage au prochain prédateur
}


    // Lecture depuis fich.txt
    Animal* l_proie = NULL;
    Animal* l_predateur = NULL;
    lire_ecosys("ecosysteme.txt",&l_proie,&l_predateur);
    afficher_ecosys(l_proie,l_predateur);
    


	


    // Libération de la mémoire
    liberer_liste_animaux(liste_proies);
    liberer_liste_animaux(liste_predateurs);
    liberer_liste_animaux(l_proie);
    liberer_liste_animaux(l_predateur);
    
    
    return 0;
}
