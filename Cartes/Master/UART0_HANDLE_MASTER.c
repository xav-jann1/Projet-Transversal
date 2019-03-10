#include "UART0_HANDLE_MASTER.h"

#include <string.h>
#include "../../Communication/Parseur_messages/PARSEUR.h"
#include "../../Communication/UART/UART0.h"

// Fonctions pour exécuter une commande:
#include "./HANDLES/HANDLES_MASTER.h"

extern char* (*HANDLES[8])(Commande*);

/**
 * Fonction déclenchée lorsqu'une ligne est reçue sur l'UART0
 * @param {char*} message : message reçu
 */
void UART0_receive_handle_message(char* message) {
  int data i = 0;

  // Réponse:
  char data response[8] = "rien";

  // Extrait la commande du message:
  Commande data commande;
  commande = PARSEUR_message(message);

  // Cherche et exécute la commande:
  for (i = 0; i < 8; i++) {
    // Exécute la commande, si elle est reconnue dans l'ensemble des fonctions:
    strcpy(response, (*HANDLES[i])(&commande));

    // S'il y a une réponse:
    if (strcmp(response, "rien") != 0) break;
  }

  // S'il y a une réponse d'erreur:
  if (strcmp(response, "error") == 0) {
    // Message d'erreur:
    UART0_setColor(RED);
    UART0_send("\r\n# ");
    UART0_send(response);
    UART0_send("\r\n#");
    UART0_resetColor();
  }

  // S'il y a une réponse:
  else if (strcmp(response, "rien") != 0) {
    // Renvoie la réponse:
    UART0_setColor(GREEN);
    UART0_send("\r\n> ");
    UART0_send(response);
    UART0_send("\r\n>");
    UART0_resetColor();
  }

  // Sinon:
  else {
    // Message d'erreur:
    UART0_setColor(RED);
    UART0_send("\r\n#");
    UART0_resetColor();
  }
}
