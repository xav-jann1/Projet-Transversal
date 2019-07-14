#include "Headers/HANDLE_Detection.h"

#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"
#include "../../../Capteurs/Ultrason/ULTRA.h"

/**
 * Détection:
 */

char* HANDLE_Detection(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1;
  bit hasError = 0;
  char response[32] = "rien";

  // MOU : Détection d’obstacle unique
  if (strcmp("MOU", cmd) == 0) {
    if (nbParams == 0) ULTRA_mesure_avant();
    else if (nbParams == 1 && params[0] == 'D') ULTRA_double_mesures();
    else hasError = 1;
  }

  // MOB : Détection des obstacles par balayage
  else if (strcmp("MOB", cmd) == 0) {
  }

  // MOS : Détection de l’obstacle le plus proche par balayage
  else if (strcmp("MOS", cmd) == 0) {
  }

  // Si aucune commande reconnue:
  else
    isCommand = 0;
  
  // Réponse :
  if (hasError)
    strcpy(response, "error");
  else if (isCommand)
    strcpy(response, "ok");

  return response;
}
