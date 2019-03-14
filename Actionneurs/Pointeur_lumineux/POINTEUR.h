#ifndef POINTEUR_H
#define POINTEUR_H

/**
 * Défini l'intensité du pointeur lumineux:
 * @param {float} pwm : rapport cyclique utilisé par la PWM (entre 0 et 1)
 */
void POINTEUR_pwm(float pwm);

/**
 * Configuration du PWM et de la sortie du pointeur lumineux:
 * Registre modifiés: PCA0MD, EIE1, EIP1, XBR0, P0MDOUT
 */
void POINTEUR_init();

/**
 * Réglage du rapport cyclique du PWM
 * @param {float} pwm : rapport cyclique utilisé par la PWM (entre 0 et 1)
 * Registre modifiés: PCA0CPL0, PCA0CPH0
 */
void POINTEUR_pwm(float pwm);

/**
 * Allume complètement le pointeur lumineux
 */
void POINTEUR_on();

/**
 * Eteint (presque) complétement le poiteur lumineux
 */
void POINTEUR_off();

#endif  // POINTEUR_H