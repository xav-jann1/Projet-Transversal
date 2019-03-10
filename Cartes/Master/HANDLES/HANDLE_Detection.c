#include "Headers/HANDLE_Detection.h"

#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

/**
 * Détection:
 */

char* HANDLE_Detection(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 0;
  bit hasError = 0;
  char response[32] = "rien";

  // MOU : Détection d’obstacle unique
  if (strcmp("MOU", cmd) == 0) {
  }

  // MOB : Détection des obstacles par balayage
  else if (strcmp("MOB", cmd) == 0) {
  }

  // MOS : Détection de l’obstacle le plus proche par balayage
  else if (strcmp("MOS", cmd) == 0) {
  }

  return response;
}
