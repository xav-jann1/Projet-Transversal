#include "Headers/HANDLE_Deplacement.h"

#include <stdio.h>
#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

#include "../../../Actionneurs/Base/BASE.h"
#include "../../../Actionneurs/Serializer/SRLZR.h"

/**
 * Déplacement:
 */

char* HANDLE_Deplacement(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 1;
  bit hasError = 0;
  char response[32] = "rien";

  // TV : Réglage de la vitesse de déplacement
  if (strcmp("TV", cmd) == 0) {
    if (nbParams == 1 && params[0] == '#') {
      hasError = BASE_setSpeed(valeurs[0]);
    } else
      hasError = 1;
  }
  // A : Avancer
  else if (strcmp("A", cmd) == 0) {
    strcpy(response, "D'accord, j'avance");
    if (nbParams >= 1 && params[0] == '#' && valeurs[0] >= 5)
      // Vitesse de la commande:
      hasError = BASE_forward_v(valeurs[0]);
    else {
      // Vitesse définie par "TV":
      hasError = BASE_forward_v(0);
    }
  }
  // B : Reculer
  else if (strcmp("B", cmd) == 0) {
    strcpy(response, "D'accord, je recule");
    if (nbParams >= 1 && params[0] == '#' && valeurs[0] >= 5)
      // Vitesse de la commande:
      hasError = BASE_backward_v(valeurs[0]);
    else {
      // Vitesse définie par "TV":
      hasError = BASE_backward_v(0);
    }
  }
  // S : Fin de déplacement
  else if (strcmp("S", cmd) == 0) {
    hasError = BASE_stop();
  }
  // RD : Rotation à droite de 90°
  else if (strcmp("RD", cmd) == 0) {
    hasError = BASE_rotate90Right();
  }
  // RG : Rotation à gauche de 90°
  else if (strcmp("RG", cmd) == 0) {
    hasError = BASE_rotate90Left();
  }
  // RC : Rotation complète de la base roulante de 180°
  else if (strcmp("RC", cmd) == 0) {
    if (nbParams >= 1)
      hasError = BASE_fullRotation(params[0]);
    else {
      // Rotation complète à droite par défaut:
      hasError = BASE_fullRotation('D');
    }
  }
  // RA : Rotation de la base roulante d’un angle donné
  else if (strcmp("RA", cmd) == 0) {
    if (nbParams >= 1)
      hasError = BASE_rotate(params[0], valeurs[0]);
    else {
      // Rotation à droite par défaut:
      hasError = BASE_rotate('D', 90);
    }
  }
  // G : Déplacement de la base roulante par coordonnées
  else if (strcmp("G", cmd) == 0) {
    if (nbParams >= 3) {
      if (params[0] == 'X' && params[1] == 'Y' && params[2] == 'A') {
        hasError = BASE_moveTo(valeurs[0], valeurs[1], valeurs[2]);
      } else
        hasError = 1;
    } else
      hasError = 1;
  }
  // IPO : Initialisation de la position de la base roulante
  else if (strcmp("IPO", cmd) == 0) {
    if (nbParams >= 3) {
      if (params[0] == 'X' && params[1] == 'Y' && params[2] == 'A') {
        hasError = BASE_setPosition(valeurs[0], valeurs[1], valeurs[2]);
      } else
        hasError = 1;
    } else
      hasError = 1;
  }
  // POS : Position de la base roulante
  else if (strcmp("POS", cmd) == 0) {
    strcpy(response, BASE_getPosition());
  }
  
  // BONUS:
  
  // SPI : envoie 'pids' à la carte SRLZR
  else if (strcmp("SPI", cmd) == 0) {
    hasError = SRLZR_PIDstate();
  }
  
  // Si aucune commande reconnue:
  else
    isCommand = 0;

  if (hasError) strcpy(response, "error");
  else if(isCommand && strcmp(response, "rien") == 0) strcpy(response, "ok");

  return response;
}
