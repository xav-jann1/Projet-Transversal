#include "CONFIG_8051.h"
#include "SERVO.h"
#include "TIME_8051.h"

void main() {
  // Initialisation du 8051:
  CONFIG_init();

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
  for (int i = 1; i <= 10; i++) {
    SERVO_pos(180 * i / 10.0);
    TIME_wait(500);
  }

  // Mouvement progressif de droite à gauche:
  for (int i = 1; i < 10; i++) {
    SERVO_pos(180 - 180 * i / 10.0);
    TIME_wait(500);
  }

  while (1)
    ;
}