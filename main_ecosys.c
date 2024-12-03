#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"


#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 400000
#define IT_MAX 100






int main(void) {
	srand(time(NULL));

    // Création des animaux
	Animal* liste_proie = NULL;
	Animal* liste_predateur = NULL;

	//PROIES
    for (int i = 0; i < NB_PROIES; i++) {
        ajouter_animal(rand()%SIZE_X, rand()%SIZE_Y, 10.0, &liste_proie); 
    }
	//PREDATEURS
	for (int i = 0; i < NB_PREDATEURS; i++) {
        ajouter_animal(rand()%SIZE_X, rand()%SIZE_Y, 10.0, &liste_predateur); 
    }
	//HERBE
	int monde[SIZE_X][SIZE_Y];
	for(int i=0; i<SIZE_X; i++){
		for(int j=0; j<SIZE_Y; j++){
			monde[i][j] = 0;
		}
	}

	// On ouvre le fichier pour le graph
	FILE* graph = fopen("Evol_pop.txt","w");

	// EV0LUTI0N DE L'ECOSYSTEME
	int i = 0;
	while(liste_proie || liste_predateur && i<IT_MAX ){
		
		rafraichir_proies(&liste_proie,monde);
		rafraichir_predateurs(&liste_predateur,&liste_proie);
		rafraichir_monde(monde);
		afficher_ecosys(liste_proie,liste_predateur);
		printf("%d\n",i);  // Pour afficher l'iteration actuelle
		usleep(T_WAIT);
		i++;
		fprintf(graph,"%d %d %d\n",i,compte_animal_rec(liste_proie),compte_animal_rec(liste_predateur)); // On ecrit dans le fichier pour le graph
		
	}

	//Libération de la mémoire
	liberer_liste_animaux(liste_proie);
	liberer_liste_animaux(liste_predateur);
	fclose(graph);
	
	
	  FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        perror("Erreur lors de l'ouverture de gnuplot");
        return 1;
    }

    // Envoyer des commandes à gnuplot
    fprintf(gnuplot, "set title 'Simulation ecosystème'\n");
    fprintf(gnuplot, "plot 'Evol_pop.txt' using 1:2 with lines title 'proies'\n");
    fprintf(gnuplot, "replot 'Evol_pop.txt' using 1:3 with lines title 'predateurs'\n");
    
    // Fermer le pipe
    pclose(gnuplot);

  // Graphe d'evolution à fournir plus tard

  return 0;
}
	


/*  Évolution de l'écosystème : Facteurs influents
Facteurs clés :

Taux de reproduction des animaux
Temps de repousse de l'herbe
Énergie des animaux
Nombre de proies et prédateurs
Rôle de l'énergie :

Les prédateurs dépendent des proies pour survivre.
Les proies survivent indépendamment grâce à l'herbe.

Impact de la reproduction :

Les animaux qui se reproduisent plus fréquemment vivent moins longtemps.
La reproduction divise l'énergie de l'animal.
Les animaux avec un taux de reproduction faible vivent plus longtemps.
Conclusion :

La gestion de l'énergie et de la reproduction a un impact direct sur la survie et la durée de vie des animaux dans l'écosystème. */
