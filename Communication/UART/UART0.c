#include "UART0.h"
#include "c8051F020.h"

/**
 * Variables de l'UART0:
 */

// Transmission:
char UART0_transmit_busy = 0;

// Réception:
char UART0_receive_buffer[50];
unsigned char UART0_receive_i = 0;
char UART0_receive_isCode = 1;
char UART0_receive_handle = 0;

// Couleur:
char UART0_color = 0;
char UART0_update_color = 0;

/**
 * Initialisation de l'UART0
 * Registres modifiés: SCON0 (SM00, SM01, REN0, RI0), IP (PS/PS0), IE (ES0)
 */
void UART0_init() {
  SM00 = 0;  // Choix du mode de l'UART
  SM10 = 1;  // Mode 1: 8bit UART variable baud rate

  // Active RX et TX de UART0 dans le Crossbar:
  XBR0 |= 1 << 2;

  // Configuration du Timer 2:
  TIMER2_config();

  // Activation des interruptions:
  PS = 1;   // Priorité élevée
  ES0 = 1;  // Enable
  RI0 = 0;  // RAZ Flag

  // Activation de la réception:
  REN0 = 1;

  UART0_setColor(1);
}

/**
 * Configuration du Timer 2 comme horloge de l'UART0
 * Registres modifiés: T2CON (TCLK0, RCLK0, TR2), RCAP2H, RCAP2L
 */
void TIMER2_config() {
  // Choix du timer 2 pour l'UART0:
  TCLK0 = 1;
  RCLK0 = 1;

  // Configuration du baud rate (19200):
  RCAP2H = 0xFF;
  RCAP2L = 0xDC;

  // Activation du Timer 2:
  TR2 = 1;
}

/**
 * Fonction de mise à jour de l'UART0 
 */
void UART0_update() {
  // Traitement de la ligne:
  if (UART0_receive_handle == 1) {
    UART0_receive_handle = 0;
    UART0_receive_handle_buffer(UART0_receive_buffer);
  }

  // Modification de la couleur du texte:
  if (UART0_update_color == 1) {
    UART0_update_color = 0;
    UART0_setColor(UART0_color);
  }
}

/**
 * Envoie un caractère par l'UART0
 * @param {char} c : caractère à envoyer
 */
void UART0_sendChar(char c) {
  // Attend la fin de l'envoie d'un caractère:
  while (UART0_transmit_busy == 1) ;
  UART0_transmit_busy = 1;

  // Charge et envoie le caractère:
  SBUF0 = c;
}

/**
 * Envoie une chaîne de caractères par l'UART0
 * @param {char*} string : chaîne de caractères à envoyer
 */
void UART0_send(char* string) {
  int i = 0;
  while (string[i] != '\0') {
    UART0_sendChar(string[i]);
    i++;
  }
}

/**
 * Définit la couleur des prochains caractères envoyés par l'UART0
 * @param {char} color : indice de la couleur 
 */
void UART0_setColor(char color) {
  UART0_sendChar(0x1b);
  if (color == 0) UART0_send("[0m");
  else if (color == 1) UART0_send("[32m");
  else if (color == 2) UART0_send("[31m");
  else if (color == 3) UART0_send("[33m");
}

/**
 * Fonction d'interruption de l'UART0
 * Registres modifiés: SCON0 (RI0, TI0), SBUF0
 */
void UART0_interrupt() interrupt 4 {
  // Transmission:
  if (TI0 == 1) {
    UART0_transmit_busy = 0;
    TI0 = 0;
  }

  // Réception:
  if (RI0 == 1) {
    // Récupère le caractère reçu:
    char c = SBUF0;

    // Vérification du caractère:
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') || c == ' ' || c == ':') {
      // Ajoute le caract�re au buffer:
      UART0_receive_buffer[UART0_receive_i] = c;
      UART0_receive_i++;

      // Gestion de la couleur:
      if (c == ' ') {
        if (UART0_receive_isCode == 1) {
          UART0_receive_isCode = 0;
          UART0_color = 1;
        } else
          UART0_color = 2;

        UART0_update_color = 1;
      } else if (c == ':') {
        UART0_color = 3;
        UART0_update_color = 1;
      }

      UART0_sendChar(c);

      // RAZ du flag:
      RI0 = 0;
    }

    // Si fin de la ligne:
    else if (c == '\r') {
      // Ajoute le caract�re de fin:
      UART0_receive_buffer[UART0_receive_i] = '\0';
      UART0_receive_i = 0;

      // RAZ du flag:
      RI0 = 0;

      // Interprète le message reçu (dans UART0_update):
      UART0_receive_handle = 1;
    }
  }
}

/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur l'UART0
 * @param {char*} buffer : ligne reçu par l'UART0
 */
void UART0_receive_handle_buffer(char* buffer) {
  UART0_send("\r\n");

  // Renvoie la ligne re�ue:
  UART0_send(buffer);

  UART0_send("\r\n");

  UART0_setColor(1);
  UART0_receive_isCode = 1;
}
