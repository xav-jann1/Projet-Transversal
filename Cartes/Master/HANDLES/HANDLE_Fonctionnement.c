#include "Headers/HANDLE_Fonctionnement.h"

#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

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
    strcpy(response, "Début de l'épreuve");
  }

  // E : Fin de l'épreuve
  else if (strcmp("E", cmd) == 0) {
    strcpy(response, "Fin de l'épreuve");
  }

  // Q : Arrêt "Urgence"
  else if (strcmp("Q", cmd) == 0) {
    strcpy(response, "Arrêt d'urgence");
  }

  return response;
}