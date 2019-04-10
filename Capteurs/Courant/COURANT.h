#ifndef COURANT_H
#define COURANT_H

/**
 * Initialisation des registres pour permettre la lecture du courant consommé
 * Actions:
 *  - mise en place de l'ADC
 *  - initialisation des variables : énergie_consommée
 */
void COURANT_init();

/**
 * Mets à zéro les valeurs d'énergie consommée
 */
void COURANT_reset();

/**
 * Effectue une mesure instantanée du courant consommé par la base
 * @return {int} : courant_mesuré
 */
float COURANT_mesure();

/**
 * Met à jour le courant consommé depuis le début de l'épreuve
 */
void COURANT_update();

/**
 * Renvoie l'énergie consommée depuis le début de l'épreuve
 * @param {int} : énergie_consommée
 */
float COURANT_getEnergie();

/**  "MI"
 * Effectue une mesure instantanée du courant consommé par la base
 * Et renvoie la valeur obtenue
 * @return {char*} : "KI courant_mesuré"
 */
void COURANT_MI();

/**  "ME"
 * Renvoie un message contenant l'énergie consommée depuis le début de l'épreuve
 * @return {char*} : "KE énergie_consommée"
 */
void COURANT_ME();

#endif  // COURANT_H
