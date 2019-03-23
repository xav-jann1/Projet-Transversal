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
    
    // Démarre la séquence d'allumage:
    POINTEUR_allumage(I, D, E, N);
  }

  // LS : Fin de l’allumage du pointeur lumineux
  else if (strcmp("LS", cmd) == 0) {
    if(nbParams == 0) hasError = POINTEUR_stop();
    else hasError = 1;
  }

  // LE : Séquence d'allumage prédéfinie
  else if (strcmp("LE", cmd) == 0) {
    if (nbParams == 1 && params[0] == '#') {
      if (valeurs[0] == 3)
        hasError = POINTEUR_allumage(100, 99, 0, 1);
      else if (valeurs[0] == 4)
        hasError = POINTEUR_allumage(100, 10, 10, 2);
      else if (valeurs[0] == 5)  // 5 = 7
        hasError = POINTEUR_allumage(100, 5, 5, 8);
      else if (valeurs[0] == 6)
        hasError = POINTEUR_allumage(10, 1, 10, 4);
      else if (valeurs[0] == 7)  // 7 = 5
        hasError = POINTEUR_allumage(100, 5, 5, 8);
      else if (valeurs[0] == 9)
        hasError = POINTEUR_allumage(10, 1, 10, 50);
      else if (valeurs[0] == 10)
        hasError = POINTEUR_allumage(10, 1, 10, 4);
      else hasError = 1;
    }
    else hasError = 1;
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
