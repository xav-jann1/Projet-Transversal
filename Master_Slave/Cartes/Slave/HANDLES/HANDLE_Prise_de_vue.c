#include "Headers/HANDLE_Prise_de_vue.h"

#include <string.h>
#include "../../../Capteurs/Photo/PHOTO.h"
#include "../../../Communication/Parseur_messages/PARSEUR.h"

/**
 * Prise de vue:
 */

char* HANDLE_Prise_de_vue(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1;
  bit hasError = 0;
  char response[32] = "rien";

  // PPH : Prise de photographie
  if (strcmp("PPH", cmd) == 0) {
    int j;
    char mode = 'O';
    unsigned char E = 1, N = 1;
    for (j = 0; j < nbParams; j++) {
      // O, C, S: Mode de la capture
      if (params[j] == 'O' || params[j] == 'C' || params[j] == 'S') {
        mode = params[j];
      }

      // E: Durée entre deux clichés
      else if (params[j] == 'E') {
        if (valeurs[j] >= 0 && valeurs[j] <= 99)
          E = valeurs[j];
        else
          hasError = 1;
      }

      // N: Nombre de clichés
      else if (params[j] == 'N') {
        if (valeurs[j] >= 1 && valeurs[j] <= 255)
          N = valeurs[j];
        else
          hasError = 1;
      }

			else hasError = 1;
    }
		
    if (!hasError) hasError = PHOTO_start(mode, E, N);
  }
	
  // SPH : Arrêt de la prise de photographies en continu
  else if (strcmp("SPH", cmd) == 0) {
  }

  // Si aucune commande reconnue:
    else isCommand = 0;
	
	// Réponse :
	if (hasError) strcpy(response, "error");
  else if(isCommand && strcmp(response, "rien") == 0) strcpy(response, "ok");

  return response;
}
