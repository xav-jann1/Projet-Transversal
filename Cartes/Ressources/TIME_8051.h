#ifndef TIME_8051_H
#define TIME_8051_H

/**
 * Configuration du Timer 0 pour déclencher une interruption toutes les ms
 * Registres modifiés: TMOD (T0M0,T0M1), TL0, TH0,
 *                     IE(PT0), IP(ET0), TCON(TF0,TR0)
 */
void TIME_init();

/**
 * Fonction d'interruption du Timer 0, pour compter les ms
 * Registres modifiés : TL0, TH0
 */
void TIME_interrupt();
/**
 * Délai d'attente en ms
 * @param {int} ms : durée en millisecondes
 */
void TIME_wait(unsigned int ms);

/**
 * Renvoie 1 si une ms vient de passer
 * @return {char} bool : 0 ou 1
 */
bit TIME_flag_ms();

/**
 * Initialise le flag indiquant les ms
 */
void TIME_clear_ms_flag();

/**
 * Renvoie si 100 ms vient de passer
 * @return {bit} 0: non, 1: oui
 */
bit TIME_flag_100ms();

/**
 * Initialise le flag indiquant les 100 ms
 */
void TIME_clear_100ms_flag();

#endif  // TIME_8051_H