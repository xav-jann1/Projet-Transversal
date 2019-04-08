#ifndef SLAVE_H
#define SLAVE_H

/**
 * Envoie un message à la carte Master
 * @param {char*} string : message à envoyer
 */
void print_MASTER(char* string);

/**
 * Fonction à exécuter toutes les ms pour mettre à jour tous les périphériques
 */
void RTOS();

/**
 * Initialisation du microcontrôlleur et de tous les périphériques
 */
void SLAVE_init();

/**
 * Arrêt d'urgence:
 *  Arrêt complet de la base roulante, les opérations en cours sont
 *  interrompues, les données en cours d’acquisition sont perdues
 */
void SLAVE_exit();

#endif