#include "../../Cartes/Ressources/CONFIG_8051.h"
#include "../../Cartes/Ressources/TIME_8051.h"
#include "POINTEUR.h"

void main() {
  int i;

  // Initialisation du 8051:
  CONFIG_init();
  TIME_init();

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
  for (i = 0; i < 100; i += 10) {
    POINTEUR_pwm(i);
    TIME_wait(1000);
  }

  // Diminue progressivement l'intensité du pointeur:
  for (i = 100; i >= 10; i -= 10) {
    POINTEUR_pwm(i);
    TIME_wait(1000);
  }

  /**
   * Clignotement
   */

  POINTEUR_pwm(1);

  // Fait clignoter le pointeur:
  for (i = 0; i <= 10; i++) {
    POINTEUR_on();
    TIME_wait(500);
    POINTEUR_off();
    TIME_wait(500);
  }

  // Commande de la fiche d'évaluation en Mode dégradé:

  POINTEUR_allumage(100, 99, 0, 1);   // 3
  // POINTEUR_allumage(100, 10, 10, 2);  // 4
  // POINTEUR_allumage(100, 5, 5, 8);    // 5
  // POINTEUR_allumage(10, 1, 10, 4);    // 6
  // POINTEUR_allumage(100, 5, 5, 8);    // 7 = 5
  // POINTEUR_allumage(10, 1, 10, 50);   // 9
  // POINTEUR_allumage(10, 1, 10, 4);    // 10

  while (1) {
    // Toutes les ms:
    if (TIME_flag_ms()) {
      TIME_clear_ms_flag();

      POINTEUR_update();
    }
  }
}