#include "Headers/HANDLE_Deplacement.h"

#include <stdio.h>
#include <string.h>
#include "../../../Communication/Parseur_messages/PARSEUR.h"

#include "../../../Actionneurs/Base/BASE.h"

/**
 * Déplacement:
 */

char* HANDLE_Deplacement(Commande* commande) {
  char nbParams = (*commande).nbParams;
  char* cmd = (*commande).cmd;
  char* params = (*commande).params;
  int* valeurs = (*commande).valeurs;

  bit isCommand = 0;
  bit hasError = 0;
  char response[32] = "rien";

  // TV : Réglage de la vitesse de déplacement
  if (strcmp("TV", cmd) == 0) {
    isCommand = 1;
    if (nbParams == 1 && params[0] == '#') {
      int speed = valeurs[0];
      hasError = BASE_setSpeed(speed);
    } else
      hasError = 1;
  }
  // A : Avancer
  else if (strcmp("A", cmd) == 0) {
    strcpy(response, "D'accord, j'avance");
    // if (nbParams >= 1) hasError = BASE_forward(params[0]);
    // else hasError = 1;
  }
  // B : Reculer
  else if (strcmp("B", cmd) == 0) {
    strcpy(response, "D'accord, je recule");
  }
  // S : Fin de déplacement
  else if (strcmp("S", cmd) == 0) {
  }
  // RD : Rotation à droite de 90°
  else if (strcmp("RD", cmd) == 0) {
  }
  // RG : Rotation à gauche de 90°
  else if (strcmp("RG", cmd) == 0) {
  }
  // RC : Rotation complète de la base roulante de 180°
  else if (strcmp("RC", cmd) == 0) {
  }
  // RA : Rotation de la base roulante d’un angle donné
  else if (strcmp("RA", cmd) == 0) {
  }
  // G : Déplacement de la base roulante par coordonnées
  else if (strcmp("G", cmd) == 0) {
  }
  // IPO : Initialisation de la position de la base roulante
  else if (strcmp("IPO", cmd) == 0) {
  }
  // POS : Position de la base roulante
  else if (strcmp("POS", cmd) == 0) {
  }

  if (hasError) strcpy(response, "error");

  return response;
}
