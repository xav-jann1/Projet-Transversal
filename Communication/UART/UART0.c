#ifdef MASTER
#include "../../Cartes/Master/UART0_HANDLE_MASTER.h"
#else
void UART0_receive_handle_message(char* buffer);
#endif

#include "../../Cartes/Ressources/TIME_8051.h"
#include "UART0.h"
#include "c8051F020.h"

/**
 * Variables de l'UART0:
 */

// Transmission:
bit UART0_transmit_busy = 0;

// Réception:
char UART0_receive_buffer[50];
unsigned char UART0_receive_i = 0;
bit UART0_receive_handle = 0;
char UART0_receive_char = '\0';

// Couleur:
bit UART0_update_color = 0;
enum color UART0_color = WHITE;

/**
 * Initialisation de l'UART0
 * Registres modifiés: SCON0 (SM00, SM01, REN0, RI0), IP (PS/PS0), IE (ES0)
 */
void UART0_init() {
  SM00 = 0;  // Choix du mode de l'UART
  SM10 = 1;  // Mode 1: 8bit UART variable baud rate

  // Active RX et TX de UART0 dans le Crossbar:
  XBR0 |= 1 << 2;

  // Sortie en Push-Pull:
  P0MDOUT |= 1;  // crossbar-pin

  // Configuration du Timer 1:
  TIMER1_config();

  // Activation des interruptions:
  PS = 1;   // Priorité élevée
  ES0 = 1;  // Enable
  RI0 = 0;  // RAZ Flag

  // Activation de la réception:
  REN0 = 1;

  UART0_resetColor();
  UART0_setColor(GREEN);
}

/**
 * Configuration du Timer 1 comme horloge de l'UART0
 * Registres modifiés: TMOD (T1M1,T1M0), T2CON (TCLK0, RCLK0),
 *                     CKCON (T1M), TCON (TR1), TH1
 */
void TIMER1_config() {
  // Timer 1 en mode 2 : auto-reload
  TMOD &= ~(1 << 4);  // T1M0 = 0;
  TMOD |= (1 << 5);   // T1M1 = 1;

  // Choix du timer 1 pour l'UART0:
  T2CON &= ~(1 << 4 + 1 << 5);  // TCLK1 = 0, RCLK1 = 0

  // Configuration du baud rate (19200):
  CKCON &= ~(1 << 4);  // Timer 1 utilise SYSCLK
  TH1 = 253;

  // Activation du Timer 1:
  TR1 = 1;
}

/**
 * Fonction de mise à jour de l'UART0
 */
void UART0_update() {
  // Renvoie d'un caractère reçu:
  if (UART0_receive_char) {
    UART0_sendChar(UART0_receive_char);
    UART0_receive_char = '\0';
  }

  // Traitement de la ligne:
  if (UART0_receive_handle == 1) {
    UART0_receive_handle = 0;
    UART0_receive_handle_message(UART0_receive_buffer);
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
  while (UART0_transmit_busy == 1)
    ;
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
 * Envoie 3 chaînes de caractères à la suite par l'UART0
 * @param {char*} string{1-3} : chaîne de caractères à envoyer
 */
void UART0_send3(char* string1, char* string2, char* string3) {
  UART0_send(string1);
  UART0_send(string2);
  UART0_send(string3);
}

/**
 * Définit la couleur des prochains caractères envoyés par l'UART0
 * @param {char} color : indice de la couleur
 */
void UART0_setColor(enum color c) {
  UART0_sendChar(0x1b);
  if (c == WHITE)       UART0_send("[0m");
  else if (c == RED)    UART0_send("[31m");
  else if (c == GREEN)  UART0_send("[32m");
  else if (c == YELLOW) UART0_send("[33m");
  else if (c == BLUE)   UART0_send("[34m");
  else if (c == PINK)   UART0_send("[35m");
  else if (c == CYAN)   UART0_send("[36m");
  else if (c == GREY)   UART0_send("[37m");
}

/**
 * Initialise la couleur des caractères de l'UART0
 * Correspond à la couleur du code de la commande
 */
void UART0_resetColor() {
  UART0_setColor(WHITE);  // Code
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
        (c >= '0' && c <= '9') || c == ' ' || c == ':' || c == '-') {
      // Ajoute le caractère au buffer:
      UART0_receive_buffer[UART0_receive_i] = c;
      UART0_receive_i++;

      // Gestion de la couleur:
      if (c == ' ') {
        UART0_color = CYAN;  // Param
        UART0_update_color = 1;
      }

      // Valeur de paramètre:
      else if (c == ':') {
        UART0_color = YELLOW;  // Valeur
        UART0_update_color = 1;
      }

      // UART0_sendChar(c);
      UART0_receive_char = c;
    }

    // Si fin de la ligne ou trop de caractères:
    if (c == '\r' || UART0_receive_i > 30) {
      // Ajoute le caractère de fin:
      UART0_receive_buffer[UART0_receive_i] = '\0';
      UART0_receive_i = 0;

      // Interprète le message reçu (dans UART0_update):
      UART0_receive_handle = 1;
    }

    // RAZ du flag:
    RI0 = 0;
  }
}

#ifndef MASTER
/**
 * Fonction déclenchée par défaut lorsqu'une ligne est reçu sur l'UART0
 * @param {char*} buffer : ligne reçu par l'UART0
 */
void UART0_receive_handle_message(char* buffer) {
  UART0_send("\r\n");

  // Renvoie la ligne reçue:
  UART0_send(buffer);
  UART0_send("\r\n");

  UART0_resetColor();
}
#endif

/**
 * ARCHIVES
 */

/**
 * Configuration du Timer 2 comme horloge de l'UART0
 * Registres modifiés: T2CON (TCLK0, RCLK0, TR2), RCAP2H, RCAP2L
 */
/*void TIMER2_config() {
  // Choix du timer 2 pour l'UART0:
  TCLK0 = 1;
  RCLK0 = 1;

  // Configuration du baud rate (19200):
  RCAP2H = 0xFF;
  RCAP2L = 0xDC;

  // Activation du Timer 2:
  TR2 = 1;
}*/
