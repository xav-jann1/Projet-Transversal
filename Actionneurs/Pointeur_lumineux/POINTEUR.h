#ifndef POINTEUR_H
#define POINTEUR_H

/**
 * Configuration du PWMet de la sortie du pointeur lumineux:
 * Registre modifiés: PCA0MD, EIE1, EIP1, XBR0, P0MDOUT
 */
void POINTEUR_init();

/**
 * Fonction déclenchée toutes les ms pour mettre à jour l'allumage du pointeur
 */
void POINTEUR_update();

/**
 * Réglage du rapport cyclique du PWM
 * @param {unsigned char} duty : rapport cyclique de la PWM ~ intensité (0-100)
 * Registre modifiés: PCA0CPL0, PCA0CPH0
 */
void POINTEUR_pwm(unsigned char pwm);

/**
 * Allume complètement le pointeur lumineux
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit POINTEUR_on();

/**
 * Eteint complétement le pointeur lumineux
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit POINTEUR_off();

/**
 * Séquence d'allumage du pointeur lumineux
 * @param {int} I : intensité du pointeur (1 - 100)
 * @param {int} D : durée d'allumage (1 - 99)
 * @param {int} E : durée d'extinction (0 - 99)
 * @param {int} N : nombre d'allumage (1 - 99)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit POINTEUR_allumage(int I, int D, int E, int N);

#endif  // POINTEUR_H