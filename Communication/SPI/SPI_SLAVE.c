#ifdef SLAVE

#ifdef SLAVE
#include "../../Cartes/Slave/SPI_HANDLE_SLAVE.h"
#else
void SPI_receive_handle_message(char* message);
#endif

#include "../../Cartes/Ressources/CONFIG_8051.h"
#include "../../Cartes/Ressources/c8051F020.h"
#include "SPI_SLAVE.h"

/** Variables globales */
sbit SPI_MOSI = P0 ^ 4;
char SPI_receive_buffer[50] = "";
char SPI_receive_i;
bit SPI_receive_handle;

int SPI_i = 0; // temp

/**
 * Initialisation de la SPI
 * Registres modifiés: P0MDOUT
 */
void SPI_init() {
  // Config SPI en mode Slave:
  SPI0CFG = 0x07;

  // Compteur du charactère:
  SPI_receive_i = 0;

  // Active les broches pour la crossbar:
  XBR0 |= 1 << 1;

  // Initialisation pin:
  SPI_MOSI = 1;

  // Interruption SPI enable:
  EIE1 |= 1;

  // Active la SPI et mode slave:
  SPI0CN = 0x1;
}

/**
 * Fonction de mise à jour de la SPI
 */
void SPI_update() {
  if (SPI_receive_handle == 1) {
    SPI_receive_handle_message(SPI_receive_buffer);
    SPI_receive_handle = 0;
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
 * Fonction d'interruption de la SPI
 */
void SPI_interrupt() interrupt 6 {
  // Réception:
  if (SPIF == 1) {
    // Récupère le caractère reçu:
    char c = SPI0DAT;
	  
	SPI_i++; // temp

    // Vérification du caractère:
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') || c == ' ' || c == ':' || c == '-') {
      // Ajoute le caractère au buffer:
      SPI_receive_buffer[SPI_receive_i] = c;
      SPI_receive_i++;
    }

    // Si fin de la chaine:
    else if (c == '\r') {
      // Ajoute le caractère de fin:
      SPI_receive_buffer[SPI_receive_i] = '\0';
      SPI_receive_i = 0;

      // Interprète le message reçu (dans SPI_update):
      SPI_receive_handle = 1;
    }

    // RAZ du flag:
    SPIF = 0;
  }
}

#ifndef SLAVE
/**
 * Fonction déclenchée par défaut lorsqu'une ligne est reçu sur la SPI
 * @param {char*} buffer : ligne reçu par la SPI
 */
void SPI_receive_handle_message(char* buffer) {
  // SPI_send("\r\n");

  // Renvoie la ligne reçue:
  // SPI_send(buffer);
  // SPI_send("\r\n");

  // SPI_resetColor();
}
#endif

#endif