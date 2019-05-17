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
		strcpy(response, "slave");
  }

  // SD : Génération de signaux sonores
  else if (strcmp("SD", cmd) == 0) {
	int j;
    int F = 6, P = 250, W = 500, B = 3;
    for (j = 0; j < nbParams; j++) {
      // F: code fréquence
      if (params[j] == 'F') {
        if (valeurs[j] >= 1 && valeurs[j] <= 99)
          F = valeurs[j];
        else
          hasError = 1;
      }

      // P: Durée son
      else if (params[j] == 'P') {
        if (valeurs[j] >= 1 && valeurs[j] <= 99)
          P = valeurs[j];
        else
          hasError = 1;
      }

      // W: Durée silence
      else if (params[j] == 'W') {
        if (valeurs[j] >= 0 && valeurs[j] <= 99)
          W = valeurs[j];
        else
          hasError = 1;
      }

      // B: Nombre de bips
      else if (params[j] == 'B') {
        if (valeurs[j] >= 1 && valeurs[j] <= 99)
          B = valeurs[j];
        else
          hasError = 1;
      } 
			
			// Non reconnue:
			else {
				hasError = 1;
			}
    }
    
    // Si pas d'erreur, envoie la commande à la carte Slave:
    if (!hasError) strcpy(response, "slave");
  }

  // Si aucune commande reconnue:
  else
    isCommand = 0;
	
	if (hasError) strcpy(response, "error");
  else if(isCommand && strcmp(response, "rien") == 0) strcpy(response, "ok");

  return response;
}
