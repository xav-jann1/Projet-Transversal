#ifndef SPI_HANDLE_SLAVE
#define SPI_HANDLE_SLAVE

/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur la SPI
 * @param {char*} buffer : ligne reçu par la SPI
 */
void SPI_receive_handle_message(char* buffer);

#endif