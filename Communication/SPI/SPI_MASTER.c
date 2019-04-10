#ifdef MASTER

#include "SPI_MASTER.h"
#include "c8051F020.h"

// Global variables
sbit SPI_MISO = P0 ^ 3;
sbit SPI_SEL_SLAVE = P1 ^ 4;

char SPI0_receive_handle;
char SPI0_receive_buffer[30];
int i;
char SPI_transmit_flag;

/**
 * Initialisation de la SPI
 * Registres modifiés:P0MD
 */
void SPI_init() {
  // Initialisation des flags:
  SPI_transmit_flag = 1;

  // Config SPI:
  SPI0CFG = 0x07;

  // Active les broches pour le Crossbar:
  XBR0 |= 0x02;

  // Initialisation pins:
  P1MDOUT |= 1 << 3;  // Push-pull: SEL_SLAVE
  SPI_SEL_SLAVE = 1;
  SPI_MISO = 1;

  // Clock rate:
  SPI0CKR = 0xFF;

  // Interruption SPI enable:
  EIE1 |= 0x01;

  // Active la SPI et mode master:
  SPI0CN = 0x03;
}

/**
 * Fonction de mise à jour de la SPI
 */
void SPI_update() {
  if (SPI0_receive_handle == 1) {
    SPI_receive_handle_buffer(SPI0_receive_buffer);
    SPI0_receive_handle = 0;
  }
}

/**
 * Envoie une chaîne de caractères par la SPI
 * @param {char*} string : chaîne de caractères à envoyer
 */
void SPI_send(char* string) {
  i = 0;
  while (string[i] != '\0') {
    SPI_sendChar(string[i]);
    i++;
  }
  SPI_sendChar('\0');
}

/**
 * Envoie un caractère par la SPI
 * @param {char} c : caractère à envoyer
 */
void SPI_sendChar(char c) {
  while (SPI_transmit_flag == 0)
    ;
  SPI_transmit_flag = 0;
  SPI_SEL_SLAVE = 0;
  SPI0DAT = c;
}

/**
 * Fonction déclenchée lorsqu'une ligne est reçue sur la SPI
 * @param {char*} buffer : ligne reçue par la SPI
 */
void SPI_receive_handle_buffer(char* buffer) {}

/**
 * Fonction déclenchée lors de l'interruption de la SPI
 */
void SPI_interrupt() interrupt 6 {
  if (SPIF == 1) {
    SPI_transmit_flag = 1;
    SPIF = 0;
    SPI_SEL_SLAVE = 1;
  }
}
#endif