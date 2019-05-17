#ifndef UART0_HANDLE_SLAVE
#define UART0_HANDLE_SLAVE

/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur l'UART0
 * @param {char*} buffer : ligne reçu par l'UART0
 */
void UART0_receive_handle_message(char* buffer);

#endif