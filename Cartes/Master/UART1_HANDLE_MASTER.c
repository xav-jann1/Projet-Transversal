#include "UART1_HANDLE_MASTER.h"

#include <string.h>
#include "../../Communication/UART/UART1.h"
#include "../../Communication/UART/UART0.h"

/**
 * Fonction déclenchée lorsqu'une ligne est reçue sur l'UART1
 * @param {char*} message : message reçu
 */
void UART1_receive_handle_message(char* message) {
  UART0_send("\r\n");

  // Renvoie la ligne reçue:
  UART0_send(message);
  UART0_send("\r\n");
  
  // Réponses de 'pids':
  if (message[0] == '0' || message[0] == '1') {
    UART0_send("pid state: ");
    UART0_send("\r\n");
    UART1_setResponse(message);
  }
}
