#include "../../Cartes/Ressources/CONFIG_8051.h"
#include "../../Cartes/Ressources/TIME_8051.h"
#include "SERVO.h"

int i = 0;
void main() {
  // Initialisation du 8051:
  CONFIG_init();
  TIME_init();

  // Initialisation du servomoteur:
  SERVO_init();

  /**
   * Fonctionnement élémentaire
   */

  // Position minimale:
  SERVO_pos(0);
  TIME_wait(3000);

  // Position maximale:
  SERVO_pos(180);
  TIME_wait(3000);

  // Retour à la position minimale:
  SERVO_pos(0);
  TIME_wait(3000);

  /**
   * Balayage
   */

  // Mouvement progressif de gauche à droite:
  for (i = 0; i <= 180; i += 10) {
    SERVO_pos(i);
    TIME_wait(500);
  }

  // Mouvement progressif de droite à gauche:
  for (i = 180; i > 0; i -= 10) {
    SERVO_pos(i);
    TIME_wait(500);
  }

  // Alterne entre 0° et 180°:
  while (1) {
    SERVO_pos(0);
    TIME_wait(5000);
    SERVO_pos(180);
    TIME_wait(5000);
  }
}