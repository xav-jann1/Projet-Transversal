#include "Headers/HANDLE_Pointeur_lumineux.h"

#include <stdio.h>
#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

#include "../../../Actionneurs/Servomoteur/SERVO.h"

/**
 * Pointeur lumineux:
 */

char* HANDLE_Pointeur_lumineux(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1;
  bit hasError = 0;
  char response[32] = "rien";

  // L : Allumage du pointeur lumineux
  if (strcmp("L", cmd) == 0) {
  }

  // LS : Fin de l’allumage du pointeur lumineux
  else if (strcmp("LS", cmd) == 0) {
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
      if (O == 'H')
        SERVO_pos(angle);
      else
        SERVO_pos(-90);
    }
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
