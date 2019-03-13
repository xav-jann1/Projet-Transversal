#include "CONFIG_8051.h"
#include "POINTEUR.h"
#include "TIME_8051.h"

void main() {
  // Initialisation du 8051:
  CONFIG_init();

  // Initialisation du pointeur lumineux:
  POINTEUR_init();

  /**
   * Fonctionnement élémentaire
   */

  // Allume le pointeur:
  POINTEUR_on();
  TIME_wait(2000);

  // Eteint le pointeur:
  POINTEUR_off();
  TIME_wait(2000);

  /**
   * Intensité
   */

  // Rallume le pointeur:
  POINTEUR_on();

  // Augmente progressivement l'intensité du pointeur:
  for (int i = 1; i < 10; i++) {
    POINTEUR_pwm(0.1 * i);
    TIME_wait(1000);
  }

  // Diminue progressivement l'intensité du pointeur:
  for (int i = 1; i < 10; i++) {
    POINTEUR_pwm(1 - 0.1 * i);
    TIME_wait(1000);
  }

  /**
   * Clignotement
   */

  POINTEUR_pwm(1);

  // Fait clignoter le pointeur:
  for (int i = 0; i < 10; i++) {
    POINTEUR_on();
    TIME_wait(500);
    POINTEUR_off();
    TIME_wait(500);
  }

  while (1)
    ;
}