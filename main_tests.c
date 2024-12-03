#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ecosys.h"



int main(void) {
    //int i;
    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;
    int energie = 10;

    srand(time(NULL));

    // Création des proies
    Animal *a1 = creer_animal(0, 10, energie);
    Animal *a2 = creer_animal(15, 35, energie);
    Animal *a3 = creer_animal(2, 42, energie);
    Animal *a4 = creer_animal(18, 13, energie);
 
    a1->suivant = a2;
    a2->suivant = a3;
    a3->suivant = a4;

    liste_proie = a1; // Premier élément de la liste des proies

    // Création des prédateurs
    Animal *a5 = creer_animal(2, 5, energie);
    Animal *a6 = creer_animal(15, 35, energie);
    Animal *a7 = creer_animal(9, 22, energie);
    Animal *a8 = creer_animal(17, 3, energie);
 
    a5->suivant = a6;
    a6->suivant = a7;
    a7->suivant = a8;

    liste_predateur = a5; // Premier élément de la liste des prédateurs
 
    // Afficher l'écosystème
    afficher_ecosys(liste_proie, liste_predateur);  

    // Enlever des animaux
    enlever_animal(&liste_proie, a1); // Enlever le premier animal de proie
    enlever_animal(&liste_predateur, a5); // Enlever le premier animal prédateur

    // Libérer la mémoire restante
    liberer_liste_animaux(liste_proie); // Libérer la liste de proies
    liberer_liste_animaux(liste_predateur); // Libérer la liste de prédateurs

    return 0;
}
