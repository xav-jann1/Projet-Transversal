#include "PARSEUR.h"
#include <string.h>

/*
 Optimisations possibles:
 - faire le traitement d'un mot en une seule fois (au lieu de d'abord récupérer
   tous les mots puis de les traiter un par un)
 - enregistrer les valeurs d'une commande dans une liste de int,
   (si toutes les valeurs sont bien des nombres dans le cahier des charges)
*/

/**
 * Extrait la commande contenue dans un message
 * @param {char*} message : message à décortiquer
 * @return {Commande} : contient le code et les paramètres (avec valeurs)
 */
Commande PARSEUR_message(char* message) {
  // Variables de la fonction:
  char counter = 0, i;
  char mots[5][7];
  char* ptr;
  Commande commande;

  // Copie du message (car strtok() modifie ses paramètres):
  char str[50];
  strcpy(str, message);

  // Récupère chaque mot séparé par un espace:
  ptr = strtok(str, " ");          // Prend le premier mot
  while (ptr != NULL) {            // Tant qu'il y a un mot
    strcpy(mots[counter++], ptr);  //   Enregistre le mot
    ptr = strtok(NULL, " ");       //   Cherche le mot suivant
  }

  // Ajout du code de la commande et du nombre de paramètres:
  strcpy(commande.cmd, mots[0]);
  commande.nbParams = counter - 1;

  // Sépare et enregistre chaque mot à la commande (mot = param[:valeur]):
  for (i = 0; i < commande.nbParams; i++) {
    // Récupère le paramètre et la valeur:
    char* params = strtok(mots[i + 1], ":");
    char* valeur = strtok(NULL, ":");

    // Ajoute le paramètre et sa valeur (s'il en possède une) à la commande:
    commande.params[i] = params[0];  // $*

    if (valeur != NULL)
      strcpy(commande.valeurs[i], valeur);
    else
      strcpy(commande.valeurs[i], "\0");
  }

  return commande;
}

// $* : Si le paramètre est composé de plusieurs caractères
//  Remplacer la ligne par : strcpy(commande.params[i], params);
//
// !: il faut aussi modifier le struct Commande => params[4] -> params[4][4]
