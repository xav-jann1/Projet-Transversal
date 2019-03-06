#include "UART1.h"
#include "c8051F020.h"

/**
 * Variables de l'UART1:
 */

// Transmission:
char UART1_transmit_busy = 0;

// Réception:
char UART1_receive_buffer[50];
unsigned char UART1_receive_i = 0;
char UART1_receive_isCode = 1;
char UART1_receive_handle = 0;

/**
 * Initialisation de l'UART1
 * Registres modifiés: SCON1 (SM01, SM11, REN1, RI1), EIP2 (EP1), IEE2 (EP1/PS1)
 */
void UART1_init() {
  // Choix du mode de l'UART:
  SCON1 = 1 << 6;  // Mode 1: 8bit UART variable baud rate

  // Active RX et TX de UART1 dans le Crossbar:
  XBR2 |= 1 << 2;

  // Sortie en Push-Pull:
  P0MDOUT |= 1;

  // Configuration du Timer 1:
  TIMER1_config();

  // Activation des interruptions:
  EIP2 |= 1 << 6;      // Priorité élevée : EP1/PS1 = 1
  EIE2 |= 1 << 6;      // Enable : ES1 = 1
  SCON1 &= ~(1 << 1);  // RAZ Flag : RT1 = 0

  // Activation de la réception:
  SCON1 |= 1 << 4;  // REN1 = 1;
}

/**
 * Configuration du Timer 4 comme horloge de l'UART1
 * Registres modifiés: T4CON (TCLK1, RCLK1, TR4), RCAP4H, RCAP4L
 */
void TIMER4_config() {
  // Choix du timer 4 pour l'UART1:
  T4CON = 3 << 4;  // TCLK1 = 1, RCLK1 = 1

  // Configuration du baud rate (19200):
  RCAP4H = 0xFF;
  RCAP4L = 0xDC;

  // Activation du Timer 4:
  T4CON |= 1 << 2;  // TR4 = 1
}

/**
 * Configuration du Timer 1 comme horloge de l'UART1
 * Registres modifiés: TMOD (T1M1,T1M0), T4CON (TCLK1, RCLK1), CKCON (T1M), TCON (TR1), TH1
 */
void TIMER1_config() {
  // Timer 1 en mode 2 : auto-reload
  TMOD &= ~(1 << 4);  // T1M0 = 0,
  TMOD |= (1 << 5);   // T1M1 = 1;

  // Choix du timer 1 pour l'UART1:
  T4CON &= ~(1 << 4 + 1 << 5);  // TCLK1 = 0, RCLK1 = 0

  // Configuration du baud rate (19200):
  CKCON &= ~(1 << 4); // Timer 1 utilise SYSCLK
  TH1 = 253;

  // Activation du Timer 1:
  TR1 = 1;
}

/**
 * Fonction de mise à jour de l'UART1
 */
void UART1_update() {
  // Traitement de la ligne:
  if (UART1_receive_handle == 1) {
    UART1_receive_handle = 0;
    UART1_receive_handle_buffer(UART1_receive_buffer);
  }
}

/**
 * Envoie un caractère par l'UART1
 * @param {char} c : caractère à envoyer
 */
void UART1_sendChar(char c) {
  // Attend la fin de l'envoie d'un caractère:
  while (UART1_transmit_busy == 1)
    ;
  UART1_transmit_busy = 1;

  // Charge et envoie le caractère:
  SBUF1 = c;
}

/**
 * Envoie une chaîne de caractères par l'UART1
 * @param {char*} string : chaîne de caractères à envoyer
 */
void UART1_send(char* string) {
  int i = 0;
  while (string[i] != '\0') {
    UART1_sendChar(string[i]);
    i++;
  }
}

/**
 * Fonction d'interruption de l'UART1
 * Registres modifiés: SCON01.0:RI1, SCON01.1:TI1, SBUF0
 */
void UART1_interrupt() /*interrupt 20*/ {
  // Transmission:
  if ((SCON1 >> 1) & 1 == 1) {
    UART1_transmit_busy = 0;
    SCON1 &= ~(1 << 1);  // RAZ TI1
  }

  // Réception:
  if (SCON1 & 1 == 1) {
    // Récupère le caractère reçu:
    char c = SBUF0;

    // Vérification du caractère:
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') || c == ' ' || c == ':') {
      // Ajoute le caractère au buffer:
      UART1_receive_buffer[UART1_receive_i] = c;
      UART1_receive_i++;

    }

    // Si fin de la ligne:
    else if (c == '\r') {
      // Ajoute le caractère de fin:
      UART1_receive_buffer[UART1_receive_i] = '\0';
      UART1_receive_i = 0;

      // Interprète le message reçu (dans UART1_update):
      UART1_receive_handle = 1;
    }

    // RAZ du flag:
    SCON1 &= ~(1 << 0);
  }
}

/**
 * Fonction déclenchée lorsqu'une ligne est reçu sur l'UART1
 * @param {char*} buffer : ligne reçu par l'UART1
 */
void UART1_receive_handle_buffer(char* buffer) {
  UART1_send("\r\n");

  // Renvoie la ligne reçue:
  UART1_send(buffer);

  UART1_send("\r\n");

  UART1_receive_isCode = 1;
}
