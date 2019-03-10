#include "Headers/HANDLE_Fonctionnement.h"

#include <stdio.h>
#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

// Epreuve en cours:
#include "../../../Cartes/Master/MASTER.h"

/**
 * Fonctionnement:
 */

char* HANDLE_Fonctionnement(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 0;
  bit hasError = 0;
  char response[32] = "rien";

  // D : Début de l'épreuve
  if (strcmp("D", cmd) == 0) {
    int epreuve = 1;
    isCommand = 1;

    if (nbParams >= 1) epreuve = valeurs[0];
    hasError = MASTER_startEpreuve(epreuve);

    if (!hasError) sprintf(response, "I Début de l'épreuve %d", epreuve);
  }

  // E : Fin de l'épreuve
  else if (strcmp("E", cmd) == 0) {
    isCommand = 1;

    if (nbParams == 0)
      hasError = MASTER_endEpreuve();
    else
      hasError = 1;

    if (!hasError) strcpy(response, "Fin de l'épreuve");
  }

  // Q : Arrêt "Urgence"
  else if (strcmp("Q", cmd) == 0) {
    isCommand = 1;

    if (nbParams == 0)
      MASTER_exit();
    else
      hasError = 1;

    if (!hasError) strcpy(response, "Arrêt d'urgence");
  }

  return response;
}