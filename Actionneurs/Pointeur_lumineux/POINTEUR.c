#include "POINTEUR.h"
#include <c8051F020.h>

#define SYSCLK 22118400

/**
 * Variables du pointeur pour l'allumage
 */

int POINTEUR_I, POINTEUR_D, POINTEUR_E, POINTEUR_N;
int POINTEUR_counter_100ms = 0;
bit POINTEUR_allumage_inProgress = 0, POINTEUR_isState_D;
int POINTEUR_state_counter;

/**
 * Configuration du PWMet de la sortie du pointeur lumineux:
 * Registre modifiés: PCA0MD, EIE1, EIP1, XBR0, P0MDOUT
 */
void POINTEUR_init() {
  /**
   * PWM
   */

  // Activation et utilisation en mode 16 bits du PWM:
  PCA0CPM0 = (1 << 7) + (1 << 6) + (1 << 1);

  // Compteur PWM sur SYSCLK / 4 et activation de l'interruption:
  PCA0MD = (1 << 1) + 1;

  // Activation de l'interruption:
  EIE1 |= 1 << 3;  // Enable: EPCA0 = 1
  EIP1 |= 1 << 3;  // Priorité élevée: PPCA0 = 1

  // Activation du Compteur:
  PCA0CN |= 1 << 6;

  /**
   * LED
   */

  // CEX0 connecté sur le Crossbar:
  XBR0 |= 1 << 3;

  // Sortie en Push-Pull:
  P0MDOUT |= 1 << 4; // crossbar-pin

  // Par défaut, désactive le pointeur:
  POINTEUR_off();
}

/**
 * Interruption du PCA
 * Registre modifiés: PCA0CN
 */
void POINTEUR_interrupt() interrupt 9 {
  // RAZ flags:
  CF = 0;
  CCF0 = 0;
}

/**
 * Fonction déclenchée toutes les ms pour mettre à jour l'allumage du pointeur
 */
void POINTEUR_update() {
  // Si allumage en cours:
  if (POINTEUR_allumage_inProgress) {
    POINTEUR_counter_100ms++;

    // Déclenchée toutes les 100 ms:
    if (POINTEUR_counter_100ms == 100) {
      POINTEUR_counter_100ms = 0;

      // Compte le nombre de 100 ms:
      POINTEUR_state_counter++;

      // Si allumage en cours:
      if (POINTEUR_isState_D) {
        // Si fin de l'allumage:
        if (POINTEUR_state_counter >= POINTEUR_D) {
          // Démarre l'extinction:
          POINTEUR_isState_D = 0;
          POINTEUR_state_counter = 0;
          POINTEUR_off();
        }
      }

      // Si extinction en cours:
      if (!POINTEUR_isState_D) {
        // Si fin de l'extinction:
        if (POINTEUR_state_counter >= POINTEUR_E) {
          POINTEUR_N--;

          // Si fin de la séquence d'allumage:
          if (POINTEUR_N == 0) {
            POINTEUR_allumage_inProgress = 0;
            POINTEUR_off();
          }
          // Sinon, allumage suivant:
          else {
            POINTEUR_isState_D = 1;
            POINTEUR_state_counter = 0;
            POINTEUR_pwm(POINTEUR_I);
          }
        }
      }
    }
  }
}

/**
 * Réglage du rapport cyclique du PWM
 * @param {unsigned char} duty : rapport cyclique de la PWM ~ intensité (0-100)
 * Registre modifiés: PCA0CPL0, PCA0CPH0
 */
void POINTEUR_pwm(unsigned char pwm) {
  // Calcul:
  float temp = (float)0xFFFF * (1.0f - pwm / 100.0f);
  int PCA = (int)temp;

  // Remplissage du registre de comparaison avec PCA0:
  PCA0CPL0 = PCA & 0x00FF;
  PCA0CPH0 = PCA >> 8;
}

/**
 * Allume complètement le pointeur lumineux
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit POINTEUR_on() {
  POINTEUR_pwm(100);
  return 0;
}

/**
 * Eteint complétement le pointeur lumineux
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit POINTEUR_off() {
  POINTEUR_pwm(0);
  return 0;
}

/**
 * Eteint complétement le pointeur lumineux et arrête la séquence d'allumage
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit POINTEUR_stop() {
  POINTEUR_allumage_inProgress = 0;
  POINTEUR_pwm(0);
  return 0;
}

/**
 * Séquence d'allumage du pointeur lumineux
 * @param {int} I : intensité du pointeur (1 - 100)
 * @param {int} D : durée d'allumage (1 - 99)
 * @param {int} E : durée d'extinction (0 - 99)
 * @param {int} N : nombre d'allumage (1 - 99)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit POINTEUR_allumage(int I, int D, int E, int N) {
  // Vérifications:
  if (I < 1 && I > 100) return 1;
  if (D < 1 && D > 99) return 1;
  if (E < 0 && E > 99) return 1;
  if (N < 1 && N > 99) return 1;

  // Enregistrement des valeurs:
  POINTEUR_I = I;
  POINTEUR_D = D;
  POINTEUR_E = E;
  POINTEUR_N = N;

  // Démarre l'allumage au prochain POINTEUR_update():
  POINTEUR_allumage_inProgress = 1;
  POINTEUR_state_counter = 0;
  POINTEUR_isState_D = 1;
  POINTEUR_pwm(POINTEUR_I);

  return 0;
}