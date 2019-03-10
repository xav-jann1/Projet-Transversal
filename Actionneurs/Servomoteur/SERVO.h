#ifndef SERVO_H
#define SERVO_H

/**
 * Initialisation pour utiliser un servomoteur
 * Registres modifiés: P1MDOUT ()
 */
void SERVO_init();

/**
 * Configuration du Timer 3
 * Registres modifiés: EIP2 (PT3), EIE2 (ET3), TMR3CN (TF3, TR3)
 */
void TIMER3_config();

/**
 * Fonction qui doit s'exécuter toutes les ms,
 * pour prévenir lorsque le servo s'est bien positionné
 * @return {bit} 0: rien, 1: servo positionné
 */
bit SERVO_update();

/**
 * Fonction d'interruption du Timer 3
 * Etat haut 1 fois sur 10 déclenchements
 * Registre modifié: TMR3CN (TF3)
 */
void SERVO_interrupt();

/**
 * Modifie la position du servomoteur
 * @param {unsigned int} pos : position du servomoteur (entre 0° et 180°)
 * Registres modifiés: TMR3RLL, TMR3RLH
 */
void SERVO_pos(char pos);

#endif  // SERVO_H