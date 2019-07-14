#include "Headers/HANDLE_Auxiliaires.h"

#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

/**
 * Auxilaires:
 */

char* HANDLE_Auxiliaires(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1;
  bit hasError = 0;
  char response[32] = "rien";

  return response;
}
