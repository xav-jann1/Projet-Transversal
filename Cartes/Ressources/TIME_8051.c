#include "TIME_8051.h"
#include "c8051F020.h"

#define SYSCLK 22118400

/**
 * Variables de TIME:
 */

// Indique si 1 ms s'est réalisée:
bit TIME_flag = 0;

// Indique si 100 ms s'est réalisée:
bit TIME_flag_100 = 0;

// Compteur de ms:
unsigned int TIME_counter = 0;

// Compteur pour 100 ms:
unsigned char TIME_counter_100ms = 0;

// Buffeur du compteur de ms, pour correctement exécuter le test d'une condition
unsigned int TIME_counter_buffer;

// Valeur de recharge du Timer pour effectuer 1 ms:
unsigned int TIME_counter_1ms = 0;

/**
 * Configuration du Timer 2 pour déclencher une interruption toutes les ms
 * Registres modifiés: T2CON (RCLK0, TCLK0, CPRL2, TF2, TR2), CKCON (T2M)
 *                     RCAP2H, RCAP2L, IP(PT2), IE(ET2)
 */
void TIME_init() {
  // Mode 1 du timer 2: 16-bit Timer with Auto-reload
  RCLK0 = 0;
  TCLK0 = 0;

  // Utilise SYSCLK: (et non SYSCLK/12)
  CKCON |= 1 << 5;  // T2M = 1

  // Source du Timer : SYSCLK
  CPRL2 = 0;

  // Valeur de recharge du Timer:
  TIME_counter_1ms = 65635 - SYSCLK / 1000;

  // Rempli les registres de recharge:
  RCAP2L = TIME_counter_1ms & 0x00FF;
  RCAP2H = TIME_counter_1ms >> 8;

  // Activation des interruptions:
  PT2 = 1;  // Priorité élevée
  ET2 = 1;  // Enable
  TF2 = 0;  // RAZ Flag

  // Activation du Timer 2:
  TR2 = 1;
}

/**
 * Fonction d'interruption du Timer 2, pour compter les ms
 */
void TIME_interrupt() interrupt 5 {
  // Compte les interruptions:
  TIME_counter++;
  TIME_flag = 1;

  // Compte pour 100ms
  TIME_counter_100ms++;
  if (TIME_counter_100ms >= 100) {
    TIME_flag_100 = 1;
    TIME_counter_100ms = 0;
  }
  // RAZ du flag:
  TF2 = 0;
}

/**
 * Délai d'attente en ms
 * (TIME_counter_buffer permet que TIME_counter ne soit pas modifiée durant
 * le test de la condition, sinon la boucle peut se terminer un peu plus tôt
 * que prévu, car une interruption peut se déclencher entre la comparaison
 * des deux octets de TIME_counter :
 * Exemple du bug : après le while, TIME_counter = 0x1300 alors que ms = 0x13EF)
 * @param {int} ms : durée en millisecondes
 */
void TIME_wait(unsigned int ms) {
  TIME_counter = 0;
  TIME_counter_buffer = 0;
   while (TIME_counter_buffer < ms) TIME_counter_buffer = TIME_counter;
}

/**
 * Renvoie si une ms vient de passer
 * @return {bit} 0: non, 1: oui
 */
bit TIME_flag_ms() { return TIME_flag; }

/**
 * Initialise le flag indiquant les ms
 */
void TIME_clear_ms_flag() { TIME_flag = 0; }

/**
 * Renvoie si 100 ms vient de passer
 * @return {bit} 0: non, 1: oui
 */
bit TIME_flag_100ms() { return TIME_flag_100; }

/**
 * Initialise le flag indiquant les 100 ms
 */
void TIME_clear_100ms_flag() { TIME_flag_100 = 0; }

/**
 * Archives:
 */

/**
 * Configuration du Timer 0 pour déclencher une interruption toutes les ms
 * Registres modifiés: TMOD (T0M0,T0M1), CKCON(T0M), TL0, TH0,
 *                     IE(PT0), IP(ET0), TCON(TF0,TR0)
 */
/*void TIME_init_Timer0() {
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
}*/