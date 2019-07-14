#ifndef UART1_HANDLE_MASTER
#define UART1_HANDLE_MASTER

/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur l'UART1
 * @param {char*} buffer : ligne reçu par l'UART0
 */
void UART1_receive_handle_message(char* buffer);

#endif