#include "Headers/HANDLE_Courant.h"

#include <string.h>
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
  }

  // ME : Mesure énergie
  else if (strcmp("ME", cmd) == 0) {
  }

  // Si aucune commande reconnue:
  else
    isCommand = 0;

  return response;
}
