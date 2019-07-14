#include "UART0_HANDLE_SLAVE.h"

#include <string.h>
#include "../../Communication/Parseur_messages/PARSEUR.h"
#include "../../Communication/UART/UART0.h"

// Fonctions pour exécuter une commande:
//#include "./HANDLES/HANDLES_SLAVE.h"
extern char* (*HANDLES[5])(Commande*);

/**
 * Fonction déclenchée lorsqu'une ligne est reçue sur l'UART0
 * @param {char*} message : message reçu
 */
void UART0_receive_handle_message(char* message) {
  char idata i = 0;

  // Réponse:
  char idata response[30] = "rien";

  // Extrait la commande du message:
  Commande idata commande;
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
    //UART0_setColor(GREEN);
    UART0_send("\r\n>");
  }
  
  // Si erreur:
  else if (strcmp(response, "error") == 0) {
    //UART0_setColor(RED);
    UART0_send("\r\n#");
  }

  // S'il y a une réponse:
  else if (strcmp(response, "rien") != 0) {
    // Renvoie la réponse:
    //UART0_setColor(GREEN);
    UART0_send("\r\n> ");
    UART0_send(response);
    UART0_send("\r\n>");
  }

  // Sinon, si aucune commande reconnue:
  else {
    // Message d'erreur:
    //UART0_setColor(RED);
    UART0_send("\r\n#");
  }
  
  //UART0_resetColor();
}