#include "PARSEUR.h"
#include <stdio.h>
#include <string.h>

/*
 Optimisation possible:
 - faire le traitement d'un mot en une seule fois (au lieu de d'abord récupérer
   tous les mots puis de les traiter un par un)
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
    char* param = strtok(mots[i + 1], ":");
    char* valeur = strtok(NULL, ":");

    // Ajoute le paramètre et sa valeur (s'il en possède une) à la commande:

    // Si le paramètre est une valeur:
    if (param[0] >= '0' && param[0] <= '9' || strlen(param) > 1) {
      int n;
      commande.params[i] = '#';

      // Transforme le paramètre en nombre:
      if (sscanf(param, "%d", &n) == 1) {
        commande.valeurs[i] = n;
      }
      // Si erreur lors de la conversion:
      else
        commande.valeurs[i] = 0xFF;
    }

    // Simple paramètre:
    else {
      // Ajoute le paramètre:
      commande.params[i] = param[0];

      // Ajoute la valeur:
      if (valeur != NULL) {
        int n;
        // Conversion en nombre:
        if (sscanf(valeur, "%d", &n) == 1) {
          commande.valeurs[i] = n;
        }
        // Si erreur lors de la conversion:
        else
          commande.valeurs[i] = 0xFF;
      }
      // Si pas de valeur:
      else
        commande.valeurs[i] = 0xFF;
    }
  }

  return commande;
}