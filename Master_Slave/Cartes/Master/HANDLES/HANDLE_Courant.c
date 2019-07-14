#include "Headers/HANDLE_Courant.h"

#include <stdio.h>
#include <string.h>
#include "../../../Capteurs/Courant/COURANT.h"
#include "../../../Communication/Parseur_messages/PARSEUR.h"

/**
 * Mesure de Courant:
 */

char* HANDLE_Courant(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1;
  bit hasError = 0;
  char response[32] = "rien";

  // MI : Mesure courant
  if (strcmp("MI", cmd) == 0) {
    if (nbParams == 0)
      sprintf(response, "KI %d", COURANT_MI());
    else
      hasError = 1;
  }

  // ME : Mesure énergie
  else if (strcmp("ME", cmd) == 0) {
    if (nbParams == 0)
      sprintf(response, "KE %d", COURANT_ME());
    else
      hasError = 1;
  }

  // Si aucune commande reconnue:
  else
    isCommand = 0;

  // Réponse :
  if (hasError)
    strcpy(response, "error");
  else if (isCommand && strcmp(response, "rien") == 0)
    strcpy(response, "ok");

  return response;
}
