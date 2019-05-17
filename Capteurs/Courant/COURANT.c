
#include "COURANT.h"
#include <stdio.h>
#include "c8051F020.h"

/**
 * Organisation des pins :
 *   PWR+ -> AIN0.0: J11.7
 *   PWR- -> AIN0.1: J11.8
 *
 * Pins du port J11:
 *     *12 *11
 *     *10 *9
 *     *8  *7  <--
 *     *6  *5
 *     *4  *3
 *     *2  *1
 */

/**
 * Variables de l'ADC:
 */

// Résistance de Shunt:
#define R_shunt 0.05f  // Ohm

// Durée entre deux mesures de l'énergie:
#define dt 0.1f  // s

// Registre de l'ADC0:
sfr16 ADC0 = 0xBE;

// Energie consommée:
float COURANT_energie = 0;

/**
 * Initialisation des registres pour permettre la lecture du courant consommé
 * Actions:
 *  - mise en place de l'ADC
 *  - initialisation des variables : énergie_consommée
 */
void COURANT_init() {
  REF0CN = 0x03;

  AMX0CF = 0x0;  // temp: AIN0 AIN1 entrées
  // AMX0CF = 0x1;  // AIN0, AIN1: entrées différentielles

  AMX0SL = 0x0;  // temp: AIN0 par défaut
  // AMX0SL = 0x1;  // +AIN0 -AIN1

  // Registre AD0CN:
  AD0CM0 = 0;  // Conversion pour AD0BUSY = 1
  AD0CM1 = 0;
  AD0TM = 0;
  AD0LJST = 0;  // Alignement à droite
  AD0EN = 1;    // Activation de l'ADC

  COURANT_reset();
}

/**
 * Mets à zéro l'énergie consommée
 */
void COURANT_reset() { COURANT_energie = 0; }

/**
 * Effectue une mesure instantanée du courant consommé par la base
 * @return {float} : courant_mesuré (A)
 */
float COURANT_mesure() {
  char i;
  float tension_moyenne = 0;

  // Sélection du canal d'entrée de l'ADC:
  AMX0SL = 0x0;  // AIN0
  // AMX0SL = 0x1;  // +AIN0 -AIN1

  // Effetue 5 mesures, pour faire une moyenne:
  for (i = 0; i < 5; i++) {
    // Lance la mesure:
    AD0INT = 0;
    AD0BUSY = 1;

    // Attend la fin de la mesure:
    while (AD0INT != 1)
      ;

    // Conversion en tension et ajout à la moyenne:
    tension_moyenne += ADC0 * 2.43f / 4095.0f;
  }

  // Moyenne sur 5 valeurs:
  tension_moyenne /= 5.0f;
	
	// Correction par l'amplification du montage en entrée:
	tension_moyenne = (tension_moyenne / 52.0f) * 3.5f;

  // Retourne le courant en A:
  return tension_moyenne / R_shunt;
}

/**
 * Met à jour le courant consommé depuis le début de l'épreuve
 * Fonction à exécuter toutes les <dt> secondes
 */
void COURANT_update() { COURANT_energie += COURANT_mesure() * dt; }

/**
 * Renvoie l'énergie consommée depuis le début de l'épreuve
 * @param {int} : énergie_consommée
 */
int COURANT_getEnergie() { return (int)COURANT_energie; }

/**  "MI"
 * Effectue une mesure instantanée du courant consommé par la base
 * et renvoie la valeur obtenue en mA
 * @return {int} : courant_mesuré (mA)
 */
int COURANT_MI() { return (int)(1000.0f * COURANT_mesure()); }

/**  "ME"
 * Renvoie l'énergie consommée depuis le début de l'épreuve
 * @return {int} : énergie_consommée
 */
int COURANT_ME() { return COURANT_getEnergie(); }
