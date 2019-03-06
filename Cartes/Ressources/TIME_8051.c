#include "TIME_8051.h"
#include "c8051F020.h"

#define SYSCLK 22118400

/**
 * Variables de TIME:
 */

// Indique si 1 ms s'est réalisée:
char TIME_flag = 0;

// Compteur de ms:
unsigned int TIME_counter = 0;

// Valeur de recharge du Timer pour effectuer 1 ms:
unsigned int TIME_counter_1ms = 0;

/**
 * Configuration du Timer 0 pour déclencher une interruption toutes les ms
 * Registres modifiés: TMOD (T0M0,T0M1), CKCON(T0M), TL0, TH0,
 *                     IE(PT0), IP(ET0), TCON(TF0,TR0)
 */
void TIME_init() {
  // Timer 0 en mode 1: 16-bit Timer
  TMOD |= (1 << 0);   // T0M0 = 1
  TMOD &= ~(1 << 1);  // T0M1 = 0

  // Utilise SYSCLK:
  CKCON |= 1 << 3;  // T0M = 1

  // Valeur de recharge du Timer:
  TIME_counter_1ms = 65635 - SYSCLK / 1000;
  TL0 = TIME_counter_1ms & 0x00FF;
  TH0 = TIME_counter_1ms >> 8;

  // Activation des interruptions:
  PT0 = 1;  // Priorité élevée
  ET0 = 1;  // Enable
  TF0 = 0;  // RAZ Flag

  // Activation du Timer 0:
  TR0 = 1;
}

/**
 * Fonction d'interruption du Timer 0, pour compter les ms
 * Registres modifiés : TL0, TH0
 */
void TIME_interrupt() interrupt 1 {
  // Recharge du Timer 0:
  TL0 = TIME_counter_1ms & 0x00FF;
  TH0 = TIME_counter_1ms >> 8;

  // Compte les interruptions:
  TIME_counter++;
  TIME_flag = 1;
}

/**
 * Délai d'attente en ms
 * @param {int} ms : durée en millisecondes
 */
void TIME_wait(int ms) {
  TIME_counter = 0;
  while (TIME_counter < ms)
    ;
}

/**
 * Renvoie si une ms vient de passer
 * @return {char} bool : 0 ou 1
 */
char TIME_flag_ms() { return TIME_flag; }

/**
 * Initialise le flag indiquant les ms
 */
void TIME_clear_ms_flag() { TIME_flag = 0; }
