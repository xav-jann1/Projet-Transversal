#include "Headers/HANDLE_Signaux_sonores.h"

#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

/**
 * Signaux sonores:
 */

char* HANDLE_Signaux_sonores(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1;
  bit hasError = 0;
  char response[32] = "rien";

  // ASS : Acquisition de signaux sonores
  if (strcmp("ASS", cmd) == 0) {
  }

  // SD : Génération de signaux sonores
  else if (strcmp("SD", cmd) == 0) {
  }

  // Si aucune commande reconnue:
  else
    isCommand = 0;

  return response;
}
