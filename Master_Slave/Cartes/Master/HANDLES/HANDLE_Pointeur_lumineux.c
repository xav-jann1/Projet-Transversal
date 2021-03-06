#include "Headers/HANDLE_Pointeur_lumineux.h"

#include <stdio.h>
#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

#include "../../../Actionneurs/Pointeur_lumineux/POINTEUR.h"
#include "../../../Actionneurs/Servomoteur/SERVO.h"

/**
 * Pointeur lumineux:
 */

char* HANDLE_Pointeur_lumineux(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1, hasError = 0;
  char response[32] = "rien";

  // L : Allumage du pointeur lumineux
  if (strcmp("L", cmd) == 0) {
    int j;
    int I = 100, D = 99, E = 0, N = 1;
    for (j = 0; j < nbParams; j++) {
      // I: Intensité
      if (params[j] == 'I') {
        if (valeurs[j] >= 1 && valeurs[j] <= 100)
          I = valeurs[j];
        else
          hasError = 1;
      }

      // D: Durée allumage
      else if (params[j] == 'D') {
        if (valeurs[j] >= 1 && valeurs[j] <= 99)
          D = valeurs[j];
        else
          hasError = 1;
      }

      // E: Durée extinction
      else if (params[j] == 'E') {
        if (valeurs[j] >= 0 && valeurs[j] <= 99)
          E = valeurs[j];
        else
          hasError = 1;
      }

      // N: Nombre d'allumage
      else if (params[j] == 'N') {
        if (valeurs[j] >= 1 && valeurs[j] <= 99)
          N = valeurs[j];
        else
          hasError = 1;
      }
    }
    
    // Si pas d'erreur, envoie la commande à la carte Slave:
    if (!hasError) strcpy(response, "slave");
  }

  // LS : Fin de l’allumage du pointeur lumineux
  else if (strcmp("LS", cmd) == 0) {
     if (nbParams == 0) strcpy(response, "slave");
     else hasError = 1;
  }

  // LE : Séquence d'allumage prédéfinie
  else if (strcmp("LE", cmd) == 0) {
    strcpy(response, "slave");
  }

  // CS : Pilotage de servomoteur
  else if (strcmp("CS", cmd) == 0) {
    char O = 'H', angle = 0;
    int i;

    for (i = 0; i < nbParams; i++) {
      char param = params[i];

      // H:
      if (param == 'H') O = 'H';

      // V:
      else if (param == 'V')
        O = 'V';

      // A: Angle
      else if (param == 'A') {
        int a = valeurs[i];
        if (a < -90 || a > 90 || a == 0xFF)
          hasError = 1;
        else
          angle = (char)a;
      }
      // Non reconnue:
      else
        hasError = 1;
    }

    if (!hasError) {
      if (O == 'H') {
				SERVO_showResponse('H');
        SERVO_pos(angle);
			}
      // Si servomoteur Vertical:
      if (O == 'V') {
				SERVO_showResponse('V');
				strcpy(response, "slave");
			}
    }
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
