#ifndef ULTRA_H
#define ULTRA_H

/**
 * Initialisation des capteurs Ultrason
 * Registres modifiés: P1MDOUT, P3MDOUT, P3IF(IE6, IE7)
 */
void ULTRA_init();

/**
 * Configuration du Timer 4 pour mesurer une impulsion
 * Registres modifiés: T4CON, CKCON (T4M)
 */
void TIMER4_config();

/**
 * Fonction qui doit s'exécuter toutes les ms,
 * pour prévenir lorsque le capteur ultrason à réaliser une mesure
 * @return {bit} 0: rien, 1: mesure réalisée
 */
bit ULTRA_update();

/**
 * Renvoie la valeur d'une mesure réalisée
 * @return {int} distance mesurée
 */
int ULTRA_getMesure();

/**
 * Delay de ~10us pour le trigger du télémètre
 */
void ULTRA_delay_10us();

/**
 * Préparation pour une mesure avec un télémètre ultrason
 * Registres modifiés: T4CON(TR4), TL4, TH4 P3IF(IE6CF)
 */
void ULTRA_mesure_init();

/**
 * Déclenche une mesure avec le télémètre avant
 */
void ULTRA_mesure_avant();

/**
 * Déclenche une mesure avec le télémètre arrière
 */
void ULTRA_mesure_arriere();

/**
 * Réalise une double mesures: 
 *  capteur avant, puis capteur arrière (si le capteur avant n'a rien détecté)
 */
void ULTRA_double_mesures();

#endif  // ULTRA_H
