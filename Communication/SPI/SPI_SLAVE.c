#ifdef SLAVE

#include "SPI_SLAVE.h"
#include "../../Cartes/Ressources/CONFIG_8051.h"
#include "../../Cartes/Ressources/c8051F020.h"

/** Variables globales */
sbit LED = P1 ^ 6;
sbit P0_2 = P0 ^ 2;
sbit P0_3 = P0 ^ 3;
char SPI0_receive_buffer[100];
char SPI0_receive_i;
char SPI0_receive_handle;

/**
 * Initialisation de la SPI
 * Registres modifiés: P0MDOUT
 */
void SPI_init() {
  // Global interrupt enabled
  IE |= 0x80;

  P0MDOUT |= 1 << 1;  // Output : P0.1 et P0.4
  P0MDOUT &= ~(1 << 2 + 1 << 3);  // Input : P0.2 et P0.3
  P0_2 = 1;
  P0_3 = 1;

  // Config SPI:
  SPI0CFG = 0x07;

  // Compteur du charactère:
  SPI0_receive_i = 0;

  // Active les broches pour la crossbar:
  XBR0 |= 0x02;

  // Clock rate:
  SPI0CKR = 0xFF;

  // Interruption SPI enable:
  EIE1 |= 1;

  // Active la SPI et mode slave:
  SPI0CN = 1;
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
void SPI_send(char* string) {}

/**
 * Envoie un caractère par la SPI
 * @param {char} c : caractère à envoyer
 */
void SPI_sendChar(char c) {}

/**
 * Fonction déclenchée lorsqu'une ligne est reçue sur la SPI
 * @param {char*} buffer : ligne reçue par la SPI
 */
void SPI_receive_handle_buffer(char* buffer) {}

void SPI_interrupt() interrupt 6 {
  LED = 1;

  // Réception:
  if (SPIF == 1) {
    // Récupère le caractère reçu:
    char c = SPI0DAT;

    // Vérification du caractère:
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') || c == ' ' || c == ':') {
      // Ajoute le caractère au buffer:
      SPI0_receive_buffer[SPI0_receive_i] = c;
      SPI0_receive_i++;
    }

    // Si fin de la chaine:
    else if (c == '\r') {
      // Ajoute le caractère de fin:
      SPI0_receive_buffer[SPI0_receive_i] = '\0';
      SPI0_receive_i = 0;

      // Interprète le message reçu (dans UART0_update):
      SPI0_receive_handle = 1;
    }

    // RAZ du flag:
    SPIF = 0;
  }
}

#endif