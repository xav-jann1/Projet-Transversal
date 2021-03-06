#include "SPI_HANDLE_SLAVE.h"

#include <string.h>
#include <stdio.h>
#include "../../Communication/Parseur_messages/PARSEUR.h"
#include "../../Communication/SPI/SPI_SLAVE.h"
#include "../../Communication/UART/UART0.h"

// Fonctions pour exécuter une commande:
#include "./HANDLES/HANDLES_SLAVE.h"

extern char* (*HANDLES[5])(Commande*);

/**
 * Fonction déclenchée lorsqu'une ligne est reçue sur la SPI
 * @param {char*} message : message reçu
 */
void SPI_receive_handle_message(char* message) {
  char data i = 0;

  // Réponse:
  char data response[30] = "rien";
  
  // Commande:
  Commande data commande;
	
  // Copie le message (car il est modifié dans PARSEUR):
  char idata message_copy[30];
  strcpy(message_copy, message);

  // Extrait la commande du message:
  commande = PARSEUR_message(message);

  // Cherche et exécute la commande:
  for (i = 0; i < 5; i++) {
    // Exécute la commande, si elle est reconnue dans l'ensemble des fonctions:
    strcpy(response, (*HANDLES[i])(&commande));

    // S'il y a une réponse:
    if (strcmp(response, "rien") != 0) break;
  }

  // Commande correctement exécutée:
  if (strcmp(response, "ok") == 0) {
    // SPI_setColor(GREEN);
    SPI_send("\r\n>");
  }
  
  // Commande à envoyer à la STM32:
  else if (strcmp(response, "stm32") == 0) {
    // Envoie la commande au Slave:
    char data command[30 + 1];
    sprintf(command, "%s\r", message_copy);
    UART0_send(command);
    
    //UART0_setColor(GREEN);
    //UART0_send("\r\n>");
  }

  // Si erreur:
  else if (strcmp(response, "error") == 0) {
    // SPI_setColor(RED);
    //SPI_send("\r\n#");
  }

  // S'il y a une réponse:
  else if (strcmp(response, "rien") != 0) {
    // Renvoie la réponse:
    // SPI_setColor(GREEN);
    //SPI_send("\r\n> ");
    //SPI_send(response);
    //SPI_send("\r\n>");
  }

  // Sinon, si aucune commande reconnue:
  else {
    // Message d'erreur:
    // SPI_setColor(RED);
    //SPI_send("\r\n#");
  }

  // SPI_resetColor();
}
