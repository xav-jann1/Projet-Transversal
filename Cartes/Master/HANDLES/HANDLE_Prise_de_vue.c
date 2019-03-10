#include "Headers/HANDLE_Prise_de_vue.h"

#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

/**
 * Prise de vue:
 */

char* HANDLE_Prise_de_vue(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 0;
  bit hasError = 0;
  char response[32] = "rien";

  // PPH : Prise de photographie
  if (strcmp("PPH", cmd) == 0) {
  }

  // SPHH : Arrêt de la prise de photographies en continu
  else if (strcmp("SPH", cmd) == 0) {
  }

  return response;
}
