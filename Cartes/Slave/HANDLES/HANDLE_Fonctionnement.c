#include "Headers/HANDLE_Fonctionnement.h"

#include <stdio.h>
#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

// Epreuve en cours:
#include "../../../Cartes/Slave/SLAVE.h"

/**
 * Fonctionnement:
 */

char* HANDLE_Fonctionnement(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1;
  bit hasError = 0;
  char response[32] = "rien";

  // Q : Arrêt "Urgence"
  if (strcmp("Q", cmd) == 0) {
    if (nbParams == 0)
      SLAVE_exit();
    else
      hasError = 1;

    if (!hasError) strcpy(response, "Arrêt d'urgence");
  }

  // Si aucune commande reconnue:
  else
    isCommand = 0;

  return response;
}