#ifndef MASTER_H
#define MASTER_H

/**
 * Envoie un message au PC Centrale de Commande
 * @param {char*} string : message à envoyer
 */
void print_PC(char* string);

/**
 * Fonction à exécuter toutes les ms pour mettre à jour tous les périphériques
 */
void RTOS();

/**
 * Fonction à exécuter toutes les 100 ms pour mettre à jour tous les périphériques
 */
void RTOS_100ms();

/**
 * Initialisation du microcontrôlleur et de tous les périphériques
 */
void MASTER_init();

/**
 * Démarrer une épreuve
 * @param {unsigned char} e : numéro de l'épreuve
 * @return {bit} 0: ok, 1: erreur -> une épreuve a déjà commencée
 */
bit MASTER_startEpreuve(unsigned char e);

/**
 * Indiquer si une épreuve est en cours
 * @return {bit} 0: non, 1: épreuve en cours
 */
bit MASTER_isEpreuveInProgress();

/**
 * Termine une épreuve
 * @return {bit} 0: ok, 1: erreur -> aucune épreuve en cours
 */
bit MASTER_endEpreuve();

/**
 * Arrêt d'urgence:
 *  Arrêt complet de la base roulante, les opérations en cours sont
 *  interrompues, les données en cours d’acquisition sont perdues
 */
void MASTER_exit();

#endif