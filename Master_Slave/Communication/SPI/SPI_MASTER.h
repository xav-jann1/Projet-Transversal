#ifndef SPI_H
#define SPI_H

/**
 * Initialisation de la SPI
 * Registres modifiés:
 */
void SPI_init();

/**
 * Fonction de mise à jour de la SPI
 */
void SPI_update();

/**
 * Envoie un caractère par la SPI
 * @param {char} c : caractère à envoyer
 */
void SPI_sendChar(char c);

/**
 * Envoie une chaîne de caractères par la SPI
 * @param {char*} string : chaîne de caractères à envoyer
 */
void SPI_send(char* string);

/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur la SPI
 * @param {char*} buffer : ligne reçue par la SPI
 */
void SPI_receive_handle_buffer(char*);

#endif  // SPI_H