#include "SERVO.h"
#include "c8051F020.h"

#define SYSCLK 22118400

/**
 * Variables du Servomoteur:
 */

// Pin du Servomoteur:
sbit SERVO_pin = P1 ^ 3;

// Paramètres de la position du Servomoteur:
unsigned int SERVO_t_pos_min = 600;   // Durée de l'impulsion pour 0°
unsigned int SERVO_t_pos_max = 2300;  // Durée de l'impulsion pour 180°
unsigned int SERVO_alpha;             // Durée d'impulsion par degré d'angle

// Calcul de la position:
unsigned int timer_load, timer_count, timer_pulse;

// Compte le nombre d'interruptions:
int SERVO_i = 0;

/**
 * Initialisation pour utiliser un servomoteur
 * Registres modifiés: P1MDOUT ()
 */
void SERVO_init() {
  // Activation de la sortie en Push-Pull:
  P1MDOUT |= 1 << 3;

  // Configuration du Timer 3:
  TIMER3_config();

  // Durée d'impulsion par degré d'angle:
  SERVO_alpha = (SERVO_t_pos_max - SERVO_t_pos_min) / 180;
}

/**
 * Configuration du Timer 3
 * Registres modifiés: EIP2 (PT3), EIE2 (ET3), TMR3CN (TF3,T3M,TR3)
 */
void TIMER3_config() {
  // Activation des interruptions:
  EIP2 |= 1;            // Priorité élevé: PT3 = 1
  EIE2 |= 1;            // Enable: ET3 = 1
  TMR3CN &= ~(1 << 7);  // RAZ Flag: TF3 = 0

  // Utilise SYSCLK:
  TMR3CN |= 1 << 1;  // T3M = 1

  // Activation du Timer 3:
  TMR3CN |= (1 << 2);  // TR3 = 1
}

/**
 * Fonction d'interruption du Timer 3
 * Etat haut 1 fois sur 10 déclenchements
 * Registre modifié: TMR3CN (TF3)
 */
void SERVO_interrupt() interrupt 14 {
  // RAZ du flag:
  TMR3CN &= ~(1 << 7);

  // Etat haut:
  if (SERVO_i == 10) {
    SERVO_pin = 1;
    SERVO_i = 0;
  }

  // Sinon, Etat bas:
  else
    SERVO_pin = 0;

  SERVO_i++;
}

/**
 * Modifie la position du servomoteur
 * @param {unsigned int} pos : position du servomoteur (entre 0° et 180°)
 * Registres modifiés: TMR3RLL, TMR3RLH
 */
void SERVO_pos(unsigned int pos) {
  // Vérification de l'angle:
  if (pos > 180) pos = 180;

  // Durée de l'impulsion nécessaire pour l'angle:
  timer_pulse = SERVO_t_pos_min + (pos * SERVO_alpha);

  // Conversion en nombre de coups d'horloge:
  timer_count = timer_pulse * 0.000001 * SYSCLK;

  // Valeur de reload du Timer 3:
  timer_load = 65535 - timer_count;

  // Chargement du Timer 3:
  TMR3RLL = timer_load & 0x00FF;
  TMR3RLH = timer_load >> 8;
}