#ifndef MASTER_H
#define MASTER_H

/**
 * Fonction déclenchée lorsqu'une ligne est reçue sur l'UART0
 * @param {char*} message : message reçu
 */
void UART0_receive_handle(char* message);

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
 * Initialisation du microcontrôlleur et de tous les périphériques
 */
void MASTER_init();

#endif