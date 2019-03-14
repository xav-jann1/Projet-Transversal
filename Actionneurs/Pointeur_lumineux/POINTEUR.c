#include "POINTEUR.h"
#include <c8051F020.h>

#define SYSCLK 22118400

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
  //P0MDOUT |= 1 << 5; // todo
  P0MDOUT |= 1 << 0;

  // Par défaut, désactive le pointeur:
  POINTEUR_off();
}

/**
 * Interruption du PCA
 * Registre modifiés: PCA0CN
 */
void POINTEUR_interrupt() interrupt 9 {
  // RAZ flags:
  PCA0CN &= ~(1 << 7 + 1);  // CF = 0, CCF0 = 0
}

/**
 * Réglage du rapport cyclique du PWM
 * @param {float} duty : rapport cyclique de la PWM ~ intensité
 * Registre modifiés: PCA0CPL0, PCA0CPH0
 */
void POINTEUR_pwm(float duty) {
  // Calcul:
  float temp = (float)0xFFFF * (1.0 - duty);
  int PCA = (int)temp;

  // Remplissage du registre de comparaison avec PCA0:
  PCA0CPL0 = PCA & 0x00FF;
  PCA0CPH0 = PCA >> 8;
}

/**
 * Allume complètement le pointeur lumineux
 */
void POINTEUR_on() { POINTEUR_pwm(1); }

/**
 * Eteint complétement le poiteur lumineux
 */
void POINTEUR_off() { POINTEUR_pwm(0); }