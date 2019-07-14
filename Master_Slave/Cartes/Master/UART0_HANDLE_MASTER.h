#ifndef UART0_HANDLE_MASTER
#define UART0_HANDLE_MASTER

/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur l'UART0
 * @param {char*} buffer : ligne reçu par l'UART0
 */
void UART0_receive_handle_message(char* buffer);

#endif